// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetAutoArrangeStyle.h"
#include "AssetAutoArrange.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FAssetAutoArrangeStyle::StyleInstance = nullptr;

void FAssetAutoArrangeStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FAssetAutoArrangeStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FAssetAutoArrangeStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("AssetAutoArrangeStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FAssetAutoArrangeStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("AssetAutoArrangeStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("AssetAutoArrange")->GetBaseDir() / TEXT("Resources"));

	Style->Set("AssetAutoArrange.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FAssetAutoArrangeStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FAssetAutoArrangeStyle::Get()
{
	return *StyleInstance;
}
