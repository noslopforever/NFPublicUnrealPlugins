#pragma once

#include "GlobalShader.h"
#include "UniformBuffer.h"
#include "RHI.h"
#include "RHICommandList.h"
#include "RHIStaticStates.h"
#include "SceneRenderTargetParameters.h"


///** Galaxy cloud density calculation Compute Shader
//*/
//struct NFGALAXYRENDERENGINE_API FNFGalaxyCloudDensityCS
//	: public FGlobalShader
//{
//	DECLARE_SHADER_TYPE(FNFGalaxyCloudDensityCS, Global);
//public:
//
//
//};


static const int32 GCloudCheckCount = 256;
static const int32 GCloudLayerCount = 8;

/** Cloud layer config
*/
BEGIN_UNIFORM_BUFFER_STRUCT(FRESkyCloudLayerConfigUniformShaderParameters, RESKYENGINE_API)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, StartPos0)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, StartPos1)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, EndPos0)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, EndPos1)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, LayerDensityFactor)
END_UNIFORM_BUFFER_STRUCT(FRESkyCloudLayerConfigUniformShaderParameters)
typedef TUniformBufferRef<FRESkyCloudLayerConfigUniformShaderParameters> FRESkyCloudLayerConfigUniformBufferRef;

/** Cloud check config
*/
BEGIN_UNIFORM_BUFFER_STRUCT(FRESkyCloudCheckConfigUniformShaderParameters, RESKYENGINE_API)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, Distance)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, FadeValue)
END_UNIFORM_BUFFER_STRUCT(FRESkyCloudCheckConfigUniformShaderParameters)
typedef TUniformBufferRef<FRESkyCloudCheckConfigUniformShaderParameters> FRESkyCloudCheckConfigUniformBufferRef;



/** Atmosphere
*/
BEGIN_UNIFORM_BUFFER_STRUCT(FRESkyAtmosphereUniformShaderParameters, RESKYENGINE_API)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, PlanetCenter)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float,	PlanetRadius)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, PlanetAtmosphereRadius)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, AmbientColor)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, SkyHazeEccentricity)
END_UNIFORM_BUFFER_STRUCT(FRESkyAtmosphereUniformShaderParameters)
typedef TUniformBufferRef<FRESkyAtmosphereUniformShaderParameters> FRESkyAtmosphereUniformBufferRef;

/** Sky lighting
*/
BEGIN_UNIFORM_BUFFER_STRUCT(FRESkyLightUniformShaderParameters, RESKYENGINE_API)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FMatrix, LightToWorld)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FMatrix, WorldToLight)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, LightDir)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, LightColor)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, LightOcclusionResponse)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, LightCheckStep)
END_UNIFORM_BUFFER_STRUCT(FRESkyLightUniformShaderParameters)
typedef TUniformBufferRef<FRESkyLightUniformShaderParameters> FRESkyLightUniformBufferRef;

/** Cloud static parameters
*/
BEGIN_UNIFORM_BUFFER_STRUCT(FRESkyCloudStaticUniformShaderParameters, RESKYENGINE_API)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, CloudEccentricity)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, CloudLightEccentricity)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, HGResponse)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, BeerResponse)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, BeerPowderResponse)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, AmbientResponse)
//DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER_ARRAY(FRESkyCloudLayerConfigUniformShaderParameters, Layers, [GCloudLayerCount])
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER_ARRAY(float, Layers_ZStart0, [GCloudLayerCount])
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER_ARRAY(float, Layers_ZStart1, [GCloudLayerCount])
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER_ARRAY(float, Layers_ZEnd0, [GCloudLayerCount])
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER_ARRAY(float, Layers_ZEnd1, [GCloudLayerCount])
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER_ARRAY(float, Layers_LayerDensityFactor, [GCloudLayerCount])
END_UNIFORM_BUFFER_STRUCT(FRESkyCloudStaticUniformShaderParameters)
typedef TUniformBufferRef<FRESkyCloudStaticUniformShaderParameters> FRESkyCloudStaticUniformBufferRef;

/** Cloud space
*/
BEGIN_UNIFORM_BUFFER_STRUCT(FRESkyCloudSpaceUniformShaderParameters, RESKYENGINE_API)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FMatrix, CloudToWorldMatrix)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FMatrix, WorldToCloudMatrix)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, CloudZoneStart)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, CloudZoneSize)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector2D, CloudQuadOffset)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, CloudQuadSize)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, DetailCloudUVMultiply)
END_UNIFORM_BUFFER_STRUCT(FRESkyCloudSpaceUniformShaderParameters)
typedef TUniformBufferRef<FRESkyCloudSpaceUniformShaderParameters> FRESkyCloudSpaceUniformBufferRef;

