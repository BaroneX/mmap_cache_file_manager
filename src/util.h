//
//  util.h
//  mmap
//
//  Created by BlakeKing on 2023/4/11.
//

#ifndef util_h
#define util_h

/**
 * Returns the byte order of the CPU.
 *
 * @return An integer representing the byte order of the CPU.
 */
int cpuByteorder(void);

/**
 * Adjusts the byte order of the given data to match the byte order of the CPU.
 *
 * @param data An array of 4 characters representing the data to adjust.
 */
void adjustByteorder(char data[4]);

#endif /* util_h */
