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


namespace GMlib {


//*****************************************
// Constructors and destructor           **
//*****************************************

// c - vector control points, d - dimension
template <typename T>
inline
MySpline<T>::MySpline(const DVector<Vector<T,3>> &c, int d) {
    _d = d;
    _makeKnotVector(c.getDim());
    _C = c;

    for (int i=0;i<_C.getDim();i++){
        Selector<T,3>* s = new Selector<T,3>(_C[i],i,this);
        this->insert(s);
    }

    auto sk = new SelectorGridVisualizer<T>;
    sk->setSelectors(_C,0,isClosed());
    this->insertVisualizer(sk);
}

// c - vector of points sampled from original curve, d - dimension, n - amount of control points
template <typename T>
inline
MySpline<T>::MySpline(const DVector<Vector<T,3>> &p, int d, int n) {
    // 2'nd degree b-spline (always = 2)
    _d = d;
    _makeKnotVector(n);
    _createControlPoints(p,n); //surfaces

    auto sk = new SelectorGridVisualizer<T>;
    sk->setSelectors(_C,0,isClosed());
    this->insertVisualizer(sk);
}

template <typename T>
inline
MySpline<T>::MySpline( const MySpline<T>& copy ) : PCurve<T,3>(copy) {}


template <typename T>
MySpline<T>::~MySpline() {}


//**************************************
//        Public local functons       **
//**************************************


//***************************************************
// Overrided (public) virtual functons from PCurve **
//***************************************************

template <typename T>
bool MySpline<T>::isClosed() const {
    return false;
}


//******************************************************
// Overrided (protected) virtual functons from PCurve **
//******************************************************

template <typename T>
void MySpline<T>::eval( T t, int d, bool /*l*/ ) const {

    this->_p.setDim( d + 1 );
    int i = _findIndex(t);
    // basis functions (= order )
    const T b1 = (1-_W(i,1,t))*(1-_W(i-1,2,t)); //A[i][j-2]
    const T b2 = ((1-_W(i,1,t))*_W(i-1,2,t))+(_W(i,1,t)*(1-_W(i,2,t))); //A[i][j-1]
    const T b3 = (_W(i,1,t)*_W(i,2,t)); //A[i][j]

    // p - vector of vectors, there we record curve (_p(0), because we have not derivatives)
    this->_p[0] = _C[i-2]*b1 + _C[i-1]*b2 + _C[i]*b3;
}


template <typename T>
T MySpline<T>::getStartP() const {
    return _t(_d);
}


template <typename T>
T MySpline<T>::getEndP()const {
    return  _t(_C.getDim());
}

// linear translation and scaling function
template<typename T>
T MySpline<T>::_W(int i, int d, T t) const
{
    return ((t - _t(i))/(_t(i+d)-_t(i)));

}

template<typename T>
int MySpline<T>::_findIndex(T t) const
{
    int i=_d;
    int n = _C.getDim();
    for(; i<=n; i++){
        if(t>=_t(i) && t<_t(i+1))
            break;
    }
    if (i >= n){
        i=n-1;
    }
    return i;
}

template<typename T>
void MySpline<T>::_makeKnotVector(int n)
{
    //n = _C.getDim();
    _t.setDim(n+_d+1);

    // 2'nd degree curve, so the first 3 knots will have the same value
    for(int i = 0; i<=_d; i++){
        _t[i] = 0;
    }

    // Starts at degree + 1 (3) and fills in the middle knots until we reach the last 2 knots
    for(int i=_d+1; i<= n; i++){
        _t[i] = i-_d;
    }

    // Last knots will have the same value, due to being of 2'nd order
    for(int i=n+1; i<=n+_d; i++){
        _t[i] = _t(i-1);
    }

}

   // Create Controllpoint set for Least Squares Method
template<typename T>
void MySpline<T>::_createControlPoints(const DVector<Vector<T, 3> > &p, int n)
{
    int m = p.getDim();
    _C.setDim(n);
    DMatrix<T> A(m,n);
    for (int i = 0; i<m; i++){
        for (int j = 0; j<n;j++){
            A[i][j] = T(0);
        }
    }

    for (int i = 0;i<m;i++){
        T t = _t[0]+i*(getEndP()-getStartP())/(m-1);

          // Find the index
        int j = _findIndex(t);

        // basis functions
        const T b1 = (1-_W(j,1,t))*(1-_W(j-1,2,t));
        const T b2 = ((1-_W(j,1,t))*_W(j-1,2,t))+(_W(j,1,t)*(1-_W(j,2,t)));
        const T b3 = (_W(j,1,t)*_W(j,2,t));

        A[i][j-2] = b1;
        A[i][j-1] = b2;
        A[i][j] = b3;
    }

    // Solve for A*p = c
    // Least squares
    DMatrix<T> Atrans = A;
    Atrans.transpose();
    DMatrix<T> B = Atrans*A;
    B.invert();
    DVector<Vector<T,3>> x = Atrans*p;
    _C = B*x;
    for(int i=0; i<_C.getDim(); i++){
        Selector<T,3>* s = new Selector<T,3>(_C[i],i,this);
        this->insert(s);
    }

}


} // END namespace GMlib
