#pragma once

//#include "NFCloudComponent.generated.h"
//
//
//USTRUCT()
//struct FCloudGenerateParameters
//{
//	GENERATED_USTRUCT_BODY()
//public:
//	FCloudGenerateParameters()
//	{
//		TEMPNoiseFunction = 3;
//		SamplePositionBase = FVector(0, 0, 0);
//		SamplePositionMultiply = FVector(1, 1, 1);
//		DensityBase = 0;
//		DensityMultiply = 1;
//	}
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFGalaxy|Cloud")
//	FVector		SamplePositionMultiply;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFGalaxy|Cloud")
//	FVector		SamplePositionBase;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFGalaxy|Cloud")
//	float		DensityBase;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFGalaxy|Cloud")
//	float		DensityMultiply;
//	
//	/**
//	case 0:	return SimplexNoise3D_TEX(Position);
//	case 1:	return GradientNoise3D_TEX(Position);
//	case 2:	return PerlinNoise3D_ALU(Position);
//	case 3:	return FastGradientPerlinNoise3D_TEX(Position);
//	*/
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFGalaxy|Cloud")
//	int32		TEMPNoiseFunction;
//
//};
//
//USTRUCT()
//struct FCloudLightingParameters
//{
//	GENERATED_USTRUCT_BODY()
//public:
//	FCloudLightingParameters()
//	{
//		LightDirection = FVector(0, 0, 1);
//		StepCount = 256;
//		SunColor = FLinearColor(1, 1, 1, 1);
//	}
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFGalaxy|Cloud")
//	FVector			LightDirection;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFGalaxy|Cloud")
//	int32			StepCount;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFGalaxy|Cloud")
//	FLinearColor	SunColor;
//};
//
///** class UNFCloudComponent
////	TODO texture Pool : 3textures : calculating 1, pending 1, using 1
//*/
//UCLASS(hidecategories = (Collision, Object, Physics, SceneComponent), ClassGroup = Rendering, MinimalAPI, editinlinenew, meta = (BlueprintSpawnableComponent))
//class UNFCloudComponent
//	: public UPrimitiveComponent
//{
//	GENERATED_UCLASS_BODY()
//public:
//
//	/** Cloud generation parameters
//	*/
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFGalaxy|Cloud")
//	FCloudGenerateParameters	GenerationParameters;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFGalaxy|Cloud")
//	FCloudLightingParameters	LightingParameters;
//
//	/** Cloud Texture
//	*/
//	UPROPERTY(BlueprintReadOnly, Category = "NFGalaxy|Cloud", Transient)
//	class UTexture3DDynamic*	CloudTexture3D;
//
//	/** Cloud Occlusion Texture
//	*/
//	UPROPERTY(BlueprintReadOnly, Category = "NFGalaxy|Cloud", Transient)
//	class UTexture3DDynamic*	CloudOpticalDepthTexture3D;
//
//	/** Cloud Texture Width
//	*/
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFGalaxy|Cloud")
//	FIntVector CloudTextureSize;
//
//	/** Cloud Texture Width
//	*/
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFGalaxy|Cloud")
//	FIntVector CloudOpticalDepthTextureSize;
//
//public:
//	// Begin UObject Interface
//#if WITH_EDITOR
//	/** \copydoc UNFCloudComponent::PostEditChangeProperty */
//	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
//#endif
//	// End UObject Interface
//
//	// Begin UActorComponent Interface
//	/** \copydoc USceneComponent::PostLoad */
//	virtual void PostLoad() override;
//	/** \copydoc USceneComponent::TickComponent */
//	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
//	// End UActorComponent Interface
//
//	virtual	FPrimitiveSceneProxy* CreateSceneProxy() override;
//
//public:
//
//	/** 
//	*/
//	// TODO use texture pool
//	void TEMPUpdateTextureResources();
//
//	/** Update Cloud Texture
//	*/
//	UFUNCTION(BlueprintCallable, Category = "NFGalaxy|Cloud")
//	void UpdateCloudTexture();
//
//	UFUNCTION(BlueprintCallable, Category = "NFGalaxy|Cloud")
//	void UpdateCloudLightingTexture();
//
//};