#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"

#include "diagnostic_monitor/node_monitor.hpp"
#include "diagnostic_monitor/topic_monitor.hpp"
#include "diagnostic_monitor/system_monitor.hpp"
#include "diagnostic_monitor/analyzer.hpp"

#include "diagnostic_msgs/msg/diagnostic_array.hpp"
#include "diagnostic_msgs/msg/diagnostic_status.hpp"
#include "diagnostic_msgs/msg/key_value.hpp"

using namespace std::chrono_literals;

class DiagnosticMonitor : public rclcpp::Node
{
public:
    DiagnosticMonitor()
        : Node("diagnostic_monitor")
    {
        RCLCPP_INFO(this->get_logger(), "ROS2 Diagnostic Monitor Started");

        node_monitor_ = std::make_unique<NodeMonitor>(this);
        topic_monitor_ = std::make_unique<TopicMonitor>(this);
        system_monitor_ = std::make_unique<SystemMonitor>(this);

        this->declare_parameter(
            "expected_nodes",
            std::vector<std::string>{});

        this->declare_parameter(
            "expected_topics",
            std::vector<std::string>{});

        std::vector<std::string> expected_nodes;
        std::vector<std::string> expected_topics;

        this->get_parameter(
            "expected_nodes",
            expected_nodes);

        this->get_parameter(
            "expected_topics",
            expected_topics);

        analyzer_ =
            std::make_unique<Analyzer>(
                expected_nodes,
                expected_topics);

        diagnostic_pub_ =
            this->create_publisher<diagnostic_msgs::msg::DiagnosticArray>(
                "/diagnostics", 10);

        timer_ = this->create_wall_timer(
            2s,
            std::bind(&DiagnosticMonitor::timer_callback, this));
    }

private:

    void publishDiagnostics(
        const SystemInfo &system,
        const HealthReport &report)
    {
        diagnostic_msgs::msg::DiagnosticArray array;

        array.header.stamp = this->now();

        diagnostic_msgs::msg::DiagnosticStatus status;

        status.name = "ROS2 Diagnostic Monitor";
        status.hardware_id = "Host PC";

        if (report.level == "HEALTHY")
            status.level = diagnostic_msgs::msg::DiagnosticStatus::OK;
        else if (report.level == "WARNING")
            status.level = diagnostic_msgs::msg::DiagnosticStatus::WARN;
        else
            status.level = diagnostic_msgs::msg::DiagnosticStatus::ERROR;

        status.message = report.level;

        diagnostic_msgs::msg::KeyValue kv;

        kv.key = "CPU";
        kv.value = std::to_string(system.cpu);
        status.values.push_back(kv);

        kv.key = "RAM";
        kv.value = std::to_string(system.ram);
        status.values.push_back(kv);

        kv.key = "Disk";
        kv.value = std::to_string(system.disk);
        status.values.push_back(kv);

        for (const auto &issue : report.issues)
        {
            diagnostic_msgs::msg::KeyValue issue_kv;

            issue_kv.key = "Issue";
            issue_kv.value = issue;

            status.values.push_back(issue_kv);
        }

        array.status.push_back(status);

        diagnostic_pub_->publish(array);
    }

    void timer_callback()
    {
        auto nodes = node_monitor_->getNodes();

        auto topics = topic_monitor_->getTopics();

        auto system = system_monitor_->getSystemInfo();

        auto topic_names =
            topic_monitor_->getTopicNames();

        auto report =
            analyzer_->analyze(
                system.cpu,
                system.ram,
                system.disk,
                nodes,
                topic_names);
        //--------------------------------------------

        RCLCPP_INFO(
            this->get_logger(),
            "\n========================================");

        RCLCPP_INFO(
            this->get_logger(),
            "        ROS2 DIAGNOSTIC MONITOR");

        RCLCPP_INFO(
            this->get_logger(),
            "========================================");

        RCLCPP_INFO(
            this->get_logger(),
            "Nodes  : %zu",
            nodes.size());

        RCLCPP_INFO(
            this->get_logger(),
            "Topics : %zu",
            topics.size());

        RCLCPP_INFO(
            this->get_logger(),
            " ");

        RCLCPP_INFO(
            this->get_logger(),
            "CPU  : %.2f%%",
            system.cpu);

        RCLCPP_INFO(
            this->get_logger(),
            "RAM  : %.2f%%",
            system.ram);

        RCLCPP_INFO(
            this->get_logger(),
            "Disk : %.2f%%",
            system.disk);

        RCLCPP_INFO(
            this->get_logger(),
            " ");

        RCLCPP_INFO(
            this->get_logger(),
            "Status : %s",
            report.level.c_str());

        if (report.issues.empty())
        {
            RCLCPP_INFO(
                this->get_logger(),
                "Issues : None");
        }
        else
        {
            RCLCPP_INFO(
                this->get_logger(),
                "Issues:");

            for (const auto &issue : report.issues)
            {
                RCLCPP_INFO(
                    this->get_logger(),
                    "  • %s",
                    issue.c_str());
            }
        }

        RCLCPP_INFO(
            this->get_logger(),
            "========================================");

        publishDiagnostics(system, report);
    }

    std::unique_ptr<NodeMonitor> node_monitor_;
    std::unique_ptr<TopicMonitor> topic_monitor_;
    std::unique_ptr<SystemMonitor> system_monitor_;

    std::unique_ptr<Analyzer> analyzer_;

    rclcpp::Publisher<diagnostic_msgs::msg::DiagnosticArray>::SharedPtr
        diagnostic_pub_;

    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<DiagnosticMonitor>());

    rclcpp::shutdown();

    return 0;
}
