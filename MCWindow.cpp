#include "MCWindow.h"
#include <QDebug>


MC_Window::MC_Window(QWindow *parent): OpenGLWindow(parent), 
    angleOfView(45), firstMouse(true), yaw(-90.0),pitch(0.0), nSample(1){
    
    cameraPos = {0.0, 0.0, 4.0};
    cameraFront = {0.0, 0.0,-1.0};
    cameraUp = {0.0, 1.0, 0.0};

}

void MC_Window::initialize()
{   
    mca = new MarchingCubes(100);
    mca->createObjFile(true);
    mca->marchingCubes(nSample);
    
    nTriangles = mca->vertices.size()/3;
    int dataSize = mca->vertices.size() * sizeof(float);

    vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if (!vertexBuffer->create())
        qFatal("Failed to create vertex buffer");
    if (!vertexBuffer->bind())
        qFatal("Failed to bind vertex buffer");
    vertexBuffer->allocate(mca->vertices.constData(), dataSize);
    vertexBuffer->release();

    normalBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if (!normalBuffer->create())
        qFatal("Failed to create normal buffer");
    if (!normalBuffer->bind())
        qFatal("Failed to bind normal buffer");
    normalBuffer->allocate(mca->normals.constData(), dataSize);
    normalBuffer->release();

    QString vertexShaderFileName = "./shaders/vertexShader.vert";
    QString fragmentShaderFileName = "./shaders/fragmentShader.frag";
    program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFileName);
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFileName);
    program->link();
    Q_ASSERT(program != nullptr);
}

void MC_Window::render()
{
    float aspectRatio = float(width())/float(height());
    glViewport(0, 0, width() , height() );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program->bind();

    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
       
    float near = 0.1;
    float far = 100;
    
    float top = near * tan(qDegreesToRadians(angleOfView)/ 2);
    float bottom = -top;
    float right = top * aspectRatio;
    float left = -top * aspectRatio;
    
    QMatrix4x4 projectionMatrix;  
    projectionMatrix.frustum(left, right, bottom, top, near, far);
    
    QMatrix4x4 normalMatrix = viewMatrix.inverted().transposed();
   
    bool isBound = program->bind();
    Q_ASSERT(isBound);
    program->setUniformValue("modelViewMatrix", viewMatrix);
    program->setUniformValue("normalMatrix", normalMatrix);
    program->setUniformValue("projectionMatrix", projectionMatrix);

    isBound = vertexBuffer->bind();
    Q_ASSERT(isBound);
    program->setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 0);
    program->enableAttributeArray("vertex");
    vertexBuffer->release();

    isBound = normalBuffer->bind();
    Q_ASSERT(isBound);
    program->setAttributeBuffer("normal", GL_FLOAT, 0, 3, 0);
    program->enableAttributeArray("normal");
    normalBuffer->release();
    
    glEnable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, nTriangles);
    if (!nTriangles)
        qDebug()<<"There are no actual triangles"; 
    program->disableAttributeArray("vertex");
    program->disableAttributeArray("normal");
   
    program->release();

}

void MC_Window::setSample(int numberOfSample){
    nSample = numberOfSample;
}

void MC_Window::keyPressEvent(QKeyEvent *event){
  
    int key = event->key();
    float cameraSpeed = 0.4;

    if  (key == Qt::Key_Right) {

        QVector3D vectorRight = QVector3D::crossProduct(cameraFront, cameraUp);
        vectorRight.normalize();
        cameraPos -= vectorRight*cameraSpeed;
    }
    
    if (key == Qt::Key_Left) {

        QVector3D vectorRight = QVector3D::crossProduct(cameraFront, cameraUp);
        vectorRight.normalize();
        cameraPos += vectorRight*cameraSpeed;
    }
    
    if (key == Qt::Key_Up){
        cameraPos -= cameraFront*cameraSpeed; 
    }
    
    if (key == Qt::Key_Down){
        cameraPos += cameraFront*cameraSpeed;
    }
    
    renderNow();

}
void MC_Window::mousePressEvent(QMouseEvent *event) {
    lastPos = event->pos();
}

void MC_Window::mouseMoveEvent(QMouseEvent *event){
    
    if(firstMouse){
        lastPos = event->pos();
        firstMouse = false;
    }
   
    float xoffset = event->x() - lastPos.x();
    float yoffset = lastPos.y() - event->y(); 
    lastPos = event->pos();

    const float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;  
    
    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    
    QVector3D direction;
    direction.setX(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    direction.setY(sin(qDegreesToRadians(pitch)));
    direction.setZ(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    direction.normalize();
    cameraFront = direction;

    renderNow();
}

