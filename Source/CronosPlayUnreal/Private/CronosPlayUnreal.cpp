#include "CronosPlayUnreal.h"

#define LOCTEXT_NAMESPACE "FCronosPlayUnrealModule"

void StopWalletConnect();
void FCronosPlayUnrealModule::StartupModule() {}

void FCronosPlayUnrealModule::ShutdownModule() { StopWalletConnect(); }

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCronosPlayUnrealModule, CronosPlayUnreal)
