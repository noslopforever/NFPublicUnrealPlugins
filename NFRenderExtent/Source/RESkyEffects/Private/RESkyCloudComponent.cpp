#include "RESkyEffectsPCH.h"
//#include "NFCloudComponent.h"
//#include "Engine/Texture3DDynamic.h"
//#include "NFGalaxyCloud.h"
//
//
////------------------------------------------------------------------------
//UNFCloudComponent::UNFCloudComponent(const class FObjectInitializer& PCIP)
//	: Super(PCIP)
//{
//	CloudTextureSize = FIntVector(128, 128, 128);
//	CloudOpticalDepthTextureSize = FIntVector(128, 128, 128);
//}
//
////------------------------------------------------------------------------
//#if WITH_EDITOR
//void UNFCloudComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
//{
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//
//	// property changed event
//	if (PropertyChangedEvent.Property
//		&& (PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UNFCloudComponent, CloudTextureSize)
//		|| PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UNFCloudComponent, CloudOpticalDepthTextureSize)
//		)
//		)
//	{
//		TEMPUpdateTextureResources();
//	}
//}
//#endif
//
////------------------------------------------------------------------------
//void UNFCloudComponent::PostLoad()
//{
//	Super::PostLoad();
//
//	TEMPUpdateTextureResources();
//}
//
////------------------------------------------------------------------------
//void UNFCloudComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//}
//
////------------------------------------------------------------------------
//FPrimitiveSceneProxy* UNFCloudComponent::CreateSceneProxy()
//{
//	return new FNFCloudGenSceneProxy(this);
//}
//
////------------------------------------------------------------------------
//void UNFCloudComponent::TEMPUpdateTextureResources()
//{
//	CloudTexture3D = UTexture3DDynamic::Create(
//		CloudTextureSize.X
//		, CloudTextureSize.Y
//		, CloudTextureSize.Z
//		, PF_G8, true);
//
//	CloudOpticalDepthTexture3D = UTexture3DDynamic::Create(
//		CloudOpticalDepthTextureSize.X
//		, CloudOpticalDepthTextureSize.Y
//		, CloudOpticalDepthTextureSize.Z
//		, PF_G8, true);
//
//	FNFCloudGenSceneProxy* sceneProxy = static_cast<FNFCloudGenSceneProxy*>(SceneProxy);
//	if (sceneProxy)
//	{
//		if (CloudOpticalDepthTexture3D)
//		{
//			sceneProxy->SetCloudOpticalDepthTexture_GameThread(CloudOpticalDepthTexture3D);
//		}
//		if (CloudTexture3D)
//		{
//			sceneProxy->SetCloudTexture_GameThread(CloudTexture3D);
//		}
//	}
//}
//
////------------------------------------------------------------------------
//void UNFCloudComponent::UpdateCloudTexture()
//{
//	FNFCloudGenSceneProxy* sceneProxy = static_cast<FNFCloudGenSceneProxy*>(SceneProxy);
//	sceneProxy->SetCloudGenerateParameters_GameThread(GenerationParameters);
//	sceneProxy->UpdateCloud_GameThread();
//}
//
////------------------------------------------------------------------------
//void UNFCloudComponent::UpdateCloudLightingTexture()
//{
//	FNFCloudGenSceneProxy* sceneProxy = static_cast<FNFCloudGenSceneProxy*>(SceneProxy);
//	sceneProxy->SetCloudLightingParameters_GameThread(LightingParameters);
//	sceneProxy->UpdateCloudLighting_GameThread();
//}
//
////------------------------------------------------------------------------
