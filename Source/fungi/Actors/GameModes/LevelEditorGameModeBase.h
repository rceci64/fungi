// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "fungi/fungiGameModeBase.h"
#include "LevelEditorGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FUNGI_API ALevelEditorGameModeBase : public AfungiGameModeBase
{
	GENERATED_BODY()

	ALevelEditorGameModeBase();

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
};
