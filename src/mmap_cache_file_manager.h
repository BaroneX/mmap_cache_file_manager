/**
 * @file mmap_cache_file_manager.h
 * @brief Header file for mmap cache file manager.
 * 
 * This header file contains function declarations for managing a memory-mapped cache file.
 * The functions can be used to check if the cache file can be used, set the target file path,
 * write to the cache file, clear and reset the cache file header, update the content length,
 * get the total content length, flush the cache file to the target file, and force a flush to the target file.
 * 
 * @param mmapCacheFilePath The path to the memory-mapped cache file.
 * @param targetFilePath The path to the target file.
 * @param message The message to write to the cache file.
 * @param len The length of the message to write to the cache file.
 * @param mmapFilePtr A pointer to the memory-mapped cache file.
 * 
 * @return The return types vary depending on the function.
 */

#ifndef mmap_cache_file_manager_h
#define mmap_cache_file_manager_h


/**
 * Checks if the specified file path can be used for memory mapping cache file.
 *
 * @param mmapCacheFilePath The file path to check.
 * @return 1 if the file can be used for memory mapping cache file, 0 otherwise.
 */
int canUseMMapCacheFile(const char * mmapCacheFilePath);

/**
 * Set the target file path for the final write of the memory mapping cache file to it.
 *
 * @param targetFilePath The file path to set as the target.
 */
void setTargetFilePath(const char * targetFilePath);


/**
 * Writes the specified message to the memory mapping cache file.
 *
 * @param message The message to write to the cache file.
 */
void writeToMMAPCacheFile(char * message);

/**
 * Writes the specified message to the memory mapping cache file.
 *
 * @param message The message to write to the cache file.
 * @param len The length of the message to write to the cache file.
 */
void writeToMMAPCacheFileWithLength(char * message, int len);


/**
 * Clears the content length in the memory mapping cache file header.
 *
 * @param mmapFilePtr A pointer to the memory mapping cache file.
 */
void clearMMAPHeaderContentLength(void * mmapFilePtr);

/**
 * Resets the memory mapping cache file header.
 *
 * @param mmapFilePtr A pointer to the memory mapping cache file.
 */
void resetMMAPHeader(void * mmapFilePtr);

/**
 * Updates the content length in the memory mapping cache file header.
 *
 * @param mmapFilePtr A pointer to the memory mapping cache file.
 */
void updateMMapHeaderContentLength(void * mmapFilePtr);

/**
 * Gets the total content length in the memory mapping cache file.
 *
 * @param mmapFilePtr A pointer to the memory mapping cache file.
 * @return The total content length in the memory mapping cache file.
 */
int getContentTotalLength(void * mmapFilePtr);


/**
 * Flushes the memory mapping cache file to the target file.
 *
 * @param mmapFilePtr A pointer to the memory mapping cache file.
 * @param filePath The path to the target file.
 */
void flushToTargetFile(void * mmapFilePtr,const char * filePath);

/**
 * Gets the path to the target file.
 *
 * @param mmapFilePtr A pointer to the memory mapping cache file.
 * @return The path to the target file.
 */
char* getTargetFilePath(void * mmapFilePtr);


/**
 * Forces a flush of the memory mapping cache file to the target file.
 */
void forceFlushToFile();


/**
 * Flushes the memory mapping cache file.
 */
void flushMMapCacheFile();


#endif /* mmap_cache_file_manager_h */
