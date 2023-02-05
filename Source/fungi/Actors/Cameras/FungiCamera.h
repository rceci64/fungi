// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FungiCamera.generated.h"

UCLASS()
class FUNGI_API AFungiCamera : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFungiCamera();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// Fields
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsDragging;
};
