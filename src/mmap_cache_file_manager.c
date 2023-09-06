/**
- * @file mmap_cache_file_manager.c
- * @brief This file contains functions for managing a memory-mapped cache file.
- * 
- * The functions in this file allow for writing  messages to a memory-mapped cache file, which can then be flushed to a  file on disk when the cache reaches a certain size.
- * 
- * The cache file is managed using the mmap() function, which maps a portion of the file into memory for faster access.
- * 
- * The functions in this file include:
- * - canUseMMapCacheFile(): checks if the cache file can be used 
- * - setTargetFilePath(): set the  file path and flushes any existing  messages to the  file
- * - writeToMMAPCacheFile(): writes a  message to the cache file
- * - flushToTargetFile(): flushes the cache to the  file on disk
- * - updateMMapHeaderContentLength(): updates the cache size in the cache file header
- * - clearMMAPHeaderContentLength(): clears the cache size in the cache file header
- * - resetMMAPHeader(): resets the cache file header to all zeros
- * - getContentTotalLength(): gets the total length of the  file from the cache file header
- * - getTargetFilePath(): gets the  file path from the cache file header
- * - forceFlushToFile(): forces a flush of the cache to the  file on disk
- * - flushMMapCacheFile(): flushes the cache to the cache file on disk
- * 
- * @author BlakeKing
- * @date 2023/4/25
- */

#include "mmap_cache_file_manager.h"
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "mmap.h"
#include "config.h"
#include "util.h"

/**
 * @brief Static variables used for mmap cache file manager.
 * 
 * _mmapCacheFileBuffer: Cache buffer that is not released.
 * _fileTotalLength: Total length of the file.
 * _targetFilePath: Path of the target file.
 */
static unsigned char *_mmapCacheFileBuffer = NULL;
static int _fileTotalLength = 0;
static char *_targetFilePath = NULL;

// This section defines the header information for the cache file, which includes the byte length of the target file path, the byte length of the target file path itself, and the byte length of the content to be written to the file.
// The byte length of the target file path is represented by TARGET_FILE_BYTE_LENGTH, which is currently set to 2.
static int TARGET_FILE_BYTE_LENGTH = 2;
// The byte length of the target file path itself is represented by TARGET_FILE_PATH_BYTE_LENGTH, which is currently set to 1024.
static int TARGET_FILE_PATH_BYTE_LENGTH = 1024;
// The byte length of the content to be written to the file is represented by CONTENT_BYTE_LENGTH, which is currently set to 4.
static int CONTENT_BYTE_LENGTH = 4;


/**
 * Checks if the specified file can be memory-mapped for caching.
 * 
 * @param mmapCacheFilePath The path of the file to be checked.
 * @return Returns 1 if the file can be memory-mapped, 0 otherwise.
 */
int canUseMMapCacheFile(const char * mmapCacheFilePath){
    int canMmap = openMMapCacheFile(mmapCacheFilePath, &_mmapCacheFileBuffer);
    return canMmap;
}

/**
 * @brief Sets the target file path for the final write of the memory mapping cache file to it.
 * 
 * @param filePath The file path to set as the target.
 */
void setTargetFilePath(const char * filePath){
    void * mmapFilePtr =_mmapCacheFileBuffer;

    // Free the previously set target file path if it exists.
    if (_targetFilePath != NULL) {
        free(_targetFilePath);
        _targetFilePath = NULL;
    }

    // Allocate memory for the new target file path and copy the input file path to it.
    _targetFilePath = (char*)malloc(strlen(filePath));
    memset(_targetFilePath, 0, strlen(filePath));
    strcpy(_targetFilePath, filePath);
    
    // Get the total length of the content in the memory mapping cache file.
    _fileTotalLength = getContentTotalLength(mmapFilePtr);
    debugPrint("mmap:_fileTotalLength:%d\n", _fileTotalLength);

    // If the content is not empty, flush it to the target file.
    if(_fileTotalLength > 0){
        char* filePath = getTargetFilePath(mmapFilePtr);
        debugPrint("mmap:filePath:%s\n", filePath);
        flushToTargetFile(mmapFilePtr,filePath);
        resetMMAPHeader(mmapFilePtr);
        free(filePath);
        filePath = NULL;
    }

    debugPrint("mmap:start write filepath \n");

    // Write the length of the file path to the memory mapping cache file.
    int filePathStringLength = (int)strlen(filePath);
    unsigned char * dataPtr = mmapFilePtr;
    *dataPtr = filePathStringLength;
    dataPtr++;
    *dataPtr = filePathStringLength>>8;
    dataPtr++;
    
    // Write the file path to the memory mapping cache file.
    memcpy(dataPtr, filePath, filePathStringLength+1);
}


