#!/bin/bash

createHeader () {
    baseName="${1}"
    upperName="${1^^}"
    text="#ifndef ${upperName}_HPP\n"
    text="${text}#define ${upperName}_HPP\n"
    text="${text}#pragma once\n\n"
    text="${text}#include \"libraries.hpp\"\n\n"
    text="${text}class ${baseName}\n{\n"
    text="${text}public:\n"
    text="${text}    explicit ${baseName}();\n"

    if [[ "${2}" = "implement" ]]; then
        text="${text}    ${baseName}(${baseName} const&);\n"
        text="${text}    ${baseName}(${baseName} &&);\n"
        text="${text}    ${baseName} & operator=(${baseName} const&);\n"
        text="${text}    ${baseName} & operator=(${baseName} &&);\n"
        text="${text}    ~${baseName}();\n\n"
    elif [[ "${2}" = "default" ]]; then
        text="${text}    ${baseName}(${baseName} const&) = default;\n"
        text="${text}    ${baseName}(${baseName} &&) = default;\n"
        text="${text}    ${baseName} & operator=(${baseName} const&) = default;\n"
        text="${text}    ${baseName} & operator=(${baseName} &&) = default;\n"
        text="${text}    ~${baseName}() = default;\n\n"
    else
        text="${text}    ${baseName}(${baseName} const&) = delete;\n"
        text="${text}    ${baseName}(${baseName} &&) = delete;\n"
        text="${text}    ${baseName} & operator=(${baseName} const&) = delete;\n"
        text="${text}    ${baseName} & operator=(${baseName} &&) = delete;\n"
        text="${text}    ~${baseName}() = default;\n\n"
    fi;

    text="${text}private:\n\n};\n\n"
    text="${text}#endif"
    echo -e "${text}" > "${baseName}.hpp"
}

createSource () {
    baseName="${1}"
    text="#include \"${baseName}.hpp\"\n\n"
    text="${text}${baseName}::${baseName}()\n{\n\n}"

    if [[ "${2}" = "implement" ]]; then
        text="\n\n${text}${baseName}::${baseName}(${baseName} const&)\n{\n\n}\n\n"
        text="${text}${baseName}::${baseName}(${baseName} &&)\n{\n\n}\n\n"
        text="${text}${baseName} & ${baseName}::operator=(${baseName} const&)\n{\n\n}\n\n"
        text="${text}${baseName} & ${baseName}::operator=(${baseName} &&)\n{\n\n}\n\n"
        text="${text}${baseName}::~${baseName}()\n{\n\n}"
    fi;

    echo -e "${text}" > "${baseName}.cpp"
}

createClass () {
    if [[ "${2}" = "base" ]]; then
        cd includes
        createHeader $1 delete
        cd ..
        cd sources
        createSource $1 delete
        cd ..
        echo "${1} created."
    else
        cd ../includes
        createHeader $1 delete
        cd ..
        cd sources
        createSource $1 delete
        cd ..
        echo "${1} created."
    fi;
}

overwrite () {
    header="includes/${1}.hpp"
    source="sources/${1}.cpp"

    if [[ -d "includes" ]] && [[ -d "sources" ]]; then
        if [[ -f "$header" || -f "$source" ]]; then
            echo "${1} already exists."
            echo "Overwrite it? [y/n]"
            read answer

            if [[ $answer == "y" ]]; then
                echo "Overwriting ${1}..."
                createClass $1 base
            fi;
        else
            createClass $1 base
        fi;
    elif [[ -d "../includes" ]] && [[ -d "../sources" ]]; then
        header="../${header}"
        source="../${source}"

        if [[ -f "$header" || -f "$source" ]]; then
            echo "${1} already exists."
            echo "Overwrite it? [y/n]"
            read answer

            if [[ $answer == "y" ]]; then
                echo "Overwriting ${1}..."
                createClass $1 back
            fi;
        else
            createClass $1 back
        fi;
    else
        echo "Couldn't find includes and sources directories"
        exit 1
    fi;
}

while [[ $# -gt 0 ]]; do
    overwrite $1
    shift
done;
exit 0;
