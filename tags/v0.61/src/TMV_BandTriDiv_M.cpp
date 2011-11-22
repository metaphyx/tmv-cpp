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



#include "TMV_Blas.h"
#include "TMV_BandLUDiv.h"
#include "TMV_BandMatrix.h"
#include "TMV_Matrix.h"
#include "TMV_VectorArith.h"
#include "TMV_MatrixArith.h"

//#define XDEBUG

#ifdef XDEBUG
#include "TMV_TriDiv.h"
#include "TMV_TriMatrixArith.h"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#endif

namespace tmv {

  template <bool ua, class T, class Ta> static void RowUpperTriLDivEq(
      const GenBandMatrix<Ta>& A, const MatrixView<T>& B)
  {
    TMVAssert(B.isrm());
    TMVAssert(A.IsSquare());
    TMVAssert(B.colsize() == A.colsize());
    TMVAssert(A.colsize() > 0);
    TMVAssert(B.rowsize() > 0);
    TMVAssert(A.nlo() == 0);
    TMVAssert(B.ct() == NonConj);

    int N = B.colsize();

    int k = A.nhi();
    if (ua) {
      for(int i=N-1; i>=0; --i) {
	B.row(i) -= A.row(i,i+1,N) * B.Rows(i+1,N);
	if (k > 0) --k; else --N;
      }
    } else {
      const int ds = A.diagstep();
      const Ta* Aii = A.cptr() + (N-1)*ds;
      for(int i=N-1; i>=0; --i,Aii-=ds) {
	B.row(i) -= A.row(i,i+1,N) * B.Rows(i+1,N);
	if (*Aii==Ta(0)) 
	  throw SingularBandLU<Ta>(A);
	B.row(i) /= (A.isconj() ? CONJ(*Aii) : *Aii);
	if (k > 0) --k; else --N;
      }
    } 
  }

  template <bool ua, class T, class Ta> static void ColUpperTriLDivEq(
      const GenBandMatrix<Ta>& A, const MatrixView<T>& B)
  {
    TMVAssert(B.isrm());
    TMVAssert(A.IsSquare());
    TMVAssert(B.colsize() == A.colsize());
    TMVAssert(A.colsize() > 0);
    TMVAssert(B.rowsize() > 0);
    TMVAssert(A.nlo() == 0);
    TMVAssert(int(A.colsize())>A.nhi());
    TMVAssert(B.ct() == NonConj);

    int N = A.colsize();

    int i1 = N-1-A.nhi();
    if (ua) {
      for(int j=N-1; j>0; --j) {
	B.Rows(i1,j) -= A.col(j,i1,j) ^ B.row(j);
	if (i1 > 0) --i1;
      }
    } else {
      const int ds = A.diagstep();
      const Ta* Ajj = A.cptr() + (N-1)*ds;
      for(int j=N-1; j>=0; --j,Ajj-=ds) {
	if (*Ajj==Ta(0)) 
	  throw SingularBandLU<Ta>(A);
	B.row(j) /= (A.isconj() ? CONJ(*Ajj) : *Ajj);
	B.Rows(i1,j) -= A.col(j,i1,j) ^ B.row(j);
	if (i1 > 0) --i1;
      }
    } 
  }

  template <bool ua, class T, class Ta> static void RowLowerTriLDivEq(
      const GenBandMatrix<Ta>& A, const MatrixView<T>& B)
  {
    TMVAssert(B.isrm());
    TMVAssert(A.IsSquare());
    TMVAssert(B.colsize() == A.colsize());
    TMVAssert(A.colsize() > 0);
    TMVAssert(B.rowsize() > 0);
    TMVAssert(A.nhi() == 0);
    TMVAssert(B.ct() == NonConj);

    const int N = B.colsize();

    int i1=0;
    int k=A.nlo();
    if (ua) {
      for(int i=0; i<N; ++i) {
	B.row(i) -= A.row(i,i1,i) * B.Rows(i1,i);
	if (k>0) --k; else ++i1;
      }
    } else {
      const int ds = A.diagstep();
      const Ta* Aii = A.cptr();
      for(int i=0; i<N; ++i,Aii+=ds) {
	B.row(i) -= A.row(i,i1,i) * B.Rows(i1,i);
	if (*Aii==Ta(0)) 
	  throw SingularBandLU<Ta>(A);
	B.row(i) /= (A.isconj() ? CONJ(*Aii) : *Aii);
	if (k>0) --k; else ++i1;
      }
    }
  }

