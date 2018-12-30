// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapon.h"
#include "NPC.h"
#include "NPCAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UNPCAnimInstance();

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Statement")
		EType WeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Statement")
		EPosition NPCPosition;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Statement")
		bool bIsFalling;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Statement")
		bool bIsReloading;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Statement")
		bool bIsDeath;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Statement")
		bool bCanJump;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Statement")
		bool bIsIronsight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Statement")
		float MovementSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Statement")
		float MovementDirection;

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void UpdateStatement();
};