 cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
 cmake --build ./build
 #./build/algoai 
 ctest --test-dir build --verbose