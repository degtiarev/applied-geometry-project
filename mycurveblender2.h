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




#ifndef GM_MYCURVBLENDER2_H
#define GM_MYCURVBLENDER2_H


#include "C:\Users\Aleksei\Desktop\applied-geometry-project\gmlib\modules\parametrics\src\gmpcurve.h"


namespace GMlib {


template <typename T>
class MyCurveBlender2 : public PCurve<T,3> {
    GM_SCENEOBJECT(MyCurveBlender2)
    public:
        MyCurveBlender2(  PCurve<T,3> *c1,  PCurve<T,3> *c2, T x);
    virtual ~MyCurveBlender2();
    bool                isClosed() const override;

protected:
    void                eval(T t, int d, bool l) const override;
    T                   getStartP() const override;
    T                   getEndP()   const override;
    T                    _x; // percentage
    PCurve<T,3> *        _C1; // curve 1
    PCurve<T,3> *        _C2; // curve 2

private:
    T                   _W(int i, int d, T t) const; //  the linear translation and scaling function
    T                   _B(T t) const;
    int                 _findIndex(T t) const;
    void                _makeKnotVector(int n);
    void                _createControlPoints(const DVector<Vector<T,3>> &p,int n);


}; // END class PCircle

} // END namepace GMlib

// Include PCircle class function implementations
#include "mycurveblender2.c"


#endif // GM_MYCURVBLENDER2_H
