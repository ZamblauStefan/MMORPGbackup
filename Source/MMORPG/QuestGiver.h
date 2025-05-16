// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Quest_UStruct.h"
#include "QuestGiver.generated.h"

UCLASS()
class MMORPG_API AQuestGiver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestGiver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	// Questsuri disponibile de oferit de acest NPC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FQuest> OfferedQuests;


	// Functie care va oferi questurile catre un jucator (poate fi apelata dintr-un UI sau in momentul apropierii)
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void OfferQuestToPlayer(class AActor* PlayerActor);

	
};
