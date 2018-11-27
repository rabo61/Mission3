// Fill out your copyright notice in the Description page of Project Settings.

#include "Mission3.h"
#include "SpawnNecromorph.h"
#include "Necromorph.h"


// Sets default values
ASpawnNecromorph::ASpawnNecromorph()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnNecromorph::BeginPlay()
{
	Super::BeginPlay();

	rotation = FRotator(0.0f, 0.0f, 0.0f);

	FActorSpawnParameters SpawnInfo;

	for (int i = 0; i < 3;i++) {
		location = FVector(FMath::RandRange(0.0f, 200.0f), FMath::RandRange(0.0f, 200.0f), 0.0f);
		GetWorld()->SpawnActor<ANecromorph>(location, rotation, SpawnInfo);
	}
	
}

// Called every frame
void ASpawnNecromorph::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

