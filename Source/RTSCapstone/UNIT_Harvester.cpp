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


	RootComponent->SetWorldScale3D(FVector(0.25f));

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);
	BodyMesh->SetRelativeScale3D(FVector(3.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Game_Assets/Models/yeetHarvyDev.yeetHarvyDev'"));
	UStaticMesh* Asset = MeshAsset.Object;
	BodyMesh->SetStaticMesh(Asset);
	BodyMesh->SetRelativeLocation(FVector(0.0, 0.0f, -120.0f));

	SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Selection Indicator"));
	SelectionIndicator->SetupAttachment(BodyMesh);
	SelectionIndicator->SetVisibility(false);
	SelectionIndicator->SetWorldLocation(GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));

	currentTimer = 0.0f;
	unitState = UNIT_STATE::IDLE;

	currentResources = 0.0f;
	maxResources = 500.0f;

	

}

// Called when the game starts or when spawned
void AUNIT_Harvester::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();
	
	
}

// Called every frame
void AUNIT_Harvester::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Detect all AActors within a Radius
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> ignoreActors;
	TArray<AActor*> outActors;

	ignoreActors.Add(this);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), detectRange, objectTypes, nullptr, ignoreActors, outActors);


	switch (unitState)
	{
	case UNIT_STATE::IDLE:
	case UNIT_STATE::SEEKING:
		DrawDebugSphere(GetWorld(), GetActorLocation(), detectRange, 24, FColor(0, 0, 255));
		//DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
		break;
	case UNIT_STATE::ATTACKING:
		//DrawDebugSphere(GetWorld(), GetActorLocation(), attackRange, 24, FColor(255, 0, 0));
		break;
	case UNIT_STATE::MOVING:
		DrawDebugSphere(GetWorld(), targetMoveDestination, 40.0, 3, FColor(0, 255, 0));  // How close I am to destination
		break;
	}


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

	if (targetNode != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%i Actors In Radius"), entitiesInRange.Num());
	}
	

	// IDLE STATE
	if (unitState == UNIT_STATE::IDLE)
	{
		//UE_LOG(LogTemp, Warning, TEXT("IDLE"));

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

				// Do Nothing?
			}

		}
	}

	// MOVEMENT STATE
	if (unitState == UNIT_STATE::MOVING)
	{

		UE_LOG(LogTemp, Warning, TEXT("MOVING"));
		// Ignore Combat until unit reaches destination


		if (FVector::Dist(GetActorLocation(), targetMoveDestination) < 50.0f)
		{
			//UE_LOG(LogTemp, Warning, TEXT("DESTINATION REACHED"));
			unitState = UNIT_STATE::IDLE;
		}

	}

	// SEEKING STATE
	if (unitState == UNIT_STATE::SEEKING)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SEEKING"));

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
			if (FVector::Dist(GetActorLocation(), targetLocation) > 25.0f)
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
		}

		// Target is out of range: chase it;
		if (FVector::Dist(GetActorLocation(), targetLocation) < 25.0f)
		{
			SetDestination(GetController(), GetActorLocation());

			if (currentTimer >= harvestRate)
			{
				currentTimer = 0.0f;
				targetRefinery->isOccupied = true;

				// Deposit Resources

				/// Temp
				if (currentResources - 20.0f >= 0)
					currentResources -= 20.0f;
				else
					currentResources = 0;

				UE_LOG(LogTemp, Warning, TEXT("Harvester : %f / %f "), currentResources, maxResources);

			}
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
					
					unitState = UNIT_STATE::INTERACTING;
					SetDestination(GetController(), targetRefinery->harvestPt->GetComponentLocation());
					return;
				}
				
			}
			
		}

		

		if (targetNode == nullptr)
			unitState = UNIT_STATE::IDLE;

		else
		{
			FVector targetLocation = Cast<AActor>(targetNode)->GetActorLocation();
			//FVector moveDestination = targetLocation - ((GetActorLocation() - targetLocation) / 2);

			// Target is out of range: chase it;
			if (FVector::Dist(GetActorLocation(), targetLocation) > 25.0f)
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

void AUNIT_Harvester::HarvestNode(AResourceNode* node)
{
	UE_LOG(LogTemp, Warning, TEXT("HARVESTING"));

	if (node != nullptr)
	{
		if (node->RemainingResources() > 0)
		{
			if (currentResources < maxResources)
			{

				currentResources += node->Harvest();
				UE_LOG(LogTemp, Warning, TEXT("Harvester : %f / %f (Resource Node : %f"), currentResources, maxResources, node->RemainingResources());
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
	SelectionIndicator->SetVisibility(state);
}

void AUNIT_Harvester::AttackOrder(II_Entity* target)
{
	/*if (target->DealDamage(attackDamage) == 1)
	{
		targetEntity = nullptr;
	}*/
}

void AUNIT_Harvester::DestroyEntity()
{
	Destroy(this);
}


