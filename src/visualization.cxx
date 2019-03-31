#include "visualization.h"
#include <Eigen/Dense>

Visualization::Visualization(unsigned int w, unsigned int h)
{
  pangolin::CreateWindowAndBind("Main", w, h);
  glEnable(GL_DEPTH_TEST);

  s_cam = pangolin::OpenGlRenderState (
        pangolin::ProjectionMatrix(w, h, 420, 420, w/2, h/2, 0.1, 100),
        pangolin::ModelViewLookAt(2, 2, 2, 0, 0, 0, pangolin::AxisY));


}

void Visualization::render()
{
  pangolin::Handler3D handler(s_cam);
  pangolin::View& d_cam = pangolin::CreateDisplay()
      .SetBounds(0, 1.0, 0, 1.0, -640.0f/480.0f)
      .SetHandler(&handler);

  Eigen::Matrix3d K;
  K << 517.3, 0.0, 318.6, 0.0, 516.5, 255.3, 0.0, 0.0, 1.0;
  Eigen::Matrix3d K_inv = K.inverse();
  while (!pangolin::ShouldQuit())
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    d_cam.Activate(s_cam);

    pangolin::glDrawFrustum(K_inv, 1, 1, 1.0);


    pangolin::FinishFrame();
  }
}
