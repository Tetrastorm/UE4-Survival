// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC.h"
#include "Player_Controller.h"

ANPC::ANPC()
{
	PrimaryActorTick.bCanEverTick = true;

	Position = EPosition::E_STAND;

	bSprint = false;
	bAction = false;
	bAttack = false;
	bInfected = false;
	bIsDeath = false;
	bIsIronsight = false;

	fMaxHealth = 100.0f;
	fMaxStamina = 100.0f;

	fArmor = 1.0f;
	fStaminaConsume = 5.0f;
	fStaminaRecovery = 2.5f;

	fHealth = 100.0f;
	fStamina = 100.0f;
	fStealth = 5.0f;
	fInventoryWeight = 0.0f;
	fMaxInventoryWeight = 100.0f;

	iPlayerLevel = 1;
	iXPRequiment = 250;
	iXP = 0;
	iLevel = 1;

	iCrouchSpeed = 300;
	iWalkingSpeed = 500;
	iSprintSpeed = 1000;
	iProneSpeed = 100;

	DommageHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Left Hand Hitbox"));
	DommageHitbox->SetupAttachment(GetCapsuleComponent());
	DommageHitbox->SetRelativeLocation(FVector(75.0, 0.0, 50.0));
	DommageHitbox->SetGenerateOverlapEvents(true);
	DommageHitbox->SetHiddenInGame(false);

	AudioComponent=CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(GetMesh(), FName("head"));
	AudioComponent->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	AudioComponent->SetHiddenInGame(false);
}

EPosition ANPC::GetPosition()
{
	return Position;
}

bool ANPC::GetAction()
{
	return bAction;
}

bool ANPC::GetAttack()
{
	return bAttack;
}

bool ANPC::GetInfected()
{
	return bInfected;
}

bool ANPC::GetCanJump()
{
	return bPressedJump;
}

bool ANPC::GetDeath()
{
	return bIsDeath;
}

bool ANPC::GetIronsight()
{
	return bIsIronsight;
}

float ANPC::GetMaxHealth()
{
	return fMaxHealth;
}

float ANPC::GetMaxStamina()
{
	return fMaxStamina;
}

float ANPC::GetHealth()
{
	return fHealth;
}

float ANPC::GetStamina()
{
	return fStamina;
}

float ANPC::GetArmor()
{
	return fArmor;
}

float ANPC::GetStealth()
{
	return fStealth;
}

// Setter

void ANPC::AddHealth(float Value)
{
	GLog->Log(FString::SanitizeFloat(Value));
	fHealth += Value;

	if (fHealth > fMaxHealth)
	{
		fHealth = fMaxHealth;
	}
}

void ANPC::AddStamina(float Value)
{
	GLog->Log(this->GetName() + " : this stamina gain  " + FString::SanitizeFloat(Value));
	fStamina += Value;

	if (fStamina > fStamina)
	{
		fStamina = fMaxStamina;
	}
}

void ANPC::SetInfected(bool bNewValue)
{
	bInfected = bNewValue;
}

void ANPC::SetIronsight(bool newValue)
{
	bIsIronsight = newValue;
}

void ANPC::GainLevel(int iValue)
{
	iXP += iValue;

	if (iXP >= iXPRequiment)
	{
		iLevel++;
		iXPRequiment = iXPRequiment * 2;
		iXP = 0;
	}
}

// Public Inventory Function

void ANPC::AddInventory(AItem* Item)
{
	if (fInventoryWeight + Item->GetWeight() <= fMaxInventoryWeight)
	{
		AWeapon* Weapon = Cast<AWeapon>(Item);

		if (Weapon)
		{
			GLog->Log(Weapon->GetName() + " is a weapon");
			EquipeWeapon(Weapon);
		}
		else
		{
			Inventory.Add(Item);
			Item->Destroy();
		}
	}
	else
	{
		GLog->Log(this->GetName() + " : Inventory is full");
	}
}

// Protected Function

// Virtual Function

void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (fHealth <= 0)
	{
		Death();
	}
}

