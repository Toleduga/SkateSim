#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimator.generated.h"

/**
 * 
 */
UCLASS()
class SKATERSIM_API UMainAnimator : public UAnimInstance
{
	GENERATED_BODY()

private:
	void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool isJump = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool isFall = false;


};
