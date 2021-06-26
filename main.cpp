#include <QGuiApplication>
#include "MCWindow.h"


int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  MC_Window window;
  window.setSample(2);
  window.resize(800, 640);
  window.show();
  
  return app.exec();
}
