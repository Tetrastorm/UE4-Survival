// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_Controller.h"
#include "Player_State.h"


APlayer_Controller::APlayer_Controller()
{

}

void APlayer_Controller::BeginPlay()
{
	Super::BeginPlay();

	if (HUD != nullptr)
	{
		UUserWidget* UMGHUD = CreateWidget<UUserWidget>(GetWorld(), HUD);
		UMGHUD->AddToViewport();
	}
}

void APlayer_Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Inventory", IE_Pressed, this, &APlayer_Controller::InventoryMenu);
	InputComponent->BindAction("Menu", IE_Pressed, this, &APlayer_Controller::InGameMenu);
}

void APlayer_Controller::ChangeWidget(TSubclassOf<UUserWidget> newWidget)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;

		SetPause(false);
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	
	if (newWidget != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), newWidget);
		CurrentWidget->AddToViewport();

		SetPause(true);
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}

void APlayer_Controller::InventoryMenu()
{
	ChangeWidget(InventoryWidget);
}

void APlayer_Controller::InGameMenu()
{
	ChangeWidget(InGameMenuWidget);
}
