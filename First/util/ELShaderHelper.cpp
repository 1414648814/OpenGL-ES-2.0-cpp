#include "ELShaderHelper.h"
#include "gl_include.h"

NS_BEGIN(elloop);

/************************************************************************/
/* related OpenGL ES 2.0 API                                            */
/************************************************************************/

/*

GLuint glCreateShader(	GLenum shaderType); // returns 0 when fail.


void glGetShaderiv(GLuint shader,
GLenum pname,
GLint *params); // return GL_TRUE or GL_FALSE


GLuint glCreateProgram(	void); // returns 0 when fail.

*/


ShaderId ShaderHelper::compileShader(const std::string &shaderSrc, 
                                     GLenum shaderType)
{
    // create shader.
    ShaderId shaderId = glCreateShader(shaderType);
    assert(shaderId != 0);

    // attach source.
    const GLchar* pSrc = shaderSrc.c_str();
    glShaderSource(shaderId, 1, &pSrc, NULL);

    // compile.
    glCompileShader(shaderId);

#ifdef _DEBUG
    // check the compile result.
    GLint compileStatus(0);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
    if (GL_FALSE == compileStatus)
    {
        GLchar msg[500];
        glGetShaderInfoLog(shaderId, sizeof(msg), 0, msg);
        glDeleteShader(shaderId);
        shaderId = 0;
        assert(msg && false);
    }
#endif

    return shaderId;
}

ProgramId ShaderHelper::linkProgram(ShaderId vertexShaderId, 
                                    ShaderId fragShaderId)
{
    ProgramId programId = glCreateProgram();
    assert(programId != 0);
    if (!programId && LOG_OPEN)
    {
        GLchar  errorMsg[256];
        GLsizei errorLength(0);
        // todo: check params valid? and log.
        glGetProgramInfoLog(programId, 256, &errorLength, errorMsg);
        // log
        return 0;
    }
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragShaderId);
    glLinkProgram(programId);
#ifdef _DEBUG
    GLint linkStatus(0);
    glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
    if (GL_FALSE == linkStatus)
    {
        GLchar msg[200];
        glGetShaderInfoLog(programId, sizeof(msg), 0, msg);
        glDeleteProgram(programId);
        programId = 0;
        assert(msg && false);
    }
#endif

    assert(linkStatus == GL_TRUE);
    return programId;
}

NS_END(elloop);