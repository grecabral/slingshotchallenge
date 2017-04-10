// Fill out your copyright notice in the Description page of Project Settings.

#include "Estilingue.h"
#include "SaveandLoadFiles.h"


/*Function responsible to delete files that are stored into the mobile*/
bool USaveandLoadFiles::DeleteFile(FString path) {
	
	FString gamePath;
	gamePath = FPaths::GameContentDir();
	gamePath += "my_data";
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *gamePath)

	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*gamePath))
	{
		return false;
	}
	else {
		gamePath += "/" + path;
		return FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*gamePath);
	}
}
//creates file json from match data struct
bool USaveandLoadFiles::saveJsonToFile(FMatchData MatchData) {

	//file path
	FString path;
	path = FPaths::GameDir();
	path += "/Content/my_data";

	
	//file name
	FString filename = FGuid::NewGuid().ToString() + ".json";

	

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject = FJsonObjectConverter::UStructToJsonObject(MatchData, 0, 0);

	FString OutputString;

	TSharedRef< TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR> > > Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR> >::Create(&OutputString);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer,true);
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*path))
	{

		//creates directory
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*path);
		if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*path)) {
			return false;
		}
	}

	path += "/";
	path += filename;
	return FFileHelper::SaveStringToFile(OutputString, *path, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);

	

}

bool USaveandLoadFiles::saveOptionsJsonToFile(FOptionsData OptionsData) {

	//file path
	FString path;
	path = FPaths::GameDir();
	path += "/Content/my_data";


	//file name
	FString filename = "options.json";



	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject = FJsonObjectConverter::UStructToJsonObject(OptionsData, 0, 0);

	FString OutputString;

	TSharedRef< TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR> > > Writer = TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR> >::Create(&OutputString);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer, true);

	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*path))
	{

		//creates directory
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*path);
		if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*path)) {
			return false;
		}
	}

	path += "/";
	path += filename;
	return FFileHelper::SaveStringToFile(OutputString, *path, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());



}



/*Get the files and return them in an array*/
TArray<FFileContent> USaveandLoadFiles::LoadDataToString() {

	FString info;
	FFileManagerGeneric FileMgr;

	TArray<FString> Files;
	
	FileMgr.SetSandboxEnabled(true);

	FString wildcard("*.json");

	FString path;
	path = FPaths::GameContentDir();
	path += "my_data";
	
	TArray<FFileContent> FileContent;

	//TArray<FString> FilesStrings;
	

	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*path))
	{
		return FileContent;
	}


	//search the directory where the files are stored
	FString searchPath = FString(FPaths::Combine(*FPaths::GameContentDir(),TEXT("my_data"), *wildcard));
	FileMgr.FindFiles(Files, *searchPath, true, true);
	path += "/";

	//Loop through the files and return the json objects.
	
	
	for (FString f : Files) {
		
		FString filepath = path + f;

		// loads file to string and convert them to json object and them convert to the original struct
		
		if (f.Contains("options"))
			continue;

		FFileHelper::LoadFileToString(info, *filepath);
		
		FFileContent fileData;
		fileData.Guid = f;
		fileData.Data = info;

		FileContent.Add(fileData);
		//FilesStrings.Add(info);
		
		//debug data
		UE_LOG(LogTemp, Warning, TEXT("%s"), *info);
		
		/*
		

		loadJsontoStruct(matchdata);*/
		
	}
	
	Files.Empty();
	return FileContent;

}

FOptionsData  USaveandLoadFiles::loadOptionsJsonToStruct() {

	FString JsonString;
	FString path = FPaths::GameDir();
	path += "/Content/my_data/options.json";

	FFileHelper::LoadFileToString(JsonString, *path);

	FOptionsData OptionsData;
	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//UE_LOG(LogTemp, Warning, TEXT("JSON = %s"), (JsonObject->GetBoolField(TEXT("researchMode")) ? TEXT("True") : TEXT("False")));
		//UE_LOG(LogTemp, Warning, TEXT("JSON = %f"), JsonObject->GetNumberField(TEXT("musicLevel")));
		//UE_LOG(LogTemp, Warning, TEXT("JSON = %f"), JsonObject->GetNumberField(TEXT("fXLevel")));
		OptionsData.FXLevel = JsonObject->GetNumberField(TEXT("fXLevel"));
		OptionsData.MusicLevel = JsonObject->GetNumberField(TEXT("musicLevel"));
		OptionsData.ResearchMode = JsonObject->GetBoolField(TEXT("researchMode"));
	}	

	return OptionsData;
}

/*Tranforsm the Jsonfile into struct of matchdata*/
FMatchData  USaveandLoadFiles::loadJsontoStruct(FString info) {

	TSharedPtr<FJsonObject> JsonObject;

	TSharedRef<TJsonReader <> > Reader = TJsonReaderFactory<>::Create(info);

	FJsonSerializer::Deserialize(Reader, JsonObject);

	FMatchData Matchdata;

	FJsonObjectConverter::JsonObjectStringToUStruct(info, &Matchdata, 0, 0);

	return Matchdata;

}