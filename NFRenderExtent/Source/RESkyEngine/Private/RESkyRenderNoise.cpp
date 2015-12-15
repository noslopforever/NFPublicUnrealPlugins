#include "RESkyEnginePrivatePCH.h"
#include "RESkyRenderEngine.h"
#include "ShaderParameterUtils.h"
#include "RHIStaticStates.h"

#include "DynamicMeshBuilder.h"
#include "ShaderParameters.h"
#include "RefCounting.h"

#include "noise/noisegen.h"


//------------------------------------------------------------------------
const double FRECloudCommon::TEMPCloudMainNoiseInitMultiplier = 0.125;
const double FRECloudCommon::TEMPCloudMainNoiseLacunarity = 2.0;
const double FRECloudCommon::TEMPCloudDetailNoiseInitMultiplier = 0.03125;
const double FRECloudCommon::TEMPCloudDetailNoiseLacunarity = 2.0;
//------------------------------------------------------------------------
RESKYENGINE_API TGlobalResource<FRECloudCommon> GRECloudCommon;
//------------------------------------------------------------------------
FRECloudCommon::FRECloudCommon()
{
	CloudInitialized = false;
}

//------------------------------------------------------------------------
FRECloudCommon::~FRECloudCommon()
{
}

//------------------------------------------------------------------------
void FRECloudCommon::InitDynamicRHI()
{
	FRenderResource::InitDynamicRHI();

	//// calculate 3d main noise texture
	//_CreateMainNoise();
	//// calculate 3d detail noise texture
	//_CreateDetailNoise();

	// TODO calculate curl motion noise texture
}

//------------------------------------------------------------------------
void FRECloudCommon::ReleaseDynamicRHI()
{
	FRenderResource::ReleaseDynamicRHI();

	// release 3d main noise texture
	CloudMainNoise3D.SafeRelease();
	// release 3d detail noise texture
	CloudDetailNoise3D.SafeRelease();
}

//------------------------------------------------------------------------
float FRECloudCommon::GetCloudMainValue(const FVector& InSampler)
{
	FVector realVec = FVector(
		FMath::Frac(InSampler.X)
		, FMath::Frac(InSampler.Y)
		, FMath::Frac(InSampler.Z)
		);
	int x = realVec.X * CloudMainNoiseTextureSize;
	int y = realVec.Y * CloudMainNoiseTextureSize;
	int z = realVec.Z * CloudMainNoiseTextureSize;
	int index =
		z * CloudMainNoiseTextureSize * CloudMainNoiseTextureSize
		+ y * CloudMainNoiseTextureSize
		+ x
		;
	return ((float)MainNoiseColors[index].A
		+ (float)MainNoiseColors[index].R * 2
		+ (float)MainNoiseColors[index].G * 2
		+ (float)MainNoiseColors[index].B * 2
		) / 255.0f;
}

//------------------------------------------------------------------------
float FRECloudCommon::GetCloudDetailValue(const FVector& InSampler)
{
	FVector realVec = FVector(
		FMath::Frac(InSampler.X)
		, FMath::Frac(InSampler.Y)
		, FMath::Frac(InSampler.Z)
		);
	int x = realVec.X * CloudDetailNoiseTextureSize;
	int y = realVec.Y * CloudDetailNoiseTextureSize;
	int z = realVec.Z * CloudDetailNoiseTextureSize;
	int index =
		z * CloudDetailNoiseTextureSize * CloudDetailNoiseTextureSize
		+ y * CloudDetailNoiseTextureSize
		+ x
		;
	return ((float)DetailNoiseColors[index].A
		+ (float)DetailNoiseColors[index].R * 2
		+ (float)DetailNoiseColors[index].G * 2
		+ (float)DetailNoiseColors[index].B * 2
		) / 255.0f;
}

