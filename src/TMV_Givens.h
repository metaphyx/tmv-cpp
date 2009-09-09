///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// The Template Matrix/Vector Library for C++ was created by Mike Jarvis     //
// Copyright (C) 2008                                                        //
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


//
// This file contains the code to implement Givens rotations
//
// A Givens rotation is a 2x2 matrix of the form:
//
// G = [  c  s  ]
//     [ -s* c* ]
//
// where |c|^2 + |s|^2 = 1.
// Normally, people consider real Givens matrices where the *'s
// (conjugations) are not necessary.  But they generalize easily
// to the complex case as above.
//
// Givens matrices are used to zero out a single element in a matrix by
// rotating a 2 element vector so that its second element is 0:
//
// [  c  s  ] [ x ] = [ r ]
// [ -s* c* ] [ y ]   [ 0 ]
// 
// [ Note: often (eg. in Golub and Van Loan) Givens matrices are defined
//   as the transpose (or adjoint) of this.  We prefer this definition. ]
//
// (1)  c x + s y = r
// (2)  c* y - s* x = 0
//
// From (2), we get  s* = yc*/x, so 
// |x|^2 |s|^2 = |y|^2 |c|^2 = |y|^2 (1 - |s|^2)
//
// |s| = |y|/sqrt(|x|^2+|y|^2)  and |c| = |x|/sqrt(|x|^2+|y|^2)
//
// From (1), r = cx+sy = cx+yy*c/x* = c/x* (|x|^2+|y|^2)
// |r| = sqrt(|x|^2+|y|^2)
//
// All that remain is to determine the phases of c,s
//
// Let x = |x| e^ip
//     y = |y| e^iq
//     r = |r| e^it
//     c = |x|/|r| e^ia
//     s = |y|/|r| e^ib
//
// Plugging these into (1), we find that  t = a+p = b+q
//
// Since a priori, we should consider all three of (a,b,t)
// to be unknown, we are left with one free parameter.
// There are (at least) 3 reasonable options:
//
// Choice 1: a = -p, b = -q, t = 0
//
// This has the advantage that r is real and positive.
// Also, the equations for c,s look much like the usual
// forms for real Givens matrices:
//
// c = x*/|r|  s = y*/|r|   r = |r|
//
// Choice 2: a = 0, b = p-q, t = p
//
// This has the advantage that c is real, so the Givens
// matrix only needs to store 3 real numbers, not 4.
// Multiplication later is also faster by a factor
// of 4/3.
//
// c = |x|/|r|  s = (x/|x|)y*/|r|  r = (x/|x|)|r|
//
// Choice 3: a = q-p, b = 0, t = q
//
// Similar to choice 2, but s is real:
// 
// c = (y/|y|)x*/|r|  s = |y|/|r|  r = (y/|y|)|r|
//
// 
// The speed of calculation is usually more important than the 
// convenience of a rotating to a real value, so choice 2 or 3
// is preferred.  
// Choice 2 is a bit better when |y|<|x|, since we prefer to 
// calculate sqrt(1+|y/x|^2), and the calculations for 2 work 
// better in this case.
// Since this is more common than |x|<|y|, we always use choice 2.
//

#ifndef TMV_Givens_H
#define TMV_Givens_H

#include "TMV_BaseVector.h"
#include "TMV_BaseMatrix.h"

namespace tmv {

  // For complex, this is quicker than the real abs and works just
  // as well for the purpose here.
  template <class T> inline T MAXABS(T x)
  { return ABS(x); }

  template <class T> inline T MAXABS(std::complex<T> x)
  { return MAX(ABS(REAL(x)),ABS(IMAG(x))); }

  template <class T> class Givens;
  // Defined in TMV_Givens.cpp
  template <class T> Givens<T> Givens_Rotate(T& x, T& y);
  // Use a Givens matrix G to rotate the vector so y = 0:
  // G [ x ] = [ r ]
  //   [ y ]   [ 0 ]
  // Also, return the Givens matrix used to do so.

