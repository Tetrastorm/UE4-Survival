// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
private:

	APlayerHUD();

public:

	UPROPERTY(EditAnywhere, Category = "UI")
		UTexture2D* Weapon;
	UPROPERTY(EditAnywhere, Category = "UI")
		UTexture2D* Crosshair;

protected:

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;
	
};
