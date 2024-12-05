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
            cout << "Error: invalid configuration line" << endl;
            continue;
        }
        if (arguments[0]=="settlement")
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
                cout << "Settlement created: " << arguments[1] << ", Type: " << arguments[2] << endl;
            }
        }
        else if (arguments[0]=="facility")
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
                cout << "Facility created: " << arguments[1] << ", Category: " << arguments[2] << ", Price: " << arguments[3] << ", Life Quality Score: " << arguments[4] << ", Economy Score: " << arguments[5] << ", Environment Score: " << arguments[6] << endl;
            }
        }
        else if (arguments[0]=="plan")
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
                if (arguments[2]=="nve")
                {
                    NaiveSelection* naiveSelection = new NaiveSelection();
                    addPlan(Simulation::getSettlement(arguments[1]), naiveSelection);
                    cout << "Plan created: " << arguments[1] << ", Type: " << arguments[2] << endl;
                }
                else if (arguments[2]=="bal")
                {
                    BalancedSelection* balancedSelection = new BalancedSelection(0,0,0);
                    addPlan(Simulation::getSettlement(arguments[1]), balancedSelection);
                    cout << "Plan created: " << arguments[1] << ", Type: " << arguments[2] << endl;
                }
                else if (arguments[2]=="eco")
                {
                    EconomySelection* economySelection = new EconomySelection();
                    addPlan(Simulation::getSettlement(arguments[1]), economySelection);
                    cout << "Plan created: " << arguments[1] << ", Type: " << arguments[2] << endl;
                }
                else if (arguments[2]=="env")
                {
                    SustainabilitySelection* sustainabilitySelection = new SustainabilitySelection();
                    addPlan(Simulation::getSettlement(arguments[1]), sustainabilitySelection);
                    cout << "Plan created: " << arguments[1] << ", Type: " << arguments[2] << endl;
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
            this->settlements.push_back(new Settlement(q->getName(), q->getType()));
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
    if(this != &other)
    {
        plans = other.plans;
        facilitiesOptions = other.facilitiesOptions;
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        for (BaseAction* ptr : actionsLog)
        {
            delete ptr;
        }
        actionsLog.clear();
        for (BaseAction* ptr: other.actionsLog)
        {
            actionsLog.push_back(ptr->clone());
        }
        for(Settlement* ptr : settlements)
        {
            delete ptr;
        }
        settlements.clear();
        for(Settlement* ptr : other.settlements)
        {
            settlements.push_back(new Settlement(ptr->getName(), ptr->getType() ));
        }
    }
    return *this;
}

void Simulation::start() {
    cout << "The simulation has started" << std::endl;
    open();
    while (isRunning) 
    {
        cout << "Enter an action you'd like to perform: ";
        string command;
        getline(cin, command);
        istringstream iss(command);
        string action;
        iss >> action;
        cout << action << endl;
        if (action == "step")
        {
            int numOfSteps;
            iss >> numOfSteps;
            const int steps = numOfSteps;
            SimulateStep *simulateStep = new SimulateStep(steps);
            simulateStep->act(*this);
            addAction(simulateStep);
        }
        if (action == "plan")
        {
            string settlementName, selectionPolicy;
            iss >> settlementName >> selectionPolicy;
            AddPlan* addPlan = new AddPlan(settlementName, selectionPolicy);
            addPlan->act(*this);
            addAction(addPlan);
        }
        if (action == "settlement")
        {
            string setname;
            int settype;
            iss >> setname >> settype;
            SettlementType a;
            if (settype == 0)
            {
                a = SettlementType::VILLAGE; 
            }
            else if (settype==1)
            {
                a = SettlementType::CITY;
            }
            else 
            {
                a = SettlementType::METROPOLIS; 
            }
            AddSettlement* addSettlement = new AddSettlement(setname, a);
            addSettlement->act(*this);
            addAction(addSettlement); 
        }
        if (action == "facility")
        {
            string name;
            int category;
            int price;
            int lifeq;
            int eco;
            int env;
            iss >> name >> category >> price >> lifeq >> eco >> env;
            FacilityCategory a;
            if (category == 0)
            {
                a = FacilityCategory::LIFE_QUALITY;
            }
            if (category == 1)
            {
                a = FacilityCategory::ECONOMY;
            }
            else 
            {
                a = FacilityCategory::ENVIRONMENT;
            }
            AddFacility* current = new AddFacility(name, a, price, lifeq, eco, env);
            current->act(*this);
            addAction(current);
        }
        if (action == "planStatus")
        {
            int id;
            iss >> id;
            PrintPlanStatus* pps = new PrintPlanStatus(id);
            pps->act(*this);
            addAction(pps);
        }
        if (action == "changePolicy")
        {
            int id;
            string policy;
            iss >> id >> policy;
            ChangePlanPolicy* cpp = new ChangePlanPolicy(id, policy);
            cpp->act(*this);
            addAction(cpp);
        }
        if (action == "log")
        {
            PrintActionsLog* pal = new PrintActionsLog();
            pal->act(*this);
            addAction(pal);
        }
        if (action == "close")
        {
            Close* close = new Close();
            close->act(*this);
            addAction(close);
        }
        if (action == "backup")
        {
            BackupSimulation* bs = new BackupSimulation();           
            bs->act(*this);
            addAction(bs);
        }
        if (action == "restore")
        {
            RestoreSimulation* rs = new RestoreSimulation();
            rs->act(*this);
            addAction(rs);
        }
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    if(!(isSettlementExists(settlement.getName()))) 
    {
        cout << "Cannot create plan" << endl;
        return;
    }
    const int currentPlanId = planCounter;
    planCounter++;
    Plan toAdd(currentPlanId,settlement, selectionPolicy,facilitiesOptions);
    plans.push_back(toAdd);
    cout << "Plan created: " << settlement.getName() << ", Type: " << selectionPolicy->toString() << endl;
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
    if (plans.empty()) 
    {
        cout << "Warning: No plans to simulate." << endl;
        return;
    }
    for(Plan& plan : plans)
    {
        plan.step();
    }
}

void Simulation::close() {
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
    isRunning = true;
}



