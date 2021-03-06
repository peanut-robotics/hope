#!/usr/bin/env python
"""
Republish cloud onto the topic used by hope
"""

import rospy
import math
import numpy as np

from std_srvs.srv import Trigger, TriggerResponse 
from sensor_msgs.msg import PointCloud2 

from peanut_log import get_nutlog
nutlog = get_nutlog(__name__, __file__)


class HopeTrigger(object):

    def __init__(self):
        if not rospy.has_param("pointcloud/camera_source_topic_name"):
            nutlog.error("Could not find rosparam pointcloud/camera_source_topic_name")
            return
        else:
            self.camera_source_topic_name = rospy.get_param("pointcloud/camera_source_topic_name")

        if not rospy.has_param("pointcloud/topic_name"):        
            nutlog.error("Could not find rosparam pointcloud/topic_name")
            return
        else:
            self.cloud_topic = rospy.get_param("pointcloud/topic_name")

        # Trigger publisher
        self.hope_cloud_pub = rospy.Publisher(self.cloud_topic, PointCloud2, queue_size=1)

        # Trigger service 
        self.trigger_srv = rospy.Service("run_trigger", Trigger, self.trigger_cb)

    def trigger_cb(self, req):
        try:
            self.cloud = rospy.wait_for_message(self.camera_source_topic_name, PointCloud2, timeout=10.0)
        except rospy.exceptions.ROSException:
            nutlog.error("Could not get point cloud msg on topic {}".format(self.camera_source_topic_name))
            return TriggerResponse(success=False)

        self.hope_cloud_pub.publish(self.cloud) 
        return TriggerResponse(success=True) 

if __name__ == '__main__':

    rospy.init_node('hope_trigger', log_level=rospy.INFO)
    try:
        head = HopeTrigger()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass