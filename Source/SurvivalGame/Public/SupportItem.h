// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "SupportItem.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASupportItem : public AItem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Item")
		void Use(AActor* CharacterActor);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Item Property")
		bool bIsOverTime;
	UPROPERTY(EditDefaultsOnly, Category = "Item Property")
		bool bCureInfection;

	UPROPERTY(EditDefaultsOnly, Category = "Item Property")
		float Heal;
	UPROPERTY(EditDefaultsOnly, Category = "Item Property")
		float Stamina;
	UPROPERTY(EditDefaultsOnly, Category = "Item Property")
		float Time;

private:

};
