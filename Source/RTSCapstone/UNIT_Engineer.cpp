// Fill out your copyright notice in the Description page of Project Settings.

#include "UNIT_Engineer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUNIT_Engineer::AUNIT_Engineer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent->SetWorldScale3D(FVector(0.5f));
	isSelected = false;

	SetHitRadius(30);

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 2.5f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	UStaticMesh* Asset = MeshAsset.Object;
	BodyMesh->SetStaticMesh(Asset);
	BodyMesh->SetRelativeLocation(FVector(0.0f));
	BodyMesh->SetCanEverAffectNavigation(false);

	SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selection Indicator"));
	SelectionIndicator->SetupAttachment(BodyMesh);
	SelectionIndicator->SetVisibility(false);
	SelectionIndicator->SetWorldLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));

	currentTimer = 0.0f;
	unitState = UNIT_STATE::IDLE;

	//Load our Sound Cue for the sound we created in the editor
	static ConstructorHelpers::FObjectFinder<USoundCue> use(TEXT("/Game/Game_Assets/Sounds/Infantry_Rifle_Sounds_V1_Fix/Inf_Rifle_-_Fire_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> select(TEXT("/Game/Game_Assets/Sounds/Infantry_Rifle_Sounds_V1_Fix/Inf_Rifle_-_Select_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> order(TEXT("/Game/Game_Assets/Sounds/Infantry_Rifle_Sounds_V1_Fix/Inf_Rifle_-_Attack_Order_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> death(TEXT("/Game/Game_Assets/Sounds/Infantry_Rifle_Sounds_V1_Fix/Inf_Rifle_-_Death_Cue"));

	//Store a reference to the Cue asset
	useCue = use.Object;
	selectCue = select.Object;
	orderCue = order.Object;
	deathCue = death.Object;

	//Create audio component that will wrap the Cue and allow us to interact with it and it's paramiters
	audioComponentUse = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentOne"));
	audioComponentSelect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentTwo"));
	audioComponentOrder = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentThree"));
	audioComponentDeath = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentFour"));

	//Stop sound from playing when it's created
	audioComponentUse->bAutoActivate = false;
	audioComponentSelect->bAutoActivate = false;
	audioComponentOrder->bAutoActivate = false;
	audioComponentDeath->bAutoActivate = false;

	//Attach the audio component so that it follows the unit around
	audioComponentUse->SetupAttachment(RootComponent);
	audioComponentSelect->SetupAttachment(RootComponent);
	audioComponentOrder->SetupAttachment(RootComponent);
	audioComponentDeath->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AUNIT_Engineer::BeginPlay()
{
	Super::BeginPlay();

	if (setPlayerOwner != nullptr)
		InitializeEntity(Cast<II_Player>(setPlayerOwner), "Engineer", startingHealth);

	SpawnDefaultController();

	overrideAI = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->SetAvoidanceEnabled(true);
	GetCharacterMovement()->AvoidanceConsiderationRadius = 200.0f;
	GetCharacterMovement()->SetRVOAvoidanceWeight(0.5f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void AUNIT_Engineer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (useCue->IsValidLowLevelFast()) {
		audioComponentUse->SetSound(useCue);
	}

	if (selectCue->IsValidLowLevelFast()) {
		audioComponentSelect->SetSound(selectCue);
	}

	if (orderCue->IsValidLowLevelFast()) {
		audioComponentOrder->SetSound(orderCue);
	}

	if (deathCue->IsValidLowLevelFast()) {
		audioComponentDeath->SetSound(deathCue);
	}
}

// Called every frame
void AUNIT_Engineer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	isDestructable = SetDestructible;

	// Detect all AActors within a Radius
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> ignoreActors;
	TArray<AActor*> outActors;

	ignoreActors.Add(this);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), detectRange, objectTypes, nullptr, ignoreActors, outActors);


	switch (unitState)
	{
	case UNIT_STATE::IDLE:
		DrawDebugSphere(GetWorld(), GetActorLocation(), detectRange, 24, FColor(0, 0, 255));
	case UNIT_STATE::SEEKING:
		//DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(1, 1, 1));
		break;
	case UNIT_STATE::ATTACKING:
		//DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
		break;
	case UNIT_STATE::MOVING:
		DrawDebugSphere(GetWorld(), targetMoveDestination, 40.0, 3, FColor(0, 255, 0));  // How close I am to destination
		break;
	}


	// Narrow down all the AActors to only ones with an II_Entity script
	if(actorsToHeal.Num() > 0){
		for (int i = 0; i < actorsToHeal.Num(); i++)
		{
			if (Cast<II_Entity>(actorsToHeal[i])->GetEntityOwner() == GetEntityOwner() && GetEntityOwner()->teamValue == Cast<II_Entity>(actorsToHeal[i])->GetEntityOwner()->teamValue)
			{
				Cast<II_Entity>(actorsToHeal[i])->SetIsBeingHealed(false);
			}
		}
	}
	entitiesInRange.Empty();
	actorsToHeal.Empty();
	for (int i = 0; i < outActors.Num(); i++)
	{
		if (Cast<II_Entity>(outActors[i]))
		{
			if (!entitiesInRange.Contains(outActors[i]))
			{
				entitiesInRange.Add(outActors[i]);
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("%i Actors In Radius"), entitiesInRange.Num());

	// IDLE STATE
	if (unitState == UNIT_STATE::IDLE)
	{
		SetDestination(GetController(), GetActorLocation());

		if (entitiesInRange.Num() > 0)
		{
			/// Check if entities are hostile

			// If there is a target, seek it.
			//if (targetActor != nullptr)
				//unitState = UNIT_STATE::SEEKING;

			// If there isn't a target, set a target.
			//else
			//{
				// Do Nothing?
			//}
			
			for (int i = 0; i < entitiesInRange.Num(); i++)
			{
				if (Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner() == GetEntityOwner() && GetEntityOwner()->teamValue == Cast<II_Entity>(entitiesInRange[i])->GetEntityOwner()->teamValue)
				{
					if (!Cast<II_Entity>(entitiesInRange[i])->GetIsBeingHealed()) {
						Cast<II_Entity>(entitiesInRange[i])->SetIsBeingHealed(true);
						if (Cast<II_Entity>(entitiesInRange[i])->GetCurrentHealth() < Cast<II_Entity>(entitiesInRange[i])->GetMaxHealth() - 10) {
							Cast<II_Entity>(entitiesInRange[i])->SetCurrentHealth(Cast<II_Entity>(entitiesInRange[i])->GetCurrentHealth() + 10);
							UE_LOG(LogTemp, Warning, TEXT("Healed Someone"));
						}
						actorsToHeal.Add(entitiesInRange[i]);
					}
				}
			}
		}
	}

	// MOVEMENT STATE
	if (unitState == UNIT_STATE::MOVING)
	{
		// Ignore Combat until unit reaches destination
		FHitResult* rayCastOne = new FHitResult();
		FHitResult* rayCastTwo = new FHitResult();

		FVector StartTrace = BodyMesh->GetComponentLocation() + (BodyMesh->GetForwardVector() * 60);

		FVector ForwardVectorOne = BodyMesh->GetForwardVector();
		FVector ForwardVectorTwo = BodyMesh->GetForwardVector();

		ForwardVectorOne = ForwardVectorOne.RotateAngleAxis(20, FVector(0.0, 0.0, 1.0));
		ForwardVectorTwo = ForwardVectorTwo.RotateAngleAxis(-20, FVector(0.0, 0.0, 1.0));

		FVector EndTraceOne = ((ForwardVectorOne * 100) + StartTrace);
		FVector EndTraceTwo = ((ForwardVectorTwo * 100) + StartTrace);

		FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

		DrawDebugLine(GetWorld(), StartTrace, EndTraceOne, FColor(255, 0, 0), false, 1);
		if (GetWorld()->LineTraceSingleByChannel(*rayCastOne, StartTrace, EndTraceOne, ECC_Visibility, *TraceParams)) {
			if (Cast<II_Unit>(rayCastOne->GetActor())) {
				FVector push = (rayCastOne->GetActor()->GetActorLocation() - GetActorLocation());
				push = FVector(push.X / 4, push.Y / 4, push.Z) + (BodyMesh->GetRightVector() * 2);
				rayCastOne->GetActor()->SetActorLocation(rayCastOne->GetActor()->GetActorLocation() + push);
			}
		}

		DrawDebugLine(GetWorld(), StartTrace, EndTraceTwo, FColor(255, 0, 0), false, 1);
		if (GetWorld()->LineTraceSingleByChannel(*rayCastTwo, StartTrace, EndTraceTwo, ECC_Visibility, *TraceParams)) {
			if (Cast<II_Unit>(rayCastTwo->GetActor())) {
				FVector push = (rayCastTwo->GetActor()->GetActorLocation() - GetActorLocation());
				push = FVector(push.X / 4, push.Y / 4, push.Z) + (-BodyMesh->GetRightVector() * 2);
				rayCastTwo->GetActor()->SetActorLocation(rayCastTwo->GetActor()->GetActorLocation() + push);
			}
		}

		if (FVector::Dist(GetActorLocation(), targetMoveDestination) < 80.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("DESTINATION REACHED"));
			unitState = UNIT_STATE::IDLE;
			overrideAI = false;
		}

	}

	// SEEKING STATE
	//if (unitState == UNIT_STATE::SEEKING)
	//{
		/*
		if (targetActor == nullptr)
		{
			unitState = UNIT_STATE::IDLE;
		}

		else
		{
			//FVector targetLocation = Cast<AActor>(targetEntity)->GetActorLocation();
			//FVector moveDestination = targetLocation - ((GetActorLocation() - targetLocation) / 2);

			//// Target is out of range: move towards it.
			//if (FVector::Dist(GetActorLocation(), targetLocation) > attackRange + Cast<II_Entity>(targetActor)->GetHitRadius())
			//{
			//	SetDestination(GetController(), moveDestination);
			//}

			//// Target is in range: Attack it.
			//else
			//{
			//	unitState = UNIT_STATE::ATTACKING;
			//}
		}
		*/
	//}


	// ATTACK STATE
	//if (unitState == UNIT_STATE::ATTACKING)
	{
		/*
		if (targetActor == nullptr)
			unitState = UNIT_STATE::IDLE;

		else
		{
			//FVector targetLocation = Cast<AActor>(targetEntity)->GetActorLocation();
			//FVector moveDestination = targetLocation - ((GetActorLocation() - targetLocation) / 2);

			//// Target is out of range: chase it;
			//if (FVector::Dist(GetActorLocation(), targetLocation) > attackRange + Cast<II_Entity>(targetActor)->GetHitRadius())
			//{
			//	unitState = UNIT_STATE::SEEKING;
			//}

			//// Target is in range: attack it.
			//else
			//{
			//	SetDestination(GetController(), GetActorLocation());

			//	if (currentTimer >= 1.0f)
			//	{
			//		currentTimer = 0.0f;
			//		AttackOrder(targetEntity);
			//	}
			//}
		}
		*/
	}

	/// Attack Rate Timer
	// Will count down regardless of whether it's combat or not.
	// Think of it as a cooldown for an attack.
	if (currentTimer < 5.0f) {
		currentTimer += DeltaTime;
	}

}

// Called to bind functionality to input
void AUNIT_Engineer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUNIT_Engineer::ResetTarget()
{
	targetActor = nullptr;
}

void AUNIT_Engineer::SetSelection(bool state)
{
	isSelected = state;
	SelectionIndicator->SetVisibility(state);
	if (state) {
		audioComponentSelect->Play();
	}
}

bool AUNIT_Engineer::GetSelection() {
	return isSelected;
}


void AUNIT_Engineer::AttackOrder(II_Entity* target)
{
	//targetActor = target->GetActor();
}

void AUNIT_Engineer::DestroyEntity()
{
	audioComponentDeath->Play();
	// Remove from Owner's Array
	if (GetEntityOwner() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("I have died"));
		if (GetEntityOwner()->GetUnits().Contains(this))
		{
			for (int i = 0; i < GetEntityOwner()->GetUnits().Num(); i++) {
				if (GetEntityOwner()->GetUnits()[i] == this)
					GetEntityOwner()->RemoveUnitAtIndex(i);
			}
		}

		if (GetEntityOwner()->GetBuildings().Contains(this))
		{
			for (int i = 0; i < GetEntityOwner()->GetBuildings().Num(); i++) {
				if (GetEntityOwner()->GetBuildings()[i] == this)
					GetEntityOwner()->RemoveBuildingAtIndex(i);
			}
		}

		if (GetEntityOwner()->GetSelectedCharacters().Contains(this))
		{
			for (int i = 0; i < GetEntityOwner()->GetSelectedCharacters().Num(); i++) {
				if (GetEntityOwner()->GetSelectedCharacters()[i] == this)
					GetEntityOwner()->RemoveSelectedCharacterAtIndex(i);
			}
		}
	}
	if (actorsToHeal.Num() > 0) {
		for (int i = 0; i < actorsToHeal.Num(); i++)
		{
				Cast<II_Entity>(entitiesInRange[i])->SetIsBeingHealed(false);
		}
	}
	entitiesInRange.Empty();
	actorsToHeal.Empty();


	if (!UObject::IsValidLowLevel()) return;

	this->K2_DestroyActor();

	//GC
	GEngine->ForceGarbageCollection();
}
