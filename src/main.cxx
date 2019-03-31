#include <iostream>

#include "io.h"

double dist(cv::Point2f const& a, cv::Point2f const& b)
{
  return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

int main()
{

  const double MAX_MATCH_KEYPOINT_DIST = 50.0;

  std::cout << "LOSC\n";

  Frame_provider fp("/home/matt/dev/RGBD/data/rgbd_dataset_freiburg1_room/list_rgb_frames.txt");

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

      std::vector<cv::Point2f> prev_pts, pts;
      for (auto const& m : matches)
      {
        auto pt_prev = prev_kp[m.trainIdx].pt;
        auto pt = keypoints[m.queryIdx].pt;
        if (dist(pt_prev, pt) < MAX_MATCH_KEYPOINT_DIST)
        {
          //cv::line(frame, pt_prev, pt, cv::Scalar(255, 0, 0));
          prev_pts.push_back(pt_prev);
          pts.push_back(pt);
        }
      }
      std::vector<unsigned char> fund_mat_mask;
      cv::Mat F = cv::findFundamentalMat(prev_pts, pts, cv::FM_RANSAC, 3.0, 0.99, fund_mat_mask);
      for (int i = 0; i < prev_pts.size(); ++i)
      {
        if (fund_mat_mask[i] == 1)
        {
          cv::line(frame, prev_pts[i], pts[i], cv::Scalar(255, 0, 0));
        }
      }
    }

    cv::drawKeypoints(frame, keypoints, frame, cv::Scalar(0, 255, 0));
    cv::namedWindow("win", cv::WINDOW_NORMAL);
    cv::imshow("win", frame);
    cv::waitKey(100);

    prev_desc = desc;
    prev_kp = keypoints;
  }
  return 0;
}
