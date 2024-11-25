#include "SelectionPolicy.h"
//naive selection class
NaiveSelection::NaiveSelection()
: lastSelectedIndex(-1){}

NaiveSelection::NaiveSelection(int lastindex)
: lastSelectedIndex(lastindex){}

const FacilityType& NaiveSelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (lastSelectedIndex+1)%facilitiesOptions.size();
return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const{
    return "naive selection";
}

NaiveSelection NaiveSelection::*clone() {
 return new NaiveSelection* = clone->this;
}


 NaiveSelection::~NaiveSelection(){}

//balanced selction class
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
: LifeQualityScore(LifeQualityScore), EconomyScore (EconomyScore), EnvironmentScore(EnvironmentScore){}

 const FacilityType& BalancedSelection::selectFacility(const std::vector<FacilityType>& facilitiesOptions){
    FacilityType* facil = facilitiesOptions[0]*;
    int a = facil.getEconomyScore()+EconomyScore;
    int b = facil.getLifeQualityScore()+LifeQualityScore;
    int c = facil.getEnvironmentScore()+EnvironmentScore;
    int sum =std::abs(a-b) + std::abs(b-c) + std::abs(a-c) ;

    for (FacilityType v:facilitiesOptions){
        int a = LifeQualityScore + v.getLifeQualityScore();
        int b = v.getEconomyScore()+EconomyScore;
        int c = v.getEnvironmentScore()+EnvironmentScore;
        int temp = std::abs(a-b) + std::abs(b-c) + std::abs(a-c);
        if (temp == 0){
            return v;
        }
        if (temp < sum){
            facil = v;}
            return v;}

    }