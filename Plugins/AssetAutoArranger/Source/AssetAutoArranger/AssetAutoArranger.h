// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FAssetAutoArrangerModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void RegisterSettings();

	void UnregisterSettings();
	void PluginButtonClicked();
    
private:
	void RegisterMenus();
	void AddToolbarButton(FToolBarBuilder& Builder);
	void OnButtonClicked();

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};