#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status

build_and_run() {
    local src_dir=$1
    local build_dir=$2

    if [[ ! -f "${src_dir}/CMakeLists.txt" ]]; then
        echo "Error: No CMakeLists.txt found in source directory."
        return 1
    fi

    mkdir -p "${build_dir}"
    pushd "${build_dir}" > /dev/null

    cmake -G "Unix Makefiles" -B "." "${src_dir}" && \
    make

    popd > /dev/null  # Go back to the parent directory
    "${build_dir}/VoiceAssistant"  # Execute VoiceAssistant from the parent directory
}

main() {
    local current_dir="$(pwd)"
    local build_dir="${current_dir}/build"

    if build_and_run "${current_dir}" "${build_dir}"; then
        exit 0
    elif [[ -d "${current_dir}/.." ]] && build_and_run "${current_dir}/.." "${build_dir}"; then
        exit 0
    else
        echo "Error: Couldn't configure and build the project."
        exit 1
    fi
}

main "$@"
