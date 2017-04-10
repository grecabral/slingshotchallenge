// Fill out your copyright notice in the Description page of Project Settings.

#include "Estilingue.h"
#include "SaveandLoadFiles.h"
#include "Blueprint/UserWidget.h"
#include "HttpActor.h"


// Sets default values
AHttpActor::AHttpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Http = &FHttpModule::Get();
	sentFiles = 0;

}

// Called when the game starts or when spawned
void AHttpActor::BeginPlay()
{
	//HttpCall();
	isDone = true;
	Super::BeginPlay();

	

	
}

// Called every frame
void AHttpActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

/*Http Call, Here we create a timer to send files */
void AHttpActor::HttpCall(TArray<FFileContent> files)
{
	
	sentFiles = 0;
	filesCount = files.Num();
	if (filesCount == 0)
		return;

	isDone = false;

	TimerDel.BindUFunction(this, FName("sendFile"), files);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 2.f, true);
	//sendFile(files);

	//http://192.168.25.25:5000
	
	

}

//trocar ip depois
//Here we create the http request and send the file to data server
void AHttpActor::sendFile(TArray<FFileContent> files) {

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResponseReceived);
	Request->OnRequestProgress().BindUObject(this, &AHttpActor::OnProgressRequest);
	//https://hidden-plateau-77550.herokuapp.com
	Request->SetURL("http://hidden-plateau-77550.herokuapp.com:80/json_post/");
	
	Request->SetVerb("POST");
	
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");

	Request->SetHeader("Content-Type", TEXT("application/json"));
	
	UE_LOG(LogTemp, Warning, TEXT("%d"),files.Num());
	

	if (filesCount > 0 && !isDone) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *files[filesCount - 1].Guid);
		
		Request->AppendToHeader(TEXT("ue4guid"), files[filesCount - 1].Guid);
		Request->SetContentAsString(files[filesCount - 1].Data);
		if(Request->ProcessRequest())
			UE_LOG(LogTemp, Warning, TEXT("Start"));
		
		
		
		filesCount--;
	}
	else {
		//if we get here is because all the requests are done.
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		//Status = sentFilesFeedBack();
		if (sentFiles == files.Num()) {
			//todos os arquivos foram enviados.
			isDone = true;
			Status = "Todos Arquivos Enviados";
			//SendingFiles->SetVisibility(ESlateVisibility::Hidden);
			
		}
		else {
			isDone = true;
			Status = "Falha No Envio De arquivos";
			//if (SendingFiles){
				//SendingFiles->SetVisibility(ESlateVisibility::Hidden);
			//}
		}
	}
		
	
	


}
//we load a widget responsible to show a button where the researcher can send the files
void AHttpActor::LoadWidget()
{

	if (wSendingFiles) {
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		SendingFiles = CreateWidget<UUserWidget>(PC, wSendingFiles);

		if (SendingFiles) {
			SendingFiles->AddToViewport();
		}
	}


}

//feedback message.
FString AHttpActor::sentFilesFeedBack()
{
	
	return FString(TEXT(" %d Arquivos Enviados"),sentFiles);
}

//connection feedback
FString AHttpActor::ConnectionFeedBack(FString ConnectionStatus)
{
	Status = ConnectionStatus;
	return ConnectionStatus;
}

//function that launchs when a request is in progress
void AHttpActor::OnProgressRequest(FHttpRequestPtr Request, int32 BytesSend, int32 InBytesReceived)
{

	UE_LOG(LogTemp, Warning, TEXT("Response progress"));

	
}


//At a received response, we check its status and then if it is a ok status we delete the file from storage.
void AHttpActor::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccess)
{
	
	UE_LOG(LogTemp, Warning, TEXT("Response Received"));
	//UE_LOG(LogTemp, Warning, TEXT("Response %d"), EHttpResponseCodes::IsOk(Response->GetResponseCode()));
	
	
	if (Request->GetResponse().IsValid()) {
		Response->GetResponseCode();

		switch (Request->GetResponse()->GetResponseCode())
		{

		case EHttpResponseCodes::Type::Ok:
			USaveandLoadFiles::DeleteFile(Response->GetContentAsString());
			sentFiles++;
			ConnectionFeedBack(Response->GetContentAsString());
			
			UE_LOG(LogTemp, Warning, TEXT("Arquivo Enviado"));
			break;

		case EHttpResponseCodes::Type::RequestTimeout:
			ConnectionFeedBack("Conexao Timed Out");
			UE_LOG(LogTemp, Warning, TEXT("Conexao Timed Out"));
			isDone = true;
			break;

		case EHttpResponseCodes::Type::ServerError:
			ConnectionFeedBack("Server Error");
			UE_LOG(LogTemp, Warning, TEXT("Server Error"));
			isDone = true;
			break;
		}

	
	}
		
	
	switch (Request->GetStatus()) {

	case EHttpRequestStatus::Failed:
		UE_LOG(LogTemp, Warning, TEXT("Request Failed"));
		ConnectionFeedBack("Falha Na Requisicao");
		isDone = true;
		break;
	case EHttpRequestStatus::Failed_ConnectionError:
		UE_LOG(LogTemp, Warning, TEXT("Failed to Connect"));
		Request->CancelRequest();
		ConnectionFeedBack("Falha Na Connexao");
		isDone = true;
		break;
	case EHttpRequestStatus::Processing:
		UE_LOG(LogTemp, Warning, TEXT("Processing Request"));
		ConnectionFeedBack("Processando...");
		break;
	case EHttpRequestStatus::Succeeded:
		
		UE_LOG(LogTemp, Warning, TEXT("Request Succeded"));
		ConnectionFeedBack("Requisicao Sucedida");
		break;

	}

}