/** Cloud view
*/
BEGIN_UNIFORM_BUFFER_STRUCT(FRESkyCloudViewUniformShaderParameters, RESKYENGINE_API)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, CloudZoneFadeDistance)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(int32, CheckCount)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, ZHint)
//DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER_ARRAY(FVector4, Checks, [GCloudCheckCount])
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER_ARRAY(float, CloudCheck_Distance, [GCloudCheckCount])
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER_ARRAY(float, CloudCheck_FadeValue, [GCloudCheckCount])
END_UNIFORM_BUFFER_STRUCT(FRESkyCloudViewUniformShaderParameters)
typedef TUniformBufferRef<FRESkyCloudViewUniformShaderParameters> FRESkyCloudViewUniformBufferRef;


/** The fog vertex declaration resource type. */
class RESKYENGINE_API FRESkyEffectVertexDeclaration : public FRenderResource
{
public:
	FVertexDeclarationRHIRef VertexDeclarationRHI;

	// Destructor
	virtual ~FRESkyEffectVertexDeclaration() {}
	/** \copydoc FRenderResource::InitRHI */
	virtual void InitRHI() override;
	/** \copydoc FRenderResource::ReleaseRHI */
	virtual void ReleaseRHI() override;
};


/** Sky effect vertex shader
*	similiar as FPostProcessVS
*/
class RESKYENGINE_API FRESkyEffectVS
	: public FGlobalShader
{
	DECLARE_SHADER_TYPE(FRESkyEffectVS, Global);
public:
	/** Default constructor */
	FRESkyEffectVS();
	virtual	~FRESkyEffectVS();

	/** Initialization constructor */
	explicit FRESkyEffectVS(const ShaderMetaType::CompiledShaderInitializerType& InInitializer);

	/** Cache only when using SM5 */
	static bool ShouldCache(EShaderPlatform InPlatform) {
		return IsFeatureLevelSupported(InPlatform, ERHIFeatureLevel::SM5);
	}

	/** Serialization
	*/
	virtual bool Serialize(FArchive& Ar) override;

	/** Set Parameters */
	void SetParameters(FRHICommandList& RHICmdList, const FSceneView& View);

};


