#include "Simulation.h"
#include <fstream>
#include <iostream>
#include <Auxiliary.h>

using namespace std; 

Simulation::Simulation(const string &configFilePath) : isRunning(true), planCounter(0) {
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        cerr << "Error: could not open file " << configFilePath << endl;
        return;
    }

    string line;
    while (getline(configFile, line))
    {
        vector<string> arguments = Auxiliary::parseArguments(line);
        if(arguments.empty())
        {
            continue;
        }
        if (arguments[0]=="Settlement")
        {
            if (arguments.size() != 3)
            {
                cerr << "Error: invalid Settlement configuration" << endl;
                continue;
            }
            else
            {
                Settlement settlement(arguments[1], (SettlementType)stoi(arguments[2]));
                addSettlement(settlement);
            }
        }
        else if (arguments[0]=="Facility")
        {
            if (arguments.size()!=7)
            {
                cerr << "Error: invalid Facility configuration" << endl;
                continue;
            }
            else
            {
                FacilityType facility(arguments[1], (FacilityCategory)stoi(arguments[2]), stoi(arguments[3]), stoi(arguments[4]), stoi(arguments[5]), stoi(arguments[6]));
                addFacility(facility);
            }
        }
        else if (arguments[0]=="Plan")
        {
            if (arguments.size()!=3)
            {
                cerr << "Error: invalid Plan configuration" << endl;
                continue;
            }
            else 
            {
                throw std::logic_error("Not implemented yet");
            }
        }
    }
}

void Simulation::start() {
    std::cout << "The simulation has started" << std::endl;
    while (isRunning)
    {
        step();
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    throw std::logic_error("Not implemented yet");
}

void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement settlement) {
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType facility) {
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName) {
    for (const Settlement &settlement : settlements)
    {
        if (settlement.getName() == settlementName)
        {
            return true;
        }
    }
    return false;
}

Settlement &Simulation::getSettlement(const string &settlementName) {
    for (Settlement &settlement : settlements)
    {
        if (settlement.getName() == settlementName)
        {
            return settlement;
        }
    }
    throw std::logic_error("Settlement not found");
}

Plan &Simulation::getPlan(const int planID) {
    for (Plan plan : plans)
    {
        if (plan.getPlanId() == planID)
        {
            return plan;
        }
    }
    throw std::logic_error("Plan not found");
}

void Simulation::step() {
    throw std::logic_error("Not implemented yet");
}

void Simulation::close() {
    throw std::logic_error("Not implemented yet");
}

void Simulation::open() {
    throw std::logic_error("Not implemented yet");
}

