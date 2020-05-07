/*
Based on the shader code given in the Arnold doc "Creating a Shader".
	https://docs.arnoldrenderer.com/display/AFMUG/Creating+a+shader#CreatingaShader-1.1.1InstallingaC++Compiler 
Malcolm Kesson
Aug 17 2019
*/
#include <ai.h>
#include <cstring>

AI_SHADER_NODE_EXPORT_METHODS(SampleMethods);
AtRGB mix( AtRGB c1, AtRGB c2, float alpha){
	return c1 * (1.0 -alpha) + c2 * alpha;
	} 
namespace {
	enum paramIndex { k_top_color, k_bottom_color, k_height, k_blur, k_mode, k_matrix};
	};
static const char* mode_lables[] = { "Objects", "World", "User", NULL};
node_parameters {
    AiParameterRGB("topColor", 0.7f, 0.7f, 0);
    AiParameterRGB("bottomColor", 0.7f, 0, 0);
	AiParameterFlt("height", 0);
	AiParameterFlt("blur", 0);
	AiParameterEnum("mode", 0, mode_lables);
	AiParameterMtx("matrixInput", AI_M4_IDENTITY);

	}
 
shader_evaluate {
	AtRGB top_color = AiShaderEvalParamRGB(k_top_color);
	AtRGB bottom_color = AiShaderEvalParamRGB(k_bottom_color);
	float height = AiShaderEvalParamFlt(k_height);
	float blur = AiShaderEvalParamFlt(k_blur);
	int mode = AiShaderEvalParamEnum(k_mode);
	float Y;
	switch(mode){
		case 0 : 
			Y = sg->Po.y;
		case 1 :
			Y = sg->P.y;
		case 2 :
			AtMatrix m = *AiShaderEvalParamMtx(k_matrix);
			AtMatrix inverted_m = AiM4Invert(m);
			AtVector transformedP = AiM4PointByMatrixMult(inverted_m, sg->P);
			Y = transformedP.y;
		}
	float minHt = height - blur/2;
	float maxHt = height + blur/2;
	float blend = AiSmoothStep(minHt, maxHt,Y);
	sg->out.RGB() = mix(top_color, bottom_color, blend);
	}
 
node_loader {
    if (i > 0)
        return false; 
    node->methods        = SampleMethods;
    node->output_type    = AI_TYPE_RGB;
	node->name           = "EffectByHeight";
    node->node_type      = AI_NODE_SHADER;
    strcpy(node->version, AI_VERSION);
    return true;
	}
	
// The remaining macros can be left "empty"
node_initialize { }
node_update { }
node_finish { }
