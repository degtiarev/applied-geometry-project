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




#ifndef GM_MyGERBScurve4_H
#define GM_MyGERBScurve4_H


#include "C:\Users\Aleksei\Desktop\applied-geometry-project\gmlib\modules\parametrics\src\gmpcurve.h"
#include "C:\Users\Aleksei\Desktop\applied-geometry-project\gmlib\modules\scene\src\selector\gmselector.h"

namespace GMlib {


template <typename T>
class MyGERBScurve4 : public PCurve<T,3> {
    GM_SCENEOBJECT(MyGERBScurve4)
    public:
        MyGERBScurve4(  PCurve<T,3> *c, int n);
    //MyGERBScurve4( const DVector<Vector<T,3>> &c, int d, int n);
    MyGERBScurve4( const MyGERBScurve4<T>& copy );
    virtual ~MyGERBScurve4();

    // Public local functions



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
    void                localSimulate(double dt) override; // Animation


    // Protected data for the curve

    DVector<PCurve<T,3>*>_C; //local curves
    DVector<T>           _t; //knot vector
    int                  _d; //degree
    T                    _s; //start
    T                    _e; //end
    bool                 _isclosed;

private:

    T                   _W(int i, int d, T t) const;  // Calculate the linear translation and scaling function
    T                   _B(T t) const; // Basis function
    int                 _findIndex(T t) const; // Find  i (find index of ti in the knot vector, which term of the sum is different from 0)
    void                _makeKnotVector(int n); // Form knotvector
    void                _createLocalCurves(PCurve<T,3> *c,int n);


}; // END class MyGERBScurve4

} // END namepace GMlib

// Include MyGERBScurve4 class function implementations
#include "myGERBScurve4.c"


#endif // GM_MyGERBScurve4_H
