// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WebToCPPTestHUD.generated.h"

UCLASS()
class AWebToCPPTestHUD : public AHUD
{
	GENERATED_BODY()

public:
	AWebToCPPTestHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

