// Fill out your copyright notice in the Description page of Project Settings.

#include "Mission3.h"
#include "Necromorph.h"
#include "PlayerPawn.h"


// Sets default values
ANecromorph::ANecromorph()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	RightArm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightArm"));
	LeftArm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftArm"));
	BodyHitbox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyHitbox"));
	RightArmHitbox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightArmHitbox"));
	LeftArmHitbox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftArmHitbox"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAssetBody(TEXT("SkeletalMesh'/Game/Models/Necromorph/NoArms/NecromorphNoArms.NecromorphNoArms'"));
	BodyAsset = MeshAssetBody.Object;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAssetRightArm(TEXT("SkeletalMesh'/Game/Models/Necromorph/RightArm/RightArm.RightArm'"));
	RightArmAsset = MeshAssetRightArm.Object;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAssetLeftArm(TEXT("SkeletalMesh'/Game/Models/Necromorph/LeftArm/LeftArm.LeftArm'"));
	LeftArmAsset = MeshAssetLeftArm.Object;

	static ConstructorHelpers::FObjectFinder<UAnimSequence>AnimationAssetrun(TEXT("AnimSequence'/Game/Models/Necromorph/NoArms/NecromorphNoArms_Anim_Walk.NecromorphNoArms_Anim_Walk'"));
	run = AnimationAssetrun.Object;

	static ConstructorHelpers::FObjectFinder<UAnimSequence>AnimationAssetrightArm(TEXT("AnimSequence'/Game/Models/Necromorph/RightArm/RightArm_Anim_Walk.RightArm_Anim_Walk'"));
	rightArmRun = AnimationAssetrightArm.Object;

	static ConstructorHelpers::FObjectFinder<UAnimSequence>AnimationAssetleftArm(TEXT("AnimSequence'/Game/Models/Necromorph/LeftArm/LeftArm_Anim_Default_Take.LeftArm_Anim_Default_Take'"));
	leftArmRun = AnimationAssetleftArm.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>AnimationAssetCube(TEXT("StaticMesh'/Game/cube.cube'"));
	BodyHitbox->SetStaticMesh(AnimationAssetCube.Object);
	LeftArmHitbox->SetStaticMesh(AnimationAssetCube.Object);
	RightArmHitbox->SetStaticMesh(AnimationAssetCube.Object);

	Body->SetSkeletalMesh(BodyAsset);
	RightArm->SetSkeletalMesh(RightArmAsset);
	LeftArm->SetSkeletalMesh(LeftArmAsset);

	RootComponent = Body;
	RightArm->AttachTo(Body, TEXT("Shoulder_RSocket"), EAttachLocation::SnapToTarget);
	LeftArm->AttachTo(Body, TEXT("Shoulder_LSocket"), EAttachLocation::SnapToTarget);

	BodyHitbox->AttachTo(Body, TEXT("Body_Socket"), EAttachLocation::SnapToTarget);
	RightArmHitbox->AttachTo(Body, TEXT("Shoulder_RSocket"), EAttachLocation::SnapToTarget);
	LeftArmHitbox->AttachTo(Body, TEXT("Shoulder_LSocket"), EAttachLocation::SnapToTarget);
}

