#ifndef OS_FINAL_COLORS_HPP
#define OS_FINAL_COLORS_HPP

#include <string>

struct Colors {

    inline static const std::string RESET = "\033[0m";
    inline static const std::string BLACK = "\033[30m";
    inline static const std::string RED = "\033[31m";
    inline static const std::string GREEN = "\033[32m";
    inline static const std::string YELLOW = "\033[33m";
    inline static const std::string BLUE = "\033[34m";
    inline static const std::string MAGENTA = "\033[35m";
    inline static const std::string CYAN = "\033[36m";
    inline static const std::string LIGHT_CYAN = "\033[96m";
    inline static const std::string WHITE = "\033[97m";

    inline static const std::string RESET_BG = "\033[49m";
    inline static const std::string BLACK_BG = "\033[40m";
    inline static const std::string RED_BG = "\033[41m";
    inline static const std::string GREEN_BG = "\033[42m";
    inline static const std::string YELLOW_BG = "\033[43m";
    inline static const std::string BLUE_BG = "\033[44m";
    inline static const std::string MAGENTA_BG = "\033[45m";
    inline static const std::string CYAN_BG = "\033[46m";

};

#endif //OS_FINAL_COLORS_HPP
