// Fill out your copyright notice in the Description page of Project Settings.


#include "FungiCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AFungiCharacter::AFungiCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->GravityScale = 0;

	APlayerController* PC = Cast<APlayerController>(GetController());

	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}

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

}