/**
 * Writes a message to the memory mapping cache file.
 * 
 * @param message The message to be written.
 */
void writeToMMAPCacheFile(char * message){
    debugPrint("mmap:writeToMMAPCacheFile\n");
    int appendSize = (int)strlen(message);

    // Divide the message into sections of SECTION_LENGTH and write each section to the memory mapping cache file.
    int size = SECTION_LENGTH;
    int times = appendSize / size;
    int remainLen = appendSize % size;
    char *temp = message;
    int i = 0;
    for (i = 0; i < times; i++) {
        writeToMMAPCacheFileWithLength(temp, size);
        temp += size;
    }
    // Write the remaining part of the message to the memory mapping cache file.
    if (remainLen) {
        writeToMMAPCacheFileWithLength(temp, remainLen);
    }
}

// This function writes a message to the memory mapping cache file with a specified length.
// It takes in a message and its length as input parameters.
void writeToMMAPCacheFileWithLength(char * message, int len){
    void * dataPtr;//mmap point
     //4个字节长度
    // Set the data pointer to the location in memory where the message should be written.
    dataPtr =_mmapCacheFileBuffer+TARGET_FILE_BYTE_LENGTH+TARGET_FILE_PATH_BYTE_LENGTH+CONTENT_BYTE_LENGTH+_fileTotalLength;
    // Copy the message to the memory location pointed to by the data pointer.
    memcpy(dataPtr, message, len);
    // Update the total length of the content in the memory mapping cache file.
    _fileTotalLength += len;
    // Update the length of the content in the memory mapping cache file in the header.
    updateMMapHeaderContentLength(_mmapCacheFileBuffer);
    // If the total length of the content in the memory mapping cache file exceeds the cache length,
    // flush the content to the target file.
    if (_fileTotalLength > (CACHE_LENGTH)){
        flushToTargetFile(_mmapCacheFileBuffer,_targetFilePath);
    }
}

// This function flushes the content in the memory mapping cache file to the target file.
// It takes in a pointer to the memory mapping cache file and the file path of the target file as input parameters.
void flushToTargetFile(void * mmapFilePtr,const char * filePath) {
    // Open the target file in append mode.
    FILE* fp = fopen(filePath, "at+");
    debugPrint("mmap:flushToFile:%s\n",filePath);
    // If the file is opened successfully, write the content in the memory mapping cache file to the target file.
    if(fp != NULL) {
        debugPrint("mmap:fwrite start\n");
        // Write the content in the memory mapping cache file to the target file.
        fwrite(mmapFilePtr+TARGET_FILE_BYTE_LENGTH+TARGET_FILE_PATH_BYTE_LENGTH+CONTENT_BYTE_LENGTH, sizeof(char), _fileTotalLength, fp);
        // Flush the output buffer of the target file.
        fflush(fp);
        // Close the target file.
        fclose(fp);
        debugPrint("mmap:fwrite end\n");
    }
    // Clear the content length in the memory mapping cache file header.
    debugPrint("mmap:clear  length\n");
    clearMMAPHeaderContentLength(mmapFilePtr);
}

// This function updates the content length in the memory mapping cache file header.
// It takes in a pointer to the memory mapping cache file as an input parameter.
// The content length is written in little-endian byte order.
void updateMMapHeaderContentLength(void * mmapFilePtr) {
    // Set the data pointer to the location in memory where the content length should be written.
    unsigned char * dataPtr = mmapFilePtr;
    dataPtr += (TARGET_FILE_BYTE_LENGTH+TARGET_FILE_PATH_BYTE_LENGTH);
    // Write the content length to the memory location pointed to by the data pointer in little-endian byte order.
    *dataPtr = _fileTotalLength;
    dataPtr++;
    *dataPtr = _fileTotalLength>>8;
    dataPtr++;
    *dataPtr = _fileTotalLength>>16;
    dataPtr++;
    *dataPtr = _fileTotalLength>>24;
}

