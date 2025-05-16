// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::ConnectToServer(const FString& ServerIP)
{
    // serverul ruleaza pe portul 7778, iar harta este ThirdPersonMap
    FString URL = ServerIP + TEXT(":7778");
    // Cheama OpenLevel cu parametrul URL
    UGameplayStatics::OpenLevel(GetWorld(), FName("ThirdPersonMap"), true, URL);


}

 void UMyGameInstance::Init()
 {
        Super::Init();
        QuestManager = NewObject<UQuestManager>(this);

 }