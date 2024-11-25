#!/bin/bash

options=( "pi" "piMonteCarlo" "gravler" "yellowstonePerm" )
hasArg3=( "yellowstonePerm" )

help(){
    if [ $# -eq 0 ]; then
        echo Options are:
        echo  ${options[*]}
        echo They can be passed as args
        echo Use option -h for more info on for each option
    elif [ $# -eq 1 ]; then
        echo $1:
        awk -v section="$1" '$0 ~ "^" section ":" {flag=1; next} /^2:/ {flag=0} flag && !/:/ {print}' ./algorithms/help.txt
    fi
}

about(){
    echo $1:
    awk -v section="$1" '$0 ~ "^" section ":" {flag=1; next} /^2:/ {flag=0} flag && !/:/ {print}' ./algorithms/about.txt
}

contains() {
    local array=("$@")
    local value=${array[-1]}  # The last argument is the value to search for
    unset array[-1]           # Remove the last element (search value)

    for item in "${array[@]}"; do
        if [ "$item" == "$value" ]; then
            return 0  # Found
        fi
    done
    return 1  # Not found
}

#gcc fibanachi.c  -o fibonachi.out
#gcc rand_test.c -o rand_test.out -lm


#if no args
if [ $# -eq 0 ]; then
    echo "Algorithms to run: "
    echo  ${options[@]}
    read choice
fi

#if has arg
if [ $# -ge 1 ]; then
    choice=$1
    if [ $choice == "-h" ]; then
        help
        exit 0;
    elif ! printf '%s\0' "${options[@]}" | grep -Fxqz -- "$1"; then
        echo uknown option "$1"
        help
        exit 1;
    fi
fi

#if a.out exist remove it
if [[ -e "a.out" ]]; then
    rm a.out
fi

#finds choices
case $choice in
    pi)
        gcc algorithms/pi.c -lm -fopenmp -O1
        ;;
    piMonteCarlo)
        gcc algorithms/piMonteCarlo.c -lm -fopenmp -O1
        ;;
    gravler)
        gcc algorithms/gravler.c -fopenmp -O3
        ;;
    yellowstonePerm)
        gcc algorithms/yellowstonePerm.c -lm -fopenmp -O2
        ;;
    *)
        echo "Invalid option"
        exit 1;
        ;;
esac

#second arg
if [ ! $# -ge 2 ]; then
    echo "Number of times to run: "
    read arg2
fi

if [ $# -ge 2 ]; then
    case $2 in
        -h)
        help $1
        exit 0;
        ;;
        -a)
        about $1
        exit 0;
        ;;
        *)
        arg2=$2
    esac
fi

#third arg
if ! contains "${hasArg3[@]}" "$choice"; then
    arg3=""
elif [ ! $# -eq 3 ]; then
    echo "Enter args"
    read $arg3
elif [ $# -eq 3 ]; then
    arg3=$3
fi

#runs it
if [[ -e "a.out" ]]; then
    ./a.out $arg2 $arg3
    rm "a.out"
fi


