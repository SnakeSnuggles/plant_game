:: rmdir /s /q "build"
:: mkdir build
rmdir /s /q "build/assets/"
robocopy "assets/" "build/assets/" /e /R:2 /W:1
cd build

cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
:: mingw32-make clean
mingw32-make LDFLAGS="-static"