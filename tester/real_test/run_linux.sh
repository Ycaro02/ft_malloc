#!/bin/sh
export LD_LIBRARY_PATH=. && export LD_PRELOAD=libft_malloc.so && export DYLD_FORCE_FLAT_NAMESPACE=1 && $@

# https://theevilbit.github.io/posts/dyld_insert_libraries_dylib_injection_in_macos_osx_deep_dive/
