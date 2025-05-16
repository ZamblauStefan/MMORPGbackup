// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGiver.h"
#include "Engine/Engine.h"
#include "MyGameInstance.h" // Clasa de GameIntance


// Sets default values
AQuestGiver::AQuestGiver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FQuest ExampleQuest(1, TEXT("Primul Quest"), TEXT("Vorbeste cu NPC 1."));
	OfferedQuests.Add(ExampleQuest);

}


void AQuestGiver::OfferQuestToPlayer(AActor* PlayerActor)
{
	if (PlayerActor)
	{
		// Pre-requisits: jucatorul are o instanta a clasei QuestManager (poate fi stocat in GameInstance)
		UMyGameInstance* GI = Cast<UMyGameInstance>(PlayerActor->GetWorld() -> GetGameInstance());
		if (GI && GI->QuestManager)
		{
			for (const FQuest& Quest : OfferedQuests)
			{
				GI->QuestManager->AddQuest(Quest);
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("QuestManager not found in GameInstance."));
			}
		}
	}
}



// Called when the game starts or when spawned
void AQuestGiver::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQuestGiver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

