#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/msg/transform_stamped.hpp>

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("tf2_listener");

    tf2_ros::Buffer tf_buffer;
    tf2_ros::TransformListener tf_listener(tf_buffer, node);

    rclcpp::Rate rate(10); // Update rate in Hz

    std::string target_frame = "target_frame";
    std::string source_frame = "source_frame";

    while (rclcpp::ok()) {
        try {
            // Check if the transformation between "target_frame" and "source_frame" is available
            if (tf_buffer.canTransform(target_frame, source_frame, tf2::TimePointZero)) {
                geometry_msgs::msg::TransformStamped transform =
                    tf_buffer.lookupTransform(target_frame, source_frame, tf2::TimePointZero);

                RCLCPP_INFO(node->get_logger(),
                            "Transformation:\nTranslation: [%f, %f, %f]\nRotation: [%f, %f, %f, %f]",
                            transform.transform.translation.x,
                            transform.transform.translation.y,
                            transform.transform.translation.z,
                            transform.transform.rotation.x,
                            transform.transform.rotation.y,
                            transform.transform.rotation.z,
                            transform.transform.rotation.w);

                // Break the loop once the transform is available
                break;
            } else {
                RCLCPP_WARN(node->get_logger(), "Transform not available yet. Waiting...");
            }

        } catch (tf2::TransformException &ex) {
            RCLCPP_WARN(node->get_logger(), "Transform lookup/wait failed: %s", ex.what());
        }

        rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}
