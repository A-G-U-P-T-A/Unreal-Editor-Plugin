// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssetAutoArranger.h"
#include "AssetAutoArrangerSettings.h"
#include "AssetAutoArrangerCommands.h"
#include "AssetAutoArrangerStyle.h"
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
#include "Framework/MultiBox/MultiBoxBuilder.h"

#define LOCTEXT_NAMESPACE "FAssetAutoArrangerModule"

void FAssetAutoArrangerModule::StartupModule()
{
    FAssetAutoArrangerStyle::Initialize();
    FAssetAutoArrangerStyle::ReloadTextures();

    FAssetAutoArrangerCommands::Register();
    
    PluginCommands = MakeShareable(new FUICommandList);
    PluginCommands->MapAction(
        FAssetAutoArrangerCommands::Get().PluginAction,
        FExecuteAction::CreateRaw(this, &FAssetAutoArrangerModule::PluginButtonClicked),
        FCanExecuteAction());

    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FAssetAutoArrangerModule::RegisterMenus));
    
    RegisterSettings();
}

void FAssetAutoArrangerModule::ShutdownModule()
{
    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);
    
    FAssetAutoArrangerCommands::Unregister();
    FAssetAutoArrangerStyle::Shutdown();
    
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

void FAssetAutoArrangerModule::PluginButtonClicked()
{
    OnButtonClicked();
}

void FAssetAutoArrangerModule::RegisterMenus()
{
    FToolMenuOwnerScoped OwnerScoped(this);

    UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
    {
        FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
        {
            FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FAssetAutoArrangerCommands::Get().PluginAction));
            Entry.SetCommandList(PluginCommands);
        }
    }
}

void FAssetAutoArrangerModule::AddToolbarButton(FToolBarBuilder& Builder)
{
    Builder.AddToolBarButton(
        FAssetAutoArrangerCommands::Get().PluginAction,
        NAME_None,
        LOCTEXT("AutoArrangeButton_Label", "Auto Arrange Assets"),
        LOCTEXT("AutoArrangeButton_Tooltip", "Auto arrange assets based on class types"),
        FSlateIcon(FAssetAutoArrangerStyle::GetStyleSetName(), "AssetAutoArranger.PluginAction")
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
                        //AssetViewUtils::SetPathColor(*DestinationFolder, LinearColor);
                    }
                }
                break;
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FAssetAutoArrangerModule, AssetAutoArranger)