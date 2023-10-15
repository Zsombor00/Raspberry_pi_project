#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status

# Function to pause script execution and prompt user to continue
prompt_continue() {
    echo "$1"  # Display message to user
    read -p "Press ENTER to continue or Ctrl+C to abort..."  # Prompt user to continue
}

build_and_run() {
    local src_dir=$1
    local build_dir=$2

    if [[ ! -f "${src_dir}/CMakeLists.txt" ]]; then
        echo "Error: No CMakeLists.txt found in source directory."
        exit 1
    fi

    prompt_continue "CMakeLists.txt found. Preparing to create build directory..."

    mkdir -p "${build_dir}"
    pushd "${build_dir}" > /dev/null

    prompt_continue "Build directory created. Preparing to configure the project..."

    if ! cmake -G "Unix Makefiles" -B "." "${src_dir}"; then
        echo "Error: Configuration failed."
        exit 1
    fi

    prompt_continue "Project configured. Preparing to build the project..."

    if ! make; then
        echo "Error: Build failed."
        exit 1
    fi

    popd > /dev/null  # Go back to the parent directory

    prompt_continue "Build succeeded. Preparing to execute VoiceAssistant..."

    if ! "${build_dir}/VoiceAssistant"; then
        echo "Error: Execution failed."
        exit 1
    fi
}

main() {
    local current_dir="$(pwd)"
    local build_dir="${current_dir}/build"

    prompt_continue "Starting script execution..."

    if ! build_and_run "${current_dir}" "${build_dir}"; then
        echo "Error: Couldn't configure and build the project."
        exit 1
    fi

    echo "Script executed successfully."
}

main "$@"
