#include "MCWindow.h"

const char *vShaderSrc =
    "#version 330\n"
    "uniform vec3 LightPos;\n"
    "uniform vec3 LightColor;\n"
    "uniform mat3 matrix;\n"

    "out vec3 LightColor;\n"

    "void main() {\n"
        "vec3 normal;\n"
        "vec3 pos;\n"
        "martix = gl_ModelViewProjectionMatrix;\n"
        "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
        "normal = normalize(gl_NormalMatrix * gl_Normal);\n"
        "pos = vec3(gl_ModelViewMatrix * gl_Vertex);\n"

        "vec3 toLight = LightPos-pos;\n"
        "LightColor  = max(dot(normalize(toLight),normal),0.0) * LightColor;\n"
        
    "}\n";

const char *fShaderSrc =
    "#version 330\n"
    "in vec3 LightColor;\n"
    "uniform vec3 Color;\n"
    "void main() {\n"
        "gl_FragColor = vec4(Color * LightColor,1.0);\n"
    "}\n";


void MC_Window::initialize()
{

    QVector3D   Color(1.0,1.0,1.0);
    QVector3D   LightColor(1.0,0.3,0.0);
    QVector3D   LightPos(+10.0,+10.0,+10.0);

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vShaderSrc);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fShaderSrc);
    m_program->link();
    m_posAttr = m_program->uniformLocation("LightPos");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = m_program->uniformLocation("LightColor");
    Q_ASSERT(m_colAttr != -1);
    m_color = m_program->uniformLocation("Color");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
    
    m_program->setUniformValue(m_color,Color);
    m_program->setUniformValue(m_colAttr,LightColor);
    m_program->setUniformValue(m_posAttr,LightPos);

    mca->vMarhingCube();
    
}

void MC_Window::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);
    matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    static const GLfloat vertices[] = {
         0.0f,  0.707f,
        -0.5f, -0.5f,
         0.5f, -0.5f
    };

    static const GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);

    m_program->release();

    ++m_frame;
}