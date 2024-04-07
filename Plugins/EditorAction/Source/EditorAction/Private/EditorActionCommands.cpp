// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorActionCommands.h"

#define LOCTEXT_NAMESPACE "FEditorActionModule"

void FEditorActionCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "EditorAction", "Bring up EditorAction window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
