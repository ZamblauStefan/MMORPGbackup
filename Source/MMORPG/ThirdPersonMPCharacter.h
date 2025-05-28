// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "LifeSkillsComponent.h"
#include "Logging/LogMacros.h"
#include "Blueprint/UserWidget.h"
#include "StatTypes.h"
#include "BuffTypes.h"
#include "BasicHUD.h"
#include "InteractionInterface.h"
#include "ThirdPersonMPCharacter.generated.h"


#define STAT_BIT(Stat) (1u << static_cast<uint8>(EStatTypes::Stat))

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AMainHUD;
class AItemPickup;
class UInventoryPanel;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);


USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.f)
	{

	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS(config = Game)
class MMORPG_API AThirdPersonMPCharacter : public ACharacter
{
	GENERATED_BODY()


	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/* Inventory-ul caracterului */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComp;


	/* Questurile asociate caracterului */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	class UQuestManager* QuestManager;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/* Fire1 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Fire1Action;

	// toggle Character Details Window
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleDetailsAction;

	// toggle inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleInventoryAction;

	// Interact
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	// Show/Hide mouse
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MouseVisibility;

	// BasicAttack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BasicAttack;

	UPROPERTY()
	UInventoryPanel* InventoryPanel;

public:
	// Sets default values for this character's properties
	AThirdPersonMPCharacter();

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// inventory
	UInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UInventoryPanel* GetInventoryPanel() const;

	void SetupInventoryConnection();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	float TurnRateGamepad;

	UFUNCTION()
	void AddBuff(const FBuff& NewBuff);
	UFUNCTION()
	void RemoveBuff(const FName BuffName);
	UFUNCTION()
	void FlushDirtyStats();

	// show mouse - toggle
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	bool bIsMouseVisible = false;
	UFUNCTION()
	void ToggleMouseVisibility();

	
	///////////////////////////////////////////////////
	// Combat System
	// 
	// Sword Attack Anim Montage
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* SwordAttackMontage;


	UFUNCTION(BlueprintCallable, Category = "Combat")
	void MeleeAttack();

	// Combat System
	///////////////////////////////////////////////////
protected:

	// lista buffs si debuffs
	UPROPERTY()
	TArray<FActiveBuff> ActiveBuffs;

	// Bitmask pentru dirty flags
	uint32 DirtyFlags = 0u;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void NotifyControllerChanged() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Called for Character details window toggle*/
	void ToggleCharacterDetails();
	// toggle Inventory
	UFUNCTION()
	void ToggleInventory();

	//=====================
	// interaction
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY()
	AMainHUD* HUD;

	float InteractionCheckFrequency;

	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	UFUNCTION()
	void BeginInteract();
	UFUNCTION()
	void EndInteract();
	UFUNCTION()
	void Interact();

	virtual void Tick(float DeltaSeconds) override;

public:	

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PickupItem(AItemPickup* ItemPickup);
	void Server_PickupItem_Implementation(AItemPickup* ItemPickup);
	bool Server_PickupItem_Validate(AItemPickup* ItemPickup) { return true; }


	// interaction
	//=====================

	/* Returns CameraBoom subobject */
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/* Returns FollowCamera subobject */
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


protected:

	/* Base stats of any character (initial values) */
	UPROPERTY(ReplicatedUsing = OnRep_BaseMaxHealth, EditAnywhere, BlueprintReadWrite, Category = "Health")
	float BaseMaxHealth;
	UPROPERTY(ReplicatedUsing = OnRep_BaseHealthRegen, EditAnywhere, BlueprintReadWrite, Category = "Regen")
	float BaseHealthRegen;
	UPROPERTY(ReplicatedUsing = OnRep_BaseHealthRegenInterval, EditAnywhere, BlueprintReadWrite, Category = "Health")
	float BaseHealthRegenInterval;
	UPROPERTY(ReplicatedUsing = OnRep_BaseMaxMana, EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float BaseMaxMana;
	UPROPERTY(ReplicatedUsing = OnRep_BaseManaRegen, EditAnywhere, BlueprintReadWrite, Category = "Regen")
	float BaseManaRegen;
	UPROPERTY(ReplicatedUsing = OnRep_BaseManaRegenInterval, EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float BaseManaRegenInterval;
	UPROPERTY(ReplicatedUsing = OnRep_BaseMaxSkillStamina, EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float BaseMaxSkillStamina;
	UPROPERTY(ReplicatedUsing = OnRep_BaseSkillStaminaRegen, EditAnywhere, BlueprintReadWrite, Category = "Regen")
	float BaseSkillStaminaRegen;
	UPROPERTY(ReplicatedUsing = OnRep_BaseSkillStaminaRegenInterval, EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float BaseSkillStaminaRegenInterval;
	UPROPERTY(ReplicatedUsing = OnRep_BaseMaxMovementStamina, EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float BaseMaxMovementStamina;
	UPROPERTY(ReplicatedUsing = OnRep_BaseMovementStaminaRegen, EditAnywhere, BlueprintReadWrite, Category = "Regen")
	float BaseMovementStaminaRegen;
	UPROPERTY(ReplicatedUsing = OnRep_BaseMovementStaminaRegenInterval, EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float BaseMovementStaminaRegenInterval;
	UPROPERTY(ReplicatedUsing = OnRep_BasePhysicalAttack, EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float BasePhysicalAttack;
	UPROPERTY(ReplicatedUsing = OnRep_BaseMagicalAttack, EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float BaseMagicalAttack;
	UPROPERTY(ReplicatedUsing = OnRep_BasePhysicalDefense, EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float BasePhysicalDefense;
	UPROPERTY(ReplicatedUsing = OnRep_BaseMagicalAttack, EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float BaseMagicalDefense;
	UPROPERTY(ReplicatedUsing = OnRep_BaseStrength, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BaseStrength;
	UPROPERTY(ReplicatedUsing = OnRep_BaseConstitution, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BaseConstitution;
	UPROPERTY(ReplicatedUsing = OnRep_BaseDexterity, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BaseDexterity;
	UPROPERTY(ReplicatedUsing = OnRep_BaseIntelligence, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BaseIntelligence;
	UPROPERTY(ReplicatedUsing = OnRep_BaseWisdom, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BaseWisdom;
	UPROPERTY(ReplicatedUsing = OnRep_BaseLuck, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float BaseLuck;


	/* The player's maximum health. This is the highest value of their healt can be.
	 This value is a value of the player's health, which starts at when spawned */
	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	/* The player's current health. When reduced to 0, they are considered dead. */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;
	UPROPERTY(ReplicatedUsing = OnRep_HealthRegen, EditAnywhere, BlueprintReadWrite, Category = "Regen")
	float HealthRegen;
	UPROPERTY(ReplicatedUsing = OnRep_HealthRegenInterval, EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthRegenInterval;
	FTimerHandle HealthRegenTimerHandle;

	/* The player's physical attack . Damage output. */
	UPROPERTY(ReplicatedUsing = OnRep_PhysicalAttack, EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float PhysicalAttack;

	/* The player's  attack . Damage output. */
	UPROPERTY(ReplicatedUsing = OnRep_MagicalAttack, EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float MagicalAttack;

	/* The player's defense . Decrease damage taken. */
	UPROPERTY(ReplicatedUsing = OnRep_PhysicalDefense, EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float PhysicalDefense;

	/* The player's magical defense . Decrease damage taken. */
	UPROPERTY(ReplicatedUsing = OnRep_MagicalDefense, EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float MagicalDefense;

	/* The player's  strength (STR). Adds to Attack */
	UPROPERTY(ReplicatedUsing = OnRep_Strength, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Strength;

	/* The player's  constitution (CON). Adds to Constitution */
	UPROPERTY(ReplicatedUsing = OnRep_Constitution, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Constitution;

	/* The player's  dexterity (DEX). Adds to Attack, hit rate, accuracy */
	UPROPERTY(ReplicatedUsing = OnRep_Dexterity, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Dexterity;

	/* The player's  intelligence (INT). Adds to Magic Attack */
	UPROPERTY(ReplicatedUsing = OnRep_Intelligence, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Intelligence;

	/* The player's curent wisdom (WIS). Adds to Magic Defense */
	UPROPERTY(ReplicatedUsing = OnRep_Wisdom, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Wisdom;

	/* The player's luck (LUC). Adds to critical chance and critical damage */
	UPROPERTY(ReplicatedUsing = OnRep_Luck, EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Luck;

	/* The player's MaxMana (MP). Mana is used to cast skills */
	UPROPERTY(ReplicatedUsing = OnRep_MaxMana, EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float MaxMana;

	/* The player's curent Mana (MP). Mana is used to cast skills */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentMana, EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float CurrentMana;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_ManaRegen, BlueprintReadWrite, Category = "Regen")
	float ManaRegen;
	UPROPERTY(ReplicatedUsing = OnRep_ManaRegenInterval, EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float ManaRegenInterval;
	/** Handle-ul timer-ului de regen */
	FTimerHandle ManaRegenTimerHandle;

	/* The player's Max Skill Stamina (SP). Skill Stamina is used by abilities */
	UPROPERTY(ReplicatedUsing = OnRep_MaxSkillStamina, EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxSkillStamina;
	/* The player's curent Skill Stamina (SP). Skill Stamina is used by abilities */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentSkillStamina, EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentSkillStamina;
	UPROPERTY(ReplicatedUsing = OnRep_SkillStaminaRegen, EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float SkillStaminaRegen;
	UPROPERTY(ReplicatedUsing = OnRep_SkillStaminaRegenInterval, EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float SkillStaminaRegenInterval;
	FTimerHandle SkillStaminaRegenTimerHandle;

	/* The player's Max Movement Stamina (SP). Movement Stamina is used by movement abilities */
	UPROPERTY(ReplicatedUsing = OnRep_MaxMovementStamina, EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxMovementStamina;
	/* The player's curent Movement Stamina (SP). Movement Stamina is used by movement abilities */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentMovementStamina, EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentMovementStamina;
	UPROPERTY(ReplicatedUsing = OnRep_MovementStaminaRegen, EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MovementStaminaRegen;
	UPROPERTY(ReplicatedUsing = OnRep_MovementStaminaRegenInterval, EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MovementStaminaRegenInterval;
	FTimerHandle MovementStaminaRegenTimerHandle;

	// Componenta Life Skills
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Life Skills")
	ULifeSkillsComponent* LifeSkillsComp;

	// Current experience and level
	UPROPERTY(ReplicatedUsing = OnRep_Level, BlueprintReadWrite, Category = "Progression")
	int32 Level = 1;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Progression")
	int32 AvailableStatPoints = 0;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentEXP, BlueprintReadWrite, Category = "Progression")
	int32 CurrentEXP = 0;
	UPROPERTY(ReplicatedUsing = OnRep_EXPToNextLevel, BlueprintReadWrite, Category = "Progression")
	int32 EXPToNextLevel = 100;

	UFUNCTION()
	void OnRep_CurrentEXP();
	UFUNCTION()
	void OnRep_EXPToNextLevel();
	UFUNCTION()
	void OnRep_Level();


	/* RepNotify for Base Stats*/
	UFUNCTION()	void OnRep_BaseMaxHealth();
	UFUNCTION()	void OnRep_BaseHealthRegen();
	UFUNCTION()	void OnRep_BaseHealthRegenInterval();
	UFUNCTION()	void OnRep_BaseMaxMana();
	UFUNCTION()	void OnRep_BaseManaRegen();
	UFUNCTION()	void OnRep_BaseManaRegenInterval();
	UFUNCTION()	void OnRep_BaseMaxSkillStamina();
	UFUNCTION()	void OnRep_BaseSkillStaminaRegen();
	UFUNCTION()	void OnRep_BaseSkillStaminaRegenInterval();
	UFUNCTION()	void OnRep_BaseMaxMovementStamina();
	UFUNCTION()	void OnRep_BaseMovementStaminaRegen();
	UFUNCTION()	void OnRep_BaseMovementStaminaRegenInterval();
	UFUNCTION()	void OnRep_BasePhysicalAttack();
	UFUNCTION()	void OnRep_BaseMagicalAttack();
	UFUNCTION()	void OnRep_BasePhysicalDefense();
	UFUNCTION()	void OnRep_BaseMagicalDefense();
	UFUNCTION()	void OnRep_BaseStrength();
	UFUNCTION()	void OnRep_BaseConstitution();
	UFUNCTION()	void OnRep_BaseDexterity();
	UFUNCTION()	void OnRep_BaseIntelligence();
	UFUNCTION()	void OnRep_BaseWisdom();
	UFUNCTION()	void OnRep_BaseLuck();


	/* RepNotify for changes made to max health */
	UFUNCTION()
	void OnRep_MaxHealth();

	/* RepNotify for changes made to current health */
	UFUNCTION()
	void OnRep_CurrentHealth();

	/* RepNotify for changes made to health regen */
	UFUNCTION()
	void OnRep_HealthRegen();

	/* RepNotify for changes made to health regen interval */
	UFUNCTION()
	void OnRep_HealthRegenInterval();

	/* Functie apelata de timer pentru health regen */
	UFUNCTION()
	void RegenHealth();

	/* RepNotify for changes made to max mana */
	UFUNCTION()
	void OnRep_MaxMana();

	/* RepNotify for changes made to current mana */
	UFUNCTION()
	void OnRep_CurrentMana();

	/* RepNotify for changes made to mana regen */
	UFUNCTION()
	void OnRep_ManaRegen();

	/* RepNotify for changes made to mana regen interval */
	UFUNCTION()
	void OnRep_ManaRegenInterval();

	/* Functie apelata de timer pentru health regen */
	UFUNCTION()
	void RegenMana();

	/* RepNotify for changes made to max SkillStamina */
	UFUNCTION()
	void OnRep_MaxSkillStamina();

	/* RepNotify for changes made to current SkillStamina */
	UFUNCTION()
	void OnRep_CurrentSkillStamina();

	/* RepNotify for changes made to SkillStamina regen */
	UFUNCTION()
	void OnRep_SkillStaminaRegen();

	/* RepNotify for changes made to SkillStamina regen interval */
	UFUNCTION()
	void OnRep_SkillStaminaRegenInterval();

	/* Functie apelata de timer pentru skill stamina regen */
	UFUNCTION()
	void RegenSkillStamina();

	/* RepNotify for changes made to max MovementStamina */
	UFUNCTION()
	void OnRep_MaxMovementStamina();

	/* RepNotify for changes made to current MovementStamina */
	UFUNCTION()
	void OnRep_CurrentMovementStamina();

	/* RepNotify for changes made to MovementStamina regen */
	UFUNCTION()
	void OnRep_MovementStaminaRegen();

	/* RepNotify for changes made to MovementStamina regen interval */
	UFUNCTION()
	void OnRep_MovementStaminaRegenInterval();

	/* Functie apelata de timer pentru movement stamina regen */
	UFUNCTION()
	void RegenMovementStamina();

	/* RepNotify for changes made to Attributes(STR, DEX, REC */
	UFUNCTION()
	void OnRep_Strength();

	UFUNCTION()
	void OnRep_Constitution();

	UFUNCTION()
	void OnRep_Dexterity();

	UFUNCTION()
	void OnRep_Intelligence();

	UFUNCTION()
	void OnRep_Wisdom();

	UFUNCTION()
	void OnRep_Luck();

	/* Rep Notify for changes made to attack / defense */
	UFUNCTION()
	void OnRep_PhysicalAttack();
	UFUNCTION()
	void OnRep_PhysicalDefense();
	UFUNCTION()
	void OnRep_MagicalAttack();
	UFUNCTION()
	void OnRep_MagicalDefense();


	UFUNCTION()
	void OnPhysicalAttackUpdate();
	UFUNCTION()
	void OnPhysicalDefenseUpdate();
	UFUNCTION()
	void OnMagicalAttackUpdate();
	UFUNCTION()
	void OnMagicalDefenseUpdate();


	// Base Stats Update
	void OnBaseMaxHealthUpdate();
	void OnBaseHealthRegenUpdate();
	void OnBaseHealthRegenIntervalUpdate();
	void OnBaseMaxManaUpdate();
	void OnBaseManaRegenUpdate();
	void OnBaseManaRegenIntervalUpdate();
	void OnBaseMaxSkillStaminaUpdate();
	void OnBaseSkillStaminaRegenUpdate();
	void OnBaseSkillStaminaRegenIntervalUpdate();
	void OnBaseMaxMovementStaminaUpdate();
	void OnBaseMovementStaminaRegenUpdate();
	void OnBaseMovementStaminaRegenIntervalUpdate();
	void OnBasePhysicalAttackUpdate();
	void OnBaseMagicalAttackUpdate();
	void OnBasePhysicalDefenseUpdate();
	void OnBaseMagicalDefenseUpdate();
	void OnBaseStrengthUpdate();
	void OnBaseConstitutionUpdate();
	void OnBaseDexterityUpdate();
	void OnBaseIntelligenceUpdate();
	void OnBaseWisdomUpdate();
	void OnBaseLuckUpdate();

	/* Response to max health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify */
	void OnMaxHealthUpdate();
	/* Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify */
	void OnCurrentHealthUpdate();
	void OnHealthRegenUpdate();
	void OnHealthRegenIntervalUpdate();
	/* Response to max mana being updated. Called on the server immediately after modification, and on clients in response to a RepNotify */
	void OnMaxManaUpdate();
	/* Response to mana being updated. Called on the server immediately after modification, and on clients in response to a RepNotify */
	void OnCurrentManaUpdate();
	void OnManaRegenUpdate();
	void OnManaRegenIntervalUpdate();
	/* Response to Max SkillStamina being updated. Called on the server immediately after modification, and on clients in response to a RepNotify */
	void OnMaxSkillStaminaUpdate();
	/* Response to SkillStamina being updated. Called on the server immediately after modification, and on clients in response to a RepNotify */
	void OnCurrentSkillStaminaUpdate();
	void OnSkillStaminaRegenUpdate();
	void OnSkillStaminaRegenIntervalUpdate();
	/* Response to Max MovementStamina being updated. Called on the server immediately after modification, and on clients in response to a RepNotify */
	void OnMaxMovementStaminaUpdate();
	/* Response to MovementStamina being updated. Called on the server immediately after modification, and on clients in response to a RepNotify */
	void OnCurrentMovementStaminaUpdate();
	void OnMovementStaminaRegenUpdate();
	void OnMovementStaminaRegenIntervalUpdate();

	/* Response to Stats being updated. Called on the server immediately after modification, and on clients in response to a RepNotify */
	void OnStrengthUpdate();
	void OnDexterityUpdate();
	void OnConstitutionUpdate();
	void OnIntelligenceUpdate();
	void OnWisdomUpdate();
	void OnLuckUpdate();

public:
	/* Getters for Base Stats*/
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetBaseMaxHealth() const { return BaseMaxHealth; }
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetBaseHealthRegen() const { return BaseHealthRegen; }
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetBaseHealthRegenInterval() const { return BaseHealthRegenInterval; }
	UFUNCTION(BlueprintPure, Category = "Mana")
	FORCEINLINE float GetBaseMaxMana() const { return BaseMaxMana; }
	UFUNCTION(BlueprintPure, Category = "Regen")
	FORCEINLINE float GetBaseManaRegen() const { return BaseManaRegen; }
	UFUNCTION(BlueprintPure, Category = "Mana")
	FORCEINLINE float GetBaseManaRegenInterval() const { return BaseManaRegenInterval; }
	UFUNCTION(BlueprintPure, Category = "Stamina")
	FORCEINLINE float GetBaseMaxSkillStamina() const { return BaseMaxSkillStamina; }
	UFUNCTION(BlueprintPure, Category = "Regen")
	FORCEINLINE float GetBaseSkillStaminaRegen() const { return BaseSkillStaminaRegen; }
	UFUNCTION(BlueprintPure, Category = "Stamina")
	FORCEINLINE float GetBaseSkillStaminaRegenInterval() const { return BaseSkillStaminaRegenInterval; }
	UFUNCTION(BlueprintPure, Category = "Stamina")
	FORCEINLINE float GetBaseMaxMovementStamina() const { return BaseMaxMovementStamina; }
	UFUNCTION(BlueprintPure, Category = "Regen")
	FORCEINLINE float GetBaseMovementStaminaRegen() const { return BaseMovementStaminaRegen; }
	UFUNCTION(BlueprintPure, Category = "Stamina")
	FORCEINLINE float GetBaseMovementStaminaRegenInterval() const { return BaseMovementStaminaRegenInterval; }
	UFUNCTION(BlueprintPure, Category = "Attack")
	FORCEINLINE float GetBasePhysicalAttack() const { return BasePhysicalAttack; }
	UFUNCTION(BlueprintPure, Category = "Attack")
	FORCEINLINE float GetBaseMagicalAttack() const { return BaseMagicalAttack; }
	UFUNCTION(BlueprintPure, Category = "Defense")
	FORCEINLINE float GetBasePhysicalDefense() const { return BasePhysicalDefense; }
	UFUNCTION(BlueprintPure, Category = "Defense")
	FORCEINLINE float GetBaseMagicalDefense() const { return BaseMagicalDefense; }
	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetBaseStrength() const { return BaseStrength; }
	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetBaseConstitution() const { return BaseConstitution; }
	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetBaseDexterity() const { return BaseDexterity; }
	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetBaseIntelligence() const { return BaseIntelligence; }
	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetBaseWisdom() const { return BaseWisdom; }
	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetBaseLuck() const { return BaseLuck; }


	/* Getter for MaxHealth */
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	/* Getter for Current Health */
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	/* Getter for Health Regen */
	UFUNCTION(BlueprintPure, Category = "Regen")
	FORCEINLINE float GetHealthRegen() const { return HealthRegen; }
	/* Getter for Health Regen Interval */
	UFUNCTION(BlueprintPure, Category = "Health")
	FORCEINLINE float GetHealthRegenInterval() const { return HealthRegenInterval; }
	/* Getter for MaxMana */
	UFUNCTION(BlueprintPure, Category = "MaxMana")
	FORCEINLINE float GetMaxMana() const { return MaxMana; }
	/* Getter for Current Mana */
	UFUNCTION(BlueprintPure, Category = "CurrentMana")
	FORCEINLINE float GetCurrentMana() const { return CurrentMana; }
	/* Getter for Mana Regen */
	UFUNCTION(BlueprintPure, Category = "ManaRegen")
	FORCEINLINE float GetManathRegen() const { return ManaRegen; }
	/* Getter for Mana Regen Interval */
	UFUNCTION(BlueprintPure, Category = "ManaRegenInterval")
	FORCEINLINE float GetManathRegenInterval() const { return ManaRegenInterval; }
	/* Getter for MaxSkillStamina */
	UFUNCTION(BlueprintPure, Category = "MaxSkillStamina")
	FORCEINLINE float GetMaxSkillStamina() const { return MaxSkillStamina; }
	/* Getter for Current SkillStamina */
	UFUNCTION(BlueprintPure, Category = "CurrentSkillStamina")
	FORCEINLINE float GetCurrentSkillStamina() const { return CurrentSkillStamina; }
	/* Getter for Skill Stamina Regen */
	UFUNCTION(BlueprintPure, Category = "SkillStaminaRegen")
	FORCEINLINE float GetSkillStaminaRegen() const { return SkillStaminaRegen; }
	/* Getter for Skill Stamina Regen Interval */
	UFUNCTION(BlueprintPure, Category = "SkillStaminaRegenInterval")
	FORCEINLINE float GetSkillStaminaRegenInterval() const { return SkillStaminaRegenInterval; }
	/* Getter for MaxMovementStamina */
	UFUNCTION(BlueprintPure, Category = "MaxMovementStamina")
	FORCEINLINE float GetMaxMovementStamina() const { return MaxMovementStamina; }
	/* Getter for Current MovementStamina */
	UFUNCTION(BlueprintPure, Category = "CurrentMovementStamina")
	FORCEINLINE float GetCurrentMovementStamina() const { return CurrentMovementStamina; }
	/* Getter for MovementStamina Regen */
	UFUNCTION(BlueprintPure, Category = "MovementStaminaRegen")
	FORCEINLINE float GetMovementStaminaRegen() const { return MovementStaminaRegen; }
	/* Getter for Skill Stamina Regen Interval */
	UFUNCTION(BlueprintPure, Category = "MovementStaminaRegenInterval")
	FORCEINLINE float GetMovementStaminaRegenInterval() const { return MovementStaminaRegenInterval; }
	/* Getters for Stats */
	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetStrength() const { return Strength; }
	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetConstitution() const { return Constitution; }
	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetDexterity() const { return Dexterity; }
	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetIntelligence() const { return Intelligence; }
	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetWisdom() const { return Wisdom; }
	UFUNCTION(BlueprintPure, Category = "Stats")
	FORCEINLINE float GetLuck() const { return Luck; }
	UFUNCTION(BlueprintPure, Category = "Attack")
	FORCEINLINE float GetPhysicalAttack() const { return PhysicalAttack; }
	UFUNCTION(BlueprintPure, Category = "Attack")
	FORCEINLINE float GetMagicalAttack() const { return MagicalAttack; }
	UFUNCTION(BlueprintPure, Category = "Defense")
	FORCEINLINE float GetPhysicalDefense() const { return PhysicalDefense; }
	UFUNCTION(BlueprintPure, Category = "Defense")
	FORCEINLINE float GetMagicalDefense() const { return MagicalDefense; }

	/* Setter for Player Stats and Attributes. Clamps the value between  0 ad Max where needed and calls On "X" Update. 
	Should only be called on the server. */
	// Setter for base Stats
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetBaseMaxHealth(float InBaseHealthValue);
	UFUNCTION(BlueprintCallable, Category = "Regen")
	void SetBaseHealthRegen(float InBaseHealthRegenValue);
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetBaseHealthRegenInterval(float InBaseHealthRegenIntervalValue);

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void SetBaseMaxMana(float InBaseManaValue);
	UFUNCTION(BlueprintCallable, Category = "Regen")
	void SetBaseManaRegen(float InBaseManaRegenValue);
	UFUNCTION(BlueprintCallable, Category = "Mana")
	void SetBaseManaRegenInterval(float InBaseManaRegenIntervalValue);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetBaseMaxSkillStamina(float InBaseSkillStaminaValue);
	UFUNCTION(BlueprintCallable, Category = "Regen")
	void SetBaseSkillStaminaRegen(float InBaseSkillStaminaRegenValue);
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetBaseSkillStaminaRegenInterval(float InBaseSkillStaminaRegenValue);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetBaseMaxMovementStamina(float InBaseMovementStaminaValue);
	UFUNCTION(BlueprintCallable, Category = "Regen")
	void SetBaseMovementStaminaRegen(float InBaseMovementStaminaRegenValue);
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetBaseMovementStaminaRegenInterval(float InBaseMovementStaminaRegenIntervalValue);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetBaseStrength(int newBaseStrength);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetBaseConstitution(int newBaseConstitution);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetBaseDexterity(int newBaseDexterity);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetBaseIntelligence(int newBaseIntelligence);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetBaseWisdom(int newBaseWisdom);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetBaseLuck(int newBaseLuck);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetBasePhysicalAttack(float newBasePhysicalAttack);
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetBaseMagicalAttack(float newBaseMagicalAttack);
	UFUNCTION(BlueprintCallable, Category = "Defense")
	void SetBasePhysicalDefense(float newBasePhysicalDefense);
	UFUNCTION(BlueprintCallable, Category = "Defense")
	void SetBaseMagicalDefense(float newBaseMagicalDefense);

	// Other stats
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetMaxHealth(float InHealthValue);
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealth(float InHealthValue);
	UFUNCTION(BlueprintCallable, Category = "Regen")
	void SetHealthRegen(float InHealthRegenValue);
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealthRegenInterval(float InHealthRegenIntervalValue);

	UFUNCTION(BlueprintCallable, Category = "Mana")
	void SetMaxMana(float InManaValue);
	UFUNCTION(BlueprintCallable, Category = "Mana")
	void SetCurrentMana(float InManaValue);
	UFUNCTION(BlueprintCallable, Category = "Regen")
	void SetManaRegen(float InManaRegenValue);
	UFUNCTION(BlueprintCallable, Category = "Mana")
	void SetManaRegenInterval(float InManaRegenIntervalValue);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetMaxSkillStamina(float InSkillStaminaValue);
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetCurrentSkillStamina(float InSkillStaminaValue);
	UFUNCTION(BlueprintCallable, Category = "Regen")
	void SetSkillStaminaRegen(float InSkillStaminaRegenValue);
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetSkillStaminaRegenInterval(float InSkillStaminaRegenValue);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetMaxMovementStamina(float InMovementStaminaValue);
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetCurrentMovementStamina(float InMovementStaminaValue);
	UFUNCTION(BlueprintCallable, Category = "Regen")
	void SetMovementStaminaRegen(float InMovementStaminaRegenValue);
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void SetMovementStaminaRegenInterval(float InMovementStaminaRegenIntervalValue);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetStrength(int newStrength);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetConstitution(int newConstitution);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetDexterity(int newDexterity);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetIntelligence(int newIntelligence);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetWisdom(int newWisdom);
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetLuck(int newLuck);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetPhysicalAttack(float newPhysicalAttack);
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetMagicalAttack(float newMagicalAttack);
	UFUNCTION(BlueprintCallable, Category = "Defense")
	void SetPhysicalDefense(float newPhysicalDefense);
	UFUNCTION(BlueprintCallable, Category = "Defense")
	void SetMagicalDefense(float newMagicalDefense);


	// Functii pentru update stats
	// health
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateMaxHealth();
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateCurrentHealth();
	UFUNCTION(BlueprintCallable, Category = "Regen")
	void UpdateHealthRegen();
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealthRegenInterval();
	// mana
	UFUNCTION(BlueprintCallable, Category = "Mana")
	void UpdateMaxMana();
	UFUNCTION(BlueprintCallable, Category = "Mana")
	void UpdateCurrentMana();
	UFUNCTION(BlueprintCallable, Category = "Regen")
	void UpdateManaRegen();
	UFUNCTION(BlueprintCallable, Category = "Mana")
	void UpdateManaRegenInterval();
	// skill stamina
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UpdateMaxSkillStamina();
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UpdateCurrentSkillStamina();
	UFUNCTION(BlueprintCallable, Category = "Regen")
	void UpdateSkillStaminaRegen();
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UpdateSkillStaminaRegenInterval();
	// movement stamina
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UpdateMaxMovementStamina();
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UpdateCurrentMovementStamina();
	UFUNCTION(BlueprintCallable, Category = "Regen")
	void UpdateMovementStaminaRegen();
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void UpdateMovementStaminaRegenInterval();
	// attack si defense
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void UpdatePhysicalAttack();
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void UpdateMagicalAttack();
	UFUNCTION(BlueprintCallable, Category = "Defense")
	void UpdatePhysicalDefense();
	UFUNCTION(BlueprintCallable, Category = "Defense")
	void UpdateMagicalDefense();
	// attributes
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateStrength();
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateConstitution();
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateDexterity();
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateIntelligence();
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateWisdom();
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateLuck();

	UFUNCTION(BlueprintCallable, Category = "Mana")
	bool ConsumeMana(float ManaCost);

	/* Event for taking damage. Overridden from APawn. */
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Progression")
	void GainEXP(int32 Amount);

	// Functie de expunere a componentei 
	FORCEINLINE ULifeSkillsComponent* GetLifeSkillsComponent() const { return LifeSkillsComp; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
	TSubclassOf<class AThirdPersonMPProjectile> ProjectileClass;

	/* Delay between shots in seconds. Used to control fire rate for test projectile, 
	 but also to prevent an overflow of server functions from binding SpawnProjectile directly to input */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	/* If true, you are in the process of firing projectiles. */
	bool bIsFiringWeapon;

	/* Function for beginning weapon(magic) fire*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartFire();

	/* Function for ending weapn(magic) fire. Once thi is called, the player can use StartFire again. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Progression")
	void LevelUp();

	void BeginPlay();

	/* Server function for spawning projectiles. */
	UFUNCTION(Server, Reliable)
	void HandleFire();

	/* A timer handle used for providing the fire rate delay in-between spawns. */
	FTimerHandle FiringTimer;




};
