// Thang Phung Dinh, 2016, Fog of War implementation for Jump Step Step game
// Feel free to use this  in your game

#include "ProFow.h"
#include "UObject/ConstructorHelpers.h" 


// Sets default values
AProFow::AProFow()
	:m_wholeTextureReagion(0, 0, 0, 0, m_textureSize, m_textureSize)
{
	m_coverSize = 5000;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a planar mesh from engine's planar static mesh
	m_squarePlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fow Plane Static Mesh"));
	RootComponent = m_squarePlane;
	m_squarePlane->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> asset(TEXT("/Engine/ArtTools/RenderToTexture/Meshes/S_1_Unit_Plane.S_1_Unit_Plane"));

		m_squarePlane->SetStaticMesh(asset.Object);
	}
	m_squarePlane->TranslucencySortPriority = 100;
	m_squarePlane->SetRelativeScale3D(FVector(m_coverSize, m_coverSize, 1));

	// Load the base material from your created material
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> asset(TEXT("Material'/Game/Fow/FowMat.FowMat'"));
		m_dynamicMaterial = asset.Object;
	}

	// Create the runtime FOW texture
	if (!m_dynamicTexture)
	{
		m_dynamicTexture = UTexture2D::CreateTransient(m_textureSize, m_textureSize, PF_G8);
		m_dynamicTexture->CompressionSettings = TextureCompressionSettings::TC_Grayscale;
		m_dynamicTexture->SRGB = 0;
		m_dynamicTexture->UpdateResource();
		m_dynamicTexture->MipGenSettings = TMGS_NoMipmaps;
	}

	// Initialise array to all black
	for (int x = 0; x < m_textureSize; ++x)
		for (int y = 0; y < m_textureSize; ++y)
			m_pixelArray[y * m_textureSize + x] = 255;

	// Propagate memory's array to texture
	if (m_dynamicTexture)
		UpdateTextureRegions(m_dynamicTexture, 0, 1, &m_wholeTextureReagion, m_textureSize, 1, m_pixelArray, false);

}

void AProFow::PostInitializeComponents()
{
	Super::PostInitializeComponents();		
		
	// Create a dynamic material instance to swap in the FOW texture
	if (m_dynamicMaterial)
	{
		m_dynamicMaterialInstance = UMaterialInstanceDynamic::Create(m_dynamicMaterial, this);
		m_dynamicMaterialInstance->SetTextureParameterValue("FowTexture", m_dynamicTexture);
	}

	// Set the dynamic material to the mesh
	if (m_dynamicMaterialInstance)
		m_squarePlane->SetMaterial(0, m_dynamicMaterialInstance);
}

void AProFow::setSize(float s)
{
	m_coverSize = s;
	m_squarePlane->SetRelativeScale3D(FVector(m_coverSize, m_coverSize, 1));
}

void AProFow::revealSmoothCircle(const FVector2D &pos, float radius)
{	
	// Calculate the where circle center is inside texture space
	FVector2D texel = pos - FVector2D(GetActorLocation().X, GetActorLocation().Y);
	texel = texel * m_textureSize / m_coverSize;
	texel += FVector2D(m_textureSize / 2, m_textureSize / 2);

	// Calculate radius in texel unit ( 1 is 1 pixel )
	float texelRadius = radius * m_textureSize / m_coverSize;
	
	// The square area to update
	int minX = FMath::Clamp<int>(texel.X - texelRadius, 0, m_textureSize - 1);
	int minY = FMath::Clamp<int>(texel.Y - texelRadius, 0, m_textureSize - 1);
	int maxX = FMath::Clamp<int>(texel.X + texelRadius, 0, m_textureSize - 1);
	int maxY = FMath::Clamp<int>(texel.Y + texelRadius, 0, m_textureSize - 1);
	uint8 theVal = 0;

	// Update our array of fow value in memory
	bool dirty = false;
	//Adding the 2s because otherwise the nested loop would stop too soon
	for (int x = minX; x < maxX + 2; ++x)
	{
		for (int y = minY; y < maxY + 2; ++y)
		{
			float distance = FVector2D::Distance(texel, FVector2D(x, y));
			if (distance < texelRadius)
			{

				uint8 oldVal = m_pixelArray[y * m_textureSize + x];

				//How smooth the transition from seen to unseen blackness is, at lower values it's smoother, but there can be artifacts
				static float smoothPct = 0.6f;
				float lerp = FMath::GetMappedRangeValueClamped(FVector2D(smoothPct, 1.0f), FVector2D(0, 1), distance / texelRadius);
				uint8 newVal = lerp * 255;
				newVal = FMath::Min(newVal, oldVal);

				m_pixelArray[y * m_textureSize + x] = newVal;
				dirty = dirty || oldVal != newVal;
			}
		}
	}

	// Propagate the values in memory's array to texture
	if (dirty)
		UpdateTextureRegions(m_dynamicTexture, 0, 1, &m_wholeTextureReagion, m_textureSize, 1, m_pixelArray, false);
}

void AProFow::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			UpdateTextureRegionsData,
			FUpdateTextureRegionsData*, RegionData, RegionData,
			bool, bFreeData, bFreeData,
			{
				for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
				{
					int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
					if (RegionData->MipIndex >= CurrentFirstMip)
					{
						RHIUpdateTexture2D(
							RegionData->Texture2DResource->GetTexture2DRHI(),
							RegionData->MipIndex - CurrentFirstMip,
							RegionData->Regions[RegionIndex],
							RegionData->SrcPitch,
							RegionData->SrcData
							+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
							+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
							);
					}
				}
		if (bFreeData)
		{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}
}
