// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataStructs.h"
#include "SkillBase.generated.h"


UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Melee      UMETA(DisplayName = "Melee"),
    Spell      UMETA(DisplayName = "Spell"),
    Buff       UMETA(DisplayName = "Buff"),
    Dash       UMETA(DisplayName = "Dash")
  
};

UENUM(BlueprintType)
enum class EWeaponRequirement : uint8
{
    None       UMETA(DisplayName = "None"),
    Sword      UMETA(DisplayName = "Sword"),
    Staff      UMETA(DisplayName = "Staff"),
    // Bow        UMETA(DisplayName = "Bow")
  
};

UCLASS()
class MMORPG_API USkillBase : public UObject
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillType SkillType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    EWeaponType RequiredWeapon = EWeaponType::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    float Cooldown;

    // functie virtuala de activare (override in skilluri concrete)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill")
    void ActivateSkill(class AThirdPersonMPCharacter* Character);
	
	
};
