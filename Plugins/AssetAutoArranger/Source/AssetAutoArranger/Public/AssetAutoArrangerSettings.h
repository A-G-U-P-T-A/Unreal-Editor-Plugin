// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma once

#include "CoreMinimal.h"
#include "AssetAutoArrangerSettings.generated.h"


USTRUCT()
struct FClassFolderPath
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Asset Auto Arranger")
	TSubclassOf<UObject> ClassType;

	UPROPERTY(EditAnywhere, Category = "Asset Auto Arranger")
	FString FolderPath;

	UPROPERTY(EditAnywhere, Category = "Asset Auto Arranger")
	FColor FolderColor = FColor::Red;

	FClassFolderPath()
	{
		FolderColor = FColor::Red;
	}
};

UCLASS(config=Engine, defaultconfig)
class UAssetAutoArrangerSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Asset Auto Arranger")
	bool bEnableAutoArrange;
	
	UPROPERTY(Config, EditAnywhere, Category = "Asset Auto Arranger")
	TArray<FClassFolderPath> ClassFolderPaths;
};