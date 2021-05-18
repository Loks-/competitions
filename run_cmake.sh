mkdir -p debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cd ..

mkdir -p release
cd release
cmake -DCMAKE_BUILD_TYPE=Release ..
cd ..

# mkdir -p relwithdebinfo
# cd relwithdebinfo
# cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
# cd ..

