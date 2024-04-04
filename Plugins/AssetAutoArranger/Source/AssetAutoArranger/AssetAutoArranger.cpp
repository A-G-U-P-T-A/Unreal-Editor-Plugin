// Copyright Epic Games, Inc. All Rights Reserved.

// Copyright Epic Games, Inc. All Rights Reserved.


#include "AssetAutoArranger.h"
#include "AssetAutoArrangerSettings.h"
#include "AssetViewUtils.h"
#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "LevelEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#define LOCTEXT_NAMESPACE "FAssetAutoArrangerModule"

void FAssetAutoArrangerModule::StartupModule()
{
    RegisterSettings();
    CreateAutomationButton();
}

void FAssetAutoArrangerModule::ShutdownModule()
{
    UnregisterSettings();
}

void FAssetAutoArrangerModule::RegisterSettings()
{
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        UAssetAutoArrangerSettings* Settings = GetMutableDefault<UAssetAutoArrangerSettings>();

        SettingsModule->RegisterSettings("Project", "Plugins", "AssetAutoArranger",
            LOCTEXT("AssetAutoArrangerSettingsName", "Asset Auto Arranger"),
            LOCTEXT("AssetAutoArrangerSettingsDescription", "Configure the Asset Auto Arranger plugin settings."),
            Settings);
    }
}

void FAssetAutoArrangerModule::UnregisterSettings()
{
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->UnregisterSettings("Project", "Plugins", "AssetAutoArranger");
    }
}

void FAssetAutoArrangerModule::CreateAutomationButton()
{
    PluginCommands = MakeShareable(new FUICommandList);

    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

    TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
    ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FAssetAutoArrangerModule::AddToolbarButton));

    LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
}

void FAssetAutoArrangerModule::AddToolbarButton(FToolBarBuilder& Builder)
{
    Builder.AddToolBarButton(
        FUIAction(FExecuteAction::CreateRaw(this, &FAssetAutoArrangerModule::OnButtonClicked)),
        NAME_None,
        LOCTEXT("AutoArrangeButton_Label", "Auto Arrange Assets"),
        LOCTEXT("AutoArrangeButton_Tooltip", "Auto arrange assets based on class types"),
        FSlateIcon()
    );
}

void FAssetAutoArrangerModule::OnButtonClicked()
{
    UAssetAutoArrangerSettings* Settings = GetMutableDefault<UAssetAutoArrangerSettings>();

    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

    TArray<FAssetData> AssetDataList;
    AssetRegistry.GetAllAssets(AssetDataList);

    for (const FAssetData& AssetData : AssetDataList)
    {
        UClass* AssetClass = AssetData.GetClass();

        for (const FClassFolderPath& ClassFolderPath : Settings->ClassFolderPaths)
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
                        AssetViewUtils::SetPathColor(*DestinationFolder, LinearColor);
                    }
                }

                break;
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FAssetAutoArrangerModule, AssetAutoArranger)