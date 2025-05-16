// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeSkillsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMORPG_API ULifeSkillsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULifeSkillsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Life skills:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life Skills")
	int32 Gathering;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life Skills")
	int32 Lumbering;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life Skills")
	int32 Processing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life Skills")
	int32 Fishing;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life Skills")
	int32 Crafting;

	// Functii pentru a creste LifeSKills:
	UFUNCTION(BlueprintCallable, Category = "Life Skills")
	void IncreaseGathering(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Life Skills")
	void IncreaseLumbering(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Life Skills")
	void IncreaseProcessing(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Life Skills")
	void IncreaseFishing(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Life Skills")
	void IncreaseCrafting(int32 Amount);

	
};
