#!/bin/bash

set -e
set -x

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
  sudo apt-get update -qq
  sudo apt-get install software-properties-common \
      python-setuptools libboost-python-dev \
      libcfitsio3-dev
  sudo add-apt-repository -y ppa:radio-astro/main
  sudo apt-get update -qq
  sudo apt-get install libcasacore2-dev casacore-data

elif [ "$TRAVIS_OS_NAME" = osx ]; then
    brew tap ska-sa/tap
    brew update >/dev/null
    brew install casacore
fi
