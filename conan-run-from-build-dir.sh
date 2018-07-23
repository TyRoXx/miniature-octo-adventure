#!/usr/bin/env sh
sudo apt-get install python3-pip
sudo pip3 install --upgrade pip
sudo pip3 install setuptools
sudo pip3 install conan
conan remote add publicconan https://api.bintray.com/conan/bincrafters/public-conan
BASEDIR=$(dirname "$0")
conan install $BASEDIR
