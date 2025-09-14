#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkateRail.generated.h"


UCLASS()
class SKATERSIM_API ASkateRail : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* SMRail;

	UPROPERTY(VisibleAnywhere)
	class USplineComponent* RailSpline;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* EntryTrigger;

public:	
	// Sets default values for this actor's properties
	ASkateRail();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	void OnEntryTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
