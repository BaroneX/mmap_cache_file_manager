// ignore_for_file: always_specify_types
// ignore_for_file: camel_case_types
// ignore_for_file: non_constant_identifier_names

// AUTO GENERATED FILE, DO NOT EDIT.
//
// Generated by `package:ffigen`.
import 'dart:ffi' as ffi;

/// Bindings for `src/mmap_cache_file_manager.h`.
///
/// Regenerate bindings with `flutter pub run ffigen --config ffigen.yaml`.
///
class MmapCacheFileManagerBindings {
  /// Holds the symbol lookup function.
  final ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
      _lookup;

  /// The symbols are looked up in [dynamicLibrary].
  MmapCacheFileManagerBindings(ffi.DynamicLibrary dynamicLibrary)
      : _lookup = dynamicLibrary.lookup;

  /// The symbols are looked up with [lookup].
  MmapCacheFileManagerBindings.fromLookup(
      ffi.Pointer<T> Function<T extends ffi.NativeType>(String symbolName)
          lookup)
      : _lookup = lookup;

  int canUseMMapCacheFile(
    ffi.Pointer<ffi.Char> mmapCacheFilePath,
  ) {
    return _canUseMMapCacheFile(
      mmapCacheFilePath,
    );
  }

  late final _canUseMMapCacheFilePtr =
      _lookup<ffi.NativeFunction<ffi.Int Function(ffi.Pointer<ffi.Char>)>>(
          'canUseMMapCacheFile');
  late final _canUseMMapCacheFile =
      _canUseMMapCacheFilePtr.asFunction<int Function(ffi.Pointer<ffi.Char>)>();

  void setTargetFilePath(
    ffi.Pointer<ffi.Char> targetFilePath,
  ) {
    return _setTargetFilePath(
      targetFilePath,
    );
  }

  late final _setTargetFilePathPtr =
      _lookup<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char>)>>(
          'setTargetFilePath');
  late final _setTargetFilePath =
      _setTargetFilePathPtr.asFunction<void Function(ffi.Pointer<ffi.Char>)>();

  void writeToMMAPCacheFile(
    ffi.Pointer<ffi.Char> message,
  ) {
    return _writeToMMAPCacheFile(
      message,
    );
  }

  late final _writeToMMAPCacheFilePtr =
      _lookup<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Char>)>>(
          'writeToMMAPCacheFile');
  late final _writeToMMAPCacheFile = _writeToMMAPCacheFilePtr
      .asFunction<void Function(ffi.Pointer<ffi.Char>)>();

  void write2ToMMAPCacheFile(
    ffi.Pointer<ffi.Char> message,
    int len,
  ) {
    return _write2ToMMAPCacheFile(
      message,
      len,
    );
  }

  late final _write2ToMMAPCacheFilePtr = _lookup<
      ffi.NativeFunction<
          ffi.Void Function(
              ffi.Pointer<ffi.Char>, ffi.Int)>>('write2ToMMAPCacheFile');
  late final _write2ToMMAPCacheFile = _write2ToMMAPCacheFilePtr
      .asFunction<void Function(ffi.Pointer<ffi.Char>, int)>();

  void clearMMAPHeaderContentLength(
    ffi.Pointer<ffi.Void> mmapFilePtr,
  ) {
    return _clearMMAPHeaderContentLength(
      mmapFilePtr,
    );
  }

  late final _clearMMAPHeaderContentLengthPtr =
      _lookup<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Void>)>>(
          'clearMMAPHeaderContentLength');
  late final _clearMMAPHeaderContentLength = _clearMMAPHeaderContentLengthPtr
      .asFunction<void Function(ffi.Pointer<ffi.Void>)>();

  void resetMMAPHeader(
    ffi.Pointer<ffi.Void> mmapFilePtr,
  ) {
    return _resetMMAPHeader(
      mmapFilePtr,
    );
  }

  late final _resetMMAPHeaderPtr =
      _lookup<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Void>)>>(
          'resetMMAPHeader');
  late final _resetMMAPHeader =
      _resetMMAPHeaderPtr.asFunction<void Function(ffi.Pointer<ffi.Void>)>();

  void updateMMapHeaderContentLength(
    ffi.Pointer<ffi.Void> mmapFilePtr,
  ) {
    return _updateMMapHeaderContentLength(
      mmapFilePtr,
    );
  }

  late final _updateMMapHeaderContentLengthPtr =
      _lookup<ffi.NativeFunction<ffi.Void Function(ffi.Pointer<ffi.Void>)>>(
          'updateMMapHeaderContentLength');
  late final _updateMMapHeaderContentLength = _updateMMapHeaderContentLengthPtr
      .asFunction<void Function(ffi.Pointer<ffi.Void>)>();

  int getContentTotalLength(
    ffi.Pointer<ffi.Void> mmapFilePtr,
  ) {
    return _getContentTotalLength(
      mmapFilePtr,
    );
  }

  late final _getContentTotalLengthPtr =
      _lookup<ffi.NativeFunction<ffi.Int Function(ffi.Pointer<ffi.Void>)>>(
          'getContentTotalLength');
  late final _getContentTotalLength = _getContentTotalLengthPtr
      .asFunction<int Function(ffi.Pointer<ffi.Void>)>();

  void flushToTargetFile(
    ffi.Pointer<ffi.Void> mmapFilePtr,
    ffi.Pointer<ffi.Char> filePath,
  ) {
    return _flushToTargetFile(
      mmapFilePtr,
      filePath,
    );
  }

  late final _flushToTargetFilePtr = _lookup<
      ffi.NativeFunction<
          ffi.Void Function(ffi.Pointer<ffi.Void>,
              ffi.Pointer<ffi.Char>)>>('flushToTargetFile');
  late final _flushToTargetFile = _flushToTargetFilePtr.asFunction<
      void Function(ffi.Pointer<ffi.Void>, ffi.Pointer<ffi.Char>)>();

  void flushMMapCacheFile() {
    return _flushMMapCacheFile();
  }

  late final _flushMMapCacheFilePtr =
      _lookup<ffi.NativeFunction<ffi.Void Function()>>('flushMMapCacheFile');
  late final _flushMMapCacheFile =
      _flushMMapCacheFilePtr.asFunction<void Function()>();

  ffi.Pointer<ffi.Char> getTargetFilePath(
    ffi.Pointer<ffi.Void> mmapFilePtr,
  ) {
    return _getTargetFilePath(
      mmapFilePtr,
    );
  }

  late final _getTargetFilePathPtr = _lookup<
      ffi.NativeFunction<
          ffi.Pointer<ffi.Char> Function(
              ffi.Pointer<ffi.Void>)>>('getTargetFilePath');
  late final _getTargetFilePath = _getTargetFilePathPtr
      .asFunction<ffi.Pointer<ffi.Char> Function(ffi.Pointer<ffi.Void>)>();

  void forceFlushToFile() {
    return _forceFlushToFile();
  }

  late final _forceFlushToFilePtr =
      _lookup<ffi.NativeFunction<ffi.Void Function()>>('forceFlushToFile');
  late final _forceFlushToFile =
      _forceFlushToFilePtr.asFunction<void Function()>();
}