// Action Function

void ANPC::StartJump()
{
	GLog->Log("StartJump()");
	GLog->Log("Actual Position: " + UUserDefinedEnum::GetValueAsString<EPosition>(TEXT("NPC.EPosition"), Position));
	if (fStamina > 0)
	{
		if (Position != EPosition::E_PRONE)
		{
			bPressedJump = true;
		}
		if (Position != EPosition::E_STAND)
		{
			Position = EPosition::E_STAND;
			GetCharacterMovement()->MaxWalkSpeed = iWalkingSpeed; 
		}
		fStamina -= fStaminaConsume * 3;
	}
	GLog->Log("New Position: " + UUserDefinedEnum::GetValueAsString<EPosition>(TEXT("NPC.EPosition"), Position));
	GLog->Log("");
}

void ANPC::StopJump()
{
	bPressedJump = false;
}

void ANPC::StartSprint()
{
	GLog->Log("StartSprint()");
	GLog->Log("Actual Position: " + UUserDefinedEnum::GetValueAsString<EPosition>(TEXT("NPC.EPosition"), Position));
	if (fStamina > 0)
	{
		if (Position != EPosition::E_STAND || Position != EPosition::E_CROUCH)
		{
			Position = EPosition::E_STAND;
			GetCharacterMovement()->MaxWalkSpeed = iSprintSpeed;
			bSprint = true;
		}
	}
	GLog->Log("New Position: " + UUserDefinedEnum::GetValueAsString<EPosition>(TEXT("NPC.EPosition"), Position));
	GLog->Log("");
}

void ANPC::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = iWalkingSpeed;
	bSprint = false;
}

void ANPC::DownPosition()
{
	GLog->Log("DownPosition()");
	GLog->Log("Actual Position: " + UUserDefinedEnum::GetValueAsString<EPosition>(TEXT("NPC.EPosition"), Position) + "\n");
	switch (Position)
	{
	case EPosition::E_STAND:
		if (bSprint)
		{
			StopSprint();
		}
		Position = EPosition::E_CROUCH;
		GetCharacterMovement()->MaxWalkSpeed = iCrouchSpeed;
		break;
	case EPosition::E_CROUCH:
		Position = EPosition::E_PRONE;
		GetCharacterMovement()->MaxWalkSpeed = iProneSpeed;
		break;
	}
	GLog->Log("New Position: " + UUserDefinedEnum::GetValueAsString<EPosition>(TEXT("NPC.EPosition"), Position) + "\n");
	GLog->Log("");
}

void ANPC::UpPosition()
{
	GLog->Log("UpPosition()");
	GLog->Log("Actual Position: " + UUserDefinedEnum::GetValueAsString<EPosition>(TEXT("NPC.EPosition"), Position) + "\n");
	switch (Position)
	{
	case EPosition::E_PRONE:
		Position = EPosition::E_CROUCH;
		GetCharacterMovement()->MaxWalkSpeed = iCrouchSpeed;
		break;
	case EPosition::E_CROUCH:
		Position = EPosition::E_STAND;
		GetCharacterMovement()->MaxWalkSpeed = iWalkingSpeed;
		break;
	}
	GLog->Log("New Position: " + UUserDefinedEnum::GetValueAsString<EPosition>(TEXT("NPC.EPosition"), Position) + "\n");
	GLog->Log("");
}

void ANPC::StartAction()
{
	GLog->Log("A actor Action On");
	bAction = true;
}

void ANPC::StopAction()
{
	GLog->Log("A actor Action Off");
	bAction = false;
}

void ANPC::StartAttack()
{
	GLog->Log("Attack start");

	bAttack = true;

	if (WeaponEquiped != nullptr)
	{
		WeaponEquiped->Attack();
	}
	else
	{
		TArray<AActor*> OverlappingActors;

		DommageHitbox->GetOverlappingActors(OverlappingActors);
		if (OverlappingActors.Max() != 0)
		{
			ANPC* OverlappingActor = Cast<ANPC>(OverlappingActors[0]);

			if (OverlappingActor != nullptr && OverlappingActor != this)
			{
				OverlappingActor->AddHealth(-FMath::RandRange(1, 15) * OverlappingActor->GetArmor());
				GLog->Log(FString::SanitizeFloat(OverlappingActor->GetHealth()));
			}
			if (combatSound)
			{
				AudioComponent->SetSound(combatSound);
				AudioComponent->Play();
			}
		}
	}
}

