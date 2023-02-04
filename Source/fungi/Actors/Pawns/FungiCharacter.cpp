// Fill out your copyright notice in the Description page of Project Settings.


#include "FungiCharacter.h"

#include "fungi/Actors/Boxes/Base.h"
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
}

// Called every frame
void AFungiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, HitResult);

	ABase* HitBaseBox = Cast<ABase>(HitResult.GetActor());

	ALevelManager* Manager = Cast<ALevelManager>(HitBaseBox->GetAttachParentActor());
		
	if (Manager)
	{
		if (HitBaseBox != LastHover)
		{
			if (LastHover)
			{
				UpdateHighlights(Manager, LastHover->GridX, LastHover->GridY, LastRange, false);
			}
		}
	
		if (HitBaseBox)
		{
			LastHover = HitBaseBox;
			LastRange = Manager->CurrentRange;
			UpdateHighlights(Manager, HitBaseBox->GridX, HitBaseBox->GridY, Manager->CurrentRange, true);
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
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AFungiCharacter::Pause).bExecuteWhenPaused = true;
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
			Manager->ExpandFunge(HitBaseBox->GridX, HitBaseBox->GridY);
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


void AFungiCharacter::UpdateHighlights(ALevelManager* Manager, int FromX, int FromY, int Range, bool Highlighted)
{

	DoHighlight(Manager->GetBlockAt(FromX, FromY), Highlighted);
	
	for (int i = 1; i < Range; ++i)
	{
		DoHighlight(Manager->GetBlockAt(FromX, FromY - 1), Highlighted);
		DoHighlight(Manager->GetBlockAt(FromX + 1, FromY), Highlighted);
		DoHighlight(Manager->GetBlockAt(FromX, FromY + 1), Highlighted);
		DoHighlight(Manager->GetBlockAt(FromX - 1, FromY), Highlighted);
	}
}