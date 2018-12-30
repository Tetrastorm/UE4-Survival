// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UserWidget.h"
#include "Player_Controller.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API APlayer_Controller : public APlayerController
{
	GENERATED_BODY()

public:

	APlayer_Controller();

	UFUNCTION(BlueprintCallable)
		void ChangeWidget(TSubclassOf<UUserWidget> newWidget);

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> InventoryWidget;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> InGameMenuWidget;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> HUD;

	UPROPERTY(VisibleAnywhere, Category = "UI")
		UUserWidget* CurrentWidget;

	void InventoryMenu();
	void InGameMenu();
};