void ANPC::StopAttack()
{
	GLog->Log("Attack stop");

	bAttack = false;
}

// Manager Function

void ANPC::StaminaManager()
{
	//Stamina

	if (bSprint)
	{
		fStamina -= fStaminaConsume;

		if (fStamina <= 0)
		{
			StopSprint();
		}
	}
	if (!bSprint)
	{
		if (fMaxStamina > fStamina)
		{
			if (Position == EPosition::E_CROUCH)
			{
				if (!bInfected)
				{
					fStamina += 0.75 * fStaminaRecovery;
				}
				else
				{
					fStamina += fStaminaRecovery;
				}	
			}
			else
			{
				if (!bInfected)
				{
					fStamina += 0.75 * fStaminaRecovery;
				}
				else
				{
					fStamina += fStaminaRecovery;
				}
			}
		}
	}
	
	if ((fStamina < fMaxStamina * 0.25) && breathingSound != nullptr && !AudioComponent->IsActive())
	{
		AudioComponent->SetSound(breathingSound);
		AudioComponent->Play();
	}

	if (fMaxStamina < fStamina)
	{
		fStamina = fMaxStamina;
	}
	if (fStamina < 0)
	{
		fStamina = 0;
	}
}

void ANPC::StealthManager()
{
	//Stealth

	if (Position == EPosition::E_CROUCH)
	{
		fStealth = 2.5;
	}
	if (!bSprint)
	{
		fStealth = 5;
	}
	if (bSprint || bPressedJump)
	{
		fStealth = 7.5;
	}
	if (bAttack)
	{
		if (WeaponEquiped)
		{
			fStealth = 10.0;
		}
	}
}

// State Function

void ANPC::Death()
{
	APlayer_Controller* Controller = Cast<APlayer_Controller>(GetController());	

	bIsDeath = true;

	this->GetMovementComponent()->Deactivate();

	if (Controller)
	{
		Controller->RestartLevel();
	}
	else
	{
		if (Inventory.Num() < 0)
		{
			GetCapsuleComponent()->SetGenerateOverlapEvents(true);
		}
		else
		{
			FTimerHandle DeathHandle;
			GetWorldTimerManager().SetTimer(DeathHandle, this, &ANPC::RemoveToWorld, 0.0f, false, 180.0f);
		}
	}
}

//	Inventory Function

void ANPC::EquipeWeapon(AWeapon* Weapon)
{
	if (WeaponEquiped)
	{
		WeaponEquiped->DettachToPawn();
	}

	WeaponEquiped = Weapon;

	if (PickupSound)
	{
		AudioComponent->SetSound(PickupSound);
		AudioComponent->Play();
	}

	WeaponEquiped->AttachToPawn(this);
}

void ANPC::DropItem()
{
	//FActorSpawnParameters SpawnParams;
	//AItem* DroppedItem = GetWorld()->SpawnActor<AItem>(Item, GetTransform(), SpawnParams);
	
	if (WeaponEquiped)
	{
		WeaponEquiped->DettachToPawn();
		WeaponEquiped = nullptr;
	}
	ActualisedInventoryWeight();
}

void ANPC::ActualisedInventoryWeight()
{
	float fWeight = 0.0f;
	
	if (0 < Inventory.Num())
	{
		for (int32 i = 0; i < Inventory.Num(); i++)
		{
			if (Inventory[i] != nullptr)
			{
				fWeight += Inventory[i]->GetWeight();
			}
		}
	}
	fInventoryWeight = fWeight;
}

void ANPC::RemoveToWorld()
{
	this->Destroy();
}