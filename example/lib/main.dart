import 'package:flutter/material.dart';
import 'package:mmap_cache_file_example/file_util.dart';

import 'package:mmap_cache_file_manager/mmap_cache_file_manager.dart';


void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  bool _canUseMMapCahce = false;

  @override
  void initState() {
    super.initState();
    initMmapCacheFileManager();
  }

  initMmapCacheFileManager() async {
    String rootPath =  await FileUtil.getDefaultLogRootDirectoryPath();
    String mmapCacheFilePath = "$rootPath/cache.mmap";
    String targetFile = "$rootPath/target.txt";
    _canUseMMapCahce = MmapCacheFileManager.canUseMMAPCacheFile(mmapCacheFilePath);
    if (_canUseMMapCahce) {
      MmapCacheFileManager.setTargetFilePath(targetFile);
    }
  }

  @override
  Widget build(BuildContext context) {
    const textStyle = TextStyle(fontSize: 25);
    const spacerSmall = SizedBox(height: 10);
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Native Packages'),
        ),
        body: SingleChildScrollView(
          child: Container(
            padding: const EdgeInsets.all(10),
            child: Column(
              children: [
                GestureDetector(
                  onTap: (){
                    if (_canUseMMapCahce) {
                      writeMessageToMmapCacheFile(DateTime.now().toString());
                    }
                  },
                  child: const Text(
                    'write message to mmap cache file',
                    style: textStyle,
                    textAlign: TextAlign.center,
                  ),
                ),
                spacerSmall,
                GestureDetector(
                  onTap: (){
                    forceFlushToFile();
                  },
                  child: const Text(
                    'flush to target file',
                    style: textStyle,
                    textAlign: TextAlign.center,
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }

  writeMessageToMmapCacheFile(String message) async {
    MmapCacheFileManager.writeToMMAPCacheFileAsync(message);
  }

  forceFlushToFile() async{
    MmapCacheFileManager.forceFlushToFileAsync();
  }
}
