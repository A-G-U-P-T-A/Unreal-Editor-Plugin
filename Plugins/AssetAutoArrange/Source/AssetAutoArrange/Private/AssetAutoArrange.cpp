// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetAutoArrange.h"
#include "AssetAutoArrangeStyle.h"
#include "AssetAutoArrangeCommands.h"
#include "AssetAutoArrangeSettings.h"
#include "AssetViewUtils.h"
#include "ISettingsModule.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "AssetRegistry/AssetRegistryModule.h"

struct FClassFolderPath;
static const FName AssetAutoArrangeTabName("AssetAutoArrange");

#define LOCTEXT_NAMESPACE "FAssetAutoArrangeModule"

void FAssetAutoArrangeModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterSettings();

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
	UnregisterSettings();
	
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FAssetAutoArrangeStyle::Shutdown();

	FAssetAutoArrangeCommands::Unregister();
}

void FAssetAutoArrangeModule::PluginButtonClicked()
{
	OnButtonClicked();
}

void FAssetAutoArrangeModule::OnButtonClicked()
{
	UAssetAutoArrangeSettings* Settings = GetMutableDefault<UAssetAutoArrangeSettings>();

	UE_LOG(LogTemp, Warning, TEXT("Asset Auto-Arrange Settings:"));
	for (const FFolderPath& ClassFolderPath : Settings->ClassFolderPaths)
	{
		UE_LOG(LogTemp, Warning, TEXT("Class: %s, Folder: %s, Color: %s"),
			*ClassFolderPath.ClassType->GetName(),
			*ClassFolderPath.FolderPath,
			*ClassFolderPath.FolderColor.ToString());

		FString DestinationFolder = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / ClassFolderPath.FolderPath);

		if (ClassFolderPath.FolderColor != FColor::Black)
		{
			FLinearColor LinearColor = ClassFolderPath.FolderColor.ReinterpretAsLinear();
			AssetViewUtils::SetPathColor(*DestinationFolder, LinearColor);

			// Save the color information to the config file
			GConfig->SetString(TEXT("PathColor"), *DestinationFolder, *LinearColor.ToString(), GEditorPerProjectIni);

			// Print the original color and the linear color
			UE_LOG(LogTemp, Warning, TEXT("Set color for directory: %s, Original Color: %s, Linear Color: %s"),
				*DestinationFolder,
				*ClassFolderPath.FolderColor.ToString(),
				*LinearColor.ToString());
		}

		// Create the destination folder if it doesn't exist
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		if (!PlatformFile.DirectoryExists(*DestinationFolder))
		{
			PlatformFile.CreateDirectoryTree(*DestinationFolder);
			UE_LOG(LogTemp, Warning, TEXT("Created directory: %s"), *DestinationFolder);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Directory already exists: %s"), *DestinationFolder);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Asset auto-arrange completed."));
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

void FAssetAutoArrangeModule::RegisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		UAssetAutoArrangeSettings* Settings = GetMutableDefault<UAssetAutoArrangeSettings>();

		SettingsModule->RegisterSettings("Project", "Plugins", "AssetAutoArranger",
			LOCTEXT("AssetAutoArrangerSettingsName", "Asset Auto Arranger"),
			LOCTEXT("AssetAutoArrangerSettingsDescription", "Configure the Asset Auto Arranger plugin settings."),
			Settings);
	}
}

void FAssetAutoArrangeModule::UnregisterSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "AssetAutoArranger");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssetAutoArrangeModule, AssetAutoArrange)