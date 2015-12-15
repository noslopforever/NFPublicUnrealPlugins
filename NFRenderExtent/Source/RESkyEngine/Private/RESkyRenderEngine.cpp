#include "RESkyEnginePrivatePCH.h"
#include "RESkyRenderEngine.h"

#include "ShaderParameterUtils.h"
#include "RHIStaticStates.h"

#include "DynamicMeshBuilder.h"
#include "ShaderParameters.h"
#include "RefCounting.h"



//------------------------------------------------------------------------
// render var : Render Extent Sky Effect enabled.
TAutoConsoleVariable<int32> CVarRESkyEffectSamplerType(
	TEXT("r.RESkyEffectSamplerType"),
	0,
	TEXT("enable Render Extent Plugin's Sky Effect.\n")
	TEXT("    0: point\n")
	TEXT("    1: SF_Bilinear\n")
	TEXT("    2: SF_Trilinear\n")
	TEXT("    3: SF_AnisotropicPoint\n")
	TEXT("    4: SF_AnisotropicLinear\n")
	TEXT("    other: SF_AnisotropicLinear\n")
	, ECVF_Default);

//------------------------------------------------------------------------
IMPLEMENT_UNIFORM_BUFFER_STRUCT(FRESkyCloudLayerConfigUniformShaderParameters, TEXT("RESkyCloudLayerConfig"));
IMPLEMENT_UNIFORM_BUFFER_STRUCT(FRESkyCloudCheckConfigUniformShaderParameters, TEXT("RESkyCloudCheckConfig"));

IMPLEMENT_UNIFORM_BUFFER_STRUCT(FRESkyAtmosphereUniformShaderParameters, TEXT("RESkyAtmosphere"));
IMPLEMENT_UNIFORM_BUFFER_STRUCT(FRESkyLightUniformShaderParameters, TEXT("RESkyLight"));

IMPLEMENT_UNIFORM_BUFFER_STRUCT(FRESkyCloudStaticUniformShaderParameters, TEXT("RESkyCloudStatic"));
IMPLEMENT_UNIFORM_BUFFER_STRUCT(FRESkyCloudSpaceUniformShaderParameters, TEXT("RESkyCloudSpace"));
IMPLEMENT_UNIFORM_BUFFER_STRUCT(FRESkyCloudViewUniformShaderParameters, TEXT("RESkyCloudView"));
//------------------------------------------------------------------------
void FRESkyEffectVertexDeclaration::InitRHI()
{
	FVertexDeclarationElementList Elements;
	Elements.Add(FVertexElement(0, 0, VET_Float2, 0, sizeof(FVector2D)));
	VertexDeclarationRHI = RHICreateVertexDeclaration(Elements);
}

//------------------------------------------------------------------------
void FRESkyEffectVertexDeclaration::ReleaseRHI()
{
	VertexDeclarationRHI.SafeRelease();
}

//------------------------------------------------------------------------
IMPLEMENT_SHADER_TYPE(, FRESkyEffectVS, TEXT("RESkyEffectShaders"), TEXT("SkyEffectVSMain"), SF_Vertex);
//------------------------------------------------------------------------
FRESkyEffectVS::FRESkyEffectVS()
{
}

//------------------------------------------------------------------------
FRESkyEffectVS::~FRESkyEffectVS()
{
}

//------------------------------------------------------------------------
FRESkyEffectVS::FRESkyEffectVS(const ShaderMetaType::CompiledShaderInitializerType& InInitializer)
	: FGlobalShader(InInitializer)
{
}

//------------------------------------------------------------------------
bool FRESkyEffectVS::Serialize(FArchive& Ar)
{
	bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
	return bShaderHasOutdatedParameters;
}

//------------------------------------------------------------------------
void FRESkyEffectVS::SetParameters(FRHICommandList& RHICmdList, const FSceneView& View)
{
	FGlobalShader::SetParameters(RHICmdList, GetVertexShader(), View);
}


//------------------------------------------------------------------------
FRESkyEffectCloudShader::FRESkyEffectCloudShader()
{
}

//------------------------------------------------------------------------
FRESkyEffectCloudShader::~FRESkyEffectCloudShader()
{
}

