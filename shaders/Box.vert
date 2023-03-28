#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNomal;

layout(binding = 0) uniform uData
{
	mat4 mViewProj;
} ubo;

layout(push_constant) uniform uPush
{
	vec4 mColor;
	mat4 mWorld;
}pc;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 localPos;

void main() 
{
	//mat4 m = mat4(0,0.317075,-0.952118,-0.856907,
	//	   -0.346896,0,0,0,
	//	   0,0.528458,0.571271,0.514144,
	//	   0,0,2.2372,2.91348);

    gl_Position = pc.mWorld * vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);
	gl_Position = ubo.mViewProj * gl_Position;
	
    fragColor = vec3(pc.mColor);
	localPos = inPosition;
}
