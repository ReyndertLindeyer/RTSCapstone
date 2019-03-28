// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraPawn.h"
#include "UObject/ConstructorHelpers.h" 
#include "Components/InputComponent.h"

// Sets default values
AMyCameraPawn::AMyCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	cameraSensitivity = 4.0f;
	updateScreenTime = 0;
	cameraQESpeed = 4;
	unlocked = false;

	//Create our components
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponenet"));
	
	//Create Root Component
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(0.0000001f);
	SphereComponent->ComponentTags.Add(FName("Player"));
	SphereComponent->SetSimulatePhysics(false);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Create spring arm
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));

	//Swet the location of the spring arm as 50 units above the root and looking 60 degrees down
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(-60.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 4000.f;
	OurCameraSpringArm->bEnableCameraLag = false;
	OurCameraSpringArm->bDoCollisionTest = false;
	OurCameraSpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	//Create the camara and attach it to the spring arm
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->SetupAttachment(OurCameraSpringArm, USpringArmComponent::SocketName);

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));

	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMyCameraPawn::BeginPlay()
{
	//Get screen size when created
	Super::BeginPlay();
	rtsPC = Cast<AMyRTSPlayerController>(GetController());
	rtsPC->GetViewportSize(screenSizeX, screenSizeY);
}

// Called every frame
void AMyCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (rtsPC->updateScreen || updateScreenTime > 0.0) {
		UpdateScreenSize();
		if (rtsPC->updateScreen) {
			updateScreenTime = 1.0;
		}
		updateScreenTime -= DeltaTime;
	}

	//Get whether the right click is held down or not
	unlocked= rtsPC->unlockCamera;

	//Check to see if the right mouse button is pressed down so that the mouse isn't always changing the pitch and yaw
	if (unlocked) {
		//Rotate our camera's pitch and yaw, but limit the pitch so we're always looking downward
		FRotator NewRotation = OurCameraSpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y * 3, -85.0f, -25.0f);
		OurCameraSpringArm->SetWorldRotation(NewRotation); 
		NewRotation = RootComponent->GetComponentRotation();
		NewRotation.Yaw += CameraInput.X * 2;
		RootComponent->SetWorldRotation(NewRotation);
	}
	if (qOrE != 0) {
		//Rotate our camera's pitch and yaw, but limit the pitch so we're always looking downward
		FRotator NewRotation = RootComponent->GetComponentRotation();
		NewRotation.Yaw += qOrE * cameraQESpeed;
		RootComponent->SetWorldRotation(NewRotation);
	}

	//Zoom controls
	if (zoom == 1 && OurCameraSpringArm->TargetArmLength > 200.0f) {
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(OurCameraSpringArm->TargetArmLength, OurCameraSpringArm->TargetArmLength + 200.0f, -zoom);
	}
	if (zoom == -1 && OurCameraSpringArm->TargetArmLength < 5000.0f) {
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(OurCameraSpringArm->TargetArmLength, OurCameraSpringArm->TargetArmLength - 200.0f, zoom);
	}

	//6 is a magic number that can be replaced by making the sensitivity higher
	FVector moveVec = FVector(MovementInput.X * cameraSensitivity * 6, MovementInput.Y * cameraSensitivity * 6, 0.0);

	//Get the mouse's position and use that to move the camera
	rtsPC->GetMousePosition(mousePos.X, mousePos.Y);

	//Don't want the camera to pan while the player is rotating the screen
	if (!unlocked) {
		//For some reason I had to switch around the X and Y's
		//Using 15 as the margin size
		if (mousePos.X < 15.0f) {
			moveVec.Y += -1 * cameraSensitivity * 6;
		}
		else if (mousePos.X > screenSizeX - 15.0f) {
			moveVec.Y += 1 * cameraSensitivity * 6;
		}

		if (mousePos.Y < 15.0f) {
			moveVec.X += 1 * cameraSensitivity * 6;
		}
		else if (mousePos.Y > screenSizeY - 15.0f) {
			moveVec.X += -1 * cameraSensitivity * 6;
		}
	}

	RootComponent->AddLocalOffset(moveVec);

	zoom = 0;
	qOrE = 0;
	MovementInput.ZeroVector;
	CameraInput.ZeroVector;
}

void AMyCameraPawn::UpdateScreenSize() {
	rtsPC->GetViewportSize(screenSizeX, screenSizeY);
}

// Called to bind functionality to input
void AMyCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/// Uneccessary Code.  We can delete this.
	//Rightclick events, currently not working
	//PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &AMyCameraPawn::RightClick);
	//PlayerInputComponent->BindAction("RightClick", IE_Released, this, &AMyCameraPawn::RightClick);

	//Hook up every-frame handling for our four axes
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCameraPawn::MoveX);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCameraPawn::MoveY);
	PlayerInputComponent->BindAxis("PitchCamera", this, &AMyCameraPawn::PitchCamera);
	PlayerInputComponent->BindAxis("YawCamera", this, &AMyCameraPawn::YawCamera);
	PlayerInputComponent->BindAxis("KeyboardYaw", this, &AMyCameraPawn::KeyboardYaw);
	PlayerInputComponent->BindAxis("Zoom", this, &AMyCameraPawn::Zoom);
}

//Input functions
void AMyCameraPawn::MoveX(float AxisValue)
{
	MovementInput.X = AxisValue;
}

void AMyCameraPawn::MoveY(float AxisValue)
{
	MovementInput.Y = AxisValue;
}

void AMyCameraPawn::PitchCamera(float AxisValue)
{
	if (unlocked) {
		CameraInput.Y = AxisValue;
	}
}

void AMyCameraPawn::YawCamera(float AxisValue)
{
	if (unlocked) {
		CameraInput.X = AxisValue;
	}
}

void AMyCameraPawn::KeyboardYaw(float AxisValue)
{
	//Checks to see if the player is pressing Q or E
	qOrE = AxisValue;
}

void AMyCameraPawn::Zoom(float AxisValue)
{
	zoom = AxisValue;
}

/// Code functionality moved to RTSPlayerController
//void AMyCameraPawn::RightClick()
//{
//	unlocked = !unlocked;
//}