//------------------------------------------------------------------------
FRESkyEffectCloudShader::FRESkyEffectCloudShader(const ShaderMetaType::CompiledShaderInitializerType& InInitializer)
	: FGlobalShader(InInitializer)
{
	CloudMainNoise3DTextureParameter.Bind(InInitializer.ParameterMap, TEXT("CloudMainNoise3DTexture"));
	CloudMainNoise3DTextureSamplerParameter.Bind(InInitializer.ParameterMap, TEXT("CloudMainNoise3DTextureSampler"));
	CloudDetailNoise3DTextureParameter.Bind(InInitializer.ParameterMap, TEXT("CloudDetailNoise3DTexture"));
	CloudDetailNoise3DTextureSamplerParameter.Bind(InInitializer.ParameterMap, TEXT("CloudDetailNoise3DTextureSampler"));
	CloudMap2DTextureParameter.Bind(InInitializer.ParameterMap, TEXT("CloudMap2DTexture"));
	CloudMap2DTextureSamplerParameter.Bind(InInitializer.ParameterMap, TEXT("CloudMap2DTextureSampler"));
	//SceneDepthTextureParameter.Bind(InInitializer.ParameterMap, TEXT("SceneDepthTexture"));
	//SceneDepthTextureParameterSampler.Bind(InInitializer.ParameterMap, TEXT("SceneDepthTextureSampler"));
}

//------------------------------------------------------------------------
void FRESkyEffectCloudShader::ModifyCompilationEnvironment(EShaderPlatform InPlatform, FShaderCompilerEnvironment& OutEnvironment)
{
	FGlobalShader::ModifyCompilationEnvironment(InPlatform, OutEnvironment);
}

//------------------------------------------------------------------------
bool FRESkyEffectCloudShader::Serialize(FArchive& Ar)
{
	bool bShaderHasOutdatedParams = FGlobalShader::Serialize(Ar);
	Ar << CloudMainNoise3DTextureParameter;
	Ar << CloudMainNoise3DTextureSamplerParameter;
	Ar << CloudDetailNoise3DTextureParameter;
	Ar << CloudDetailNoise3DTextureSamplerParameter;
	Ar << CloudMap2DTextureParameter;
	Ar << CloudMap2DTextureSamplerParameter;
	//Ar << SceneDepthTextureParameter;
	//Ar << SceneDepthTextureParameterSampler;

	return bShaderHasOutdatedParams;
}

//------------------------------------------------------------------------


//------------------------------------------------------------------------
IMPLEMENT_SHADER_TYPE(, FRESkyEffectPS, TEXT("RESkyEffectShaders"), TEXT("SkyEffectPSMain"), SF_Pixel);
//------------------------------------------------------------------------
FRESkyEffectPS::FRESkyEffectPS()
{
}

//------------------------------------------------------------------------
FRESkyEffectPS::~FRESkyEffectPS()
{
}

//------------------------------------------------------------------------
FRESkyEffectPS::FRESkyEffectPS(const ShaderMetaType::CompiledShaderInitializerType& InInitializer)
	: FRESkyEffectCloudShader(InInitializer)
{
	SceneTextureShaderParameters.TEMPBind(InInitializer.ParameterMap);
	CloudLight3DTextureParameter.Bind(InInitializer.ParameterMap, TEXT("CloudLight3DTexture"));
	CloudLight3DTextureSamplerParameter.Bind(InInitializer.ParameterMap, TEXT("CloudLight3DTextureSampler"));
}

//------------------------------------------------------------------------
void FRESkyEffectPS::ModifyCompilationEnvironment(EShaderPlatform InPlatform, FShaderCompilerEnvironment& OutEnvironment)
{
	FRESkyEffectCloudShader::ModifyCompilationEnvironment(InPlatform, OutEnvironment);
}

//------------------------------------------------------------------------
bool FRESkyEffectPS::Serialize(FArchive& Ar)
{
	bool bShaderHasOutdatedParams = FRESkyEffectCloudShader::Serialize(Ar);
	//Ar << CloudMainNoise3DTextureParameter;
	//Ar << CloudMainNoise3DTextureSamplerParameter;
	Ar << CloudLight3DTextureParameter;
	Ar << CloudLight3DTextureSamplerParameter;
	SceneTextureShaderParameters.TEMPSerialize(Ar);
	return bShaderHasOutdatedParams;
}

