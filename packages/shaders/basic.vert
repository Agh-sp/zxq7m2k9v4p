#version 120
varying vec3 vnormal;
varying vec3 vpos;
void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vnormal = gl_NormalMatrix * gl_Normal;
    vpos = vec3(gl_ModelViewMatrix * gl_Vertex);
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}