// Called when the game starts or when spawned
void ANecromorph::BeginPlay()
{
	Super::BeginPlay();

	Body->SetWorldScale3D(FVector(7.0f, 7.0f, 7.0f));
	//RightArmHitbox->SetWorldScale3D(FVector(0.05f, 0.05f, 0.05f));
	//LeftArmHitbox->SetWorldScale3D(FVector(0.05f, 0.05f, 0.05f));

	components = this->GetComponents();

	player = (APlayerPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	health = 100;

	rightArmD = false;
	leftArmD = false;

	rightArmH = 0;
	leftArmH = 0;
}

// Called every frame
void ANecromorph::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//UE_LOG(LogTemp, Warning, TEXT("%f / %f / %f"), RighArmHitbox->GetActorLocation().X, RighArmHitbox->GetActorLocation().Y, RighArmHitbox->GetActorLocation().Z);

	if(!Body->IsPlaying())Body->PlayAnimation(run, false);
	if (!RightArm->IsPlaying())RightArm->PlayAnimation(rightArmRun, false);
	if (!LeftArm->IsPlaying())LeftArm->PlayAnimation(leftArmRun, false);
	move(player->GetActorLocation());

	if (rightArmH > 3 && !rightArmD) {
		rightArmP = GetActorLocation();
		rightArmR = GetActorRotation();
		RightArm->SetWorldLocation(FVector(rightArmP.X, rightArmP.Y,0.0f));
		RightArmHitbox->SetWorldLocation(FVector(rightArmP.X, rightArmP.Y, 0.0f));
		RightArm->DetachFromParent(true);
		RightArmHitbox->DetachFromParent(true);
		RightArm->SetWorldRotation(rightArmR);

		rightArmD = true;
		LeftArm->Stop();
	}

	if (leftArmH > 3 && !leftArmD) {
		leftArmP = GetActorLocation();
		leftArmR = GetActorRotation();
		LeftArm->SetWorldLocation(FVector(leftArmP.X, leftArmP.Y, 0.0f));
		LeftArmHitbox->SetWorldLocation(FVector(leftArmP.X, leftArmP.Y, 0.0f));
		LeftArm->DetachFromParent(true);
		LeftArmHitbox->DetachFromParent(true);
		LeftArm->SetWorldRotation(leftArmR);

		leftArmD = true;
		RightArm->Stop();
	}

	if (rightArmD) {
		RightArm->SetWorldLocation(FVector(rightArmP.X, rightArmP.Y, 0.0f));
		RightArm->SetWorldRotation(rightArmR);
	}

	if (leftArmD) {
		LeftArm->SetWorldLocation(FVector(leftArmP.X, leftArmP.Y, 0.0f));
		LeftArm->SetWorldRotation(leftArmR);
	}

	if (health <= 0) {
		respawn();
		health = 100;
	}
}

void ANecromorph::setHealth(int damage) {
	health = health - damage;
}

void ANecromorph::rightArmHit() {
	rightArmH++;
}

void ANecromorph::leftArmHit() {
	leftArmH++;
}

void ANecromorph::hit(UPrimitiveComponent* component,AActor* actor) {
	if (component->GetName().Contains("RightArmHitbox")) {
		ANecromorph* nec = (ANecromorph*)actor;
		nec->setHealth(10);
		nec->rightArmHit();
	}

	if (component->GetName().Contains("LeftArmHitbox")) {
		ANecromorph* nec = (ANecromorph*)actor;
		nec->setHealth(10);
		nec->leftArmHit();
	}

	if (component->GetName().Contains("BodyHitbox")) {
		ANecromorph* nec = (ANecromorph*)actor;
		nec->setHealth(30);
	}

	//UE_LOG(LogTemp, Warning, TEXT("health: %f"),health);
	
}

void ANecromorph::move(FVector playerPosition) {
	moveVector.X = playerPosition.X - GetActorLocation().X;
	moveVector.Y = playerPosition.Y - GetActorLocation().Y;

	moveNormalize = sqrt(moveVector.X*moveVector.X + moveVector.Y*moveVector.Y);

	moveVector.X /= moveNormalize;
	moveVector.Y /= moveNormalize;

	moveAngle = FMath::Atan2(moveVector.Y, moveVector.X);

	//UE_LOG(LogTemp, Warning, TEXT("%f"), moveAngle*(180 / PI));

	SetActorLocation(FVector(GetActorLocation().X + moveVector.X * 1.5f, GetActorLocation().Y + moveVector.Y * 1.5f,15.0f));
	Body->SetWorldRotation(FRotator(0.0f, moveAngle*(180 / PI) - 90, 0.0f));
}

void ANecromorph::respawn() {

	health = 100;

	rightArmD = false;
	leftArmD = false;

	RightArm->AttachTo(Body, TEXT("Shoulder_RSocket"), EAttachLocation::SnapToTarget);
	LeftArm->AttachTo(Body, TEXT("Shoulder_LSocket"), EAttachLocation::SnapToTarget);

	BodyHitbox->AttachTo(Body, TEXT("Body_Socket"), EAttachLocation::SnapToTarget);
	RightArmHitbox->AttachTo(Body, TEXT("Shoulder_RSocket"), EAttachLocation::SnapToTarget);
	LeftArmHitbox->AttachTo(Body, TEXT("Shoulder_LSocket"), EAttachLocation::SnapToTarget);

	rightArmH = 0;
	leftArmH = 0;

	SetActorLocation(FVector(FMath::RandRange(0.0f, 100.0f), FMath::RandRange(-200.0f, 200.0f), 0.0f));
}
