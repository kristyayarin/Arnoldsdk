#include <ai.h>
#include <cstring>

AI_SHADER_NODE_EXPORT_METHODS(SampleMethods);

AtRGB mix( AtRGB c1, AtRGB c2, float alpha){
	return c1 * (1.0 -alpha) + c2 * alpha;
	} 

namespace {
	enum paramIndex { k_face_color, k_edge_color, k_gain };
	};
 
node_parameters {
    AiParameterRGB("faceRGB", 0.7f, 0.7f, 0);
    AiParameterRGB("edgeRGB", 0.7f, 0, 0);
	AiParameterFlt("gain", 3);
	}
 
shader_evaluate {
	AtRGB faceRGB = AiShaderEvalParamRGB(k_face_color);
	AtRGB edgeRGB = AiShaderEvalParamRGB(k_edge_color);
	float gain = AiShaderEvalParamFlt(k_gain);

	AtVector i = AiV3Normalize(sg->Rd);
	AtVector n = AiV3Normalize(sg->Nf);
	float cosineOfTheAngle = 1 - fabs(AiV3Dot(i,n));
	sg->out.RGB() = mix(faceRGB, edgeRGB,pow(cosineOfTheAngle, gain));
	}
 
node_loader {
    if (i > 0)
        return false; 
    node->methods        = SampleMethods;
    node->output_type    = AI_TYPE_RGB;
	node->name           = "xhEdge";
    node->node_type      = AI_NODE_SHADER;
    strcpy(node->version, AI_VERSION);
    return true;
	}
	
// The remaining macros can be left "empty"
node_initialize { }
node_update { }
node_finish { }
