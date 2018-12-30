// Fill out your copyright notice in the Description page of Project Settings.

#include "Munition.h"

AMunition::AMunition()
{
	MunitionStocked = 0;
}

int  AMunition::GetMunition()
{
	return MunitionStocked;
}

void AMunition::SetMunition(int NewValue)
{
	MunitionStocked = NewValue;
}

void AMunition::BeginPlay()
{
	Super::BeginPlay();

	MunitionStocked = FMath::RandRange(0, MaxMunition);
}