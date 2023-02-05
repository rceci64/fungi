// Fill out your copyright notice in the Description page of Project Settings.


#include "FungiCharacter.h"

#include "fungi/Actors/Boxes/Base.h"
#include "fungi/Actors/Cameras/FungiCamera.h"
#include "fungi/Actors/Managers/LevelManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFungiCharacter::AFungiCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->GravityScale = 0;
	LastHover = nullptr;
}

// Called when the game starts or when spawned
void AFungiCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Unpause game in case it is paused
	UWorld* WorldInstance = GetWorld();
	UGameplayStatics::SetGamePaused(WorldInstance, false);
}

void AFungiCharacter::SetCameraDragging(bool Dragging)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AFungiCamera::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		AFungiCamera* FungiCamera = Cast<AFungiCamera>(FoundActors[0]);
		FungiCamera->bIsDragging = Dragging;
	}
	
}

// Called every frame
void AFungiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, HitResult);

	ABase* HitBaseBox = Cast<ABase>(HitResult.GetActor());
	
	if (HitBaseBox != LastHover)
	{
		if (LastHover)
		{
			ALevelManager* Manager = Cast<ALevelManager>(LastHover->GetAttachParentActor());
			if (Manager)
			{
				UpdateHighlights(Manager, LastHover->GridX, LastHover->GridY, LastRange, false, LastCorrect);
			}
		}
		
		if (HitBaseBox)
		{
			ALevelManager* Manager = Cast<ALevelManager>(HitBaseBox->GetAttachParentActor());
			if (Manager)
			{
				LastCorrect = HitBaseBox->bIsFunged && Manager->WouldFungeAny(HitBaseBox->GridX, HitBaseBox->GridY); 
				LastHover = HitBaseBox;
				LastRange = Manager->CurrentRange;
				UpdateHighlights(Manager, HitBaseBox->GridX, HitBaseBox->GridY, HitBaseBox->bIsFunged ? Manager->CurrentRange : 0, true, LastCorrect);
			}
		} else
		{
			LastHover = nullptr;
		}
	}
	
}

// Called to bind functionality to input
void AFungiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFungiCharacter::Interact);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AFungiCharacter::Pause).bExecuteWhenPaused = false;
	PlayerInputComponent->BindAction("DragCamera",  IE_Pressed, this, &AFungiCharacter::DragCameraStart);
	PlayerInputComponent->BindAction("DragCamera",  IE_Released, this, &AFungiCharacter::DragCameraEnd);
}

void AFungiCharacter::DragCameraStart()
{
	SetCameraDragging(true);
}
void AFungiCharacter::DragCameraEnd()
{
	SetCameraDragging(false);
}

void AFungiCharacter::Interact()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, HitResult);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, HitResult.ToString());
	ABase* HitBaseBox = Cast<ABase>(HitResult.GetActor());
	if (HitBaseBox)
	{
		ALevelManager* Manager = Cast<ALevelManager>(HitBaseBox->GetAttachParentActor());
		
		if (Manager)
		{
			if (Manager->ExpandFunge(HitBaseBox->GridX, HitBaseBox->GridY))
			{
				UpdateHighlights(Manager, HitBaseBox->GridX, HitBaseBox->GridY, Manager->CurrentRange, true, false);
			}
		}
	}
}

/*
 * Used from the widget continue button
 */
void AFungiCharacter::ContinueGame()
{
	Pause();
}


void AFungiCharacter::Pause()
{
	UWorld* WorldInstance = GetWorld();
	UGameplayStatics::SetGamePaused(WorldInstance ,!UGameplayStatics::IsGamePaused(WorldInstance));
	TogglePauseMenu(UGameplayStatics::IsGamePaused(WorldInstance));
}


void AFungiCharacter::UpdateHighlights(ALevelManager* Manager, int FromX, int FromY, int Range, bool Highlighted, bool Correct)
{
	ABase* Block = Manager->GetBlockAt(FromX, FromY);
	if (Block && Block->bAllowsFunging)
	{
		Block->DoHighlight(Highlighted, Correct);
	}
	
	for (int i = 1; i <= Range; ++i)
	{
		Block = Manager->GetBlockAt(FromX, FromY - i);
		if (Block && Block->bAllowsFunging) {
			Block->DoHighlight(Highlighted, Correct);
		}
		Block = Manager->GetBlockAt(FromX + i, FromY);
		if (Block && Block->bAllowsFunging) {
			Block->DoHighlight(Highlighted, Correct);
		}
		Block = Manager->GetBlockAt(FromX, FromY + i);
		if (Block && Block->bAllowsFunging) {
			Block->DoHighlight(Highlighted, Correct);
		}
		Block = Manager->GetBlockAt(FromX - i, FromY);
		if (Block && Block->bAllowsFunging) {
			Block->DoHighlight(Highlighted, Correct);
		}
	}
}
