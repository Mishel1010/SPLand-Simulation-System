#include "Plan.h"

 Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
 : plan_id(planId),
  settlement(settlement),
  selectionPolicy(selectionPolicy),
  status(PlanStatus::AVALIABLE),
  facilities(vector<Facility*>()), 
  underConstruction(vector<Facility*>()),
  facilityOptions(facilityOptions),
  life_quality_score(0), economy_score(0), environment_score(0){}


  const int Plan::getlifeQualityScore() const{
    return life_quality_score;
  }
  const int Plan::getEconomyScore() const{
            return economy_score;
        }
  const int Plan::getEnvironmentScore() const{
            return environment_score;
        }

    void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
        this->selectionPolicy=selectionPolicy;
    }
  
  void Plan::step(){
    if (status == PlanStatus::AVALIABLE){
      FacilityType Faciltype = selectionPolicy->selectFacility(facilityOptions);
      Facility facil = Facility(facil, settlement);

      
    for (int i=0; i<underConstruction.size();){
      Facility* ptr = underConstruction[i];
      if (ptr->step() == FacilityStatus::OPERATIONAL){
        facilities.push_back(ptr);
        underConstruction.erase(underConstruction.begin()+i);
        life_quality_score += ptr->getLifeQualityScore();
        economy_score += ptr->getEconomyScore();
        environment_score += ptr->getEnvironmentScore();
      }
      else {i++;}
    }
    if ((settlement.getType() == SettlementType::VILLAGE && underConstruction.size() == 1 ) ||
     (settlement.getType() == SettlementType::CITY && underConstruction.size() == 2) || 
      (settlement.getType() == SettlementType::METROPOLIS && underConstruction.size() == 3)){
        status = PlanStatus::BUSY;
      }
  }