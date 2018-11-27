// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Necromorph.generated.h"

UCLASS()
class MISSION3_API ANecromorph : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANecromorph();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Body;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BodyHitbox;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* RightArm;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RightArmHitbox;
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* LeftArm;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LeftArmHitbox;

	//UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimSequence* run;
	UAnimSequence* rightArmRun;
	UAnimSequence* leftArmRun;

	UStaticMeshComponent* component;
	TArray<UActorComponent*> components;

	USkeletalMesh* BodyAsset;
	USkeletalMesh* RightArmAsset;
	USkeletalMesh* LeftArmAsset;

	UStaticMesh* AnimationAssetCube;

	class APlayerPawn* player;

	bool rightArmD;
	bool leftArmD;

	int rightArmH;
	int leftArmH;

	FVector rightArmP;
	FVector leftArmP;
	FRotator rightArmR;
	FRotator leftArmR;

	FVector moveVector;
	float moveNormalize;
	float moveAngle;
	int health;

	void setHealth(int damage);
	void rightArmHit();
	void leftArmHit();
	void hit(UPrimitiveComponent* component,AActor* actor);
	void move(FVector playerPosition);
	void respawn();
	
};
