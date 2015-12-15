/**
*	基本思路：
*		最外层的大气效果是一个随机发生器。
*			实际上是多个随机发生器的叠加，首要发生器产生随机后，会被Turbulence湍流化，次要发生器、大气移动发生器及自转方向影响器对其进行Filter
*			除Turbulence湍流化外，素有的随机发生器都需要具有CPU和GPU同步的版本。
*		根据当前地面基本法线（球面对球心位置连线所求得的法线）决定随机发生器的采样点。
*		采样点取得的最终数值对之后接近后的效果会有影响。
*/
#pragma once
//
//#include "NFGalaxyCloudActor.h"
//
//#include "NFGalaxyCloudRenderEngine.h"
//
//
////struct FAtmosphereCloudGenerateParameter
////{
////	virtual ~FAtmosphereCloudGenerateParameter();
////
////	FName ParameterName;
////
////	virtual ApplyToMaterial(class UMaterialInterface* InMaterial) = 0;
////
////};
////struct FAtmosphereCloudGenerateParameterFloat
////	: public FAtmosphereCloudGenerateParameter
////{
////	float DefaultValue;
////	float MinValue;
////	float MaxValue;
////	float CurrentValue;
////
////	virtual ApplyToMaterial(class UMaterialInterface* InMaterial);
////};
////struct FAtmosphereCloudGenerateParameterVec3
////	: public FAtmosphereCloudGenerateParameter
////{
////	FName ParameterName;
////	FVector DefaultValue;
////	FVector MinValue;
////	FVector MaxValue;
////	FVector CurrentValue;
////
////	virtual ApplyToMaterial(class UMaterialInterface* InMaterial);
////};
//
////
////class UCloudNoisePhase
////{
////public:
////
////};
//
////
/////** 大气层云生成器
////*/
////class UAtmosphereCloudGenerator
////{
////public:
////
////	/** 首要生成阶段Noise
////	*/
////	UPROPERTY(EditAnywhere, Category = "NFNoise")
////		UNFNoiseGenerator*	ChiefNoise;
////
////	UPROPERTY(EditAnywhere, Category = "NFNoise")
////		int32 ChiefNoiseTextureSize;
////
////	/** 首要Noise纹理
////	*/
////	UPROPERTY(VisibleAnywhere, Category = "NFNoise")
////		UTexture2D*			ChiefNoiseTexture;
////
////	/** Turbulence湍流阶段
////	*	其配置需要跟UE同步，详见UNFNoiseGeneratorModule_UE4ShaderTurbulence文档
////	*/
////	UPROPERTY(VisibleAnywhere, Category = "NFNoise")
////		class UNFNoiseGeneratorModule* TurblenceModule;
////
////
////	// TODO PostProcess
////	///** 后期影响Noise
////	//*/
////	//UPROPERTY()
////	//	UNFNoiseGenerator*	PostProcessNoise;
////
////	///** 后期影响阶段Noise 纹理
////	//*/
////	//UTexture2D*			PostProcessNoiseTexture;
////
////private:
////
////	/** 生成各式Texture纹理
////	*	生成到特殊位置，并改变当前的纹理资源
////	*/
////	void _GenerateNoiseTexture();
////
////	/** 应用Turbulence到材质中
////	*/
////	void _ApplyTurbulenceToMaterial(UMaterialInterface* InMaterial);
////
////	/** 应用Noise纹理到材质中
////	*	一般是Noise发生变化后才需要调用，这些纹理都是具有明确路径明确访问的特殊纹理
////	*/
////	void _ApplyNoiseTextureToMaterial(UMaterialInterface* InNoiseTexture);
////
////};
////
////
//
///** 生成Cloud所需要的所有参数
//*	Render resources for Cloud rendering
//*/
//class FNFCloudGenSceneProxy
//	: public FPrimitiveSceneProxy
//{
//public:
//	FNFCloudGenSceneProxy(class UNFCloudComponent* InComponent);
//	virtual	~FNFCloudGenSceneProxy();
//
//private:
//
//	/** cloud generation parameters
//	*/
//	FCloudGenerateUniformShaderParameters GenerateParameters;
//
//	/** lighting parameters
//	*/
//	FCloudLightingUniformShaderParameters LightingParameters;
//
//private:
//
//	/** Host component
//	*/
//	UNFCloudComponent* HostComponent;
//
//	/** cloud texture
//	*	regenerate when cloud changed
//	*/
//	FTexture3DRHIRef CloudTextureRHI;
//
//	/** UAV for cloud density texture
//	*	access from ComputeShader
//	*/
//	FUnorderedAccessViewRHIRef CloudTextureUAV;
//
//	FTexture3DRHIRef CloudLightingOcclusionTextureRHI;
//
//	/** UAV for cloud density texture
//	*	access from ComputeShader
//	*/
//	FUnorderedAccessViewRHIRef CloudLighingOcclusionTextureUAV;
//
//public:
//
//	// Begin UPrimitiveSceneProxy interface
//	/*!	\copydoc FPrimitiveSceneProxy::CreateRenderThreadResources
//	*/
//	virtual void CreateRenderThreadResources() override;
//	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View);
//	virtual bool CanBeOccluded() const override;
//	virtual uint32 GetMemoryFootprint() const override;
//	uint32 GetAllocatedSize() const;
//	/*!	\copydoc FPrimitiveSceneProxy::DrawDynamicElements
//	*/
//	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override;
//	// End UPrimitiveSceneProxy interface
//
//public:
//
//	void SetCloudTexture_GameThread(UTexture3DDynamic* InTexture);
//	void SetCloudTexture_RenderThread(UTexture3DDynamic* InTexture);
//	void SetCloudOpticalDepthTexture_GameThread(UTexture3DDynamic* InTexture);
//	void SetCloudOpticalDepthTexture_RenderThread(UTexture3DDynamic* InTexture);
//
//	/** Set cloud generation parameters (at game thread) */
//	void SetCloudGenerateParameters_GameThread(const FCloudGenerateParameters& InParameters);
//	/** Set cloud generation parameters (at render thread) */
//	void SetCloudGenerateParameters_RenderThread(const FCloudGenerateParameters& InParameters);
//
//	/** Set cloud lighting parameters (at game thread) */
//	void SetCloudLightingParameters_GameThread(const FCloudLightingParameters& InParameters);
//	/** Set cloud lighting parameters (at render thread) */
//	void SetCloudLightingParameters_RenderThread(const FCloudLightingParameters& InParameters);
//
//	void UpdateCloud_GameThread();
//	void UpdateCloudLighting_GameThread();
//
//private:
//
//	/** regenerate cloud resources
//	*/
//	void _UpdateGenerator(FSceneInterface* InScene, FRHICommandList& RHICmdList);
//
//	/** regenerate cloud lighting resource
//	*/
//	void _UpdateLighting(FSceneInterface* InScene, FRHICommandList& RHICmdList);
//
//};
