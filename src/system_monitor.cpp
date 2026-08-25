#include "diagnostic_monitor/system_monitor.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <sys/statvfs.h>

SystemMonitor::SystemMonitor(rclcpp::Node *node)
    : node_(node)
{
    std::ifstream file("/proc/stat");

    std::string cpu;
    long long user, nice, system, idle, iowait, irq, softirq, steal;

    file >> cpu
         >> user
         >> nice
         >> system
         >> idle
         >> iowait
         >> irq
         >> softirq
         >> steal;

    previousIdle = idle + iowait;

    previousTotal =
        user +
        nice +
        system +
        idle +
        iowait +
        irq +
        softirq +
        steal;
}

float SystemMonitor::getCpuUsage()
{
    std::ifstream file("/proc/stat");

    std::string cpu;

    long long user;
    long long nice;
    long long system;
    long long idle;
    long long iowait;
    long long irq;
    long long softirq;
    long long steal;

    file >> cpu
         >> user
         >> nice
         >> system
         >> idle
         >> iowait
         >> irq
         >> softirq
         >> steal;

    long long idleTime = idle + iowait;

    long long totalTime =
        user +
        nice +
        system +
        idle +
        iowait +
        irq +
        softirq +
        steal;

    long long totalDelta =
        totalTime - previousTotal;

    long long idleDelta =
        idleTime - previousIdle;

    previousTotal = totalTime;
    previousIdle = idleTime;

    if (totalDelta == 0)
        return 0.0f;

    return
        (float)(totalDelta - idleDelta) /
        totalDelta *
        100.0f;
}

float SystemMonitor::getMemoryUsage()
{
    std::ifstream file("/proc/meminfo");

    std::string label;
    long total = 0;
    long available = 0;
    long value;

    while (file >> label >> value)
    {
        if (label == "MemTotal:")
            total = value;

        if (label == "MemAvailable:")
            available = value;

        file.ignore(256, '\n');
    }

    if (total == 0)
        return 0;

    return ((total - available) * 100.0f) / total;
}

float SystemMonitor::getDiskUsage()
{
    struct statvfs stat;

    if (statvfs("/", &stat) != 0)
        return 0;

    unsigned long total = stat.f_blocks;
    unsigned long free = stat.f_bfree;

    return ((total - free) * 100.0f) / total;
}

SystemInfo SystemMonitor::getSystemInfo()
{
    SystemInfo info;

    info.cpu = getCpuUsage();
    info.ram = getMemoryUsage();
    info.disk = getDiskUsage();

    return info;
}
