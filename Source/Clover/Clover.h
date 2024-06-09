// Copyright Clover Games, Inc. All Rights Reserved.

#pragma once
#include "Modules/ModuleManager.h"

class FCloverGameModule final : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};