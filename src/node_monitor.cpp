#include "diagnostic_monitor/node_monitor.hpp"

NodeMonitor::NodeMonitor(rclcpp::Node *node)
    : node_(node)
{
}

std::vector<std::string> NodeMonitor::getNodes()
{
    return node_->get_node_names();
}
