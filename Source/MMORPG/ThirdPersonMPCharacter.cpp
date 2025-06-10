// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonMPCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "ThirdPersonMPProjectile.h"
#include "LifeSkillsComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h" 
#include "MyPlayerController.h"
#include "MainHUD.h"
#include "ItemBase.h"
#include "ItemPickup.h"
#include "InventoryPanel.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponBase.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);



// Sets default values
AThirdPersonMPCharacter::AThirdPersonMPCharacter()
{
	// initializare BaseStats
	BaseMaxHealth = 100.f;
	BaseHealthRegen = 5.f;
	BaseHealthRegenInterval = 10.f;
	BaseMaxMana = 100.f;
	BaseManaRegen = 5.f;
	BaseManaRegenInterval = 10.f;
	BaseMaxSkillStamina = 100.f;
	BaseSkillStaminaRegen = 5.f;
	BaseSkillStaminaRegenInterval = 10.f;
	BaseMaxMovementStamina = 100.f;
	BaseMovementStaminaRegen = 5.f;
	BaseMovementStaminaRegenInterval = 10.f;
	BasePhysicalAttack = 10.f;
	BaseMagicalAttack = 10.f;
	BasePhysicalDefense = 10.f;
	BaseMagicalDefense = 10.f;
	BaseStrength = 10;
	BaseConstitution = 10;
	BaseDexterity = 10;
	BaseIntelligence = 10;
	BaseWisdom = 10;
	BaseLuck = 10;

	// fortam replicare actor
	bReplicates = true;


	// initializare inventory component
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	// setam distanta maxima (in unitati UE) la care se va replica actorul
	const float CullRadius = 2000.f;
	SetNetCullDistanceSquared(FMath::Square(CullRadius));

	// Set size for colision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; //Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	// Create a camera boom (pulls in towards the player if there is collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance begind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller


	// Create a follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and
																				//let the boom adjust to match the controller orientation 
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 250.0f;


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	///////////////////////////////////////////////////////////////////////

	// Initialize projectile class
	ProjectileClass = AThirdPersonMPProjectile::StaticClass();

	// Initialize fire rate
	FireRate = 0.25f;

	// Crearea componentei LifeSkills
	LifeSkillsComp = CreateDefaultSubobject<ULifeSkillsComponent>(TEXT("LifeSkillsComponent"));


/*
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
*/

}

	//////////////////////////////////////////////////////////////////
	// Input
void AThirdPersonMPCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


void AThirdPersonMPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AThirdPersonMPCharacter::CustomJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AThirdPersonMPCharacter::CustomStopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThirdPersonMPCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThirdPersonMPCharacter::Look);

		// Fire1
		EnhancedInputComponent->BindAction(Fire1Action, ETriggerEvent::Started, this, &AThirdPersonMPCharacter::StartFire);
		EnhancedInputComponent->BindAction(Fire1Action, ETriggerEvent::Completed, this, &AThirdPersonMPCharacter::StopFire);

		// Toggle Character Details Window
		EnhancedInputComponent->BindAction(ToggleDetailsAction, ETriggerEvent::Started, this, &AThirdPersonMPCharacter::ToggleCharacterDetails);
		// Toggle pentru inventory
		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AThirdPersonMPCharacter::ToggleInventory);

		// Interact Binding "R" key for now
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AThirdPersonMPCharacter::BeginInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AThirdPersonMPCharacter::EndInteract);

		// Input for combat BasicAttacks (left click)
		EnhancedInputComponent->BindAction(BasicAttack, ETriggerEvent::Started, this, &AThirdPersonMPCharacter::MeleeAttack);
		// Input for mouse visibility toggle
		EnhancedInputComponent->BindAction(MouseVisibility, ETriggerEvent::Started, this, &AThirdPersonMPCharacter::ToggleMouseVisibility);



	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AThirdPersonMPCharacter::Move(const FInputActionValue& Value)
{

	if (!bCanMove) return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AThirdPersonMPCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AThirdPersonMPCharacter::ToggleCharacterDetails()
{
	if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetController()))
	{
		PC->ToggleCharacterDetails();
	}
}

void AThirdPersonMPCharacter::ToggleInventory()
{
	if (AMyPlayerController* PC = Cast<AMyPlayerController>(GetController()))
	{
		PC->ToggleInventory();
	}
}


//////////////////////////////////////////////////////////
// interact
void AThirdPersonMPCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{GetPawnViewLocation()};
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

	// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FHitResult TraceHit;

	float LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

	if(LookDirection > 0)
	{
		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());

					// verificare ca am lovit un obiect ce implementeaza InteractionInterface cu TraceHit
					AActor* HitActor = TraceHit.GetActor();
					UE_LOG(LogTemp, Warning, TEXT("Interactable detectat: %s"), *HitActor->GetName());

					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}

	NoInteractableFound();

}

void AThirdPersonMPCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	//HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();

}

void AThirdPersonMPCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		//HUD->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}

}

void AThirdPersonMPCharacter::BeginInteract()
{
	// verify nothing has changed with the interactable state since beginning interaction
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
					this,
					&AThirdPersonMPCharacter::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
	}

}

void AThirdPersonMPCharacter::EndInteract()
{

	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}

}

// inventory and interact

UInventoryComponent* AThirdPersonMPCharacter::GetInventoryComponent() const
{
	return InventoryComp;
}

void AThirdPersonMPCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);


	if (IsValid(InteractionData.CurrentInteractable))
	{
		IInteractionInterface* Interactable = Cast<IInteractionInterface>(InteractionData.CurrentInteractable);
		if (Interactable)
		{
		
			UE_LOG(LogTemp, Warning, TEXT("Interactiune detectata cu obiectul!"));
			Interactable->Interact(this);
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Obiectul nu implementeaza interfaaa de interactiune!"));
		}
		
	}

}


void AThirdPersonMPCharacter::Server_PickupItem_Implementation(AItemPickup* ItemPickup)
{
	if (ItemPickup && InventoryComp)
	{

		UE_LOG(LogTemp, Warning, TEXT("[Server_PickupItem] Am ajuns pe server, item: %s"), *ItemPickup->ItemID.ToString());

		// Cream un obiect nou pentru inventory
		UItemBase* NewItem = NewObject<UItemBase>(UItemBase::StaticClass());

		if (NewItem)
		{
			// Setam datele obiectului din lume in instanta de item
			NewItem->ItemID = ItemPickup->ItemID;
			NewItem->Quantity = ItemPickup->Quantity;

			// Incercam sa adaugam in inventory
			bool bAdded = InventoryComp->AddItem(NewItem);

			if (bAdded)
			{
				// Distrugem actorul din lume
				UE_LOG(LogTemp, Warning, TEXT("Item %s a fost adaugat in inventory!"), *NewItem->ItemID.ToString());
				ItemPickup->Destroy();
			
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Nu s-a putut adauga item-ul in inventory."));
			}
		}
	}
}






void AThirdPersonMPCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}
}



// interact
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
// Replicated Properties

void AThirdPersonMPCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate BaseStats
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseMaxHealth, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseHealthRegen, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseHealthRegenInterval, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseMaxMana, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseManaRegen, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseManaRegenInterval, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseMaxSkillStamina, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseSkillStaminaRegen, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseSkillStaminaRegenInterval, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseMaxMovementStamina, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseMovementStaminaRegen, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseMovementStaminaRegenInterval, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseStrength,COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseConstitution, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseDexterity, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseIntelligence, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseWisdom, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseLuck, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BasePhysicalAttack, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BasePhysicalDefense, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseMagicalAttack, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, BaseMagicalDefense, COND_OwnerOnly);

	// Replicate  health, mana and stamina.
	DOREPLIFETIME(AThirdPersonMPCharacter, MaxHealth);
	DOREPLIFETIME(AThirdPersonMPCharacter, CurrentHealth);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, HealthRegen, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, HealthRegenInterval, COND_OwnerOnly);

	DOREPLIFETIME(AThirdPersonMPCharacter, MaxMana);
	DOREPLIFETIME(AThirdPersonMPCharacter, CurrentMana);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, ManaRegen, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter, ManaRegenInterval, COND_OwnerOnly);

	DOREPLIFETIME(AThirdPersonMPCharacter, MaxSkillStamina);
	DOREPLIFETIME(AThirdPersonMPCharacter, CurrentSkillStamina);
	DOREPLIFETIME(AThirdPersonMPCharacter, SkillStaminaRegen);
	DOREPLIFETIME(AThirdPersonMPCharacter, SkillStaminaRegenInterval);

	DOREPLIFETIME(AThirdPersonMPCharacter, MaxMovementStamina);
	DOREPLIFETIME(AThirdPersonMPCharacter, CurrentMovementStamina);
	DOREPLIFETIME(AThirdPersonMPCharacter, MovementStaminaRegen);
	DOREPLIFETIME(AThirdPersonMPCharacter, MovementStaminaRegenInterval);


	// Replicate  attack and defense.
	DOREPLIFETIME(AThirdPersonMPCharacter, PhysicalAttack);
	DOREPLIFETIME(AThirdPersonMPCharacter, PhysicalDefense);
	DOREPLIFETIME(AThirdPersonMPCharacter, MagicalAttack);
	DOREPLIFETIME(AThirdPersonMPCharacter, MagicalDefense);



	// Replicate Stats and Attributes
	DOREPLIFETIME_CONDITION(
		AThirdPersonMPCharacter, // clasa
		Strength,		         // proprietatea
		COND_OwnerOnly			 // conditia
	);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter,Constitution,COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter,Dexterity,COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter,Intelligence,COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter,Wisdom,COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AThirdPersonMPCharacter,Luck,COND_OwnerOnly);

	// replicated exp
	DOREPLIFETIME(AThirdPersonMPCharacter, CurrentEXP);
	DOREPLIFETIME(AThirdPersonMPCharacter, EXPToNextLevel);
	DOREPLIFETIME(AThirdPersonMPCharacter, Level);
	DOREPLIFETIME(AThirdPersonMPCharacter, AvailableStatPoints);



}


// OnXUpdate for BaseStats
void AThirdPersonMPCharacter::OnBaseMaxHealthUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseHealthRegenUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseHealthRegenIntervalUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseStrengthUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseConstitutionUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseDexterityUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseIntelligenceUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseWisdomUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseLuckUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseMaxManaUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseManaRegenUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseManaRegenIntervalUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseMaxSkillStaminaUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseSkillStaminaRegenUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseSkillStaminaRegenIntervalUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseMaxMovementStaminaUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseMovementStaminaRegenUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBaseMovementStaminaRegenIntervalUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnBasePhysicalAttackUpdate()
{
	// 
}
void AThirdPersonMPCharacter::OnBasePhysicalDefenseUpdate()
{
	
}
void AThirdPersonMPCharacter::OnBaseMagicalAttackUpdate()
{
	
}
void AThirdPersonMPCharacter::OnBaseMagicalDefenseUpdate()
{
	//
}

/////////////////////////////////
void AThirdPersonMPCharacter::OnMaxHealthUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnCurrentHealthUpdate()
{
	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}

	//Functions that occur on all machines.
			/*
				Any special functionality that should occur as a result of damage or death should be placed here.
			*/
}
void AThirdPersonMPCharacter::OnHealthRegenUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnHealthRegenIntervalUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnMaxManaUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnCurrentManaUpdate()
{
	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f mana remaining."), CurrentMana);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentMana <= 0)
		{
			FString noManaMessage = FString::Printf(TEXT("Mana depleted!!!"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, noManaMessage);
		}
	}

	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString manaMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentMana);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, manaMessage);
	}

	//Functions that occur on all machines.
			/*
				Any special functionality that should occur as a result of damage or death should be placed here.
			*/
}
void AThirdPersonMPCharacter::OnManaRegenUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnManaRegenIntervalUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnMaxSkillStaminaUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnCurrentSkillStaminaUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnSkillStaminaRegenUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnSkillStaminaRegenIntervalUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnMaxMovementStaminaUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnCurrentMovementStaminaUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnMovementStaminaRegenUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnMovementStaminaRegenIntervalUpdate()
{
	// nothing yet
}
void AThirdPersonMPCharacter::OnPhysicalAttackUpdate()
{
	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString physicalAttackMessage = FString::Printf(TEXT("You now have %f Physical Attack!"), PhysicalAttack);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, physicalAttackMessage);

	}

	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString physicalAttackMessage = FString::Printf(TEXT("%s now has %f Physical Attack !"), *GetFName().ToString(), PhysicalAttack);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, physicalAttackMessage);
	}

	//Functions that occur on all machines.
			/*
				Any special functionality that should occur as a result of damage or death should be placed here.
			*/
}
void AThirdPersonMPCharacter::OnPhysicalDefenseUpdate()
{
	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString physicaldefenseMessage = FString::Printf(TEXT("You now have %f Physical Defense!"), PhysicalDefense);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, physicaldefenseMessage);

	}

	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString physicaldefenseMessage = FString::Printf(TEXT("%s now has %f Physical Defense!"), *GetFName().ToString(), PhysicalDefense);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, physicaldefenseMessage);
	}

	//Functions that occur on all machines.
			/*
				Any special functionality that should occur as a result of damage or death should be placed here.
			*/

}
void AThirdPersonMPCharacter::OnMagicalAttackUpdate()
{
	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString magicalAttackMessage = FString::Printf(TEXT("You now have %f Magical Attack!"), MagicalAttack);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, magicalAttackMessage);

	}

	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString magicalAttackMessage = FString::Printf(TEXT("%s now has %f Magical Attack!"), *GetFName().ToString(), MagicalAttack);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, magicalAttackMessage);
	}

	//Functions that occur on all machines.
			/*
				Any special functionality that should occur as a result of damage or death should be placed here.
			*/
}
void AThirdPersonMPCharacter::OnMagicalDefenseUpdate()
{
	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString magicaldefenseMessage = FString::Printf(TEXT("You now have %f Magical Defense!"), MagicalDefense);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, magicaldefenseMessage);

	}

	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString magicaldefenseMessage = FString::Printf(TEXT("%s now has %f Magical Defense!"), *GetFName().ToString(), MagicalDefense);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, magicaldefenseMessage);
	}

	//Functions that occur on all machines.
			/*
				Any special functionality that should occur as a result of damage or death should be placed here.
			*/

}
void AThirdPersonMPCharacter::OnStrengthUpdate()
{
	if (IsLocallyControlled())
	{
		FString strText = FString::Printf(TEXT("Strength updated: %d"), Strength);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, strText);
	}
}
void AThirdPersonMPCharacter::OnConstitutionUpdate()
{
	if (IsLocallyControlled())
	{
		FString strText = FString::Printf(TEXT("Constitution updated: %d"), Constitution);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, strText);
	}
}
void AThirdPersonMPCharacter::OnDexterityUpdate()
{
	if (IsLocallyControlled())
	{
		FString strText = FString::Printf(TEXT("Dexterity updated: %d"), Dexterity);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, strText);
	}
}
void AThirdPersonMPCharacter::OnIntelligenceUpdate()
{
	if (IsLocallyControlled())
	{
		FString strText = FString::Printf(TEXT("Intelligence updated: %d"), Intelligence);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, strText);
	}
}
void AThirdPersonMPCharacter::OnWisdomUpdate()
{
	if (IsLocallyControlled())
	{
		FString strText = FString::Printf(TEXT("Wisdom updated: %d"), Wisdom);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, strText);
	}
}
void AThirdPersonMPCharacter::OnLuckUpdate()
{
	if (IsLocallyControlled())
	{
		FString strText = FString::Printf(TEXT("Luck updated: %d"), Luck);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, strText);
	}
}