  template <bool ua, class T, class Ta> static void ColLowerTriLDivEq(
      const GenBandMatrix<Ta>& A, const MatrixView<T>& B)
  {
    TMVAssert(B.isrm());
    TMVAssert(A.IsSquare());
    TMVAssert(B.colsize() == A.colsize());
    TMVAssert(A.colsize() > 0);
    TMVAssert(B.rowsize() > 0);
    TMVAssert(A.nhi() == 0);
    TMVAssert(B.ct() == NonConj);

    const int N = B.colsize();

    int i2=A.nlo()+1;
    if (ua) {
      for(int j=0; j<N; ++j) {
	B.Rows(j+1,i2) -= A.col(j,j+1,i2) ^ B.row(j);
	if (i2 < N) ++i2;
      }
    } else {
      const int ds = A.diagstep();
      const Ta* Ajj = A.cptr();
      for(int j=0; j<N; ++j,Ajj+=ds) {
	if (*Ajj==Ta(0)) 
	  throw SingularBandLU<Ta>(A);
	B.row(j) /= (A.isconj() ? CONJ(*Ajj) : *Ajj);
	B.Rows(j+1,i2) -= A.col(j,j+1,i2) ^ B.row(j);
	if (i2 < N) ++i2;
      }
    }
  }

  template <bool ua, class T, class Ta> static void NonLapTriLDivEq(
      const GenBandMatrix<Ta>& A, const MatrixView<T>& B)
  {
    TMVAssert(A.IsSquare());
    TMVAssert(B.colsize() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.colsize() > 0);
    TMVAssert(B.rowsize() > 1);
    TMVAssert(A.nhi() < int(A.colsize()));
    TMVAssert(A.nlo() < int(A.colsize()));
    TMVAssert(B.ct() == NonConj);

    if (B.isrm()) 
      if (A.nlo()==0) 
	if (A.isrm()) RowUpperTriLDivEq<ua>(A,B);
	else if (A.iscm()) ColUpperTriLDivEq<ua>(A,B);
	else RowUpperTriLDivEq<ua>(A,B);
      else 
	if (A.isrm()) RowLowerTriLDivEq<ua>(A,B);
	else if (A.iscm()) ColLowerTriLDivEq<ua>(A,B);
	else RowLowerTriLDivEq<ua>(A,B);
    else {
      const int N = B.rowsize();
      for(int j=0;j<N;++j) {
	TriLDivEq(A,B.col(j),ua?UnitDiag:NonUnitDiag);
      }
    }
  }

#ifdef LAP
  template <class T, class Ta> static inline void LapTriLDivEq(
      const GenBandMatrix<Ta>& A, const MatrixView<T>& B, DiagType dt)
  { 
    if (dt == UnitDiag) NonLapTriLDivEq<true>(A,B);
    else NonLapTriLDivEq<false>(A,B);
  }
#ifdef INST_DOUBLE
  template <> void LapTriLDivEq(
      const GenBandMatrix<double>& A, const MatrixView<double>& B, DiagType dt)
  {
    TMVAssert(A.IsSquare());
    TMVAssert(B.colsize() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.colsize() > 0);
    TMVAssert(B.rowsize() > 1);
    TMVAssert(A.nhi() < int(A.colsize()));
    TMVAssert(A.nlo() < int(A.colsize()));
    TMVAssert(A.ct() == NonConj);
    TMVAssert(B.ct() == NonConj);
    TMVAssert(A.iscm() || A.isrm());
    TMVAssert(B.iscm());
    int n = A.colsize();
    int kd = A.nlo()==0 ? A.nhi() : A.nlo();
    int nrhs = B.rowsize();
    int aoffset = 
      (A.nlo()==0 && A.iscm()) ? A.nhi() :
      (A.nhi()==0 && A.isrm()) ? A.nlo() : 0;
    int lda = A.diagstep();
    int ldb = B.stepj();
    LAPNAME(dtbtrs) (LAPCM (A.iscm()?A.nlo():A.nhi())==0?LAPCH_UP:LAPCH_LO,
	A.iscm()?LAPCH_NT:LAPCH_T, dt==UnitDiag?LAPCH_U:LAPCH_NU,
	LAPV(n),LAPV(kd),LAPV(nrhs), LAPP(A.cptr()-aoffset),LAPV(lda),
	LAPP(B.ptr()),LAPV(ldb) LAPINFO LAP1 LAP1 LAP1);
    LAP_Results("dtbtrs");
  }
  template <> void LapTriLDivEq(
      const GenBandMatrix<std::complex<double> >& A,
      const MatrixView<std::complex<double> >& B, DiagType dt)
  {
    TMVAssert(A.IsSquare());
    TMVAssert(B.colsize() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.colsize() > 0);
    TMVAssert(B.rowsize() > 1);
    TMVAssert(A.nhi() < int(A.colsize()));
    TMVAssert(A.nlo() < int(A.colsize()));
    TMVAssert(B.ct() == NonConj);
    TMVAssert(A.iscm() || A.isrm());
    TMVAssert(B.iscm());
    int n = A.colsize();
    int kd = A.nlo()==0 ? A.nhi() : A.nlo();
    int nrhs = B.rowsize();
    int aoffset = 
      (A.nlo()==0 && A.iscm()) ? A.nhi() :
      (A.nhi()==0 && A.isrm()) ? A.nlo() : 0;
    int lda = A.diagstep();
    int ldb = B.stepj();
    if (A.iscm() && A.isconj()) {
      B.ConjugateSelf();
      LAPNAME(ztbtrs) (LAPCM A.nlo()==0?LAPCH_UP:LAPCH_LO,
	  LAPCH_NT, dt==UnitDiag?LAPCH_U:LAPCH_NU,
	  LAPV(n),LAPV(kd),LAPV(nrhs), LAPP(A.cptr()-aoffset),LAPV(lda),
	  LAPP(B.ptr()),LAPV(ldb) LAPINFO LAP1 LAP1 LAP1);
      B.ConjugateSelf();
    } else {
      LAPNAME(ztbtrs) (LAPCM (A.iscm()?A.nlo():A.nhi())==0?LAPCH_UP:LAPCH_LO,
	  A.iscm()?LAPCH_NT:A.isconj()?LAPCH_CT:LAPCH_T, 
	  dt==UnitDiag?LAPCH_U:LAPCH_NU,
	  LAPV(n),LAPV(kd),LAPV(nrhs), LAPP(A.cptr()-aoffset),LAPV(lda),
	  LAPP(B.ptr()),LAPV(ldb) LAPINFO LAP1 LAP1 LAP1);
    }
    LAP_Results("ztbtrs");
  }
#endif
#ifdef INST_FLOAT
  template <> void LapTriLDivEq(
      const GenBandMatrix<float>& A, const MatrixView<float>& B, DiagType dt)
  {
    TMVAssert(A.IsSquare());
    TMVAssert(B.colsize() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.colsize() > 0);
    TMVAssert(B.rowsize() > 1);
    TMVAssert(A.nhi() < int(A.colsize()));
    TMVAssert(A.nlo() < int(A.colsize()));
    TMVAssert(A.ct() == NonConj);
    TMVAssert(B.ct() == NonConj);
    TMVAssert(A.iscm() || A.isrm());
    TMVAssert(B.iscm());
    int n = A.colsize();
    int kd = A.nlo()==0 ? A.nhi() : A.nlo();
    int nrhs = B.rowsize();
    int aoffset = 
      (A.nlo()==0 && A.iscm()) ? A.nhi() :
      (A.nhi()==0 && A.isrm()) ? A.nlo() : 0;
    int lda = A.diagstep();
    int ldb = B.stepj();
    LAPNAME(stbtrs) (LAPCM (A.iscm()?A.nlo():A.nhi())==0?LAPCH_UP:LAPCH_LO,
	A.iscm()?LAPCH_NT:LAPCH_T, dt==UnitDiag?LAPCH_U:LAPCH_NU,
	LAPV(n),LAPV(kd),LAPV(nrhs), LAPP(A.cptr()-aoffset),LAPV(lda),
	LAPP(B.ptr()),LAPV(ldb) LAPINFO LAP1 LAP1 LAP1);
    LAP_Results("stbtrs");
  }
  template <> void LapTriLDivEq(
      const GenBandMatrix<std::complex<float> >& A,
      const MatrixView<std::complex<float> >& B, DiagType dt)
  {
    TMVAssert(A.IsSquare());
    TMVAssert(B.colsize() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.colsize() > 0);
    TMVAssert(B.rowsize() > 1);
    TMVAssert(A.nhi() < int(A.colsize()));
    TMVAssert(A.nlo() < int(A.colsize()));
    TMVAssert(B.ct() == NonConj);
    TMVAssert(A.iscm() || A.isrm());
    TMVAssert(B.iscm());
    int n = A.colsize();
    int kd = A.nlo()==0 ? A.nhi() : A.nlo();
    int nrhs = B.rowsize();
    int aoffset = 
      (A.nlo()==0 && A.iscm()) ? A.nhi() :
      (A.nhi()==0 && A.isrm()) ? A.nlo() : 0;
    int lda = A.diagstep();
    int ldb = B.stepj();
    if (A.iscm() && A.isconj()) {
      B.ConjugateSelf();
      LAPNAME(ctbtrs) (LAPCM A.nlo()==0?LAPCH_UP:LAPCH_LO,
	  LAPCH_NT, dt==UnitDiag?LAPCH_U:LAPCH_NU,
	  LAPV(n),LAPV(kd),LAPV(nrhs), LAPP(A.cptr()-aoffset),LAPV(lda),
	  LAPP(B.ptr()),LAPV(ldb) LAPINFO LAP1 LAP1 LAP1);
      B.ConjugateSelf();
    } else {
      LAPNAME(ctbtrs) (LAPCM (A.iscm()?A.nlo():A.nhi())==0?LAPCH_UP:LAPCH_LO,
	  A.iscm()?LAPCH_NT:A.isconj()?LAPCH_CT:LAPCH_T, 
	  dt==UnitDiag?LAPCH_U:LAPCH_NU,
	  LAPV(n),LAPV(kd),LAPV(nrhs), LAPP(A.cptr()-aoffset),LAPV(lda),
	  LAPP(B.ptr()),LAPV(ldb) LAPINFO LAP1 LAP1 LAP1);
    }
    LAP_Results("ctbtrs");
  }
#endif
#endif // LAP

