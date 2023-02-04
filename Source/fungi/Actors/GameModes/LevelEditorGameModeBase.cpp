// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelEditorGameModeBase.h"

#include "Camera/CameraActor.h"
#include "fungi/Actors/Pawns/FungiCharacter.h"
#include "Kismet/GameplayStatics.h"

ALevelEditorGameModeBase::ALevelEditorGameModeBase()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
    PrimaryActorTick.bAllowTickOnDedicatedServer = false;

    DefaultPawnClass = AFungiCharacter::StaticClass();
}

void ALevelEditorGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // Get camera and set it as default view
    /*AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
	ACameraActor* CameraActor = Cast<ACameraActor>(FoundActor);*/

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    //PlayerController->SetViewTargetWithBlend(CameraActor);
    PlayerController->bShowMouseCursor = true;
}

void ALevelEditorGameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

}
