// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class SURVIVALGAME_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

	UFUNCTION(BlueprintCallable, Category = "Setter")
		void SetNumItem(int newValue);

	UFUNCTION(BlueprintCallable, Category = "Getter")
		int GetNumItem();
	UFUNCTION(BlueprintCallable, Category = "Getter")
		float GetWeight();

	void PickUp(AActor* OtherActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Property")
		UTexture2D* ItemImage;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
		void TriggerEnter(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComponent, int32 OtherBodyIdex, bool dFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Item Property")
		UStaticMeshComponent* ItemMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Item Property")
		UBoxComponent* TriggerComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Item Property")
		float fWeight;

private:

	int numItem;
};
