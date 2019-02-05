// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Bullet.h"
#include "Weapon.generated.h"

/**
**/

UENUM(BlueprintType)
enum class EType : uint8
{
	E_NONE = 0,
	E_BLUNT,
	E_SHARP,
	E_HANDGUN,
	E_RIFFLE,
	E_SHOTGUN
};

UCLASS()
class SURVIVALGAME_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();

	EType GetType();

	void AttachToPawn(class ACharacter* Actor);
	void DettachToPawn();

	virtual void Attack();

	UPROPERTY(VisibleAnywhere,Category = "Weapon Property")
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Property")
		UTexture2D* HUDPicture;
	
	UFUNCTION(BlueprintCallable, Category = "Getter")
		int GetMaxMunition();
	UFUNCTION(BlueprintCallable, Category = "Getter")
		int GetMunition();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Property")
		float BaseDamage = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Property")
		EType weaponType;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Property")
		float RateOfFire;

	UPROPERTY()
		UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Property")
		USoundWave* AttackSound;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Property")
		USoundWave* HitSound;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Property")
		USoundWave* MissSound;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Property")
		USoundWave* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Property")
		TSubclassOf<class ABullet> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Property")
		float RayLenght = 100000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Property")
		float shootDeviation = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Property")
		float projectileVelocity = 300.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Property")
		int maxMunition = 30;

	UPROPERTY()
		FTimerHandle ShotRate;

	void Shot();
	void ShotPhysic();
	void RaytracedShot();

	void Reload();

	void ReactivateOverlappingEvents();
	void CQCAttack(AActor* TargetActor);

	bool CanCQC(const AWeapon* Weapon, AActor* OverlappedActor);

private:
	
	int Munition;
};