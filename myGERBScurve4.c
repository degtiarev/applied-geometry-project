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
#include "C:\Users\Aleksei\Desktop\applied-geometry-project\gmlib\modules\scene\src\visualizers\gmselectorgridvisualizer.h"
#include "C:\Users\Aleksei\Desktop\applied-geometry-project\gmlib\modules\parametrics\src\curves\gmpsubcurve.h"

namespace GMlib {


//*****************************************
// Constructors and destructor           **
//*****************************************
template<typename T>
GMlib::MyGERBScurve4<T>::MyGERBScurve4(PCurve<T,3> *c, int n)
{
    _d = 1;
    _s  = c->getParStart();
    _e = c->getParEnd();

    _makeKnotVector(n);
    _C.setDim(n);
    for(int i=0;i<n;i++){
        auto cu = new PSubCurve<T>(c,_t[i],_t[i+2],_t[i+1]);
        cu->toggleDefaultVisualizer();
        cu->replot(21,0);
        cu->setCollapsed(true);
        _C[i] = cu;
        this->insert(cu);
    }
}

template <typename T>
inline
MyGERBScurve4<T>::MyGERBScurve4( const MyGERBScurve4<T>& copy ) : PCurve<T,3>(copy) {}


template <typename T>
MyGERBScurve4<T>::~MyGERBScurve4() {}


//**************************************
//        Public local functons       **
//**************************************


//***************************************************
// Overrided (public) virtual functons from PCurve **
//***************************************************

template <typename T>
bool MyGERBScurve4<T>::isClosed() const {
    return false;
}


//******************************************************
// Overrided (protected) virtual functons from PCurve **
//******************************************************

template <typename T>
void MyGERBScurve4<T>::eval( T t, int d, bool /*l*/ ) const {

    this->_p.setDim( d + 1 );
    int i = _findIndex(t);

    const T b1 = 1-_B(_W(i,1,t));
    const T b2 = _B(_W(i,1,t));



    //                 local curves
    this->_p = b1*_C[i-1]->evaluateParent(t,0) + b2*_C[i]->evaluateParent(t,0);//_C[i-2]*b1 + _C[i-1]*b2 + _C[i]*b3;



}


template <typename T>
T MyGERBScurve4<T>::getStartP() const {
    return _t(_d);
}


template <typename T>
T MyGERBScurve4<T>::getEndP()const {
    return _t(_C.getDim());
}

template<typename T>
T MyGERBScurve4<T>::_W(int i, int d, T t) const
{
    return ((t - _t(i))/(_t(i+d)-_t(i)));

}

template<typename T>
int MyGERBScurve4<T>::_findIndex(T t) const
{
    int i=_d;
    int n = _C.getDim();
    for(;i<=n;i++){
        if(t>=_t(i) && t<_t(i+1))
            break;
    }
    if (i >= n){
        i=n-1;
    }
    return i;
}

template<typename T>
void MyGERBScurve4<T>::_makeKnotVector(int n)
{

    auto local_d = (_e-_s)/(n-1);

    _t.setDim(n+_d+1);
    _t[0] = _t[1] = _s;

    for(int i = 0;i<n;i++){
        _t[i+2] = _s+i*local_d;
    }

    _t[n] = _t[n+1] = _e;

}

template<typename T>
void MyGERBScurve4<T>::_createControlPoints(const DVector<Vector<T, 3> > &p, int n)
{
    int m = p.getDim();
    DMatrix<Vector<T,3>> A(m,n);
    for (int i = 0; i<m; i++){
        for (int j = 0; j<n;j++){
            A[i][j] = T(0);
        }
    }

}

template<typename T>
T MyGERBScurve4<T>::_B(T t) const
{
    return 3*(t*t) - 2*(t*t*t);
}

template<typename T>
void GMlib::MyGERBScurve4<T>::localSimulate(double dt)
{
    for (int i = 0;i<_C.getDim();i++){
        _C[i]->rotate(dt,GMlib::Vector<float,3>(1,0,0));
    }

}




} // END namespace GMlib
