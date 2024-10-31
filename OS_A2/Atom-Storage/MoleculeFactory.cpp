#ifndef MOLECULEFACTORY_CPP
#define MOLECULEFACTORY_CPP
#include "Molecule.cpp"

class MoleculeFactory {

    static const Molecule WATER;
    static const Molecule CARBON_DIOXIDE;
    static const Molecule ALCOHOL;
    static const Molecule GLUCOSE;

    static const char* SOFT_DRINK[];
    static const char* VODKA[];
    static const char* CHAMPAGNE[];

public:
    static Molecule getMolecule(const char* type) {
        if (strcmp(type, "WATER") == 0) return WATER;
        if (strcmp(type, "CARBON_DIOXIDE") == 0) return CARBON_DIOXIDE;
        if (strcmp(type, "ALCOHOL") == 0) return ALCOHOL;
        if (strcmp(type, "GLUCOSE") == 0) return GLUCOSE;
        throw std::invalid_argument("Unknown molecule type");
    }

    static const char** getDrink(const char* type) {
        if (strcmp(type, "SOFT_DRINK") == 0) return SOFT_DRINK;
        if (strcmp(type, "VODKA") == 0) return VODKA;
        if (strcmp(type, "CHAMPAGNE") == 0) return CHAMPAGNE;
        throw std::invalid_argument("Unknown drink type");
    }

};

const Molecule MoleculeFactory::WATER = 2_H + 1_O;
const Molecule MoleculeFactory::CARBON_DIOXIDE = 1_C + 2_O;
const Molecule MoleculeFactory::ALCOHOL = 1_C + 1_O + 2_H;
const Molecule MoleculeFactory::GLUCOSE = 6_C + 6_O + 12_H;

const char* MoleculeFactory::SOFT_DRINK[] = {"WATER", "CARBON_DIOXIDE", "GLUCOSE"};
const char* MoleculeFactory::VODKA[] = {"WATER", "ALCOHOL", "GLUCOSE"};
const char* MoleculeFactory::CHAMPAGNE[] = {"WATER", "CARBON_DIOXIDE", "ALCOHOL"};

#endif