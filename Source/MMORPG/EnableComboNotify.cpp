// Fill out your copyright notice in the Description page of Project Settings.

#include "EnableComboNotify.h"
#include "GameFramework/Actor.h"
#include "ThirdPersonMPCharacter.h"


void UEnableComboNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	AThirdPersonMPCharacter* Character = Cast<AThirdPersonMPCharacter>(Owner);
	if (Character)
	{
		Character->bCanDoCombo = true;

		// aici verificam daca jucatorul a apasat deja click
		Character->bComboInputBuffered = false;

	}

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("Combo ENABLED"));

}


