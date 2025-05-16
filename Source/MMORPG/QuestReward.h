// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestReward.generated.h"

UENUM(BlueprintType)
enum class EQuestRewardType: uint8
{
	Gold UMETA(DisplayName = "Gold"),
	Experience UMETA(DisplayName ="Experience"),
	Item UMETA(DisplayName = "Item")
	
};


USTRUCT(BlueprintType)
struct FQuestReward
{
	GENERATED_BODY()


public:
	// Numele recompensei ( Gold, Item-Sabie, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestReward")
	FString RewardName;

	// Cantitatea recompensei
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestReward")
	int32 RewardAmount;

	// Tipul recompensei (Gold, Experience, Item)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestReward")
	EQuestRewardType RewardType;

	// Item ID ( folosit doar daca RewardType == Item)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestReward", meta = (EditCondition = "RewardType == EQuestRewardType::Item"))
	FString ItemID;



	FQuestReward()
		: RewardName(TEXT("Default Reward"))
		, RewardAmount(0)
	{}

	FQuestReward(const FString& InName, int32 InAmount)
		:RewardName(InName)
		,RewardAmount(InAmount)
	{ }


	FQuestReward(const FString& InName, int32 InAmount,EQuestRewardType InType, const FString& InItemID = TEXT(""))
		:RewardName(InName)
		, RewardAmount(InAmount)
		,RewardType(InType)
		,ItemID(InItemID)
	{}



};


/**
 * 

class MMORPG_API QuestReward
{
public:
	QuestReward();
	~QuestReward();
};
 */