info=`ls ./build/lib`
if [[ -z "$info" ]]; then
    ./build.sh
fi
sudo cp ./build/lib/libsimpool.* /usr/local/lib
sudo cp ./src/simpool.h /usr/local/include
echo "Install OK"
