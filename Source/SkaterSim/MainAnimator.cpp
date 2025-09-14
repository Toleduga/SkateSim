#include "MainAnimator.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "SkateCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"



void UMainAnimator::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* OwnerPawn = TryGetPawnOwner();
    if (!OwnerPawn) return;

    ASkateCharacter* Character = Cast<ASkateCharacter>(OwnerPawn);
    if (!Character) return;

    if (UCharacterMovementComponent* MoveComp = Character->GetCharacterMovement())
    {
        Speed = MoveComp->MaxWalkSpeed;
        isJump = MoveComp->IsFalling();
        isFall = Character->bIsFall;
        //UE_LOG(LogTemp, Warning, TEXT("MaxWalkSpeed = %f"), Speed);

    }
}