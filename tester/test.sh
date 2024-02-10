#!/bin/bash

# Color definition
RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
CYAN="\e[36m"
RESET="\e[0m"



# Set the appropriate value to the time command
TIME="/usr/bin/time"

if [ ! -f /usr/bin/time ]
then
TIME="/bin/time"
fi

REAL="./real/"
ME="./me/"

LIBFT_TEST="libft_malloc_test"

REAL_TEST="malloc_test"

NULL="/dev/null"

# check given source file
# if [ -f ${REAL}${1}.c ] && [ -f ${ME}${2}.c ];
# then
    make -s -C .. > ${NULL}
    mkdir -p .tmp

    echo -e "${GREEN}${REAL_TEST} ${1} ${RESET}"
    "${TIME}" -v ./${REAL_TEST} ${1} 2> .tmp/out_real${1}
    
    echo -e "${CYAN}${LIBFT_TEST} ${2} ${RESET}"
    "${TIME}" -v ./${LIBFT_TEST} ${2} 2> .tmp/out_libft${2}


    # echo -e "${GREEN}Compile ${REAL}${1}.c to ${1}_real ${RESET}"
    # gcc -o ${1}_real ${REAL}${1}.c
    
    # echo -e "${CYAN}Compile ${ME}${2}.c   to ${2}_libft ${RESET}"
    # echo -e "${CYAN}Compile ${ME}${2}.c   to ${2}_libft ${RESET}"
    # gcc -o ${2}_libft ${ME}${2}.c ../libft_malloc.so

    # "${TIME}" -v ./${1}_real 2> .tmp/out_real${1}
    # "${TIME}" -v ./${2}_libft 2> .tmp/out_libft${2}
    
    echo -e "${RED}Diff ${RESET}"
    diff .tmp/out_real${1} .tmp/out_libft${2} 2> .tmp/diff_out
    cat .tmp/diff_out

    echo -e "${GREEN}Page for real malloc  :${RESET}"
    cat .tmp/out_real${1} | grep Minor
    echo -e "${CYAN}Page for libft malloc :${RESET}"
    cat .tmp/out_libft${2} | grep Minor
    
    rm -rf .tmp
    # rm -rf .tmp ${1}_real ${2}_libft
# else
#     echo -e "${YELLOW}Invalid argument usage ./test.sh {real_malloc_test}, {libft_malloc_test}${RESET}"
#     echo -e "${RED}File $1 function or $2 function not found ${RESET}"
# fi


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