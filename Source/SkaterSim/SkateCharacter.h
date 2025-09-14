#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/BoxComponent.h"
#include "SkateCharacter.generated.h"


UCLASS()
class SKATERSIM_API ASkateCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta= (AllowPrivateAcess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UAnimSequence* MoveAnimation;

	UPROPERTY(EditAnywhere)
	class UAnimSequence* PushAnimation;



protected:

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Skateboard;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* Push;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* BreakAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LookAction;

	
	UPROPERTY(EditAnywhere, Category = "Skate|Push")
	float PushStrength = 1200.f;   

	UPROPERTY(EditAnywhere, Category = "Skate|Push")
	float MinStrength = 900.f;   


	//Jump
	UPROPERTY(EditAnywhere, Category = "Skate|Push")
	float JumpMaxhStrength = 700.f;

	UPROPERTY(EditAnywhere, Category = "Skate|Push")
	float JumpMinhStrength = 300.f;
	FTimerHandle JumpCooldownTimer;
	bool bIsJump = false;
	bool bCanJumpAgain = true;



	float Strength = 900.f;
	float PushThreshold = 0.45f;  
	float BrakeThreshold = -0.35f;       

	UPROPERTY(EditAnywhere, Category = "Skate|Push")
	float BrakeStrength = 900.f;        
	FVector SteeringInputY;

	UPROPERTY(EditAnywhere, Category = "Skate|Push")
	float TurnRateDegPerSec = 0.4f;   

	float SavedGroundFriction = -1.f;
	float SavedBrakingFrictionFactor = -1.f;
	float SavedBrakingDecel = -1.f;
	

	bool bIsRolling = false; 
	bool bIsPush = false;
	

	FTimerHandle FallResetTimer;
	FRotator InitialMeshRelativeRotation;
	int32 Score = 0;


public:

	ASkateCharacter();

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* RailCollider;

	bool bIsFall = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void AddPoints(int32 Amount);
	void GetPoints();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	void PushInput();
	void PushInputEnd();
	void JumpInput();
	void ResetJump();
	void BreakInput();
	void Fall();
	void ResetFall();
	void TeleportPlayerToPlayerStart(AController* PlayerController);
	void ResetRagdoll();
	void Move(const FInputActionValue& InputValue);

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
		bool bSelfMoved, FVector HitLocation, FVector HitNormal,
		FVector NormalImpulse, const FHitResult& Hit) override;

};
