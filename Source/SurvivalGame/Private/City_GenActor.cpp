// Fill out your copyright notice in the Description page of Project Settings.


#include "City_GenActor.h"


// Sets default values
ACity_GenActor::ACity_GenActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACity_GenActor::BeginPlay()
{
	Super::BeginPlay();

	SpawnAsset(FVector::ZeroVector, FRotator::ZeroRotator, 0);
}

// Called every frame
void ACity_GenActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACity_GenActor::SpawnAsset(FVector Location, FRotator Rotation, unsigned int Index)
{
	UWorld * const World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	if (World != nullptr)
		World->SpawnActor<AStaticMeshActor>(BuildingModules, Location, Rotation, SpawnParams);
}