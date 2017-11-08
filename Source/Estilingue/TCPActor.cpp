// Fill out your copyright notice in the Description page of Project Settings.

#include "Estilingue.h"
#include "Runtime/Core/Public/Serialization/BufferArchive.h"
#include "TCPActor.h"


const FString  ATCPActor::PINGME = FString(TEXT("PingMe"));
const FString  ATCPActor::CONNECTME = FString(TEXT("ConnectMe"));
const FString  ATCPActor::DISCONNECTED = FString(TEXT("Disconnected"));
const FString  ATCPActor::QUIT = FString(TEXT("Quit"));
const FString  ATCPActor::HOST = FString(TEXT("Host"));
const FString  ATCPActor::CLIENT = FString(TEXT("Client"));
const FString  ATCPActor::DONE = FString(TEXT("Done"));
const FString  ATCPActor::MYIP = FString(TEXT("LocalIp"));

// Sets default values
ATCPActor::ATCPActor()
{
 	

}

// Creates the socket for connecting to server
bool ATCPActor::BeginTCPConnection(const FString & SocketName)
{
	ConnectionSocket = CreatesSocket(SocketName);
	//if the socket was not created
	if (!ConnectionSocket) {
		UE_LOG(LogTemp, Warning, TEXT("TCP Socket Listener Socket Could Not Be Created"));
		return false;
	}

	


	/*Connects to the address with the socket*/

	uint32 ip;
	//192.168.25.25 162.243.71.157
	

	FResolveInfo *Resolveinfo = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetHostByName("104.131.118.48");
	//wait until resolve is completed	
	while (!Resolveinfo->IsComplete());

	//sets the server's ip and its port for socket connection
	Resolveinfo->GetResolvedAddress().GetIp(ip);
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip);
	addr->SetPort(40040);
	
	if (ConnectionSocket->Connect(*addr)) {
		UE_LOG(LogTemp, Warning, TEXT("Connected!!!"));
		
		UE_LOG(LogTemp, Warning, TEXT("Connection Description %s "),*ConnectionSocket->GetDescription());
		ConnectionSocket->SetNonBlocking(true);

		switch (ConnectionSocket->GetConnectionState()) {
		case ESocketConnectionState::SCS_Connected:
			UE_LOG(LogTemp, Warning, TEXT("Connection State Connected"));
			break;
		case ESocketConnectionState::SCS_ConnectionError:
			UE_LOG(LogTemp, Warning, TEXT("Connection State Error"));
			break;
		case ESocketConnectionState::SCS_NotConnected:
			UE_LOG(LogTemp, Warning, TEXT("Connection State Not connected"));
			break;
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Not Connected =("));
	}


	/*Starts the thread to listen for server messages*/
	GetWorld()->GetTimerManager().SetTimer(TCPReceiverTimerHandle, this, &ATCPActor::TCPSocketListener, 0.01, true);
	
	
	return true;



}
//checks If The Server's IP Is Valid
FSocket * ATCPActor::CreatesSocket(const FString & SocketName)
{
	
	FSocket* ListenSocket = FTcpSocketBuilder(*SocketName);//.AsReusable().BoundToEndpoint(EndPoint).Listening(8);
	
	return ListenSocket;
}



/*Listener state machine*/
void ATCPActor::TCPSocketListener()
{

	//UE_LOG(LogTemp, Warning, TEXT("begin"));
	if (!ConnectionSocket) {
		UE_LOG(LogTemp, Warning, TEXT("Listener off"));
		return;
	}


	//Array of Binary
	TArray<uint8> ReceivedData;

	/*Receives messages from server and react to them*/

	uint32 Size;
	while (ConnectionSocket->HasPendingData(Size)) {
		ReceivedData.Init(1, FMath::Min(Size, 65507u));
		UE_LOG(LogTemp, Warning, TEXT("Data Num! %d"), ReceivedData.Num());
		int32 Read = 0;

		ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);

	}

	if (ReceivedData.Num() <= 0) {
		//UE_LOG(LogTemp, Warning, TEXT("No Data Received"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Total Data Read! %d"), ReceivedData.Num());


	const FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);
	UE_LOG(LogTemp, Warning, TEXT("Data = %s"), *ReceivedUE4String);
	if (ReceivedUE4String == PINGME) {
		SendMsg(PINGME);
	}
	else if (ReceivedUE4String == DISCONNECTED) {
		//TO DO
		ConnectionSocket->Close();
	}
	else if (ReceivedUE4String.Contains(CLIENT)) {
		TArray<FString> Ip;
		
		ReceivedUE4String.ParseIntoArray(Ip, TEXT("-"),1);

		UE_LOG(LogTemp, Warning, TEXT("ip ? %s"), *Ip[1]);
		
		IpForConnection = Ip[1];
		SendMsg(QUIT);
		
		GetWorld()->GetTimerManager().SetTimer(TCPOpenConnectionTimerHandle, this, &ATCPActor::OpenConnection, 0.5, true);
	}
	else if (ReceivedUE4String == HOST) {
		SendMsg(QUIT);
		UGameplayStatics::OpenLevel(GetWorld(), "Lobby", false, "?listen");
	}
	else if (ReceivedUE4String.Contains(DONE)) {
		SendMsg(CONNECTME);
	}
	else if (ReceivedUE4String == MYIP) {
		bool canBind = false;
		
		UE_LOG(LogAndroid, Warning, TEXT("msg"));
		
		TSharedRef<FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
		
		if (localIp->IsValid()) {
			FString LocalIpMessage = "LocalIP ";
			UE_LOG(LogAndroid,Warning,TEXT("Local IP %s "), *localIp->ToString(false))
			GLog->Log(localIp->ToString(false));
			LocalIpMessage += localIp->ToString(false);
			SendMsg(LocalIpMessage);
		}
		else
			UE_LOG(LogAndroid, Warning, TEXT("Not valid "));
	}

}



