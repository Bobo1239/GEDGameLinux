#pragma once

#include "DXUT.h"
#include "SDKmisc.h"
#include "d3dx11effect.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>

// Convenience macros for safe effect variable retrieval
#define SAFE_GET_PASS(Technique, name, var)                                                        \
    {                                                                                              \
        assert(Technique != NULL);                                                                 \
        var = Technique->GetPassByName(name);                                                      \
        assert(var->IsValid());                                                                    \
    }
#define SAFE_GET_TECHNIQUE(effect, name, var)                                                      \
    {                                                                                              \
        assert(effect != NULL);                                                                    \
        var = effect->GetTechniqueByName(name);                                                    \
        assert(var->IsValid());                                                                    \
    }
#define SAFE_GET_SCALAR(effect, name, var)                                                         \
    {                                                                                              \
        assert(effect != NULL);                                                                    \
        var = effect->GetVariableByName(name)->AsScalar();                                         \
        assert(var->IsValid());                                                                    \
    }
#define SAFE_GET_VECTOR(effect, name, var)                                                         \
    {                                                                                              \
        assert(effect != NULL);                                                                    \
        var = effect->GetVariableByName(name)->AsVector();                                         \
        assert(var->IsValid());                                                                    \
    }
#define SAFE_GET_MATRIX(effect, name, var)                                                         \
    {                                                                                              \
        assert(effect != NULL);                                                                    \
        var = effect->GetVariableByName(name)->AsMatrix();                                         \
        assert(var->IsValid());                                                                    \
    }
#define SAFE_GET_SAMPLER(effect, name, var)                                                        \
    {                                                                                              \
        assert(effect != NULL);                                                                    \
        var = effect->GetVariableByName(name)->AsSampler();                                        \
        assert(var->IsValid());                                                                    \
    }
#define SAFE_GET_RESOURCE(effect, name, var)                                                       \
    {                                                                                              \
        assert(effect != NULL);                                                                    \
        var = effect->GetVariableByName(name)->AsShaderResource();                                 \
        assert(var->IsValid());                                                                    \
    }

struct GameEffect {
    // A D3DX rendering effect
    ID3DX11Effect *effect;                // The whole rendering effect
    ID3DX11EffectTechnique *technique;    // One technique to render the effect
    ID3DX11EffectPass *pass0;             // One rendering pass of the technique
    ID3DX11EffectMatrixVariable *worldEV; // World matrix effect variable
    ID3DX11EffectMatrixVariable
        *worldViewProjectionEV;                     // WorldViewProjection matrix effect variable
    ID3DX11EffectShaderResourceVariable *diffuseEV; // Effect variable for the diffuse color texture
    ID3DX11EffectVectorVariable *lightDirEV;        // Light direction in object space

    GameEffect() { ZeroMemory(this, sizeof(*this)); } // WARNING: This will set ALL members to 0!

    HRESULT create(ID3D11Device *device) {
        HRESULT hr;
        // WCHAR path[MAX_PATH];

        // Find and load the rendering effect
        // V_RETURN(DXUTFindDXSDKMediaFileCch(path, MAX_PATH, L"shader\\game.fxo"));
        std::ifstream is("shader/game.fx", std::ios_base::binary);
        if (!is.is_open()) {
            std::cerr << "failed to load shader" << std::endl;
            exit(EXIT_FAILURE);
        }
        is.seekg(0, std::ios_base::end);
        std::streampos pos = is.tellg();
        is.seekg(0, std::ios_base::beg);
        std::vector<char> effectBuffer((unsigned int)pos);
        is.read(&effectBuffer[0], pos);
        is.close();
        printf("PRE CREATE EFFECT\n");
        V_RETURN(D3DX11CompileEffectFromMemory((const void *)&effectBuffer[0], effectBuffer.size(),
                                               "game.fx", nullptr, nullptr, 0, 0, device, &effect,
                                               nullptr));
        printf("POST CREATE EFFECT\n");
        hr = 0;
        printf("%p", (void *)device);
        printf("%d", hr);
        assert(effect->IsValid());

        // Obtain the effect technique
        SAFE_GET_TECHNIQUE(effect, "Render", technique);

        // Obtain the effect pass
        SAFE_GET_PASS(technique, "P0", pass0);

        // Obtain the effect variables
        SAFE_GET_RESOURCE(effect, "g_Diffuse", diffuseEV);
        SAFE_GET_MATRIX(effect, "g_World", worldEV);
        SAFE_GET_MATRIX(effect, "g_WorldViewProjection", worldViewProjectionEV);
        SAFE_GET_VECTOR(effect, "g_LightDir", lightDirEV);

        return S_OK;
    }

    void destroy() { SAFE_RELEASE(effect); }
};

extern GameEffect g_gameEffect;