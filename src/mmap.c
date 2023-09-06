//
//  MMapUtil.c
//  mmap
//
//  Created by BlakeKing on 2023/4/11.
//

#include "mmap.h"
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "config.h"

/**
 * @brief Open a memory-mapped file and return a pointer to the mapped memory.
 * 
 * @param filePath The path of the file to be memory-mapped.
 * @param buffer A pointer to the buffer that will hold the mapped memory.
 * @return int Returns OPEN_MMAP_SUCCESS if the memory-mapped file is opened successfully, otherwise returns OPEN_MMAP_FAIL.
 */
int openMMapCacheFile(const char *filePath, unsigned char **buffer)
{
    int operationCode = OPEN_MMAP_FAIL;
    if (NULL == filePath || 0 == strnlen(filePath, 128))
    {
        operationCode = OPEN_MMAP_FAIL;
    }
    else
    {
        unsigned char *pMap = NULL;
        int size = MMAP_LENGTH;
        int fd = open(filePath, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP); 
        int isNeedCheck = 0;                                                             
        if (fd != -1)
        {   
            // Protection
            int isFileOk = 0;
            // Check if the file has a value before memory mapping
            FILE *file = fopen(filePath, "rb+"); 
            if (NULL != file)
            {
                fseek(file, 0, SEEK_END);
                long longBytes = ftell(file);
                if (longBytes < MMAP_LENGTH)
                {
                    fseek(file, 0, SEEK_SET);
                    char *zeroData = malloc(size);
                    memset(zeroData, 0, size);
                    size_t _size = 0;
                    _size = fwrite(zeroData, sizeof(char), size, file);
                    fflush(file);
                    if (_size == size)
                    {
                        debugPrint("copy data 2 mmap file success\n");
                        isFileOk = 1;
                        isNeedCheck = 1;
                    }
                    else
                    {
                        isFileOk = 0;
                    }
                    free(zeroData); 
                }
                else
                {
                    isFileOk = 1;
                }
                fclose(file);
            }
            else
            {
                isFileOk = 0;
            }

            if (isNeedCheck)
            { 
                // Strengthen protection by ensuring that the mapped file has an appropriate length
                FILE *file = fopen(filePath, "rb");
                if (file != NULL)
                {
                    fseek(file, 0, SEEK_END);
                    long longBytes = ftell(file);
                    if (longBytes >= MMAP_LENGTH)
                    {
                        isFileOk = 1;
                    }
                    else
                    {
                        isFileOk = 0;
                    }
                    fclose(file);
                }
                else
                {
                    isFileOk = 0;
                }
            }

            if (isFileOk)
            {
                pMap = (unsigned char *)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            }
            if (pMap != MAP_FAILED && NULL != pMap && isFileOk)
            {
                operationCode = OPEN_MMAP_SUCCESS;
            }
            else
            {
                operationCode = OPEN_MMAP_FAIL;
                debugPrint("open mmap fail , reason : %s \n", strerror(errno));
            }
            close(fd);

            if (operationCode == OPEN_MMAP_SUCCESS &&
                access(filePath, F_OK) != -1)
            { 
                // Before returning mmap, make a final check to ensure that there is an mmap file to use.
                operationCode = OPEN_MMAP_SUCCESS;
                *buffer = pMap;
            }
            else
            {
                operationCode = OPEN_MMAP_FAIL;
                if (NULL != pMap)
                    munmap(pMap, size);
            }
        }
        else
        {
            debugPrint("open(%s) fail: %s\n", filePath, strerror(errno));
        }
    }

    return operationCode;
}