// UI Base Stats update
void AThirdPersonMPCharacter::OnRep_BaseMaxHealth()
{
	// aici
	OnBaseMaxHealthUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseHealthRegen()
{
	// aici
	OnBaseHealthRegenUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseHealthRegenInterval()
{
	// aici
	OnBaseHealthRegenIntervalUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseMaxMana()
{
	// aici
	OnBaseMaxManaUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseManaRegen()
{
	// aici
	OnBaseManaRegenUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseManaRegenInterval()
{
	// aici
	OnBaseManaRegenIntervalUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseMaxSkillStamina()
{
	// aici
	OnBaseMaxSkillStaminaUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseSkillStaminaRegen()
{
	// aici
	OnBaseSkillStaminaRegenUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseSkillStaminaRegenInterval()
{
	// aici
	OnBaseSkillStaminaRegenIntervalUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseMaxMovementStamina()
{
	// aici
	OnBaseMaxMovementStaminaUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseMovementStaminaRegen()
{
	// aici
	OnBaseMovementStaminaRegenUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseMovementStaminaRegenInterval()
{
	// aici
	OnBaseMovementStaminaRegenIntervalUpdate();
}
void AThirdPersonMPCharacter::OnRep_BasePhysicalAttack()
{
	// aici
	OnBasePhysicalAttackUpdate();
}
void AThirdPersonMPCharacter::OnRep_BasePhysicalDefense()
{
	// aici
	OnBasePhysicalDefenseUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseMagicalAttack()
{
	// aici
	OnBaseMagicalAttackUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseMagicalDefense()
{
	// aici
	OnBaseMagicalDefenseUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseStrength()
{
	// aici
	OnBaseStrengthUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseConstitution()
{
	// aici
	OnBaseConstitutionUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseDexterity()
{
	// aici
	OnBaseDexterityUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseIntelligence()
{
	// aici
	OnBaseIntelligenceUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseWisdom()
{
	// aici
	OnBaseWisdomUpdate();
}
void AThirdPersonMPCharacter::OnRep_BaseLuck()
{
	// aici
	OnBaseLuckUpdate();
}
// UI update, effects, etc. in interior la fiecare
void AThirdPersonMPCharacter::OnRep_MaxHealth()
{
	// aici
	OnMaxHealthUpdate();
}
void AThirdPersonMPCharacter::OnRep_CurrentHealth()
{
	// aici
	OnCurrentHealthUpdate();
}
void AThirdPersonMPCharacter::OnRep_HealthRegen()
{
	// aici
	OnHealthRegenUpdate();
}
void AThirdPersonMPCharacter::OnRep_HealthRegenInterval()
{
	// aici
	OnHealthRegenIntervalUpdate();
}
void AThirdPersonMPCharacter::OnRep_MaxMana()
{
	// aici
	OnMaxManaUpdate();
}
void AThirdPersonMPCharacter::OnRep_CurrentMana()
{
	// aici
	OnCurrentManaUpdate();
}
void AThirdPersonMPCharacter::OnRep_ManaRegen()
{
	// aici
	OnManaRegenUpdate();
}
void AThirdPersonMPCharacter::OnRep_ManaRegenInterval()
{
	// aici
	OnManaRegenIntervalUpdate();
}
void AThirdPersonMPCharacter::OnRep_MaxSkillStamina()
{
	// aici
	OnMaxSkillStaminaUpdate();
}
void AThirdPersonMPCharacter::OnRep_CurrentSkillStamina()
{
	// aici
	OnCurrentSkillStaminaUpdate();
}
void AThirdPersonMPCharacter::OnRep_SkillStaminaRegen()
{
	// aici
	OnSkillStaminaRegenUpdate();
}
void AThirdPersonMPCharacter::OnRep_SkillStaminaRegenInterval()
{
	// aici
	OnSkillStaminaRegenIntervalUpdate();
}
void AThirdPersonMPCharacter::OnRep_MaxMovementStamina()
{
	// aici
	OnMaxMovementStaminaUpdate();
}
void AThirdPersonMPCharacter::OnRep_CurrentMovementStamina()
{
	// aici
	OnCurrentMovementStaminaUpdate();
}
void AThirdPersonMPCharacter::OnRep_MovementStaminaRegen()
{
	// aici
	OnMovementStaminaRegenUpdate();
}
void AThirdPersonMPCharacter::OnRep_MovementStaminaRegenInterval()
{
	// aici
	OnMovementStaminaRegenIntervalUpdate();
}
void AThirdPersonMPCharacter::OnRep_PhysicalAttack()
{
	// aici
	OnPhysicalAttackUpdate();
}
void AThirdPersonMPCharacter::OnRep_PhysicalDefense()
{
	// aici
	OnPhysicalDefenseUpdate();
}
void AThirdPersonMPCharacter::OnRep_MagicalAttack()
{
	// aici
	OnMagicalAttackUpdate();
}
void AThirdPersonMPCharacter::OnRep_MagicalDefense()
{
	// aici
	OnMagicalDefenseUpdate();
}
void AThirdPersonMPCharacter::OnRep_Strength()
{
	// aici
	OnStrengthUpdate();
}
void AThirdPersonMPCharacter::OnRep_Constitution()
{
	// aici
	OnConstitutionUpdate();
}
void AThirdPersonMPCharacter::OnRep_Dexterity()
{
	// aici
	OnDexterityUpdate();
}
void AThirdPersonMPCharacter::OnRep_Intelligence()
{
	// aici
	OnIntelligenceUpdate();
}
void AThirdPersonMPCharacter::OnRep_Wisdom()
{
	// aici
	OnWisdomUpdate();
}
void AThirdPersonMPCharacter::OnRep_Luck()
{
	// aici
	OnLuckUpdate();
}

void AThirdPersonMPCharacter::RegenHealth()
{
	// Daca CurrentHealth < MaxHealth, activam regen-ul , limitat la MaxHealth
	if (CurrentHealth < MaxHealth)
	{
		float NewHealth = FMath::Clamp(CurrentHealth + HealthRegen, 0.f, MaxHealth);
		SetCurrentHealth(NewHealth);
	}
}
void AThirdPersonMPCharacter::RegenMana()
{
	// Daca CurrentMana < MaxMana, activam regen-ul , limitat la MaxMana
	if (CurrentMana < MaxMana)
	{
		float NewMana = FMath::Clamp(CurrentMana + ManaRegen, 0.f, MaxMana);
		SetCurrentMana(NewMana);
	}
}

void AThirdPersonMPCharacter::RegenSkillStamina()
{
	// Daca CurrentSkillStamina < MaxSkillStamina, activam regen-ul , limitat la MaxSkillStamina
	if (CurrentSkillStamina < MaxSkillStamina)
	{
		float NewSkillStamina = FMath::Clamp(CurrentSkillStamina + SkillStaminaRegen, 0.f, MaxSkillStamina);
		SetCurrentSkillStamina(NewSkillStamina);
	}
}

void AThirdPersonMPCharacter::RegenMovementStamina()
{
	// Daca CurrentMovementStamina < MaxMovementStamina, activam regen-ul , limitat la MaxMovementStamina
	if (CurrentMovementStamina < MaxMovementStamina)
	{
		float NewMovementStamina = FMath::Clamp(CurrentMovementStamina + MovementStaminaRegen, 0.f, MaxMovementStamina);
		SetCurrentMovementStamina(NewMovementStamina);
	}
}

//Set BaseStats 
// Set Health related properties
void AThirdPersonMPCharacter::SetBaseMaxHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseMaxHealth = healthValue;
	}
}
void AThirdPersonMPCharacter::SetBaseHealthRegen(float regenValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseHealthRegen = regenValue;
	}
}
void AThirdPersonMPCharacter::SetBaseHealthRegenInterval(float intervalValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseHealthRegenInterval = intervalValue;
	}
}
// Set Mana related Properties
void AThirdPersonMPCharacter::SetBaseMaxMana(float manaValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseMaxMana = manaValue;
	}
}
void AThirdPersonMPCharacter::SetBaseManaRegen(float newManaRegen)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseManaRegen = newManaRegen;
	}
}
void AThirdPersonMPCharacter::SetBaseManaRegenInterval(float newManaRegenInterval)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseManaRegenInterval = newManaRegenInterval;
	}
}
// Set SkillStamina related properties
void AThirdPersonMPCharacter::SetBaseMaxSkillStamina(float skillStaminaValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseMaxSkillStamina = skillStaminaValue;
	}
}
void AThirdPersonMPCharacter::SetBaseSkillStaminaRegen(float regenValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseSkillStaminaRegen = regenValue;
	}
}
void AThirdPersonMPCharacter::SetBaseSkillStaminaRegenInterval(float intervalValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseSkillStaminaRegenInterval = intervalValue;
	}
}
// Set SkillStamina related properties
void AThirdPersonMPCharacter::SetBaseMaxMovementStamina(float movementStaminaValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		MaxMovementStamina = movementStaminaValue;
	}
}
void AThirdPersonMPCharacter::SetBaseMovementStaminaRegen(float regenValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseMovementStaminaRegen = regenValue;
	}
}
void AThirdPersonMPCharacter::SetBaseMovementStaminaRegenInterval(float intervalValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseMovementStaminaRegenInterval = intervalValue;
	}
}
// Set Attack related properties
void AThirdPersonMPCharacter::SetBasePhysicalAttack(float attackValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BasePhysicalAttack = attackValue;
	}
}
void AThirdPersonMPCharacter::SetBaseMagicalAttack(float attackValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseMagicalAttack = attackValue;
	}
}
// Set Defense related properties
void AThirdPersonMPCharacter::SetBasePhysicalDefense(float defenseValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BasePhysicalDefense = defenseValue;
	}
}
void AThirdPersonMPCharacter::SetBaseMagicalDefense(float defenseValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseMagicalDefense = defenseValue;
	}
}
// Set Player Stats properites (STR, CON, DEX, INT, WIS, LUC)
void AThirdPersonMPCharacter::SetBaseStrength(int statValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseStrength = statValue;
	}
}
void AThirdPersonMPCharacter::SetBaseConstitution(int statValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseConstitution = statValue;
	}
}
void AThirdPersonMPCharacter::SetBaseIntelligence(int statValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseIntelligence = statValue;
	}
}
void AThirdPersonMPCharacter::SetBaseDexterity(int statValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseDexterity = statValue;
	}
}
void AThirdPersonMPCharacter::SetBaseWisdom(int statValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseWisdom = statValue;
	}
}
void AThirdPersonMPCharacter::SetBaseLuck(int statValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		BaseLuck = statValue;
	}
}

// Set Health related properties
void AThirdPersonMPCharacter::SetMaxHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		MaxHealth = healthValue;
	}
}
void AThirdPersonMPCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
	}
}
void AThirdPersonMPCharacter::SetHealthRegen(float regenValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		HealthRegen = regenValue;
	}
}
void AThirdPersonMPCharacter::SetHealthRegenInterval(float intervalValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		HealthRegenInterval = intervalValue;
	}
}

// Set Mana related Properties
void AThirdPersonMPCharacter::SetMaxMana(float manaValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		MaxMana = manaValue;
	}
}
void AThirdPersonMPCharacter::SetCurrentMana(float NewManaValue)
{
	NewManaValue = FMath::Clamp(NewManaValue, 0.f, MaxMana);
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentMana = NewManaValue;
	}
}
void AThirdPersonMPCharacter::SetManaRegen(float newManaRegen)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ManaRegen = newManaRegen;
	}
}
void AThirdPersonMPCharacter::SetManaRegenInterval(float newManaRegenInterval)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ManaRegenInterval = newManaRegenInterval;
	}
}

// Set SkillStamina related properties
void AThirdPersonMPCharacter::SetMaxSkillStamina(float skillStaminaValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		MaxSkillStamina = skillStaminaValue;
	}
}
void AThirdPersonMPCharacter::SetCurrentSkillStamina(float skillStaminaValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentSkillStamina = skillStaminaValue;
	}
}
void AThirdPersonMPCharacter::SetSkillStaminaRegen(float regenValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		SkillStaminaRegen = regenValue;
	}
}
void AThirdPersonMPCharacter::SetSkillStaminaRegenInterval(float intervalValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		SkillStaminaRegenInterval = intervalValue;
	}
}

// Set SkillStamina related properties
void AThirdPersonMPCharacter::SetMaxMovementStamina(float movementStaminaValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		MaxMovementStamina = movementStaminaValue;
	}
}
void AThirdPersonMPCharacter::SetCurrentMovementStamina(float movementStaminaValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentMovementStamina = FMath::Clamp(movementStaminaValue, 0.f, MaxMovementStamina);
	}
}
void AThirdPersonMPCharacter::SetMovementStaminaRegen(float regenValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		MovementStaminaRegen = regenValue;
	}
}
void AThirdPersonMPCharacter::SetMovementStaminaRegenInterval(float intervalValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		MovementStaminaRegenInterval = intervalValue;
	}
}
 
// Set Attack related properties
void AThirdPersonMPCharacter::SetPhysicalAttack(float attackValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		PhysicalAttack = attackValue;
	}
}
void AThirdPersonMPCharacter::SetMagicalAttack(float attackValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		MagicalAttack = attackValue;
	}
}
// Set Defense related properties
void AThirdPersonMPCharacter::SetPhysicalDefense(float defenseValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		PhysicalDefense = defenseValue;
	}
}
void AThirdPersonMPCharacter::SetMagicalDefense(float defenseValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		MagicalDefense = defenseValue;
	}
}

// Set Player Stats properites (STR, CON, DEX, INT, WIS, LUC)
void AThirdPersonMPCharacter::SetStrength(int statValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Strength = statValue;
	}
	
}
void AThirdPersonMPCharacter::SetConstitution(int statValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Constitution = statValue;
	}


}
void AThirdPersonMPCharacter::SetIntelligence(int statValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Intelligence = statValue;
	}

}
void AThirdPersonMPCharacter::SetDexterity(int statValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Dexterity = statValue;
	}

}
void AThirdPersonMPCharacter::SetWisdom(int statValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Wisdom = statValue;
	}

}
void AThirdPersonMPCharacter::SetLuck(int statValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Luck = statValue;
	}

}

