// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "NPC.h"
#include "PlayerCharacter.h"
#include "Player_State.h"
#include "Weapon.h"


// Sets default values
ABullet::ABullet(const FObjectInitializer& ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->InitSphereRadius(5.0f);
	CollisionSphere->bGenerateOverlapEvents = true;
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABullet::Dommage);
	CollisionSphere->SetHiddenInGame(false);

	RootComponent = CollisionSphere;

	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = CollisionSphere;
	ProjectileMovement->InitialSpeed = 3400.0f;
	ProjectileMovement->MaxSpeed = ProjectileMovement->InitialSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.5f;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetHiddenInGame(false);

	BaseDamage = 0;
}

void ABullet::SetProjectileDamage(float newValue)
{
	BaseDamage = newValue;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::SetSpeed(float newSpeed)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->InitialSpeed = newSpeed;
	}
}

void ABullet::InitialVelocity(const FVector& ShootDirection)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void ABullet::Dommage(class UPrimitiveComponent* OtherComponent, class AActor* OtherActor, class UPrimitiveComponent* Component, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	ANPC* Character = Cast<ANPC>(OtherActor);
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	AWeapon* Weapon = Cast <AWeapon>(OtherActor);

	if (!Player && !Weapon)
	{
		if (Character)
		{
			if (BodyImpactSound)
			{
				AudioComponent->SetSound(BodyImpactSound);
				AudioComponent->Play();
			}

			Character->AddHealth(-BaseDamage);

			GLog->Log(this->GetName() + " : Damage " + Character->GetName() + " why is now " + FString::SanitizeFloat(Character->GetHealth()) + " HP");
			GLog->Log(this->GetName() + " : Cause " + FString::SanitizeFloat(-BaseDamage) + " HP to damage");
			GLog->Log(GetVelocity().ToString());

			Destroy();
		}
		else
		{
			if (ImpactSound)
			{
				AudioComponent->SetSound(ImpactSound);
				AudioComponent->Play();
			}

			GLog->Log(this->GetName() + " : collide with environnement");

			Destroy();
		}
	}
}