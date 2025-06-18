// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseNPC.h"


// Sets default values
ABaseNPC::ABaseNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameWidget"));
	NameWidget->SetupAttachment(RootComponent);
	NameWidget->SetWidgetSpace(EWidgetSpace::World);
	NameWidget->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	NameWidget->SetDrawSize(FVector2D(120.f, 50.f));
	NameWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ABaseNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseNPC::Interact()
{
	// implementarea in clasele ce o mostenesc
}
