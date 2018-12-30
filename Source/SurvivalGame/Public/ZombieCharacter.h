// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include "ZombieCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API AZombieCharacter : public ANPC
{
	GENERATED_BODY()
	
public:

	AZombieCharacter();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UPawnSensingComponent* PawnSensingComp;
	UPROPERTY(EditAnywhere, Category = "AI")
		class UBehaviorTree* BehaviorTree;

protected:

private:

	UFUNCTION()
		void OnSeePlayer(APawn* Pawn);	
};
