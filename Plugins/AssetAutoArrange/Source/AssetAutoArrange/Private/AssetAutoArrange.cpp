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

    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

    TArray<FAssetData> AssetDataList;
    AssetRegistry.GetAllAssets(AssetDataList);

    for (const FAssetData& AssetData : AssetDataList)
    {
        UClass* AssetClass = AssetData.GetClass();

        for (const FFolderPath& ClassFolderPath : Settings->ClassFolderPaths)
        {
            if (AssetClass->IsChildOf(ClassFolderPath.ClassType))
            {
                FString DestinationFolder = FPaths::ProjectContentDir() / ClassFolderPath.FolderPath;
                FString SourceFilename = AssetData.GetObjectPathString();
                FString DestinationFilename = DestinationFolder / FPaths::GetCleanFilename(SourceFilename);

                // Create the destination folder if it doesn't exist
                IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
                if (!PlatformFile.DirectoryExists(*DestinationFolder))
                {
                    PlatformFile.CreateDirectory(*DestinationFolder);
                }

                // Move the asset file to the destination folder
                if (IFileManager::Get().Move(*DestinationFilename, *SourceFilename))
                {
                    // Update the asset registry
                    FAssetRegistryModule::AssetRenamed(AssetData.GetAsset(), DestinationFilename);

                    // Set the folder color if specified
                    if (ClassFolderPath.FolderColor != FColor::Black)
                    {
                        FLinearColor LinearColor = ClassFolderPath.FolderColor.ReinterpretAsLinear();
                        //AssetViewUtils::SetPathColor(*DestinationFolder, LinearColor);
                    }
                }

                break;
            }
        }
    }
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