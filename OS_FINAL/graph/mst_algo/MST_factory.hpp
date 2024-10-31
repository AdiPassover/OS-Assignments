#ifndef OS_FINAL_MST_FACTORY_HPP
#define OS_FINAL_MST_FACTORY_HPP

#include <memory>
#include <stdexcept>
#include "MST_algorithms.hpp"

class MST_factory {
public:
    enum AlgorithmName {
        PRIM,
        KRUSKAL
    };

    inline static AlgorithmName get_algorithm_name(const std::string &algo) {
        if (algo == "prim")return PRIM;
        else if (algo == "kruskal")return KRUSKAL;
        else throw std::invalid_argument("Invalid algorithm name");
    }

    inline static std::unique_ptr<MST_algorithm> get_algorithm(AlgorithmName algo) {
        switch (algo) {
            case PRIM:
                return std::make_unique<Prim>();
            case KRUSKAL:
                return std::make_unique<Kruskal>();
            default:
                return nullptr;
        }
    }

    inline static std::unique_ptr<MST_algorithm> get_algorithm(const std::string& algo) {
        return get_algorithm(get_algorithm_name(algo));
    }

};


#endif //OS_FINAL_MST_FACTORY_HPP
