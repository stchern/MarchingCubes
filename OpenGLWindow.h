#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>


class OpenGLWindow : public QWindow, protected QOpenGLFunctions {

    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = nullptr);
    ~OpenGLWindow(){
        delete context;
        delete device;
    }

    virtual void render(QPainter *painter);
    virtual void render();
    virtual void initialize();

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent* event) override;
    void exposeEvent(QExposeEvent* event) override;

private:
    QOpenGLContext *context;
    QOpenGLPaintDevice *device;
};


#endif