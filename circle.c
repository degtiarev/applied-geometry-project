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





namespace GMlib {


//*****************************************
// Constructors and destructor           **
//*****************************************

template <typename T>
inline
Circle<T>::Circle( T radius ) {
    _r = radius;
}


template <typename T>
inline
Circle<T>::Circle( const Circle<T>& copy ) : PCurve<T,3>(copy) {}


template <typename T>
Circle<T>::~Circle() {}


//**************************************
//        Public local functons       **
//**************************************

template <typename T>
inline
T Circle<T>::getRadius() const {
    return _r;
}


template <typename T>
inline
void Circle<T>::setRadius( T radius ) {
    _r = radius;
}


//***************************************************
// Overrided (public) virtual functons from PCurve **
//***************************************************

template <typename T>
bool Circle<T>::isClosed() const {
    return true;
}


//******************************************************
// Overrided (protected) virtual functons from PCurve **
//******************************************************

template <typename T>
void Circle<T>::eval( T t, int d, bool /*l*/ ) const {

    // d - degree
    this->_p.setDim( d + 1 );

    const T ct1 = cos(t);
    const T st1 = sin(t);

    this->_p[0][0] = ct1;
    this->_p[0][1] = st1;
    this->_p[0][2] = 0;
}


template <typename T>
T Circle<T>::getStartP() const {
    return T(0);
}


template <typename T>
T Circle<T>::getEndP()const {
    return T( M_2PI );
}


} // END namespace GMlib
