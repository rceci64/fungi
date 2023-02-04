// Fill out your copyright notice in the Description page of Project Settings.


#include "MyfungiGameModeBase.h"

#include "Camera/CameraActor.h"
#include "fungi/Actors/Pawns/FungiCharacter.h"
#include "Kismet/GameplayStatics.h"

AMyfungiGameModeBase::AMyfungiGameModeBase()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;
    PrimaryActorTick.bAllowTickOnDedicatedServer = false;

    DefaultPawnClass = AFungiCharacter::StaticClass();
}

void AMyfungiGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // Get camera and set it as default view
    /*AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
	ACameraActor* CameraActor = Cast<ACameraActor>(FoundActor);*/

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    //PlayerController->SetViewTargetWithBlend(CameraActor);
    PlayerController->bShowMouseCursor = true;
}

void AMyfungiGameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

}
