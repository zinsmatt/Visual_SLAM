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
    return cv::Mat();
  } else {
    return cv::imread(frames[++cur_index], mode);
  }
}
