//
//  util.c
//  mmap
//
//  Created by BlakeKing on 2023/4/11.
//

#include "util.h"
#include "config.h"

/**
 * Returns the byte order of the CPU.
 *
 * @return An integer representing the byte order of the CPU.
 */
int cpuByteorder(void) {
    static int BYTEORDER = BYTEORDER_NONE;
    if (BYTEORDER == BYTEORDER_NONE) {
        union {
            int i;
            char c;
        } t;
        t.i = 1;
        if (t.c == 1) {
            BYTEORDER = BYTEORDER_LOW;
        } else {
            BYTEORDER = BYTEORDER_HIGH;
        }
    }
    return BYTEORDER;
}

/**
 * Adjust byte order, the default byte order passed in is little-endian, if the system is big-endian, convert it to big-endian
 * The writing of C language byte order is little-endian, and the reading is also little-endian by default
 * The byte order of Java language is big-endian by default
 * @param data data
 */
void adjustByteorder(char data[4]) {
    if (cpuByteorder() == BYTEORDER_HIGH) {
        char *temp = data;
        char data_temp = *temp;
        *temp = *(temp + 3);
        *(temp + 3) = data_temp;
        data_temp = *(temp + 1);
        *(temp + 1) = *(temp + 2);
        *(temp + 2) = data_temp;
    }
}