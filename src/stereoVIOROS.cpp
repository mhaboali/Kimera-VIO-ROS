/* @file   stereoVIOKitti.cpp
 * @brief  example of VIO pipeline running on the KITTI dataset
 * @author Yun Chang based off stereoVIOEuroc.cpp
 */

// Still need gflags for parameters in VIO
#include <gflags/gflags.h>
#include <glog/logging.h>

// Dependencies from ROS 
#include <ros/ros.h>

// Dependencies from VIO
#include "utils/Timer.h"
#include "LoggerMatlab.h"

// Dependencies from this repository 
#include "RosDataSource.h"

////////////////////////////////////////////////////////////////////////////////
// stereoVIOexample using ROS wrapper example
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
  // Initialize ROS node
  ros::init(argc, argv, "spark_vio");

  // Initialize Google's flags library.
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);

  // Parse topic names from parameter server
  ros::NodeHandle nh; 
  std::string left_camera_topic, right_camera_topic, imu_topic; 
  nh.getParam("left_camera_topic", left_camera_topic);
  nh.getParam("right_camera_topic", right_camera_topic); 
  nh.getParam("imu_topic", imu_topic); 

  VIO::RosDataProvider ros_wrapper(left_camera_topic, right_camera_topic, imu_topic);

  // Spin dataset.
  auto tic = VIO::utils::Timer::tic();
  const bool is_pipeline_successful = ros_wrapper.spin();

  auto spin_duration = VIO::utils::Timer::toc(tic);
  LOG(WARNING) << "Spin took: " << spin_duration.count() << " ms.";

  if (is_pipeline_successful) {
    // Log overall time of pipeline run.
    VIO::LoggerMatlab logger;
    logger.openLogFiles(11);
    logger.logPipelineOverallTiming(spin_duration);
    logger.closeLogFiles();
  }

  return is_pipeline_successful? EXIT_SUCCESS : EXIT_FAILURE;
}
