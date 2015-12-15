#include "RESkyEffectsPCH.h"
//#include "RESkyCloud.h"
#include "RESkyCloudActor.h"
//
//#include "NFCloudComponent.h"
//#include "Engine/Texture3DDynamic.h"
//#include "NFGalaxyCloudRenderEngine.h"
//#include "SceneInterface.h"



//------------------------------------------------------------------------
ATestSkyEffectActor::ATestSkyEffectActor(const FObjectInitializer& PCIP)
	: Super(PCIP)
	, TestCloudMap(nullptr)
{
}

//------------------------------------------------------------------------
#if WITH_EDITOR
void ATestSkyEffectActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// modify what , update what
	// Transform In ViewBase
	if (PropertyChangedEvent.Property
		&& PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ATestSkyEffectActor, TestCloudMap)
		)
	{
		SkyEffectProxy->UpdateCloudMap(TestCloudMap);
	}
	if (PropertyChangedEvent.Property
		&& PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(ATestSkyEffectActor, TestUpdateLighting)
		)
	{
		SkyEffectProxy->UpdateCloudLightingCache();
	}

	if (PropertyChangedEvent.MemberProperty
		&& PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ATestSkyEffectActor, AtmosphereInfo)
		)
	{
		SkyEffectProxy->UpdateAtmosphereInfo(AtmosphereInfo);
	}
	if (PropertyChangedEvent.MemberProperty
		&& PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ATestSkyEffectActor, LightingInfo)
		)
	{
		SkyEffectProxy->UpdateCloudLightingInfo(LightingInfo);
	}
	if (PropertyChangedEvent.MemberProperty
		&& PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ATestSkyEffectActor, CloudStaticInfo)
		)
	{
		SkyEffectProxy->UpdateCloudStaticInfo(CloudStaticInfo);
	}
	if (PropertyChangedEvent.MemberProperty
		&& PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ATestSkyEffectActor, CloudSpaceInfo)
		)
	{
		SkyEffectProxy->UpdateCloudSpaceInfo(CloudSpaceInfo);
	}
}
#endif //WITH_EDITOR

//------------------------------------------------------------------------
void ATestSkyEffectActor::PostInitProperties()
{
	Super::PostInitProperties();

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		UpdateSkyEffect();
	}
}

//------------------------------------------------------------------------
void ATestSkyEffectActor::PostLoad()
{
	Super::PostLoad();

	if (SkyEffectProxy != nullptr)
	{
		SkyEffectProxy->UpdateCloudMap(TestCloudMap);
		SkyEffectProxy->UpdateAtmosphereInfo(AtmosphereInfo);
		SkyEffectProxy->UpdateCloudLightingInfo(LightingInfo);
		SkyEffectProxy->UpdateCloudStaticInfo(CloudStaticInfo);
		SkyEffectProxy->UpdateCloudSpaceInfo(CloudSpaceInfo);
	}
}

//------------------------------------------------------------------------
void ATestSkyEffectActor::FinishDestroy()
{
	Super::FinishDestroy();

	if (SkyEffectProxy != nullptr) {
		GSkyEffectManager.RemoveSkyEffect(SkyEffectProxy);
		SkyEffectProxy = nullptr;
	}
}

//------------------------------------------------------------------------
void ATestSkyEffectActor::UpdateSkyEffect()
{
	if (SkyEffectProxy != nullptr) {
		GSkyEffectManager.RemoveSkyEffect(SkyEffectProxy);
		SkyEffectProxy = nullptr;
	}

	SkyEffectProxy = new FRESkyEffectProxy();
	if (SkyEffectProxy != nullptr) {
		GSkyEffectManager.AddSkyEffect(SkyEffectProxy);
	}
}


