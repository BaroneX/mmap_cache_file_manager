import 'dart:async';
import 'dart:io';

import 'package:path_provider/path_provider.dart';

class FileUtil {
  static String? _rootDirectoryPath;
  static Future<String> getDefaultLogRootDirectoryPath() async {
    if (_rootDirectoryPath != null) {
      return _rootDirectoryPath!;
    }
    Directory? rootDirectory;
    if (Platform.isAndroid) {
      List<Directory>? documentsDir = await getExternalStorageDirectories();
      if (documentsDir != null && documentsDir.isNotEmpty) {
        rootDirectory = documentsDir[0];
      }
    } else {
      rootDirectory = await getApplicationDocumentsDirectory();
    }
    if (rootDirectory == null) {
      throw Exception("Log directory is null");
    }
    _rootDirectoryPath = rootDirectory.path;
    return _rootDirectoryPath!;
  }
}