#include "Action.h" 
#include "Auxiliary.h"
#include "Plan.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "Facility.h"
#include "Simulation.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

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

Simulation::Simulation(Simulation&& other)
:   isRunning(other.isRunning),
    planCounter(other.planCounter),
    actionsLog(std::move(other.actionsLog)),
    plans(std::move(other.plans)),
    settlements(std::move(other.settlements)),
    facilitiesOptions(std::move(other.facilitiesOptions)) {}

Simulation::Simulation(Simulation& other)
:   isRunning(other.isRunning),
    planCounter(other.planCounter),
    plans(other.plans),
    facilitiesOptions(other.facilitiesOptions) {
        for (BaseAction* ptr : other.actionsLog)
        {
            this->actionsLog.push_back(ptr->clone());
        }
        for(Settlement* q: other.settlements)
        {
            this->settlements.push_back (new Settlement(q->getName(), q->getType()));
        }
}

Simulation& Simulation::operator=(Simulation&& other) {
    if (this != &other)
    {
        for (BaseAction* ptr : actionsLog)
        {
            delete ptr;
        }
        for (Settlement* ptr : settlements)
        {
            delete ptr;
        }
        actionsLog = std::move(other.actionsLog);
        plans = std::move(other.plans);
        settlements = std::move(other.settlements);
        facilitiesOptions = std::move(other.facilitiesOptions);
        isRunning = other.isRunning;
        planCounter = other.planCounter;
    }
    return *this;
}

   Simulation& Simulation::operator=(Simulation& other){
    if(this != &other){
    plans = other.plans;
    facilitiesOptions = other.facilitiesOptions;
    isRunning = other.isRunning;
    planCounter = other.planCounter;
    for (BaseAction* ptr : actionsLog){
        delete ptr;
    }
    for (BaseAction* ptr: other.actionsLog){
        actionsLog.push_back(ptr->clone());
    }
    for(Settlement* ptr : settlements){
        delete ptr;
    }

    for(Settlement* ptr : other.settlements){
        settlements.push_back(new Settlement(ptr->getName(), ptr->getType() ));
    }}
    return *this;
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
    for (Plan& plan : plans)
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
    for(size_t i=0; i<plans.size();i++)
    {
        cout << "plan ID: " + std::to_string(plans[i].getPlanId()) << endl; 
        cout << "settlementName " + plans[i].getSettlementName() << endl;
        cout << "LifeQuality_score " + std::to_string(plans[i].getlifeQualityScore()) << endl;
        cout << "Economy_Score " + std::to_string(plans[i].getEconomyScore()) << endl;
        cout << "Environment score" + std::to_string(plans[i].getEnvironmentScore()) << endl;
    }
    for (BaseAction* action : actionsLog)
    {
        delete action;
    }
    for (Settlement* settlement : settlements)
    {
        delete settlement;
    }
    isRunning = false;
}

void Simulation::open() {
    throw std::logic_error("Not implemented yet");
}



