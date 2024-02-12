// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"
#include "DebugHUD.generated.h"

/**
 * 
 */
UCLASS()
class ADebugHUD : public AHUD
{
	GENERATED_BODY()

protected:
	// Construct the debugging HUD, mainly establishing a font to use for display.
	ADebugHUD();


	// Add a FText to the HUD for rendering.
	void AddText(const TCHAR* title, const FText& value)
	{
		RenderStatistic(title, value);
	}

	// Add a float to the HUD for rendering.
	void AddFloat(const TCHAR* title, float value, const FLinearColor& valueColor = FLinearColor::White)
	{
		RenderStatistic(title, FText::AsNumber(value), valueColor);
	}

	// Add an int32 to the HUD for rendering.
	void AddInt(const TCHAR* title, int32 value)
	{
		RenderStatistic(title, FText::AsNumber(value));
	}

	// Add a bool to the HUD for rendering.
	void AddBool(const TCHAR* title, bool value)
	{
		RenderStatistic(title, BoolToText(value), (value == false) ? FLinearColor::Red : FLinearColor::Green);
	}

	void AddVector(const TCHAR* title, FVector value, const FLinearColor& valueColor = FLinearColor::White)
	{
		FCanvasTextItem item0(FVector2D(X, Y), CStringToText(title), MainFont, FLinearColor::White);
		item0.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
		Canvas->DrawItem(item0);
		float offset = 80;
		float xpos = X + HorizontalOffset;

		FCanvasTextItem item1(FVector2D(xpos, Y), FText::AsNumber(value.X), MainFont, valueColor);
		item1.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
		Canvas->DrawItem(item1);
		xpos += offset;

		FCanvasTextItem item2(FVector2D(xpos, Y), FText::AsNumber(value.Y), MainFont, valueColor);
		item2.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
		Canvas->DrawItem(item2);
		xpos += offset;

		FCanvasTextItem item3(FVector2D(xpos, Y), FText::AsNumber(value.Z), MainFont, valueColor);
		item3.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
		Canvas->DrawItem(item3);
		Y += LineHeight;
	}

	// Draw the HUD.
	virtual void DrawHUD() override
	{
		X = 250;
		Y = 50.0f;
	}

	// The horizontal offset to render the statistic values at.
	float HorizontalOffset = 150.0f;

	float Y = 100.0f;
private:

	// Convert a TCHAR pointer to FText.
	FText CStringToText(const TCHAR* text)
	{
		return FText::FromString(text);
	}

	// Convert a bool to FText.
	FText BoolToText(bool value)
	{
		return CStringToText((value == true) ? TEXT("true") : TEXT("false"));
	}

	// Render a statistic onto the HUD canvas.
	void RenderStatistic(const TCHAR* title, const FText& value, const FLinearColor& valueColor = FLinearColor::White)
	{
		FCanvasTextItem item0(FVector2D(X, Y), CStringToText(title), MainFont, FLinearColor::White);
		item0.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
		Canvas->DrawItem(item0);
		FCanvasTextItem item1(FVector2D(X + HorizontalOffset, Y), value, MainFont, valueColor);
		item1.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
		Canvas->DrawItem(item1);
		Y += LineHeight;
	}

	// Font used to render the debug information.
	UPROPERTY(Transient)
		UFont* MainFont = nullptr;

	// The current X coordinate.
	float X = 150.0f;

	// The current Y coordinate.

	// The line height to separate each HUD entry.
	float LineHeight = 16.0f;

};
