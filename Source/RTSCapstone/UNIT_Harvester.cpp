// Fill out your copyright notice in the Description page of Project Settings.

#include "UNIT_Harvester.h"

#include "KIsmet/KismetSystemLibrary.h"

#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"


// Sets default values
AUNIT_Harvester::AUNIT_Harvester()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//RootComponent->SetWorldScale3D(FVector(0.25f));
	isSelected = false;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/yeetHarvyDev.yeetHarvyDev'"));
	UStaticMesh* Asset = MeshAsset.Object;
	BodyMesh->SetStaticMesh(Asset);
	BodyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -120.0f));
	BodyMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	BodyMesh->SetRelativeScale3D(FVector(5.0f));
	BodyMesh->SetCanEverAffectNavigation(false);
	//RootComponent = BodyMesh;

	SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selection Indicator"));
	SelectionIndicator->SetupAttachment(BodyMesh);
	SelectionIndicator->SetVisibility(false);
	SelectionIndicator->SetWorldLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));

	currentTimer = 0.0f;
	unitState = UNIT_STATE::IDLE;

	currentResources = 0.0f;
	maxResources = 200.0f;


	//Load our Sound Cue for the sound we created in the editor
	static ConstructorHelpers::FObjectFinder<USoundCue> select(TEXT("/Game/Game_Assets/Sounds/Harvester_Sounds_V1_Fix/Harvester_-_Select_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> order(TEXT("/Game/Game_Assets/Sounds/Harvester_Sounds_V1_Fix/Harvester_-_Harvest_Order_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> death(TEXT("/Game/Game_Assets/Sounds/Harvester_Sounds_V1_Fix/Harvester_-_Death_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> idle(TEXT("/Game/Game_Assets/Sounds/Harvester_Sounds_V1_Fix/Harvester_-_Idle_LOOP_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> accel(TEXT("/Game/Game_Assets/Sounds/Harvester_Sounds_V1_Fix/Harvester_-_Accelerate_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> drive(TEXT("/Game/Game_Assets/Sounds/Harvester_Sounds_V1_Fix/Harvester_-_Drive_LOOP_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> deccel(TEXT("/Game/Game_Assets/Sounds/Harvester_Sounds_V1_Fix/Harvester_-_Decelerate_Cue"));
	static ConstructorHelpers::FObjectFinder<USoundCue> harvest(TEXT("/Game/Game_Assets/Sounds/Harvester_Sounds_V1_Fix/Harvester_-_Harvest_LOOP_Cue"));

	//Store a reference to the Cue asset
	selectCue = select.Object;
	orderCue = order.Object;
	deathCue = death.Object;
	idleCue = idle.Object;
	accelerateCue = accel.Object;
	driveCue = drive.Object;
	deccelerateCue = deccel.Object;
	harvestCue = harvest.Object;

	//Create audio component that will wrap the Cue and allow us to interact with it and it's paramiters
	audioComponentSelect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentOne"));
	audioComponentOrder = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentTwo"));
	audioComponentDeath = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentThree"));
	audioComponentIdle = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentFour"));
	audioComponentAccelerate = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentFive"));
	audioComponentDrive = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentSix"));
	audioComponentDeccelerate = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentSeven"));
	audioComponentHarvest = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentEight"));

	//Stop sound from playing when it's created
	audioComponentSelect->bAutoActivate = false;
	audioComponentOrder->bAutoActivate = false;
	audioComponentDeath->bAutoActivate = false;
	audioComponentIdle->bAutoActivate = false;
	audioComponentAccelerate->bAutoActivate = false;
	audioComponentDrive->bAutoActivate = false;
	audioComponentDeccelerate->bAutoActivate = false;
	audioComponentHarvest->bAutoActivate = false;

	//Attach the audio component so that it follows the unit around
	audioComponentSelect->SetupAttachment(RootComponent);
	audioComponentOrder->SetupAttachment(RootComponent);
	audioComponentDeath->SetupAttachment(RootComponent);
	audioComponentIdle->SetupAttachment(RootComponent);
	audioComponentAccelerate->SetupAttachment(RootComponent);
	audioComponentDrive->SetupAttachment(RootComponent);
	audioComponentDeccelerate->SetupAttachment(RootComponent);
	audioComponentHarvest->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AUNIT_Harvester::BeginPlay()
{
	Super::BeginPlay();

	if (setPlayerOwner != nullptr)
		InitializeEntity(Cast<II_Player>(setPlayerOwner), "Harvester", startingHealth);
	
	SpawnDefaultController();

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->SetAvoidanceEnabled(true);
	GetCharacterMovement()->AvoidanceConsiderationRadius = 800.0f;
	GetCharacterMovement()->SetRVOAvoidanceWeight(1.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void AUNIT_Harvester::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (selectCue->IsValidLowLevelFast()) {
		audioComponentSelect->SetSound(selectCue);
	}

	if (orderCue->IsValidLowLevelFast()) {
		audioComponentOrder->SetSound(orderCue);
	}

	if (deathCue->IsValidLowLevelFast()) {
		audioComponentDeath->SetSound(deathCue);
	}

	if (idleCue->IsValidLowLevelFast()) {
		audioComponentIdle->SetSound(idleCue);
	}
	
	if (accelerateCue->IsValidLowLevelFast()) {
		audioComponentSelect->SetSound(accelerateCue);
	}

	if (driveCue->IsValidLowLevelFast()) {
		audioComponentOrder->SetSound(driveCue);
	}

	if (deccelerateCue->IsValidLowLevelFast()) {
		audioComponentDeath->SetSound(deccelerateCue);
	}

	if (harvestCue->IsValidLowLevelFast()) {
		audioComponentHarvest->SetSound(harvestCue);
	}
}

// Called every frame
void AUNIT_Harvester::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (unitState)
	{
	case UNIT_STATE::IDLE:
		//UE_LOG(LogTemp, Warning, TEXT("IDLE"));
	case UNIT_STATE::SEEKING:
		//DrawDebugSphere(GetWorld(), GetActorLocation(), detectRange, 24, FColor(0, 0, 255));
		//DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
		//UE_LOG(LogTemp, Warning, TEXT("SEEKING"));
		break;
	case UNIT_STATE::ATTACKING:
		//DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
		//UE_LOG(LogTemp, Warning, TEXT("ATTACKING"));
		break;
	case UNIT_STATE::MOVING:
		//DrawDebugSphere(GetWorld(), targetMoveDestination, 40.0, 3, FColor(0, 255, 0));  // How close I am to destination
		//UE_LOG(LogTemp, Warning, TEXT("MOVING"));
		break;
	case UNIT_STATE::INTERACTING:
		//UE_LOG(LogTemp, Warning, TEXT("INTERACTING"));
		break;
	}

	if (!returning)
	{
		// Detect all AActors within a Radius
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		TArray<AActor*> ignoreActors;
		TArray<AActor*> outActors;

		ignoreActors.Add(this);
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), detectRange, objectTypes, nullptr, ignoreActors, outActors);

		// Narrow down all the AActors to only ones with an II_Entity script
		entitiesInRange.Empty();
		for (int i = 0; i < outActors.Num(); i++)
		{
			if (Cast<AResourceNode>(outActors[i]))
			{
				if (!entitiesInRange.Contains(outActors[i]))
				{
					entitiesInRange.Add(outActors[i]);
				}
			}
		}
	}
	
	
	// IDLE STATE
	if (unitState == UNIT_STATE::IDLE)
	{
		//UE_LOG(LogTemp, Warning, TEXT("IDLE"));

		if (currentResources >= maxResources || returning)
			unitState = UNIT_STATE::INTERACTING;

		SetDestination(GetController(), GetActorLocation());

		if (entitiesInRange.Num() > 0)
		{
			/// Check if entities are hostile

			// If there is a target, seek it.
			if (targetNode != nullptr)
			{
				for (int i = 0; i < entitiesInRange.Num(); i++)
				{
					if (FVector::Dist(GetActorLocation(), entitiesInRange[i]->GetActorLocation()) < FVector::Dist(GetActorLocation(), targetNode->GetActorLocation()))
					{
						if (!Cast<AResourceNode>(entitiesInRange[i])->isOccupied)
						{
							targetNode = Cast<AResourceNode>(entitiesInRange[i]);
						}
					}
				}

				unitState = UNIT_STATE::SEEKING;
			}
				

			// If there isn't a target, set a target.
			else
			{
				targetNode = Cast<AResourceNode>(entitiesInRange[0]);
				if (!audioComponentIdle->IsPlaying())
					audioComponentIdle->Play();
				// Do Nothing?
			}

		}
	}

	// MOVEMENT STATE
	if (unitState == UNIT_STATE::MOVING)
	{

		// Ignore Combat until unit reaches destination
		if (FVector::Dist(GetActorLocation(), targetMoveDestination) < 150.0f)
		{
			unitState = UNIT_STATE::IDLE;
		}

	}

	// SEEKING STATE
	if (unitState == UNIT_STATE::SEEKING)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SEEKING"));

		if (currentResources >= maxResources)
			unitState = UNIT_STATE::INTERACTING;

		if (targetNode == nullptr)
		{
			unitState = UNIT_STATE::IDLE;
		}

		else
		{
			if (targetNode->isOccupied) 
			{
				UNIT_STATE::IDLE;
			}

			FVector targetLocation = Cast<AActor>(targetNode)->GetActorLocation();
			//FVector moveDestination = targetLocation - ((GetActorLocation() - targetLocation) / 2);

			// Target is out of range: move towards it.
			if (FVector::Dist(GetActorLocation(), targetLocation) > 150.0f)
			{
				SetDestination(GetController(), targetLocation);
			}

			// Target is in range: Attack it.
			else
			{
				unitState = UNIT_STATE::ATTACKING;
			}
		}
	}

	// Depositing
	if (unitState == UNIT_STATE::INTERACTING)
	{
		FVector targetLocation = targetRefinery->harvestPt->GetComponentLocation();
		//FVector moveDestination = targetLocation - ((GetActorLocation() - targetLocation) / 2);

		if (currentResources <= 0)
		{
			unitState = UNIT_STATE::IDLE;
			returning = false;
		}

		// Target is out of range: chase it;
		if (FVector::Dist(GetActorLocation(), targetLocation) < 150.0f)
		{
			SetDestination(GetController(), GetActorLocation());

			if (currentTimer >= harvestRate)
			{
				currentTimer = 0.0f;
				targetRefinery->isOccupied = true;

				// Deposit Resources

				/// Temp
				if (currentResources - 20.0f >= 0) {
					currentResources -= 20.0f;
					GetEntityOwner()->ChangeResources(20);
				}
				else
					currentResources = 0;

				//UE_LOG(LogTemp, Warning, TEXT("Harvester : %f / %f "), currentResources, maxResources);

				if (!audioComponentHarvest->IsPlaying())
					audioComponentHarvest->Play();
			}
		}

		else
		{
			MoveOrder(GetController(), targetLocation);
		}

		
	}


	// Harvesting
	if (unitState == UNIT_STATE::ATTACKING)
	{

		if (currentResources >= maxResources)
		{
			targetNode = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("Full Resource Load"));
			if (GetEntityOwner() != nullptr)
			{
				ReturnToRefinery();
			}
			
		}

		

		if (targetNode == nullptr)
			unitState = UNIT_STATE::IDLE;

		else
		{
			FVector targetLocation = Cast<AActor>(targetNode)->GetActorLocation();
			//FVector moveDestination = targetLocation - ((GetActorLocation() - targetLocation) / 2);

			// Target is out of range: chase it;
			if (FVector::Dist(GetActorLocation(), targetLocation) > 1000.0f)
			{
				unitState = UNIT_STATE::SEEKING;
				targetNode->isOccupied = false;
			}

			// Target is in range: attack it.
			else
			{
				SetDestination(GetController(), GetActorLocation());

				if (currentTimer >= harvestRate)
				{
					currentTimer = 0.0f;
					targetNode->isOccupied = true;
					HarvestNode(targetNode);
				}
			}
		}

	}

	/// Attack Rate Timer
	// Will count down regardless of whether it's combat or not.
	// Think of it as a cooldown for an attack.
	if (currentTimer < harvestRate) {
		currentTimer += DeltaTime;
	}

}

void AUNIT_Harvester::ReturnToRefinery()
{
	if (targetRefinery == nullptr)
	{
		for (int i = 0; i < GetEntityOwner()->GetBuildings().Num(); i++)
		{
			if (Cast<ABuilding_Refinery>(GetEntityOwner()->GetBuildings()[i]))
			{
				if (targetRefinery == nullptr)
					targetRefinery = Cast<ABuilding_Refinery>(GetEntityOwner()->GetBuildings()[i]);

				else
				{
					if (FVector::Dist(GetEntityOwner()->GetBuildings()[i]->GetActorLocation(), GetActorLocation()) < FVector::Dist(targetRefinery->GetActorLocation(), GetActorLocation()))
					{
						targetRefinery = Cast<ABuilding_Refinery>(GetEntityOwner()->GetBuildings()[i]);
					}
				}

			}
		}
	}

	if (targetRefinery != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Refinery Found.  Returning"));
		MoveOrder(GetController(), targetRefinery->harvestPt->GetComponentLocation());
		returning = true;
		targetNode = nullptr;

		return;
	}
}

void AUNIT_Harvester::ReturnToRefinery(ABuilding_Refinery* refinery)
{
	targetRefinery = refinery;
	ReturnToRefinery();
}

void AUNIT_Harvester::TargetNode(AResourceNode* node)
{
	targetNode = node;
}

void AUNIT_Harvester::HarvestNode(AResourceNode* node)
{
	//UE_LOG(LogTemp, Warning, TEXT("HARVESTING"));

	if (node != nullptr)
	{
		if (node->RemainingResources() > 0)
		{
			if (currentResources < maxResources)
			{

				currentResources += node->Harvest();
				//UE_LOG(LogTemp, Warning, TEXT("Harvester : %f / %f (Resource Node : %f"), currentResources, maxResources, node->RemainingResources());
			}

		}

		else
		{
			targetNode = nullptr;
		}
	}

	else
	{
		targetNode = nullptr;
	}
}

// Called to bind functionality to input
void AUNIT_Harvester::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AUNIT_Harvester::SetSelection(bool state)
{
	isSelected = state;
	SelectionIndicator->SetVisibility(state);
	if (state && !audioComponentSelect->IsPlaying()) {
		audioComponentSelect->Play();
	}
}

bool AUNIT_Harvester::GetSelection() {
	return isSelected;
}


void AUNIT_Harvester::AttackOrder(II_Entity* target)
{
	//targetActor = target->GetActor();
}

void AUNIT_Harvester::DestroyEntity()
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


	if (!UObject::IsValidLowLevel()) return;

	this->K2_DestroyActor();

	//GC
	GEngine->ForceGarbageCollection();
}


