// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};

class USphereComponent;
class UWidgetComponent;
class UAnimationAsset;
class ACasing;
class UTexture2D;

UCLASS()
class UNREALMPSHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void ShowPickupWidget(bool bShowWidget);
	virtual void Fire(const FVector& HitTarget);

	//
	// Textures for the weapon crosshairs
	//

	UPROPERTY(EditAnywhere, Category = Crosshair)
		UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = Crosshair)
		UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = Crosshair)
		UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = Crosshair)
		UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = Crosshair)
		UTexture2D* CrosshairsBottom;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnSphereOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	UFUNCTION()
		virtual void OnSphereEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		USphereComponent* AreaSphere;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
		EWeaponState WeaponState;

	UFUNCTION()
		void OnRep_WeaponState();

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACasing> CasingClass;

	//
	// Zoom configs
	//

	UPROPERTY(EditAnywhere)
		float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
		float ZoomInterpSpeed = 20.f;

public:
	void SetWeaponState(EWeaponState State);
	FORCEINLINE USphereComponent* GetAreaSphere() const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	FORCEINLINE float GetZoomedFOV() const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }
};
