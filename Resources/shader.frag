#version 120
//input textures
uniform sampler2D colourMap;
uniform sampler2D normalMap;

//information input
uniform vec3 lightPosition = vec3(400, 300, 200);
uniform float rotation = 0.0;
uniform float intensity = 1.1;
uniform float alphaSubtractor = 0.0;

mat4 rotationMatrix(vec3 axis, float angle);

void main()
{
    mat4 rotMat = rotationMatrix( vec3(0,0,1), (rotation)*(3.1415/180.0) );
    
	//get the colour and the normal colour
	vec2 coord = gl_TexCoord[0].xy;
	vec4 colour = texture2D(colourMap, coord);
	vec3 normalColour = texture2D(normalMap, coord).rgb;
    
    vec3 lightDir = -normalize(vec3(gl_FragCoord.xy,0) - lightPosition.xyz);
    vec3 normal = normalize(normalColour * 2.0 - 1.0);
    vec3 rotatedNormal = vec3(rotMat*vec4(normal.rg,1.0,1.0));
    
    float colourIntensity = dot(lightDir,rotatedNormal);
    
    gl_FragColor = clamp(vec4(intensity*colourIntensity*colour.rgb,colour.a-alphaSubtractor), 0.0, 1.0);
}


mat4 rotationMatrix(vec3 axis, float angle){
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}



