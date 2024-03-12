// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OutOfBoundsWidget.generated.h"

/**
 * 
 */
UCLASS()
class UOutOfBoundsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
//	//void SetNameOfKiller(const FString& name);
	UPROPERTY(EditAnywhere)
		float Lifetime = 5.0f;
	void ResetTimer();
	UFUNCTION(BlueprintCallable)
	void PlayExitAnimation();
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION()
		void AnimationFinished();

protected:
	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* RespawnTimer;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWigetAnim))
		class UWidgetAnimation* FadeAwayAnim;
	UPROPERTY(EditAnywhere, Category = "Animation")
		float PlaySpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Animation")
		bool RestoreState = true;
	//UPROPERTY(EditAnywhere, Category = "Animation")
	//	EUMGSequencePlayMode PlayMode = EUMGSequencePlayMode::Forward;
	FWidgetAnimationDynamicEvent EndDelegate;
	//UPROPERTY(meta = (BindWidget))
	//class UImage* Background;
//	//UPROPERTY(BlueprintReadWrite)
	float timer = 5.0f;

};
