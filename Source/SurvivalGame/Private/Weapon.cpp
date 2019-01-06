// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "NPC.h"
#include "Munition.h"
#include "Player_State.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	Munition = FMath::RandRange(0, maxMunition);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	WeaponMesh->bNoSkeletonUpdate = true;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetSimulatePhysics(true);

	RootComponent = WeaponMesh;
	TriggerComponent->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetHiddenInGame(false);
}

int AWeapon::GetMaxMunition()
{
	return maxMunition;
}

int AWeapon::GetMunition()
{
	return Munition;
}


void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Permitted to get the weapon type

EType AWeapon::GetType()
{
	return weaponType;
}

// Attach the weapon to NPC character

void AWeapon::AttachToPawn(ACharacter* Actor)
{
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GLog->Log(this->GetName() + " : " + this->GetActorLocation().ToString());
	RootComponent->AttachToComponent(Actor->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Weapon_Socket_R"));
	GLog->Log(this->GetName() + " : " + this->GetActorLocation().ToString());
	RootComponent->SetRelativeLocation(FVector::ZeroVector);
	GLog->Log("A Weapon is now attaching to " + this->GetName());
	GLog->Log(this->GetName() + " : " + this->GetActorLocation().ToString());
}

// Dettach th weapon to a NPC character

void AWeapon::DettachToPawn()
{
	FTimerHandle TimerHandle;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AWeapon::ReactivateOverlappingEvents, 2.5, false);
	GLog->Log("A Weapon dettach to PlayerCharacter");
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

void AWeapon::Attack()
{
	AActor* CQCTarget = NULL;

	if (CanCQC(this, CQCTarget) == true)
	{
		CQCAttack(CQCTarget);
	}
	else
	{
		if (weaponType != EType::E_NONE || weaponType != EType::E_BLUNT || weaponType != EType::E_SHARP)
		{
			Shot();
		}
	}
}

// Function that reactivate the overlap event after a delay after the dettachment from a NPC (use with a timer)

void AWeapon::ReactivateOverlappingEvents()
{
	TriggerComponent->SetGenerateOverlapEvents(true);
}

void AWeapon::CQCAttack(AActor* TargetActor)
{
	GLog->Log(this->GetName() + " : CQC Attack");
	if (TargetActor != nullptr)
	{
		ANPC* NPC = Cast<ANPC>(TargetActor);

		if (NPC != nullptr)
		{
			NPC->AddHealth(-BaseDamage);
			GLog->Log(this->GetName() + " : Cause " + FString::SanitizeFloat(-BaseDamage) + " in CQC, to " + NPC->GetName() + "who is now in " + FString::SanitizeFloat(NPC->GetHealth()) + " HP");
		}
	}
}

bool AWeapon::CanCQC(const AWeapon* Weapon, AActor* OverlappedActor = nullptr)
{
	GLog->Log(this->GetName() + " : CanCQC");

	TArray<AActor*> OverlappingActors;
	TSubclassOf<ANPC> NPCClass;

	if (Weapon->GetAttachParentActor() != nullptr)
	{
		ANPC* ParentNPC = Cast<ANPC>(Weapon->GetAttachParentActor());

		if (ParentNPC != nullptr)
		{
			ParentNPC->GetCapsuleComponent()->GetOverlappingActors(OverlappingActors, NPCClass);

			for (int i = 0; i < OverlappingActors.Num(); i++)
			{
				if (OverlappingActors[i] != nullptr && OverlappingActors[i] != this->GetAttachParentActor())
				{
					ANPC* NPC = Cast<ANPC>(OverlappingActors[i]);

					if (NPC != nullptr)
					{
						OverlappedActor = NPC;
						GLog->Log(this->GetName() + " : CanCQC = true, find " + OverlappedActor->GetName());
						return (true);
					}
				}
			}
		}
	}
	GLog->Log(this->GetName() + " : CanCQC = false");
	return (false);
}

// A shot function with raycasted traced technique (high performance but physics don't apply)

void AWeapon::Shot()
{
	GLog->Log(this->GetName() + " : Shot Attack");

	if (Munition > 0)
	{	
		switch (weaponType)
		{
		case EType::E_HANDGUN:
			Munition--;
			RaytracedShot();
			break;
		case EType::E_RIFFLE:
			GetWorldTimerManager().SetTimer(ShotRate, this, &AWeapon::ShotPhysic, 60 / RateOfFire, true, 0.0f);
			break;
		case EType::E_SHOTGUN:
			Munition--;
			for (int i = 0; i <= 50; i++)
			{
				RaytracedShot();
			}
			break;
		}
	}
	else
	{
		Reload();
	}
}

void AWeapon::RaytracedShot()
{
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	FVector StartLocation = WeaponMesh->GetSocketLocation(FName("b_gun_muzzleflash"));
	FRotator ShootRotation = FRotator(0.0, 90.0, 0.0);
	ShootRotation.Pitch += FMath::RandRange(-shootDeviation, shootDeviation);
	ShootRotation.Yaw += FMath::RandRange(-shootDeviation, shootDeviation);
	FVector EndLocation = GetActorLocation() + ((GetActorRotation() + ShootRotation).Vector() * RayLenght);

	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_Pawn, CollisionParams);
	if (FireSound != nullptr)
	{
		AudioComponent->SetSound(FireSound);
		AudioComponent->Play();
	}
	GLog->Log(Hit.ToString());
	if (Hit.GetActor() != nullptr)
	{
		ANPC* HitActor = Cast<ANPC>(Hit.GetActor());

		if (HitActor != nullptr)
		{
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 0.5, 0, 0.25);
			GLog->Log(this->GetName() + " : Raytracing -Hit " + HitActor->GetName() + "-");
			GLog->Log(this->GetName() + " : Hit at " + FString::SanitizeFloat(Hit.Distance) + " cm");
			if (Hit.Distance < RayLenght / 3)
			{
				HitActor->AddHealth(-BaseDamage);
				GLog->Log(this->GetName() + " : Cause " + FString::SanitizeFloat(-BaseDamage) + " (Max damage) to " + HitActor->GetName());
			}
			else
			{
				HitActor->AddHealth(-BaseDamage * (Hit.Distance / RayLenght));
				GLog->Log(this->GetName() + " : Cause " + FString::SanitizeFloat(-BaseDamage) + " to " + HitActor->GetName());
			}
			GLog->Log(this->GetName() + " : The hit actor are " + FString::SanitizeFloat(HitActor->GetHealth()) + " HP now");
			if (HitSound != nullptr)
			{
				AudioComponent->SetSound(HitSound);
				AudioComponent->Play();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Orange, false, 0.5, 0, 0.25);
			GLog->Log(this->GetName() + " : Raytracing -Actor is not NPC-");
			GLog->Log(FString::SanitizeFloat(Hit.Distance));
			if (MissSound != nullptr)
			{
				AudioComponent->SetSound(MissSound);
				AudioComponent->Play();
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0.5, 0, 0.25);
		GLog->Log(this->GetName() + " : Raytracing -No Actor Hit-");
	}
}

