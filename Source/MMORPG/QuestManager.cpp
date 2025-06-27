#include "QuestManager.h"
#include "Engine/Engine.h"

UQuestManager::UQuestManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UQuestManager::BeginPlay()
{
	Super::BeginPlay();
	InitializeMainQuests();
    InitializeBlacksmithQuests();
}


void UQuestManager::InitializeMainQuests()
{
    MainQuests.Empty();

    FQuest Q1;
    Q1.QuestID = 1;
    Q1.QuestName = "Leave the Village";
    Q1.QuestDescription = "Paraseste satul natal.";
    Q1.QuestExperienceReward = 20;
    Q1.QuestStatus = EQuestStatus::InProgress;

    FQuest Q2;
    Q2.QuestID = 2;
    Q2.QuestName = "Defeat 3 Enemies";
    Q2.QuestDescription = "Invinge 3 inamici.";
    Q2.QuestKillsNeeded = 3;
    Q2.QuestExperienceReward = 20;
    Q2.QuestStatus = EQuestStatus::NotStarted;
  

    FQuest Q3;
    Q3.QuestID = 3;
    Q3.QuestName = "Reach Level 5";
    Q3.QuestDescription = "Ajungi la nivelul 5.";
    Q3.QuestExperienceReward = 20;
    Q3.QuestStatus = EQuestStatus::NotStarted;

    // Q4 si Q5 temporar goale
    FQuest Q4, Q5;

    MainQuests = { Q1, Q2, Q3, Q4, Q5 };
}

void UQuestManager::InitializeBlacksmithQuests()
{
    SideQuests_Blacksmith.Empty();

    FQuest Q1;
    Q1.QuestID = 100;
    Q1.QuestName = "Vorbeste cu fierarul";
    Q1.QuestDescription = "Talk to Blacksmith!";
    Q1.QuestStatus = EQuestStatus::InProgress;

    FQuest Q2;
    Q2.QuestID = 101;
    Q2.QuestName = "Adu 3 lemne";
    Q2.QuestDescription = "Bring 3 logs to Blacksmith.";
    Q2.QuestStatus = EQuestStatus::NotStarted;

    FQuest Q3;
    Q3.QuestID = 102;
    Q3.QuestName = "Adu 1 fier";
    Q3.QuestDescription = "Bring Iron to Blacksmith.";
    Q3.QuestStatus = EQuestStatus::NotStarted;

    SideQuests_Blacksmith = { Q1, Q2, Q3 };
}

void UQuestManager::CompleteQuest(int32 QuestID, bool bIsMain)
{
    TArray<FQuest>& TargetList = bIsMain ? MainQuests : SideQuests_Blacksmith;

    for (FQuest& Quest : TargetList)
    {
        if (Quest.QuestID == QuestID  && Quest.QuestStatus == EQuestStatus::InProgress)
        {
            Quest.QuestStatus = EQuestStatus::Completed;
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Quest completat: %s"), *Quest.QuestName));

            break;
        }
    }


    for (FQuest& Quest : TargetList)
    {
        if (Quest.QuestStatus == EQuestStatus::NotStarted)
        {
            Quest.QuestStatus = EQuestStatus::InProgress;
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("Quest nou activat: %s"), *Quest.QuestName));


            UE_LOG(LogTemp, Warning, TEXT("QUEST UPDATE"));
            for (FQuest& QuestT : TargetList)
            {
                UE_LOG(LogTemp, Warning, TEXT("ID: %d | Status: %d"), QuestT.QuestID, (int32)QuestT.QuestStatus);
            }

            break;
        }
    }
}

FQuest UQuestManager::GetActiveQuest()
{
    for (FQuest& Quest : MainQuests)
    {
        if (Quest.QuestStatus == EQuestStatus::InProgress)
        {
            return Quest;
        }
    }
    return FQuest();    
}
FQuest UQuestManager::GetBlacksmithActiveQuest()
{
    for (FQuest& Quest : SideQuests_Blacksmith)
    {
        if (Quest.QuestStatus == EQuestStatus::InProgress)
        {
            return Quest;
        }
    }
    return FQuest();
}


int32 UQuestManager::GetCurrentQuestIndex() const
{
    for (int32 i = 0; i < MainQuests.Num(); ++i)
    {
        if (MainQuests[i].QuestStatus == EQuestStatus::InProgress)
        {
            return i;
        }
    }
    return -1;
}

int32 UQuestManager::GetCurrentBlacksmithQuestIndex() const
{
    for (int32 i = 0; i < SideQuests_Blacksmith.Num(); ++i)
    {
        if (SideQuests_Blacksmith[i].QuestStatus == EQuestStatus::InProgress)
        {
            return i;
        }
    }
    return -1;
}

void UQuestManager::RegisterEnemyKill()
{
    for (FQuest& Quest : MainQuests)
    {
        if (Quest.QuestStatus == EQuestStatus::InProgress && Quest.QuestKillsNeeded > 0)
        {
            Quest.QuestCurrentKills++;

            UE_LOG(LogTemp, Warning, TEXT("Quest kill: %d"), Quest.QuestCurrentKills);

            if (Quest.QuestCurrentKills >= Quest.QuestKillsNeeded)
            {
                CompleteQuest(Quest.QuestID, true);
            }

            break;
        }
    }
}



