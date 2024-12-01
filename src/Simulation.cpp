#include "Simulation.h"
#include <fstream>
#include <iostream>
#include <Auxiliary.h>

using namespace std; 

Simulation* backup = nullptr;

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
                Settlement* settlement = new Settlement(arguments[1], (SettlementType)stoi(arguments[2]));
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
                if (!Simulation::isSettlementExists(arguments[1]))
                {
                    cerr << "Error: Settlement does not exist" << endl;
                    continue;
                }
                Settlement settlement = Simulation::getSettlement(arguments[1]);
                if (arguments[2]=="nve")
                {
                    NaiveSelection* naiveSelection = new NaiveSelection();
                    addPlan(settlement, naiveSelection);
                }
                else if (arguments[2]=="bal")
                {
                    BalancedSelection* balancedSelection = new BalancedSelection(0,0,0);
                    addPlan(settlement, balancedSelection);
                }
                else if (arguments[2]=="eco")
                {
                    EconomySelection* economySelection = new EconomySelection();
                    addPlan(settlement, economySelection);
                }
                else if (arguments[2]=="env")
                {
                    SustainabilitySelection* sustainabilitySelection = new SustainabilitySelection();
                    addPlan(settlement, sustainabilitySelection);
                }
                else
                {
                    cerr << "Error: Invalid selection policy" << endl;
                    continue;
                }
            }
        }
    }
}

void Simulation::start() {
    std::cout << "The simulation has started" << std::endl;
    isRunning = true;  
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    if(!(isSettlementExists(settlement.getName()))) 
    {
        cout << "Cannot create plan" << endl;
        return;
    }
    const int currentPlanId = planCounter;
    planCounter++;
    const vector<FacilityType> facilityOptions = facilitiesOptions;
    Plan toAdd(currentPlanId,settlement, selectionPolicy,facilityOptions);
    plans.push_back(toAdd);
}

void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement) {
    if(isSettlementExists(settlement->getName()))
    {
        return false;
    }
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType facility) {
    for(FacilityType facil: facilitiesOptions)
    {
        if (facil.getName()==facility.getName())
        {
            return false;
        }
    }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName) {
    for (const Settlement *settlement : settlements)
    {
        if (settlement->getName() == settlementName)
        {
            return true;
        }
    }
    return false;
}

bool Simulation::isPlanExists(const int planID) {
    for (Plan plan : plans)
    {
        if (plan.getPlanId() == planID)
        {
            return true;
        }
    }
    return false;
}
Settlement &Simulation::getSettlement(const string &settlementName) {
    if (isSettlementExists(settlementName))
    {
        for (Settlement *settlement : settlements)
        {
            if (settlement->getName() == settlementName)
            {
                return *settlement;
            }
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

vector<BaseAction*>& Simulation::getActionsLog() {
    return actionsLog;
}

vector<Plan>& Simulation::getPlans() {
    return plans;
}

void Simulation::step() {
    for(Plan x: plans){
        x.step();
    }
}

void Simulation::close() {
    // for(int i=0; i<plans.size();i++)
    // {
    //     cout << "plan ID: " + std::to_string(plans[i].getPlanId()) << endl; 
    //     cout << "settlementName " + plans[i].getSettlementName() << endl;
    //     cout << "LifeQuality_score " + std::to_string(plans[i].getlifeQualityScore()) << endl;
    //     cout << "Economy_Score " + std::to_string(plans[i].getEconomyScore()) << endl;
    //     cout << "Environment score" + std::to_string(plans[i].getEnvironmentScore()) << endl;
    //     delete(plans[i]);
    //     plans.clear();
    // }
    // for (int i = 0; i< actionsLog.size(); i++)
    // {
    //     delete (actionsLog[i]);
    // }
}

void Simulation::open() {
    throw std::logic_error("Not implemented yet");
}

