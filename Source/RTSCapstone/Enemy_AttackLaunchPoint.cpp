// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_AttackLaunchPoint.h"

// Sets default values
AEnemy_AttackLaunchPoint::AEnemy_AttackLaunchPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	collectionRadius = 800;

	radiusSphere = CreateDefaultSubobject<USphereComponent>(TEXT("baseRadius"));
	radiusSphere->SetSphereRadius(collectionRadius);
	radiusSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	radiusSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_AttackLaunchPoint::BeginOverlap);
	radiusSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy_AttackLaunchPoint::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AEnemy_AttackLaunchPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy_AttackLaunchPoint::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		///Check to see if the colliding unit is an enemy unit, what kind it is, and add it to the correct array
		if (Cast<AUNIT_Grinder>(OtherActor)) {
			basicArrayA.Add(Cast<AUNIT_Grinder>(OtherActor));
		}
		if (Cast<AUNIT_Gattling>(OtherActor)) {
			basicArrayB.Add(Cast<AUNIT_Gattling>(OtherActor));
		}
		if (Cast<AUNIT_Roomba>(OtherActor)) {
			basicArrayC.Add(Cast<AUNIT_Roomba>(OtherActor));
		}
		if (Cast<AUNIT_Prism>(OtherActor)) {
			basicArrayD.Add(Cast<AUNIT_Prism>(OtherActor));
		}
	}
}

void AEnemy_AttackLaunchPoint::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) {
		if (Cast<AUNIT_Grinder>(OtherActor)) {
			basicArrayA.Remove(Cast<AUNIT_Grinder>(OtherActor));
		}
		if (Cast<AUNIT_Gattling>(OtherActor)) {
			basicArrayB.Remove(Cast<AUNIT_Gattling>(OtherActor));
		}
		if (Cast<AUNIT_Roomba>(OtherActor)) {
			basicArrayC.Remove(Cast<AUNIT_Roomba>(OtherActor));
		}
		if (Cast<AUNIT_Prism>(OtherActor)) {
			basicArrayD.Remove(Cast<AUNIT_Prism>(OtherActor));
		}
	}
}

// Called every frame
void AEnemy_AttackLaunchPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	///If all of the arrays are full of the correct unit then launch a wave towards the nearest player structure
	if (basicArrayA.Num() >= numOfBasicMelee && basicArrayB.Num() >= numOfBasicRanged && basicArrayC.Num() >= numOfAdvancedMelee && basicArrayD.Num() >= numOfAdvancedRanged) { //Add more variables for more units
		int32 closestBuilding = 10000;
		AActor* tempActor = nullptr;
		TArray<AActor*> tempArray;

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_PowerPlant::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}				
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_Refinery::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_Barrecks::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_VehicleFactory::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_TechCenter::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_Superweapon::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_Turret_Gattling::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_Turret_Cannon::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_Turret_Artillery::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), ABuilding_Turret_Tesla::StaticClass(), tempArray);
		if (tempArray.Num() > 0) {
			for (int i = 0; i < tempArray.Num(); i++) {
				if (FVector::Dist(this->GetActorLocation(), tempArray[i]->GetActorLocation()) < closestBuilding) {
					tempActor = tempArray[i];
				}
			}
		}

		tempArray.Empty();

		if (tempActor != nullptr) {
			for (int i = 0; i < basicArrayA.Num(); i++) {
				basicArrayA[i]->MoveOrder(basicArrayA[i]->GetController(), tempActor->GetActorLocation());
			}
			for (int i = 0; i < basicArrayB.Num(); i++) {
				basicArrayB[i]->MoveOrder(basicArrayB[i]->GetController(), tempActor->GetActorLocation());
			}
			for (int i = 0; i < basicArrayC.Num(); i++) {
				basicArrayC[i]->MoveOrder(basicArrayC[i]->GetController(), tempActor->GetActorLocation());
			}
		}

	}
}

void AEnemy_AttackLaunchPoint::EnableTick()
{
	PrimaryActorTick.bCanEverTick = true;
}

