#include "./Human/include/Rendering/OPshader.h"
#include "./Human/include/Utilities/Errors.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Data/include/OPcman.h"
#include "./Data/include/OPstream.h"
#include "./Data/include/OPfile.h"
#include "./Core/include/OPlog.h"

//-----------------------------------------------------------------------------
// ______                _   _
//|  ____|              | | (_)
//| |__ _   _ _ __   ___| |_ _  ___  _ __  ___
//|  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
//| |  | |_| | | | | (__| |_| | (_) | | | \__ \
//|_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
// Shader creation
OPint OPshaderLoadVertex(OPstream* source, OPshader** shader){
	
	*shader = OPRENDERER_ACTIVE->Shader.Create(OPshaderType::VERTEX, source);

	return 1;
}
//
//#ifdef OPIFEX_DIRECTX_11
//#include <windows.h>
//#include <windowsx.h>
//#include <d3d11.h>
//#include <d3d11_1.h>
//#include <d3dcompiler.h>
//
//OPint OPshaderCompileDX11(OPstream* source, OPshader** shader, const OPchar* shaderType, const OPchar* shaderVersion, ID3DBlob** blob) {
//	HRESULT hr = S_OK;
//
//	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#ifdef _DEBUG
//	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
//	// Setting this flag improves the shader debugging experience, but still allows
//	// the shaders to be optimized and to run exactly the way they will run in
//	// the release configuration of this program.
//	dwShaderFlags |= D3DCOMPILE_DEBUG;
//
//	// Disable optimizations to further improve shader debugging
//	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
//#endif
//
//	ID3DBlob* pErrorBlob = nullptr;
//	hr = D3DCompile(
//		source->Data, // pSrcData
//		source->Length, // SrcDataSize
//		source->Source, // pSourceName
//		nullptr, //pDefines
//		0, // pInclude
//		shaderType, // pEntryPoint
//		shaderVersion, // pTarget
//		dwShaderFlags, // Flags1
//		0, // Flags2
//		blob, // ppCode
//		&pErrorBlob); //ppErrorMsgs
//
//					  //hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
//					  //	dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
//	if (FAILED(hr))
//	{
//		if (pErrorBlob)
//		{
//			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
//			OPlogErr(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
//			pErrorBlob->Release();
//		}
//		return -1;
//	}
//	if (pErrorBlob) pErrorBlob->Release();
//
//	return 1;
//}
//
//#include "./Human/include/Rendering/OPrender.h"
//OPint OPshaderLoadVertexDX11(OPstream* source, OPshader** shader) {
//	ID3DBlob* blob;
//	OPint result = OPshaderCompileDX11(source, shader, "VS", "vs_4_0", &blob);
//	if (result <= 0) {
//		return result;
//	}
//	HRESULT hr = S_OK;
//
//	ID3D11VertexShader* g_pVertexShader = nullptr;
//	hr = g_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &g_pVertexShader);
//	if (FAILED(hr)) {
//		OPlogErr("Failed to create the Vertex Shader");
//		return -2;
//	}
//
//	return result;
//}
//
//OPint OPshaderLoadFragmentDX11(OPstream* source, OPshader** shader) {
//	ID3DBlob* blob;
//	OPint result = OPshaderCompileDX11(source, shader, "PS", "ps_4_0", &blob);
//	if (result <= 0) {
//		return result;
//	}
//	HRESULT hr = S_OK;
//
//	ID3D11PixelShader* g_pPixelShader = nullptr;
//	hr = g_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &g_pPixelShader);
//	if (FAILED(hr)) {
//		OPlogErr("Failed to create the Pixel Shader");
//		return -2;
//	}
//
//	return result;
//}
//#endif

//-----------------------------------------------------------------------------
OPint OPshaderLoadFragment(OPstream* source, OPshader** shader){

	*shader = OPRENDERER_ACTIVE->Shader.Create(OPshaderType::FRAGMENT, source);

	return 1;
}
//-----------------------------------------------------------------------------
// shader destruction
OPint OPshaderUnload(OPshader* shader){
	OPRENDERER_ACTIVE->Shader.Destroy(shader);
	OPfree(shader); // free up the integer
	return 1;
}