  // Do: [ x ] <- [  c   s ] [ x ]
  //     [ y ]    [ -s*  c ] [ y ]
  template <class Tg, class T> void GivensMult(RealType(Tg) c, Tg s,
      T& x, T& y);
  template <class Tg, class T> void GivensMult(RealType(Tg) c, Tg s,
      T& x, ConjRef<T> y);
  template <class Tg, class T> void GivensMult(RealType(Tg) c, Tg s,
      ConjRef<T> x, T& y);
  template <class Tg, class T> void GivensMult(RealType(Tg) c, Tg s,
      ConjRef<T> x, ConjRef<T> y);
  template <class Tg, class T> void GivensMult(RealType(Tg) c, Tg s,
      T& x, VarConjRef<T> y);
  template <class Tg, class T> void GivensMult(RealType(Tg) c, Tg s,
      VarConjRef<T> x, T& y);
  template <class Tg, class T> void GivensMult(RealType(Tg) c, Tg s,
      VarConjRef<T> x, VarConjRef<T> y);

  template <class Tg, class T> void GivensMult(
      RealType(Tg) c, Tg s,
      const VectorView<T>& v0, const VectorView<T>& v1);

  // Do: [ d0 e0* ] <- [  c  s ] [ d0 e0* ] [ c  -s ]
  //     [ e0 d1  ]    [ -s* c ] [ e0 d1  ] [ s*  c ]
  template <class Tg, class T> void GivensHermMult(RealType(Tg) c, Tg s,
      T& d0, T& d1, T& e0);

  // Do: [ d0 e0 ] <- [  c  s ] [ d0 e0 ] [ c -s* ]
  //     [ e0 d1 ]    [ -s* c ] [ e0 d1 ] [ s  c  ]
  template <class Tg, class T> void GivensSymMult(RealType(Tg) c, Tg s,
      T& d0, T& d1, T& e0);
  
  template <class T> class Givens 
  {

    public:

      // Constructors
      inline Givens(const RealType(T)& _c, const T& _s) : c(_c), s(_s) {}
      inline Givens() {} // Uninitialized
      inline ~Givens() {}
      // Use default copy, op=

      inline Givens<T> Transpose() const 
      { return Givens<T>(c,-tmv::CONJ(s)); }

      inline Givens<T> Conjugate() const 
      { return Givens<T>(c,tmv::CONJ(s)); }

      inline Givens<T> Adjoint() const 
      { return Givens<T>(c,-s); }

      template <class T2> inline void Mult(T2& x, T2& y) const 
      { GivensMult(c,s,x,y); }
      template <class T2> inline void ConjMult(T2& x, T2& y) const 
      { GivensMult(c,tmv::CONJ(s),x,y); }

      template <class T2> inline void Mult(ConjRef<T2> x, ConjRef<T2> y) const 
      { GivensMult(c,s,x,y); }
      template <class T2> inline void ConjMult(ConjRef<T2> x, ConjRef<T2> y) const 
      { GivensMult(c,tmv::CONJ(s),x,y); }
      template <class T2> inline void Mult(VarConjRef<T2> x, VarConjRef<T2> y) const 
      { GivensMult(c,s,x,y); }
      template <class T2> inline void ConjMult(
	  VarConjRef<T2> x, VarConjRef<T2> y) const 
      { GivensMult(c,tmv::CONJ(s),x,y); }

      template <class T2> inline void Mult(const VectorView<T2>& v) const
      { TMVAssert(v.size()==2); GivensMult(c,s,v(0),v(1)); }
      template <class T2> inline void ConjMult(const VectorView<T2>& v) const
      { TMVAssert(v.size()==2); GivensMult(c,tmv::CONJ(s),v(0),v(1)); }

      template <class T2> inline void Mult(const MatrixView<T2>& m) const
      { 
	TMVAssert(m.colsize()==2);
	GivensMult(c,s,m.row(0),m.row(1));
      }
      template <class T2> inline void ConjMult(const MatrixView<T2>& m) const
      { 
	TMVAssert(m.colsize()==2);
	GivensMult(c,tmv::CONJ(s),m.row(0),m.row(1));
      }

      template <class T2> inline void HermMult(T2& d0, T2& d1, T2& e0) const 
      { GivensHermMult(c,s,d0,d1,e0); }
      template <class T2> inline void SymMult(T2& d0, T2& d1, T2& e0) const 
      { GivensSymMult(c,s,d0,d1,e0); }

    private:

      RealType(T) c;
      T s;
  };

} // namespace tmv

#endif
