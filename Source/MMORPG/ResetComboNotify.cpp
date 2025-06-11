// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetComboNotify.h"
#include "ThirdPersonMPCharacter.h"


void UResetComboNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AThirdPersonMPCharacter* Character = Cast<AThirdPersonMPCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->bCanDoCombo = false;
		Character->CurrentComboIndex = 0;
		Character->bCanAttack = true;
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Combo RESET"));

}


