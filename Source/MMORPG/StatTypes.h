#pragma once

#include "CoreMinimal.h"
#include "StatTypes.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EStatTypes : uint8
{
	MaxHealth UMETA(DisplayName = "MaxHealth"),
	HealthRegen UMETA(DisplayName = "HealthRegen"),
	HealthRegenInterval UMETA(DisplayName = "HealthRegenInterval"),
	MaxMana UMETA(DisplayName = "MaxMana"),
	ManaRegen UMETA(DisplayName = "ManaRegen"),
	ManaRegenInterval UMETA(DisplayName = "ManaRegenInterval"),
	MaxSkillStamina UMETA(DisplayName = "MaxSkillStamina"),
	SkillStaminaRegen UMETA(DisplayName = "SkillStaminaRegen"),
	SkillStaminaRegenInterval UMETA(DisplayName = "SkillStaminaRegenInterval"),
	MaxMovementStamina UMETA(DisplayName = "MaxMovementStamina"),
	MovementStaminaRegen UMETA(DisplayName = "MovementStaminaRegen"),
	MovementStaminaRegenInterval UMETA(DisplayName = "MovementStaminaRegenInterval"),
	PhysicalAttack UMETA(DisplayName = "PhysicalAttack"),
	MagicalAttack UMETA(DisplayName = "MagicalAttack"),
	PhysicalDefense UMETA(DisplayName = "PhysicalDefense"),
	MagicalDefense UMETA(DisplayName = "MagicalDefense"),
	Strength UMETA(DisplayName = "Strength"),
	Constitution UMETA(DisplayName = "Constitution"),
	Dexterity UMETA(DisplayName = "Dexterity"),
	Intelligence UMETA(DisplayName = "Intelligence"),
	Wisdom UMETA(DisplayName = "Wisdom"),
	Luck UMETA(DisplayName = "Luck"),
	Count UMETA(Hidden),

};
