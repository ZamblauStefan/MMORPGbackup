

#include "ItemTooltip.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "ItemBase.h"



void UItemTooltip::NativeConstruct()
{
	Super::NativeConstruct();
}

void UItemTooltip::SetItemDetails(UItemBase* Item)
{
	if (!StatsBox || !Item) return;

	StatsBox->ClearChildren();

	auto AddStatIfNotZero = [this](const FString& StatName, float Value)
		{
			if (FMath::Abs(Value) > KINDA_SMALL_NUMBER)
			{
				UTextBlock* StatText = NewObject<UTextBlock>(this);
				StatText->SetText(FText::FromString(StatName + ": " + FString::SanitizeFloat(Value)));
				StatsBox->AddChild(StatText);
			}
		};

	// Adaugam doar valorile diferite de 0
	AddStatIfNotZero("Max Health", Item->ItemStatistics.MaxHealth);
	AddStatIfNotZero("Max Mana", Item->ItemStatistics.MaxMana);
	AddStatIfNotZero("Max Skill Stamina", Item->ItemStatistics.MaxSkillStamina);
	AddStatIfNotZero("Max Movement Stamina", Item->ItemStatistics.MaxMovementStamina);
	AddStatIfNotZero("Physical Attack", Item->ItemStatistics.PhysicalAttack);
	AddStatIfNotZero("Magical Attack", Item->ItemStatistics.MagicalAttack);
	AddStatIfNotZero("Physical Defense", Item->ItemStatistics.PhysicalDefense);
	AddStatIfNotZero("Magical Defense", Item->ItemStatistics.MagicalDefense);
	AddStatIfNotZero("Strength", Item->ItemStatistics.Strength);
	AddStatIfNotZero("Dexterity", Item->ItemStatistics.Dexterity);
	AddStatIfNotZero("Constitution", Item->ItemStatistics.Constitution);
	AddStatIfNotZero("Intelligence", Item->ItemStatistics.Intelligence);
	AddStatIfNotZero("Wisdom", Item->ItemStatistics.Wisdom);
	AddStatIfNotZero("Luck", Item->ItemStatistics.Luck);
}