keytool ^
-genkey -v -keystore key.keystore -alias Mykey -keyalg RSA -keysize 2048 -validity 10000 ^
-dname "CN=TestGuy, OU=MyCompany, O=MyGame, L=MyCity, ST=NC, C=00" ^
-storepass 123456 -keypass 123456
if not exist "Build\Android" mkdir Build\Android
move key.keystore Build\Android
