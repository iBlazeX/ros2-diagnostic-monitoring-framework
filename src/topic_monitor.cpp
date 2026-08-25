#include "diagnostic_monitor/topic_monitor.hpp"

TopicMonitor::TopicMonitor(rclcpp::Node *node)
    : node_(node)
{
}

std::map<std::string, std::vector<std::string>>
TopicMonitor::getTopics()
{
    return node_->get_topic_names_and_types();
}

std::vector<std::string> TopicMonitor::getTopicNames()
{
    std::vector<std::string> names;

    auto topics = getTopics();

    for (const auto& topic : topics)
        names.push_back(topic.first);

    return names;
}