class RESKYENGINE_API FRESkyEffectCloudShader
	: public FGlobalShader
{
protected:
	/** Default constructor */
	FRESkyEffectCloudShader();
	virtual	~FRESkyEffectCloudShader();

	/** Initialization constructor */
	explicit FRESkyEffectCloudShader(const ShaderMetaType::CompiledShaderInitializerType& InInitializer);

public:
	/** Cache only when using SM5 */
	static bool ShouldCache(EShaderPlatform InPlatform) {
		return IsFeatureLevelSupported(InPlatform, ERHIFeatureLevel::SM5);
	}

	/** Modify compilation environment
	*/
	static void ModifyCompilationEnvironment(
		EShaderPlatform InPlatform
		, FShaderCompilerEnvironment& OutEnvironment
		);

protected:

	/** Serialization
	*/
	virtual bool Serialize(FArchive& Ar) override;

	template<ESamplerFilter SamplerFilter, typename ShaderRHIParamRef>
	void SetSkyEffectCloudParameters(
		FRHICommandList& InRHICmdList
		, const ShaderRHIParamRef InShaderRHI
		, FRESkyAtmosphereUniformBufferRef InAtmosphereBufferRef
		, FRESkyLightUniformBufferRef InLightBufferRef
		, FRESkyCloudStaticUniformBufferRef InCloudStaticRef
		, FRESkyCloudSpaceUniformBufferRef InCloudSpaceRef
		, FTexture3DRHIRef InCloudMainNoise3DTexture
		, FTexture3DRHIRef InCloudDetailNoise3DTexture
		, UTexture2D* InCloudMapTexture
		)
	{
		// Set uniform buffers
		SetUniformBufferParameter(
			InRHICmdList
			, InShaderRHI
			, GetUniformBufferParameter<FRESkyAtmosphereUniformShaderParameters>()
			, InAtmosphereBufferRef
			);
		SetUniformBufferParameter(
			InRHICmdList
			, InShaderRHI
			, GetUniformBufferParameter<FRESkyLightUniformShaderParameters>()
			, InLightBufferRef
			);
		SetUniformBufferParameter(
			InRHICmdList
			, InShaderRHI
			, GetUniformBufferParameter<FRESkyCloudStaticUniformShaderParameters>()
			, InCloudStaticRef
			);
		SetUniformBufferParameter(
			InRHICmdList
			, InShaderRHI
			, GetUniformBufferParameter<FRESkyCloudSpaceUniformShaderParameters>()
			, InCloudSpaceRef
			);
		if (CloudMainNoise3DTextureParameter.IsBound() && CloudMainNoise3DTextureSamplerParameter.IsBound()) {
			SetTextureParameter(
				InRHICmdList,
				InShaderRHI,
				CloudMainNoise3DTextureParameter,
				CloudMainNoise3DTextureSamplerParameter,
				TStaticSamplerState<SamplerFilter, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
				InCloudMainNoise3DTexture
				);
		}
		if (CloudDetailNoise3DTextureParameter.IsBound() && CloudDetailNoise3DTextureSamplerParameter.IsBound()) {
			SetTextureParameter(
				InRHICmdList,
				InShaderRHI,
				CloudDetailNoise3DTextureParameter,
				CloudDetailNoise3DTextureSamplerParameter,
				TStaticSamplerState<SamplerFilter, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
				InCloudDetailNoise3DTexture
				);
		}

		if (CloudMap2DTextureParameter.IsBound() && CloudMap2DTextureSamplerParameter.IsBound()) {
			if (InCloudMapTexture != nullptr)
			{
				SetTextureParameter(
					InRHICmdList,
					InShaderRHI,
					CloudMap2DTextureParameter,
					CloudMap2DTextureSamplerParameter,
					//TStaticSamplerState<SF_Bilinear>::GetRHI(),
					InCloudMapTexture->Resource
					);
			}
		}
	}

	/** Unbinds bound buffers
	*/
	template<typename ShaderRHIParamRef>
	void UnbindBuffers(FRHICommandList& InRHICmdList
		, const ShaderRHIParamRef ShaderRHI
		)
	{
		if (CloudMainNoise3DTextureParameter.IsBound() && CloudMainNoise3DTextureSamplerParameter.IsBound()) {
			SetTextureParameter(
				InRHICmdList,
				ShaderRHI,
				CloudMainNoise3DTextureParameter,
				CloudMainNoise3DTextureSamplerParameter,
				TStaticSamplerState<SF_Point, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
				FTextureRHIRef()
				);
		}
		if (CloudDetailNoise3DTextureParameter.IsBound() && CloudDetailNoise3DTextureSamplerParameter.IsBound()) {
			SetTextureParameter(
				InRHICmdList,
				ShaderRHI,
				CloudDetailNoise3DTextureParameter,
				CloudDetailNoise3DTextureSamplerParameter,
				TStaticSamplerState<SF_Bilinear, AM_Wrap, AM_Wrap, AM_Wrap>::GetRHI(),
				FTextureRHIRef()
				);
		}
		if (CloudMap2DTextureParameter.IsBound() && CloudMap2DTextureSamplerParameter.IsBound()) {
			SetTextureParameter(
				InRHICmdList,
				ShaderRHI,
				CloudMap2DTextureParameter,
				CloudMap2DTextureSamplerParameter,
				TStaticSamplerState<SF_Bilinear>::GetRHI(),
				FTextureRHIRef()
				);
		}
	}

private:

	/** The CloudMainNoise3DTexture parameter
	*/
	FShaderResourceParameter CloudMainNoise3DTextureParameter;
	FShaderResourceParameter CloudMainNoise3DTextureSamplerParameter;

	/** The CloudMainNoise3DTexture parameter
	*/
	FShaderResourceParameter CloudDetailNoise3DTextureParameter;
	FShaderResourceParameter CloudDetailNoise3DTextureSamplerParameter;

	/** The Cloud map 2D parameter
	*/
	FShaderResourceParameter CloudMap2DTextureParameter;
	FShaderResourceParameter CloudMap2DTextureSamplerParameter;

};


/** Sky effect pixel shader
*/
class RESKYENGINE_API FRESkyEffectPS
	: public FRESkyEffectCloudShader
{
	DECLARE_SHADER_TYPE(FRESkyEffectPS, Global);
public:
	/** Default constructor */
	FRESkyEffectPS();
	virtual	~FRESkyEffectPS();

	/** Initialization constructor */
	explicit FRESkyEffectPS(const ShaderMetaType::CompiledShaderInitializerType& InInitializer);

	/** Cache only when using SM5 */
	static bool ShouldCache(EShaderPlatform InPlatform) {
		return IsFeatureLevelSupported(InPlatform, ERHIFeatureLevel::SM5);
	}

	/** Modify compilation environment
	*/
	static void ModifyCompilationEnvironment(
		EShaderPlatform InPlatform
		, FShaderCompilerEnvironment& OutEnvironment
		);

	/** Serialization
	*/
	virtual bool Serialize(FArchive& Ar) override;

	/** Set parameters
	*/
	void SetParameters(
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
		);

	/** Unbinds bound buffers
	*/
	void UnbindBuffers(FRHICommandList& InRHICmdList);

private:

	///** The SceneDepthTexture parameter for materials that use SceneDepth */
	//FShaderResourceParameter SceneDepthTextureParameter;
	//FShaderResourceParameter SceneDepthTextureParameterSampler;

	/** The Cloud Lighting 3DTexture parameter
	*/
	FShaderResourceParameter CloudLight3DTextureParameter;
	FShaderResourceParameter CloudLight3DTextureSamplerParameter;

	FSceneTextureShaderParameters SceneTextureShaderParameters;
};