// A shot function with physics object technique (lowwer performance but physics apply)

void AWeapon::ShotPhysic()
{
	GLog->Log(this->GetName() + " : Physics Shot Attack");

	if (ProjectileClass != NULL && Munition > 0 && this->GetAttachParentActor() != nullptr)
	{
		ANPC* AttachActor = Cast<ANPC>(this->GetAttachParentActor());
		
		if (AttachActor != nullptr && AttachActor->GetAttack())
		{
			Munition--;

			FVector CameraLoc;
			FRotator CameraRot;

			this->GetAttachParentActor()->GetActorEyesViewPoint(CameraLoc, CameraRot);

			FVector MuzzleLocation = WeaponMesh->GetSocketLocation(FName("b_gun_muzzleflash"));
			FRotator MuzzleRotation = CameraRot;
			MuzzleRotation.Pitch += FMath::FRandRange(-shootDeviation, shootDeviation);
			MuzzleRotation.Yaw += FMath::FRandRange(-shootDeviation, shootDeviation);
			UWorld * const World = GetWorld();

			if (World != nullptr)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Cast<ANPC>(this->GetAttachParentActor());

				ABullet* const Projectile = World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

				if (Projectile != nullptr)
				{
					Projectile->SetProjectileDamage(BaseDamage);
					Projectile->SetSpeed(projectileVelocity);

					FVector const LaunchDir = MuzzleRotation.Vector();
					Projectile->InitialVelocity(LaunchDir);

					if (FireSound != nullptr)
					{
						AudioComponent->SetSound(FireSound);
						AudioComponent->Play();
					}
					GLog->Log(this->GetName() + " : Projectile Spawn");
				}
			}
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ShotRate);
	}
}

void AWeapon::Reload()
{
	ANPC* ParentNPC = Cast<ANPC>(this->GetAttachParentActor());

	if (ParentNPC != nullptr)
	{
		for (int32 i = 0; i < ParentNPC->Inventory.Num(); i++)
		{
			AMunition* MunitionItem = Cast<AMunition>(ParentNPC->Inventory[i]);

			if (MunitionItem != nullptr)
			{
				Munition = MunitionItem->GetMunition();
				if (Munition > maxMunition)
				{
					MunitionItem->SetMunition(Munition - maxMunition);
					Munition = maxMunition;
				}
				else
				{
					ParentNPC->Inventory[i] = nullptr;
				}
				break;
			}
		}
	}
}