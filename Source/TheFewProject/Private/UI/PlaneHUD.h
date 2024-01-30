// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaneHUD.generated.h"

/**
 * 
 */
class APlanePawn;
UCLASS()
class UPlaneHUD : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetPlaneReference(APlanePawn* ref);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ThrottleBar;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> CrosshairImageClass;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AltitudeValue;

	// compass widget

	APlanePawn* ControlledPlane;
	float* ThrottleRef = NULL;
	UUserWidget* CrosshairWidget = NULL;
};
