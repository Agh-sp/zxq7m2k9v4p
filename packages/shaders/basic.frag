#version 120
uniform sampler2D tex;
uniform int hasTex;
varying vec3 vnormal;
varying vec3 vpos;
void main() {
    vec3 n = normalize(vnormal);
    vec3 l = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(n, l), 0.0);
    vec4 color = (hasTex == 1) ? texture2D(tex, gl_TexCoord[0].xy) : gl_Color;
    gl_FragColor = color * (0.4 + 0.6 * diff);
}
