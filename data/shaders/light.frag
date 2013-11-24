#version 420

uniform sampler2D depth;
uniform sampler2D color0;
uniform sampler2D color1;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform mat4 invProj;
uniform float lightRadius;
uniform vec2 invResolution;

out vec4 color;

vec3 getFragPos(vec2 vTexCoord) {
    vec4 sPos = vec4(vTexCoord*2-1, texture(depth, vTexCoord).x*2-1, 1.0);
    sPos = invProj * sPos;
    return sPos.xyz/sPos.w;
}

void main(void) {
    vec2 vTexCoord = gl_FragCoord.xy*invResolution;

    vec4 valColor0 = texture(color0, vTexCoord);
    vec4 valColor1 = texture(color1, vTexCoord);

    //material properties
    vec3 matDiffuseColor = valColor0.xyz;
    vec3 matSpecularColor = vec3(valColor1.w);

    //fragment light parameters
    vec3 fragmentPos = getFragPos(vTexCoord); //view space
    vec3 lightVector = normalize(lightPos - fragmentPos); //view space
    vec2 fenc = valColor1.xy*4;
    float f = dot(fenc,fenc);
    float  g = sqrt(1-f/4);
    vec3 normalVector = vec3(fenc*g, 1-f/2);

//    vec3 normalVector = vec3(normalVector2, sqrt(1-dot(normalVector2, normalVector2))); //view space

    //Blinn-Phong shading
    vec3 E = normalize(-fragmentPos);
    vec3 H = normalize(lightVector+E);
    float cosAlpha = clamp(dot(normalVector, H), 0.0f, 1.0f);
    float cosTheta = max(dot(normalize(normalVector), normalize(lightVector)), 0.0f);
    float attenuationFactor = max(0.0, 1-length(fragmentPos-lightPos)/lightRadius);

    color = vec4(matDiffuseColor*lightColor*cosTheta*attenuationFactor + //sun light (diffuse)
                 matSpecularColor*lightColor*pow(cosAlpha, 100)*cosTheta, 1.0f)*attenuationFactor; //sun light (specular)
}
