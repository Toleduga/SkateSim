#pragma once

#include "CoreMinimal.h"
#include "TriggerPoints.h"
#include "Obstacle.generated.h"

UCLASS()
class SKATERSIM_API AObstacle : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshObstacle;


public:	
	// Sets default values for this actor's properties
	AObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};
