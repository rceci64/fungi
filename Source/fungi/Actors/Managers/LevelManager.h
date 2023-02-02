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

	UFUNCTION(BlueprintCallable)
	void ExpandFunge(int X, int Y);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	int Pos(int X, int Y) const;
	bool ProtectFunge(ABase* BlockFrom, int X, int Y, int OutDir, int InDir);
	void Funge(ABase* BlockFrom, ABase* BlockTo, int OutDir, int InDir);
	void AddSpline(ABase* Base, ABase* BlockTo, int OutDir, int InDir);
	
	// Properties
public:	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map")
	int Width;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map")
	int Height;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map")
	int StartX;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Map")
	int StartY;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(MultiLine="true"), Category="Map")
	FString MapString;

	UPROPERTY(EditAnywhere,Category="Boxes")
	TSubclassOf<ABase> GrassBox;
	UPROPERTY(EditAnywhere,Category="Boxes")
	TSubclassOf<ABase> RockBox ;

	UPROPERTY(BlueprintReadWrite)
	TArray<ABase*> Map;
	
};
