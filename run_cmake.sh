mkdir -p debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cd ..

mkdir -p release
cd release
cmake -DCMAKE_BUILD_TYPE=Release ..
cd ..
