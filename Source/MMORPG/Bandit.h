//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
//#include "Bandit.generated.h"
//
//// Enum pentru starea banditului
//UENUM(BlueprintType)
//enum class EBanditStatus : uint8
//{
//    Idle        UMETA(DisplayName = "Idle"),
//    Attacking   UMETA(DisplayName = "Attacking"),
//    Chasing     UMETA(DisplayName = "Chasing"),
//    Fleeing     UMETA(DisplayName = "Fleeing"),
//    Dead        UMETA(DisplayName = "Dead")
//};
//
//UCLASS()
//class YOURPROJECT_API ABandit : public ACharacter
//{
//    GENERATED_BODY()
//
//public:
//    ABandit();
//
//protected:
//    virtual void BeginPlay() override;
//
//public:
//    // Atribute de bază pentru Bandit
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bandit Stats")
//    float MaxHealth;
//
//    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bandit Stats")
//    float CurrentHealth;
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bandit Stats")
//    float AttackDamage;
//
//    // Informații descriptive
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bandit Info")
//    FString Description;
//
//    // Starea curentă a banditului
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bandit State")
//    EBanditStatus BanditStatus;
//
//    // Metodă pentru a iniția un atac
//    UFUNCTION(BlueprintCallable, Category = "Bandit Actions")
//    void Attack();
//
//    // Metodă pentru a primi daune
//    UFUNCTION(BlueprintCallable, Category = "Bandit Actions")
//    void ReceiveDamage(float DamageAmount);
//
//    // Metodă pentru a trata moartea banditului
//    UFUNCTION(BlueprintCallable, Category = "Bandit Actions")
//    void Die();
//
//    // (Opțional) Ai putea adăuga metode suplimentare pentru comportament, AI, replicare etc.
//};
