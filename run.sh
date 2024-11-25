#!/bin/bash

options=("pi" "piMonteCarlo" "gravler" "-h")

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

#gcc yellowstoneAlg.c -lm -o yellowstoneAlg.out
#gcc yellowstoneAlgEuclidian.c -lm -o yellowstoneAlgE.out
#gcc fibanachi.c  -o fibonachi.out
#gcc gravler.c -fopenmp -O3 -o gravler.out
#gcc rand_test.c -o rand_test.out -lm



if [ $# -eq 0 ]; then
    echo "Algorithms to run: "
    echo  ${options[@]}
    read choice
fi

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

if [[ -e "a.out" ]]; then
    rm a.out
fi


case $choice in
    pi)
        gcc algorithms/pi.c -lm -fopenmp
        ;;
    piMonteCarlo)
        gcc algorithms/piMonteCarlo.c -lm -fopenmp -O1
        ;;
    gravler)
        gcc algorithms/gravler.c -fopenmp -O3
        ;;
    *)
        echo "Invalid option"
        exit 1;
        ;;
esac

if [ ! $# -eq 2 ]; then
    echo "Number of times to run: "
    read arg2
fi

if [ $# -eq 2 ]; then
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

if [[ -e "a.out" ]]; then
    ./a.out $arg2
    rm "a.out"
fi


