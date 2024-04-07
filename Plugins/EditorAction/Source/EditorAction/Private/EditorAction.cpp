// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorAction.h"
#include "EditorActionStyle.h"
#include "EditorActionCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName EditorActionTabName("EditorAction");

#define LOCTEXT_NAMESPACE "FEditorActionModule"

void FEditorActionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FEditorActionStyle::Initialize();
	FEditorActionStyle::ReloadTextures();

	FEditorActionCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FEditorActionCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FEditorActionModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FEditorActionModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(EditorActionTabName, FOnSpawnTab::CreateRaw(this, &FEditorActionModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FEditorActionTabTitle", "EditorAction"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FEditorActionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FEditorActionStyle::Shutdown();

	FEditorActionCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(EditorActionTabName);
}

TSharedRef<SDockTab> FEditorActionModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FEditorActionModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("EditorAction.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FEditorActionModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(EditorActionTabName);
}

void FEditorActionModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FEditorActionCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FEditorActionCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorActionModule, EditorAction)