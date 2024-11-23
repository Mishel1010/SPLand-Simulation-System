#include "Facility.h"
#include <string>
#include <iostream>

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

Facility::Facility(
    const string &name,
    const string &settlementName,
    const FacilityCategory category,
    const int price,
    const int lifeQuality_score,
    const int economy_score,
    const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
      settlementName(settlementName){}

//A constructor that recieves a facillity type        
Facility::Facility(   
        FacilityType &type, 
        const string &settlementName)
        : FacilityType(type),
          settlementName(settlementName){}

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
    throw "Not implemented";
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
    throw "Not implemented";
}
        