/* Creates String from given binary array*/
FString ATCPActor::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	BinaryArray.Add(0);//Add a 0 termination
	
	return FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryArray.GetData() )));
}


/* Launch the Socket Connection*/
void ATCPActor::Launch()
{
	if (!BeginTCPConnection("mysocket")) {
		UE_LOG(LogTemp, Warning, TEXT("TCP Socket  Listener NOT Created"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("TCP Socket  Listener Created"));

}

//Closes connection and destroys Actor
void ATCPActor::CloseConnection()
{
	if (ConnectionSocket) {
		SendMsg(QUIT);
		ConnectionSocket->Close();
		
	}
	Destroy();
	
}

/*This function is responsible to send messages to the server.*/
bool ATCPActor::SendMsg(FString Msg)
{
	FBufferArchive BinaryArrayArchive;

	BinaryArrayArchive.Flush();

	BinaryArrayArchive.Empty();
	
	BinaryArrayArchive << Msg;

	int32 Size = BinaryArrayArchive.Num();

	int32 BytesSent = 0;

	bool PendingData;

	ConnectionSocket->HasPendingConnection(PendingData);

	return ConnectionSocket->Send(BinaryArrayArchive.GetData(), Size, BytesSent);


}

/*End play function*/

void ATCPActor::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	//End Play
	
	Super::EndPlay(EndPlayReason);
	CloseConnection();

	
}




/*Try to  open connection with given ip*/
void ATCPActor::OpenConnection() {
	if (!IpForConnection.IsEmpty()) {
		UE_LOG(LogTemp, Warning, TEXT("Trying to open connection"));
		APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		FString Command = FString(TEXT("open "));
		Command += IpForConnection;
		PController->ConsoleCommand(Command, true);
		//UE_LOG(LogTemp, Warning, TEXT("Command %s"), *Command);
		//UE_LOG(LogTemp, Warning, TEXT("not connecting"));
		

	}
		
}