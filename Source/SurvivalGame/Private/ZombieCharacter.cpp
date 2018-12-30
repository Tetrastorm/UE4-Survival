// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieCharacter.h"
#include "ZombieAIController.h"
#include "Perception/PawnSensingComponent.h"

AZombieCharacter::AZombieCharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("View Component"));
	PawnSensingComp->SetPeripheralVisionAngle(90.f);
}

void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AZombieCharacter::OnSeePlayer);
	}
}

void AZombieCharacter::OnSeePlayer(APawn* Pawn)
{
	AZombieAIController* AIController = Cast<AZombieAIController>(GetController());

	if (AIController)
	{
	AIController->SetSeenTarget(Pawn);
	GLog->Log("Zombie detect a pawn");
	}
}