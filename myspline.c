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
  MSpline<T>::MSpline(const DVector<Vector<T,3>> &c, int d) {
    _d = d;
    _makeKnotVector(c.getDim());
    _C = c;
  }

  template <typename T>
  inline
  MSpline<T>::MSpline(const DVector<Vector<T,3>> &c, int d, int n) {
  }

  template <typename T>
  inline
  MSpline<T>::MSpline( const MSpline<T>& copy ) : PCurve<T,3>(copy) {}


  template <typename T>
  MSpline<T>::~MSpline() {}


  //**************************************
  //        Public local functons       **
  //**************************************


  //***************************************************
  // Overrided (public) virtual functons from PCurve **
  //***************************************************

  template <typename T>
  bool MSpline<T>::isClosed() const {
    return false;
  }


  //******************************************************
  // Overrided (protected) virtual functons from PCurve **
  //******************************************************

  template <typename T>
  void MSpline<T>::eval( T t, int d, bool /*l*/ ) const {

    this->_p.setDim( d + 1 );
    int i = _findIndex(t);
    const T b1 = (1-_W(i,1,t))*(1-_W(i-1,2,t));
    const T b2 = ((1-_W(i,1,t))*_W(i-1,2,t))+(_W(i,1,t)*(1-_W(i,2,t)));
    const T b3 = (_W(i,1,t)*_W(i,2,t));


    this->_p[0] = _C[i-2]*b1 + _C[i-1]*b2 + _C[i]*b3;



  }


  template <typename T>
  T MSpline<T>::getStartP() const {
    return T(0);
  }


  template <typename T>
  T MSpline<T>::getEndP()const {
      return T( M_2PI );
  }

  template<typename T>
  T MSpline<T>::_W(int i, int d, T t) const
  {
      return ((t - _t(i))/(_t(i+d)-_t(i)));

  }

  template<typename T>
  int MSpline<T>::_findIndex(T t) const
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
  void MSpline<T>::_makeKnotVector(int n)
  {

      //n = _C.getDim();
      _t.setDim(n+_d+1);

        for(int i = 0;i<=_d;i++){
            _t[i] = 0;
        }
        for(int i=_d+1;i<= n;i++){
            _t[i] = i-_d;
        }
        for(int i=n+1;i<=n+_d;i++){
            _t[i] = _t(i-1);
        }
  }


} // END namespace GMlib
