#include "visualization.h"
#include <Eigen/Dense>

Visualization::Visualization(unsigned int w, unsigned int h)
{
  pangolin::CreateWindowAndBind("Main", w, h);
  glEnable(GL_DEPTH_TEST);

  s_cam = pangolin::OpenGlRenderState (
        pangolin::ProjectionMatrix(w, h, fu, fv, w/2, h/2, near, far),
        pangolin::ModelViewLookAt(2, 2, 2, 0, 0, 0, pangolin::AxisZ));


}

void Visualization::render()
{
  pangolin::Handler3D handler(s_cam);
  pangolin::View& d_cam = pangolin::CreateDisplay()
      .SetBounds(0, 1.0, 0, 1.0, -640.0f/480.0f)
      .SetHandler(&handler);


  while (!pangolin::ShouldQuit())
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    d_cam.Activate(s_cam);


    for (int c = 0; c < cameras.size(); ++c)
    {
      pangolin::glDrawFrustum<double>(cameras[c], cameras_scale, cameras_scale, cameras_transform[c], 1.0);
    }
    pangolin::glDrawAxis(axis_size);
//  pangolin::glDraw_x0(10, 5);
//  pangolin::glDraw_y0(10, 5);
//  pangolin::glDraw_z0(10, 5);

    pangolin::FinishFrame();
  }
}

void Visualization::add_camera(const Eigen::Matrix3d &Kinv, const Eigen::Matrix4d &T)
{
  cameras.push_back(Kinv);
  cameras_transform.push_back(T);
}
