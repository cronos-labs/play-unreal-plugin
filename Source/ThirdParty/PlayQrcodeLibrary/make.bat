cl ./Source/qrcodegen.cpp -I ./Include /c /MD
lib ./qrcodegen.obj 
move ./qrcodegen.lib ./Lib/Win64