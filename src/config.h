//
//  config.h
//  mmap
//
//  Created by BlakeKing on 2023/4/11.
//
#include <stdio.h>

#ifndef CONFIG_H
#define CONFIG_H

#define OPEN_MMAP_SUCCESS 1
#define OPEN_MMAP_FAIL 0

#define MMAP_LENGTH  600 * 1024 //600k
#define CACHE_LENGTH  400 * 1024 //400k
#define SECTION_LENGTH  80 * 1024 //80k

#define BYTEORDER_NONE  0
#define BYTEORDER_HIGH 1
#define BYTEORDER_LOW 2


#ifdef DEBUG
#define debugPrint(...) printf(__VA_ARGS__)
#else
#define debugPrint(...)
#endif


#endif //CONFIG_H
