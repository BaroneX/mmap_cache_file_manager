// ignore_for_file: constant_identifier_names

import 'dart:async';
import 'dart:ffi';
import 'dart:io';
import 'dart:isolate';

import 'package:ffi/ffi.dart';
import 'package:mmap_cache_file_manager/mmap_cache_file_manager_bindings_generated.dart';

const String _libName = 'mmap_cache_file_manager';

/// This private final variable `_dylib` is a `DynamicLibrary` object that loads the shared library of the mmap_cache_file_manager package.
/// The shared library loaded depends on the current platform the code is running on.
/// If the platform is macOS or iOS, it loads the shared library with the name `$_libName.framework/$_libName`.
/// If the platform is Android or Linux, it loads the shared library with the name `lib$_libName.so`.
/// If the platform is Windows, it loads the shared library with the name `$_libName.dll`.
/// If the platform is not recognized, an `UnsupportedError` is thrown with the message "Unknown platform: ${Platform.operatingSystem}".
final DynamicLibrary _dylib = () {
  if (Platform.isMacOS || Platform.isIOS) {
    return DynamicLibrary.open('$_libName.framework/$_libName');
  }
  if (Platform.isAndroid || Platform.isLinux) {
    return DynamicLibrary.open('lib$_libName.so');
  }
  if (Platform.isWindows) {
    return DynamicLibrary.open('$_libName.dll');
  }
  throw UnsupportedError('Unknown platform: ${Platform.operatingSystem}');
}();

/// A private class representing a message request with an ID and a message.
class _MessageRequest {
  final int id;
  final String message;

  const _MessageRequest(this.id, this.message);
}

/// A private class representing a message response with an ID.
class _MessageResponse {
  /// The ID of the message response.
  final int id;

  /// Creates a new instance of [_MessageResponse] with the given [id].
  const _MessageResponse(this.id);
}

/// A private class representing a flush request.
class _FlushRequest {
  /// The ID of the flush request.
  final int id;

  const _FlushRequest(this.id);
}

/// A private class representing a flush response.
class _FlushRespose {
  /// The ID of the flush response.
  final int id;

  const _FlushRespose(this.id);
}

/// The bindings to the native functions in [_dylib].

/// A class that manages a memory-mapped cache file.
class MmapCacheFileManager {
  static int _nextSumRequestId = 0;

  static SendPort? isolateSendPort;

  static final Map<int, Completer<int>> _requests = <int, Completer<int>>{};

  static final Future<SendPort> _isolateSendPort = () async {
    final Completer<SendPort> completer = Completer<SendPort>();

    final ReceivePort receivePort = ReceivePort()
      ..listen((dynamic data) {
        if (data is SendPort) {
          completer.complete(data);
          return;
        }
        if (data is _MessageResponse) {
          final Completer<int> completer = _requests[data.id]!;
          _requests.remove(data.id);
          completer.complete(data.id);
          return;
        }else if(data is _FlushRespose){
           final Completer<int> completer = _requests[data.id]!;
          _requests.remove(data.id);
          completer.complete(data.id);
          return;
        }
        throw UnsupportedError('Unsupported message type: ${data.runtimeType}');
      });

    await Isolate.spawn((SendPort sendPort) async {
      final ReceivePort helperReceivePort = ReceivePort()
        ..listen((dynamic data) {
          if (data is _MessageRequest) {
            writeToMMAPCacheFile(data.message);
            final _MessageResponse response = _MessageResponse(data.id);
            sendPort.send(response);
            return;
          }else if(data is _FlushRequest){
            forceFlushToFile();
            final _FlushRespose response = _FlushRespose(data.id);
            sendPort.send(response);
            return;
          }
          throw UnsupportedError(
              'Unsupported message type: ${data.runtimeType}');
        });

      sendPort.send(helperReceivePort.sendPort);
    }, receivePort.sendPort);

    return completer.future;
  }();

  static final MmapCacheFileManagerBindings _bindings =
      MmapCacheFileManagerBindings(_dylib);

  /// Checks if MMAP cache file can be used for the given [pathName].
  /// 
  /// Returns `true` if MMAP cache file can be used, `false` otherwise.
  static bool canUseMMAPCacheFile(String pathName) {
    //convert to Pointer<Char>
    final inPathName = pathName.toNativeUtf8().cast<Char>();
    return _bindings.canUseMMapCacheFile(inPathName) == 1;
  }

  /// Sets the target file path for the cache file manager.
  /// 
  /// The [pathName] parameter is the path to the target file.
  /// 
  /// This function converts the [pathName] to a Pointer<Char> and passes it to the native function setTargetFilePath.
  static setTargetFilePath(String pathName) {
    //convert to Pointer<Char>
    final inPathName = pathName.toNativeUtf8().cast<Char>();
    _bindings.setTargetFilePath(inPathName);
  }

  /// Writes the given [message] to the MMAP cache file.
  ///
  /// The [message] parameter is the string message to be written to the MMAP cache file.
  /// This method converts the [message] to a native UTF-8 string and appends it to the cache file.
  static writeToMMAPCacheFile(String message) {
    final appendStr = message.toNativeUtf8().cast<Char>();
    _bindings.writeToMMAPCacheFile(appendStr);
  }

  /// Writes the given [message] to the MMAP cache file asynchronously.
  ///
  /// Returns a [Future] that completes with an [int] value when the write operation is complete.
  /// The returned [int] value is the ID of the request.
  ///
  /// Throws an error if the [isolateSendPort] is null.
  static writeToMMAPCacheFileAsync(String message) async {
    isolateSendPort ??= await _isolateSendPort;
    final int requestId = _nextSumRequestId++;
    final _MessageRequest request = _MessageRequest(requestId, message);
    final Completer<int> completer = Completer<int>();
    _requests[requestId] = completer;
    isolateSendPort?.send(request);
    return completer.future;
  }

  /// Forces the cache file manager to flush its contents to the file system.
  /// This is a synchronous operation and may block the calling thread.
  static forceFlushToFile() {
    _bindings.forceFlushToFile();
  }

  /// Forces the cache file to be flushed to disk asynchronously.
  /// This method calls the underlying C++ function `forceFlushToFile()`.
  static forceFlushToFileAsync() async{
    isolateSendPort ??= await _isolateSendPort;
    final int requestId = _nextSumRequestId++;
    final _FlushRequest request = _FlushRequest(requestId);
    final Completer<int> completer = Completer<int>();
    _requests[requestId] = completer;
    isolateSendPort?.send(request);
    return completer.future;    
  }
}
