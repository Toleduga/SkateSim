#include "SkaterSim/SkaterUI.h"
#include "SkateCharacter.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void USkaterUI::SetPoints(int points)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return;

    ASkateCharacter* Skater = Cast<ASkateCharacter>(PC->GetPawn());
    if (!Skater) return;

    points = Skater->Score;
    TextScore->SetText(FText::FromString(FString::FromInt(points)));
}