#version 420

uniform sampler2D depth;
uniform sampler2D normal;
uniform sampler2D diffuse;
uniform vec3 lightPos;

in vec2 vTexCoord;

out vec4 color;

#define M_PI 3.1415926535897932384626433832795

void main(void) {
    // Sun light properties
    vec3 sunLightColor = vec3(1.0f);
    float sunLightPower = 1.0f;

    // material properties
    vec3 matDiffuseColor = texture(diffuse, vTexCoord).xyz;
    vec3 matSpecularColor = vec3(0.5f);

    //light calculations
    float aspectRatio = 16.0f/9.0f; //width/height
    float fov = 60/180*M_PI;

    vec2 screenPos = vTexCoord*2-1;
    screenPos.x *= aspectRatio;
    screenPos *= sin(fov);

    float f=100.0;
    float n = 0.01;
    float z = (2 * n) / (f + n - texture( depth, vTexCoord ).x * (f - n));
    z = n + z*(f-n);

    vec3 fragmentPos = -vec3(screenPos, 1.0)*z;

    vec3 E = normalize(-fragmentPos);
    vec3 lightVector = lightPos - fragmentPos;
    vec3 normalVector = texture(normal, vTexCoord).xyz;
    vec3 R = reflect(-normalize(lightVector), normalize(normalVector));
    float cosAlpha = clamp(dot(E,R), 0.0f, 1.0f);
    float cosTheta = max(dot(normalize(normalVector), normalize(lightVector)),0.0f);

    // Sample the shadow map 16 times, 4 texture() calls * 4 samples each call
    float visibility = 1.0;

    color = //vec4(z, 0, 0, 1.0) + 0.0001*
            vec4(matDiffuseColor*sunLightColor*sunLightPower*visibility*cosTheta + //sun light (diffuse)
                 matSpecularColor*sunLightColor*sunLightPower*visibility*pow(cosAlpha,10)*cosTheta,1.0f); //sun light (specular)
}
