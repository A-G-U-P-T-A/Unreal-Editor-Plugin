// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AssetAutoArrangeSettings.generated.h"

/**
 * 
 */
USTRUCT()
struct FFolderPath
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Asset Auto Arrange")
	TSubclassOf<UObject> ClassType;

	UPROPERTY(EditAnywhere, Category = "Asset Auto Arrange")
	FString FolderPath;

	UPROPERTY(EditAnywhere, Category = "Asset Auto Arrange")
	FColor FolderColor = FColor::Red;

	FFolderPath()
	{
		FolderColor = FColor::Red;
	}
};


UCLASS(config=Engine, defaultconfig)
class ASSETAUTOARRANGE_API UAssetAutoArrangeSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Asset Auto Arrange")
	bool bEnableAutoArrange;
	
	UPROPERTY(Config, EditAnywhere, Category = "Asset Auto Arrange")
	TArray<FFolderPath> ClassFolderPaths;
};
