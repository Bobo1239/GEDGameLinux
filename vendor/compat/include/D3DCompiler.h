#include <d3dcompiler.h>

#include <safe_lib.h>
#include <climits>
#include <cfloat>
#include <string.h>
#include "shared.h"

// #define _GUIDDEF_H_
#define INITGUID
#include <guiddef.h>

#define _UI32_MAX UINT32_MAX
#define _UI64_MAX UINT64_MAX

#define D3DCOMPILE_EFFECT_CHILD_EFFECT (1 << 0)
#define D3D11_SHVER_VERTEX_SHADER 1
#define D3D11_SHVER_GET_TYPE(_Version) \
(((_Version) >> 16) & 0xffff)
#define D3D11_DEFAULT_BORDER_COLOR_COMPONENT    ( 0.0f )
#define _stricmp strcasecmp
#define sprintf_s sprintf

#ifdef REALLY_DEFINE_GUID
    DEFINE_GUID(IID_ID3D11ShaderReflection, 2371054753, 3274, 18774, 168, 55, 120, 105, 99, 117, 85, 132);
#else
    extern const GUID IID_ID3D11ShaderReflection;
#endif