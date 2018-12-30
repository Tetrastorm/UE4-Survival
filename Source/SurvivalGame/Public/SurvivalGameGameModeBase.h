// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "SurvivalGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASurvivalGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASurvivalGameGameModeBase();

protected:

	virtual void BeginPlay() override;
	
private:

};
