
#include "CronosPlaySdkGameMode.h"
#include "CronosPlaySdkCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACronosPlaySdkGameMode::ACronosPlaySdkGameMode() {
  // set default pawn class to our Blueprinted character
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
      TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
  if (PlayerPawnBPClass.Class != NULL) {
    DefaultPawnClass = PlayerPawnBPClass.Class;
  }
}
