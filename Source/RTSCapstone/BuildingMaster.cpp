// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingMaster.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABuildingMaster::ABuildingMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinderOptional<UParticleSystem> DustPS(TEXT("ParticleSystem'/Game/Game_Assets/Particle_Systems/P_BuildingDust.P_BuildingDust'"));
	dustParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MyPSC"));
	dustParticleComp->SetTemplate(DustPS.Get());
	dustParticleComp->bAutoActivate = false;

	buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	RootComponent = buildingMesh;
	buildingMesh->SetWorldScale3D(FVector(2, 2, 2));

	//Create the building area decal and sets the material, has to rotate by -90 for some reason
	decal = CreateDefaultSubobject<UDecalComponent>(TEXT("buildAreaDecal"));
	//decal->SetDecalMaterial (ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/MAT_Decal_RoundBuildingRadius")).Get());
	//decal->CreateDynamicMaterialInstance();
	decal->RelativeRotation = FRotator(-90, 0, 0);
	decal->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	decalMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/MAT_Decal_RoundBuildingRadius")).Get();


	selectedDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("selectionDecal"));
	//selectedDecal->SetDecalMaterial(ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/MAT_Decal_SelectionCircle")).Get());
	//selectedDecal->CreateDynamicMaterialInstance();
	selectedDecal->RelativeRotation = FRotator(-90, 0, 0);
	selectedDecal->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	selectedDecal->SetupAttachment(RootComponent);

	selectMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("/Game/Game_Assets/Materials/MAT_Decal_SelectionCircle")).Get();

	decal->SetVisibility(false);
	selectedDecal->SetVisibility(false);

	constructed = false;
	buildRadiusActive = false;
	team = 1;
	spawnTime = 1.0f;
	buildRadius = 0.0f;
	sightRadius = 300;

	buildingMesh->ComponentTags.Add(FName("Building"));
}

void ABuildingMaster::EnableBuildDecal()
{
	decal->SetVisibility(true);
	PrimaryActorTick.bCanEverTick = true;
	buildRadiusActive = true;
}

void ABuildingMaster::DisableBuildDecal()
{
	decal->SetVisibility(false);
	PrimaryActorTick.bCanEverTick = false;
	buildRadiusActive = false;
}

int32 ABuildingMaster::GetHeight()
{
	return buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Z;
}

float ABuildingMaster::GetConstructionRadius()
{
	return buildRadius;
}

float ABuildingMaster::GetSightRadius()
{
	return sightRadius;
}

void ABuildingMaster::SetSelection(bool selectionType)
{
	selectedDecal->SetVisibility(selectionType);
	selected = selectionType;
}

bool ABuildingMaster::IsSelected()
{
	return selected;
}

bool ABuildingMaster::IsDead()
{
	return false;
}

void ABuildingMaster::Suicide()
{
	//Destroy self and play particle effect
	Destroy();
	//In future replace this with destructing mesh that lowers into ground before deletion
}

// Called when the game starts or when spawned
void ABuildingMaster::BeginPlay()
{
	Super::BeginPlay();

	DynamicMaterialInstA = UMaterialInstanceDynamic::Create(decalMaterial, decal);
	decal->SetDecalMaterial(DynamicMaterialInstA);

	DynamicMaterialInstB = UMaterialInstanceDynamic::Create(selectMaterial, selectedDecal);
	selectedDecal->SetDecalMaterial(DynamicMaterialInstB);

	if(buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Y > buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.X)
		selectedDecal->DecalSize = FVector(200, buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Y + 20, buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Y + 20);
	else
		selectedDecal->DecalSize = FVector(200, buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.X + 20, buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.X + 20);

}

// Called every frame
void ABuildingMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	isDestructable = SetDestructible;

	if (constructed == false) {
		buildingMesh->SetWorldLocation(FMath::VInterpTo(buildingMesh->GetComponentLocation(), FVector(buildingMesh->GetComponentLocation().X, buildingMesh->GetComponentLocation().Y, tempHeight), DeltaTime, spawnTime));
		if (buildingMesh->GetComponentLocation().Z >= tempHeight) {
			dustParticleComp->DeactivateSystem();
			constructed = true;
			PrimaryActorTick.bCanEverTick = false;
		}
	}
}

UStaticMeshComponent * ABuildingMaster::GetBuildingMesh()
{
	return buildingMesh;
}

bool ABuildingMaster::constructAtLocation(II_Player* player)
{
	dustParticleComp->SetWorldLocation(this->GetActorLocation());
	dustParticleComp->ActivateSystem();
	tempHeight = RootComponent->GetComponentLocation().Z;
	buildingMesh->SetWorldLocation(FVector(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y, RootComponent->GetComponentLocation().Z - buildingMesh->CalcBounds(buildingMesh->GetRelativeTransform()).BoxExtent.Z));
	
	
	return false;
}

void ABuildingMaster::InitializeStructure(II_Player* player_, FString name, float health)
{
	InitializeEntity(player_, name, health);
}

void ABuildingMaster::DestroyEntity() 
{
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

		if (GetEntityOwner()->GetSelectedBuilding() == this)
		{
			GetEntityOwner()->SetSelectedBuilding(nullptr);
		}
	}

	GetWorld()->SpawnActor<AExplosiveActor>(ExplosionBlueprint, buildingMesh->RelativeLocation, FRotator(0.0f, 0.0f, 0.0f));


	if (!UObject::IsValidLowLevel()) return;

	this->K2_DestroyActor();

	//GC
	GEngine->ForceGarbageCollection();
}