#include "Obstacle.h"
#include "SkateCharacter.h"


AObstacle::AObstacle()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshObstacle = CreateDefaultSubobject<UStaticMeshComponent>("SM Obstacle");
	MeshObstacle->SetupAttachment(RootComponent);

		
}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();

}

void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




