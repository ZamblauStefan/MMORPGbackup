// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"
#include "Engine/Engine.h"




void UQuestManager::AddQuest(const FQuest& NewQuest)
{
	PlayerQuests.Add(NewQuest);
	if (GEngine)
	{
		FString Message = FString::Printf(TEXT("Quest Added: %s"), *NewQuest.QuestName);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message);
	}
}


bool UQuestManager::AcceptQuest(int32 QuestID)
{
	for (FQuest& Quest : PlayerQuests)
	{
		if (Quest.QuestID == QuestID)
		{
			if (Quest.QuestStatus == EQuestStatus::NotStarted)
			{
				Quest.QuestStatus = EQuestStatus::InProgress;
				if (GEngine)
				{
					FString Message = FString::Printf(TEXT("Quest Accepted: %s"), *Quest.QuestName);
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Message);
				}
				return true;
			}
		}
	}
	return false;
}


bool UQuestManager::CompleteQuest(int32 QuestID)
{
	for (FQuest& Quest : PlayerQuests)
	{
		if (Quest.QuestID == QuestID)
		{
			if (Quest.QuestStatus == EQuestStatus::InProgress)
			{
				Quest.QuestStatus = EQuestStatus::Completed;
				if (GEngine)
				{
					FString Message = FString::Printf(TEXT("Quest Completed: %s"), *Quest.QuestName);
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);
				}
				return true;
			}
		}
	}
	return false;
}




UQuestManager::UQuestManager()
{

	// TODO initializare questuri de baza

}




/*
QuestManager::~QuestManager()
{

}
*/