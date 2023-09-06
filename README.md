# mmap_cache_file_manager

mmap_cache_file_manager is a cache file manager implemented using mmap that can help reduce IO write frequency and improve file write performance.

In traditional file write methods, IO operations are required every time a file is written, which can lead to frequent disk access and reduced file write performance. Using mmap can map the file to memory, avoiding frequent IO operations and improving file write performance.

mmap_cache_file_manager creates a memory mapping cache file by mapping the file to memory, and writes the content to be written to the file to the cache file. When the cache file reaches the write threshold, the content of the cache file is automatically written to the target file. This avoids frequent IO operations and improves file write performance.

mmap_cache_file_manager also provides a method to manually flush the cache file, allowing you to manually write the content of the cache file to the target file at the desired time to ensure data integrity and consistency.

In summary, mmap_cache_file_manager is a high-performance cache file manager that can help improve file write performance, reduce IO operations, and improve application performance and response speed.

## Getting Started

First you need to check if you can use mmap cache file.

```
  String rootPath =  await FileUtil.getDefaultLogRootDirectoryPath();
  String mmapCacheFilePath = "$rootPath/cache.mmap";
  String targetFile = "$rootPath/target.txt";
  bool _canUseMMapCahce = MmapCacheFileManager.canUseMMAPCacheFile(mmapCacheFilePath);
  if (_canUseMMapCahce) {
    MmapCacheFileManager.setTargetFilePath(targetFile);
  }
```

if you can use mmap cache file ,you can add message to cache file

```
  if (_canUseMMapCahce) {
    MmapCacheFileManager.writeToMMAPCacheFileAsync(message);
  }
```

If the total length of the content in the memory mapping cache file exceeds the cache length, it will auto flush the content to the target file. You can also manually flush the cache file to the target file at any time.

```
  MmapCacheFileManager.forceFlushToFileAsync();
```
