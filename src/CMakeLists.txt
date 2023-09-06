cmake_minimum_required(VERSION 3.4.1)  # for example

project(mmap_cache_file_manager VERSION 1.0.3 LANGUAGES C)

add_library( mmap_cache_file_manager
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             "mmap_cache_file_manager.c"
             "mmap.c"
             "util.c" )

target_compile_definitions(mmap_cache_file_manager PUBLIC DART_SHARED_LIB)