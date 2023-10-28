#!/bin/bash

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

# }

# ft_test "${1}" "${2}"

make -C .. re

mkdir -p .tmp
gcc -o ${1}_real real/${1}.c
gcc -o ${2}_me me/${2}.c ../libft_malloc.so
/usr/bin/time -v ./${1}_real 2> .tmp/out_real${1}
/usr/bin/time -v ./${2}_me 2> .tmp/out_me${2}
diff .tmp/out_real${1} .tmp/out_me${2} 2> .tmp/diff_out
echo "diff :"
cat .tmp/diff_out
echo "page for real malloc :"
cat .tmp/out_real${1} | grep Minor
echo "page for my malloc :"
cat .tmp/out_me${2} | grep Minor
rm -rf .tmp ${1}_real ${2}_me
