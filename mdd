#!/bin/sh

if ! make ; then
    echo "Make failed!"
    exit 1
fi

./dump.sh

./diff.sh