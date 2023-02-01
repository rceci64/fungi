// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "fungi/fungiGameModeBase.h"
#include "MyfungiGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FUNGI_API AMyfungiGameModeBase : public AfungiGameModeBase
{
	GENERATED_BODY()

	AMyfungiGameModeBase();

public:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
};
