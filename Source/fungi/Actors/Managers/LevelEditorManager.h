// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelManager.h"
#include "GameFramework/Actor.h"
#include "fungi/Actors/Boxes/Base.h"
#include "fungi/Helpers/Boxes.h"
#include "LevelEditorManager.generated.h"

UCLASS()
class FUNGI_API ALevelEditorManager : public ALevelManager
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelEditorManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnClick(int X, int Y);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void UpdateMapString();
	EBox NextBoxType(EBox Current);
};
