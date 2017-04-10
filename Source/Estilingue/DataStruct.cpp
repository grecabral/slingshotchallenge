// Fill out your copyright notice in the Description page of Project Settings.

#include "Estilingue.h"
#include "DataStruct.h"


// Sets default values
ADataStruct::ADataStruct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADataStruct::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADataStruct::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

