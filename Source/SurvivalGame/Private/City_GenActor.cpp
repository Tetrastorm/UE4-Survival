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
	unsigned int** matrix = nullptr;

	GLog->Log("City_GenActor: BeginPlay");
	matrix = MapMatrixGen();
	MatrixGen(matrix);
	this->Destroy();
}

// Called every frame
void ACity_GenActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

unsigned int** ACity_GenActor::InitMatrix(unsigned int size)
{
	unsigned int** matrix = new unsigned int* [size]();

	for (unsigned int i = 0; i < size; i++)
		matrix[i] = new unsigned int[size];
	return (matrix);
}

static unsigned int HowManyModule(unsigned int ** const matrix, unsigned int const x, unsigned int const  y, unsigned int const size)
{
	unsigned int numberofmodule = 0;

	if (x > 0 && x < size && y > 0 && y < size && matrix[y][x] == 1)
	{
		numberofmodule++;
		numberofmodule += HowManyModule(matrix, x - 1, y, size);
		numberofmodule += HowManyModule(matrix, x, y - 1, size);
		numberofmodule += HowManyModule(matrix, x - 1, y - 1, size);
	}
	return (numberofmodule);
}

unsigned int** ACity_GenActor::MapMatrixGen()
{
	unsigned int size = maxSize / caseLenght;
	unsigned int** matrix = InitMatrix(size);

	for (unsigned int y = 2; y < size; y++)
	{
		for (unsigned int x = 2; x < size; x++)
		{
			if (sin(y * x) < 0.5f && ((matrix[y - 1][x] != 0 && matrix[y - 2][x] != 0) || (matrix[y][x - 1] != 0 && matrix[y][x - 2] != 0)))
			{
				matrix[y][x] = 1;
			}
			else
			{
				matrix[y][x] = 0;
			}
		}
	}
	return (matrix);
}

void ACity_GenActor::MatrixGen(unsigned int** matrix)
{
	unsigned int size = maxSize / caseLenght;

	for (unsigned int y = 0; y < size; y++)
	{
		for (unsigned int x = 0; x < size; x++)
		{
			if (matrix[y][x] != 0)
			{
				FVector Location = FVector(x * (caseLenght / 2) - ((size / 2) * caseLenght / 2), y * (caseLenght / 2) - ((size / 2) * caseLenght / 2), 0.0f);
				SpawnAsset(Location, FRotator::ZeroRotator, 0);
			}
		}
	}
}

void ACity_GenActor::SpawnAsset(FVector Location, FRotator Rotation, unsigned int Index)
{
	UWorld* const World = GetWorld();
	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;
	if (World != nullptr && Index < (unsigned int)BuildingModules.Num() && BuildingModules[Index] != nullptr)
	{
		World->SpawnActor<AActor>(BuildingModules[Index], Location, Rotation, SpawnParams);
	}
}