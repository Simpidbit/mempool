cd ./src

osname=`uname`
macos="Darwin"

if [[ $osname =~ $macos ]]; then
    cc ./*.c -o libsimpool.dylib -shared -fPIC
else
    cc ./*.c -o libsimpool.so -shared -fPIC
fi

cd ..
mkdir -p ./build/lib
mv ./src/libsimpool.* ./build/lib
echo "Build OK"
