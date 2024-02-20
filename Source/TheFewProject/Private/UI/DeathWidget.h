// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class UDeathWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetNameOfKiller(const FString& name);
	UPROPERTY(EditAnywhere)
		float Lifetime = 5.0f;
	void ResetTimer();
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* KillerName;
	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* RespawnTimer;
	//UPROPERTY(meta = (BindWidget))
	//class UImage* Background;
	//UPROPERTY(BlueprintReadWrite)
	float timer = 5.0f;

};