//------------------------------------------------------------------------
void FRESkyEffectPS::SetParameters(
	FRHICommandList& InRHICmdList
	, const FSceneView& InView
	, FRESkyAtmosphereUniformBufferRef InAtmosphereBufferRef
	, FRESkyLightUniformBufferRef InLightBufferRef
	, FRESkyCloudStaticUniformBufferRef InCloudStaticRef
	, FRESkyCloudSpaceUniformBufferRef InCloudSpaceRef
	, FRESkyCloudViewUniformBufferRef InCloudViewRef
	, FTexture3DRHIRef InCloudMainNoise3DTexture
	, FTexture3DRHIRef InCloudDetailNoise3DTexture
	, FTexture3DRHIRef InCloudLightTexture
	, UTexture2D* InCloudMapTexture
	, FTexture2DRHIRef InDepthTexture
	)
{
	FPixelShaderRHIParamRef PixelShaderRHI = GetPixelShader();
	FGlobalShader::SetParameters(InRHICmdList, PixelShaderRHI, InView);

	int32 skySamplerType = CVarRESkyEffectSamplerType.GetValueOnRenderThread();
	switch (skySamplerType)
	{
	case SF_Point:
		FRESkyEffectCloudShader::SetSkyEffectCloudParameters<SF_Point>(
			InRHICmdList
			, PixelShaderRHI
			, InAtmosphereBufferRef
			, InLightBufferRef
			, InCloudStaticRef
			, InCloudSpaceRef
			, InCloudMainNoise3DTexture
			, InCloudDetailNoise3DTexture
			, InCloudMapTexture
			);
		if (CloudLight3DTextureParameter.IsBound() && CloudLight3DTextureSamplerParameter.IsBound()) {
			SetTextureParameter(
				InRHICmdList,
				PixelShaderRHI,
				CloudLight3DTextureParameter,
				CloudLight3DTextureSamplerParameter,
				TStaticSamplerState<SF_Point, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
				InCloudLightTexture
				);
		}
		break;
	case SF_Bilinear:
		FRESkyEffectCloudShader::SetSkyEffectCloudParameters<SF_Bilinear>(
			InRHICmdList
			, PixelShaderRHI
			, InAtmosphereBufferRef
			, InLightBufferRef
			, InCloudStaticRef
			, InCloudSpaceRef
			, InCloudMainNoise3DTexture
			, InCloudDetailNoise3DTexture
			, InCloudMapTexture
			);
		if (CloudLight3DTextureParameter.IsBound() && CloudLight3DTextureSamplerParameter.IsBound()) {
			SetTextureParameter(
				InRHICmdList,
				PixelShaderRHI,
				CloudLight3DTextureParameter,
				CloudLight3DTextureSamplerParameter,
				TStaticSamplerState<SF_Bilinear, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
				InCloudLightTexture
				);
		}
		break;
	case SF_Trilinear:
		FRESkyEffectCloudShader::SetSkyEffectCloudParameters<SF_Trilinear>(
			InRHICmdList
			, PixelShaderRHI
			, InAtmosphereBufferRef
			, InLightBufferRef
			, InCloudStaticRef
			, InCloudSpaceRef
			, InCloudMainNoise3DTexture
			, InCloudDetailNoise3DTexture
			, InCloudMapTexture
			);
		if (CloudLight3DTextureParameter.IsBound() && CloudLight3DTextureSamplerParameter.IsBound()) {
			SetTextureParameter(
				InRHICmdList,
				PixelShaderRHI,
				CloudLight3DTextureParameter,
				CloudLight3DTextureSamplerParameter,
				TStaticSamplerState<SF_Trilinear, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
				InCloudLightTexture
				);
		}
		break;
	case SF_AnisotropicPoint:
		FRESkyEffectCloudShader::SetSkyEffectCloudParameters<SF_AnisotropicPoint>(
			InRHICmdList
			, PixelShaderRHI
			, InAtmosphereBufferRef
			, InLightBufferRef
			, InCloudStaticRef
			, InCloudSpaceRef
			, InCloudMainNoise3DTexture
			, InCloudDetailNoise3DTexture
			, InCloudMapTexture
			);
		if (CloudLight3DTextureParameter.IsBound() && CloudLight3DTextureSamplerParameter.IsBound()) {
			SetTextureParameter(
				InRHICmdList,
				PixelShaderRHI,
				CloudLight3DTextureParameter,
				CloudLight3DTextureSamplerParameter,
				TStaticSamplerState<SF_AnisotropicPoint, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
				InCloudLightTexture
				);
		}
		break;
	case SF_AnisotropicLinear:
		FRESkyEffectCloudShader::SetSkyEffectCloudParameters<SF_AnisotropicLinear>(
			InRHICmdList
			, PixelShaderRHI
			, InAtmosphereBufferRef
			, InLightBufferRef
			, InCloudStaticRef
			, InCloudSpaceRef
			, InCloudMainNoise3DTexture
			, InCloudDetailNoise3DTexture
			, InCloudMapTexture
			);
		if (CloudLight3DTextureParameter.IsBound() && CloudLight3DTextureSamplerParameter.IsBound()) {
			SetTextureParameter(
				InRHICmdList,
				PixelShaderRHI,
				CloudLight3DTextureParameter,
				CloudLight3DTextureSamplerParameter,
				TStaticSamplerState<SF_AnisotropicLinear, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
				InCloudLightTexture
				);
		}
		break;
	default:
		break;
	}

	// Set uniform buffers
	SetUniformBufferParameter(
		InRHICmdList
		, GetPixelShader()
		, GetUniformBufferParameter<FRESkyCloudViewUniformShaderParameters>()
		, InCloudViewRef
		);


	//if (SceneDepthTextureParameter.IsBound() && SceneDepthTextureParameterSampler.IsBound()) {
	//	SetTextureParameter(
	//		InRHICmdList,
	//		PixelShaderRHI,
	//		SceneDepthTextureParameter,
	//		SceneDepthTextureParameterSampler,
	//		TStaticSamplerState<SF_Point>::GetRHI(),
	//		InDepthTexture
	//		);
	//}

	SceneTextureShaderParameters.TEMPSetPixel(InRHICmdList, PixelShaderRHI, InView);
}

