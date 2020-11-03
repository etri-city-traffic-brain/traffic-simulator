mkdir -p build; 

cd build; 
#cmake --verbose ../src;
cmake --verbose -DCMAKE_BUILD_TYPE=Debug ../src;
make -j8 libsalt
