// Fill out your copyright notice in the Description page of Project Settings.

#include "SupportItem.h"
#include "NPC.h"

void ASupportItem::Use(AActor* CharacterActor)
{
	ANPC* CharacterNPC = Cast<ANPC>(CharacterActor);

	if (bIsOverTime)
	{

	}
	else
	{
		CharacterNPC->AddHealth(Heal);
		CharacterNPC->AddStamina(Stamina);
		CharacterNPC->SetInfected(bCureInfection);
	}
}


