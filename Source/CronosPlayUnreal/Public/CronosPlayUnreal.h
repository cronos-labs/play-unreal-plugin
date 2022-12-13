// Copyright 2022, Cronos Labs. All Rights Reserved

#pragma once

#include "Modules/ModuleManager.h"

class FCronosPlayUnrealModule : public IModuleInterface {
  public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
