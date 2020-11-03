mkdir -p build;
cd build;
#cmake -f  ..
cmake -f -DCMAKE_BUILD_TYPE=Debug ..
make -j8 VisServer.out
