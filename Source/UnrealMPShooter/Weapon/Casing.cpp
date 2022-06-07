// Fill out your copyright notice in the Description page of Project Settings.


#include "Casing.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

ACasing::ACasing()
{
	PrimaryActorTick.bCanEverTick = false;

	CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(CasingMesh);
	CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	CasingMesh->SetSimulatePhysics(true);
	CasingMesh->SetEnableGravity(true);
	CasingMesh->SetNotifyRigidBodyCollision(true);
	ShellEjectionImpulse = 200.f;
	ShellEjectionRotation = 200.f;
	bHasPlayedSound = false;
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();

	CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);
	CasingMesh->AddForce(GetActorForwardVector() * (ShellEjectionImpulse * FMath::RandRange(1, 4)));

	FVector RandomRotation(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), FMath::RandRange(-1, 1));
	CasingMesh->AddTorqueInDegrees(RandomRotation * (ShellEjectionRotation * FMath::RandRange(1, 4)));
}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bHasPlayedSound) return;

	if (ShellSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ShellSound, Hit.ImpactPoint);
		bHasPlayedSound = true;
	}

	FTimerHandle ShellDestructionTimer;
	GetWorldTimerManager().SetTimer(ShellDestructionTimer, this, &ACasing::DestroyShell, 2, false);
}

void ACasing::DestroyShell()
{
	Destroy();
}