// This function clears the content length in the memory mapping cache file header.
// It takes in a pointer to the memory mapping cache file as an input parameter.
void clearMMAPHeaderContentLength(void * mmapFilePtr){
    // Read the total length of the content in the memory mapping cache file.
    int totalLength = getContentTotalLength(mmapFilePtr);
    // Set the content length in the memory mapping cache file header to 0.
    memset(mmapFilePtr+TARGET_FILE_BYTE_LENGTH+TARGET_FILE_PATH_BYTE_LENGTH, 0, totalLength+CONTENT_BYTE_LENGTH);
    // Reset the total length of the content in the memory mapping cache file to 0.
    _fileTotalLength = 0;
    // Update the content length in the memory mapping cache file header to 0.
    updateMMapHeaderContentLength(mmapFilePtr);
}

/**
 * @brief Resets the memory mapping cache file header and content length to 0.
 * 
 * @param mmapFilePtr Pointer to the memory mapping cache file.
 */
void resetMMAPHeader(void * mmapFilePtr){
    // Read the total length of the content in the memory mapping cache file.
    int totalLength = getContentTotalLength(mmapFilePtr);
    // Set all values in the memory mapping cache file header to 0.
    memset(mmapFilePtr, 0, totalLength+TARGET_FILE_BYTE_LENGTH+TARGET_FILE_PATH_BYTE_LENGTH+CONTENT_BYTE_LENGTH);
    // Reset the total length of the content in the memory mapping cache file to 0.
    _fileTotalLength = 0;
    // Update the content length in the memory mapping cache file header to 0.
    updateMMapHeaderContentLength(mmapFilePtr);
}

// This function gets the total length of the content saved in the cache file.
// It takes in a pointer to the memory mapping cache file as an input parameter.
// The content length is read in little-endian byte order.
int getContentTotalLength(void * mmapFilePtr){
    // Set the data pointer to the location in memory where the content length should be read.
    unsigned char *dataPtr = mmapFilePtr;
    dataPtr +=(TARGET_FILE_BYTE_LENGTH+TARGET_FILE_PATH_BYTE_LENGTH);
    // Read the content length from the memory location pointed to by the data pointer in little-endian byte order.
    char lenArray[] = {'\0', '\0', '\0', '\0'};
        lenArray[0] = *dataPtr;
        dataPtr++;
        lenArray[1] = *dataPtr;
        dataPtr++;
        lenArray[2] = *dataPtr;
        dataPtr++;
        lenArray[3] = *dataPtr;

    adjustByteorder(lenArray);

    // Convert the content length to an integer and return it.
    int *contentTotalLength = (int *) lenArray;
    return *contentTotalLength;
}

// This function gets the target file path saved in the memory mapping cache file header.
// It takes in a pointer to the memory mapping cache file as an input parameter.
// The target file path is read in little-endian byte order.
char* getTargetFilePath(void * mmapFilePtr){
    // Set the data pointer to the location in memory where the target file path should be read.
    unsigned char *tempMmapFilePtr = mmapFilePtr;
    char lenArray[] = {'\0', '\0','\0','\0'};
        lenArray[0] = *tempMmapFilePtr;
        tempMmapFilePtr++;
        lenArray[1] = *tempMmapFilePtr;
        tempMmapFilePtr++;
    
    // Adjust the byte order of the target file path to little-endian.
    adjustByteorder(lenArray);
    
    // Get the total length of the target file path.
    int *totalLen = (int *) lenArray;
    int len = *totalLen;
    
    // Allocate memory for the target file path and copy it from the memory mapping cache file.
    char* dst = (char*)malloc(len);
    memcpy(dst, tempMmapFilePtr, len+1);
    
    // Print the target file path for debugging purposes.
    debugPrint("mmap:dst:%s\n", dst);    

    // Return the target file path.
    return dst;
}

/**
 * @brief Forces the mmap file to be written to the target file.
 * 
 */
void forceFlushToFile(){
    flushToTargetFile(_mmapCacheFileBuffer,_targetFilePath);
}

/**
 * Flushes the memory mapping cache file.
 */
void flushMMapCacheFile(){
    msync(_mmapCacheFileBuffer, _fileTotalLength, MS_ASYNC);
}
