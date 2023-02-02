// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"

#define TILE_SIZE 100.0

void ALevelManager::Funge(int X, int Y)
{
	UE_LOG(LogTemp, Display, TEXT("Clicked: %d, %d"), X, Y);
}

// Sets default values
ALevelManager::ALevelManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	Map = MapString.Replace(*FString("\n"), *FString(""));
	
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			int Pos = x + y * Width;
			char Cell = Map.GetCharArray()[Pos];
			FVector Location = FVector(TILE_SIZE * x, TILE_SIZE * y, 0);
			ABase* Block = nullptr;
			
			switch (Cell)
			{
			case 'G':
				Block = World->SpawnActor<ABase>(Grass, Location, FRotator());
				break;
			}
			if (Block)
			{
				Block->GridX = x;
				Block->GridY = y;
				Block->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false), TEXT("Cell"));
			}
		}
	}
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

