// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "NPC.h"
#include "Player_State.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;

	numItem = 1;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	ItemMesh->SetRelativeLocation(FVector::ZeroVector);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	ItemMesh->SetSimulatePhysics(true);

	RootComponent = ItemMesh;

	TriggerComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Trigger Component"));
	TriggerComponent->SetupAttachment(RootComponent);
	TriggerComponent->SetRelativeLocation(FVector::ZeroVector);
	TriggerComponent->bGenerateOverlapEvents = true;
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::TriggerEnter);
	TriggerComponent->SetCollisionObjectType(ECC_Pawn);
	TriggerComponent->SetHiddenInGame(false);
}

// Public Function

// Getter

void AItem::SetNumItem(int newValue)
{
	numItem = newValue;
}

int AItem::GetNumItem()
{
	return numItem;
}

float AItem::GetWeight()
{
	return fWeight;
}

// Protected Function

//Virtual Function

void AItem::BeginPlay()
{
	Super::BeginPlay();

	TriggerComponent->bGenerateOverlapEvents = true;
}

// State Function

void AItem::PickUp(AActor* OtherActor)
{
	ANPC* NPC = Cast<ANPC>(OtherActor);

	GLog->Log("Picking up");

	if (NPC)
	{
		if (NPC->GetAction())
		{
			TriggerComponent->bGenerateOverlapEvents = false;

			NPC->AddInventory(this);

			GLog->Log(NPC->GetName() + " Picking up : " + this->GetName());
		}
	}
}

//	Trigger Function

void AItem::TriggerEnter(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int32 OtherBodyIdex, bool dFromSweep, const FHitResult& SweepResult)
{
	GLog->Log(this->GetName() + " : Trigerring");
	PickUp(OtherActor);
}