/** Cloud check config
*/
BEGIN_UNIFORM_BUFFER_STRUCT(FRECloudLightingCSUniformShaderParameters, RESKYENGINE_API)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, StartXYZ)
END_UNIFORM_BUFFER_STRUCT(FRECloudLightingCSUniformShaderParameters)
typedef TUniformBufferRef<FRECloudLightingCSUniformShaderParameters> FRECloudLightingCSUniformBufferRef;



/** Galaxy cloud Compute Shader
*/
struct RESKYENGINE_API FRECloudLightingCS
	: public FRESkyEffectCloudShader
{
	DECLARE_SHADER_TYPE(FRECloudLightingCS, Global);
public:
	/** Default constructor */
	FRECloudLightingCS() { }
	virtual	~FRECloudLightingCS();

	/** Initialization constructor */
	explicit FRECloudLightingCS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	/** Cache only when using SM5 */
	static bool ShouldCache(EShaderPlatform Platform) {
		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
	}

	/** Modify compilation environment
	*/
	static void ModifyCompilationEnvironment(EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment);

	/** Serialization
	*/
	virtual bool Serialize(FArchive& Ar) override;

	/** Set output buffers */
	void SetOutput(FRHICommandList& RHICmdList, FUnorderedAccessViewRHIRef OutCloudLightingTexture3D);

	/** Set parameters
	*/
	void SetParameters(
		FRHICommandList& InRHICmdList
		, FRESkyAtmosphereUniformBufferRef InAtmosphereBufferRef
		, FRESkyLightUniformBufferRef InLightBufferRef
		, FRESkyCloudStaticUniformBufferRef InCloudStaticRef
		, FRESkyCloudSpaceUniformBufferRef InCloudSpaceRef
		, FRECloudLightingCSUniformBufferRef InCloudLightingCSRef
		, FTexture3DRHIRef InCloudMainNoise3DTexture
		, FTexture3DRHIRef InCloudDetailNoise3DTexture
		, UTexture2D* InCloudMapTexture
		);

	/** Unbinds bound buffers */
	void UnbindBuffers(FRHICommandList& RHICmdList);

private:

	/* Output buffer
	*/
	FShaderResourceParameter OutCloudLightingTexture3DParameter;

};



/** Cloud Commons
*/
struct RESKYENGINE_API FRECloudCommon
	: public FRenderResource
{
public:
	FRECloudCommon();
	~FRECloudCommon();


	// Common textures dimension
	static const int CloudMainNoiseTextureSize = 128;
	static const EPixelFormat CloudMainNoiseTextureFormat = EPixelFormat::PF_FloatRGBA;
	static const int TEMPCloudMainNoiseSeed = 2015;
	static const double TEMPCloudMainNoiseInitMultiplier;
	static const double TEMPCloudMainNoiseLacunarity;

	static const int CloudDetailNoiseTextureSize = 32;
	static const EPixelFormat CloudDetailNoiseTextureFormat = EPixelFormat::PF_FloatRGBA;
	static const int TEMPCloudDetailNoiseSeed = 5012;
	static const double TEMPCloudDetailNoiseInitMultiplier;
	static const double TEMPCloudDetailNoiseLacunarity;

private:

	/** main noise texture of cloud
	*/
	FTexture3DRHIRef CloudMainNoise3D;

	/** detail noise texture of cloud
	*/
	FTexture3DRHIRef CloudDetailNoise3D;

	bool CloudInitialized;

	TArray<FFloat16Color> MainNoiseColors;
	TArray<FFloat16Color> DetailNoiseColors;

public:
	FORCEINLINE FTexture3DRHIRef GetCloudMain() const {
		if (!CloudInitialized) {
			const_cast<FRECloudCommon*>(this)->_CreateMainNoise();
			const_cast<FRECloudCommon*>(this)->_CreateDetailNoise();
			const_cast<FRECloudCommon*>(this)->CloudInitialized = true;
		}
		return CloudMainNoise3D;
	}
	FORCEINLINE FTexture3DRHIRef GetCloudDetail() const {
		if (!CloudInitialized) {
			const_cast<FRECloudCommon*>(this)->_CreateMainNoise();
			const_cast<FRECloudCommon*>(this)->_CreateDetailNoise();
			const_cast<FRECloudCommon*>(this)->CloudInitialized = true;
		}
		return CloudDetailNoise3D;
	}

	/** \copydoc FRenderResource::InitDynamicRHI */
	virtual void InitDynamicRHI() override;
	/** \copydoc FRenderResource::ReleaseDynamicRHI */
	virtual void ReleaseDynamicRHI() override;

	/** get cloud loop perlin value
	*/
	float GetCloudMainValue(const FVector& InSampler);
	/** get cloud loop worley value
	*/
	float GetCloudDetailValue(const FVector& InSampler);


private:

	/** create main cloud noise 3D
	*/
	void _CreateMainNoise();

	/** create detail cloud noise 3D
	*/
	void _CreateDetailNoise();

};
extern RESKYENGINE_API TGlobalResource<FRECloudCommon> GRECloudCommon;











