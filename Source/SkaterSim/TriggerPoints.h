#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TriggerPoints.generated.h"


UCLASS()
class SKATERSIM_API ATriggerPoints : public ATriggerBox
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    int32 PointsToAdd = 10;

    public:
        ATriggerPoints();



    protected:
        virtual void BeginPlay() override;

        UFUNCTION()
        void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

        UFUNCTION()
        void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