  template <class T, class Ta> void TriLDivEq(
      const GenBandMatrix<Ta>& A, const MatrixView<T>& B, DiagType dt)
  {
#ifdef XDEBUG
    Matrix<T> B0 = B;
    Matrix<T> A0 = A;
    if (dt == UnitDiag) A0.diag().SetAllTo(T(1));
#endif
    TMVAssert(A.IsSquare());
    TMVAssert(B.colsize() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.colsize() > 0);
    if (B.rowsize() == 0) return;
    TMVAssert(A.nhi() < int(A.colsize()));
    TMVAssert(A.nlo() < int(A.colsize()));

    if (B.rowsize() == 1) TriLDivEq(A,B.col(0),dt);
    else if (B.isconj()) 
      TriLDivEq(A.Conjugate(),B.Conjugate(),dt);
    else
#ifdef LAP
      if (!(B.iscm() && B.stepj()>0)) {
	Matrix<T,ColMajor> BB=B;
	TriLDivEq(A,BB.View(),dt);
	B = BB;
      } else if ( !((A.iscm() && A.stepj()>0) || (A.isrm() && A.stepi()>0))) {
	BandMatrix<T,ColMajor> AA = A;
	LapTriLDivEq(AA,B,dt);
      } else 
	LapTriLDivEq(A,B,dt);
#else
      if (dt == UnitDiag) NonLapTriLDivEq<true>(A,B);
      else NonLapTriLDivEq<false>(A,B);
#endif

#ifdef XDEBUG
    Matrix<T> BB = A0*B;
    if (Norm(BB-B0) > 0.001*Norm(B0)*Norm(A0)*Norm(A0.Inverse())) {
      cerr<<"TriLDivEq Matrix:\n";
      cerr<<"A = "<<Type(A)<<A0<<endl;
      cerr<<"B = "<<Type(B)<<B0<<endl;
      cerr<<"--> B = "<<B<<endl;
      cerr<<"A*B = "<<BB<<endl;
      abort();
    }
#endif
  }

#define InstFile "TMV_BandTriDiv_M.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace mv

