// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAssetAutoArrangerModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void CreateAutomationButton();

private:
	static void RegisterSettings();
	static void UnregisterSettings();
	void OnButtonClicked();
	void AddToolbarButton(FToolBarBuilder& Builder);
	TSharedPtr<class FUICommandList> PluginCommands;
};