//------------------------------------------------------------------------
void FRESkyEffectPS::UnbindBuffers(FRHICommandList& InRHICmdList)
{
	FPixelShaderRHIParamRef PixelShaderRHI = GetPixelShader();
	FRESkyEffectCloudShader::UnbindBuffers(InRHICmdList, PixelShaderRHI);

	if (CloudLight3DTextureParameter.IsBound() && CloudLight3DTextureSamplerParameter.IsBound()) {
		SetTextureParameter(
			InRHICmdList,
			PixelShaderRHI,
			CloudLight3DTextureParameter,
			CloudLight3DTextureSamplerParameter,
			TStaticSamplerState<SF_Point, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
			FTextureRHIRef()
			);
	}
}

//------------------------------------------------------------------------

//------------------------------------------------------------------------
IMPLEMENT_SHADER_TYPE(, FRECloudLightingCS, TEXT("RESkyEffectCloudLight"), TEXT("ComputeCloudLighting"), SF_Compute);
IMPLEMENT_UNIFORM_BUFFER_STRUCT(FRECloudLightingCSUniformShaderParameters, TEXT("RECloudLightingCS"));
//------------------------------------------------------------------------
FRECloudLightingCS::~FRECloudLightingCS()
{
}

//------------------------------------------------------------------------
FRECloudLightingCS::FRECloudLightingCS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: FRESkyEffectCloudShader(Initializer)
{
	OutCloudLightingTexture3DParameter.Bind(Initializer.ParameterMap, TEXT("CloudLightingOcclusionTextureRW"));
}

//------------------------------------------------------------------------
void FRECloudLightingCS::ModifyCompilationEnvironment(EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment)
{
	FRESkyEffectCloudShader::ModifyCompilationEnvironment(Platform, OutEnvironment);
}

//------------------------------------------------------------------------
bool FRECloudLightingCS::Serialize(FArchive& Ar)
{
	bool bShaderHasOutdatedParams = FRESkyEffectCloudShader::Serialize(Ar);
	Ar << OutCloudLightingTexture3DParameter;
	return bShaderHasOutdatedParams;
}

//------------------------------------------------------------------------
void FRECloudLightingCS::SetOutput(FRHICommandList& RHICmdList, FUnorderedAccessViewRHIRef OutputCloudLightingTexture3D)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();
	if (OutCloudLightingTexture3DParameter.IsBound()) {
		RHICmdList.SetUAVParameter(ComputeShaderRHI
			, OutCloudLightingTexture3DParameter.GetBaseIndex()
			, OutputCloudLightingTexture3D);
	}
}

