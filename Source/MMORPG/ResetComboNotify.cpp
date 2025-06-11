// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetComboNotify.h"
#include "ThirdPersonMPCharacter.h"


void UResetComboNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	if (AThirdPersonMPCharacter* Character = Cast<AThirdPersonMPCharacter>(MeshComp->GetOwner()))
	{
		Character->ResetComboSection();
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, TEXT("Combo RESET"));

}


