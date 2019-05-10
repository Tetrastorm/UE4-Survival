// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "ZombieAIController.generated.h"

UCLASS()
class SURVIVALGAME_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	AZombieAIController();

	virtual void OnPossess(APawn* Pawn) override;

	void SetSeenTarget(APawn* Pawn);

private:

	UBehaviorTreeComponent * BehaviorComp;
	UBlackboardComponent* BlackboardComp;	
};
