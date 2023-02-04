// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "fungi/Helpers/Direction.h"
#include "fungi/Actors/Mycelium/Root.h"
#include "fungi/Helpers/Boxes.h"
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
	
	UFUNCTION(BlueprintImplementableEvent)
	void DoHighlight(bool Highlighted);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Properties
	public:	

	
	UPROPERTY(VisibleAnywhere, Category = "Map")
	TEnumAsByte<EBox> BoxType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map")
	int GridX;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map")
	int GridY;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mechanics")
	bool bAllowsFunging = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mechanics")
	bool bIncreasesRange = false;
	

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Funging")
	bool bIsFunged = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Funging")
	bool bIsMycelled = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Funging")
	int Depth = 0;
	UPROPERTY(VisibleAnywhere, Category = "Funging")
	int Height = 0;
	
	UPROPERTY(VisibleAnywhere, Category = "Funging")
	TArray<ABase*> ChildArray;
	UPROPERTY(VisibleAnywhere, Category = "Funging")
	ABase* Parent;
	UPROPERTY(VisibleAnywhere, Category = "Funging")
	TArray<ARoot*> RootArray;
};
