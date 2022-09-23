
#include "CronosPlayUnrealDemoGameMode.h"
#include "CronosPlayUnrealDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACronosPlayUnrealDemoGameMode::ACronosPlayUnrealDemoGameMode() {
  // set default pawn class to our Blueprinted character
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
      TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
  if (PlayerPawnBPClass.Class != NULL) {
    DefaultPawnClass = PlayerPawnBPClass.Class;
  }
}
