#!/bin/bash

# Color definition
RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
PURPLE="\e[35m"
CYAN="\e[36m"
RESET="\e[0m"
# linux run script
LINUX_RUN=./real_test/run_linux.sh
# test source code
TEST0_SRC=real_test/test0.c
TEST1_SRC=real_test/test1.c
TEST2_SRC=real_test/test2.c
TEST3_SRC=real_test/test3.c
TEST4_SRC=real_test/test4.c
TEST5_SRC=real_test/test5.c

compite_test_with_lib() {
    FILE_SRC=${1}
    FILE_EXEC=${2}
    echo -e "${CYAN}Compile ${FILE_SRC} ${RESET}"
    gcc -o ${FILE_EXEC} ${FILE_SRC} -L. -lft_malloc
}

compile_real_test_get_minor() {
    FILE_SRC=${1}
    FILE_EXEC=${2}
    echo -e "${YELLOW}Compile and test ${FILE_SRC} real malloc ${RESET}"
    gcc -o ${FILE_EXEC} ${FILE_SRC} && /usr/bin/time -v ./${FILE_EXEC} 2> .tmp/out_${FILE_EXEC}
    MALLOC_PAGE=$(cat .tmp/out_${FILE_EXEC} | grep Minor | rev | cut -d ' ' -f 1  | rev)
    echo -e "${GREEN}${MALLOC_PAGE}${RESET}"
}

test_libft_malloc_get_minor() {
    echo -e "${PURPLE}Test ${FILE_SRC} libft malloc ${RESET}"
    ${LINUX_RUN} /usr/bin/time -v ./${1} 2> .tmp/out_${1}
    MALLOC_PAGE=$(cat .tmp/out_${1} | grep Minor | rev | cut -d ' ' -f 1  | rev)
    echo -e "${RED}${MALLOC_PAGE}${RESET}"
}

test_libft_malloc() {
    echo -e "${CYAN}Test ${FILE_SRC} libft malloc ${RESET}"
    ${LINUX_RUN} ./${1}
}
# TEST
mkdir -p .tmp
cp ../libft_malloc.so .

compile_real_test_get_minor ${TEST0_SRC} test0
compile_real_test_get_minor ${TEST1_SRC} test1

test_libft_malloc_get_minor test0
test_libft_malloc_get_minor test1

compite_test_with_lib ${TEST2_SRC} test2
test_libft_malloc_get_minor test2

compite_test_with_lib ${TEST3_SRC} test3
test_libft_malloc test3

# dev null cause implicit declaration of function ‘show_alloc_mem’ 
compite_test_with_lib ${TEST4_SRC} test4 2> /dev/null
test_libft_malloc test4

compite_test_with_lib ${TEST5_SRC} test5
test_libft_malloc test5

rm -rf .tmp test0 test1 test2 test3 test4 test5
