// Copyright 2022, Cronos Labs. All Rights Reserved

#include "Utlis.h"
#include "Containers/UnrealString.h"

using namespace rust;

FString UUtlis::ToHex(TArray<uint8> address) {
    return BytesToHex(address.GetData(), address.Num());
}

std::array<std::uint8_t, 20> UUtlis::ToArray(TArray<uint8> address) {
    std::array<std::uint8_t, 20> std_array{};
    if (address.Num() == 20) {
        auto data_begin = address.GetData();
        std::copy_n(data_begin, 20, std_array.begin());
    } else {
        std_array.fill(0);
    }
    return std_array;
}

void UUtlis::copyTArrayToVec(const TArray<uint8> &src, rust::Vec<uint8_t> &dst) {
    dst.clear();
    for (int i = 0; i < src.Num(); i++) {
        dst.push_back(src[i]);
    }
    assert(dst.size() == src.Num());
}
void UUtlis::copyVecToTArray(const rust::Vec<uint8_t> &src,
                             TArray<uint8> &dst) {
    dst.Empty();
    for (int i = 0; i < src.size(); i++) {
        dst.Add(src[i]);
    }
    assert(dst.Num() == src.size());
}
