// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "fungi/Actors/Boxes/Base.h"

#include "LevelManager.generated.h"

UCLASS()
class FUNGI_API ALevelManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Funge(int X, int Y);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Properties
public:	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Width;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Height;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int StartX;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int StartY;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int EndX;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int EndY;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine="true"))
	FString MapString;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="ActorSpawning")
	TSubclassOf<ABase> Grass;

	private:
		FString Map;
};