//------------------------------------------------------------------------
void FRECloudLightingCS::SetParameters(
	FRHICommandList& InRHICmdList
	, FRESkyAtmosphereUniformBufferRef InAtmosphereBufferRef
	, FRESkyLightUniformBufferRef InLightBufferRef
	, FRESkyCloudStaticUniformBufferRef InCloudStaticRef
	, FRESkyCloudSpaceUniformBufferRef InCloudSpaceRef
	, FRECloudLightingCSUniformBufferRef InCloudLightingCSRef
	, FTexture3DRHIRef InCloudMainNoise3DTexture
	, FTexture3DRHIRef InCloudDetailNoise3DTexture
	, UTexture2D* InCloudMapTexture
	)
{
	check(GetUniformBufferParameter<FViewUniformShaderParameters>().IsInitialized());
	CheckShaderIsValid();
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();


	int32 skySamplerType = CVarRESkyEffectSamplerType.GetValueOnRenderThread();
	switch (skySamplerType)
	{
	case SF_Point:
		FRESkyEffectCloudShader::SetSkyEffectCloudParameters<SF_Point>(
			InRHICmdList
			, ComputeShaderRHI
			, InAtmosphereBufferRef
			, InLightBufferRef
			, InCloudStaticRef
			, InCloudSpaceRef
			, InCloudMainNoise3DTexture
			, InCloudDetailNoise3DTexture
			, InCloudMapTexture
			);
		break;
	case SF_Bilinear:
		FRESkyEffectCloudShader::SetSkyEffectCloudParameters<SF_Bilinear>(
			InRHICmdList
			, ComputeShaderRHI
			, InAtmosphereBufferRef
			, InLightBufferRef
			, InCloudStaticRef
			, InCloudSpaceRef
			, InCloudMainNoise3DTexture
			, InCloudDetailNoise3DTexture
			, InCloudMapTexture
			);
		break;
	case SF_Trilinear:
		FRESkyEffectCloudShader::SetSkyEffectCloudParameters<SF_Trilinear>(
			InRHICmdList
			, ComputeShaderRHI
			, InAtmosphereBufferRef
			, InLightBufferRef
			, InCloudStaticRef
			, InCloudSpaceRef
			, InCloudMainNoise3DTexture
			, InCloudDetailNoise3DTexture
			, InCloudMapTexture
			);
		break;
	case SF_AnisotropicPoint:
		FRESkyEffectCloudShader::SetSkyEffectCloudParameters<SF_AnisotropicPoint>(
			InRHICmdList
			, ComputeShaderRHI
			, InAtmosphereBufferRef
			, InLightBufferRef
			, InCloudStaticRef
			, InCloudSpaceRef
			, InCloudMainNoise3DTexture
			, InCloudDetailNoise3DTexture
			, InCloudMapTexture
			);
		break;
	case SF_AnisotropicLinear:
		FRESkyEffectCloudShader::SetSkyEffectCloudParameters<SF_AnisotropicLinear>(
			InRHICmdList
			, ComputeShaderRHI
			, InAtmosphereBufferRef
			, InLightBufferRef
			, InCloudStaticRef
			, InCloudSpaceRef
			, InCloudMainNoise3DTexture
			, InCloudDetailNoise3DTexture
			, InCloudMapTexture
			);
		break;
	default:
		break;
	}

	// uniform buffer
	SetUniformBufferParameter(
		InRHICmdList
		, ComputeShaderRHI
		, GetUniformBufferParameter<FRECloudLightingCSUniformShaderParameters>()
		, InCloudLightingCSRef
		);
}

//------------------------------------------------------------------------
void FRECloudLightingCS::UnbindBuffers(FRHICommandList& InRHICmdList)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();
	FRESkyEffectCloudShader::UnbindBuffers(InRHICmdList, ComputeShaderRHI);

	if (OutCloudLightingTexture3DParameter.IsBound()) {
		InRHICmdList.SetUAVParameter(ComputeShaderRHI
			, OutCloudLightingTexture3DParameter.GetBaseIndex()
			, FUnorderedAccessViewRHIRef());
	}
}

//------------------------------------------------------------------------






























//------------------------------------------------------------------------
IMPLEMENT_UNIFORM_BUFFER_STRUCT(FCloudGenerateUniformShaderParameters, TEXT("CloudGenerateParameters"));
//IMPLEMENT_UNIFORM_BUFFER_STRUCT(FCloudUniformShaderParameters,			TEXT("CloudParameters"));
IMPLEMENT_UNIFORM_BUFFER_STRUCT(FCloudLightingUniformShaderParameters, TEXT("CloudLightingParameters"));
//------------------------------------------------------------------------
IMPLEMENT_SHADER_TYPE(, FNFCloudGenerateCS, TEXT("NFCloudGenerateComputeShader"), TEXT("ComputeCloudNoise"), SF_Compute);
//------------------------------------------------------------------------
FNFCloudGenerateCS::~FNFCloudGenerateCS()
{
}

