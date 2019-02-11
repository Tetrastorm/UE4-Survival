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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
		FVector FPSCameraLocation = FVector(10.0f, 20.0f, 0.0f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
		FVector TPSCameraLocation = FVector(-100.0f, 0.0f, 90.0f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
		float DefaultRadius  = 250.0f;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ForwardBackMovement(float fValue);
	void RightLeftMovement(float fValue);

	void CameraPitch(float scale);
	void CameraYaw(float scale);

	void ChangeCamera();

	void StartWeaponSight();
	void StopWeaponSight();

	void ViewRaycast();

	void Manager();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class USpringArmComponent *CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		class UCameraComponent *PlayerCamera;
	UPROPERTY()
		FTimerHandle ManagerTimer;

private:

	bool bIsFPS;
	float RadiusCamera;
	FRotator CameraRotation;
};