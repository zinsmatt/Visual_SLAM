#include <iostream>

#include "io.h"

int main()
{

  std::cout << "LOSC\n";

  Frame_provider fp("/home/matthieu/dev/RGBD/data/rgbd_dataset_freiburg1_room/list_rgb_frames.txt");


  cv::Ptr<cv::Feature2D> detector = cv::ORB::create();
  cv::Ptr<cv::BFMatcher> matcher =  cv::BFMatcher::create(cv::NORM_HAMMING, true);

  cv::Mat prev_desc;
  std::vector<cv::KeyPoint> prev_kp;
  while (!fp.eof())
  {
    cv::Mat frame = fp.next_frame();
    if (frame.rows == 0 || frame.cols == 0)
      continue;

    std::vector<cv::KeyPoint> keypoints;
    cv::Mat desc;
    detector->detectAndCompute(frame, cv::Mat(), keypoints, desc);

    if (prev_desc.rows > 0)
    {
      //  can do matching
      std::vector<cv::DMatch> matches;
      matcher->match(desc, prev_desc, matches);
      for (auto const& m : matches)
      {
        cv::line(frame, prev_kp[m.trainIdx].pt, keypoints[m.queryIdx].pt, cv::Scalar(255, 0, 0));
      }
    }

    cv::drawKeypoints(frame, keypoints, frame, cv::Scalar(0, 255, 0));
    cv::namedWindow("win", cv::WINDOW_NORMAL);
    cv::imshow("win", frame);
    cv::waitKey(100);

    prev_desc = desc;
    prev_kp = std::move(keypoints);
  }
  return 0;
}
