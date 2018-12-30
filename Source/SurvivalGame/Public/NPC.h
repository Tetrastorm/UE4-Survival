// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Item.h"
#include "Weapon.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

UENUM(BlueprintType)
enum class EPosition : uint8
{
	E_STAND = 0,
	E_CROUCH,
	E_PRONE
};

UCLASS()
class SURVIVALGAME_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:

	ANPC();

	EPosition GetPosition();

	UFUNCTION(BlueprintCallable, Category = "Getter")
		bool GetAction();
	UFUNCTION(BlueprintCallable, Category = "Getter")
		bool GetAttack();
	UFUNCTION(BlueprintCallable, Category = "Getter")
		bool GetInfected();
	UFUNCTION(BlueprintCallable, Category = "Getter")
		bool GetCanJump();
	UFUNCTION(BlueprintCallable, Category = "Getter")
		bool GetDeath();
	UFUNCTION(BlueprintCallable, Category = "Getter")
		bool GetIronsight();

	UFUNCTION(BlueprintCallable, Category = "Getter")
		float GetMaxHealth();
	UFUNCTION(BlueprintCallable, Category = "Getter")
		float GetMaxStamina();
	UFUNCTION(BlueprintCallable, Category = "Getter")
		float GetHealth();
	UFUNCTION(BlueprintCallable, Category = "Getter")
		float GetStamina();
	UFUNCTION(BlueprintCallable, Category = "Getter")
		float GetArmor();
	UFUNCTION(BlueprintCallable, Category = "Getter")
		float GetStealth();

	UFUNCTION(BlueprintCallable, Category = "Setter")
		void AddHealth(float Value);
	UFUNCTION(BlueprintCallable, Category = "Setter")
		void AddStamina(float fValue);
	UFUNCTION(BlueprintCallable, Category = "Setter")
		void SetInfected(bool bNewValue);
	UFUNCTION(BlueprintCallable, Category = "Setter")
		void SetIronsight(bool newValue);

	void GainLevel(int iValue);
	void AddInventory(AItem* Item);

	void ActualisedInventoryWeight();

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundWave* breathingSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundWave* combatSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundWave* PickupSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		AWeapon* WeaponEquiped;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
		TArray<AItem*> Inventory;

protected:

	virtual void Tick(float DeltaTime) override;

	void StartJump();
	void StopJump();

	void StartSprint();
	void StopSprint();

	void DownPosition();
	void UpPosition();

	void StartAction();
	void StopAction();

	void StartAttack();
	void StopAttack();

	void StaminaManager();
	void StealthManager();

	void Death();

	void EquipeWeapon(AWeapon* Weapon);
	void DropItem();

	void RemoveToWorld();

	UPROPERTY()
		UCapsuleComponent* DommageHitbox;
	UPROPERTY(VisibleAnywhere, Category = "Audio")
		UAudioComponent* AudioComponent;

private:

	EPosition Position;

	bool bSprint;
	bool bAction;
	bool bAttack;
	bool bInfected;
	bool bIsDeath;
	bool bIsIronsight;

	float fMaxHealth;
	float fMaxStamina;

	float fArmor;
	float fStaminaConsume;
	float fStaminaRecovery;

	float fHealth;
	float fStamina;
	float fStealth;
	float fInventoryWeight;
	float fMaxInventoryWeight;

	int iPlayerLevel;
	int iXPRequiment;
	int iXP;
	int iLevel;

	int iCrouchSpeed;
	int iWalkingSpeed;
	int iSprintSpeed;
	int iProneSpeed;
};