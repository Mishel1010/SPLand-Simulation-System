#include "Settlement.h"

Settlement::Settlement(const string &name, SettlementType type): name(name), type(type) {}

const string &Settlement::getName() const {
    return name;
}

SettlementType Settlement::getType() const {
    return type;
}

const string Settlement::toString() const {
    string ret = name;
    if (type == SettlementType::VILLAGE)
    {
        ret += "is a village";
    }
    else if (type == SettlementType::CITY)
    {
        ret += "is a city";
    }
    else if (type == SettlementType::METROPOLIS)
    {
        ret += "is a metropolis";
    }
}