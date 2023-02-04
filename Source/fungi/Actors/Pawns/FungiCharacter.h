// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "fungi/Actors/Managers/LevelManager.h"
#include "FungiCharacter.generated.h"

UCLASS()
class FUNGI_API AFungiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFungiCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Interact();
	void Pause();
	
	// { -- UI RELATED FUNCTIONS --
	UFUNCTION(BlueprintImplementableEvent)
	void TogglePauseMenu(bool bPaused);

	UFUNCTION(BlueprintCallable) //Used to make UI button work
	void ContinueGame();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowWinScreen(int StepsToShow);
	
	// } -- UI

private:
	void UpdateHighlights(ALevelManager* Manager, int FromX, int FromY, int Range, bool Highlighted, bool Correct);

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Properties
public:
	UPROPERTY(VisibleAnywhere)
	ABase* LastHover;
	UPROPERTY(VisibleAnywhere)
	int LastRange;
	UPROPERTY(VisibleAnywhere)
	bool LastCorrect;
};
