#pragma once

// From https://github.com/DavidEGrayson/intsafe/blob/master/generated/intsafe.h

#define INTSAFE_E_ARITHMETIC_OVERFLOW ((HRESULT)0x80070216)

#define __MINGW_INTSAFE_API FORCEINLINE

#define __MINGW_INTSAFE_BODY(operation, x, y) \
{ \
  if (__builtin_##operation##_overflow(x, y, result)) \
  { \
      *result = 0; \
      return INTSAFE_E_ARITHMETIC_OVERFLOW; \
  } \
  return S_OK; \
}

#define __MINGW_INTSAFE_CONV(name, type_src, type_dest) \
    HRESULT name(type_src operand, type_dest * result) \
    __MINGW_INTSAFE_BODY(add, operand, 0)

#define __MINGW_INTSAFE_MATH(name, type, operation) \
    HRESULT name(type x, type y, type * result) \
__MINGW_INTSAFE_BODY(operation, x, y)

__MINGW_INTSAFE_API __MINGW_INTSAFE_MATH(UIntAdd, UINT, add)