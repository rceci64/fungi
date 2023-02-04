// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FungiCharacter.h"
#include "GameFramework/Character.h"
#include "MastermindCharacter.generated.h"

UCLASS()
class FUNGI_API AMastermindCharacter : public AFungiCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMastermindCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Interact();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
