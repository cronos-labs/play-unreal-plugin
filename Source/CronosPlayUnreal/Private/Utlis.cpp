// Copyright 2022, Cronos Labs. All Rights Reserved

#include "Utlis.h"
#include "Containers/UnrealString.h"

FString UUtlis::ToHex(TArray<uint8> address) {
    return BytesToHex(address.GetData(), address.Num());
}
