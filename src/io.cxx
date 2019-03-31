#include "io.h"
#include <fstream>

Frame_provider::Frame_provider(const std::string &filename)
{
    std::ifstream in(filename);
    if (!in.is_open())
    {
        std::cerr << "Could not load frames" << std::endl;
        return;
    }

    std::string directory;
    in >> directory;
    if (directory[directory.size()-1] != '/')
      directory += "/";
    while (!in.eof())
    {
        std::string frame;
        in >> frame;
        if (frame.size() > 0)
          frames.push_back(directory + frame);
    }
}

cv::Mat Frame_provider::next_frame(int mode)
{
  if (cur_index == frames.size() - 1) {
    std::cerr << "End of frames" << std::endl;
    return cv::Mat();
  } else {
    return cv::imread(frames[++cur_index], mode);
  }
}

void write_obj(const std::string &filename, const std::vector<Eigen::Vector3d> &points)
{
  std::ofstream file(filename);
  for (Eigen::Vector3d const& p: points)
  {
    file << "v " << p.x() << " " << p.y() << " " << p.z() << "\n";
  }
  file.close();
}

void write_points_time_csv(const std::string &filename, const std::vector<Eigen::Vector3d> &points, std::vector<double> times)
{
  std::ofstream file(filename);
  file << "X, Y, Z, time\n";
  int i = 0;
  for (Eigen::Vector3d const& p: points)
  {
    file << p.x() << ", " << p.y() << ", " << p.z() << ", " << times[i] << "\n";
    ++i;
  }
  file.close();
}