bool AThirdPersonMPCharacter::ConsumeMana(float ManaCost)
{
	if (CurrentMana >= ManaCost)
	{
		SetCurrentMana(CurrentMana - ManaCost);
		return true;
	}
	return false;

}
void AThirdPersonMPCharacter::UpdateMaxHealth()
{
	// the actual Update
	float NewMaxHealth = BaseMaxHealth;
	// add Max Health from items - not yet implemented
	// add Max Health from passive skills - not yet implemented
	// add max health from buffs/debuffs
	NewMaxHealth += 10 * Constitution;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::MaxHealth)
				NewMaxHealth += Mod.Amount;

	SetMaxHealth(NewMaxHealth);
}
void AThirdPersonMPCharacter::UpdateCurrentHealth()
{
	//
}
void AThirdPersonMPCharacter::UpdateHealthRegen()
{
	float NewHealthRegen = BaseHealthRegen;
	NewHealthRegen += Constitution / 10;

	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::HealthRegen)
				NewHealthRegen += Mod.Amount;
	SetHealthRegen(NewHealthRegen);
}
void AThirdPersonMPCharacter::UpdateHealthRegenInterval()
{
	float NewHealthRegenInterval = BaseHealthRegenInterval;
	NewHealthRegenInterval -= Constitution / 100;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::HealthRegenInterval)
				NewHealthRegenInterval += Mod.Amount;
	SetHealthRegen(NewHealthRegenInterval);
}
void AThirdPersonMPCharacter::UpdateMaxMana()
{
	float NewMaxMana = BaseMaxMana;
	NewMaxMana += (Intelligence * 2) + (Wisdom * 3);
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::MaxMana)
				NewMaxMana += Mod.Amount;

	SetMaxMana(NewMaxMana);
}
void AThirdPersonMPCharacter::UpdateCurrentMana()
{
	//
}
void AThirdPersonMPCharacter::UpdateManaRegen()
{
	float NewManaRegen = BaseManaRegen;
	NewManaRegen += (Intelligence / 10) + (Wisdom / 10);
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::ManaRegen)
				NewManaRegen += Mod.Amount;

	SetManaRegen(NewManaRegen);
}
void AThirdPersonMPCharacter::UpdateManaRegenInterval()
{
	float NewManaRegenInterval = BaseManaRegenInterval;
	NewManaRegenInterval -= (Intelligence + Wisdom) / 100;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::ManaRegenInterval)
				NewManaRegenInterval += Mod.Amount;

	SetManaRegenInterval(NewManaRegenInterval);
}
void AThirdPersonMPCharacter::UpdateMaxSkillStamina()
{
	float NewMaxSkillStamina = BaseMaxSkillStamina;
	NewMaxSkillStamina += Dexterity * 5;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::MaxSkillStamina)
				NewMaxSkillStamina += Mod.Amount;

	SetMaxSkillStamina(NewMaxSkillStamina);
}
void AThirdPersonMPCharacter::UpdateCurrentSkillStamina()
{
	//
}
void AThirdPersonMPCharacter::UpdateSkillStaminaRegen()
{
	float NewSkillStaminaRegen = BaseSkillStaminaRegen;
	NewSkillStaminaRegen += Dexterity / 10;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::SkillStaminaRegen)
				NewSkillStaminaRegen += Mod.Amount;

	SetSkillStaminaRegen(NewSkillStaminaRegen);
}
void AThirdPersonMPCharacter::UpdateSkillStaminaRegenInterval()
{
	float NewSkillStaminaRegenInterval = BaseSkillStaminaRegenInterval;
	NewSkillStaminaRegenInterval -= Dexterity / 100;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::SkillStaminaRegenInterval)
				NewSkillStaminaRegenInterval += Mod.Amount;

	SetSkillStaminaRegenInterval(NewSkillStaminaRegenInterval);
}
void AThirdPersonMPCharacter::UpdateMaxMovementStamina()
{
	float NewMaxMovementStamina = BaseMaxMovementStamina;
	NewMaxMovementStamina += (Constitution * 3) + (Dexterity / 2);
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::MaxMovementStamina)
				NewMaxMovementStamina += Mod.Amount;

	SetMaxMovementStamina(NewMaxMovementStamina);
}
void AThirdPersonMPCharacter::UpdateCurrentMovementStamina()
{
	//
}
void AThirdPersonMPCharacter::UpdateMovementStaminaRegen()
{
	float NewMovementStaminaRegen = BaseSkillStaminaRegen;
	NewMovementStaminaRegen += (Constitution / 10) + (Dexterity / 20);
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::MovementStaminaRegen)
				NewMovementStaminaRegen += Mod.Amount;

	SetMovementStaminaRegen(NewMovementStaminaRegen);
}
void AThirdPersonMPCharacter::UpdateMovementStaminaRegenInterval()
{
	float NewMovementStaminaRegenInterval = BaseSkillStaminaRegenInterval;
	NewMovementStaminaRegenInterval -= (Constitution / 100) + (Dexterity / 200);
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::MovementStaminaRegenInterval)
				NewMovementStaminaRegenInterval += Mod.Amount;

	SetMovementStaminaRegenInterval(NewMovementStaminaRegenInterval);
}
void AThirdPersonMPCharacter::UpdatePhysicalAttack()
{
	float NewPhysicalAttack = BasePhysicalAttack;
	NewPhysicalAttack += (Strength * 5) + (Dexterity * 2);
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::PhysicalAttack)
				NewPhysicalAttack += Mod.Amount;

	SetPhysicalAttack(NewPhysicalAttack);
}
void AThirdPersonMPCharacter::UpdateMagicalAttack()
{
	float NewMagicalAttack = BaseMagicalAttack;
	NewMagicalAttack += (Intelligence * 5) + (Wisdom * 2);
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::MagicalAttack)
				NewMagicalAttack += Mod.Amount;

	SetMagicalAttack(NewMagicalAttack);
}
void AThirdPersonMPCharacter::UpdatePhysicalDefense()
{
	float NewPhysicalDefense = BasePhysicalDefense;
	NewPhysicalDefense += 5 * Constitution;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::PhysicalDefense)
				NewPhysicalDefense += Mod.Amount;

	SetPhysicalDefense(NewPhysicalDefense);
}
void AThirdPersonMPCharacter::UpdateMagicalDefense()
{
	float NewMagicalDefense = BaseMagicalDefense;
	NewMagicalDefense += 5 * Wisdom;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::MagicalDefense)
				NewMagicalDefense += Mod.Amount;

	SetMagicalDefense(NewMagicalDefense);
}


