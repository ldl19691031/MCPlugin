// Some copyright should be here...

#pragma once

#include "ModuleManager.h"
#include "Engine.h"


class FMCpluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};