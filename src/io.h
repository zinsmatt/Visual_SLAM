#ifndef IO_H
#define IO_H

#include <opencv2/opencv.hpp>


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


#endif // IO_H
