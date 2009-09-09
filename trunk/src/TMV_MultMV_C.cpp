///////////////////////////////////////////////////////////////////////////////
// vim:et:ts=2:sw=2:ci:cino=f0,g0,t0,+0:
//                                                                           //
// The Template Matrix/Vector Library for C++ was created by Mike Jarvis     //
// Copyright (C) 1998 - 2009                                                 //
//                                                                           //
// The project is hosted at http://sourceforge.net/projects/tmv-cpp/         //
// where you can find the current version and current documention.           //
//                                                                           //
// For concerns or problems with the software, Mike may be contacted at      //
// mike_jarvis@users.sourceforge.net                                         //
//                                                                           //
// This program is free software; you can redistribute it and/or             //
// modify it under the terms of the GNU General Public License               //
// as published by the Free Software Foundation; either version 2            //
// of the License, or (at your option) any later version.                    //
//                                                                           //
// This program is distributed in the hope that it will be useful,           //
// but WITHOUT ANY WARRANTY; without even the implied warranty of            //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             //
// GNU General Public License for more details.                              //
//                                                                           //
// You should have received a copy of the GNU General Public License         //
// along with this program in the file LICENSE.                              //
//                                                                           //
// If not, write to:                                                         //
// The Free Software Foundation, Inc.                                        //
// 51 Franklin Street, Fifth Floor,                                          //
// Boston, MA  02110-1301, USA.                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#include "TMV_Blas.h"
#include "tmv/TMV_MultMV.h"
#include "tmv/TMV_Matrix.h"
#include "tmv/TMV_ProdXV.h"
#include "tmv/TMV_SumVV.h"

#ifdef BLAS
#include "TMV_MultMV_Blas.h"
#endif

namespace tmv {

  // 
  //
  // MultMV
  //

  template <bool add, class T, class M1, class V2, class V3>
  static void DoMultMV(const T x, const M1& m1, const V2& v2, V3& v3)
  {
    // Check for non-unit step and x != 1, and do the necessary copies here,
    // rather than in the InlineMultMV function.  
    // This is faster to compile, since it keeps the InlineMultMV
    // algo path to the ones that have vsize == 1.

    typedef RealType(T) RT;
    const Scaling<1,RT> one;
    const int M = m1.colsize();
    const int N = m1.rowsize();

    if (x == RT(0)) 
    { Maybe<!add>::zero(v3); }
    else if (v2.step() != 1) 
    {
      Vector<T> v2c = x*v2;
      if (v3.step() != 1) 
      {
        Vector<T> v3c(v3.size());
        VectorView<T,1> v3u = v3c.UnitView();
        InlineMultMV<false>(one,m1,v2c.UnitView(),v3u);
        Maybe<add>::add(v3,v3c);
      }
      else 
      {
        VectorView<T,1> v3u = v3.UnitView();
        InlineMultMV<add>(one,m1,v2c.UnitView(),v3u);
      }
    } 
    else if (v3.step() != 1)
    {
      Vector<T> v3c(v3.size());
      VectorView<T,1> v3u = v3c.UnitView();
      InlineMultMV<false>(one,m1,v2.UnitView(),v3u);
      Maybe<add>::add(v3,x*v3c);
    }
    else 
    {
      VectorView<T,1> v3u = v3.UnitView();
      if (x == RT(1))
        InlineMultMV<add>(one,m1,v2.UnitView(),v3u);
      else if (x == RT(-1))
        InlineMultMV<add>(Scaling<-1,RT>(),m1,v2.UnitView(),v3u);
      else if (TMV_IMAG(x) == RT(0))
        InlineMultMV<add>(Scaling<0,RT>(TMV_REAL(x)),m1,v2.UnitView(),v3u);
      else 
        InlineMultMV<add>(Scaling<0,T>(x),m1,v2.UnitView(),v3u);
    }
  }

#if 0
  template <bool add, class T, class M1, class V2, class V3>
  static void DoMultMV(const T x, const M1& m1, const V2& v2, V3& v3)
  {
    // Check for 0's in beginning or end of v2:
    // y = [ A1 A2 A3 ] [ 0 ]  -->  y = A2 x
    //                  [ x ]
    //                  [ 0 ]

    const int N = v2.size();
    int j2 = N;
    for(;j2 > 0 && v2.cref(j2-1) == T(0); --j2);
    if (j2 == 0) { if (!add) v3.Zero(); return; }
    int j1 = 0;
    for(;v2.cref(j1) == T(0); ++j1);
    TMVAssert(j1 != j2);
    DoMultMV2<add>(x,m1.CCols(j1,j2),v2.CSubVector(j1,j2),v3);
  }
#endif

#ifdef BLAS
#ifdef TMV_INST_DOUBLE
  template <bool add> 
  double beta() { return 0.; } // add = false
  template <>
  double beta<true>() { return 1.; } // add = false

  template <bool add>
  void DoMultMV(const double x,
      const ConstMatrixView<double,1>& m1,
      const ConstVectorView<double>& v2, VectorView<double> v3)
  { BlasMultMV(x,m1,v2,beta<add>(),v3); }

  template <bool add, class T1, bool C1, class T2, bool C2>
  void DoMultMV(const std::complex<double>  x,
      const ConstMatrixView<T1,1,UNKNOWN,C1>& m1,
      const ConstVectorView<T2,UNKNOWN,C2>& v2,
      VectorView<std::complex<double> > v3)
  { BlasMultMV(x,m1,v2,beta<add>(),v3); }
#endif // DOUBLE
#ifdef TMV_INST_FLOAT
  template <bool add> 
  static float fbeta() { return 0.; } // add = false
  template <>
  static float fbeta<true>() { return 1.; } // add = false

  template <bool add>
  void DoMultMV(const float x,
      const ConstMatrixView<float,1>& m1,
      const ConstVectorView<float>& v2, VectorView<float> v3)
  { BlasMultMV(x,m1,v2,fbeta<add>(),v3); }

  template <bool add, class T1, bool C1, class T2, bool C2>
  void DoMultMV(const std::complex<float>  x,
      const ConstMatrixView<T1,1,UNKNOWN,C1>& m1,
      const ConstVectorView<T2,UNKNOWN,C2>& v2,
      VectorView<std::complex<float> > v3)
  { BlasMultMV(x,m1,v2,fbeta<add>(),v3); }
#endif // FLOAT
#endif // BLAS

  template <class T1, bool C1, class T2, bool C2, class T3>
  void InstMultMV(const T3 x,
      const ConstMatrixView<T1,1,UNKNOWN,C1>& m1,
      const ConstVectorView<T2,UNKNOWN,C2>& v2, VectorView<T3> v3)
  { DoMultMV<false>(x,m1,v2,v3); }
  template <class T1, bool C1, class T2, bool C2, class T3>
  void InstAddMultMV(const T3 x,
      const ConstMatrixView<T1,1,UNKNOWN,C1>& m1,
      const ConstVectorView<T2,UNKNOWN,C2>& v2, VectorView<T3> v3)
  { DoMultMV<true>(x,m1,v2,v3); }


#define InstFile "TMV_MultMV_C.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv


