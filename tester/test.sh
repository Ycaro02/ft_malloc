#!/bin/bash

mkdir -p .tmp

gcc -o ${1} real/${1}.c
gcc -o ${2} me/${2}.c
/usr/bin/time -v ./${1} 2> .tmp/out_${1}
/usr/bin/time -v ./${2} 2> .tmp/out_${2}
diff .tmp/out_${1} .tmp/out_${2} 2> .tmp/diff_out
cat .tmp/diff_out
rm -rf .tmp ${1} ${2}

