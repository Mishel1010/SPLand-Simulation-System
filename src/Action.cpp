#include "Action.h" 
using namespace std;
#include <iostream>

//----------------------------------------------------------------
//BaseAction Class
// ---------------------------------------------------------------

BaseAction::BaseAction(): errorMsg("Action is not completed yet!"), status(ActionStatus::ERROR) {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    cout << "Error: " << errorMsg << endl;
}

const string& BaseAction::getErrorMsg() const {
    return errorMsg;
}

//----------------------------------------------------------------
//SimulateStep Class
//----------------------------------------------------------------

SimulateStep::SimulateStep(const int numOfSteps):BaseAction(), numOfSteps(numOfSteps) {
}

void SimulateStep::act(Simulation& simulation) {
    for (int i = 0; i < numOfSteps; i++) 
    {
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const {
    return "Simulating " + to_string(numOfSteps) + " steps";
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(numOfSteps);
}

//----------------------------------------------------------------
//AddPlan Class
//----------------------------------------------------------------

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) :BaseAction(), settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation& simulation) {
    if (!(simulation.Simulation::isSettlementExists(settlementName)))
    {
        BaseAction::error("Cannot create this plan");
        cout << "Error: " + BaseAction::getErrorMsg() << endl;
        return;
    } 
    const Settlement actSet = simulation.Simulation::getSettlement(settlementName);
    if (selectionPolicy == "bal")
    {
        BalancedSelection* balancedSelection = new BalancedSelection(0,0,0);
        simulation.Simulation::addPlan(actSet, balancedSelection);
    }
    else if (selectionPolicy == "nve")
    {
        NaiveSelection* naiveSelection = new NaiveSelection();
        simulation.Simulation::addPlan(actSet, naiveSelection);
    }
    else if (selectionPolicy == "eco")
    {
        EconomySelection* economySelection = new EconomySelection();
        simulation.Simulation::addPlan(actSet, economySelection);
    }
    else if (selectionPolicy == "env")
    {
        SustainabilitySelection* sustainabilitySelection = new SustainabilitySelection();
        simulation.Simulation::addPlan(actSet, sustainabilitySelection);
    }
    else 
    {
        BaseAction::error("Cannot create this plan");
        cout << "Error: " + BaseAction::getErrorMsg() << endl;
    }
    complete();
}

const string AddPlan::toString() const {
    return "Adding plan for settlement: " + settlementName + ", selection policy: " + selectionPolicy;
}

AddPlan* AddPlan::clone() const {
    return new AddPlan(settlementName, selectionPolicy);
}

//----------------------------------------------------------------
//AddSettlement Class
//----------------------------------------------------------------

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : BaseAction(), settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation& simulation) {
    Settlement* toAdd = new Settlement(settlementName, settlementType);
    if (!(simulation.Simulation::addSettlement(toAdd)))
    {
        BaseAction::error("Settlement already exists");
        cout << "Error: " + BaseAction::getErrorMsg() << endl;
        return;
    }
    complete();
}

AddSettlement* AddSettlement::clone() const {
    return new AddSettlement(settlementName, settlementType);
}

const string AddSettlement::toString() const {
    string type;
    if (settlementType == SettlementType::VILLAGE)
    {
        type = "Village";
    }
    else if (settlementType == SettlementType::CITY)
    {
        type = "City";
    }
    else if (settlementType == SettlementType::METROPOLIS)
    {
        type = "Metropolis";
    }
    return "Adding settlement: " + settlementName + ", type: " + type;
}

//----------------------------------------------------------------
//AddFacility Class
//----------------------------------------------------------------

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) : 
    BaseAction(), 
    facilityName(facilityName),
    facilityCategory(facilityCategory),
    price(price),
    lifeQualityScore(lifeQualityScore),
    economyScore(economyScore),
    environmentScore(environmentScore) {}

void AddFacility::act(Simulation& simulation) {
    FacilityType facilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if (!(simulation.Simulation::addFacility(facilityType)))
    {
        BaseAction::error("Facility already exists");
        cout << "Error: " + BaseAction::getErrorMsg() << endl;
        return;
    }
    complete();
}

AddFacility* AddFacility::clone() const {
    return new AddFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
}

const string AddFacility::toString() const {
    string str;
    if (facilityCategory == FacilityCategory::LIFE_QUALITY)
    {
        str = "Life Quality";
    }
    else if (facilityCategory == FacilityCategory::ECONOMY)
    {
        str = "Economy";
    }
    else if (facilityCategory == FacilityCategory::ENVIRONMENT)
    {
        str = "Environment";
    }
    return "Adding facility: " + facilityName + ", category: " + str;
}

//----------------------------------------------------------------
//PrintPlanStatus Class
//----------------------------------------------------------------

PrintPlanStatus::PrintPlanStatus (int planId) : BaseAction(), planId(const_cast<int const&>(planId)) {}

void PrintPlanStatus::act(Simulation& simulation) {
    if (!(simulation.Simulation::isPlanExists(planId))) 
    {
        BaseAction::error("Plan doesn't exist");
        cout << "Error: " + BaseAction::getErrorMsg() << endl;
        return;
    }
    const Plan& plan = simulation.Simulation::getPlan(planId);
    const vector<Facility*> facilities = plan.Plan::getFacilities();
    const vector<Facility*> underConstructionFacilities = plan.Plan::getUnderConstructionFacilities();
    cout << "Plan ID: " << planId << endl;
    cout << "Settlement name: " << plan.Plan::getSettlementName() << endl;
    cout << "Plan status: " << plan.Plan::getStatus() << endl;
    cout << "Selection Policy: " << plan.Plan::getSelectionPolicyName() << endl;
    cout << "Life Quality Score: " << plan.Plan::getlifeQualityScore() << endl;
    cout << "Economy Score: " << plan.Plan::getEconomyScore() << endl;
    cout << "Environmanation Score: " << plan.Plan::getEnvironmentScore() << endl;
    for (Facility* facility : facilities)
    {
        cout << "Facility Name: " << facility->FacilityType::getName() << endl;
        cout << "Facility Status: Operational" << endl;
    }
    for (Facility* facility : underConstructionFacilities)
    {
        cout << "Facility Name: " << facility->FacilityType::getName() << endl;
        cout << "Facility Status: Under Construction" << endl;
    }
    complete();
}
