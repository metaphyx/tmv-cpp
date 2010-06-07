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


//#define XDEBUG


#include "tmv/TMV_SymBandMatrixArithFunc.h"
#include "tmv/TMV_SymBandMatrix.h"
#include "tmv/TMV_BandMatrix.h"
#include "tmv/TMV_BandMatrixArith.h"
#include "tmv/TMV_SymBandMatrixArith.h"

#ifdef XDEBUG
#include "tmv/TMV_MatrixArith.h"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#endif

namespace tmv {

#ifdef TMV_BLOCKSIZE
#define SYM_MM_BLOCKSIZE TMV_BLOCKSIZE
#define SYM_MM_BLOCKSIZE2 (TMV_BLOCKSIZE/2)
#else
#define SYM_MM_BLOCKSIZE 64
#define SYM_MM_BLOCKSIZE2 32
#endif

  //
  // MultMM
  //

  template <bool add, class T, class Ta, class Tb> static void DoMultMM(
      const T alpha, const GenSymBandMatrix<Ta>& A, const GenBandMatrix<Tb>& B,
      const BandMatrixView<T>& C)
  {
    TMVAssert(A.size() == C.colsize());
    TMVAssert(A.size() == B.colsize());
    TMVAssert(B.rowsize() == C.rowsize());
    TMVAssert(C.colsize() > 0);
    TMVAssert(C.rowsize() > 0);
    TMVAssert(A.rowsize() > 0);
    TMVAssert(alpha != T(0));

    const int N = A.size();

    if (add) C += alpha * A.LowerBand() * B;
    else C = alpha * A.LowerBand() * B;

    if (N > 1 && A.nlo() > 0) {
      const int M = C.rowsize();
      if (B.nlo() > 0) {
        C.SubBandMatrix(0,N-1,0,M,
            C.nlo()==int(C.colsize())-1?C.nlo()-1:C.nlo(),C.nhi()) += 
        alpha * A.UpperBandOff() * 
        B.SubBandMatrix(1,N,0,M,B.nlo()-1,
            B.nhi()==int(B.rowsize())-1?B.nhi():B.nhi()+1);
      }
      else  {
        C.SubBandMatrix(0,N-1,1,M,
            C.nlo()>=int(C.colsize())-2?
            int(C.colsize())-2:C.nlo()+1,C.nhi()-1) += 
        alpha * A.UpperBandOff() *
        B.SubBandMatrix(1,N,1,M,B.nlo(),B.nhi());
      }
    }
  }

  template <bool add, class T, class Ta, class Tb> static void TempMultMM(
      const T alpha, const GenSymBandMatrix<Ta>& A, const GenBandMatrix<Tb>& B,
      const BandMatrixView<T>& C)
  {
    if (C.isrm()) {
      BandMatrix<T,RowMajor> C2(C.colsize(),C.rowsize(),C.nlo(),C.nhi());
      DoMultMM<false>(T(1),A,B,C2.View());
      if (add) C += alpha*C2;
      else C = alpha*C2;
    } else if (C.iscm()) {
      BandMatrix<T,ColMajor> C2(C.colsize(),C.rowsize(),C.nlo(),C.nhi());
      DoMultMM<false>(T(1),A,B,C2.View());
      if (add) C += alpha*C2;
      else C = alpha*C2;
    } else {
      BandMatrix<T,DiagMajor> C2(C.colsize(),C.rowsize(),C.nlo(),C.nhi());
      DoMultMM<false>(T(1),A,B,C2.View());
      if (add) C += alpha*C2;
      else C = alpha*C2;
    }
  }

  template <bool add, class T, class Ta, class Tb> void MultMM(
      const T alpha, const GenSymBandMatrix<Ta>& A, const GenBandMatrix<Tb>& B,
      const BandMatrixView<T>& C)
  // C (+)= alpha * A * B
  {
#ifdef XTEST
    TMVAssert(A.HermOK());
#endif
    TMVAssert(A.size() == C.colsize());
    TMVAssert(A.size() == B.colsize());
    TMVAssert(B.rowsize() == C.rowsize());
#ifdef XDEBUG
    //cout<<"Start MultMM: alpha = "<<alpha<<endl;
    //cout<<"A = "<<A.cptr()<<"  "<<TypeText(A)<<"  "<<A<<endl;
    //cout<<"B = "<<B.cptr()<<"  "<<TypeText(B)<<"  "<<B<<endl;
    //cout<<"C = "<<C.cptr()<<"  "<<TypeText(C)<<"  "<<C<<endl;
    Matrix<Ta> A0 = A;
    Matrix<Tb> B0 = B;
    Matrix<T> C0 = C;
    Matrix<T> C2 = alpha*A0*B0;
    if (add) C2 += C0;
#endif

    if (C.colsize() > 0 && C.rowsize() > 0) {
      if (alpha == T(0)) {
        if (!add) C.Zero();
      }
      else if (C.isconj()) {
        MultMM<add>(CONJ(alpha),A.Conjugate(),B.Conjugate(),C.Conjugate());
      }
      else if (SameStorage(A,C) || SameStorage(B,C))
        TempMultMM<add>(alpha,A,B,C);
      else DoMultMM<add>(alpha, A, B, C);
    }

#ifdef XDEBUG
    //cout<<"Done: C = "<<C<<endl;
    if (Norm(C-C2) > 0.001*(ABS(alpha)*Norm(A0)*Norm(B0)+
          (add?Norm(C0):RealType(T)(0)))) {
      cerr<<"MultMM: alpha = "<<alpha<<endl;
      cerr<<"add = "<<add<<endl;
      cerr<<"A = "<<TypeText(A)<<"  "<<A0<<endl;
      cerr<<"B = "<<TypeText(B)<<"  "<<B0<<endl;
      cerr<<"C = "<<TypeText(C)<<"  "<<C0<<endl;
      cerr<<"--> C = "<<C<<endl;
      cerr<<"C2 = "<<C2<<endl;
      abort();
    }
#endif
  }

  template <bool add, class T, class Ta, class Tb> void MultMM(
      const T alpha, const GenSymBandMatrix<Ta>& A, 
      const GenSymBandMatrix<Tb>& B, const BandMatrixView<T>& C)
  // C (+)= alpha * A * B
  {
#ifdef XTEST
    TMVAssert(A.HermOK());
#endif
    TMVAssert(A.size() == C.colsize());
    TMVAssert(A.size() == B.colsize());
    TMVAssert(B.rowsize() == C.rowsize());
#ifdef XDEBUG
    //cout<<"Start MultMM: alpha = "<<alpha<<endl;
    //cout<<"A = "<<A.cptr()<<"  "<<TypeText(A)<<"  "<<A<<endl;
    //cout<<"B = "<<B.cptr()<<"  "<<TypeText(B)<<"  "<<B<<endl;
    //cout<<"C = "<<C.cptr()<<"  "<<TypeText(C)<<"  "<<C<<endl;
    Matrix<Ta> A0 = A;
    Matrix<Tb> B0 = B;
    Matrix<T> C0 = C;
    Matrix<T> C2 = alpha*A0*B0;
    if (add) C2 += C0;
#endif

    if (C.colsize() > 0 && C.rowsize() > 0) {
      if (alpha == T(0)) {
        if (!add) C.Zero();
      }
      else if (C.isconj()) {
        MultMM<add>(CONJ(alpha),A.Conjugate(),B.Conjugate(),C.Conjugate());
      }
      else {
        if (A.nlo() > B.nlo()) {
          if (IMAG(alpha) == RealType(T)(0)) {
            BandMatrix<Tb> B2 = REAL(alpha)*B;
            MultMM<add>(T(1),A,B2,C);
          } else {
            BandMatrix<T> B2 = alpha*B;
            MultMM<add>(T(1),A,B2,C);
          }
        } else {
          if (IMAG(alpha) == RealType(T)(0)) {
            BandMatrix<Ta> A2 = REAL(alpha)*A;
            MultMM<add>(T(1),B.Transpose(),A2.Transpose(),C.Transpose());
          } else {
            BandMatrix<T> A2 = alpha*A;
            MultMM<add>(T(1),B.Transpose(),A2.Transpose(),C.Transpose());
          }
        }
      }
    }

#ifdef XDEBUG
    //cout<<"Done: C = "<<C<<endl;
    if (Norm(C-C2) > 0.001*(ABS(alpha)*Norm(A0)*Norm(B0)+
          (add?Norm(C0):RealType(T)(0)))) {
      cerr<<"MultMM: alpha = "<<alpha<<endl;
      cerr<<"add = "<<add<<endl;
      cerr<<"A = "<<TypeText(A)<<"  "<<A0<<endl;
      cerr<<"B = "<<TypeText(B)<<"  "<<B0<<endl;
      cerr<<"C = "<<TypeText(C)<<"  "<<C0<<endl;
      cerr<<"--> C = "<<C<<endl;
      cerr<<"C2 = "<<C2<<endl;
      abort();
    }
#endif
  }

#define InstFile "TMV_MultsBB.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv

