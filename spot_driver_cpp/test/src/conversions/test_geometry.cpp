// Copyright (c) 2023 Boston Dynamics AI Institute LLC. All rights reserved.

#include <gmock/gmock.h>

#include <bosdyn/math/proto_math.h>
#include <spot_driver_cpp/conversions/geometry.hpp>

namespace {
bool arePositionsEqual(const geometry_msgs::msg::Vector3 ros, const ::bosdyn::api::Vec3 proto) {
  return ros.x == proto.x() && ros.y == proto.y() && ros.z == proto.z();
}

bool areRotationsEqual(const geometry_msgs::msg::Quaternion ros, const ::bosdyn::api::Quaternion proto) {
  return ros.x == proto.x() && ros.y == proto.y() && ros.z == proto.z() && ros.w == proto.w();
}

bool areTransformsEqual(const geometry_msgs::msg::Transform tf1, const ::bosdyn::api::SE3Pose tf2) {
  return areRotationsEqual(tf1.rotation, tf2.rotation()) && arePositionsEqual(tf1.position, tf2.position());
}
}  // namespace

namespace spot_ros2::conversions::test {

TEST(GeometryConversion, toRosTranslation) {
  // GIVEN a bosdyn position
  const auto position = ::bosdyn::api::CreateVec3(1, 2, 3);

  // WHEN converted to a ROS position msg
  const auto ros_position = toRosTranslation(q);

  // THEN expect them to be equal
  EXPECT_TRUE(areTranslationsEqual(ros_position, position));
}

TEST(GeometryConversion, toRosRotation) {
  // GIVEN a bosdyn quaternion
  const auto q = ::bosdyn::api::CreateQuaternion(1, 2, 3, 4);

  // WHEN converted to a ROS quaternion msg
  const auto ros_q = toRosRotation(q);

  // THEN expect them to be equal
  EXPECT_TRUE(areRotationsEqual(ros_q, q));
}

TEST(GeometryConversion, toTransformStampedTest) {
  // GIVEN a bosdyn protobuf transform, the parent frame name, the child frame name, and a timestamp
  const auto transform =
      ::bosdyn::api::CreateSE3Pose(::bosdyn::api::CreateQuaternion(1, 2, 3, 4), ::bosdyn::api::CreateVec3(5, 6.7));
  const auto parent_frame_name = "Parent";
  const auto child_frame_name = "Child";
  const auto tf_time = builtin_interfaces::build<::builtin_interfaces::msg::Time>().sec(10).nanosec(11);

  // WHEN generating a ROS TransformStamped msg
  const auto msg = toTransformStamped(old_transform, parent_frame_name, child_frame_name, tf_time);

  // THEN expect the correct values
  EXPECT_EQ(msg.header.frame_id, parent_frame_name);
  EXPECT_EQ(msg.child_frame_id, child_frame_name);
  EXPECT_EQ(msg.header.stamp, tf_time);
  EXPECT_TRUE(areTransformsEqual(msg.transform, transform));
}

}  // namespace spot_ros2::conversions::test
