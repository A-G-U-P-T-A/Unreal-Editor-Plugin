// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "EditorActionStyle.h"

class FEditorActionCommands : public TCommands<FEditorActionCommands>
{
public:

	FEditorActionCommands()
		: TCommands<FEditorActionCommands>(TEXT("EditorAction"), NSLOCTEXT("Contexts", "EditorAction", "EditorAction Plugin"), NAME_None, FEditorActionStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};