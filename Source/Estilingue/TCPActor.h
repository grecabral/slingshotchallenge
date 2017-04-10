// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Networking.h"
#include "GameFramework/Actor.h"
#include "TCPActor.generated.h"

UCLASS()
class ESTILINGUE_API ATCPActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATCPActor();
	static const FString PINGME;
	static const FString CONNECTME;
	static const FString DISCONNECTED;
	static const FString QUIT;
	static const FString HOST;
	static const FString CLIENT;
	static const FString DONE;
	static const FString MYIP;
	//static const FString 

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	FSocket* ConnectionSocket;
	//FSocket* ConnectionSocket;
	FIPv4Endpoint  RemoteAddressConnection;
	FString IpForConnection;

	UPROPERTY()
		FTimerHandle TCPConnectionListenerTimerHandle;
	UPROPERTY()
		FTimerHandle TCPReceiverTimerHandle;
	UPROPERTY()
		FTimerHandle TCPOpenConnectionTimerHandle;

	bool BeginTCPConnection(const FString& SocketName);

	FSocket* CreatesSocket(const FString& SocketName);

	void TCPSocketListener();



	FString StringFromBinaryArray(TArray<uint8> BinaryArray);
	
	UFUNCTION(BlueprintCallable, Category = "TCP")
		void Launch();
	UFUNCTION(BlueprintCallable, Category = "TCP")
		void CloseConnection();

	bool SendMsg(FString Msg);


	void OpenConnection();
	
};
