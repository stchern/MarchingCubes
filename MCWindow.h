#ifndef MC_WINDOW_H
#define MC_WINDOW_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QtMath>
#include <QKeyEvent>
#include <QVector3D>
#include <QScreen>

#include "OpenGLWindow.h"
#include "marchingCubes.h"

class MC_Window : public OpenGLWindow {
public:

    using OpenGLWindow::OpenGLWindow;
    MC_Window(QWindow *parent = nullptr);
    ~MC_Window(){
        delete vertexBuffer;
        delete normalBuffer;
        delete program;
    }

    void initialize() override;
    void render() override;
    void setSample(int numberOfSample = 1);

private:
    
    QVector3D cameraPos;
    QVector3D cameraFront;
    QVector3D cameraUp;
    QPoint lastPos;

    float yaw;
    float pitch;
    float angleOfView;
    bool firstMouse;
    
    MarchingCubes* mca;
    int nTriangles;
    int nSample;
    QOpenGLBuffer *vertexBuffer;
    QOpenGLBuffer *normalBuffer;
    QOpenGLShaderProgram *program;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;    
    void keyPressEvent(QKeyEvent* event) override;
    
};

#endif