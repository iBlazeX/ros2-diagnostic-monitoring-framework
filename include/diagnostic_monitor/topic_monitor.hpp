#ifndef TOPIC_MONITOR_HPP
#define TOPIC_MONITOR_HPP

#include <map>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"

class TopicMonitor
{
public:
    explicit TopicMonitor(rclcpp::Node *node);

    std::map<std::string, std::vector<std::string>>
    getTopics();
    std::vector<std::string> getTopicNames();

private:
    rclcpp::Node *node_;
};

#endif
