#pragma once

#include "RESkyRenderEngine.h"
#include "RESkyEffectCommon.h"



/** RE Sky Effects settings
*	Run in render thread only.
*/
class FRESkyEffectProxy
{
public:
	FRESkyEffectProxy();
	~FRESkyEffectProxy();

//public:
//
//	/** atmosphere
//	*/
//	FRESkyAtmosphereInfo AtmosphereInfo;
//
//	/** sky lighting info
//	*/
//	FRESkyLightingInfo LightingInfo;
//
//	/** cloud static info
//	*/
//	FRESkyCloudStaticInfo CloudStaticInfo;
//
//	/** cloud space info
//	*/
//	FRESkyCloudSpaceInfo CloudSpaceInfo;

	int32 _ManageID;

	/** Cloud to world
	*/
	FMatrix CloudToWorldMatrix;

	/** World to cloud
	*/
	FMatrix WorldToCloudMatrix;

	/** Cloud zone start
	*/
	FVector CloudZoneStart;

	/** Cloud zone size
	*/
	FVector CloudZoneSize;

	/** Cloudmap Texture
	*/
	UTexture2D* CloudMapTexture;

public:
	// Render thread datas
	TUniformBuffer<FRESkyAtmosphereUniformShaderParameters> RESkyAtmosphereUniformBuffer;
	TUniformBuffer<FRESkyLightUniformShaderParameters> RESkyLightUniformBuffer;
	TUniformBuffer<FRESkyCloudSpaceUniformShaderParameters> RESkyCloudSpaceUniformBuffer;
	TUniformBuffer<FRESkyCloudStaticUniformShaderParameters> RESkyCloudStaticUniformBuffer;
	//TUniformBuffer<FRESkyCloudViewUniformShaderParameters> RESkyCloudViewUniformShaderParameters;

	/** Cloud lighing textures
	*/
	FTexture3DRHIRef CloudLightingTextures[2];

	/** Pending UAV
	*/
	FUnorderedAccessViewRHIRef CloudLightingTextureUAV[2];


	/** Current executing thread of compute shader
	*/
	int32 CloudLightingExecutingThread;

	/** pending cloud lighting execute
	*/
	int32 CloudLightingIndexPending;
	/** using cloud lighting execute
	*/
	int32 CloudLightingIndexUsing;

	/** using cloud lighting current id
	*/
	int32 CloudLightingCurrentID;

public:

	/** [Game Thread] update lighting cache
	*	changed when space, static, lighting, and atmosphere changed
	*	or called manually
	*/
	void UpdateCloudLightingCache();

	/** [Game Thread] update when atmosphere changed
	*/
	void UpdateAtmosphereInfo(const FRESkyAtmosphereInfo& InAtmosphereInfo);

	/** [Game Thread] update when lighting changed
	*/
	void UpdateCloudLightingInfo(const FRESkyLightingInfo& InLightingInfo);

	/** [Game Thread] update when cloud static changed
	*/
	void UpdateCloudStaticInfo(const FRESkyCloudStaticInfo& InCloudStaticInfo);

	/** [Game Thread] update when cloud moved
	*/
	void UpdateCloudSpaceInfo(const FRESkyCloudSpaceInfo& InCloudSpaceInfo);

	/** [Game Thread] update when cloud map changed
	*/
	void UpdateCloudMap(UTexture2D* InTexture);

	/** update every frame
	*/
	FRESkyCloudViewUniformBufferRef GetViewParamsFromInfo_RenderThread(const FSceneView& InSceneView) const;

	/** dispatch when cloud lighting cache
	*/
	void CreateLightingCache_RenderThread();
	/** dispatch when cloud lighting cache
	*/
	void DispatchLightingCache_RenderThread(FRHICommandList& InRHICmdList, ERHIFeatureLevel::Type InFeatureType);

private:

	/** update when cloud lighting cache
	*/
	void UpdateCloudLightingCache_RenderThread();

	/** update when atmosphere changed
	*/
	void UpdateAtmosphereInfo_RenderThread(const FRESkyAtmosphereInfo& InAtmosphereInfo);

	/** update when lighting changed
	*/
	void UpdateLightingInfo_RenderThread(const FRESkyLightingInfo& InLightingInfo);

	/** update when cloud static changed
	*/
	void UpdateCloudStaticInfo_RenderThread(const FRESkyCloudStaticInfo& InCloudStaticInfo);

	/** update when cloud space changed
	*/
	void UpdateCloudSpaceInfo_RenderThread(const FRESkyCloudSpaceInfo& InCloudSpaceInfo);

	/** update when cloud map changed
	*/
	void UpdateCloudMap_RenderThread(UTexture2D* InTexture);

};


/** sky effect manager
*	manage all sky's effect
*/
class FRESkyEffectManager
{
public:

	FRESkyEffectManager();
	~FRESkyEffectManager();

private:

	/** sky proxies
	*/
	TSparseArray<FRESkyEffectProxy*> SkyEffectProxies;

public:

	/** add sky effect to this manager
	*/
	void AddSkyEffect(FRESkyEffectProxy* InSkyEffect);

	/** remove sky effect from this managerS
	*/
	void RemoveSkyEffect(FRESkyEffectProxy* InSkyEffect);

	/** Render callback:
	*/
	void RenderCB_RenderSkyEffects(class FPostOpaqueRenderParameters& InParameter);


	void AddSkyEffect_RenderThread(FRESkyEffectProxy* InSkyEffect);
	void RemoveSkyEffect_RenderThread(FRESkyEffectProxy* InSkyEffect);

};
extern FRESkyEffectManager GSkyEffectManager;