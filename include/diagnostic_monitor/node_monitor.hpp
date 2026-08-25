#ifndef NODE_MONITOR_HPP
#define NODE_MONITOR_HPP

#include <vector>
#include <string>

#include "rclcpp/rclcpp.hpp"

class NodeMonitor
{
public:
    explicit NodeMonitor(rclcpp::Node *node);

    // Returns all active nodes
    std::vector<std::string> getNodes();

    // Prints all active nodes
    void printNodes();

private:
    rclcpp::Node *node_;
};

#endif
