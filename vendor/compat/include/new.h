#pragma once

// Notes:
//     https://stackoverflow.com/a/34477302
//     https://www.winehq.org/pipermail/wine-devel/2013-May/099757.html

// This file contains a bunch of fixes for DXUT

// DXUT.h includes <new.h> instead of <new>
#include <iterator>
#include <new>
#include <safe_lib.h>
#include <windows.h>
#include <wchar.h>
#include <fileapi.h>

#define _GUIDDEF_H_
#include <guiddef.h>

#include "shared.h"

// typedef struct _GUID {  DWORD Data1;  WORD Data2;  WORD Data3;  BYTE Data4[8]; } GUID;
// static _GUID UNIVERSAL_GUID = {0x42424242};

// This file is only included by DXUT which uses this MSVC-specific function annotation
// We just let it expand to nothing
#define _MAX_EXT 256
#define _MAX_DRIVE (3)

#ifdef UNICODE
#define XINPUT_DLL L"xinput9_1_0.dll"
#else
#define XINPUT_DLL "xinput9_1_0.dll"
#endif

// From Google's V8
#define ARRAYSIZE(a) ((sizeof(a) / sizeof(*(a))) / static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
// TODO: maybe this could work:
// #define __uuidof(T) __wine_uuidof(T)
// #define __uuidof(T) UNIVERSAL_GUID
// #define IID_PPV_ARGS(pp) UNIVERSAL_GUID, reinterpret_cast<void **>(pp)
// TODO: These actually do something...
// https://docs.microsoft.com/en-us/visualstudio/code-quality/annotating-locking-behavior?view=vs-2019
#define _Acquires_lock_(expr)
#define _Releases_lock_(expr)
#define DXGI_ADAPTER_FLAG_SOFTWARE 2
// #define CreateFile2 CreateFile

// From https://github.com/Microsoft/DirectXShaderCompiler/issues/1342
// #include <typeindex>
// #define uuid(guid_str)
// #define __uuidof(T) std::type_index(typeid(T))
#define IID_PPV_ARGS(ppType) __uuidof(**(ppType)), reinterpret_cast<void **>(ppType)
// typedef std::type_index REFIID;
// Not available from g++

// #define wcsnlen_s(str, strsz) wcsnlen(str, strsz)
// #define wcsncpy_s(dest, destsz, src, n) wcsncpy(dest, src, n)

// // TODO: just forward declare all normal functions

template <size_t size>
errno_t wcsncpy_s(
   wchar_t (&strDest)[size],
   const wchar_t *strSource,
   size_t count
) {
    return wcsncpy_s(strDest, size, strSource, count);
}

template <size_t size>
errno_t wcscpy_s(
   wchar_t (&dest)[size],
   const wchar_t *src
) {
    return wcscpy_s(dest, size, src);
}

#define swprintf_s swprintf
char16_t wtolower(char16_t c);
int _wtoi(const wchar_t *str);
int _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count);
#define _wcscmp(s1, s2) _wcsncmp(s1, 9999, s2)
#define _wcsicmp(s1, s2) _wcsnicmp(s1, s2, 9999)
int _wsplitpath_s(const WCHAR *_Path, WCHAR *_Drive, size_t _DriveSize, WCHAR *_Dir,
                  size_t _DirSize, WCHAR *_Filename, size_t _FilenameSize, WCHAR *_Ext,
                  size_t _ExtSize);
int vswprintf_s(
   wchar_t *buffer,
   size_t numberOfElements,
   const wchar_t *format,
   va_list argptr
);

// int swprintf_s(
//    wchar_t *buffer,
//    int n,
//    const wchar_t *format,
//    ...
// ) {
//     return swprintf_s(buffer, format, );
// }
