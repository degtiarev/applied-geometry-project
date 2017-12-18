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




#ifndef GM_PARAMETRICS_CURVES_MySpline_H
#define GM_PARAMETRICS_CURVES_MySpline_H


#include "C:\Users\Aleksei\Desktop\applied-geometry-project\gmlib\modules\parametrics\src\gmpcurve.h"
#include "C:\Users\Aleksei\Desktop\applied-geometry-project\gmlib\modules\scene\src\selector\gmselector.h"


namespace GMlib {


template <typename T>
class MySpline : public PCurve<T,3> {
    GM_SCENEOBJECT(MySpline)
    public:
        // First constructor: Input is points -> set them as controll points. Make knotvector
        MySpline( const DVector<Vector<T,3>> &c, int d);

    // Second constructor: Input is vector for curve points and number of controll points, made with Least-Squares. Set degree/order. Make knotvector
    MySpline( const DVector<Vector<T,3>> &p, int d, int n);

    MySpline( const MySpline<T>& copy );
    virtual ~MySpline();

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


    // Protected data for the curve
    DVector<Vector<T,3>> _C;//control points vector
    DVector<T>           _t; //knot vector
    int                  _d; //degree

private:
    // Find / calculate W
    T                   _W(int i, int d, T t) const; // linear translation and scaling function
    // Find  i
    int                 _findIndex(T t) const;
    void                _makeKnotVector(int n);
    void                _createControlPoints(const DVector<Vector<T,3>> &p,int n);


}; // END class MySpline

} // END namepace GMlib

// Include MySpline class function implementations
#include "myspline.c"


#endif // GM_PARAMETRICS_CURVES_MySpline_H
