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




#ifndef GM_PARAMETRICS_CURVES_MSpline_H
#define GM_PARAMETRICS_CURVES_MSpline_H


#include "C:\Users\Aleksei\Desktop\applied-geometry-project\gmlib\modules\parametrics\src\gmpcurve.h"


namespace GMlib {


  template <typename T>
  class MSpline : public PCurve<T,3> {
    GM_SCENEOBJECT(MSpline)
  public:
    MSpline( const DVector<Vector<T,3>> &c, int d);
    MSpline( const DVector<Vector<T,3>> &c, int d, int n);
    MSpline( const MSpline<T>& copy );
    virtual ~MSpline();

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

    T                   _W(int i, int d, T t) const;
    int                 _findIndex(T t) const;
    void                _makeKnotVector(int n);


  }; // END class MSpline

} // END namepace GMlib

// Include MSpline class function implementations
#include "myspline.c"


#endif // GM_PARAMETRICS_CURVES_MSpline_H
