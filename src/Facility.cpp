#include "Facility.h"
#include <string>
#include <iostream>

//--------------------------------------------------------------
//FacilityType class
//--------------------------------------------------------------
FacilityType::FacilityType(
    const string &name,
    const FacilityCategory category,
    const int price,
    const int lifeQuality_score,
    const int economy_score,
    const int environment_score)
    : name(name),
      category(category),
      price(price),
      lifeQuality_score(lifeQuality_score),
      economy_score(economy_score),
      environment_score(environment_score){}
     
const string &FacilityType::getName() const
{
    return name;
}

int FacilityType::getCost() const
{
    return price;
}

int FacilityType::getLifeQualityScore() const
{
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const
{
        return economy_score;
}

int FacilityType::getEconomyScore() const
{
        return economy_score;
}

FacilityCategory FacilityType::getCategory() const
{
    return category;
}

//--------------------------------------------------------------
//Facility class
//--------------------------------------------------------------
Facility::Facility(
    const string &name,
    const string &settlementName,
    const FacilityCategory category,
    const int price,
    const int lifeQuality_score,
    const int economy_score,
    const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
      settlementName(settlementName),
      status(FacilityStatus::UNDER_CONSTRUCTIONS),
      timeLeft(price){}

       
Facility::Facility (const FacilityType &type, const string &settlementName):
    FacilityType(type),
    settlementName(settlementName),
    status(FacilityStatus::UNDER_CONSTRUCTIONS),
    timeLeft(type.getCost()) {}

const string &Facility::getSettlementName() const
{
    return this->settlementName;
}
     
const int Facility::getTimeLeft() const
{
    return timeLeft;
}
        
FacilityStatus Facility::step()
{
    if (timeLeft==1)
    {
        status = FacilityStatus::OPERATIONAL;
    }
    timeLeft--;
}


void Facility::setStatus(FacilityStatus status)
{
    status=status;
}

const FacilityStatus& Facility::getStatus() const
{
    return status;
}
        
const string Facility::toString() const
{
    return  "settlement: " + getSettlementName() + ", faciility: " + getName() + ", price: " + std::to_string(getCost()) + ", time left: " + std::to_string(getTimeLeft()) + ", life quality score: " + std::to_string(getLifeQualityScore())
    + ", economy score: " + std::to_string(getEconomyScore()) + ", environment score: " + std::to_string(getEnvironmentScore());
}
        







