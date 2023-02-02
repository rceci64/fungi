// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base.generated.h"

UCLASS()
class FUNGI_API ABase : public AActor
{
	GENERATED_BODY()

	// Functions
	public:	
	// Sets default values for this actor's properties
	ABase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Funge();


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UnFunge();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Properties
	public:	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map")
	int GridX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map")
	int GridY;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Funging")
	bool bAllowsFunging = true;

	UPROPERTY(BlueprintReadWrite, Category = "Funging")
	bool bIsFunged = false;

	/*
	
	UPROPERTY(BlueprintReadWrite, Category = "Funging")
	TArray<ABase*> AdjacentArray;
	
	UPROPERTY(BlueprintReadWrite, Category = "Funging")
	int AdjacentCount;
	*/
};
