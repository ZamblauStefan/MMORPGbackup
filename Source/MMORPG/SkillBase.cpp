// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"


void USkillBase::ActivateSkill_Implementation(AThirdPersonMPCharacter* Character)
{
    // logica default, suprascrisa in skilluri derivate

     // Verificare Weapon Requirement
    
    if (RequiredWeapon != EWeaponType::None)
    {
        if (!Character->HasWeaponType(RequiredWeapon))
        {
            UE_LOG(LogTemp, Warning, TEXT("Cannot use this skill without correct weapon equipped."));
            return;
        }
    }
 
    // Skill logic here
}



