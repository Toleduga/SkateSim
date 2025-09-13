
#include "SkaterSim/SkateCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASkateCharacter::ASkateCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	Skateboard = CreateDefaultSubobject<UStaticMeshComponent>("Skateboard");
	Skateboard->SetupAttachment(RootComponent);
	GetMesh()->SetupAttachment(Skateboard);
	//Skateboard->SetupAttachment(GetMesh(), FName("SkateBoardSocket"));

}

// Called when the game starts or when spawned
void ASkateCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (auto* Move = GetCharacterMovement())
	{
		SavedGroundFriction = Move->GroundFriction;
		SavedBrakingFrictionFactor = Move->BrakingFrictionFactor;
		SavedBrakingDecel = Move->BrakingDecelerationWalking;

		Move->bOrientRotationToMovement = true;     // que mire hacia donde se mueve
		Move->RotationRate = FRotator(0.f, 540.f, 0.f);
	}
	PrimaryActorTick.bCanEverTick = true;


}

// Called every frame
void ASkateCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsRolling)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Strength = %f"), Strength);
		if (!GetCharacterMovement()->IsFalling() && !bIsFall)
		{
			AddMovementInput(SteeringInputY);
		}	
	}

	


	UCharacterMovementComponent* Move = GetCharacterMovement();
	if (!Move) return;

	if (Move->IsMovingOnGround() && Move->CurrentFloor.bBlockingHit)
	{
		const FVector FloorNormal = Move->CurrentFloor.HitResult.ImpactNormal.GetSafeNormal();

		// Yaw del actor se mantiene (gira solo sobre Z para dirección)
		const float Yaw = GetActorRotation().Yaw;

		// Cuaternión que alinea el "Up" (Z) del mesh con la normal del piso
		const FQuat AlignUp = FQuat::FindBetweenNormals(FVector::UpVector, FloorNormal);

		// Combinamos: primero yaw, luego inclinación por la normal
		const FQuat YawQ = FQuat(FRotator(0.f, Yaw, 0.f));
		const FQuat Target = AlignUp * YawQ;

		// Suavizado opcional
		const float TiltInterpSpeed = 12.f;
		const FQuat NewQuat = FQuat::Slerp(Skateboard->GetComponentQuat(), Target, FMath::Clamp(DeltaTime * TiltInterpSpeed, 0.f, 1.f));

		Skateboard->SetWorldRotation(NewQuat);
	}
	else
	{
		// En aire u otra cosa: vuelve a solo yaw (sin inclinación)
		const float Yaw = GetActorRotation().Yaw;
		const FQuat Target = FQuat(FRotator(0.f, Yaw, 0.f));

		const float RecoverSpeed = 10.f;
		const FQuat NewQuat = FQuat::Slerp(Skateboard->GetComponentQuat(), Target, FMath::Clamp(DeltaTime * RecoverSpeed, 0.f, 1.f));
		Skateboard->SetWorldRotation(NewQuat);
	}

	
}

// Called to bind functionality to input
void ASkateCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(Push, ETriggerEvent::Started, this, &ASkateCharacter::PushInput);
		Input->BindAction(Push, ETriggerEvent::Completed, this, &ASkateCharacter::PushInputEnd);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASkateCharacter::JumpInput);
		Input->BindAction(BreakAction, ETriggerEvent::Triggered, this, &ASkateCharacter::BreakInput);
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASkateCharacter::Move);
	
	}

}

void ASkateCharacter::PushInput()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Pressed input action push");
	GetCharacterMovement()->MaxWalkSpeed =  PushStrength;
	bIsPush = true;
}
void ASkateCharacter::PushInputEnd()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Completed input action push");
	GetCharacterMovement()->MaxWalkSpeed = MinStrength;
	bIsPush = false;
}

void ASkateCharacter::JumpInput()
{
	if (!bCanJumpAgain || !GetCharacterMovement()->IsMovingOnGround())
		return;

	if (bIsPush)
	{
		GetCharacterMovement()->JumpZVelocity = JumpMaxhStrength;
	}
	else
	{
		GetCharacterMovement()->JumpZVelocity = JumpMinhStrength;
	}
	ACharacter::Jump();
	bCanJumpAgain = false;
	GetWorldTimerManager().SetTimer(JumpCooldownTimer, this, &ASkateCharacter::ResetJump, 1.f, false); // 0.3 segundos de cooldown
}

void ASkateCharacter::ResetJump()
{
	bCanJumpAgain = true;
}

void ASkateCharacter::BreakInput()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Pressed input action break");
	bIsRolling = false;
}

void ASkateCharacter::Fall()
{
	bIsFall = true;
	BreakInput();
	GetWorldTimerManager().SetTimer(FallResetTimer, this, &ASkateCharacter::ResetFall, 2.0f, false);
}

void ASkateCharacter::ResetFall()
{
	bIsFall = false;

	const float TeleportDistance = 300.f; 

	FVector BackDirection = -GetActorForwardVector();
	BackDirection.Z = 0.f;
	BackDirection.Normalize();

	FVector NewLocation = GetActorLocation() + BackDirection * TeleportDistance;

	SetActorLocation(NewLocation, true);
}

void ASkateCharacter::Move(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();
	if (!IsValid(Controller)) return;

	//Get forward dirction
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	
	SteeringInputY = ForwardDirection;

	const bool bForwardNow = (InputVector.Y > PushThreshold);

	if (!bIsRolling && InputVector.Y > PushThreshold)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ForwardNow: X=%f"), bForwardNow);
		bIsRolling = true;
		if (PushAnimation)
		{
			//GetMesh()->PlayAnimation(PushAnimation, false);
		}
	}
	// Freno (reduce CruiseSpeed)
	if (bIsRolling && InputVector.Y < BrakeThreshold)
	{
		BreakInput();
	}

	AddControllerYawInput(InputVector.X * TurnRateDegPerSec);
		// Add movement input
		/*
		UE_LOG(LogTemp, Warning, TEXT("InputVector: X=%f, Y=%f"), InputVector.X, InputVector.Y);
		AddMovementInput(ForwardDirection, InputVector.Y);
		AddMovementInput(RightDirection, InputVector.X);
		*/
	
}

void ASkateCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Considerar solo colisiones horizontales (ignorar suelo o techo)
	float VerticalDot = FVector::DotProduct(HitNormal, FVector::UpVector); // = HitNormal.Z

	// Si la normal no apunta mayormente hacia arriba o abajo
	if (FMath::Abs(VerticalDot) < 0.5f)
	{
		Fall();
		
	}
}



