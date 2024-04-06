// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetAutoArrange.h"
#include "AssetAutoArrangeStyle.h"
#include "AssetAutoArrangeCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName AssetAutoArrangeTabName("AssetAutoArrange");

#define LOCTEXT_NAMESPACE "FAssetAutoArrangeModule"

void FAssetAutoArrangeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FAssetAutoArrangeStyle::Initialize();
	FAssetAutoArrangeStyle::ReloadTextures();

	FAssetAutoArrangeCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FAssetAutoArrangeCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FAssetAutoArrangeModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAssetAutoArrangeModule::RegisterMenus));
}

void FAssetAutoArrangeModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FAssetAutoArrangeStyle::Shutdown();

	FAssetAutoArrangeCommands::Unregister();
}

void FAssetAutoArrangeModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FAssetAutoArrangeModule::PluginButtonClicked()")),
							FText::FromString(TEXT("AssetAutoArrange.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FAssetAutoArrangeModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FAssetAutoArrangeCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FAssetAutoArrangeCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssetAutoArrangeModule, AssetAutoArrange)