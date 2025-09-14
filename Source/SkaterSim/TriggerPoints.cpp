#include "TriggerPoints.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "SkateCharacter.h"

ATriggerPoints::ATriggerPoints()
{
    OnActorBeginOverlap.AddDynamic(this, &ATriggerPoints::OnBeginOverlap);
    OnActorEndOverlap.AddDynamic(this, &ATriggerPoints::OnEndOverlap);
}

void ATriggerPoints::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("TriggerBox %s listo"), *GetName());
}

void ATriggerPoints::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor->IsA<ACharacter>())
    {
        UE_LOG(LogTemp, Warning, TEXT("Entr� al Trigger: %s"), *OtherActor->GetName());
        // L�gica personalizada ac�
    }
}

void ATriggerPoints::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor->IsA<ACharacter>())
    {
        UE_LOG(LogTemp, Warning, TEXT("Sali� del Trigger: %s"), *OtherActor->GetName());
        if (ASkateCharacter* Character = Cast<ASkateCharacter>(OtherActor))
        {
            UE_LOG(LogTemp, Warning, TEXT("Overlap con %s"), *Character->GetName());
            Character->AddPoints(PointsToAdd);
        }

    }
}