void AThirdPersonMPCharacter::UpdateStrength()
{
	float NewStrength = BaseStrength;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::Strength)
				NewStrength += Mod.Amount;

	SetStrength(NewStrength);
	UpdatePhysicalAttack();
}
void AThirdPersonMPCharacter::UpdateConstitution()
{
	float NewConstitution = BaseConstitution;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::Constitution)
				NewConstitution += Mod.Amount;

	SetConstitution(NewConstitution);
	UpdateMaxHealth();
	UpdateHealthRegen();
	UpdateHealthRegenInterval();
	UpdatePhysicalDefense();
	UpdateMaxMovementStamina();
	UpdateMovementStaminaRegen();
	UpdateMovementStaminaRegenInterval();
}
void AThirdPersonMPCharacter::UpdateDexterity()
{
	float NewDexterity = BaseDexterity;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::Dexterity)
				NewDexterity += Mod.Amount;

	SetDexterity(NewDexterity);
	UpdatePhysicalAttack();
	UpdateMaxSkillStamina();
	UpdateSkillStaminaRegen();
	UpdateSkillStaminaRegenInterval();
	UpdateMaxMovementStamina();
	UpdateMovementStaminaRegen();
	UpdateMovementStaminaRegenInterval();
	// UpdateAccuracy();
	// UpdateEvasion();
}
void AThirdPersonMPCharacter::UpdateIntelligence()
{
	float NewIntelligence = BaseIntelligence;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::Intelligence)
				NewIntelligence += Mod.Amount;

	SetIntelligence(NewIntelligence);
	UpdateMagicalAttack();
	UpdateMaxMana();
	UpdateManaRegen();
	UpdateManaRegenInterval();
}
void AThirdPersonMPCharacter::UpdateWisdom()
{
	float NewWisdom = BaseWisdom;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::Wisdom)
				NewWisdom += Mod.Amount;

	SetWisdom(NewWisdom);
	UpdateMagicalDefense();
	UpdateMaxMana();
	UpdateManaRegen();
	UpdateManaRegenInterval();
}
void AThirdPersonMPCharacter::UpdateLuck()
{
	float NewLuck = BaseLuck;
	for (auto& B : ActiveBuffs)
		for (auto& Mod : B.Definition.Modifiers)
			if (Mod.Stat == EStatTypes::Luck)
				NewLuck += Mod.Amount;

	SetLuck(NewLuck);
	// UpdateCriticalChance();
	// UpdateCriticalDamage();
}


// Update All Stats ( not STR, DEX, CON, INT, WIS, LUCK)
void AThirdPersonMPCharacter::UpdateAll()
{
	UpdateMaxHealth();
	UpdateHealthRegen();
	UpdateHealthRegenInterval();
	UpdateMaxMana();
	UpdateManaRegen();
	UpdateManaRegenInterval();
	UpdateMaxSkillStamina();
	UpdateSkillStaminaRegen();
	UpdateSkillStaminaRegenInterval();
	UpdateMaxMovementStamina();
	UpdateMovementStaminaRegen();
	UpdateMovementStaminaRegenInterval();

}
void AThirdPersonMPCharacter::FlushDirtyStats()
{
	if (!HasAuthority())
		return;

	if (DirtyFlags & STAT_BIT(MaxHealth))
	{
		UpdateMaxHealth();
		DirtyFlags &= ~STAT_BIT(MaxHealth);
	}
	if (DirtyFlags & STAT_BIT(HealthRegen))
	{
		UpdateHealthRegen();
		DirtyFlags &= ~STAT_BIT(HealthRegen);
	}
	if (DirtyFlags & STAT_BIT(HealthRegenInterval))
	{
		UpdateHealthRegenInterval();
		DirtyFlags &= ~STAT_BIT(HealthRegenInterval);
	}
	if (DirtyFlags & STAT_BIT(MaxMana))
	{
		UpdateMaxMana();
		DirtyFlags &= ~STAT_BIT(MaxMana);
	}
	if (DirtyFlags & STAT_BIT(ManaRegen))
	{
		UpdateManaRegen();
		DirtyFlags &= ~STAT_BIT(ManaRegen);
	}
	if (DirtyFlags & STAT_BIT(ManaRegenInterval))
	{
		UpdateManaRegenInterval();
		DirtyFlags &= ~STAT_BIT(ManaRegenInterval);
	}
	if (DirtyFlags & STAT_BIT(MaxSkillStamina))
	{
		UpdateMaxSkillStamina();
		DirtyFlags &= ~STAT_BIT(MaxSkillStamina);
	}
	if (DirtyFlags & STAT_BIT(SkillStaminaRegen))
	{
		UpdateSkillStaminaRegen();
		DirtyFlags &= ~STAT_BIT(SkillStaminaRegen);
	}
	if (DirtyFlags & STAT_BIT(SkillStaminaRegenInterval))
	{
		UpdateSkillStaminaRegenInterval();
		DirtyFlags &= ~STAT_BIT(SkillStaminaRegenInterval);
	}
	if (DirtyFlags & STAT_BIT(MaxSkillStamina))
	{
		UpdateMaxMovementStamina();
		DirtyFlags &= ~STAT_BIT(MaxSkillStamina);
	}
	if (DirtyFlags & STAT_BIT(MovementStaminaRegen))
	{
		UpdateMovementStaminaRegen();
		DirtyFlags &= ~STAT_BIT(MovementStaminaRegen);
	}
	if (DirtyFlags & STAT_BIT(MovementStaminaRegenInterval))
	{
		UpdateMovementStaminaRegenInterval();
		DirtyFlags &= ~STAT_BIT(MovementStaminaRegenInterval);
	}
	if (DirtyFlags & STAT_BIT(PhysicalAttack))
	{
		UpdatePhysicalAttack();
		DirtyFlags &= ~STAT_BIT(PhysicalAttack);
	}
	if (DirtyFlags & STAT_BIT(MagicalAttack))
	{
		UpdateMagicalAttack();
		DirtyFlags &= ~STAT_BIT(MagicalAttack);
	}
	if (DirtyFlags & STAT_BIT(PhysicalDefense))
	{
		UpdatePhysicalDefense();
		DirtyFlags &= ~STAT_BIT(PhysicalDefense);
	}
	if (DirtyFlags & STAT_BIT(MagicalDefense))
	{
		UpdateMagicalDefense();
		DirtyFlags &= ~STAT_BIT(MagicalDefense);
	}
	if (DirtyFlags & STAT_BIT(Strength))
	{
		UpdateStrength();
		DirtyFlags &= ~STAT_BIT(Strength);
	}
	if (DirtyFlags & STAT_BIT(Constitution))
	{
		UpdateConstitution();
		DirtyFlags &= ~STAT_BIT(Constitution);
	}
	if (DirtyFlags & STAT_BIT(Dexterity))
	{
		UpdateDexterity();
		DirtyFlags &= ~STAT_BIT(Dexterity);
	}
	if (DirtyFlags & STAT_BIT(Intelligence))
	{
		UpdateIntelligence();
		DirtyFlags &= ~STAT_BIT(Intelligence);
	}
	if (DirtyFlags & STAT_BIT(Wisdom))
	{
		UpdateWisdom();
		DirtyFlags &= ~STAT_BIT(Wisdom);
	}
	if (DirtyFlags & STAT_BIT(Luck))
	{
		UpdateLuck();
		DirtyFlags &= ~STAT_BIT(Luck);
	}

}

void AThirdPersonMPCharacter::AddBuff(const FBuff& NewBuff)
{
	if (!HasAuthority())
		return;

	// creare intrare FActiveBuff
	FActiveBuff AB;
	AB.Definition = NewBuff;
	ActiveBuffs.Add(AB);

	// setare timer -> call RemoveBuff
	FTimerDelegate Del;
	Del.BindUFunction(this, FName("RemoveBuff"), NewBuff.BuffName);
	GetWorld()->GetTimerManager().SetTimer(AB.ExpirationHandle, Del, NewBuff.Duration, false);

	// marcare recalculare stat
	for (auto& Mod : NewBuff.Modifiers)
	{
		// se seteaza bitul de dirty flag
		DirtyFlags |= (1u << static_cast<uint8>(Mod.Stat));
	}
	FlushDirtyStats();
}

