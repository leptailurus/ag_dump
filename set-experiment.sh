#!/bin/sh

usage() {
    echo "Usage: set-experiment.sh experiment-name-or-folder"
    exit 1
}

if [ $# -ne 1 ]; then
    usage
fi

experiment_name=`basename "$1"`
experiment_dir="examine/experiments/${experiment_name}"

if [ ! -d "${experiment_dir}" ]; then
    echo "Invalid experiment!"
    exit 1
fi

echo -n ${experiment_name} > experiment

echo "Experiment: ${experiment_name}"