// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "City_GenActor.generated.h"

UCLASS()
class SURVIVALGAME_API ACity_GenActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACity_GenActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnAsset(FVector Location, FRotator Rotation, unsigned int Index);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category="Procedural")
		unsigned int seed;
	UPROPERTY(EditDefaultsOnly, Category="Asset Bank")
		TArray <TSubclassOf<class AActor>> BuildingModules;
};
