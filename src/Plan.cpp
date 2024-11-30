#include "Plan.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "Facility.h"
#include "Simulation.h"
#include<iostream>
using namespace std;

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):
    plan_id(planId),
    settlement(const_cast<Settlement&>(settlement)),
    selectionPolicy(selectionPolicy),
    status(PlanStatus::AVAILABLE),
    facilities(vector<Facility*>()), 
    underConstruction(vector<Facility*>()),
    facilityOptions(facilityOptions),
    life_quality_score(0), economy_score(0), environment_score(0){}

const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}

const int Plan::getEconomyScore() const {
    return economy_score;
}
const int Plan::getEnvironmentScore() const {
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    this->selectionPolicy=selectionPolicy;
}
  
void Plan::step(){
    while (status == PlanStatus::AVAILABLE)
    {
        Facility* facil = new Facility(const_cast<FacilityType&>(selectionPolicy->SelectionPolicy::selectFacility(facilityOptions)), settlement.Settlement::getName());
        underConstruction.push_back(facil);
        if ((settlement.getType() == SettlementType::VILLAGE && underConstruction.size() == 1 ) ||
            (settlement.getType() == SettlementType::CITY && underConstruction.size() == 2) || 
            (settlement.getType() == SettlementType::METROPOLIS && underConstruction.size() == 3))
        {
            status = PlanStatus::BUSY;
        }
    }
    int i = 0;
    while (i < underConstruction.size()) 
    {
        Facility* ptr = underConstruction[i];
        if (ptr->step() == FacilityStatus::OPERATIONAL)
        {
            addFacility(ptr);
            underConstruction.erase(underConstruction.begin()+i);
            status = PlanStatus::AVAILABLE;
            life_quality_score += ptr->getLifeQualityScore();
            economy_score += ptr->getEconomyScore();
            environment_score += ptr->getEnvironmentScore();
        }
        else 
        {
            i++;
        }
    }
}
void Plan::printStatus() {
    if (status == PlanStatus::AVAILABLE)
    {
        cout << "Plan ID: " << plan_id << ", Status: Available" << endl;
    }
    else if (status == PlanStatus::BUSY) 
    {
        cout << "Plan ID: " << plan_id << ", Status: Busy" << endl;
    }
}

const string Plan::getStatus() const {
    if (status == PlanStatus::AVAILABLE)
    {
        return "Available";
    }
    else if (status == PlanStatus::BUSY)
    {
        return "Busy";
    }
}

const vector<Facility*>& Plan::getFacilities() const {
    return facilities;
}

const vector<Facility*>& Plan::getUnderConstructionFacilities() const {
    return underConstruction;
}

const string Plan::getSettlementName() const {
    return settlement.getName();
}

const string Plan::getSelectionPolicyName() const {
    return selectionPolicy->toString();
}

void Plan::addFacility(Facility* facility) {
    facilities.push_back(facility);
}

const string Plan::toString() const {
    string str;
    str += "this is plan number " + to_string(plan_id) + "for the settlement " + settlement.getName() + "\n";
    str += "Current status: ";
    if (status == PlanStatus::AVAILABLE)
    {
        str += "Available\n";
    }
    else if (status == PlanStatus::BUSY) 
    {
        str += "Busy\n";
    }
    return str;
}

const int Plan::getPlanId() const {
    return plan_id;
}

Plan::~Plan() {
    for (Facility* facil : facilities)
    {
        delete facil;
    }
    facilities.clear();
    for (Facility* facil : underConstruction)
    {
        delete facil;
    }
    underConstruction.clear();
}
