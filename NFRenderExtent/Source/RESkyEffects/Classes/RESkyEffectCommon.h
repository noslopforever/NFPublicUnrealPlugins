#pragma once

#include "Core.h"
#include "CoreUObject.h"
#include "RESkyEffectCommon.generated.h"

/** sky cloud layer configuration
*/
USTRUCT(BlueprintType)
struct FRESkyCloudLayerConfig
{
	GENERATED_USTRUCT_BODY()
public:
	FRESkyCloudLayerConfig()
	{
		ZStart0 = 150000.0f;
		ZStart1 = 200000.0f;
		ZEnd0 = 200000.0f;
		ZEnd1 = 300000.0f;
		LayerDensityFactor = 1.0f;
	}

	/** layer fade start
	*/
	UPROPERTY(EditAnywhere, Category=Default)
		float ZStart0;

	/** layer fade start 1
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float ZStart1;

	/** layer fade end 0
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float ZEnd0;

	/** layer fade end 1
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float ZEnd1;

	/** layer total density factor
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float LayerDensityFactor;
};


/** atmosphere info
*/
USTRUCT(BlueprintType)
struct FRESkyAtmosphereInfo
{
	GENERATED_USTRUCT_BODY()
public:

	FRESkyAtmosphereInfo()
	{
		PlanetCenter = FVector::ZeroVector;
		PlanetRadius = 600000;
		PlanetAtmosphereRadius = 640000;
		AmbientColor = FLinearColor(0.1f, 0.11f, 0.12f);
		SkyHazeEccentricity = 0.8f;
	}

	/** earth center
	*	in uu
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FVector PlanetCenter;

	/** earth radius
	*	in uu
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float PlanetRadius;

	/** Atmosphere radius
	*	in uu
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float PlanetAtmosphereRadius;

	/** ambient color
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FLinearColor AmbientColor;

	/** sky haze eccentricity
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float SkyHazeEccentricity;

};

/** light info
*/
USTRUCT(BlueprintType)
struct FRESkyLightingInfo
{
	GENERATED_USTRUCT_BODY()
public:

	FRESkyLightingInfo()
	{
		LightDir = FVector(1, 0, 0);
		LightColor = FLinearColor(1, 1, 1, 1);
		LightOcclusionResponse = 1.0f;
	}

	/** sky light direction (where light is direct to +X)
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FVector LightDir;

	/** color of light
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FLinearColor LightColor;

	/** light occlusion of light
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float LightOcclusionResponse;

	/** light check step
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float LightCheckStep;
};


/** cloud static info
*/
USTRUCT(BlueprintType)
struct FRESkyCloudStaticInfo
{
	GENERATED_USTRUCT_BODY()
public:

	FRESkyCloudStaticInfo()
	{
		HGFactor = 1.0f;
		BeerFactor = 1.0f;
		BeerPowderResponse = 1.0f;
		AmbientResponse = 1.0f;
		CloudEccentricity = 0.5f;
		CloudLightEccentricity = 0.5f;
	}

	/** cloud layers
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FRESkyCloudLayerConfig Layers[GCloudLayerCount];

	/** HenseyGreenstain response
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float HGFactor;

	/** Beer's Law response
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float BeerFactor;

	/** Beer's Powder (back effect) response
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float BeerPowderResponse;

	/** Cloud ambient response
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float AmbientResponse;

	/** Cloud Constant Eccentricity(?)
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float CloudEccentricity;

	/** Cloud Constant Lighting Eccentricity(?)
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float CloudLightEccentricity;
};

/** cloud space info, changed when moved
*/
USTRUCT(BlueprintType)
struct FRESkyCloudSpaceInfo
{
	GENERATED_USTRUCT_BODY()
public:
	FRESkyCloudSpaceInfo()
	{
		CloudZoneCenter = FVector(0, 0, 0);
		CloudZoneSize = FVector(4000000.0f, 4000000.0f, 4000000.0f);
		CloudZoneRotation = FRotator::ZeroRotator;
		CloudQuadOffset = FVector2D(0, 0);
		CloudQuadSize = FVector(100000.0f, 100000.0f, 100000.0f);
		DetailCloudUVMultiply = 15.7f;
	}

	/** cloud loop center
	*	in uu
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FVector CloudZoneCenter;

	/** cloud zone size
	*	in uu
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FVector CloudZoneSize;

	/** cloud zone rotation
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FRotator CloudZoneRotation;

	/** cloud loop tile offset
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FVector2D CloudQuadOffset;

	/** cloud loop tile size
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FVector CloudQuadSize;

	/** cloud detail UV size
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float DetailCloudUVMultiply;

};

/** sky cloud view info
*	Used to generate cloud map.
*/
USTRUCT(BlueprintType)
struct FRESkyCloudViewInfo
{
	GENERATED_USTRUCT_BODY()
public:
	FRESkyCloudViewInfo()
	{
		CloudFadeDistance = 1500000;
	}

	/** cloud fade dis tance
	*	in unreal unit
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		float CloudFadeDistance;


};
