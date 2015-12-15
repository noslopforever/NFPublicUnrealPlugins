/**
*	����˼·��
*		�����Ĵ���Ч����һ�������������
*			ʵ�����Ƕ������������ĵ��ӣ���Ҫ��������������󣬻ᱻTurbulence����������Ҫ�������������ƶ�����������ת����Ӱ�����������Filter
*			��Turbulence�������⣬���е��������������Ҫ����CPU��GPUͬ���İ汾��
*		���ݵ�ǰ����������ߣ����������λ����������õķ��ߣ���������������Ĳ����㡣
*		������ȡ�õ�������ֵ��֮��ӽ����Ч������Ӱ�졣
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
/////** ��������������
////*/
////class UAtmosphereCloudGenerator
////{
////public:
////
////	/** ��Ҫ���ɽ׶�Noise
////	*/
////	UPROPERTY(EditAnywhere, Category = "NFNoise")
////		UNFNoiseGenerator*	ChiefNoise;
////
////	UPROPERTY(EditAnywhere, Category = "NFNoise")
////		int32 ChiefNoiseTextureSize;
////
////	/** ��ҪNoise����
////	*/
////	UPROPERTY(VisibleAnywhere, Category = "NFNoise")
////		UTexture2D*			ChiefNoiseTexture;
////
////	/** Turbulence�����׶�
////	*	��������Ҫ��UEͬ�������UNFNoiseGeneratorModule_UE4ShaderTurbulence�ĵ�
////	*/
////	UPROPERTY(VisibleAnywhere, Category = "NFNoise")
////		class UNFNoiseGeneratorModule* TurblenceModule;
////
////
////	// TODO PostProcess
////	///** ����Ӱ��Noise
////	//*/
////	//UPROPERTY()
////	//	UNFNoiseGenerator*	PostProcessNoise;
////
////	///** ����Ӱ��׶�Noise ����
////	//*/
////	//UTexture2D*			PostProcessNoiseTexture;
////
////private:
////
////	/** ���ɸ�ʽTexture����
////	*	���ɵ�����λ�ã����ı䵱ǰ��������Դ
////	*/
////	void _GenerateNoiseTexture();
////
////	/** Ӧ��Turbulence��������
////	*/
////	void _ApplyTurbulenceToMaterial(UMaterialInterface* InMaterial);
////
////	/** Ӧ��Noise����������
////	*	һ����Noise�����仯�����Ҫ���ã���Щ�����Ǿ�����ȷ·����ȷ���ʵ���������
////	*/
////	void _ApplyNoiseTextureToMaterial(UMaterialInterface* InNoiseTexture);
////
////};
////
////
//
///** ����Cloud����Ҫ�����в���
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
