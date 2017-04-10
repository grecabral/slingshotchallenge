// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CoreMisc.h"
#include "DataStruct.generated.h"



/*Here we found all the structs that represents the data we need to send to the server
Later we transform the match data struct into a json file and then send it to the data server*/

/* Options struct */
USTRUCT(BlueprintType)
struct FOptionsData {
	GENERATED_BODY()

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Options")
		float MusicLevel;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Options")
		float FXLevel;

		UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Options")
		bool ResearchMode;


	FOptionsData() {}

};


/* File struct */
USTRUCT(BlueprintType)
struct FFileContent {
	GENERATED_BODY()

		UPROPERTY(BlueprintReadOnly, Category = "File")
		FString Guid;

		UPROPERTY()
		FString Data;


		FFileContent(){}

};


/*Player Struct*/
USTRUCT(BlueprintType)
struct FPlayerData {
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 Age;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		FString Sex;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		FString Locality;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 School;
	

	FPlayerData() {

	}

};

/* Round data Struct*/
USTRUCT(BlueprintType)
struct FRoundData {
	GENERATED_BODY()

	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 Round;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 player1Score;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 player2Score;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 player1Coop;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 player2Coop;
	
	



	FRoundData() {

	}

	 


};

/*Match Data Struct*/
USTRUCT(BlueprintType)
struct FMatchData {
	GENERATED_BODY()

	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		TArray<FPlayerData> Players;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 number_of_rounds;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 coop_points;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 no_coop_points;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 difficulty;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 strategy;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		TArray<FRoundData> rounds;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Data")
		int32 gameFlag;

	FMatchData() {

	}

	



};



UCLASS()
class ESTILINGUE_API ADataStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADataStruct();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
