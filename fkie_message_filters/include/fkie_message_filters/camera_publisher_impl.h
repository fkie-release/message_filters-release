/****************************************************************************
 *
 * fkie_message_filters
 * Copyright © 2018-2025 Fraunhofer FKIE
 * Author: Timo Röhling
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
#ifndef INCLUDE_FKIE_MESSAGE_FILTERS_CAMERA_PUBLISHER_IMPL_H_
#define INCLUDE_FKIE_MESSAGE_FILTERS_CAMERA_PUBLISHER_IMPL_H_

#include "camera_publisher.h"
#include "version.h"

namespace fkie_message_filters
{

template<template<typename> class Translate>
CameraPublisher<Translate>::CameraPublisher(rclcpp::Node::SharedPtr& node, const std::string& base_topic,
                                            const rclcpp::QoS& qos, const rclcpp::PublisherOptions& options) noexcept
{
    advertise(node, base_topic, qos, options);
}

template<template<typename> class Translate>
CameraPublisher<Translate>::~CameraPublisher()
{
    shutdown_monitor();
}

template<template<typename> class Translate>
bool CameraPublisher<Translate>::is_active() const noexcept
{
    return pub_.getNumSubscribers() > 0;
}

template<template<typename> class Translate>
std::string CameraPublisher<Translate>::topic() const noexcept
{
    return pub_.getTopic();
}

template<template<typename> class Translate>
void CameraPublisher<Translate>::advertise(rclcpp::Node::SharedPtr& node, const std::string& base_topic,
                                           const rclcpp::QoS& qos, const rclcpp::PublisherOptions& options) noexcept
{
#if RCLCPP_VERSION_MAJOR >= 28
    // ROS jazzy and newer
    pub_ = image_transport::create_camera_publisher(node.get(), base_topic, qos.get_rmw_qos_profile(), options);
#else
    pub_ = image_transport::create_camera_publisher(node.get(), base_topic, qos.get_rmw_qos_profile());
#endif
    start_monitor(node);
    update_subscriber_state();
}

template<template<typename> class Translate>
void CameraPublisher<Translate>::receive(
    helpers::argument_t<typename Translate<sensor_msgs::msg::Image>::FilterType> img,
    helpers::argument_t<typename Translate<sensor_msgs::msg::CameraInfo>::FilterType> info) noexcept
{
    Translate<sensor_msgs::msg::Image>::publish(pub_, img, info);
}

}  // namespace fkie_message_filters

#endif