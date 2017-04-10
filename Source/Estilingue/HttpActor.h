// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "DataStruct.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "HttpActor.generated.h"

UCLASS()
class ESTILINGUE_API AHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	
	AHttpActor();

	FHttpModule* Http;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "msg")
	FString msg;
	UPROPERTY()
	FTimerHandle HttpTimerHandle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "network")
		void HttpCall(TArray<FFileContent> files);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccess);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wSendingFiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* SendingFiles;
	
		FTimerDelegate TimerDel;
	UPROPERTY()
		FTimerHandle TimerHandle;
	UFUNCTION()
		void sendFile(TArray<FFileContent> files);

	UFUNCTION(BlueprintCallable, Category = "network")
		void LoadWidget();

	

	int32 filesCount;
	int32 sentFiles;
	
	UFUNCTION(BlueprintCallable, Category = "network")
		FString sentFilesFeedBack();

	UFUNCTION(BlueprintCallable, Category = "network")
		FString ConnectionFeedBack(FString ConnectionStatus);

	UPROPERTY(BluePrintReadOnly, Category = "network")
		FString Status = "Enviando Arquivos";

	UPROPERTY(BluePrintReadOnly, Category = "network")
		bool isDone;

	

	void OnProgressRequest(FHttpRequestPtr Request, int32 BytesSend, int32 InBytesReceived);


};
