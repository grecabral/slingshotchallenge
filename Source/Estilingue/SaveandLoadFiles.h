// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "DataStruct.h"
#include "Runtime/Core/Public/HAL/FileManagerGeneric.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "CoreMisc.h"
#include "Paths.h"
#include "SaveandLoadFiles.generated.h"

/**
 * 
 */
UCLASS()
class ESTILINGUE_API USaveandLoadFiles : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
		

		UFUNCTION(BlueprintCallable, Category = "Data")
		static TArray<FFileContent> LoadDataToString();

		UFUNCTION(BlueprintCallable, Category = "Data")
		static bool saveJsonToFile(FMatchData MatchData);

		UFUNCTION(BlueprintCallable, Category = "Data")
		static bool saveOptionsJsonToFile(FOptionsData OptionsData);

		UFUNCTION()
		static FMatchData loadJsontoStruct(FString info);

		UFUNCTION(BlueprintCallable, Category = "Data")
		static FOptionsData loadOptionsJsonToStruct();

		UFUNCTION()
		static bool DeleteFile(FString path);
	
}; 
