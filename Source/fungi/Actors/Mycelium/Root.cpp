// Fill out your copyright notice in the Description page of Project Settings.


#include "Root.h"


// Sets default values
ARoot::ARoot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARoot::BeginPlay()
{
	Super::BeginPlay();
	
	BranchArray = TArray<ARoot*>();
}

// Called every frame
void ARoot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

