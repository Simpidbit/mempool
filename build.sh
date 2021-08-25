cd ./src

osname=`uname`
macos="Darwin"

if [[ $osname =~ $macos ]]; then
    cc ./*.c -o libsimpool.dylib -shared -fPIC
else
    cc ./*.c -o libsimpool.so -shared -fPIC
fi

cd ..
mv ./src/libsimpool.* ./build/lib
echo "Build OK"
