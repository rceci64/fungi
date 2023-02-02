// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"

#define TILE_SIZE 100.0
#define SPLINE_HEIGHT 60.0
#define GRASS 'G'
#define ROCK 'R'

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

	MyceliumInit(World);
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
		bool AnyFunged = false;
		//UE_LOG(LogTemp, Display, TEXT("Clicked: %d, %d"), X, Y);

		AnyFunged = ProtectFunge(Block, X + 1, Y, right, left) || AnyFunged;
		AnyFunged = ProtectFunge(Block, X - 1, Y, left, right) || AnyFunged;
		AnyFunged = ProtectFunge(Block, X, Y + 1, down, up) || AnyFunged;
		AnyFunged = ProtectFunge(Block, X, Y - 1, up, down) || AnyFunged;

		if (AnyFunged)
		{
			MyceliumExpand();
		}
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

	BlockFrom->ChildArray[OutDir] = BlockTo;
	BlockTo->ChildArray[InDir] = BlockFrom;

	BlockTo->Depth = BlockFrom->Depth + 1;
	BlockTo->Height = 0;
	BlockTo->Parent = BlockFrom;

	UpdateParentHeights(BlockTo->Parent, BlockTo->Height + 1);
}

void ALevelManager::UpdateParentHeights(ABase* Base, int NewHeight)
{
	if (NewHeight > Base->Height)
	{
		Base->Height = NewHeight;
		if (Base->Parent)
		{
			UpdateParentHeights(Base->Parent, NewHeight + 1);
		}
	}
}

void ALevelManager::MyceliumInit(UWorld* World)
{
	FVector Location = FVector(TILE_SIZE * StartX, TILE_SIZE * StartY, SPLINE_HEIGHT);

	Map[Pos(StartX, StartY)]->bIsMycelled = true;

	MyceliumArray = TArray<ARoot*>();
	MyceliumArray.SetNumZeroed(NUM_CARDINAL_DIRECTIONS);

	for (int i = 0; i < NUM_CARDINAL_DIRECTIONS; ++i)
	{
		MyceliumArray[i] = World->SpawnActor<ARoot>(MyceliumRoot, Location, FRotator());
		MyceliumArray[i]->Direction = static_cast<EDirection>(i);
		MyceliumArray[i]->StartX = StartX;
		MyceliumArray[i]->StartY = StartY;
		MyceliumArray[i]->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false), TEXT("Root"));
	}
}

void ALevelManager::MyceliumExpand()
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	MyceliumExpand(World, MyceliumArray[up]);
	MyceliumExpand(World, MyceliumArray[right]);
	MyceliumExpand(World, MyceliumArray[down]);
	MyceliumExpand(World, MyceliumArray[left]);
}

void ALevelManager::MyceliumExpand(UWorld* World, ARoot* Root)
{
	ABase* Current = Map[Pos(Root->StartX, Root->StartY)];

	while (Current)
	{
		if (Current->bIsFunged && !Current->bIsMycelled)
		{
			Current->bIsMycelled = true;
			FVector Location = FVector(TILE_SIZE * Current->GridX, TILE_SIZE * Current->GridY, SPLINE_HEIGHT);
			Root->Spline->AddSplinePoint(Location, ESplineCoordinateSpace::World, true);
			
			for (int i = 0; i < NUM_CARDINAL_DIRECTIONS; ++i)
			{
				const int Diff = abs(Root->Direction - i);
				if (Diff != 0 && Diff != 2)
				{
					ARoot* Branch = World->SpawnActor<ARoot>(MyceliumRoot, Location, FRotator());
					Branch->StartX = Current->GridX;
					Branch->StartY = Current->GridY;
					Branch->Direction = static_cast<EDirection>(i);
					Branch->AttachToActor(Root, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false),
					                      TEXT("Root"));
					Root->BranchArray.Add(Branch);
				}
			}
		}
		Current = Current->ChildArray[Root->Direction];
		for (ARoot* Branch : Root->BranchArray) {
			MyceliumExpand(World, Branch);
		}
	}
}

int ALevelManager::Pos(const int X, const int Y) const
{
	return X + Y * Width;
}
