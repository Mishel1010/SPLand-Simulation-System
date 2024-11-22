#include "Facility.h"

class Facilitytype {

protected:

//members
     const string name;
     const FacilityCategory category;
     const int price;
     const int lifeQuality_score;
     const int economy_score;
     const int environment_score;


     public: 
     //constructor
     facilitytype
       (const string& nameInput,
       FacilityCategory inputcategory,
       int priceInput,
       int lifeinput,
       int ecoinput,
       int envinput)
        : name(nameInput), category(inputcategory), price(priceInput), lifeQuality_score(lifeinput), economy_score(ecoinput), environment_score(envinput)
        {}
     

     const string &getName() const{
      return name;}

     int getCost() const{
        return price;}

     int getLifeQualityScore() const{
        return lifeQuality_score;
     }

     int getEnvironmentScore() const{
        return Environment_score;
     }
     int getEconomyScore() const{
        return Economy_score;
     }

     FacilityCategory getCategory() const{
        return Category;
     }







class Facility: public FacilityType {

    private:
    //members
        const string settlementName;
        FacilityStatus status;
        int timeLeft;
        const FacilityType type;


    //constructor
    facility(
        const string &nameinput,
        const string &settnameinput,
        const FacilityCategory category,
        const int priceinput,
        const int lifescore,
        const int ecoscore,
        const int envscore)
        : settlementName(settnameinput), type(nameinput, category, priceinput, lifescore, ecoscore, envscore), timeLeft(priceinput), status(UNDER_CONSTRUCTIONS)
        {}
    Facility(
        //a constructor that recieves a facillity type
            const string &typeinput,
            const string &settnameinput)
            : settlementname(settnameinput), type(typeinput), tymeleft(typeinput.getCost()), status(UNDER_CONSTRUCTIONS)
            {}

             const string &getSettlementName() const{
             return settlementname;}

     
             const string &gettype const{
                return type.getname();
             }
     
        const int getTimeLeft() const{
            return timeLeft;}
        
        FacilityStatus step(){
            if getTimeLeft()==1{
               setStatus(OPERATIONAL)}
            timeLeft--;}

        void setStatus(FacilityStatus status){
            this.status=status;}

        const FacilityStatus& getStatus() const{
            return status;}
        
        const string toString() const{
            return ""+ gettype + ", " + getsettlementname() + ", time left: " + getTimeLeft() + 
        }
    


    }







