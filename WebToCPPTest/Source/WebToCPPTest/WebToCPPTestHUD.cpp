// Copyright Epic Games, Inc. All Rights Reserved.

#include "WebToCPPTestHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AWebToCPPTestHUD::AWebToCPPTestHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

	static ConstructorHelpers::FClassFinder<UUserWidget> BrowserWidgetBPClass(TEXT("/Game/MyBrowserWidget_WBP"));
	BrowserWidgetClass = BrowserWidgetBPClass.Class;
}

void AWebToCPPTestHUD::BeginPlay()
{
	Super::BeginPlay();

	if (BrowserWidgetClass)
	{
		BrowserWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), BrowserWidgetClass);
		if (BrowserWidgetInstance)
		{
			BrowserWidgetInstance->AddToViewport();
			UE_LOG(LogTemp, Log, TEXT("Browser widget added to viewport"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create browser widget"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BrowserWidgetClass is null"));
	}
}

void AWebToCPPTestHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}
