#include <cstdio>
#include <cstring>
#include <stdexcept>

class Molecule {
private:
    unsigned long long carbon;
    unsigned long long oxygen;
    unsigned long long hydrogen;

public:
    Molecule() : carbon(0), oxygen(0), hydrogen(0) {}
    Molecule(unsigned int c, unsigned int o, unsigned int h) : carbon(c), oxygen(o), hydrogen(h) {}

    unsigned long long getCarbon() const { return carbon; }
    unsigned long long getOxygen() const { return oxygen; }
    unsigned long long getHydrogen() const { return hydrogen; }

    void addCarbon(unsigned long long c) { carbon += c; }
    void addOxygen(unsigned long long o) { oxygen += o; }
    void addHydrogen(unsigned long long h) { hydrogen += h; }

    Molecule operator+(const Molecule& other) const {
        return Molecule(
                this->carbon + other.carbon,
                this->oxygen + other.oxygen,
                this->hydrogen + other.hydrogen
        );
    }

    Molecule operator-(const Molecule& other) const {
        return Molecule(
                this->carbon > other.carbon ? this->carbon - other.carbon : 0,
                this->oxygen > other.oxygen ? this->oxygen - other.oxygen : 0,
                this->hydrogen > other.hydrogen ? this->hydrogen - other.hydrogen : 0
        );
    }

    Molecule& operator+=(const Molecule& other) {
        this->carbon += other.carbon;
        this->oxygen += other.oxygen;
        this->hydrogen += other.hydrogen;
        return *this;
    }

    Molecule& operator-=(const Molecule& other) {
        this->carbon = this->carbon > other.carbon ? this->carbon - other.carbon : 0;
        this->oxygen = this->oxygen > other.oxygen ? this->oxygen - other.oxygen : 0;
        this->hydrogen = this->hydrogen > other.hydrogen ? this->hydrogen - other.hydrogen : 0;
        return *this;
    }

    bool operator<=(const Molecule& other) const {
        return this->carbon <= other.carbon && this->oxygen <= other.oxygen && this->hydrogen <= other.hydrogen;
    }
    bool operator>=(const Molecule& other) const {
        return other <= *this;
    }

    Molecule operator*(unsigned long long n) const {
        return Molecule(this->carbon * n, this->oxygen * n, this->hydrogen * n);
    }

    unsigned long long operator/(const Molecule& other) const {
        unsigned long long c = other.carbon != 0 ? this->carbon / other.carbon : -1;
        unsigned long long o = other.oxygen != 0 ? this->oxygen / other.oxygen : -1;
        unsigned long long h = other.hydrogen != 0 ? this->hydrogen / other.hydrogen : -1;
        return std::min(std::min(c, o), h);
    }

    char* toString() const {
        char* str = new char[100];
        sprintf(str, "Carbon: %llu\nOxygen: %llu\nHydrogen: %llu\n", carbon, oxygen, hydrogen);
        return str;
    }

    friend Molecule operator"" _C(unsigned long long c);
    friend Molecule operator"" _O(unsigned long long o);
    friend Molecule operator"" _H(unsigned long long h);

};

Molecule operator"" _C(unsigned long long c) { return Molecule(c, 0, 0); }
Molecule operator"" _O(unsigned long long o) { return Molecule(0, o, 0); }
Molecule operator"" _H(unsigned long long h) { return Molecule(0, 0, h); }