// Fill out your copyright notice in the Description page of Project Settings.


#include "FungiCharacter.h"

#include "fungi/Actors/Boxes/Base.h"
#include "fungi/Managers/LevelManager.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AFungiCharacter::AFungiCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->GravityScale = 0;
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

}

// Called to bind functionality to input
void AFungiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFungiCharacter::Interact);
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
		HitBaseBox->Funge();

		ALevelManager* Manager = Cast<ALevelManager>(HitBaseBox->GetParentActor());
		Manager->Funge(HitBaseBox->GridX, HitBaseBox->GridY);
	}
}

