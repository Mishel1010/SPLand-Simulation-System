#include "SelectionPolicy.h"
#include <iostream>
//naive selection class
NaiveSelection::NaiveSelection(): lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (lastSelectedIndex+1)%facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "naive selection";
}

NaiveSelection NaiveSelection::*clone() {
    throw std::logic_error("Not implemented yet");
}

 NaiveSelection::~NaiveSelection(){}

//balanced selction class
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
: LifeQualityScore(LifeQualityScore), EconomyScore (EconomyScore), EnvironmentScore(EnvironmentScore){}

 const FacilityType& BalancedSelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions){
    const FacilityType*  ptr = &facilitiesOptions[0];
    int a = ptr->getEconomyScore()+EconomyScore;
    int b = ptr->getLifeQualityScore()+LifeQualityScore;
    int c = ptr->getEnvironmentScore()+EnvironmentScore;
    int sum =std::abs(a-b) + std::abs(b-c) + std::abs(a-c) ;

    for (const FacilityType& v:facilitiesOptions){
        int a = LifeQualityScore + v.getLifeQualityScore();
        int b = v.getEconomyScore()+EconomyScore;
        int c = v.getEnvironmentScore()+EnvironmentScore;
        int temp = std::abs(a-b) + std::abs(b-c) + std::abs(a-c);
        if (temp == 0){
            return v;
        }
        if (temp < sum){
            ptr = &v;}}
            return *ptr;

    }

    EconomySelection::EconomySelection()
    :lastSelectedIndex(-1){}

    const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
        lastSelectedIndex = (lastSelectedIndex+1)%facilitiesOptions.size();
        while (facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ECONOMY){
          lastSelectedIndex = (lastSelectedIndex+1)%facilitiesOptions.size();  
        }
        return facilitiesOptions[lastSelectedIndex];
        }

    const string EconomySelection::toString() const{
        return "Economy selection";
    }


     EconomySelection* EconomySelection::clone() const {
        throw std::logic_error("Not implemented yet");
     }


     EconomySelection::~EconomySelection(){}

    SustainabilitySelection::SustainabilitySelection()
    : lastSelectedIndex(-1){}

    const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
        lastSelectedIndex = (lastSelectedIndex+1)%facilitiesOptions.size();
        while (facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ENVIRONMENT){
          lastSelectedIndex = (lastSelectedIndex+1)%facilitiesOptions.size();  
        }
        return facilitiesOptions[lastSelectedIndex];
        }

        const string SustainabilitySelection::toString() const{
            return "SustainabilitySelection";
        }

        SustainabilitySelection* SustainabilitySelection::clone() const{
            throw std::logic_error("Not implemented yet");
        }
        SustainabilitySelection:: ~SustainabilitySelection(){}