#include <ros/ros.h>

#include <geometry_msgs/TransformStamped.h>

#include <math.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/PointCloud2.h>

#include <std_msgs/Bool.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Header.h>
#include <std_msgs/UInt16MultiArray.h>

#include <geometry_msgs/PoseStamped.h>

#include <cv_bridge/cv_bridge.h>

#include <image_transport/image_transport.h>
#include <image_transport/subscriber_filter.h>

#include <message_filters/sync_policies/approximate_time.h>
#include <message_filters/sync_policies/exact_time.h>
#include <message_filters/subscriber.h>

// PCL-ROS
#include <pcl_ros/point_cloud.h>
#include <pcl_ros/transforms.h>
#include <pcl_conversions/pcl_conversions.h>

// PCL
#include <pcl/filters/extract_indices.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>

//OpenCV
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Customized message
#include "lib/get_cloud.h"
#include "lib/plane_segment.h"
#include "lib/utilities.h"

//#define DEBUG

using namespace std;

// Publishers

/// Transform frame, only used with real time data
/// You may change the name based on your robot configuration

int main(int argc, char **argv)
{
  // You need to run roscore to launch this program
  ros::init(argc, argv, "hope_ros");

  data_type type;

  type = REAL;
  ROS_INFO("Using real data.");

  ros::NodeHandle nh;
  ros::NodeHandle pnh("~");

  float xy_resolution, z_resolution; // In meter

  cout << "Using threshold: xy@" << xy_resolution 
       << " " << "z@" << z_resolution << endl;

  Params params;

  pnh.getParam("/hope_ros/blob_properties/x_dimension", params.x_dim);
  pnh.getParam("/hope_ros/blob_properties/y_dimension", params.y_dim);
  pnh.getParam("/hope_ros/ground_clearance/min_height", params.z_min);
  pnh.getParam("/hope_ros/ground_clearance/max_height", params.z_max);
  pnh.getParam("/hope_ros/blob_properties/min_surface_area", params.area_min);
  pnh.getParam("/hope_ros/blob_properties/max_surface_area", params.area_max);
  pnh.getParam("/hope_ros/plane_segment/xy_resolution", params.xy_resolution);
  pnh.getParam("/hope_ros/plane_segment/z_resolution", params.z_resolution);
  pnh.getParam("/hope_ros/pointcloud/base_frame", params.base_frame);
  pnh.getParam("/hope_ros/pointcloud/topic_name", params.cloud_topic);

  PlaneSegment hope(params, nh);

  ros::Rate loop_rate(2);
  hope.setMode(type);
  while (ros::ok()) {
    // The src_cloud is actually not used here
    hope.getHorizontalPlanes();
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
