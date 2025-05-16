// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatTypes.h"
#include "BuffTypes.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FStatModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatTypes Stat;

	// + pentru buff, - pentru debuff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount;

};

USTRUCT(BlueprintType)
struct FBuff
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BuffName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FStatModifier> Modifiers;

	// durata buff-ului in secunde
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0.f;

};

USTRUCT(BlueprintType)
struct FActiveBuff
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBuff Definition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle ExpirationHandle;
};