#include "Action.h" 
using namespace std;
#include <iostream>

BaseAction::BaseAction(): errorMsg("Action resulted in an error"), status(ActionStatus::ERROR) {};

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

SimulateStep::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps) {}