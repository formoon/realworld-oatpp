#!/bin/sh

rm -rf tmp

mkdir tmp
cd tmp
##########################################################
prepare(){
    sudo apt install build-essential cmake git -y
    sudo apt install zlib1g-dev libssl-dev nlohmann-json3-dev libpq-dev postgresql-server-dev-12 -y
}

##########################################################
installModule(){
    git clone --depth=1 https://github.com/oatpp/$1

    cd $1
    mkdir build
    cd build

    cmake ..
    make
    sudo make install

    cd ../../
}
##########################################################
## install oatpp

prepare
installModule "oatpp"
installModule "oatpp-zlib"
installModule "oatpp-postgresql"


##########################################################

cd ../

#rm -rf tmp