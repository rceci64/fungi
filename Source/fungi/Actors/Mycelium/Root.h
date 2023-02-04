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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MyceliumRender();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	USplineComponent* Spline;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SectionLength = 100.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SubRootFrequency = 100.0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EDirection> Direction = up;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Growth")
	int Depth;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Growth")
	bool bIsLeaf;
	
};
