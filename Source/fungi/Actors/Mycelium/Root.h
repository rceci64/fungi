// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "fungi/Helpers/Direction.h"
#include "GameFramework/Actor.h"
#include "Root.generated.h"

UCLASS()
class FUNGI_API ARoot : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoot();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	USplineComponent* Spline;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SectionLength = 100.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EDirection> Direction = up;
	
};
