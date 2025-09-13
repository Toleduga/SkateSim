#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SkateCharacter.generated.h"


UCLASS()
class SKATERSIM_API ASkateCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta= (AllowPrivateAcess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcess = "true"))
	class USpringArmComponent* SpringArm;



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


	// --- Push (empuje de skate) ---
	UPROPERTY(EditAnywhere, Category = "Skate|Push")
	float PushStrength = 1200.f;   // ajusta a tu gusto (700–1200 suele ir bien)

	UPROPERTY(EditAnywhere, Category = "Skate|Push")
	float MinStrength = 900.f;   // ajusta a tu gusto (700–1200 suele ir bien)

	float Strength = 900.f;
	float PushThreshold = 0.45f;  // umbral de “toque” hacia adelante
	float BrakeThreshold = -0.35f;       // umbral “freno” (Y negativo)
	float BrakeStrength = 1600.f;        // qué tan rápido se come la velocidad al frenar
	FVector SteeringInputY;

	UPROPERTY(EditAnywhere, Category = "Skate|Push")
	float TurnRateDegPerSec = 20.f;   // grados por segundo a plena X (ajusta 120–360)

	
	// Guardar fricción original para restaurar
	float SavedGroundFriction = -1.f;
	float SavedBrakingFrictionFactor = -1.f;
	float SavedBrakingDecel = -1.f;
	
	// Estado para detectar flanco del botón adelante
	bool bIsRolling = false;             // estamos “rodando” tras un push



public:
	// Sets default values for this character's properties
	ASkateCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	void PushInput();
	void PushInputEnd();
	void JumpInput();
	void BreakInput();
	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);

};
