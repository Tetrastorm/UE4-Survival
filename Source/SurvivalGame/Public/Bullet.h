// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Bullet.generated.h"

UCLASS()
class SURVIVALGAME_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABullet(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Physics)
		USphereComponent* CollisionSphere;
	UPROPERTY()
		UParticleSystem* ProjectileParticle;
	UPROPERTY()
		UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleDefaultsOnly, Category = "Audio")
		UAudioComponent* AudioComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundWave* ImpactSound;
	UPROPERTY(EditDefaultsOnly)
		USoundWave* BodyImpactSound;

	void InitialVelocity(const FVector& ShootDirection);

	UFUNCTION()
		void Dommage(class UPrimitiveComponent* OtherComponent, class AActor* OtherActor, class UPrimitiveComponent* Component, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	void SetProjectileDamage(float newValue);
	void SetSpeed(float newSpeed);

private:

	float BaseDamage;
};