//------------------------------------------------------------------------
void FRECloudCommon::_CreateMainNoise()
{
	// TODO noslopforever @ 2015-11-18 : from r.XXX
	const int noiseTextureSize = FRECloudCommon::CloudMainNoiseTextureSize;

	// create information
	FRHIResourceCreateInfo CreateInfo;
	FTexture3DRHIRef newTexture3D = RHICreateTexture3D(
		noiseTextureSize
		, noiseTextureSize
		, noiseTextureSize
		, FRECloudCommon::CloudMainNoiseTextureFormat
		, 1
		, TexCreate_ShaderResource
		, CreateInfo);

	// Calculate noise
	TArray<FFloat16Color> noiseColors;
	noiseColors.AddZeroed(noiseTextureSize * noiseTextureSize * noiseTextureSize);
	for (int z = 0; z < noiseTextureSize; ++z)
	{
		for (int y = 0; y < noiseTextureSize; ++y)
		{
			for (int x = 0; x < noiseTextureSize; ++x)
			{
				int index =
					z * noiseTextureSize * noiseTextureSize
					+ y * noiseTextureSize
					+ x;
				FFloat16Color& destColor = noiseColors[index];

				// call libnoise to generate noise
				double nx = x * TEMPCloudMainNoiseInitMultiplier;
				double ny = y * TEMPCloudMainNoiseInitMultiplier;
				double nz = z * TEMPCloudMainNoiseInitMultiplier;
				double init = noise::GradientCoherentNoise3D(nx, ny, nz, TEMPCloudMainNoiseSeed, noise::QUALITY_BEST);
				//init = FMath::Pow(FMath::Max(init, 0.0), 2);
				destColor.A = init;

				nx *= TEMPCloudMainNoiseLacunarity;
				ny *= TEMPCloudMainNoiseLacunarity;
				nz *= TEMPCloudMainNoiseLacunarity;
				init = noise::GradientCoherentNoise3D(nx, ny, nz, TEMPCloudMainNoiseSeed, noise::QUALITY_BEST);
				destColor.R = init;

				nx *= TEMPCloudMainNoiseLacunarity;
				ny *= TEMPCloudMainNoiseLacunarity;
				nz *= TEMPCloudMainNoiseLacunarity;
				init = noise::GradientCoherentNoise3D(nx, ny, nz, TEMPCloudMainNoiseSeed, noise::QUALITY_BEST);
				destColor.G = init;

				nx *= TEMPCloudMainNoiseLacunarity;
				ny *= TEMPCloudMainNoiseLacunarity;
				nz *= TEMPCloudMainNoiseLacunarity;
				init = noise::GradientCoherentNoise3D(nx, ny, nz, TEMPCloudMainNoiseSeed, noise::QUALITY_BEST);
				destColor.B = init;
			}
		}
	}

	// upload to texture
	const FUpdateTextureRegion3D UpdateRegion(
		0,
		0,
		0,
		0,
		0,
		0,
		noiseTextureSize,
		noiseTextureSize,
		noiseTextureSize);

	const int32 FormatSize = GPixelFormats[FRECloudCommon::CloudMainNoiseTextureFormat].BlockBytes;

	// Update the volume texture atlas
	RHIUpdateTexture3D(
		newTexture3D
		, 0
		, UpdateRegion
		, noiseTextureSize * FormatSize
		, noiseTextureSize * noiseTextureSize * FormatSize
		, (const uint8*)noiseColors.GetData()
		);

	CloudMainNoise3D = newTexture3D;
	MainNoiseColors = noiseColors;
}

//------------------------------------------------------------------------
void FRECloudCommon::_CreateDetailNoise()
{
	// TODO noslopforever @ 2015-11-18 : from r.XXX
	const int noiseTextureSize = FRECloudCommon::CloudDetailNoiseTextureSize;

	// create information
	FRHIResourceCreateInfo CreateInfo;
	FTexture3DRHIRef newTexture3D = RHICreateTexture3D(
		noiseTextureSize
		, noiseTextureSize
		, noiseTextureSize
		, FRECloudCommon::CloudDetailNoiseTextureFormat
		, 1
		, TexCreate_ShaderResource
		, CreateInfo);

	// Calculate noise
	TArray<FFloat16Color> noiseColors;
	noiseColors.AddZeroed(noiseTextureSize * noiseTextureSize * noiseTextureSize);
	for (int z = 0; z < noiseTextureSize; ++z)
	{
		for (int y = 0; y < noiseTextureSize; ++y)
		{
			for (int x = 0; x < noiseTextureSize; ++x)
			{
				int index =
					z * noiseTextureSize * noiseTextureSize
					+ y * noiseTextureSize
					+ x;
				FFloat16Color& destColor = noiseColors[index];

				// call libnoise to generate noise
				double nx = x * TEMPCloudDetailNoiseInitMultiplier;
				double ny = y * TEMPCloudDetailNoiseInitMultiplier;
				double nz = z * TEMPCloudDetailNoiseInitMultiplier;
				double init = noise::GradientCoherentNoise3D(nx, ny, nz, TEMPCloudMainNoiseSeed, noise::QUALITY_BEST);
				destColor.A = init;

				nx *= TEMPCloudDetailNoiseLacunarity;
				ny *= TEMPCloudDetailNoiseLacunarity;
				nz *= TEMPCloudDetailNoiseLacunarity;
				init = noise::GradientCoherentNoise3D(nx, ny, nz, TEMPCloudMainNoiseSeed, noise::QUALITY_BEST);
				destColor.R = init;

				nx *= TEMPCloudDetailNoiseLacunarity;
				ny *= TEMPCloudDetailNoiseLacunarity;
				nz *= TEMPCloudDetailNoiseLacunarity;
				init = noise::GradientCoherentNoise3D(nx, ny, nz, TEMPCloudMainNoiseSeed, noise::QUALITY_BEST);
				destColor.G = init;

				nx *= TEMPCloudDetailNoiseLacunarity;
				ny *= TEMPCloudDetailNoiseLacunarity;
				nz *= TEMPCloudDetailNoiseLacunarity;
				init = noise::GradientCoherentNoise3D(nx, ny, nz, TEMPCloudMainNoiseSeed, noise::QUALITY_BEST);
				destColor.B = init;
			}
		}
	}

	// upload to texture
	const FUpdateTextureRegion3D UpdateRegion(
		0,
		0,
		0,
		0,
		0,
		0,
		noiseTextureSize,
		noiseTextureSize,
		noiseTextureSize);

	const int32 FormatSize = GPixelFormats[FRECloudCommon::CloudDetailNoiseTextureFormat].BlockBytes;

	// Update the volume texture atlas
	RHIUpdateTexture3D(
		newTexture3D
		, 0
		, UpdateRegion
		, noiseTextureSize * FormatSize
		, noiseTextureSize * noiseTextureSize * FormatSize
		, (const uint8*)noiseColors.GetData()
		);

	CloudDetailNoise3D = newTexture3D;
	DetailNoiseColors = noiseColors;
}

//------------------------------------------------------------------------

