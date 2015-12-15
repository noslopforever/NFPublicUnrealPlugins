#include "RESkyEffectsPCH.h"

#include "RESkyRenderEngine.h"

#include "ShaderParameterUtils.h"
#include "RHIStaticStates.h"

#include "DynamicMeshBuilder.h"
#include "ShaderParameters.h"
#include "RefCounting.h"
#include "SceneUtils.h"
#include "IConsoleManager.h"


//------------------------------------------------------------------------
IMPLEMENT_MODULE(FDefaultModuleImpl, RESkyEffects)
//------------------------------------------------------------------------
// predefines
void SetSkyEffectShaders(class FPostOpaqueRenderParameters& InParameters, FRHICommandList& RHICmdList, const FSceneView& View, const FRESkyEffectProxy& InSkyEffect);

// render var : Render Extent Sky Effect enabled.
TAutoConsoleVariable<int32> CVarRESkyEffect(
	TEXT("r.RESkyEffect"),
	0,
	TEXT("enable Render Extent Plugin's Sky Effect.\n")
	TEXT("    0: off\n")
	TEXT("non 0: on\n"),
	ECVF_Default);

//FAutoConsoleVariableRef CVarRESkyEffect_CloudCheckCount(
//	TEXT("r.RESkyEffect_CloudCheckCount"),
//	GCloudCheckCount,
//	TEXT("cloud step count.\n"),
//	ECVF_Default);

//FAutoConsoleVariableRef<int32> CVarRESkyEffect_LightingCheckCount(
//	TEXT("r.RESkyEffect_LightCheckCount"),
//	GLightCheckCount,
//	TEXT("cloud step count.\n"),
//	ECVF_Default);

//------------------------------------------------------------------------
FRESkyEffectProxy::FRESkyEffectProxy()
	: CloudMapTexture(nullptr)
{
	// this function always called from game thread.

	BeginInitResource(&RESkyAtmosphereUniformBuffer);
	BeginInitResource(&RESkyLightUniformBuffer);
	BeginInitResource(&RESkyCloudSpaceUniformBuffer);
	BeginInitResource(&RESkyCloudStaticUniformBuffer);

	CloudLightingIndexPending = 1;
	CloudLightingIndexUsing = 0;
	CloudLightingExecutingThread = 0;
	CloudLightingCurrentID = -1;

	// tell Render Thread to create textures
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		CreateLightingTextures,
		FRESkyEffectProxy*, ThisProxy, this,
		{
			ThisProxy->CreateLightingCache_RenderThread();
		}
	);

}

