// Fill out your copyright notice in the Description page of Project Settings.


#include "YCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "YInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "YAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "AI/YAICharacter.h"
#include "EngineUtils.h"

// Sets default values
AYCharacter::AYCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp -> SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UYInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<UYAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	SpawnLocationName_PrimaryAttack = "Muzzle_01";
	SpawnLocationName_AdvancedAttack = "Muzzle_03";
	SpawnLocationName_Dash = "Muzzle_01";
}

// Called when the game starts or when spawned
void AYCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 40.0f;
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -50.0f;

	AttributeComp->OnHealthChanged.AddDynamic(this, &AYCharacter::OnHealthChanged);
}

// Called every frame
void AYCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AYCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AYCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AYCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AYCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AYCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("AdvancedAttack", IE_Pressed, this, &AYCharacter::AdvancedAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AYCharacter::Dash);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AYCharacter::PrimaryInteract);
}

FVector AYCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

//向前移动
void AYCharacter::MoveForward(float AxisValue)
{
	
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// 找出向前方向
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 获取向前矢量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

//向右移动
void AYCharacter::MoveRight(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// 找出向右方向
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// X = Forward (Red)
		// Y = Right (Green)
		// Z = Up (Blue)

		// 获取向右矢量 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// 以该方向增加移动
		AddMovementInput(Direction, AxisValue);
	}
}

//获取攻击的TransformMatrix
void AYCharacter::GetAttackTM(FTransform& SpawnTM, FName SpawnLocationName)
{
	FVector HandLocation = GetMesh()->GetSocketLocation(SpawnLocationName);
	FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector TargetLocation = CameraLocation + (GetControlRotation().Vector() * AttackDistance);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);//检测静态物体

	//对目标方向进行光线追踪，检测物体
	FHitResult Hit;
	FRotator AttackRotation;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, TargetLocation, ObjectQueryParams);

	if (bBlockingHit)
	{
		//若有物体，对作用点方向进行攻击
		UE_LOG(LogTemp, Log, TEXT("The attack target found is %s, at game time %f"), *GetNameSafe(Hit.GetActor()), GetWorld()->TimeSeconds);
		FVector HitPoint = Hit.ImpactPoint;
		AttackRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, HitPoint);
	}
	else
	{
		//若无物体，对默认方向进行攻击
		UE_LOG(LogTemp, Log, TEXT("There is nothing found."));
		AttackRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetLocation);
	}

	SpawnTM = FTransform(AttackRotation, HandLocation);
}

//发射炮弹
void AYCharacter::SpawnAttackProjectile(TSubclassOf<AActor> ProjectileClass, FName SpawnLocationName)
{
	//发射特效
	UGameplayStatics::SpawnEmitterAttached(CastingEffect, GetMesh(), SpawnLocationName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

	//计算
	FTransform SpawnTM;
	GetAttackTM(SpawnTM, SpawnLocationName);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

//初级攻击
void AYCharacter::PrimaryAttack()
{
	PlayAnimMontage(PrimaryAttackAnim);

	//计时
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AYCharacter::PrimaryAttack_TimeElapsed, 0.2f);

}

//计时触发
void AYCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnAttackProjectile(PrimaryProjectileClass, SpawnLocationName_PrimaryAttack);
}

//高级攻击
void AYCharacter::AdvancedAttack()
{
	PlayAnimMontage(AdvancedAttackAnim);

	//计时
	GetWorldTimerManager().SetTimer(TimerHandle_AdvancedAttack, this, &AYCharacter::AdvancedAttack_TimeElapsed, 0.2f);
}

//计时触发
void AYCharacter::AdvancedAttack_TimeElapsed()
{
	SpawnAttackProjectile(AdvancedProjectileClass, SpawnLocationName_AdvancedAttack);
}

//传送
void AYCharacter::Dash()
{
	PlayAnimMontage(DashAnim);

	//计时
	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &AYCharacter::Dash_TimeElapsed, 0.2f);
}

//计时触发
void AYCharacter::Dash_TimeElapsed()
{
	SpawnAttackProjectile(DashProjectileClass, SpawnLocationName_Dash);
}

void AYCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

//生命变动时
void AYCharacter::OnHealthChanged(AActor* InstigatorActor, UYAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Flash"));
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		if (NewHealth <= 0.0f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);
			SetActorEnableCollision(false);
			GetMesh()->SetScalarParameterValueOnMaterials("TimeDissolve", GetWorld()->TimeSeconds);
		}
	}
}

void AYCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}
