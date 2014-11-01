#version 330 core

in vec3 color;
in vec2 texc;
out vec4 fragColor;

uniform sampler2D tex;
uniform int useTexture = 0;
uniform float blend = 0;

void main(){
    vec3 texColor = texture(tex, texc).rgb;
    texColor = clamp(texColor + vec3(1-useTexture), vec3(0), vec3(1));
    fragColor = vec4(color*(1-blend) + texColor * blend, 1);
//    fragColor = vec4(blend, blend, blend, 1);
//        fragColor = vec4(1, 1, 1, 1);

//        fragColor = vec4(color*texColor, 1);
//    fragColor = vec4(color, 1);

//        fragColor = vec4(texc, 0, 1);
}
