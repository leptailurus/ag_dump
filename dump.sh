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

ag_dump=$PWD/bin/ag_dump

if [ -f "${experiment_dir}/BEFORE.AG" ]; then
    (
        cd "${experiment_dir}"
        "${ag_dump}" BEFORE.AG > before.txt
    )
fi

if [ -f "${experiment_dir}/AFTER.AG" ]; then
    (
        cd "${experiment_dir}"
        "${ag_dump}" AFTER.AG > after.txt
    )
fi
