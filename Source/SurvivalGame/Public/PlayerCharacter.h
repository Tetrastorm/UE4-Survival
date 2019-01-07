// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API APlayerCharacter : public ANPC
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	void GetPlayerHUD();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
		FVector FPSCameraLocation = FVector(10.0, 20.0, 0.0);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
		FVector TPSCameraLocation = FVector(-100, 75.0, 90);

protected:

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ForwardBackMovement(float fValue);
	void RightLeftMovement(float fValue);

	void CameraPitch(float scale);
	void CameraRaw(float scale);

	void ChangeCamera();

	void StartWeaponSight();
	void StopWeaponSight();

	void ViewRaycast();

	void Manager();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* PlayerCamera;
	UPROPERTY()
		FTimerHandle ManagerTimer;

private:

	bool bIsFPS;
};
