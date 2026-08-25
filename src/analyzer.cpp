#include "diagnostic_monitor/analyzer.hpp"

Analyzer::Analyzer(
    const std::vector<std::string>& expected_nodes,
    const std::vector<std::string>& expected_topics)
    : expected_nodes_(expected_nodes),
      expected_topics_(expected_topics)
{
}

bool Analyzer::nodeExists(
    const std::vector<std::string>& nodes,
    const std::string& name)
{
    for (const auto& node : nodes)
    {
        if (node == name)
            return true;
    }

    return false;
}

bool Analyzer::topicExists(
    const std::vector<std::string>& topics,
    const std::string& name)
{
    for (const auto& topic : topics)
    {
        if (topic == name)
            return true;
    }

    return false;
}

HealthReport Analyzer::analyze(
    float cpu,
    float ram,
    float disk,
    const std::vector<std::string>& nodes,
    const std::vector<std::string>& topics)
{
    HealthReport report;

    report.level = "HEALTHY";

    //------------------------
    // Critical Checks
    //------------------------

    if (cpu > 90)
    {
        report.level = "CRITICAL";
        report.issues.push_back("CPU usage above 90%");
    }

    if (ram > 90)
    {
        report.level = "CRITICAL";
        report.issues.push_back("RAM usage above 90%");
    }

    if (disk > 95)
    {
        report.level = "CRITICAL";
        report.issues.push_back("Disk usage above 95%");
    }

    //------------------------
    // Warning Checks
    //------------------------

    if (cpu > 75 && cpu <= 90)
    {
        if (report.level == "HEALTHY")
            report.level = "WARNING";

        report.issues.push_back("High CPU usage");
    }

    if (ram > 75 && ram <= 90)
    {
        if (report.level == "HEALTHY")
            report.level = "WARNING";

        report.issues.push_back("High RAM usage");
    }

    if (disk > 80 && disk <= 95)
    {
        if (report.level == "HEALTHY")
            report.level = "WARNING";

        report.issues.push_back("High Disk usage");
    }

    //------------------------
    // Expected Nodes
    //------------------------

    for (const auto& expected : expected_nodes_)
    {
        if (!nodeExists(nodes, expected))
        {
            if (report.level == "HEALTHY")
                report.level = "WARNING";

            report.issues.push_back(
                expected + " node missing");
        }
    }

    //------------------------
    // Expected Topics
    //------------------------

    for (const auto& expected : expected_topics_)
    {
        if (!topicExists(topics, expected))
        {
            if (report.level == "HEALTHY")
                report.level = "WARNING";

            report.issues.push_back(
                expected + " topic missing");
        }
    }

    return report;
}
