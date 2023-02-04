// Fill out your copyright notice in the Description page of Project Settings.


#include "MastermindCharacter.h"

#include "fungi/Actors/Managers/LevelEditorManager.h"


// Sets default values
AMastermindCharacter::AMastermindCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMastermindCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMastermindCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMastermindCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMastermindCharacter::Interact()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false,
	                                                   HitResult);

	ABase* HitBaseBox = Cast<ABase>(HitResult.GetActor());
	if (HitBaseBox)
	{
		UE_LOG(LogViewport, Log, TEXT("%d, %d"), HitBaseBox->GridX, HitBaseBox->GridY);

		ALevelEditorManager* Manager = Cast<ALevelEditorManager>(HitBaseBox->GetAttachParentActor());

		if (Manager)
		{
			Manager->OnClick(HitBaseBox->GridX, HitBaseBox->GridY);
		}
	}
}
