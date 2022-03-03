all: updatesource copyfiles 

ue4all: updatesource copyfiles compileunreal

copyfiles:
	cd defi-wallet-core-rs && make cpp
	cd defi-wallet-core-rs && make cppx86_64
	cp ./defi-wallet-core-rs/example/cpp-example/*.h DefiWalletCoreUnreal/Plugins/DefiWalletCore/Source/DefiWalletCore/Private/
	cp ./defi-wallet-core-rs/example/cpp-example/*.a DefiWalletCoreUnreal/Plugins/DefiWalletCore/Source/DefiWalletCore/Private/

updatesource:
	git submodule update --init --recursive

compileunreal:
	cd DefiWalletCoreUnreal && compileunreal.sh 
	
