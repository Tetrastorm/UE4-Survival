// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"
#include "ZombieCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AZombieAIController::AZombieAIController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AZombieAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	AZombieCharacter* ZombieCharacter = Cast<AZombieCharacter>(Pawn);

	if (ZombieCharacter)
	{
		if (ZombieCharacter->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(ZombieCharacter->BehaviorTree->BlackboardAsset));
			BehaviorComp->StartTree(*ZombieCharacter->BehaviorTree);

			GLog->Log("ZombieAIController possess by a ZombieCharacter");
		}
	}
}

void AZombieAIController::SetSeenTarget(APawn* Pawn)
{
	
	GLog->Log("Player repered");

	if (BlackboardComp)
	{
		ANPC* Character = Cast<ANPC>(Pawn);

		if (Character)
		{
			BlackboardComp->SetValueAsObject(FName("Target"), Character);
			BlackboardComp->SetValueAsInt(FName("State"), 2);
		}
	}
}