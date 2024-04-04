// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "TestEditorStyle.h"

class FTestEditorCommands : public TCommands<FTestEditorCommands>
{
public:

	FTestEditorCommands()
		: TCommands<FTestEditorCommands>(TEXT("TestEditor"), NSLOCTEXT("Contexts", "TestEditor", "TestEditor Plugin"), NAME_None, FTestEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
