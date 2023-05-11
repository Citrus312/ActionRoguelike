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
#include "YActionComponent.h"

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

	ActionComp = CreateDefaultSubobject<UYActionComponent>("ActionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

}

// Called when the game starts or when spawned
void AYCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax = 40.0f;
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin = -50.0f;

	AttributeComp->OnHealthChanged.AddDynamic(this, &AYCharacter::OnHealthChanged);
	AttributeComp->OnRageChanged.AddDynamic(this, &AYCharacter::OnRageChanged);
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
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AYCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AYCharacter::SprintStop);

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

void AYCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AYCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

//初级攻击
void AYCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

//高级攻击
void AYCharacter::AdvancedAttack()
{
	ActionComp->StartActionByName(this, "AdvancedAttack");
// 	if (AttributeComp->IsRageMax())
// 	{
// 		if (ActionComp->StartActionByName(this, "AdvancedAttack"))
// 		{
// 			AttributeComp->ClearRage();
// 		}
// 	}
// 	else
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Rage Not Enough");
// 	}
}

//传送
void AYCharacter::Dash()
{
	ActionComp->StartActionByName(this, "Dash");
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
		//UE_LOG(LogTemp, Warning, TEXT("Character Flash"));
		FString DebugMsg = GetNameSafe(InstigatorActor) + " hit " + GetNameSafe(this) + ", Damage : "+ FString::SanitizeFloat(Delta);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White,  DebugMsg);
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		

		if (NewHealth <= 0.0f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);
			//SetActorEnableCollision(false);
			GetMesh()->SetScalarParameterValueOnMaterials("TimeDissolve", GetWorld()->TimeSeconds);

			SetLifeSpan(5.0f);
		}
	}
}

void AYCharacter::OnRageChanged(AActor* InstigatorActor, UYAttributeComponent* OwningComp, float NewRage, float Delta)
{
	if (OwningComp->IsRageMax())
	{
		// Some effect
	}
}

void AYCharacter::HealSelf(float Amount /* = 100 */)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}
