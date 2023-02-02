// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"

#define TILE_SIZE 100.0
#define GRASS 'G'
#define ROCK 'R'

enum EDirection { up, right, down, left };

/*
 *  0
 * 3X1
 *  2 
 */

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

	Map = TArray<ABase*>();
	Map.SetNumUninitialized(Width * Height);
	FString Aux = MapString.Replace(*FString("\n"), *FString(""));

	for (int Y = 0; Y < Height; Y++)
	{
		for (int X = 0; X < Width; X++)
		{
			char Cell = Aux.GetCharArray()[Pos(X, Y)];
			FVector Location = FVector(TILE_SIZE * X, TILE_SIZE * Y, 0);
			ABase* Block = nullptr;

			switch (Cell)
			{
			case GRASS:
				Block = World->SpawnActor<ABase>(GrassBox, Location, FRotator());
				break;
			case ROCK:
				Block = World->SpawnActor<ABase>(RockBox, Location, FRotator());
				break;
			}
			if (Block)
			{
				Block->GridX = X;
				Block->GridY = Y;
				Block->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false), TEXT("Cell"));

				if (X == StartX && Y == StartY)
				{
					Block->Funge();
				}
			}
			Map[Pos(X, Y)] = Block;
		}
	}
}

// Called when the game starts or when spawned
void ALevelManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelManager::ExpandFunge(int X, int Y)
{
	ABase* Block = Map[Pos(X, Y)];

	if (Block && Block->bIsFunged)
	{
		UE_LOG(LogTemp, Display, TEXT("Clicked: %d, %d"), X, Y);

		ProtectFunge(Block, X + 1, Y, right, left);
		ProtectFunge(Block, X - 1, Y, left, right);
		ProtectFunge(Block, X, Y + 1, down, up);
		ProtectFunge(Block, X, Y - 1, up, down);
	}
}

bool ALevelManager::ProtectFunge(ABase* BlockFrom, int X, int Y, int OutDir, int InDir)
{
	if (X >= 0 && X < Width && Y >= 0 && Y < Height)
	{
		ABase* BlockTo = Map[Pos(X, Y)];

		if (BlockTo && BlockTo->bAllowsFunging && !BlockTo->bIsFunged)
		{
			Funge(BlockFrom, BlockTo, OutDir, InDir);
			return true;
		}
	}
	return false;
}

void ALevelManager::Funge(ABase* BlockFrom, ABase* BlockTo, int OutDir, int InDir)
{
	BlockTo->Funge();
	
	BlockFrom->AdjacentArray[OutDir] = BlockTo;
	BlockTo->AdjacentArray[InDir] = BlockFrom;
	
	BlockFrom->AdjacentCount++;
	BlockTo->AdjacentCount++;
	AddSpline(BlockFrom, BlockTo, OutDir, InDir);
}

void ALevelManager::AddSpline(ABase* Base, ABase* BlockTo, int OutDir, int InDir)
{
}


int ALevelManager::Pos(const int X, const int Y) const
{
	return X + Y * Width;
}