//------------------------------------------------------------------------
//
//
////------------------------------------------------------------------------
//ANFGalaxyCloudActor::ANFGalaxyCloudActor(const FObjectInitializer& PCIP)
//	: Super(PCIP)
//{
//}
//
////------------------------------------------------------------------------
//
//
//
//
//
//
//
//
////------------------------------------------------------------------------
//FNFCloudGenSceneProxy::FNFCloudGenSceneProxy(UNFCloudComponent* InComponent)
//	: FPrimitiveSceneProxy(InComponent)
//	, HostComponent(InComponent)
//{
//	check(InComponent->CloudTexture3D);
//}
//
////------------------------------------------------------------------------
//FNFCloudGenSceneProxy::~FNFCloudGenSceneProxy()
//{
//	CloudTextureUAV.SafeRelease();
//	CloudTextureRHI.SafeRelease();
//	CloudLightingOcclusionTextureRHI.SafeRelease();
//	CloudLighingOcclusionTextureUAV.SafeRelease();
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::CreateRenderThreadResources()
//{
//	SetCloudTexture_RenderThread(HostComponent->CloudTexture3D);
//	SetCloudOpticalDepthTexture_RenderThread(HostComponent->CloudOpticalDepthTexture3D);
//	//CloudTextureRHI = HostComponent->CloudTexture3D->Resource->TextureRHI->GetTexture3D();
//	//CloudTextureUAV = RHICreateUnorderedAccessView(CloudTextureRHI);
//
//	//CloudLightingOcclusionTextureRHI = HostComponent->CloudOpticalDepthTexture3D->Resource->TextureRHI->GetTexture3D();
//	//CloudLighingOcclusionTextureUAV = RHICreateUnorderedAccessView(CloudLightingOcclusionTextureRHI);
//}
//
////------------------------------------------------------------------------
//FPrimitiveViewRelevance FNFCloudGenSceneProxy::GetViewRelevance(const FSceneView* View)
//{
//	FPrimitiveViewRelevance Result;
//	Result.bDrawRelevance = IsShown(View);
//	Result.bShadowRelevance = false;
//	Result.bDynamicRelevance = true;
//	return Result;
//}
//
////------------------------------------------------------------------------
//bool FNFCloudGenSceneProxy::CanBeOccluded() const
//{
//	return false;
//}
//
////------------------------------------------------------------------------
//uint32 FNFCloudGenSceneProxy::GetMemoryFootprint() const
//{
//	// TODO impl
//	return (sizeof(*this) + GetAllocatedSize());
//}
//
////------------------------------------------------------------------------
//uint32 FNFCloudGenSceneProxy::GetAllocatedSize() const
//{
//	// TODO impl
//	return (FPrimitiveSceneProxy::GetAllocatedSize());
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
//{
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::SetCloudTexture_GameThread(UTexture3DDynamic* InTexture)
//{
//	check(InTexture);
//	check(IsInGameThread());
//	// Enqueue a message to the rendering thread containing the interaction to add.
//	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
//		SetCloudTexture,
//		FNFCloudGenSceneProxy*, PrimitiveSceneProxy, this,
//		UTexture3DDynamic*, Texture, InTexture,
//		{
//			PrimitiveSceneProxy->SetCloudTexture_RenderThread(Texture);
//		});
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::SetCloudTexture_RenderThread(UTexture3DDynamic* InTexture)
//{
//	CloudTextureRHI.SafeRelease();
//	CloudTextureUAV.SafeRelease();
//	CloudTextureRHI = InTexture->Resource->TextureRHI->GetTexture3D();
//	CloudTextureUAV = RHICreateUnorderedAccessView(CloudTextureRHI);
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::SetCloudOpticalDepthTexture_GameThread(UTexture3DDynamic* InTexture)
//{
//	check(InTexture);
//	check(IsInGameThread());
//	// Enqueue a message to the rendering thread containing the interaction to add.
//	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
//		SetCloudOpticalDepthTexture,
//		FNFCloudGenSceneProxy*, PrimitiveSceneProxy, this,
//		UTexture3DDynamic*, Texture, InTexture,
//		{
//			PrimitiveSceneProxy->SetCloudOpticalDepthTexture_RenderThread(Texture);
//		});
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::SetCloudOpticalDepthTexture_RenderThread(UTexture3DDynamic* InTexture)
//{
//	CloudLightingOcclusionTextureRHI.SafeRelease();
//	CloudLighingOcclusionTextureUAV.SafeRelease();
//	CloudLightingOcclusionTextureRHI = InTexture->Resource->TextureRHI->GetTexture3D();
//	CloudLighingOcclusionTextureUAV = RHICreateUnorderedAccessView(CloudLightingOcclusionTextureRHI);
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::SetCloudGenerateParameters_GameThread(const FCloudGenerateParameters& InParameters)
//{
//	check(IsInGameThread());
//	// Enqueue a message to the rendering thread containing the interaction to add.
//	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
//		SetCloudGenerateParameters,
//		FNFCloudGenSceneProxy*, PrimitiveSceneProxy, this,
//		FCloudGenerateParameters, Parameters, InParameters,
//		{
//			PrimitiveSceneProxy->SetCloudGenerateParameters_RenderThread(Parameters);
//		});
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::SetCloudGenerateParameters_RenderThread(const FCloudGenerateParameters& InParameters)
//{
//	check(IsInRenderingThread());
//	GenerateParameters.TEMPNoiseFunction = InParameters.TEMPNoiseFunction;
//	GenerateParameters.SamplePositionBase = InParameters.SamplePositionBase;
//	GenerateParameters.SamplePositionMultiply = InParameters.SamplePositionMultiply;
//	GenerateParameters.DensityBase = InParameters.DensityBase;
//	GenerateParameters.DensityMultiply = InParameters.DensityMultiply;
//
//	GenerateParameters.CloudTextureSize = FIntVector(CloudTextureRHI->GetSizeX(), CloudTextureRHI->GetSizeY(), CloudTextureRHI->GetSizeZ());
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::SetCloudLightingParameters_GameThread(const FCloudLightingParameters& InParameters)
//{
//	check(IsInGameThread());
//	// Enqueue a message to the rendering thread containing the interaction to add.
//	ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
//		SetCloudLightingParameters,
//		FNFCloudGenSceneProxy*, PrimitiveSceneProxy, this,
//		FCloudLightingParameters, Parameters, InParameters,
//		{
//			PrimitiveSceneProxy->SetCloudLightingParameters_RenderThread(Parameters);
//		});
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::SetCloudLightingParameters_RenderThread(const FCloudLightingParameters& InParameters)
//{
//	check(IsInRenderingThread());
//	LightingParameters.LightDirection = InParameters.LightDirection;
//	LightingParameters.StepCount = InParameters.StepCount;
//	LightingParameters.SunColor = InParameters.SunColor;
//	LightingParameters.CloudLighingOcclusionTextureSize =
//		FIntVector(CloudLightingOcclusionTextureRHI->GetSizeX()
//		, CloudLightingOcclusionTextureRHI->GetSizeY()
//		, CloudLightingOcclusionTextureRHI->GetSizeZ()
//		);
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::UpdateCloud_GameThread()
//{
//	check(IsInGameThread());
//	// Enqueue a message to the rendering thread containing the interaction to add.
//	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
//		UpdateCloud,
//		FNFCloudGenSceneProxy*, PrimitiveSceneProxy, this,
//		{
//			FRHICommandListImmediate& RHICmdList = GRHICommandList.GetImmediateCommandList();
//			PrimitiveSceneProxy->_UpdateGenerator(&PrimitiveSceneProxy->GetScene(), RHICmdList);
//		});
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::UpdateCloudLighting_GameThread()
//{
//	check(IsInGameThread());
//	// Enqueue a message to the rendering thread containing the interaction to add.
//	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
//		UpdateCloudLighting,
//		FNFCloudGenSceneProxy*, PrimitiveSceneProxy, this,
//		{
//			FRHICommandListImmediate& RHICmdList = GRHICommandList.GetImmediateCommandList();
//			PrimitiveSceneProxy->_UpdateLighting(&PrimitiveSceneProxy->GetScene(), RHICmdList);
//		});
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::_UpdateGenerator(FSceneInterface* InScene, FRHICommandList& RHICmdList)
//{
//	{
//		uint32 ClearValues[4] = { 0 };
//		RHICmdList.ClearUAV(CloudTextureUAV, ClearValues);
//	}
//
//	// Compute shader calculation
//	TShaderMapRef<FNFCloudGenerateCS> GalaxyCloudCS(GetGlobalShaderMap(InScene->GetFeatureLevel()));
//	RHICmdList.SetComputeShader(GalaxyCloudCS->GetComputeShader());
//
//	// Set inputs/outputs and dispatch compute shader
//	GalaxyCloudCS->SetOutput(RHICmdList, CloudTextureUAV);
//	GalaxyCloudCS->SetUniformBuffers(RHICmdList, GenerateParameters);
//	GalaxyCloudCS->SetParameters(RHICmdList);
//
//	FIntVector threadGroupSizeDiv = FIntVector(32, 32, 1);
//	DispatchComputeShader(RHICmdList, *GalaxyCloudCS
//		, GenerateParameters.CloudTextureSize.X / threadGroupSizeDiv.X
//		, GenerateParameters.CloudTextureSize.Y / threadGroupSizeDiv.Y
//		, GenerateParameters.CloudTextureSize.Z / threadGroupSizeDiv.Z
//		);
//	GalaxyCloudCS->UnbindBuffers(RHICmdList);
//
//}
//
////------------------------------------------------------------------------
//void FNFCloudGenSceneProxy::_UpdateLighting(FSceneInterface* InScene, FRHICommandList& RHICmdList)
//{
//	{
//		uint32 ClearValues[4] = { 0 };
//		RHICmdList.ClearUAV(CloudLighingOcclusionTextureUAV, ClearValues);
//	}
//
//	// Compute shader calculation
//	TShaderMapRef<FNFCloudLightingCS> LightingCS(GetGlobalShaderMap(InScene->GetFeatureLevel()));
//	RHICmdList.SetComputeShader(LightingCS->GetComputeShader());
//
//	// Set inputs/outputs and dispatch compute shader
//	LightingCS->SetOutput(RHICmdList, CloudLighingOcclusionTextureUAV);
//	LightingCS->SetUniformBuffers(RHICmdList, LightingParameters);
//	LightingCS->SetParameters(RHICmdList, CloudTextureRHI);
//
//	FIntVector threadGroupSizeDiv = FIntVector(32, 32, 1);
//	DispatchComputeShader(RHICmdList, *LightingCS
//		, LightingParameters.CloudLighingOcclusionTextureSize.X / threadGroupSizeDiv.X
//		, LightingParameters.CloudLighingOcclusionTextureSize.Y / threadGroupSizeDiv.Y
//		, LightingParameters.CloudLighingOcclusionTextureSize.Z / threadGroupSizeDiv.Z
//		);
//	LightingCS->UnbindBuffers(RHICmdList);
//
//}
//
//
//
//
