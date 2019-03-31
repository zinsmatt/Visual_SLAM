#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <pangolin/pangolin.h>

class Visualization
{

public:
  Visualization(unsigned int w=640, unsigned int h=480);
  void render();

private:
  pangolin::OpenGlRenderState s_cam;

};


#endif // VISUALIZATION_H
