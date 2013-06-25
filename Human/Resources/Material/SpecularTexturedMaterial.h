#pragma once 
#include "Material.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamWorld.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamView.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamProjection.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamColorTexture.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamNormalTexture.h"
#include "./Human/Resources/Material/Shader/Params/ShaderParamSpecularTexture.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributePosition.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributeNormal.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributeUV.h"
#include "./Human/Resources/Material/Shader/Attributes/ShaderAttributeTangent.h"
#include "./Human/Rendering/Camera.h"

class SpecularTexturedMaterial : public Material{
public:
	SpecularTexturedMaterial();
	~SpecularTexturedMaterial();

	void EnableAttributes();
	void DisableAttributes();

	void SetMeshData(Mesh* mesh);	
	void SetColorTexture(Texture2D* tex);
	void SetNormalTexture(Texture2D* tex);
	void SetSpecularTexture(Texture2D* tex);
	void SetTextures(Texture2D* color, Texture2D* normal, Texture2D* specular);
	void SetViewProjection();
	
	ShaderParamWorld* World;
	ShaderParamView* View;
	ShaderParamProjection* Projection;
	ShaderParamColorTexture* ColorTexture;
	ShaderParamNormalTexture* NormalTexture;
	ShaderParamSpecularTexture* SpecularTexture;

private:
	ShaderAttributePosition* _Position;
	ShaderAttributeNormal* _Normal;
	ShaderAttributeUV* _UV;
	ShaderAttributeTangent* _Tangent;
};


