// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"

#include "fungi/Actors/Pawns/FungiCharacter.h"
#include "Kismet/GameplayStatics.h"

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
	Aux = Aux.Replace(*FString("\r"), *FString(""));
	FungableCells = 0;
	FungedCells = 1;	// Start already funged
	CurrentSteps = 0;

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
				if (Block->bAllowsFunging)
				{
					FungableCells++;
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
		CurrentSteps++;
		
		ProtectFunge(Block, X + 1, Y, right, left);
		ProtectFunge(Block, X - 1, Y, left, right);
		ProtectFunge(Block, X, Y + 1, down, up);
		ProtectFunge(Block, X, Y - 1, up, down);
		MyceliumExpand(Block);
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

	FungedCells++;
	if (FungedCells >= FungableCells)
	{
		AFungiCharacter* FungiCharacter = Cast<AFungiCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		FungiCharacter->ShowWinScreen(CurrentSteps);
	}

	BlockTo->Depth = BlockFrom->Depth + 1;
	BlockTo->Height = 0;
	BlockTo->Parent = BlockFrom;

	// MARK: HEIGHTS DISABLED FOR PERFORMANCE REASONS
	// UpdateParentHeights(BlockTo->Parent, BlockTo->Height + 1);
}

void ALevelManager::UpdateParentHeights(ABase* Block, int NewHeight)
{
	if (NewHeight > Block->Height)
	{
		Block->Height = NewHeight;
		if (Block->Parent)
		{
			UpdateParentHeights(Block->Parent, NewHeight + 1);
		}
	}
}

void ALevelManager::MyceliumInit(UWorld* World)
{
	FVector Location = FVector(TILE_SIZE * StartX, TILE_SIZE * StartY, SPLINE_HEIGHT);

	ABase* Start = Map[Pos(StartX, StartY)];
	Start->bIsMycelled = true;

	for (int i = 0; i < NUM_CARDINAL_DIRECTIONS; ++i)
	{
		ARoot* Root = World->SpawnActor<ARoot>(MyceliumRoot, Location, FRotator());
		Root->Direction = static_cast<EDirection>(i);
		Start->RootArray[i] = Root;
		Root->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false), TEXT("Root"));
	}
}

void ALevelManager::MyceliumExpand(ABase* Block)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	// Special system to manage turns
	int Possible = 0;
	int MaxLength = 0;
	ARoot* LastPossibleRoot = nullptr;
	ARoot* LongestRoot = nullptr;
	for (ARoot* Root : Block->RootArray)
	{
		if (Root) {
			ABase* Current = Block->ChildArray[Root->Direction];
			if (Current && Current->bIsFunged && !Current->bIsMycelled) {
				Possible++;
				LastPossibleRoot = Root;
			}
			if (Root->Spline->GetNumberOfSplinePoints() >= MaxLength)
			{
				MaxLength = Root->Spline->GetNumberOfSplinePoints();
				LongestRoot = Root;
			}
		}
	}
	
	if (Possible == 1)
	{
		Block->RootArray[LastPossibleRoot->Direction] = LongestRoot;
		LongestRoot->Direction = LastPossibleRoot->Direction;

		MyceliumExpand(World, Block, LastPossibleRoot->Direction);
	} else
	{
		for (ARoot* Root : Block->RootArray)
		{
			if (Root)
			{
				MyceliumExpand(World, Block, Root->Direction);
			}
		}
	}
}

void ALevelManager::MyceliumExpand(UWorld* World, ABase* Block, EDirection Direction)
{
	ARoot* Root = Block->RootArray[Direction];
	ABase* Current = Block->ChildArray[Direction];

	if (Current && Current->bIsFunged && !Current->bIsMycelled)
	{
		Current->bIsMycelled = true;
		const FVector Location = FVector(TILE_SIZE * Current->GridX, TILE_SIZE * Current->GridY, SPLINE_HEIGHT);
		Root->Spline->AddSplinePoint(Location, ESplineCoordinateSpace::World, true);
		Root->MyceliumRender();
		
		for (int i = 0; i < NUM_CARDINAL_DIRECTIONS; ++i)
		{
			const int Diff = abs(Direction - i);
			if (Diff != 0 && Diff != 2)
			{
				ARoot* Branch = World->SpawnActor<ARoot>(MyceliumRoot, Location, FRotator());
				Branch->Direction = static_cast<EDirection>(i);
				Branch->AttachToActor(Root, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false),
				                      TEXT("Root"));
				Current->RootArray[i] = Branch;
			}
			if (Diff == 0)
			{
				Current->RootArray[i] = Root;
			}
		}
	}
	
}

int ALevelManager::Pos(const int X, const int Y) const
{
	return X + Y * Width;
}
