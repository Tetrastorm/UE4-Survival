// Fill out your copyright notice in the Description page of Project Settings.

#include "NPCAnimInstance.h"
#include "Player_State.h"

UNPCAnimInstance::UNPCAnimInstance()
{
	WeaponType = EType::E_NONE;
	NPCPosition = EPosition::E_STAND;

	bIsFalling = false;
	bIsReloading = false;
	bIsDeath = false;
	bIsIronsight = false;

	MovementSpeed = 0.0;
	MovementDirection = 0.0;
}

void UNPCAnimInstance::UpdateStatement()
{
	if (!bIsDeath)
	{
		if (this->GetOwningActor() != nullptr)
		{
			ANPC* NPC = Cast<ANPC>(this->GetOwningActor());

			if (NPC != nullptr)
			{
				if (NPC->WeaponEquiped != nullptr && WeaponType != NPC->WeaponEquiped->GetType())
				{
					WeaponType = NPC->WeaponEquiped->GetType();
				}
				NPCPosition = NPC->GetPosition();

				bIsFalling = NPC->GetMovementComponent()->IsFalling();
				bCanJump = NPC->GetCanJump();
				bIsDeath = NPC->GetDeath();
				bIsIronsight = NPC->GetIronsight();

				MovementSpeed = NPC->GetVelocity().Size();
				MovementDirection = CalculateDirection(NPC->GetVelocity(), NPC->GetActorRotation());
			}
		}
	}
}