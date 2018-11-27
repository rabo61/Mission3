// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class MISSION3_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimSequence* idle;
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimSequence* walk;
	UCameraComponent* ourCamera;
	class AItem* item;
	AActor* enemy;
	class ANecromorph* necromorph;

	FVector position;

	float distance = 100.0f;
	FVector velocity;
	FVector forwardDirection;
	FVector strafeDirection;
	FVector mouse;
	float mouseSensitivity = 10.0f;
	
	UFUNCTION()
	void Forward(float AxisValue);
	void Strafe(float AxisValue);
	void Pitch(float AxisValue);
	void Yaw(float AxisValue);

	void Grab();
	void shoot();
};
