#version 420

uniform sampler2D depth;
uniform sampler2D normal; //view space
uniform sampler2D diffuse;
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

    //material properties
    vec3 matDiffuseColor = texture(diffuse, vTexCoord).xyz;
    vec3 matSpecularColor = vec3(2.0f);

    //fragment light parameters
    vec3 fragmentPos = getFragPos(vTexCoord); //view space
    vec3 lightVector = normalize(lightPos - fragmentPos); //view space
    vec3 normalVector = normalize(texture(normal, vTexCoord).xyz); //view space

    //phong shading
    vec3 E = normalize(-fragmentPos);
    vec3 R = reflect(-normalize(lightVector), normalize(normalVector));
    float cosAlpha = clamp(dot(E,R), 0.0f, 1.0f);
    float cosTheta = max(dot(normalize(normalVector), normalize(lightVector)),0.0f);
    float attenuationFactor = max(0.0, 1-length(fragmentPos-lightPos)/lightRadius);

    color = vec4(matDiffuseColor*lightColor*cosTheta*attenuationFactor + //sun light (diffuse)
                 matSpecularColor*lightColor*pow(cosAlpha,100)*cosTheta,1.0f)*attenuationFactor; //sun light (specular)
}
