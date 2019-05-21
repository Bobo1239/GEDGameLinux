#include "new.h"
#include <cassert>

#include <d3d11_1.h>

int _wtoi(const wchar_t *str) { return wcstol(str, nullptr, 0); }

// Adapted from https://github.com/microsoft/ChakraCore/blob/master/pal/src/cruntime/wchar.cpp
char16_t wtolower(char16_t c) {
    if (c < 128) { // fast path for ascii characters
        if (c >= 'A' && c <= 'Z') {
            c += ('a' - 'A');
            return c;
        } else {
            return c;
        }
    } else {
        assert(false);
    }
}

// Adapted from https://github.com/microsoft/ChakraCore/blob/master/pal/src/cruntime/wchar.cpp
int _wcsnicmp(const wchar_t *string1, const wchar_t *string2, size_t count) {
    size_t i;
    int diff = 0;

    for (i = 0; i < count; i++) {
        diff = wtolower(string1[i]) - wtolower(string2[i]);
        if (diff != 0 || 0 == string1[i] || 0 == string2[i]) {
            break;
        }
    }
    return diff;
}

int vswprintf_s(wchar_t *buffer, size_t numberOfElements, const wchar_t *format, va_list argptr) {
    assert(false);
    return 0;
}

// Adapted from https://github.com/Microsoft/ChakraCore/blob/master/pal/inc/rt/safecrt.h
int _wsplitpath_s(const wchar_t *_Path, wchar_t *_Drive, size_t _DriveSize, wchar_t *_Dir,
                  size_t _DirSize, wchar_t *_Filename, size_t _FilenameSize, wchar_t *_Ext,
                  size_t _ExtSize) {
    const wchar_t *tmp;
    const wchar_t *last_slash;
    const wchar_t *dot;
    int drive_set = 0;
    size_t length = 0;
    int bEinval = 0;

    /* validation section */
    if ((_Drive == nullptr && _DriveSize != 0) || (_Drive != nullptr && _DriveSize == 0)) {
        goto error_einval;
    }
    if ((_Dir == nullptr && _DirSize != 0) || (_Dir != nullptr && _DirSize == 0)) {
        goto error_einval;
    }
    if ((_Filename == nullptr && _FilenameSize != 0) ||
        (_Filename != nullptr && _FilenameSize == 0)) {
        goto error_einval;
    }
    if ((_Ext == nullptr && _ExtSize != 0) || (_Ext != nullptr && _ExtSize == 0)) {
        goto error_einval;
    }

    /* check if _Path begins with the longpath prefix */
    if (_Path[0] == L'\\' && _Path[1] == L'\\' && _Path[2] == L'?' && _Path[3] == L'\\') {
        _Path += 4;
    }

    /* extract drive letter and ':', if any */
    if (!drive_set) {
        size_t skip = _MAX_DRIVE - 2;
        tmp = _Path;
        while (skip > 0 && *tmp != 0) {
            skip--;
            tmp++;
        }
        if (*tmp == L':') {
            if (_Drive != nullptr) {
                if (_DriveSize < _MAX_DRIVE) {
                    goto error_erange;
                }
                wcsncpy_s(_Drive, _DriveSize, _Path, _MAX_DRIVE - 1);
            }
            _Path = tmp + 1;
        } else {
            if (_Drive != nullptr) {
                // _SAFECRT__RESET_STRING(_Drive, _DriveSize);
            }
        }
    }

    /* extract path string, if any. _Path now points to the first character
     * of the path, if any, or the filename or extension, if no path was
     * specified.  Scan ahead for the last occurence, if any, of a '/' or
     * '\' path separator character.  If none is found, there is no path.
     * We will also note the last '.' character found, if any, to aid in
     * handling the extension.
     */
    last_slash = nullptr;
    dot = nullptr;
    tmp = _Path;
    for (; *tmp != 0; ++tmp) {
        {
            if (*tmp == L'/' || *tmp == L'\\') {
                /* point to one beyond for later copy */
                last_slash = tmp + 1;
            } else if (*tmp == L'.') {
                dot = tmp;
            }
        }
    }

    if (last_slash != nullptr) {
        /* found a path - copy up through last_slash or max characters
         * allowed, whichever is smaller
         */
        if (_Dir != nullptr) {
            length = (size_t)(last_slash - _Path);
            if (_DirSize <= length) {
                goto error_erange;
            }
            wcsncpy_s(_Dir, _DirSize, _Path, length);
        }
        _Path = last_slash;
    } else {
        /* there is no path */
        if (_Dir != nullptr) {
            // _SAFECRT__RESET_STRING(_Dir, _DirSize);
        }
    }

    /* extract file name and extension, if any.  Path now points to the
     * first character of the file name, if any, or the extension if no
     * file name was given.  Dot points to the '.' beginning the extension,
     * if any.
     */
    if (dot != nullptr && (dot >= _Path)) {
        /* found the marker for an extension - copy the file name up to the '.' */
        if (_Filename) {
            length = (size_t)(dot - _Path);
            if (_FilenameSize <= length) {
                goto error_erange;
            }
            wcsncpy_s(_Filename, _FilenameSize, _Path, length);
        }
        /* now we can get the extension - remember that tmp still points
         * to the terminating nullptr character of path.
         */
        if (_Ext) {
            length = (size_t)(tmp - dot);
            if (_ExtSize <= length) {
                goto error_erange;
            }
            wcsncpy_s(_Ext, _ExtSize, dot, length);
        }
    } else {
        /* found no extension, give empty extension and copy rest of
         * string into fname.
         */
        if (_Filename) {
            length = (size_t)(tmp - _Path);
            if (_FilenameSize <= length) {
                goto error_erange;
            }
            wcsncpy_s(_Filename, _FilenameSize, _Path, length);
        }
        if (_Ext) {
            // _SAFECRT__RESET_STRING(_Ext, _ExtSize);
        }
    }

    return 0;

error_einval:
    bEinval = 1;

error_erange:
    if (_Drive != nullptr && _DriveSize > 0) {
        // _SAFECRT__RESET_STRING(_Drive, _DriveSize);
    }
    if (_Dir != nullptr && _DirSize > 0) {
        // _SAFECRT__RESET_STRING(_Dir, _DirSize);
    }
    if (_Filename != nullptr && _FilenameSize > 0) {
        // _SAFECRT__RESET_STRING(_Filename, _FilenameSize);
    }
    if (_Ext != nullptr && _ExtSize > 0) {
        // _SAFECRT__RESET_STRING(_Ext, _ExtSize);
    }

    if (bEinval) {
        // _SAFECRT__RETURN_EINVAL;
    }

    // _SAFECRT__RETURN_BUFFER_TOO_SMALL(_Strings, _StringSizes);
    /* should never happen, but compiler can't tell */
    return EINVAL;
}

void unimplemented() {
    printf("Unimplemented!");
    exit(EXIT_FAILURE);
}

// Functions from the files we can't compile
namespace DirectX {
HRESULT SaveDDSTextureToFile(ID3D11DeviceContext *pContext, ID3D11Resource *pSource,
                             const wchar_t *fileName) {
    unimplemented();
    return 0;
}

HRESULT CreateWICTextureFromFileEx(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext,
                                   const wchar_t *szFileName, size_t maxsize, D3D11_USAGE usage,
                                   unsigned int bindFlags, unsigned int cpuAccessFlags,
                                   unsigned int miscFlags, unsigned int loadFlags,
                                   ID3D11Resource **texture,
                                   ID3D11ShaderResourceView **textureView) {
    unimplemented();
    return 0;
}

HRESULT CreateWICTextureFromFile(ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dContext,
                                 const wchar_t *szFileName, ID3D11Resource **texture,
                                 ID3D11ShaderResourceView **textureView, size_t maxsize = 0) {
    unimplemented();
    return 0;
}
} // namespace DirectX
