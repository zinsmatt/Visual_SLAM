#include <iostream>
#include <Eigen/Dense>
#include "io.h"
#include "visualization.h"

double dist(cv::Point2f const& a, cv::Point2f const& b)
{
  return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

int main()
{

  Visualization visu;
  Eigen::Matrix3d Kk, Kkinv;
  Kk << 517.3, 0.0, 318.6, 0.0, 516.5, 255.3, 0.0, 0.0, 1.0;
  Kkinv = Kk.inverse();

  Eigen::Matrix3d current_rot = Eigen::Matrix3d::Identity();
  Eigen::Vector3d current_pos = Eigen::Vector3d::Zero();
  Eigen::Matrix4d temp;
  temp << current_rot, current_pos, 0, 0, 0, 1;
  visu.add_camera(Kkinv, temp);

//  return 0;


  const double MAX_MATCH_KEYPOINT_DIST = 50.0;

  std::cout << "LOSC\n";

  Frame_provider fp("/home/matt/dev/RGBD/data/rgbd_dataset_freiburg1_room/list_rgb_frames.txt");

  cv::Ptr<cv::Feature2D> detector = cv::ORB::create();
  cv::Ptr<cv::BFMatcher> matcher =  cv::BFMatcher::create(cv::NORM_HAMMING, true);

  cv::Mat K(3, 3, CV_64F);
  K.at<double>(0, 0) = 517.3; K.at<double>(0, 1) = 0.0; K.at<double>(0, 2) = 318.6;
  K.at<double>(1, 0) = 0.0; K.at<double>(1, 1) = 516.5; K.at<double>(1, 2) = 255.3;
  K.at<double>(2, 0) = 0.0; K.at<double>(2, 1) = 0.0; K.at<double>(2, 2) = 1.0;

  std::vector<Eigen::Vector3d> positions;
  std::vector<double> times;
  positions.push_back(Eigen::Vector3d::Zero());
  times.push_back(0.0);

  cv::Mat prev_desc;
  std::vector<cv::KeyPoint> prev_kp;
  double time = 0.0;
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
      std::vector<unsigned char> essential_matrix_mask;
      cv::Mat E = cv::findEssentialMat(prev_pts, pts, K, cv::RANSAC, 0.999, 1.0, essential_matrix_mask);
//      std::vector<unsigned char> fund_mat_mask;
//      cv::Mat F = cv::findFundamentalMat(prev_pts, pts, cv::FM_RANSAC, 3.0, 0.99, fund_mat_mask);
      cv::Mat R, t;
      cv::Mat chierality_mask;
      cv::recoverPose(E, prev_pts, pts, K, R, t, chierality_mask);
      Eigen::Vector3d prev_pos = positions.back();
      Eigen::Vector3d translation(t.at<double>(0, 0), t.at<double>(0, 1), t.at<double>(0, 2));
      Eigen::Matrix3d rotation;
      rotation << R.at<double>(0, 0), R.at<double>(0, 1), R.at<double>(0, 2), R.at<double>(1, 0), R.at<double>(1, 1), R.at<double>(1, 2),
                  R.at<double>(2, 0), R.at<double>(2, 1), R.at<double>(2, 2);
      positions.push_back(prev_pos + translation);
      times.push_back(time);

      temp << rotation * current_rot,  current_pos + translation, 0, 0, 0, 1;
      visu.add_camera(Kkinv, temp);

      for (int i = 0; i < prev_pts.size(); ++i)
      {
        if (essential_matrix_mask[i] == 1)
        {
          cv::line(frame, prev_pts[i], pts[i], cv::Scalar(255, 0, 0));
        }
      }
    }

    cv::drawKeypoints(frame, keypoints, frame, cv::Scalar(0, 255, 0));
    cv::namedWindow("win");
    cv::imshow("win", frame);
    cv::waitKey(20);

    prev_desc = desc;
    prev_kp = keypoints;
    time += 1.0;
  }


  write_points_time_csv("trajectory.csv", positions, times);
  write_obj("trajectory.obj", positions);


  visu.render();

  return 0;
}
