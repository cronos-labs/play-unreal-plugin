// Fill out your copyright notice in the Description page of Project Settings.
#include "DefiWalletCoreActor.h"
#include "lib.rs.h"
#include <iostream>
#include <sstream>
using namespace std;
using namespace org::defi_wallet_core;

// Sets default values
ADefiWalletCoreActor::ADefiWalletCoreActor()
    : myCosmosRpc("http://mynode:1317"), myTendermintRpc("http://mynode:26657"),
      myMnemonics("your mnemonics"), myChainID("testnet-baseball-1"),
      myFrom("cro1....."), myTo("cro1....."), myAmount(1),
      myAmountDenom("basecro")

{

  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADefiWalletCoreActor::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ADefiWalletCoreActor::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
