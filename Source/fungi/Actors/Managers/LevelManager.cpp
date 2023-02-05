// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"

#include "fungi/Actors/Pawns/FungiCharacter.h"
#include "Kismet/GameplayStatics.h"

#define TILE_SIZE 100.0
#define SPLINE_HEIGHT 60.0

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
	FungedCells = 0; // Start already funged
	CurrentSteps = 1;
	CurrentRange = 1;
	RangeMustIncreaseBy = 0;

	for (int Y = 0; Y < Height; Y++)
	{
		for (int X = 0; X < Width; X++)
		{
			char Cell = Aux.GetCharArray()[Pos(X, Y)];
			FVector Location = FVector(TILE_SIZE * X, TILE_SIZE * Y, 0);
			FTransform Transform = {
				FRotator(),
				Location,
				{1.0f, 1.0f, 1.0f},
			};
			ABase* Block = nullptr;

			switch (Cell)
			{
			case grass:
				Block = World->SpawnActorDeferred<ABase>(GrassBox, Transform);
				break;
			case rock:
				Block = World->SpawnActorDeferred<ABase>(RockBox, Transform);
				break;
			case tree:
				Block = World->SpawnActorDeferred<ABase>(TreeBox, Transform);
				break;
			case mushroom:
				Block = World->SpawnActorDeferred<ABase>(MushroomBox, Transform);
				break;
			default: break;
			}
			if (Block)
			{
				Block->GridX = X;
				Block->GridY = Y;
				Block->FinishSpawning(Transform);
				if (Cell == mushroom){
					MyceliumInit(World, Block);
					Block->Funge();
					FungedCells++;
				}
				Block->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false), TEXT("Cell"));
				if (Block->bAllowsFunging)
				{
					FungableCells++;
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


bool ALevelManager::WouldFungeAny(int X, int Y)
{
	ABase* BlockTo;
	if (ValidPos(X + 1, Y))
	{
		BlockTo = Map[Pos(X + 1, Y)];
		if (BlockTo && BlockTo->IsFungable())
		{
			return true;
		}
	}
	if (ValidPos(X - 1, Y))
	{
		BlockTo = Map[Pos(X - 1, Y)];
		if (BlockTo && BlockTo->IsFungable())
		{
			return true;
		}
	}
	if (ValidPos(X, Y - 1))
	{
		BlockTo = Map[Pos(X, Y - 1)];
		if (BlockTo && BlockTo->IsFungable())
		{
			return true;
		}
	}
	if (ValidPos(X, Y + 1))
	{
		BlockTo = Map[Pos(X, Y + 1)];
		if (BlockTo && BlockTo->IsFungable())
		{
			return true;
		}
	}

	return false;
}

bool ALevelManager::ExpandFunge(int X, int Y)
{
	ABase* Block = Map[Pos(X, Y)];
	bool AnyFunged = false;

	if (Block && Block->bIsFunged)
	{
		AnyFunged = ProtectFunge(Block, X, Y - 1, up, down, CurrentRange) || AnyFunged;
		AnyFunged = ProtectFunge(Block, X + 1, Y, right, left, CurrentRange) || AnyFunged;
		AnyFunged = ProtectFunge(Block, X, Y + 1, down, up, CurrentRange) || AnyFunged;
		AnyFunged = ProtectFunge(Block, X - 1, Y, left, right, CurrentRange) || AnyFunged;
		MyceliumExpand(Block);

		if (AnyFunged)
		{
			StepDone();
			CurrentSteps++;
		}
	}

	if (RangeMustIncreaseBy > 0)
	{
		CurrentRange += RangeMustIncreaseBy;
		RangeMustIncreaseBy = 0;
	}
	return AnyFunged;
}

ABase* ALevelManager::GetBlockAt(int X, int Y)
{
	if (ValidPos(X, Y))
	{
		return Map[Pos(X, Y)];
	}
	return nullptr;
}

bool ALevelManager::ProtectFunge(ABase* BlockFrom, int X, int Y, EDirection OutDir, EDirection InDir, int RangeLeft)
{
	if (ValidPos(X, Y))
	{
		ABase* BlockTo = Map[Pos(X, Y)];

		if (BlockTo && BlockTo->IsFungable())
		{
			Funge(BlockFrom, BlockTo, OutDir, InDir);

			if (RangeLeft > 1)
			{
				switch (OutDir)
				{
				case up:
					ProtectFunge(BlockTo, X, Y - 1, OutDir, InDir, RangeLeft - 1);
					break;
				case right:
					ProtectFunge(BlockTo, X + 1, Y, OutDir, InDir, RangeLeft - 1);
					break;
				case down:
					ProtectFunge(BlockTo, X, Y + 1, OutDir, InDir, RangeLeft - 1);
					break;
				case left:
					ProtectFunge(BlockTo, X - 1, Y, OutDir, InDir, RangeLeft - 1);
					break;
				default: ;
				}
			}
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

	// MARK: HEIGHT UPDATE DISABLED AS ITS UNNECESSARY + HAS COST O(HEIGHT)
	// UpdateParentHeights(BlockTo->Parent, BlockTo->Height + 1);

	if (BlockTo->bIncreasesRange)
	{
		TreeDone();
		RangeMustIncreaseBy++;
	}

	FungedCells++;
	if (FungedCells >= FungableCells)
	{
		AFungiCharacter* FungiCharacter = Cast<AFungiCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		UWorld* WorldInstance = GetWorld();
		UGameplayStatics::SetGamePaused(WorldInstance, true);
		FungiCharacter->ShowWinScreen(CurrentSteps);
	}
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

void ALevelManager::MyceliumInit(UWorld* World, ABase* Block)
{
	FVector Location = FVector(TILE_SIZE * Block->GridX, TILE_SIZE * Block->GridY, SPLINE_HEIGHT);

	Block->bIsMycelled = true;

	for (int i = 0; i < NUM_CARDINAL_DIRECTIONS; ++i)
	{
		ARoot* Root = World->SpawnActor<ARoot>(MyceliumRoot, Location, FRotator());
		Root->Depth = 0;
		Root->bIsLeaf = true;
		Root->Direction = static_cast<EDirection>(i);
		Block->RootArray[i] = Root;
		Root->AttachToActor(Block, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false), TEXT("Root"));
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
		if (Root)
		{
			ABase* Current = Block->ChildArray[Root->Direction];
			if (Current && Current->IsMycelable())
			{
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
	if (Possible == 1 && Block->Depth == Block->RootArray[LongestRoot->Direction]->Depth)
	{
		Block->RootArray[LastPossibleRoot->Direction] = LongestRoot;
		LongestRoot->Direction = LastPossibleRoot->Direction;

		MyceliumExpand(World, Block, LastPossibleRoot->Direction);
	}
	else
	{
		if (Possible > 0)
		{
			for (ARoot* Root : Block->RootArray)
			{
				if (Root)
				{
					MyceliumExpand(World, Block, Root->Direction);
				}
			}
			if (Block->Parent)
			{
				for (int i = 0; i < NUM_CARDINAL_DIRECTIONS; ++i)
				{
					if (Block->Parent->ChildArray[i] == Block && !Block->ChildArray[i])
					{
						Block->Parent->RootArray[i]->bIsLeaf = false;
						Block->Parent->RootArray[i]->MyceliumRender();
					}
				}
			}
		}
	}
}

void ALevelManager::MyceliumExpand(UWorld* World, ABase* Block, EDirection Direction)
{
	for (int j = 1; j <= CurrentRange; ++j)
	{
		ARoot* Root = Block->RootArray[Direction];
		ABase* Current = Block->ChildArray[Direction];

		if (Current && Current->IsMycelable())
		{
			Current->bIsMycelled = true;
			const FVector Location = FVector(TILE_SIZE * Current->GridX, TILE_SIZE * Current->GridY, SPLINE_HEIGHT);
			Root->Spline->AddSplinePoint(Location, ESplineCoordinateSpace::World, true);
			Root->Depth++;
			Root->MyceliumRender();
			
			for (int i = 0; i < NUM_CARDINAL_DIRECTIONS; ++i)
			{
				const int Diff = abs(Direction - i);
				if (Diff != 0 && Diff != 2)
				{
					ARoot* Branch = World->SpawnActor<ARoot>(MyceliumRoot, Location, FRotator());
					Branch->Depth = Root->Depth;
					Branch->bIsLeaf = true;
					Branch->Direction = static_cast<EDirection>(i);
					Branch->AttachToActor(Current, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false), TEXT("Root"));
					Current->RootArray[i] = Branch;
				}
				if (Diff == 0)
				{
					Current->RootArray[i] = Root;
				}
			}
			Block = Current;
		} else 
		{
			break;
		}
	}
}

int ALevelManager::Pos(const int X, const int Y) const
{
	return X + Y * Width;
}

bool ALevelManager::ValidPos(const int X, const int Y) const
{
	return X >= 0 && X < Width && Y >= 0 && Y < Height;
}
