#!/bin/sh

if [ ! -f experiment ] ; then
    echo "Current experiment not set"
fi

experiment_name=`cat experiment`
echo "Experiment: ${experiment_name}"

experiment_dir="examine/experiments/${experiment_name}"
if [ ! -d "${experiment_dir}" ]; then
    echo "Current experiment is not valid"
    exit 1
fi

vimdiff "${experiment_dir}/before.txt" "${experiment_dir}/after.txt"