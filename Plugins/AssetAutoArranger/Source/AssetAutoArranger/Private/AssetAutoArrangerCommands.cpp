// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetAutoArrangerCommands.h"

#define LOCTEXT_NAMESPACE "FAssetAutoArrangerModule"

void FAssetAutoArrangerCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "AssetAutoArranger", "Execute AssetAutoArranger action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE