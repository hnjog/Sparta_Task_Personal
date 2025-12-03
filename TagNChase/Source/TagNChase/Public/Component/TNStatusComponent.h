// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TNStatusComponent.generated.h"

UENUM(BlueprintType)
enum class ERoleType : uint8
{
	None,
	Police,
	Thief,
	Civilian,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentHPChangedDelegate, float /*InCurrentHP*/);
DECLARE_MULTICAST_DELEGATE(FOnOutOfCurrentHPDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxHPChangedDelegate, float /*InMaxHP*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoleChangedDelegate, ERoleType /*InRole*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAGNCHASE_API UTNStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTNStatusComponent();

	float ApplyDamage(float InDamage);

	float GetCurrentHP() const { return CurrentHP; }
	void SetCurrentHP(float InCurrentHP);

	float GetMaxHP() const { return MaxHP; }
	void SetMaxHP(float InMaxHP);

	ERoleType GetRole() const { return Role; }
	void SetRole(ERoleType InRole);

	void ApplyProtect();
	void UseProtect();

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_CurrentHP();

	UFUNCTION()
	void OnRep_MaxHP();

	UFUNCTION()
	void OnRep_Role();

public:
	FOnCurrentHPChangedDelegate OnCurrentHPChanged;

	FOnOutOfCurrentHPDelegate OnOutOfCurrentHP;

	FOnMaxHPChangedDelegate OnMaxHPChanged;

	FOnRoleChangedDelegate OnRoleChanged;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHP)
	float CurrentHP;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHP)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Role)
	ERoleType Role;

	// 당장은 보호막 용도를 표현할 생각은 없기에 테스트 용
	bool bProtect;
};
