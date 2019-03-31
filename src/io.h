#ifndef IO_H
#define IO_H

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

class Frame_provider
{

public:
    Frame_provider(const std::string& filename);
    cv::Mat next_frame(int mode = cv::IMREAD_UNCHANGED);
    bool eof() const { return cur_index == frames.size() - 1; }

private:
    std::vector<std::string> frames;
    int cur_index = -1;
};

void write_obj(std::string const& filename, std::vector<Eigen::Vector3d> const& points);
void write_points_time_csv(std::string const& filename, std::vector<Eigen::Vector3d> const& points, std::vector<double> times);
#endif // IO_H