void AThirdPersonMPCharacter::RemoveBuff(FName BuffName)
{
	if (!HasAuthority()) return;

	// cautare si scoatere buff din lista ActiveBuffs
	for (int32 i = ActiveBuffs.Num() - 1; i >= 0; i--)
	{
		if (ActiveBuffs[i].Definition.BuffName == BuffName)
		{
			// anulare timer
			GetWorld()->GetTimerManager().ClearTimer(ActiveBuffs[i].ExpirationHandle);

			// remove buff
			ActiveBuffs.RemoveAt(i);
		}
	}

	// recalculare stats
	DirtyFlags = 0u;
	for (auto& AB : ActiveBuffs)
		for (auto& Mod : AB.Definition.Modifiers)
			DirtyFlags |= (1u << static_cast<uint8>(Mod.Stat));
	FlushDirtyStats();

}

// functii de replicare pentru adaugarea unu punct la stats
void AThirdPersonMPCharacter::ServerAddStrengthPoint_Implementation()
{
	if (AvailableStatPoints > 0)
	{
		AvailableStatPoints--;
		BaseStrength++;
		UpdateStrength();
	}
}
void AThirdPersonMPCharacter::ServerAddConstitutionPoint_Implementation()
{
	if (AvailableStatPoints > 0)
	{
		AvailableStatPoints--;
		BaseConstitution++;
		UpdateConstitution();
	}
}
void AThirdPersonMPCharacter::ServerAddDexterityPoint_Implementation()
{
	if (AvailableStatPoints > 0)
	{
		AvailableStatPoints--;
		BaseDexterity++;
		UpdateDexterity();
	}
}
void AThirdPersonMPCharacter::ServerAddIntelligencePoint_Implementation()
{
	if (AvailableStatPoints > 0)
	{
		AvailableStatPoints--;
		BaseIntelligence++;
		UpdateIntelligence();
	}
}
void AThirdPersonMPCharacter::ServerAddWisdomPoint_Implementation()
{
	if (AvailableStatPoints > 0)
	{
		AvailableStatPoints--;
		BaseWisdom++;
		UpdateWisdom();
	}
}
void AThirdPersonMPCharacter::ServerAddLuckPoint_Implementation()
{
	if (AvailableStatPoints > 0)
	{
		AvailableStatPoints--;
		BaseLuck++;
		UpdateLuck();
	}
}




/* Folosire Consume Mana
if (ConsumeMana(25.f))
{
	// mana scazuta cu 25 → lanseaza abilitatea
}
else
{
	// nu e suficient mana → feedback jucator
}


*/

void AThirdPersonMPCharacter::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &AThirdPersonMPCharacter::StopFire, FireRate, false);
		HandleFire();
	}
}

void AThirdPersonMPCharacter::StopFire()
{
	bIsFiringWeapon = false;
}


void AThirdPersonMPCharacter::HandleFire_Implementation()
{
	FVector spawnLocation = GetActorLocation() + (GetActorRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FRotator spawnRotation = GetActorRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	AThirdPersonMPProjectile* spawnedProjectile = GetWorld()->SpawnActor<AThirdPersonMPProjectile>(spawnLocation, spawnRotation, spawnParameters);
}

void AThirdPersonMPCharacter::GainEXP(int32 Amount)
{
	if (!HasAuthority()) return;

	CurrentEXP += Amount;
	while (CurrentEXP >= EXPToNextLevel)
	{
		CurrentEXP -= EXPToNextLevel;
		LevelUp();
		EXPToNextLevel = FMath::RoundToInt(EXPToNextLevel * 1.2f); // +20% per level
	}
	// DEBUG
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("EXP: %d"), CurrentEXP));

	OnEXPChanged();
	OnRep_CurrentEXP(); // update pentru server local
	OnRep_EXPToNextLevel();
	OnRep_Level();

	// Update UI

}

float AThirdPersonMPCharacter::GetEXPPercent() const
{
	if (EXPToNextLevel == 0) return 0.f;
	return static_cast<float>(CurrentEXP) / static_cast<float>(EXPToNextLevel);
}

void AThirdPersonMPCharacter::OnEXPChanged()
{

}

void AThirdPersonMPCharacter::OnRep_CurrentEXP()
{
	if (IsLocallyControlled())
	{
		// notificare UI
	}
}

void AThirdPersonMPCharacter::OnRep_EXPToNextLevel()
{
	if (IsLocallyControlled())
	{
		// notificare UI
	}
}

void AThirdPersonMPCharacter::OnRep_Level()
{
	if (IsLocallyControlled())
	{
		// notificare UI
	}
}

void AThirdPersonMPCharacter::OnRep_AvailableStatPoints()
{
	// notificare UI
}


void AThirdPersonMPCharacter::LevelUp()
{
	Level++;
	AvailableStatPoints += 7;

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT(" Congratz you reached a new Level!!!")));

	// base stats increase
	BaseMaxHealth += 20.f;
	BaseMaxMana += 10.f;
	BaseMaxSkillStamina += 10.f;
	BaseMaxMovementStamina += 5.f;
	BaseStrength += 1;
	BaseConstitution += 1;
	BaseDexterity += 1;
	BaseIntelligence += 1;
	BaseWisdom += 1;
	BaseLuck += 1;

	// recalculare stats
	UpdateMaxHealth();
	UpdateMaxMana();
	UpdateMaxSkillStamina();
	UpdateMaxMovementStamina();
	UpdateStrength();
	UpdateConstitution();
	UpdateDexterity();
	UpdateIntelligence();
	UpdateWisdom();
	UpdateLuck();

	// notify client
	OnMaxHealthUpdate();
	OnMaxManaUpdate();
	OnMaxSkillStaminaUpdate();
	OnMaxMovementStaminaUpdate();
	OnStrengthUpdate();
	OnConstitutionUpdate();
	OnDexterityUpdate();
	OnIntelligenceUpdate();
	OnWisdomUpdate();
	OnLuckUpdate();

}


UInventoryPanel* AThirdPersonMPCharacter::GetInventoryPanel() const
{
	return InventoryPanel;
}

void AThirdPersonMPCharacter::SetupInventoryConnection()
{
	if (UInventoryComponent* InvComp = GetInventoryComponent())
	{
		if (UInventoryPanel* InvPanel = GetInventoryPanel())
		{
			// First bind the inventory
			InvPanel->BindToInventory(InvComp);

			// Then connect the delegate
			InvComp->OnInventoryUpdated.AddDynamic(InvPanel, &UInventoryPanel::RefreshInventory);

			// Force initial refresh
			InvPanel->RefreshInventory(InvComp->GetItems());
		}
	}
}

