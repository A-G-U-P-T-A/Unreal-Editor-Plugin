// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "AssetAutoArrangeStyle.h"

class FAssetAutoArrangeCommands : public TCommands<FAssetAutoArrangeCommands>
{
public:

	FAssetAutoArrangeCommands()
		: TCommands<FAssetAutoArrangeCommands>(TEXT("AssetAutoArrange"), NSLOCTEXT("Contexts", "AssetAutoArrange", "AssetAutoArrange Plugin"), NAME_None, FAssetAutoArrangeStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
