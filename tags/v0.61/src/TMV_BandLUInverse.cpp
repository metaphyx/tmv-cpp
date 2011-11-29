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



#include "TMV_BandLUDiv.h"
#include "TMV_BandMatrix.h"
#include "TMV_TriMatrix.h"
#include "TMV_TriMatrixArith.h"
#include "TMV_TriDiv.h"
#include "TMV_DiagMatrix.h"
#include "TMV_Matrix.h"

//#define XDEBUG

#ifdef XDEBUG
#include "TMV_MatrixArith.h"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#endif


#ifdef TMV_BLOCKSIZE
#define TRI_DIV_BLOCKSIZE TMV_BLOCKSIZE
#else
#define TRI_DIV_BLOCKSIZE 64
#endif
 
namespace tmv {

  template <class T, class T1> void LU_Inverse(const GenBandMatrix<T1>& LUx,
      const int* p, const MatrixView<T>& minv)
  {
    TMVAssert(LUx.IsSquare());
    TMVAssert(minv.IsSquare());
    TMVAssert(minv.colsize() == LUx.colsize());
#ifdef XDEBUG
    //cout<<"LUInverse:\n";
    //cout<<"LUx = "<<LUx<<endl;
    //cout<<"p = ";
    //for(size_t i=0;i<LUx.colsize();i++) cout<<p[i]<<" ";
    //cout<<endl;
    LowerTriMatrix<T,UnitDiag> L0(LUx.colsize());
    LU_PackedPL_Unpack(LUx,p,L0.View());
    UpperTriMatrix<T> U0 = BandMatrixViewOf(LUx,0,LUx.nhi());
    Matrix<T> PLU = L0 * U0;
    if (LUx.nlo() > 0) PLU.ReversePermuteRows(p);
    //cout<<"PLU = "<<PLU<<endl;
    Matrix<T> minv2 = PLU.Inverse();
    //cout<<"PLU^-1 = "<<minv2<<endl;
#endif

    if (minv.colsize() > 0) {
      if ( !(minv.iscm() || minv.isrm())) {
	Matrix<T,ColMajor> temp(minv.colsize(),minv.colsize());
	LU_Inverse(LUx,p,temp.View());
	minv = temp;
      } else {
	minv.Zero();
	UpperTriMatrixView<T> U = UpperTriMatrixViewOf(minv);
	U = BandMatrixViewOf(LUx,0,LUx.nhi());
	Tri_Inverse(U,LUx.nhi());
	LU_PackedPL_RDivEq(LUx,p,minv);
      }
    }

#ifdef XDEBUG
    //cout<<"minv = "<<minv<<endl;
    RealType(T) normdiff = Norm(PLU*minv - T(1));
    RealType(T) kappa = Norm(PLU)*Norm(minv);
    if (normdiff > 0.001*kappa*minv.colsize()) {
      cerr<<"LUInverse:\n";
      cerr<<"LUx = "<<LUx<<endl;
      cerr<<"p = ";
      for(size_t i=0;i<LUx.colsize();i++) cerr<<p[i]<<" ";
      cerr<<endl;
      cerr<<"PLU = "<<PLU<<endl;
      cerr<<"minv = "<<minv<<endl;
      cerr<<"minv2 = "<<minv2<<endl;
      cerr<<"m*minv = "<<PLU*minv<<endl;
      cerr<<"minv*m = "<<minv*PLU<<endl;
      cerr<<"Norm(m*minv - 1) = "<<normdiff<<endl;
      cerr<<"kappa = "<<kappa<<endl;
      abort();
    }
#endif
  }

  template <bool unit, class T> static void RecursiveInverse(
      const UpperTriMatrixView<T>& U, int nhi)
  {
    TMVAssert(U.iscm() || U.isrm());
    TMVAssert(unit == U.isunit());

    const int N = U.size();
    const int nb = TRI_DIV_BLOCKSIZE;

    if (N == 1) {
      if (!unit) {
	T*const Uptr = U.ptr();
	if (*Uptr == T(0)) 
	  throw SingularUpperTriMatrix<T>(U);
#ifdef TMVFLDEBUG
	TMVAssert(Uptr >= U.first);
	TMVAssert(Uptr < U.last);
#endif
	*Uptr = RealType(T)(1) / (*Uptr);
      }
    } else {
      int k = N/2;
      if (k > nb) k = k/nb*nb;

      UpperTriMatrixView<T> U00 = U.SubTriMatrix(0,k);
      MatrixView<T> U01 = U.SubMatrix(0,k,k,N);
      UpperTriMatrixView<T> U11 = U.SubTriMatrix(k,N);

      // U00 U01' + U01 U11' = 0
      // U00 U01' = -U01 U11'
      // U01' = -U00' U01 U11'

      RecursiveInverse<unit>(U00,nhi);
      RecursiveInverse<unit>(U11,nhi);

      if (nhi < N-k)
	U01.Cols(0,nhi) = -U00 * U01.Cols(0,nhi);
      else
	U01 = -U00 * U01;
      U01 *= U11;
    }
  }

  template <class T> static inline void DoInverse(
      const UpperTriMatrixView<T>& U, int nhi)
  {
    try {
      if (U.isunit()) RecursiveInverse<true>(U,nhi);
      else RecursiveInverse<false>(U,nhi);
    }
    catch (Singular) {
      throw SingularUpperTriMatrix<T>(U);
    }
  }

  template <class T> void Tri_Inverse(const UpperTriMatrixView<T>& U,
      int nhi)
  {
#ifdef XDEBUG
    Matrix<T> U0(U);
#endif

    if (U.size() > 0) {
      if (nhi == 0) DiagMatrixViewOf(U.diag()).InvertSelf();
      else if (U.iscm() || U.isrm()) DoInverse(U,nhi);
      else {
	UpperTriMatrix<T> temp = U;
	DoInverse(temp.View(),nhi);
	U = temp;
      }
    }
#ifdef XDEBUG
    Matrix<T> eye = U*U0;
    if (Norm(eye-T(1)) > 0.0001*(Norm(U0)+Norm(U))) {
      cerr<<"UpperTriMatrix Inverse:\n";
      cerr<<"U = "<<tmv::Type(U)<<"  "<<U0<<endl;
      cerr<<"Uinv = "<<U<<endl;
      cerr<<"Uinv*U = "<<U*U0<<endl;
      cerr<<"U*Uinv = "<<U0*U<<endl;
      abort();
    }
#endif
  }

#define InstFile "TMV_BandLUInverse.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace mv

