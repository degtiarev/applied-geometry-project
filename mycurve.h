/**********************************************************************************
**
** Copyright (C) 1994 Narvik University College
** Contact: GMlib Online Portal at http://episteme.hin.no
**
** This file is part of the Geometric Modeling Library, GMlib.
**
** GMlib is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** GMlib is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with GMlib.  If not, see <http://www.gnu.org/licenses/>.
**
**********************************************************************************/




#ifndef GM_PARAMETRICS_CURVES_PCIRCLE_H
#define GM_PARAMETRICS_CURVES_PCIRCLE_H


#include "C:\Users\Aleksei\Desktop\applied-geometry-project\gmlib\modules\parametrics\src\gmpcurve.h"


namespace GMlib {


template <typename T>
class MyCurve : public PCurve<T,3> {
    GM_SCENEOBJECT(MyCurve)
    public:
        MyCurve( T radius = T(20) );
    MyCurve( const MyCurve<T>& copy );
    virtual ~MyCurve();

    // Public local functions
    T               getRadius() const;
    void            setRadius( T radius = T(20) );

    //****************************************
    //****** Virtual public functions   ******
    //****************************************

    // from PCurve
    bool                isClosed() const override;

protected:
    // Virtual function from PCurve that has to be implemented locally
    void                eval(T t, int d, bool l) const override;
    T                   getStartP() const override;
    T                   getEndP()   const override;


    // Radius
    T               _r;

}; // END class PCircle

} // END namepace GMlib

// Include PCircle class function implementations
#include "mycurve.c"


#endif // GM_PARAMETRICS_CURVES_PCIRCLE_H