//------------------------------------------------------------------------
FNFCloudGenerateCS::FNFCloudGenerateCS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: FGlobalShader(Initializer)
{
	OutputCloudTexture3D.Bind(Initializer.ParameterMap, TEXT("CloudNoiseTextureRW"));
	// only used it Material has expression that requires PerlinNoiseGradientTexture
	PerlinNoiseGradientTexture.Bind(Initializer.ParameterMap, TEXT("PerlinNoiseGradientTexture"));
	PerlinNoiseGradientTextureSampler.Bind(Initializer.ParameterMap, TEXT("PerlinNoiseGradientTextureSampler"));
	// only used it Material has expression that requires PerlinNoise3DTexture
	PerlinNoise3DTexture.Bind(Initializer.ParameterMap, TEXT("PerlinNoise3DTexture"));
	PerlinNoise3DTextureSampler.Bind(Initializer.ParameterMap, TEXT("PerlinNoise3DTextureSampler"));
}

//------------------------------------------------------------------------
void FNFCloudGenerateCS::ModifyCompilationEnvironment(EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment)
{
	FGlobalShader::ModifyCompilationEnvironment(Platform, OutEnvironment);
	//OutEnvironment.CompilerFlags.Add(CFLAG_StandardOptimization);
}

//------------------------------------------------------------------------
bool FNFCloudGenerateCS::Serialize(FArchive& Ar)
{
	bool bShaderHasOutdatedParams = FGlobalShader::Serialize(Ar);
	Ar << OutputCloudTexture3D;
	Ar << PerlinNoiseGradientTexture;
	Ar << PerlinNoiseGradientTextureSampler;
	Ar << PerlinNoise3DTexture;
	Ar << PerlinNoise3DTextureSampler;
	return bShaderHasOutdatedParams;
}

//------------------------------------------------------------------------
void FNFCloudGenerateCS::SetOutput(FRHICommandList& RHICmdList, FUnorderedAccessViewRHIRef OutCloudTexture3D)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();
	if (OutputCloudTexture3D.IsBound()){
		RHICmdList.SetUAVParameter(ComputeShaderRHI, OutputCloudTexture3D.GetBaseIndex(), OutCloudTexture3D);
	}
}

//------------------------------------------------------------------------
void FNFCloudGenerateCS::SetUniformBuffers(FRHICommandList& RHICmdList, FCloudGenerateUniformShaderParameters& GenerateParameters)
{
	FCloudGenerateUniformBufferRef CloudGenerateUniformBuffer;

	CloudGenerateUniformBuffer = FCloudGenerateUniformBufferRef::CreateUniformBufferImmediate(GenerateParameters, UniformBuffer_SingleDraw);

	// Set uniform buffers
	SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FCloudGenerateUniformShaderParameters>(), CloudGenerateUniformBuffer);
}


//------------------------------------------------------------------------
void FNFCloudGenerateCS::SetParameters(FRHICommandList& RHICmdList)
{
	// TODO impl :do not get 3D noise from system
	//FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();
	//if (PerlinNoiseGradientTexture.IsBound())
	//{
	//	FTexture2DRHIRef Texture = _GetSystemPerlinNoiseGradient();
	//	// Bind the PerlinNoiseGradientTexture as a texture
	//	SetTextureParameter(
	//		RHICmdList,
	//		ComputeShaderRHI,
	//		PerlinNoiseGradientTexture,
	//		PerlinNoiseGradientTextureSampler,
	//		TStaticSamplerState<SF_Point, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
	//		Texture
	//		);
	//}

	//if (PerlinNoise3DTexture.IsBound())
	//{
	//	FTexture3DRHIRef Texture = _GetSystemPerlinNoise3D();
	//	// Bind the PerlinNoise3DTexture as a texture
	//	SetTextureParameter(
	//		RHICmdList,
	//		ComputeShaderRHI,
	//		PerlinNoise3DTexture,
	//		PerlinNoise3DTextureSampler,
	//		TStaticSamplerState<SF_Bilinear, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
	//		Texture
	//		);
	//}
}

//------------------------------------------------------------------------
void FNFCloudGenerateCS::UnbindBuffers(FRHICommandList& RHICmdList)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();
	if (OutputCloudTexture3D.IsBound()){
		RHICmdList.SetUAVParameter(ComputeShaderRHI, OutputCloudTexture3D.GetBaseIndex(), FUnorderedAccessViewRHIRef());
	}

	//if (Cloud3DTextureSampler.IsBound())
	//	RHICmdList.SetShaderSampler();
}

//------------------------------------------------------------------------







