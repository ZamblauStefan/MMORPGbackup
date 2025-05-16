// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeSkillsComponent.h"


// Sets default values for this component's properties
ULifeSkillsComponent::ULifeSkillsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Initializare valori implicite:
	Gathering = 0;
	Lumbering = 0;
	Processing = 0;
	Fishing = 0;

	// ...
}


// Called when the game starts
void ULifeSkillsComponent::BeginPlay()
{
	Super::BeginPlay();

	// Aici se poate initializa cu date dintr-o salvare / DB


	// ...
	
}

void ULifeSkillsComponent::IncreaseGathering(int32 Amount)
{
	Gathering += Amount;
	// Se poate adauga logica suplimentara, cum ar fi actualizarea UI-ului, update sau replicare
}

void ULifeSkillsComponent::IncreaseLumbering(int32 Amount)
{
	Lumbering += Amount;
	// Se poate adauga logica suplimentara, cum ar fi actualizarea UI-ului, update sau replicare}

}

void ULifeSkillsComponent::IncreaseProcessing(int32 Amount)
{
	Processing += Amount;
	// Se poate adauga logica suplimentara, cum ar fi actualizarea UI-ului, update sau replicare}

}

void ULifeSkillsComponent::IncreaseFishing(int32 Amount)
{
	Fishing += Amount;
	// Se poate adauga logica suplimentara, cum ar fi actualizarea UI-ului, update sau replicare}

}

void ULifeSkillsComponent::IncreaseCrafting(int32 Amount)
{
	Crafting += Amount;
	// Se poate adauga logica suplimentara, cum ar fi actualizarea UI-ului, update sau replicare}

}


// Called every frame
void ULifeSkillsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

