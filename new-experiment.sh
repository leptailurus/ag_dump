#!/bin/sh

usage() {
    echo "Usage: new-experiment.sh input-file experiment-name"
    exit 1
}

if [ $# -ne 2 ]; then
    usage
fi

if [ ! -f "$1" ]; then
    echo "Invalid input file!"
    exit 1
fi

timestamp=`date +%Y%m%d_%H%M%S`
experiment_name="${timestamp}_$2"
experiment_dir="examine/experiments/${experiment_name}"

if ! mkdir -p "${experiment_dir}" ; then
    echo "Failed to create experiment directory"
    exit 1
fi

if ! cp -p "$1" "${experiment_dir}/BEFORE.AG" ; then
    echo "Failed to copy input file"
    exit 1
fi

if ! cp -p "${experiment_dir}/BEFORE.AG" "${experiment_dir}/AFTER.AG" ; then
    echo "Failed to create output file"
    exit 1
fi

echo -n ${experiment_name} > experiment

dosbox -conf examine/local.conf -c "MOUNT C examine/CDrive" -c "MOUNT D ${experiment_dir}" -c "C:" -c "CD AGENDA" -c "AGENDA D:\\AFTER.AG" -c "EXIT" 2> /dev/null
