// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetAutoArrangeCommands.h"

#define LOCTEXT_NAMESPACE "FAssetAutoArrangeModule"

void FAssetAutoArrangeCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "AssetAutoArrange", "Execute AssetAutoArrange action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
