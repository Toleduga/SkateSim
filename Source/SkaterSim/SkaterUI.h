#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkaterUI.generated.h"

UCLASS()
class SKATERSIM_API USkaterUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextScore;

	void SetPoints(int points);

};
