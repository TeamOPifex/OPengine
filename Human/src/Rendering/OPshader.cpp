#include "./Human/include/Rendering/OPshader.h"
#include "./Human/include/Rendering/OPrender.h"
#include "./Data/include/OPcman.h"
#include "./Data/include/OPstream.h"
#include "./Data/include/OPfile.h"
#include "./Data/include/OPstring.h"
#include "./Core/include/OPlog.h"

OPint OPshaderLoadVertex(OPstream* source, OPshader** shader){	
	*shader = OPRENDERER_ACTIVE->Shader.Create(OPshaderType::VERTEX, source);
#ifdef _DEBUG
	(*shader)->source = OPstringCopy(source->Source);
#endif
	return 1;
}

OPint OPshaderLoadFragment(OPstream* source, OPshader** shader){
	*shader = OPRENDERER_ACTIVE->Shader.Create(OPshaderType::FRAGMENT, source);
#ifdef _DEBUG
	(*shader)->source = OPstringCopy(source->Source);
#endif
	return 1;
}


OPint OPshaderReloadVertex(OPstream* str, OPshader** shader) {
	OPlog("Reload Vertex Shader");
	OPshader* resultShader;
	OPshader* tex = (OPshader*)(*shader);
	i32 result = OPshaderLoadVertex(str, &resultShader);
	if (result) {
		OPmemcpy(*shader, resultShader, sizeof(OPshader));
		OPfree(resultShader);
	}
	return result;
}

OPint OPshaderReloadFragment(OPstream* str, OPshader** shader) {
	OPlog("Reload Fragment Shader");
	OPshader* resultShader;
	OPshader* tex = (OPshader*)(*shader);
	i32 result = OPshaderLoadFragment(str, &resultShader);
	if (result) {
		OPmemcpy(*shader, resultShader, sizeof(OPshader));
		OPfree(resultShader);
	}
	return result;
}

OPint OPshaderUnload(OPshader* shader) {
#ifdef _DEBUG
	OPfree(shader->source);
#endif
	OPRENDERER_ACTIVE->Shader.Destroy(shader);
	OPfree(shader); // free up the integer
	return 1;
}

OPassetLoader OPASSETLOADER_VERT = {
	".vert",
#ifdef _DEBUG
	#if defined(OPIFEX_ANDROID) || defined(OPIFEX_IOS)
		"Shaders/OPENGL_ES_2_0/",
	#else
		#ifdef OPIFEX_DIRECTX_11
			"Shaders/DIRECTX_11/",
		#else
			#ifdef OPIFEX_OPENGL_3_3
				"Shaders/OPENGL_3_3/",
			#else
				"Shaders/OPENGL_2_0/",
			#endif
		#endif
	#endif
#else
	"Shaders/",
#endif
	sizeof(OPshader),
#ifdef OPIFEX_DIRECTX_11
	(OPint(*)(OPstream*, void**))OPshaderLoadVertexDX11,
#else
	(OPint(*)(OPstream*, void**))OPshaderLoadVertex,
#endif
	(OPint(*)(void*))OPshaderUnload,
	(OPint(*)(OPstream*, void**))OPshaderReloadVertex
};

OPassetLoader OPASSETLOADER_FRAG = {
	".frag",
#ifdef _DEBUG
	#if defined(OPIFEX_ANDROID) || defined(OPIFEX_IOS)
		"Shaders/OPENGL_ES_2_0/",
	#else
		#ifdef OPIFEX_DIRECTX_11
			"Shaders/DIRECTX_11/",
		#else
			#ifdef OPIFEX_OPENGL_3_3
				"Shaders/OPENGL_3_3/",
			#else
				"Shaders/OPENGL_2_0/",
			#endif
		#endif
	#endif
#else
	"Shaders/",
#endif
	sizeof(OPshader),
#ifdef OPIFEX_DIRECTX_11
	(OPint(*)(OPstream*, void**))OPshaderLoadFragmentDX11,
#else
	(OPint(*)(OPstream*, void**))OPshaderLoadFragment,
#endif
	(OPint(*)(void*))OPshaderUnload,
	(OPint(*)(OPstream*, void**))OPshaderReloadFragment
};