// Some copyright should be here...

#pragma once

#include "ModuleManager.h"



class FNFRenderExtentModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};