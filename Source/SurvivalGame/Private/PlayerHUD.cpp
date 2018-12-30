// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHUD.h"
#include "Weapon.h"
#include "PlayerCharacter.h"


APlayerHUD::APlayerHUD()
{

}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();


}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	if (GetOwningPawn() != nullptr)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPawn());

		FVector2D Screen(Canvas->ClipX, Canvas->ClipY);

		if (Player != nullptr)
		{
			if (Crosshair != nullptr)
			{
				DrawTextureSimple(Crosshair, Screen.X*0.5, Screen.Y*0.5, 1.0f);
			}

			if (Player->WeaponEquiped != nullptr)
			{
				DrawText(FString("Munition : " + FString::SanitizeFloat(Player->WeaponEquiped->GetMunition()) + " / " + FString::SanitizeFloat(Player->WeaponEquiped->GetMaxMunition())), FColor::White, (Screen.X * 0.80), (Screen.Y * 0.9), NULL, 2.0, false);

				if (Player->WeaponEquiped->HUDPicture != nullptr)
				{
					DrawTextureSimple(Player->WeaponEquiped->HUDPicture, Screen.X * 0.8, Screen.Y * 0.8, 1.0f);
				}
			}

			DrawText(FString("Heath : " + FString::SanitizeFloat(Player->GetHealth())), FColor::White, (Screen.X * 0.05), (Screen.Y * 0.85), NULL, 2.0, false);
			DrawText(FString("Stamina : " + FString::SanitizeFloat(Player->GetStamina())), FColor::White, (Screen.X * 0.05), (Screen.Y * 0.9), NULL, 2.0, false);
		}
	}
}