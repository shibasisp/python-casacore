#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then

    # Install some custom requirements on OS X
    brew install pyenv-virtualenv

    case "${TOXENV}" in
        py27)
            # Install some custom Python 2.7 requirements on OS X
            brew update;
            brew install python;
            virtualenv venv -p python;
            source venv/bin/activate;
            ;;
        py35)
            # Install some custom Python 3.5 requirements on OS X
            brew update;
            brew install python;3
            virtualenv venv -p python3;
            source venv/bin/activate;
            ;;
        py36)
            # Install some custom Python 3.6 requirements on OS X
            brew update;
            brew install python3;
            virtualenv venv -p python3;
            source venv/bin/activate;
            ;;
    esac
else
    # Install some custom requirements on Linux
fi