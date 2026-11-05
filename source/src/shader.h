#ifndef SHADER_H
#define SHADER_H

#include "cube.h"

typedef GLuint (APIENTRY *PFNGLCREATESHADERPROC)(GLenum);
typedef void (APIENTRY *PFNGLSHADERSOURCEPROC)(GLuint, GLsizei, const GLchar **, const GLint *);
typedef void (APIENTRY *PFNGLCOMPILESHADERPROC)(GLuint);
typedef void (APIENTRY *PFNGLGETSHADERIVPROC)(GLuint, GLenum, GLint *);
typedef void (APIENTRY *PFNGLGETSHADERINFOLOGPROC)(GLuint, GLsizei, GLsizei *, GLchar *);
typedef GLuint (APIENTRY *PFNGLCREATEPROGRAMPROC)(void);
typedef void (APIENTRY *PFNGLATTACHSHADERPROC)(GLuint, GLuint);
typedef void (APIENTRY *PFNGLLINKPROGRAMPROC)(GLuint);
typedef void (APIENTRY *PFNGLGETPROGRAMIVPROC)(GLuint, GLenum, GLint *);
typedef void (APIENTRY *PFNGLGETPROGRAMINFOLOGPROC)(GLuint, GLsizei, GLsizei *, GLchar *);
typedef void (APIENTRY *PFNGLUSEPROGRAMPROC)(GLuint);
typedef GLint (APIENTRY *PFNGLGETUNIFORMLOCATIONPROC)(GLuint, const GLchar *);
typedef void (APIENTRY *PFNGLUNIFORM1IPROC)(GLint, GLint);
typedef void (APIENTRY *PFNGLUNIFORM1FPROC)(GLint, GLfloat);
typedef void (APIENTRY *PFNGLUNIFORM3FPROC)(GLint, GLfloat, GLfloat, GLfloat);
typedef void (APIENTRY *PFNGLUNIFORMMATRIX4FVPROC)(GLint, GLsizei, GLboolean, const GLfloat *);
typedef void (APIENTRY *PFNGLACTIVETEXTUREPROC)(GLenum);
typedef void (APIENTRY *PFNGLDELETEPROGRAMPROC)(GLuint);
typedef void (APIENTRY *PFNGLDELETESHADERPROC)(GLuint);

extern PFNGLCREATESHADERPROC pglCreateShader;
extern PFNGLSHADERSOURCEPROC pglShaderSource;
extern PFNGLCOMPILESHADERPROC pglCompileShader;
extern PFNGLGETSHADERIVPROC pglGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC pglGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC pglCreateProgram;
extern PFNGLATTACHSHADERPROC pglAttachShader;
extern PFNGLLINKPROGRAMPROC pglLinkProgram;
extern PFNGLGETPROGRAMIVPROC pglGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC pglGetProgramInfoLog;
extern PFNGLUSEPROGRAMPROC pglUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC pglGetUniformLocation;
extern PFNGLUNIFORM1IPROC pglUniform1i;
extern PFNGLUNIFORM1FPROC pglUniform1f;
extern PFNGLUNIFORM3FPROC pglUniform3f;
extern PFNGLUNIFORMMATRIX4FVPROC pglUniformMatrix4fv;
extern PFNGLACTIVETEXTUREPROC pglActiveTexture;
extern PFNGLDELETEPROGRAMPROC pglDeleteProgram;
extern PFNGLDELETESHADERPROC pglDeleteShader;

bool initshaders();

struct shader
{
    GLuint program;
    GLuint vert, frag;
    shader();
    ~shader();
    bool load(const char *name);
    void use();
    void unuse();
    GLint uniform(const char *name);
};

shader *loadshader(const char *name);

#endif