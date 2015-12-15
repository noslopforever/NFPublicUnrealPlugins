#pragma once

#include "RESkyEffectCommon.h"
#include "RESkyCloudActor.generated.h"


//
///** class ANFGalaxyCloudActor
//*	根据一定规则自动生成的Cloud Actor
//*/
//UCLASS()
//class ANFGalaxyCloudActor
//	: public AActor
//{
//	GENERATED_UCLASS_BODY()
//public:
//private:
//public:
//};


/** test sky effect actor
*/
UCLASS(Blueprintable, BlueprintType)
class ATestSkyEffectActor : public AActor
{
	GENERATED_UCLASS_BODY()
public:

	/** Cloud atmosphere info
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FRESkyAtmosphereInfo AtmosphereInfo;

	/** Cloud lighting info
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FRESkyLightingInfo LightingInfo;

	/** Cloud static info
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FRESkyCloudStaticInfo CloudStaticInfo;

	/** Cloud space info
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		FRESkyCloudSpaceInfo CloudSpaceInfo;

	/** Cloud map info
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		UTexture2D* TestCloudMap;

	/** Update lighting
	*/
	UPROPERTY(EditAnywhere, Category = Default)
		bool TestUpdateLighting;

	//UPROPERTY(EditAnywhere, Category = Default)
	//	FSkyCloudViewInfo CloudViewInfo;

	/** sky effect proxy
	*	like scene proxy
	*/
	FRESkyEffectProxy* SkyEffectProxy;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void PostInitProperties() override;
	virtual void PostLoad() override;
	virtual void FinishDestroy() override;

	void UpdateSkyEffect();

};