//------------------------------------------------------------------------
FRESkyEffectProxy::~FRESkyEffectProxy()
{
	check(IsInRenderingThread());
	RESkyAtmosphereUniformBuffer.ReleaseResource();
	RESkyLightUniformBuffer.ReleaseResource();
	RESkyCloudSpaceUniformBuffer.ReleaseResource();
	RESkyCloudStaticUniformBuffer.ReleaseResource();
	CloudLightingTextureUAV[0].SafeRelease();
	CloudLightingTextureUAV[1].SafeRelease();
	CloudLightingTextures[0].SafeRelease();
	CloudLightingTextures[1].SafeRelease();
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::UpdateCloudLightingCache()
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		UpdateCloudLightingCache,
		FRESkyEffectProxy*, ThisProxy, this,
		{
			ThisProxy->UpdateCloudLightingCache_RenderThread();
		});
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::UpdateAtmosphereInfo(const FRESkyAtmosphereInfo& InAtmosphereInfo)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
		UpdateAtmosphereInfo,
		FRESkyEffectProxy*, ThisProxy, this,
		FRESkyAtmosphereInfo, InAtmosphereInfo, InAtmosphereInfo,
		{
			ThisProxy->UpdateAtmosphereInfo_RenderThread(InAtmosphereInfo);
		});
	//UpdateCloudLightingCache();
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::UpdateCloudLightingInfo(const FRESkyLightingInfo& InLightingInfo)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
		UpdateCloudLightingInfo,
		FRESkyEffectProxy*, ThisProxy, this,
		FRESkyLightingInfo, InLightingInfo, InLightingInfo,
		{
			ThisProxy->UpdateLightingInfo_RenderThread(InLightingInfo);
		});
	//UpdateCloudLightingCache();
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::UpdateCloudStaticInfo(const FRESkyCloudStaticInfo& InCloudStaticInfo)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
		UpdateCloudStaticInfo,
		FRESkyEffectProxy*, ThisProxy, this,
		FRESkyCloudStaticInfo, InCloudStaticInfo, InCloudStaticInfo,
		{
			ThisProxy->UpdateCloudStaticInfo_RenderThread(InCloudStaticInfo);
		});
	//UpdateCloudLightingCache();
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::UpdateCloudSpaceInfo(const FRESkyCloudSpaceInfo& InCloudSpaceInfo)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
		UpdateCloudSpaceInfo,
		FRESkyEffectProxy*, ThisProxy, this,
		FRESkyCloudSpaceInfo, InCloudSpaceInfo, InCloudSpaceInfo,
		{
			ThisProxy->UpdateCloudSpaceInfo_RenderThread(InCloudSpaceInfo);
		});
	//UpdateCloudLightingCache();
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::UpdateCloudMap(UTexture2D* InTexture)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
		UpdateCloudMap,
		FRESkyEffectProxy*, ThisProxy, this,
		UTexture2D*, InTexture, InTexture,
		{
			ThisProxy->UpdateCloudMap_RenderThread(InTexture);
		});
	//UpdateCloudLightingCache();
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::UpdateCloudLightingCache_RenderThread()
{
	CloudLightingCurrentID = 0;

	// clear
	FRHICommandListImmediate& RHICmdList = GRHICommandList.GetImmediateCommandList();
	uint32 ClearValues[4] = { 1 };
	RHICmdList.ClearUAV(CloudLightingTextureUAV[CloudLightingIndexPending], ClearValues);

	// TODO impl 直接给SM5这样肯定不行
	//DispatchLightingCache_RenderThread(RHICmdList, ERHIFeatureLevel::SM5);
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::UpdateAtmosphereInfo_RenderThread(const FRESkyAtmosphereInfo& InAtmosphereInfo)
{
	// set atmosphere uniform
	FRESkyAtmosphereUniformShaderParameters atmoParams;
	atmoParams.PlanetCenter = InAtmosphereInfo.PlanetCenter;
	atmoParams.PlanetRadius = InAtmosphereInfo.PlanetRadius;
	atmoParams.PlanetAtmosphereRadius = InAtmosphereInfo.PlanetAtmosphereRadius;
	atmoParams.AmbientColor = FVector(InAtmosphereInfo.AmbientColor.R, InAtmosphereInfo.AmbientColor.G, InAtmosphereInfo.AmbientColor.B);
	atmoParams.SkyHazeEccentricity = InAtmosphereInfo.SkyHazeEccentricity;
	RESkyAtmosphereUniformBuffer.SetContents(atmoParams);
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::UpdateLightingInfo_RenderThread(const FRESkyLightingInfo& InLightingInfo)
{
	FRESkyLightUniformShaderParameters lightParams;
	lightParams.LightDir = InLightingInfo.LightDir;
	FMatrix mat = FRotationMatrix::MakeFromX(InLightingInfo.LightDir);
	lightParams.LightToWorld = mat;
	lightParams.WorldToLight = mat.Inverse();
	lightParams.LightColor = FVector(InLightingInfo.LightColor.R, InLightingInfo.LightColor.G, InLightingInfo.LightColor.B);
	lightParams.LightOcclusionResponse = InLightingInfo.LightOcclusionResponse;
	lightParams.LightCheckStep = InLightingInfo.LightCheckStep;
	RESkyLightUniformBuffer.SetContents(lightParams);
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::UpdateCloudStaticInfo_RenderThread(const FRESkyCloudStaticInfo& InCloudStaticInfo)
{
	FRESkyCloudStaticUniformShaderParameters cloudStaticParams;
	cloudStaticParams.CloudEccentricity = InCloudStaticInfo.CloudEccentricity;
	cloudStaticParams.CloudLightEccentricity = InCloudStaticInfo.CloudLightEccentricity;
	cloudStaticParams.HGResponse = InCloudStaticInfo.HGFactor;
	cloudStaticParams.BeerResponse = InCloudStaticInfo.BeerFactor;
	cloudStaticParams.BeerPowderResponse = InCloudStaticInfo.BeerPowderResponse;
	cloudStaticParams.AmbientResponse = InCloudStaticInfo.AmbientResponse;
	for (int i = 0; i < GCloudLayerCount; ++i)
	{
		cloudStaticParams.Layers_ZStart0[i] = InCloudStaticInfo.Layers[i].ZStart0;
		cloudStaticParams.Layers_ZStart1[i] = InCloudStaticInfo.Layers[i].ZStart1;
		cloudStaticParams.Layers_ZEnd0[i] = InCloudStaticInfo.Layers[i].ZEnd0;
		cloudStaticParams.Layers_ZEnd1[i] = InCloudStaticInfo.Layers[i].ZEnd1;
		cloudStaticParams.Layers_LayerDensityFactor[i] = InCloudStaticInfo.Layers[i].LayerDensityFactor;
	}

	RESkyCloudStaticUniformBuffer.SetContents(cloudStaticParams);
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::UpdateCloudSpaceInfo_RenderThread(const FRESkyCloudSpaceInfo& InCloudSpaceInfo)
{
	FRotationTranslationMatrix cloudToWorld(
		InCloudSpaceInfo.CloudZoneRotation
		, InCloudSpaceInfo.CloudZoneCenter - (InCloudSpaceInfo.CloudZoneSize * 0.5f)
		);
	CloudToWorldMatrix = cloudToWorld;
	WorldToCloudMatrix = cloudToWorld.Inverse();
	CloudZoneStart = InCloudSpaceInfo.CloudZoneCenter - (InCloudSpaceInfo.CloudZoneSize * 0.5f);
	CloudZoneSize = InCloudSpaceInfo.CloudZoneSize;

	FRESkyCloudSpaceUniformShaderParameters params;
	params.CloudToWorldMatrix = CloudToWorldMatrix;
	params.WorldToCloudMatrix = WorldToCloudMatrix;
	params.CloudZoneStart = CloudZoneStart;
	params.CloudZoneSize = CloudZoneSize;
	params.CloudQuadOffset = InCloudSpaceInfo.CloudQuadOffset;
	params.CloudQuadSize = InCloudSpaceInfo.CloudQuadSize;
	params.DetailCloudUVMultiply = InCloudSpaceInfo.DetailCloudUVMultiply;
	RESkyCloudSpaceUniformBuffer.SetContents(params);
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::UpdateCloudMap_RenderThread(UTexture2D* InTexture)
{
	CloudMapTexture = InTexture;
}

// TODO impl 计算Slice切片位置
//------------------------------------------------------------------------
FRESkyCloudViewUniformBufferRef FRESkyEffectProxy::GetViewParamsFromInfo_RenderThread(const FSceneView& InSceneView) const
{
	// 
	FVector viewDir = InSceneView.InvViewMatrix.TransformVector(FVector(0, 0, 1));
	FVector viewPtInWorld = InSceneView.ViewMatrices.ViewOrigin;
	FVector viewPtInCloud = WorldToCloudMatrix.TransformPosition(viewPtInWorld);

	float zHint = 0;
	// 云之下
	if (viewPtInCloud.Z < 0) {
		zHint = 0.0f - viewPtInCloud.Z;
	}
	else {
		// 云之上
		if (viewPtInCloud.Z > CloudZoneSize.Z) {
			zHint = viewPtInCloud.Z - CloudZoneSize.Z;
		}
		// 云中
		else{
			zHint = 0;
		}
	}

	FBox cloudBox(CloudZoneStart, CloudZoneStart + CloudZoneSize);
	// inside
	const bool insideCloud = cloudBox.IsInsideOrOn(viewPtInCloud);

	// calculate parameter from view
	FRESkyCloudViewUniformShaderParameters viewParameter;
	viewParameter.CloudZoneFadeDistance = 1500000.0f;
	viewParameter.CheckCount = GCloudCheckCount;
	viewParameter.ZHint = zHint;
	// check view to cloud zone
	for (int32 i = 0; i < GCloudCheckCount; ++i)
	{
		viewParameter.CloudCheck_Distance[i] = CloudZoneSize.Z / (float)GCloudCheckCount * i;
		viewParameter.CloudCheck_FadeValue[i] = 1.0f;
	}

	FRESkyCloudViewUniformBufferRef viewUniformBuffer =
		FRESkyCloudViewUniformBufferRef::CreateUniformBufferImmediate(
		viewParameter
		, UniformBuffer_SingleDraw
		);
	return viewUniformBuffer;
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::CreateLightingCache_RenderThread()
{
	// TODO noslopforever @ 2015-11-18 : from r.XXX
	// TODO noslopforever @ 2015-11-18 : format
	const int noiseTextureSize = FRECloudCommon::CloudMainNoiseTextureSize;
	// create information
	FRHIResourceCreateInfo CreateInfo1;
	CloudLightingTextures[0] = RHICreateTexture3D(
		noiseTextureSize
		, noiseTextureSize
		, noiseTextureSize
		, PF_FloatRGBA
		, 1
		, TexCreate_ShaderResource | TexCreate_UAV
		, CreateInfo1);
	FRHIResourceCreateInfo CreateInfo2;
	CloudLightingTextures[1] = RHICreateTexture3D(
		noiseTextureSize
		, noiseTextureSize
		, noiseTextureSize
		, PF_FloatRGBA
		, 1
		, TexCreate_ShaderResource | TexCreate_UAV
		, CreateInfo2);

	// UAVs
	CloudLightingTextureUAV[0] = RHICreateUnorderedAccessView(CloudLightingTextures[0]);
	CloudLightingTextureUAV[1] = RHICreateUnorderedAccessView(CloudLightingTextures[1]);
}

//------------------------------------------------------------------------
void FRESkyEffectProxy::DispatchLightingCache_RenderThread(FRHICommandList& InRHICmdList, ERHIFeatureLevel::Type InFeatureType)
{
	// no pending update
	if (CloudLightingCurrentID == -1) {
		return;
	}

	// TODO noslopforever @ 2015-11-18 : from r.XXX
	// TODO noslopforever @ 2015-11-18 : format
	const int noiseTextureSize = FRECloudCommon::CloudMainNoiseTextureSize;
	// Compute shader calculation
	TShaderMapRef<FRECloudLightingCS> LightingCS(GetGlobalShaderMap(InFeatureType));

	InRHICmdList.SetComputeShader(LightingCS->GetComputeShader());

	// set parameters
	FRECloudLightingCSUniformShaderParameters params;
	params.StartXYZ = FVector(0, 0, 0);
	FRECloudLightingCSUniformBufferRef cloudLightingBuffer =
		FRECloudLightingCSUniformBufferRef::CreateUniformBufferImmediate(params, UniformBuffer_SingleDraw);

	LightingCS->SetOutput(InRHICmdList, CloudLightingTextureUAV[CloudLightingIndexPending]);
	// Set inputs/outputs and dispatch compute shader
	LightingCS->SetParameters(InRHICmdList
		, RESkyAtmosphereUniformBuffer
		, RESkyLightUniformBuffer
		, RESkyCloudStaticUniformBuffer
		, RESkyCloudSpaceUniformBuffer
		, cloudLightingBuffer
		, GRECloudCommon.GetCloudMain()
		, GRECloudCommon.GetCloudDetail()
		, CloudMapTexture
		);

	FIntVector threadGroupSizeDiv = FIntVector(16, 16, 1);
	// TODO separate to different pass
	const int CloudLightingPassCount = 1;
	FIntVector vec = FIntVector(noiseTextureSize / threadGroupSizeDiv.X, noiseTextureSize / threadGroupSizeDiv.Y, noiseTextureSize / threadGroupSizeDiv.Z);
	vec /= CloudLightingPassCount;

	// dispatch
	DispatchComputeShader(InRHICmdList, *LightingCS, vec.X, vec.Y, vec.Z);
	LightingCS->UnbindBuffers(InRHICmdList);

	// cloud lighting current ID
	if (++CloudLightingCurrentID >= CloudLightingPassCount) {
		// swap pending and using
		if (CloudLightingIndexPending == 0) {
			CloudLightingIndexPending = 1;
			CloudLightingIndexUsing = 0;
		}
		else{
			CloudLightingIndexPending = 0;
			CloudLightingIndexUsing = 1;
		}
		CloudLightingCurrentID = -1;
	}
}

//------------------------------------------------------------------------

//class FPostOpaqueRenderParameters
//{
//public:
//	FIntRect ViewportRect;
//	FMatrix ViewMatrix;
//	FMatrix ProjMatrix;
//	FRHITexture2D* DepthTexture;
//	FRHITexture2D* SmallDepthTexture;
//	FRHICommandListImmediate* RHICmdList;
//	void* Uid; // A unique identifier for the view.
//};





//------------------------------------------------------------------------
FRESkyEffectManager GSkyEffectManager;
//------------------------------------------------------------------------
FRESkyEffectManager::FRESkyEffectManager()
{
}

//------------------------------------------------------------------------
FRESkyEffectManager::~FRESkyEffectManager()
{
	check(SkyEffectProxies.Num() == 0);
}

//------------------------------------------------------------------------
void FRESkyEffectManager::AddSkyEffect(FRESkyEffectProxy* InSkyEffect)
{
	static bool GInitialized = false;
	if (!GInitialized)
	{
		GetRendererModule().RegisterPostOpaqueRenderDelegate(
			FPostOpaqueRenderDelegate::CreateRaw(this, &FRESkyEffectManager::RenderCB_RenderSkyEffects)
			);
	}

	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
		AddSkyEffect,
		FRESkyEffectManager*, This, this,
		FRESkyEffectProxy*, InSkyEffect, InSkyEffect,
		{
			This->AddSkyEffect_RenderThread(InSkyEffect);
		});

}

//------------------------------------------------------------------------
void FRESkyEffectManager::RemoveSkyEffect(FRESkyEffectProxy* InSkyEffect)
{
	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
		RemoveSkyEffect,
		FRESkyEffectManager*, This, this,
		FRESkyEffectProxy*, InSkyEffect, InSkyEffect,
		{
			This->RemoveSkyEffect_RenderThread(InSkyEffect);
		});
}

//------------------------------------------------------------------------
void FRESkyEffectManager::AddSkyEffect_RenderThread(FRESkyEffectProxy* InSkyEffect)
{
	InSkyEffect->_ManageID = SkyEffectProxies.Add(InSkyEffect);
}

//------------------------------------------------------------------------
void FRESkyEffectManager::RemoveSkyEffect_RenderThread(FRESkyEffectProxy* InSkyEffect)
{
	SkyEffectProxies.RemoveAt(InSkyEffect->_ManageID);

	delete InSkyEffect;
}

//------------------------------------------------------------------------
void FRESkyEffectManager::RenderCB_RenderSkyEffects(class FPostOpaqueRenderParameters& InParameter)
{
	extern TAutoConsoleVariable<int32> CVarRESkyEffect;
	int32 RESkyEffect = CVarRESkyEffect.GetValueOnRenderThread();
	if (RESkyEffect > 0)
	{
		//GRenderTargetPool.AddPhaseEvent(FString::Format(TEXT("RESkyEffects_%d"), reinterpret_cast<int>(InParameters.Uid)));

		FRHICommandListImmediate& RHICmdList = *InParameter.RHICmdList;
		const FSceneView& View = *static_cast<const FSceneView*>(InParameter.Uid);
		const FSceneInterface* Scene = View.Family->Scene;

		if (View.GetFeatureLevel() >= ERHIFeatureLevel::SM5)
		{
			SCOPED_DRAW_EVENT(RHICmdList, SkyEffectsCompute);
			for (TSparseArray<FRESkyEffectProxy*>::TConstIterator It(SkyEffectProxies); It; ++It)
			{
				FRESkyEffectProxy* proxy = *It;
				proxy->DispatchLightingCache_RenderThread(RHICmdList, View.GetFeatureLevel());
			}
		}

		// TODO impl is enabled this feature ?
		if (View.GetFeatureLevel() >= ERHIFeatureLevel::SM5)
		{
			SCOPED_DRAW_EVENT(RHICmdList, SkyEffects);

			// TODO calculate screen quad
			static const FVector2D Vertices[4] =
			{
				FVector2D(-1, -1),
				FVector2D(-1, +1),
				FVector2D(+1, +1),
				FVector2D(+1, -1),
			};
			static const uint16 Indices[6] =
			{
				0, 1, 2,
				0, 2, 3
			};

			RHICmdList.SetRasterizerState(TStaticRasterizerState<FM_Solid, CM_None>::GetRHI());
			// disable alpha writes in order to preserve scene depth values on PC
			//RHICmdList.SetBlendState(TStaticBlendState<CW_RGB, BO_Add, BF_One, BF_SourceAlpha>::GetRHI());
			RHICmdList.SetBlendState(TStaticBlendState<CW_RGB, BO_Add, BF_InverseSourceAlpha, BF_SourceAlpha>::GetRHI());
			RHICmdList.SetDepthStencilState(TStaticDepthStencilState<false, CF_Always>::GetRHI());

			// RHI cmd list
			for (TSparseArray<FRESkyEffectProxy*>::TConstIterator It(SkyEffectProxies); It; ++It)
			{
				FRESkyEffectProxy* proxy = *It;
				SetSkyEffectShaders(InParameter, RHICmdList, View, *proxy);

				// Draw a quad covering the view.
				DrawIndexedPrimitiveUP(
					RHICmdList,
					PT_TriangleList,
					0,
					ARRAY_COUNT(Vertices),
					2,
					Indices,
					sizeof(Indices[0]),
					Vertices,
					sizeof(Vertices[0])
					);
			}
		}
	}
}

//------------------------------------------------------------------------





//------------------------------------------------------------------------
/** Vertex declaration for the light function fullscreen 2D quad. */
TGlobalResource<FRESkyEffectVertexDeclaration> GRESkyEffectVertexDeclaration;
//------------------------------------------------------------------------
void SetSkyEffectShaders(
class FPostOpaqueRenderParameters& InParameters
	, FRHICommandList& RHICmdList
	, const FSceneView& View
	, const FRESkyEffectProxy& InSkyEffect
	)
{
	auto ShaderMap = GetGlobalShaderMap(View.GetFeatureLevel());

	TShaderMapRef<FRESkyEffectVS> VertexShader(ShaderMap);
	TShaderMapRef<FRESkyEffectPS> PixelShader(ShaderMap);

	// global bound shader state
	static FGlobalBoundShaderState BoundShaderState;
	SetGlobalBoundShaderState(RHICmdList
		, View.GetFeatureLevel()
		, BoundShaderState
		, GRESkyEffectVertexDeclaration.VertexDeclarationRHI
		, *VertexShader
		, *PixelShader
		);

	// shader texture parameters
	VertexShader->SetParameters(RHICmdList, View);

	// create view parameters
	FRESkyCloudViewUniformBufferRef viewUniformBuffer = InSkyEffect.GetViewParamsFromInfo_RenderThread(View);
	// set PS parameters
	PixelShader->SetParameters(RHICmdList
		, View
		, InSkyEffect.RESkyAtmosphereUniformBuffer
		, InSkyEffect.RESkyLightUniformBuffer
		, InSkyEffect.RESkyCloudStaticUniformBuffer
		, InSkyEffect.RESkyCloudSpaceUniformBuffer
		, viewUniformBuffer
		, GRECloudCommon.GetCloudMain()
		, GRECloudCommon.GetCloudDetail()
		, InSkyEffect.CloudLightingTextures[InSkyEffect.CloudLightingIndexUsing]
		, InSkyEffect.CloudMapTexture
		, InParameters.DepthTexture
		);

}

