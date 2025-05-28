#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

USTRUCT()
struct FTauntData
{
	GENERATED_BODY()

	UPROPERTY()
	ACharacter* PlayerRef;

	UPROPERTY()
	float TauntValue = 0.f;

	UPROPERTY()
	float LastDamageTime = 0.f; // last time damage

	UPROPERTY()
	float TotalDamage = 0.f;
};


UCLASS()
class MMORPG_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Stats")
	float MaxHealth = 100.0f;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 ExperienceGiven = 50;


public:	


	UPROPERTY()
	TArray<AActor*> Hostiles;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void Die();

	void AddTaunt(ACharacter* Damager, float Amount);

	void HealthUpdate();
	
	UPROPERTY()
	TArray<FTauntData> TauntedPlayers;

	float TauntResetTime = 10.0f; // secunde pana se reseteaza agro

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UWidgetComponent* HealthBarWidget;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	FText EnemyDisplayName = FText::FromString("Enemy");

	UFUNCTION()
	void OnRep_CurrentHealth();

	bool bInitializedHealthBar = false;


};
