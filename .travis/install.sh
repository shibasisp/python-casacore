#!/usr/bin/env bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then

    # Install some custom requirements on OS X
    brew install pyenv-virtualenv

    case "${TOXENV}" in
        py27)
            # Install some custom Python 2.7 requirements on OS X
            
            ;;
        py35)
            # Install some custom Python 3.5 requirements on OS X
            
            ;;
        py36)
            # Install some custom Python 3.6 requirements on OS X
            
            ;;
    esac
else
    # Install some custom requirements on Linux
fi