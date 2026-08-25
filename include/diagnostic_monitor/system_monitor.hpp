#ifndef SYSTEM_MONITOR_HPP
#define SYSTEM_MONITOR_HPP

#include "rclcpp/rclcpp.hpp"
#include "diagnostic_monitor/system_info.hpp"

class SystemMonitor
{
public:
    explicit SystemMonitor(rclcpp::Node *node);

    SystemInfo getSystemInfo();

private:
    float getCpuUsage();
    float getMemoryUsage();
    float getDiskUsage();

    rclcpp::Node *node_;

    long long previousIdle = 0;
    long long previousTotal = 0;
};

#endif
