#!/bin/bash

if [ ! -f ../libft_malloc.so ]
then 
    make -s -C ..
fi

if [ ! -f ../libft/libft.a ]
then 
    make -s -C ../libft
fi