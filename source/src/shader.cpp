#include "cube.h"
#include "shader.h"
#include <cstdio>

PFNGLCREATESHADERPROC pglCreateShader = NULL;
PFNGLSHADERSOURCEPROC pglShaderSource = NULL;
PFNGLCOMPILESHADERPROC pglCompileShader = NULL;
PFNGLGETSHADERIVPROC pglGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog = NULL;
PFNGLCREATEPROGRAMPROC pglCreateProgram = NULL;
PFNGLATTACHSHADERPROC pglAttachShader = NULL;
PFNGLLINKPROGRAMPROC pglLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC pglGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC pglGetProgramInfoLog = NULL;
PFNGLUSEPROGRAMPROC pglUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation = NULL;
PFNGLUNIFORM1IPROC pglUniform1i = NULL;
PFNGLUNIFORM1FPROC pglUniform1f = NULL;
PFNGLUNIFORM3FPROC pglUniform3f = NULL;
PFNGLUNIFORMMATRIX4FVPROC pglUniformMatrix4fv = NULL;
PFNGLACTIVETEXTUREPROC pglActiveTexture = NULL;
PFNGLDELETEPROGRAMPROC pglDeleteProgram = NULL;
PFNGLDELETESHADERPROC pglDeleteShader = NULL;

bool initshaders()
{
    pglCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
    pglShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
    pglCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
    pglGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
    pglGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
    pglCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
    pglAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
    pglLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
    pglGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
    pglGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
    pglUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
    pglGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
    pglUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
    pglUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
    pglUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
    pglUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
    pglActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
    pglDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
    pglDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
    return pglCreateShader && pglCreateProgram && pglUseProgram;
}

static char *readfile(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = new char[size + 1];
    fread(buf, 1, size, f);
    buf[size] = 0;
    fclose(f);
    return buf;
}

shader::shader() : program(0), vert(0), frag(0) {}

shader::~shader()
{
    if (program) pglDeleteProgram(program);
    if (vert) pglDeleteShader(vert);
    if (frag) pglDeleteShader(frag);
}

bool shader::load(const char *name)
{
    if (!pglCreateShader) return false;
    char vpath[256], fpath[256];
    snprintf(vpath, sizeof(vpath), "packages/shaders/%s.vert", name);
    snprintf(fpath, sizeof(fpath), "packages/shaders/%s.frag", name);
    char *vsrc = readfile(vpath);
    char *fsrc = readfile(fpath);
    if (!vsrc || !fsrc) { delete[] vsrc; delete[] fsrc; return false; }
    vert = pglCreateShader(GL_VERTEX_SHADER);
    pglShaderSource(vert, 1, (const GLchar **)&vsrc, NULL);
    pglCompileShader(vert);
    frag = pglCreateShader(GL_FRAGMENT_SHADER);
    pglShaderSource(frag, 1, (const GLchar **)&fsrc, NULL);
    pglCompileShader(frag);
    program = pglCreateProgram();
    pglAttachShader(program, vert);
    pglAttachShader(program, frag);
    pglLinkProgram(program);
    delete[] vsrc;
    delete[] fsrc;
    return true;
}

void shader::use() { if (program) pglUseProgram(program); }
void shader::unuse() { pglUseProgram(0); }
GLint shader::uniform(const char *name) { return pglGetUniformLocation(program, name); }

static hashtable<const char *, shader *> shaders;

shader *loadshader(const char *name)
{
    shader **s = shaders.access(name);
    if (s) return *s;
    shader *sh = new shader();
    if (!sh->load(name)) { delete sh; return NULL; }
    shaders.access(name, sh);
    return sh;
}