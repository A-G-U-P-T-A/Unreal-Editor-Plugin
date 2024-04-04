// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestEditorCommands.h"

#define LOCTEXT_NAMESPACE "FTestEditorModule"

void FTestEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "TestEditor", "Execute TestEditor action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
