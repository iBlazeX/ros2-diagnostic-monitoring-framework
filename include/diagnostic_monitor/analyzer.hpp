#ifndef ANALYZER_HPP
#define ANALYZER_HPP

#include <string>
#include <vector>

struct HealthReport
{
    std::string level;
    std::vector<std::string> issues;
};

class Analyzer
{
public:
    Analyzer(
        const std::vector<std::string>& expected_nodes,
        const std::vector<std::string>& expected_topics);

    HealthReport analyze(
        float cpu,
        float ram,
        float disk,
        const std::vector<std::string>& nodes,
        const std::vector<std::string>& topics);

private:
    bool nodeExists(
        const std::vector<std::string>& nodes,
        const std::string& name);

    bool topicExists(
        const std::vector<std::string>& topics,
        const std::string& name);

    std::vector<std::string> expected_nodes_;
    std::vector<std::string> expected_topics_;
};

#endif
