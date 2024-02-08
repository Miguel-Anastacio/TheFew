// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Senses/AISenseConfig_NoseSight.h"

UAISenseConfig_NoseSight::UAISenseConfig_NoseSight(const FObjectInitializer& ObjectInitializer)
{
	DebugColor = FColor::Cyan;
	MaxAge = 0;
	bStartsEnabled = true;
	
}

TSubclassOf<UAISense> UAISenseConfig_NoseSight::GetSenseImplementation() const
{
	return *Implementation;
}
