#include "SkateRail.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "SkateCharacter.h"

// Sets default values
ASkateRail::ASkateRail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SMRail = CreateDefaultSubobject<UStaticMeshComponent>("SM Rail");
	SMRail->SetupAttachment(RootComponent);

	RailSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Rail Spline"));
	RailSpline->SetupAttachment(SMRail);

	EntryTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Entry Trigger"));
	EntryTrigger->SetupAttachment(RailSpline);
	EntryTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called when the game starts or when spawned
void ASkateRail::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void ASkateRail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkateRail::OnEntryTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Intentar castear al Character
	ASkateCharacter* SkateChar = Cast<ASkateCharacter>(OtherActor);
	if (SkateChar && OtherComp == SkateChar->RailCollider)
	{
		UE_LOG(LogTemp, Warning, TEXT("El personaje ha tocado el rail con el GrindCollider"));

		// Aquí puedes iniciar el grindeo sobre el spline
	}
}

