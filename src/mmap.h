//
//  mmap.h
//  mmap
//
//  Created by BlakeKing on 2023/4/11.
//

#ifndef mmap_h
#define mmap_h


/**
 * Opens a memory-mapped cache file at the specified file path and returns a pointer to the buffer.
 * 
 * @param filePath The path to the cache file.
 * @param buffer A pointer to the buffer that will hold the contents of the cache file.
 * @return An integer representing the file descriptor of the opened cache file. Returns -1 if the file could not be opened.
 */
int openMMapCacheFile(const char * filePath, unsigned char **buffer);

#endif /* mmap_h */
