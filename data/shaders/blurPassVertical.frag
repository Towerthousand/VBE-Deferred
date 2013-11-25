#version 420

uniform sampler2D RTBlurH; // this should hold the texture rendered by the horizontal blur pass
uniform vec2 invResolution;

out vec4 color;

const float blurSize = invResolution.y;

void main(void) {
   vec2 texCoord = gl_FragCoord.xy*invResolution;
   vec4 sum = vec4(0.0);

   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture2D(RTBlurH, vec2(texCoord.x, texCoord.y - 4.0*blurSize)) * 0.05;
   sum += texture2D(RTBlurH, vec2(texCoord.x, texCoord.y - 3.0*blurSize)) * 0.09;
   sum += texture2D(RTBlurH, vec2(texCoord.x, texCoord.y - 2.0*blurSize)) * 0.12;
   sum += texture2D(RTBlurH, vec2(texCoord.x, texCoord.y - blurSize)) * 0.15;
   sum += texture2D(RTBlurH, vec2(texCoord.x, texCoord.y)) * 0.18;
   sum += texture2D(RTBlurH, vec2(texCoord.x, texCoord.y + blurSize)) * 0.15;
   sum += texture2D(RTBlurH, vec2(texCoord.x, texCoord.y + 2.0*blurSize)) * 0.12;
   sum += texture2D(RTBlurH, vec2(texCoord.x, texCoord.y + 3.0*blurSize)) * 0.09;
   sum += texture2D(RTBlurH, vec2(texCoord.x, texCoord.y + 4.0*blurSize)) * 0.05;

   color = sum;
}
