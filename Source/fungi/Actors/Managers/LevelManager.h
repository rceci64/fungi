// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "fungi/Actors/Boxes/Base.h"
#include "fungi/Actors/Mycelium/Root.h"
#include "fungi/Helpers/Direction.h"
#include "fungi/Helpers/Boxes.h"

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

	UFUNCTION(BlueprintCallable)
	bool ExpandFunge(int X, int Y);
	
	UFUNCTION(BlueprintCallable)
	ABase* GetBlockAt(int X, int Y);

	UFUNCTION(BlueprintImplementableEvent)
	void StepDone();

	UFUNCTION(BlueprintImplementableEvent)
	void TreeDone();

	UFUNCTION(BlueprintCallable)
	bool WouldFungeAny(int X, int Y);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	int Pos(int X, int Y) const;
	bool ValidPos(int X, int Y) const;
	bool ProtectFunge(ABase* BlockFrom, int ToX, int ToY, EDirection OutDir, EDirection InDir, int RangeLeft);
	void Funge(ABase* BlockFrom, ABase* BlockTo, int OutDir, int InDir);
	void UpdateParentHeights(ABase* Block, int NewHeight);
	void MyceliumExpand(ABase* Block);
	void MyceliumExpand(UWorld* World, ABase* Block, EDirection Direction);
	void MyceliumInit(UWorld* World, int X, int Y);

	// Properties
public:	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map")
	int Width;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map")
	int Height;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine="true"), Category="Map")
	FString MapString;

	UPROPERTY(EditAnywhere,Category="Actors")
	TSubclassOf<ABase> GrassBox;
	UPROPERTY(EditAnywhere,Category="Actors")
	TSubclassOf<ABase> RockBox ;
	UPROPERTY(EditAnywhere,Category="Actors")
	TSubclassOf<ABase> TreeBox ;
	UPROPERTY(EditAnywhere,Category="Actors")
	TSubclassOf<ABase> MushroomBox ;
	UPROPERTY(EditAnywhere,Category="Actors")
	TSubclassOf<ARoot> MyceliumRoot ;

	UPROPERTY(BlueprintReadWrite)
	TArray<ABase*> Map;

	UPROPERTY(VisibleAnywhere, Category = "Mechanics")
	int CurrentRange;
	UPROPERTY(VisibleAnywhere, Category="Score")
	int FungableCells;
	UPROPERTY(VisibleAnywhere, Category="Score")
	int FungedCells;
	UPROPERTY(VisibleAnywhere, Category = "Score")
	int CurrentSteps;

	int RangeMustIncreaseBy;
};
