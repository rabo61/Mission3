// Fill out your copyright notice in the Description page of Project Settings.

#include "Mission3.h"
#include "PlayerPawn.h"
#include "Item.h"
#include "Necromorph.h"
#include "Engine.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	ourCamera = CreateAbstractDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	ourCamera->AttachTo(RootComponent);

	Mesh->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));

	position = GetActorLocation();
	
}

// Called every frame
void APlayerPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	{
		position.X -= (forwardDirection.X + strafeDirection.X) * DeltaTime * 100;
		position.Y += (forwardDirection.Y + strafeDirection.Y) * DeltaTime * 100;
		position.Z = 0;

		/*transform.position.x = player.transform.position.x - dirx*distance;
		transform.position.y = player.transform.position.y + 5 - diry*distance;
		transform.position.z = player.transform.position.z - dirz*distance;*/

		float horDist = distance * cos(mouse.X*(PI / 180));
		float vertDist = distance * sin(mouse.X*(PI / 180));

		float playerRot = mouse.Y*(PI / 180);
		float offsetX = horDist * cos(playerRot);
		float offsetY = horDist * sin(playerRot);
		float offsetZ = vertDist;

		ourCamera->SetRelativeLocation(FVector(-offsetX, -offsetY, -offsetZ + 50.0f));
		ourCamera->SetRelativeRotation(FRotator(mouse.X, mouse.Y, 0.0f));
		Mesh->SetRelativeRotation(FRotator(0.0f, mouse.Y-90, 0.0f));
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		SetActorLocation(position);

		if (velocity.X != 0 || velocity.Y != 0) {
			if(!Mesh->IsPlaying())
			Mesh->PlayAnimation(walk, false);
		}
		else {
			if (!Mesh->IsPlaying())
			Mesh->PlayAnimation(idle, false);
		}

		//UE_LOG(LogTemp, Warning, TEXT("%f"), distance);
	}

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("Forward", this ,&APlayerPawn::Forward);
	InputComponent->BindAxis("Strafe", this, &APlayerPawn::Strafe);
	InputComponent->BindAxis("Pitch", this, &APlayerPawn::Pitch);
	InputComponent->BindAxis("Yaw", this, &APlayerPawn::Yaw);
	InputComponent->BindAction("Grab", IE_Pressed, this, &APlayerPawn::Grab);
	InputComponent->BindAction("shoot", IE_Pressed, this, &APlayerPawn::shoot);

}

void APlayerPawn::Forward(float AxisValue) {
	velocity.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
	forwardDirection.X = cos(mouse.Y*(PI / 180) + PI) * velocity.X;
	forwardDirection.Y = sin(mouse.Y*(PI / 180)) * velocity.X;
}

void APlayerPawn::Strafe(float AxisValue) {
	velocity.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
	strafeDirection.X = sin(mouse.Y*(PI / 180)) * velocity.Y;
	strafeDirection.Y = -cos(mouse.Y*(PI / 180) + PI) * velocity.Y;
}

void APlayerPawn::Pitch(float AxisValue) {
	if(mouse.X > 90) {
		mouse.X = 89;
	}
	else if (mouse.X < -90) {
		mouse.X = -89;
	}else{
		mouse.X += FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)*mouseSensitivity;
	}
}

void APlayerPawn::Yaw(float AxisValue){
	mouse.Y += FMath::Clamp<float>(AxisValue, -1.0f, 1.0f)*mouseSensitivity;
}

void APlayerPawn::Grab(){
	FHitResult* hitresult = new FHitResult();
	FVector start = ourCamera->GetComponentLocation();
	FVector forwardVector = FVector(GetActorLocation().X - ourCamera->GetComponentLocation().X,GetActorLocation().Y - ourCamera->GetComponentLocation().Y,GetActorLocation().Z+50.0f - ourCamera->GetComponentLocation().Z);
	FVector end = (forwardVector * 100.0f) + start;
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 1.0f);

	if (GetWorld()->LineTraceSingleByChannel(*hitresult, start, end, ECC_Visibility, *TraceParams)){
		//GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("%s"), *hitresult->GetComponent()->GetName()));
		FVector ptoi = FVector(0, 0, 0);
		ptoi.X = hitresult->ImpactPoint.X - GetActorLocation().X;
		ptoi.Y = hitresult->ImpactPoint.Y - GetActorLocation().Y;
		ptoi.Z = hitresult->ImpactPoint.Z - GetActorLocation().Z;

		float distance = sqrt(ptoi.X*ptoi.X + ptoi.Y*ptoi.Y + ptoi.Z*ptoi.Z);

		//UE_LOG(LogTemp, Warning, TEXT("%f"), distance);

		if (distance < 50) {
			item = (AItem*)hitresult->GetActor();
			item->respawn();
			UE_LOG(LogTemp, Warning, TEXT("pick up!"));
		}
	}
}

void APlayerPawn::shoot() {
	FHitResult* hitresult = new FHitResult();
	FVector start = ourCamera->GetComponentLocation();
	FVector forwardVector = FVector(GetActorLocation().X - ourCamera->GetComponentLocation().X, GetActorLocation().Y - ourCamera->GetComponentLocation().Y, GetActorLocation().Z + 50.0f - ourCamera->GetComponentLocation().Z);
	FVector end = (forwardVector * 100.0f) + start;
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 1.0f);

	if (GetWorld()->LineTraceSingleByChannel(*hitresult, start, end, ECC_Visibility, *TraceParams)) {
		GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::Printf(TEXT("%s"), *hitresult->GetComponent()->GetName()));

		necromorph->hit(hitresult->GetComponent(),hitresult->GetActor());
	}
}
