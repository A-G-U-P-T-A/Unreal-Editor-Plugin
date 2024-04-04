// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AssetAutoArrangerStyle.h"

class FAssetAutoArrangerCommands : public TCommands<FAssetAutoArrangerCommands>
{
public:
	FAssetAutoArrangerCommands()
		: TCommands<FAssetAutoArrangerCommands>(TEXT("AssetAutoArranger"), NSLOCTEXT("Contexts", "AssetAutoArranger", "AssetAutoArranger Plugin"), NAME_None, FAssetAutoArrangerStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> PluginAction;
};