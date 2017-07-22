#!/usr/bin/env bash

# Inspired by https://conda.io/docs/travis.html
env

set -e
set -v

# if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then brew update; fi
# if [[ "$TRAVIS_OS_NAME" == "osx" ]]; then 
# 	if [[ "$TRAVIS_PYTHON_VERSION" == "2.7" ]]; then
#   		brew install python;
#   		virtualenv venv -p python;
#   		source venv/bin/activate;
# 	else
#   		brew install python3;
#   		virtualenv venv -p python3;
#   		source venv/bin/activate;
#   	fi
# fi


if [ "$TRAVIS_OS_NAME" = linux ]; then
    sudo apt-get update
    MINICONDAVERSION="Linux"
else
    MINICONDAVERSION="MacOSX"
fi

if [[ "$TRAVIS_PYTHON_VERSION" == "2.7" ]]; then
  wget https://repo.continuum.io/miniconda/Miniconda2-latest-$MINICONDAVERSION-x86_64.sh -O miniconda.sh;
else
  wget https://repo.continuum.io/miniconda/Miniconda3-latest-$MINICONDAVERSION-x86_64.sh -O miniconda.sh;
  fi

bash miniconda.sh -b -p $HOME/miniconda
export PATH="$HOME/miniconda/bin:$PATH"
hash -r
conda config --set always_yes yes --set changeps1 no
conda update -q conda
conda config --add channels conda-forge
# Useful for debugging any issues with conda
conda info -a
which python
conda create -q -n testenv python=$TRAVIS_PYTHON_VERSION casacore=2.3.0

echo "measures.directory: /home/travis/data" > $HOME/.casarc
