#!/bin/bash

RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
CYAN="\e[36m"
RESET="\e[0m"

REAL="real/"
ME="me/"
NULL="/dev/null"
TIME="/usr/bin/time"

if [ -f ${REAL}${1}.c ] && [ -f ${ME}${2}.c ];
then
    make -s -C .. re > ${NULL}
    mkdir -p .tmp
    
    gcc -o ${1}_real ${REAL}${1}.c
    echo -e "${GREEN}Compile ${REAL}${1}.c to  ${1}_real ${RESET}"
    
    gcc -o ${2}_me ${ME}${2}.c ../libft_malloc.so
    echo -e "${CYAN}Compile ${ME}${2}.c to  ${2}_me ${RESET}"
    ${TIME} -v ./${1}_real 2> .tmp/out_real${1}
    ${TIME} -v ./${2}_me 2> .tmp/out_me${2}
    echo -e "${RED}Diff ${RESET}"
    diff .tmp/out_real${1} .tmp/out_me${2} 2> .tmp/diff_out
    cat .tmp/diff_out
    echo -e "${GREEN}Page for real malloc ${RESET}"
    cat .tmp/out_real${1} | grep Minor
    echo -e "${CYAN}Page for my malloc ${RESET}"
    cat .tmp/out_me${2} | grep Minor
    rm -rf .tmp ${1}_real ${2}_me
else
    echo -e "${YELLOW}Invalid argument usage ./test.sh {real_malloc_test}, {my_malloc_test}"
    echo -e "${RED}File ${REAL}$1.c or ${ME}$2.c not found ${RESET}"
fi


# echo at begin hosttype = $HOSTTYPE
# if [ ! -z ${HOSTTYPE} ];  
# then
#     echo "Variable having name 'HOSTTYPE' is already set."
# else
#     echo "Variable having name 'HOSTTYPE' is not set."  
#     HOSTTYPE=$(uname -m)_$(uname -s)
# fi  
# function ft_test {
#     local NAME=libft_malloc_
#     NAME+=$HOSTTYPE
#     NAME+=.so
#     echo HOSTYPE: $HOSTTYPE
#     echo NAME: $NAME
# ft_test "${1}" "${2}"