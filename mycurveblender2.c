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

template<typename T>
MyCurveBlender2<T>::MyCurveBlender2( PCurve<T,3> *c1,  PCurve<T,3> *c2, T x)
{
    _x = x;
    _C1 = c1;
    _C2 = c2;
}


template <typename T>
MyCurveBlender2<T>::~MyCurveBlender2() {}


//**************************************
//        Public local functons       **
//**************************************



//***************************************************
// Overrided (public) virtual functons from PCurve **
//***************************************************

template <typename T>
bool MyCurveBlender2<T>::isClosed() const {
    return false;
}


//******************************************************
// Overrided (protected) virtual functons from PCurve **
//******************************************************

template <typename T>
void MyCurveBlender2<T>::eval( T t, int d, bool /*l*/ ) const {

    this->_p.setDim( d + 1 );
    if (t < _C1->getParEnd() - _x*_C1->getParDelta()){ // from start1 to end-percentage for blending
        this->_p = _C1->evaluateParent(t,0); //evaluate first part
    }
    else if (t < _C1->getParEnd()){ //blending interval
        T _t1 = (t-_C1->getParEnd()+_x*_C1->getParDelta())/(_x*_C1->getParDelta()); //(t - end of c1 + blended interval)/blended interval

        const T b1 = 1-_B(_t1);
        const T b2 = _B(_t1); //blending

        this->_p = b1*_C1->evaluateParent(t,0) + b2*_C2->evaluateParent(_C2->getParStart()+_t1*_x*_C2->getParDelta(),0);//_C[i-2]*b1 + _C[i-1]*b2 + _C[i]*b3;
    }
    else
        this->_p = _C2->evaluateParent(_C2->getParStart()+t-_C1->getParEnd()+_x*_C2->getParDelta(),0); //evaluate the part from the second curve
}


template <typename T>
T MyCurveBlender2<T>::getStartP() const {
    return _C1->getParStart();
}


template <typename T>
T MyCurveBlender2<T>::getEndP()const {
    return _C1->getParEnd()+(1-_x)*_C2->getParDelta();
}

template<typename T>
T MyCurveBlender2<T>::_B(T t) const
{
    return 3*(t*t) - 2*(t*t*t);
}


} // END namespace GMlib
