#ifndef SCENARIO_H
#define SCENARIO_H


#include "application/gmlibwrapper.h"

// qt
#include <QObject>

namespace GMlib {

template <typename T, int n>
class PCurve;


}


class Scenario : public GMlibWrapper {
    Q_OBJECT
public:
    using GMlibWrapper::GMlibWrapper;

    void    initializeScenario() override;
    void    cleanupScenario() override;

private:
    GMlib::PCurve<float,3>* myPCurve{nullptr};
    GMlib::PCurve<float,3>* openBCurve{nullptr};
    GMlib::PCurve<float,3>* closedBCurve{nullptr};
    GMlib::PCurve<float,3>* myPCurve2Const{nullptr};
    GMlib::PSurf<float,3>* planeBSurface{nullptr};
    GMlib::PSurf<float,3>* torusBSurface{nullptr};
    GMlib::PSurf<float,3>* cylinderBSurface{nullptr};


public slots:
    void callGL ();

};

#endif // SCENARIO_H
