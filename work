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

if ! cp -p "${experiment_dir}/AFTER.AG" "${experiment_dir}/WORK.AG" ; then
    echo "Failed to create working file"
    exit 1
fi

dosbox -conf examine/local.conf -c "MOUNT C examine/CDrive" -c "MOUNT D ${experiment_dir}" -c "C:" -c "CD AGENDA" -c "AGENDA D:\\WORK.AG" -c "EXIT" 2> /dev/null