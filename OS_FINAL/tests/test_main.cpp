#define DOCTEST_CONFIG_IMPLEMENT
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
using namespace doctest;

int return_code = -1;

struct ReporterCounter : public ConsoleReporter
{
    ReporterCounter(const ContextOptions &input_options)
            : ConsoleReporter(input_options) {}

    void test_run_end(const TestRunStats &run_stats) override
    {
        const std::string RESET = "\033[0m";
        const std::string RED = "\033[31m";
        const std::string GREEN = "\033[32m";
        std::cout << "You have " << run_stats.numAsserts << " tests." << std::endl;
        std::cout << GREEN << "Number of tests passed: " << run_stats.numAsserts-run_stats.numAssertsFailed << std::endl;
        std::cout << RED << "Number of tests failed: " << run_stats.numAssertsFailed << RESET << std::endl;
        if (run_stats.numAssertsFailed == 0)
        {
            std::cout << GREEN << "All tests passed!" << RESET << std::endl;
            return_code = 0;
        }
        else
        {
            std::cout << RED << "Some tests failed." << RESET << std::endl;
            return_code = -1;
        }
    }
};

REGISTER_REPORTER("counter", 1, ReporterCounter);

int main()
{
    Context context;
    context.addFilter("reporters", "counter");
    context.run();
    return return_code;
}