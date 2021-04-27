#ifndef MC_WINDOW_H
#define MC_WINDOW_H

#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector3D>
#include <QScreen>
#include "OpenGLWindow.h"
#include "marchingCubes.h"

class MC_Window : public OpenGLWindow {
public:
    using OpenGLWindow::OpenGLWindow;

    void initialize() override;
    void render() override;

private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_color = 0;
    GLint m_matrixUniform = 0;

    QOpenGLShaderProgram *m_program;
    MC_Window* mca;
    int m_frame = 0;
    
};

#endif