// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEditorManager.h"

#define TILE_SIZE 100.0

// Sets default values
ALevelEditorManager::ALevelEditorManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALevelEditorManager::BeginPlay()
{
	AActor::BeginPlay();

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	Map = TArray<ABase*>();
	Map.SetNumUninitialized(Width * Height);

	for (int Y = 0; Y < Height; Y++)
	{
		for (int X = 0; X < Width; X++)
		{
			FVector Location = FVector(TILE_SIZE * X, TILE_SIZE * Y, 0);
			FTransform Transform = {
				FRotator(),
				Location,
				{1.0f, 1.0f, 1.0f},
			};
			ABase* Block = World->SpawnActorDeferred<ABase>(GrassBox, Transform);
			Block->BoxType = grass;
			Block->GridX = X;
			Block->GridY = Y;
			MapString.AppendChar(grass);

			Block->FinishSpawning(Transform);
			Block->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false), TEXT("Cell"));
			Map[Pos(X, Y)] = Block;
		}
		MapString.AppendChar('\n');
	}
}

// Called every frame
void ALevelEditorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelEditorManager::OnClick(int X, int Y)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}
	
	if (ValidPos(X, Y))
	{
		ABase* Block = Map[Pos(X, Y)];
		if (Block)
		{
			World->DestroyActor(Block);

			EBox Type = NextBoxType(Block->BoxType);
			SpawnBlockAt(World, X, Y, Type);
		}
	}
}


void ALevelEditorManager::UpdateMapString()
{
	MapString.Empty();
	for (int Y = 0; Y < Height; Y++)
	{
		for (int X = 0; X < Width; X++)
		{
			ABase* Block = Map[Pos(X, Y)];
			if (Block) {
				MapString.AppendChar(Block->BoxType);
			}
			else
			{
				MapString.AppendChar('E');
			}
		}
		MapString.AppendChar('\n');
	}
}

EBox ALevelEditorManager::NextBoxType(EBox Current)
{
	switch (Current)
	{
	case grass: return rock;
	case rock: return tree;
	case tree: return mushroom;
	case mushroom: return grass;
	case empty:
	default:
		return grass;
	}
}
