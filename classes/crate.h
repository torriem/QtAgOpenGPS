#ifndef CRATE_H
#define CRATE_H

class CModuleComm;
class CVehicle;

class CRate
{
public:
    int rate1;
    int rate2;
    double rateSetPoint;
    double rateActual;
    double volumeDelivered;
    bool isRateControlOn = false;
    bool isRate1Selected = true;

    CRate();

    void shutdownRateControl(CModuleComm *mc); //should be in CModuleComm
    void calculateRateLitersPerMinute(CVehicle *v);
};


#endif // CRATE_H
