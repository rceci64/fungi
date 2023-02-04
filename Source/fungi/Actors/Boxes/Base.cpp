// Fill out your copyright notice in the Description page of Project Settings.


#include "Base.h"

// Sets default values
ABase::ABase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ABase::BeginPlay()
{
	Super::BeginPlay();
	
	ChildArray = TArray<ABase*>();
	ChildArray.SetNumZeroed(NUM_CARDINAL_DIRECTIONS);	// Eventually change to 8 if we want to do diagonals
	RootArray = TArray<ARoot*>();
	RootArray.SetNumZeroed(NUM_CARDINAL_DIRECTIONS);	
}

// Called every frame
void ABase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABase::IsFungable()
{
	return bAllowsFunging && !bIsFunged;
}


bool ABase::IsMycelable()
{
	return bAllowsFunging && !bIsMycelled;
}

