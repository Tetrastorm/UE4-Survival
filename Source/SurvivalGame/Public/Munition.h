// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Munition.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMunitionType : uint8
{
	E_NONE = 0,
	E_RIFFLE,
	E_HANDGUN,
	E_SHOTGUN
};

UCLASS()
class SURVIVALGAME_API AMunition : public AItem
{
	GENERATED_BODY()

public:

	AMunition();

	UFUNCTION(BlueprintCallable, Category = "Getter")
		int GetMunition();
	
	UFUNCTION(BlueprintCallable, Category = "Setter")
		void SetMunition(int NewValue);

protected: 

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Item Property")
		int MaxMunition;

private:

	int MunitionStocked;

};
