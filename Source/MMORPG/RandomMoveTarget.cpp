// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomMoveTarget.h"


// Sets default values
ARandomMoveTarget::ARandomMoveTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	bReplicates = false;

}

// Called when the game starts or when spawned
void ARandomMoveTarget::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ARandomMoveTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

