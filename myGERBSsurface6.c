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
#include "simplesubsurf.h"

namespace GMlib {


//*****************************************
// Constructors and destructor           **
//*****************************************
template<typename T>
// n1 & n2 -amount of items in knot vectors
GMlib::MyGERBSsurface6<T>::MyGERBSsurface6(PSurf<T,3> *s, int n1, int n2)
{
    _d = 1;
    _su = s->getParStartU();
    _eu = s->getParEndU();
    _sv = s->getParStartV();
    _ev = s->getParEndV();

    // Check if original surface closed or not
    if (s->isClosedU()){
        // If the surface is closed in any direction, add additional knot in that direction
        _isclosedU = true;
        n1++;
    }
    else {
        _isclosedU = false;
    }

    if (s->isClosedV()){
        // If the surface is closed in any direction, add additional knot in that direction
        _isclosedV = true;
        n2++;
    }
    else {
        _isclosedV = false;
    }

    // Knotvector for U
    _makeKnotVectors(_u,n1,_su,_eu,_isclosedU);
    // Knotvector for V
    _makeKnotVectors(_v,n2,_sv,_ev,_isclosedV);

    // Number of subsurfaces
    _S.setDim(n1,n2);

    // Create subsurfaces
    _createLocalSurfaces(s,n1,n2);
}

template <typename T>
inline
MyGERBSsurface6<T>::MyGERBSsurface6( const MyGERBSsurface6<T>& copy ) : PSurf<T,3>(copy) {}


template <typename T>
MyGERBSsurface6<T>::~MyGERBSsurface6() {}


//**************************************
//        Public local functons       **
//**************************************


//***************************************************
// Overrided (public) virtual functons from PCurve **
//***************************************************


//******************************************************
// Overrided (protected) virtual functons from PCurve **
//******************************************************

template <typename T>
void MyGERBSsurface6<T>::eval( T u, T v, int d1, int d2, bool lu, bool lv ) const {

    this->_p.setDim( d1 + 1, d2+1);
    int i = _findIndex(u,_u);
    int j = _findIndex(v,_v);

    GMlib::DMatrix<T> Bu(2,2); // Matrix of basis functions for U
    GMlib::DMatrix<T> Bv(2,2); // Matrix of basis functions for V

    Bu[0][1] = _B(_W(i,1,u,_u)); // basis function U
    Bu[0][0] = 1 - Bu[0][1]; // basis function U

    Bv[1][0] = _B(_W(j,1,v,_v)); //  basis function V
    Bv[0][0] = 1 - Bv[1][0];    // basis function V

    Bu[1][1] = _BDeriv(_W(i,1,u,_u)); // derivatives basis functions U
    Bu[1][0] = - Bu[1][1]; // basis function U

    Bv[1][1] = _BDeriv(_W(j,1,v,_v)); // derivatives basis functions V
    Bv[0][1] = - Bv[1][1]; // basis function V

    DMatrix< DMatrix< Vector<T,3> > >C(2,2); // Matrix containing subsurfaces from original surface
    DMatrix< Vector<T,3> > C1 = _S(i-1)(j-1)->evaluateParent(u,v,d1,d2); // Gives 4 additional submatrices; S00 (Position), S00V( V deriative), S00U (U Der), and S00 UV (UV Der)
    DMatrix< Vector<T,3> > C2 = _S(i-1)(j)->evaluateParent(u,v,d1,d2); // Gives 4 additional submatrices: S01 (Position), S01V (V Der), S01U (U Der), and S01 UV (UV Der)
    DMatrix< Vector<T,3> > C3 = _S(i)(j-1)->evaluateParent(u,v,d1,d2); // --------- ||-------------
    DMatrix< Vector<T,3> > C4 = _S(i)(j)->evaluateParent(u,v,d1,d2);

    //    local curves this->_p = Bu*(C^Bv), where * is dot product & ^ is
    auto h1 = Bu[0][0]*C1[0][0] + Bu[0][1]*C3[0][0];
    auto h2 = Bu[0][0]*C2[0][0] + Bu[0][1]*C4[0][0];
    auto h3 = Bu[1][0]*C1[0][0] + Bu[1][1]*C3[0][0];
    auto h4 = Bu[1][0]*C2[0][0] + Bu[1][1]*C4[0][0];
    auto h5 = Bu[0][0]*C1[1][0] + Bu[0][1]*C3[1][0];
    auto h6 = Bu[0][0]*C2[1][0] + Bu[0][1]*C4[1][0];
    auto h7 = Bu[0][0]*C1[0][1] + Bu[0][1]*C3[0][1];
    auto h8 = Bu[0][0]*C2[0][1] + Bu[0][1]*C4[0][1];

    this->_p[0][0] = h1*Bv[0][0] + h2*Bv[1][0];
    this->_p[1][0] = h3*Bv[0][0] + h4*Bv[1][0] + h5*Bv[0][0] + h6*Bv[1][0];
    this->_p[0][1] = h1*Bv[0][1] + h2*Bv[1][1] + h7*Bv[0][0] + h8*Bv[1][0];
}


template <typename T>
T MyGERBSsurface6<T>::getStartPU() const {
    return _u(_d);
}


template <typename T>
T MyGERBSsurface6<T>::getEndPU()const {
    return _u(_S.getDim1());
}

template <typename T>
T MyGERBSsurface6<T>::getStartPV() const {
    return _v(_d);
}


template <typename T>
T MyGERBSsurface6<T>::getEndPV()const {
    return _v(_S.getDim2());
}


template<typename T>
T MyGERBSsurface6<T>::_W(int i, int d, T t,const DVector<T>& knot) const // the linear translation and scaling function
{
    return ((t - knot(i))/(knot(i+d)-knot(i)));
}

template<typename T>
int MyGERBSsurface6<T>::_findIndex(T t,const DVector<T>& knot) const
{
    int i=_d;
    int n = knot.getDim()-2;
    for(; i<=n; i++){
        if(t>=knot(i) && t<knot(i+1))
            break;
    }
    if (i >= n){
        i = n - 1;
    }

    return i;
}

template<typename T>
void MyGERBSsurface6<T>::_makeKnotVectors(DVector<T>& t,int n,T start,T end, bool isclosed)
{
    auto local_d = (end-start)/(n-1);

    t.setDim(n+_d+1);
    t[0] = t[1] = start;

    for(int i = 1;i<n;i++){
        t[i+1] = start+i*local_d;
    }

    t[n] = t[n+1] = end;

    if (isclosed){
        t[0] = t[1] - (t[n] - t[n-1]);
        t[n+1] = t[n] + (t[2] - t[1]);
    }
    //std::cout << "t is " << t << std::endl;
}

template<typename T>
void MyGERBSsurface6<T>::_createLocalSurfaces( PSurf<T, 3>* s, int n1,int n2)
{

    // If the surface is closed, we should add another knot into vector, but that complicates things when making local surfaces,
    // so we decrease the index by 1 if any direction is closed
    int nsurf1 = n1;
    int nsurf2 = n2;

    if (_isclosedU)           nsurf1--;
    if (_isclosedV)           nsurf2--;

    for(int i=0;i<nsurf1;i++){  // Cycle by U direction
        for(int j=0;j<nsurf2;j++){ // Cycle by U direction

            // Create a new Subsurface (surface, previous U, next U, current U, previous V, next V, current V)
            auto su = new PSimpleSubSurf<T>(s,_u[i],_u[i+2],_u[i+1],_v[j],_v[j+2],_v[j+1]);
            su->toggleDefaultVisualizer();
            su->replot(21,21,1,1);
            su->setCollapsed(true);
            _S[i][j] = su;
            this->insert(su);
        }
    }

    // If the surface is closed in U direction, we fold it in V direction
    if (_isclosedU){
        for(int indexU = 0;indexU<n2;indexU++){
            _S[n1-1][indexU] = _S[0][indexU];
        }
    }

    // If the surface is closed V direction, we fold it in U direction
    if (_isclosedV){
        for(int indexV = 0;indexV<n1;indexV++){
            _S[indexV][n2-1] = _S[indexV][0];
        }
    }
}

// Check U and V
template<typename T>
bool MyGERBSsurface6<T>::isClosedU() const
{
    return false;
}

template<typename T>
bool MyGERBSsurface6<T>::isClosedV() const
{
    return false;
}


// Blending Functions
// derivative of basis function
template<typename T>
T MyGERBSsurface6<T>::_BDeriv(T t) const
{
    return 6*(t) - 6*(t*t);
}

// basis function
template<typename T>
T MyGERBSsurface6<T>::_B(T t) const
{
    return 3*(t*t) - 2*(t*t*t);
}

} // END namespace GMlib
