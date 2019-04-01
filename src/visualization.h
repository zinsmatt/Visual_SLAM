#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <pangolin/pangolin.h>
#include <Eigen/Dense>

class Visualization
{

public:
  Visualization(unsigned int w=640, unsigned int h=480);
  void render();

  /// Add a camera to the list of cameras that are rendered
  /// The matrix passed is the K inv of the camera
  void add_camera(Eigen::Matrix3d const& Kinv, Eigen::Matrix4d const& T=Eigen::Matrix4d::Identity());

private:
  pangolin::OpenGlRenderState s_cam;

  double fu = 240, fv = 240;
  double near = 0.1, far = 100.0;
  int cameras_scale = 100;
  double axis_size = 2;

  std::vector<Eigen::Matrix3d> cameras;
  std::vector<Eigen::Matrix4d, Eigen::aligned_allocator<Eigen::Matrix4d> > cameras_transform;

};


#endif // VISUALIZATION_H