///
/// 参数表请参考NFCloudCommon.usf
///		Please see NFCloudCommon.usf for details of these parameters
///

BEGIN_UNIFORM_BUFFER_STRUCT(FCloudGenerateUniformShaderParameters, )
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, SamplePositionMultiply)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, SamplePositionBase)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FIntVector, CloudTextureSize)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, DensityBase)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, DensityMultiply)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(int, TEMPNoiseFunction)
END_UNIFORM_BUFFER_STRUCT(FCloudGenerateUniformShaderParameters)
typedef TUniformBufferRef<FCloudGenerateUniformShaderParameters> FCloudGenerateUniformBufferRef;


//BEGIN_UNIFORM_BUFFER_STRUCT(FCloudUniformShaderParameters, )
//DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, CloudPosition)
//DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, CloudStep)
//DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FIntVector, CloudTextureSize)
//END_UNIFORM_BUFFER_STRUCT(FCloudUniformShaderParameters)
//typedef TUniformBufferRef<FCloudUniformShaderParameters> FCloudUniformBufferRef;


BEGIN_UNIFORM_BUFFER_STRUCT(FCloudLightingUniformShaderParameters, )
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, LightDirection)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(int, StepCount)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FLinearColor, SunColor)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FIntVector, CloudLighingOcclusionTextureSize)
END_UNIFORM_BUFFER_STRUCT(FCloudLightingUniformShaderParameters)
typedef TUniformBufferRef<FCloudLightingUniformShaderParameters> FCloudLightingUniformBufferRef;



/** Galaxy cloud Compute Shader
*/
struct RESKYENGINE_API FNFCloudGenerateCS
	: public FGlobalShader
{
	DECLARE_SHADER_TYPE(FNFCloudGenerateCS, Global);
public:
	/** Default constructor */
	FNFCloudGenerateCS() { }
	virtual	~FNFCloudGenerateCS();

	/** Initialization constructor */
	explicit FNFCloudGenerateCS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	/** Cache only when using SM5 */
	static bool ShouldCache(EShaderPlatform Platform) {
		return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5);
	}

	/** Modify compilation environment
	*/
	static void ModifyCompilationEnvironment(EShaderPlatform Platform, FShaderCompilerEnvironment& OutEnvironment);

	/** Serialization
	*/
	virtual bool Serialize(FArchive& Ar) override;

	/** Set output buffers */
	void SetOutput(FRHICommandList& RHICmdList, FUnorderedAccessViewRHIRef OutCloudTexture3D);
	/** Sets uniform buffers */
	void SetUniformBuffers(FRHICommandList& RHICmdList, FCloudGenerateUniformShaderParameters& GenerateParameters);
	/** Set input parameters */
	void SetParameters(FRHICommandList& RHICmdList);

	/** Unbinds bound buffers */
	void UnbindBuffers(FRHICommandList& RHICmdList);

private:

	/* Output buffer
	*/
	FShaderResourceParameter OutputCloudTexture3D;

	/** The PerlinNoiseGradientTexture parameter for materials that use GradientNoise */
	FShaderResourceParameter PerlinNoiseGradientTexture;
	FShaderResourceParameter PerlinNoiseGradientTextureSampler;
	/** The PerlinNoise3DTexture parameter for materials that use GradientNoise */
	FShaderResourceParameter PerlinNoise3DTexture;
	FShaderResourceParameter PerlinNoise3DTextureSampler;

};



