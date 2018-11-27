// Fill out your copyright notice in the Description page of Project Settings.

#include "Mission3.h"
#include "Item.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	Mesh->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetRelativeScale3D(FVector(20.0f, 20.0f, 20.0f));

	SetActorLocation(FVector(FMath::RandRange(0.0f, 100.0f), FMath::RandRange(0.0f, 100.0f),0.0f));
	
}

// Called every frame
void AItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AItem::respawn() {
	SetActorLocation(FVector(FMath::RandRange(0.0f, 100.0f), FMath::RandRange(-200.0f, 200.0f), 0.0f));
}

