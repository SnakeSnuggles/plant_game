cd build

cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
:: mingw32-make clean
mingw32-make