// Called when the game starts or when spawned
void AThirdPersonMPCharacter::BeginPlay()
{
	Super::BeginPlay();


	// Legatura intre componenta si UI
	if (UInventoryComponent* InvComp = GetInventoryComponent())
	{
		if (UInventoryPanel* InvPanel = GetInventoryPanel())
		{
			InvComp->OnInventoryUpdated.AddDynamic(InvPanel, &UInventoryPanel::RefreshInventory);
			// Forteaza actualizarea initiala
			InvPanel->RefreshInventory(InvComp->GetItems());
		}
	}

	if (HasAuthority()) 
	{
		SetMaxHealth(BaseMaxHealth);
		SetCurrentHealth(MaxHealth);
		SetHealthRegen(BaseHealthRegen);
		SetHealthRegenInterval(BaseHealthRegenInterval);
		SetMaxMana(BaseMaxMana);
		SetCurrentMana(MaxMana);
		SetManaRegen(BaseManaRegen);
		SetManaRegenInterval(BaseManaRegenInterval);
		SetMaxSkillStamina(BaseMaxSkillStamina);
		SetCurrentSkillStamina(MaxSkillStamina);
		SetSkillStaminaRegen(BaseSkillStaminaRegen);
		SetSkillStaminaRegenInterval(BaseSkillStaminaRegenInterval);
		SetMaxMovementStamina(BaseMaxMovementStamina);
		SetCurrentMovementStamina(MaxMovementStamina);
		SetMovementStaminaRegen(BaseMovementStaminaRegen);
		SetMovementStaminaRegenInterval(BaseMovementStaminaRegenInterval);
		SetPhysicalAttack(BasePhysicalAttack);
		SetMagicalAttack(BasePhysicalAttack);
		SetPhysicalDefense(BasePhysicalAttack);
		SetMagicalDefense(BasePhysicalAttack);
		SetStrength(BaseStrength);
		SetConstitution(BaseConstitution);
		SetDexterity(BaseDexterity);
		SetIntelligence(BaseIntelligence);
		SetWisdom(BaseWisdom);
		SetLuck(BaseLuck);

		DirtyFlags = (1u << static_cast<uint8>(EStatTypes::Count)) - 1;
		FlushDirtyStats();
	}
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Sub =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Sub->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (HasAuthority()) // regen-ul sa face pe server si se replica la client
	{
		GetWorld()->GetTimerManager().SetTimer(
			HealthRegenTimerHandle,
			this,
			&AThirdPersonMPCharacter::RegenHealth,
			HealthRegenInterval,
			true //looping
		); // health regen

		GetWorld()->GetTimerManager().SetTimer(
			ManaRegenTimerHandle,
			this,
			&AThirdPersonMPCharacter::RegenMana,
			ManaRegenInterval,
			true //looping
		); // mana regen

		GetWorld()->GetTimerManager().SetTimer(
			SkillStaminaRegenTimerHandle,
			this,
			&AThirdPersonMPCharacter::RegenSkillStamina,
			SkillStaminaRegenInterval,
			true //looping
		); // skill stamina regen

		GetWorld()->GetTimerManager().SetTimer(
			MovementStaminaRegenTimerHandle,
			this,
			&AThirdPersonMPCharacter::RegenMovementStamina,
			MovementStaminaRegenInterval,
			true //looping
		); // movement stamina regen
	}

	if (InventoryComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ThirdPersonMPCharacter] InventoryComponent este valid!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[ThirdPersonMPCharacter] InventoryComponent este NULL!"));
	}


	EquipWeapon(WeaponClass);

}

////////////////////
// Show Mouse
void AThirdPersonMPCharacter::ToggleMouseVisibility()
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		bIsMouseVisible = !bIsMouseVisible;

		if (bIsMouseVisible)
		{
			PC->bShowMouseCursor = true;

			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			PC->SetInputMode(InputMode);
		}
		else
		{
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
		}
	}
}



/////////////////////////////////////////////////////////////
// Combat System

void AThirdPersonMPCharacter::MeleeAttack()
{
	if (!bCanAttack) return;

	// daca characterul este intr-o animatie de atack, se pregateste urmatoarea
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(EquippedWeapon->AttackMontage))
	{
		if (bCanDoCombo)
		{
			CurrentComboIndex = FMath::Clamp(CurrentComboIndex + 1, 0, 2); // max 3 combo-uri
			bCanDoCombo = false;
		}
		return;
	}


	bCanAttack = false;
	CurrentComboIndex = 0;
	PlayAttackSection(CurrentComboIndex);
	float AnimDuration = PlayAnimMontage(EquippedWeapon->AttackMontage);
	// pornire timer
	GetWorldTimerManager().SetTimer(MeleeAttackCooldownTimer, this, &AThirdPersonMPCharacter::ResetAttackCooldown, MeleeAttackCooldown,	false);
	// se activeaza atacul efectiv
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Atac permis!"));
	ServerMeleeAttack();
}
void AThirdPersonMPCharacter::PlayAttackSection(int32 Index)
{
	FName SectionName;
	switch (Index)
	{
	case 0: SectionName = "Attack1"; break;
	case 1: SectionName = "Attack2"; break;
	case 2: SectionName = "Attack3"; break;
	default: SectionName = "Attack1"; break;
	}

	GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, EquippedWeapon->AttackMontage);
}
void AThirdPersonMPCharacter::EnableCombo()
{
	bCanDoCombo = true;
}


void AThirdPersonMPCharacter::ServerMeleeAttack_Implementation()
{
	if (!HasAuthority()) return;

	MeleeAttack_Internal(); // logica de damage
	Multicast_PlayAttackMontage(); // animatie + restrictie movement
}

void AThirdPersonMPCharacter::MeleeAttack_Internal()
{

	if (SwordAttackMontage && !GetMesh()->GetAnimInstance()->Montage_IsPlaying(SwordAttackMontage))
	{

		PlayAnimMontage(EquippedWeapon->AttackMontage);

		FVector Start = GetActorLocation();
		FVector Forward = GetActorForwardVector();
		FVector End = Start + Forward * 150.0f;

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, Params);

		if (bHit && Hit.GetActor())
		{
			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyDamage(HitActor, PhysicalAttack, GetController(), this, nullptr);
		}

		


	}
	
}


float AThirdPersonMPCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float NewCurrentHealth = CurrentHealth - DamageTaken;
	SetCurrentHealth(NewCurrentHealth);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("HP: %f"), CurrentHealth));

	return NewCurrentHealth;
}

void AThirdPersonMPCharacter::EquipWeapon(TSubclassOf<AWeaponBase> NewWeaponClass)
{
	if (!NewWeaponClass) return;

	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
		EquippedWeapon = nullptr;
	}

	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = this;

	EquippedWeapon = GetWorld()->SpawnActor<AWeaponBase>(NewWeaponClass, Params);
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("wep_r"));
	}
}


void AThirdPersonMPCharacter::Multicast_PlayAttackMontage_Implementation()
{
	if (!EquippedWeapon || !EquippedWeapon->AttackMontage) return;
	
		bCanMove = false;

		if (IsLocallyControlled())
		{
			PlayAnimMontage(EquippedWeapon->AttackMontage);
		}
		
			float AnimDuration = PlayAnimMontage(EquippedWeapon->AttackMontage);

			// enable character movement
			if (AnimDuration > 0.f)
			{
				FTimerHandle TimerHandle;
				GetWorldTimerManager().SetTimer(TimerHandle, this, &AThirdPersonMPCharacter::ResetMovementRestrictions, AnimDuration, false);

			}

		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Hit.ImpactPoint);
		//UGameplayStatics::PlaySoundAtLocation(this, SwordHitSound, GetActorLocation());

	

}

void AThirdPersonMPCharacter::SetCanMove(bool bNewState)
{
	bCanMove = bNewState;
}
void AThirdPersonMPCharacter::CustomJump()
{
	if (!bCanMove) return;

	Jump(); // apeleaza metoda din ACharacter
}

void AThirdPersonMPCharacter::CustomStopJumping()
{
	if (!bCanMove) return;

	StopJumping(); // tot de la ACharacter
}

void AThirdPersonMPCharacter::ResetAttackCooldown()
{
	bCanAttack = true;
}

void AThirdPersonMPCharacter::ResetMovementRestrictions()
{
	bCanMove = true;

}

// Combat System
/////////////////////////////////////////////////////////////



/*
// Called every frame
void AThirdPersonMPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

*/
