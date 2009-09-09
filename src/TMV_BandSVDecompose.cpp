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
#include "TMV_BandSVDiv.h"
#include "TMV_BandSVD.h"
#include "TMV_BandMatrix.h"
#include "TMV_DiagMatrix.h"
#include "TMV_Householder.h"

//#define XDEBUG

#ifdef XDEBUG
#include "TMV_VectorArith.h"
#include "TMV_MatrixArith.h"
#include "TMV_BandMatrixArith.h"
#include <iostream>
using std::cerr;
using std::endl;
#endif

namespace tmv {

#define RT RealType(T)

  template <class T> static void MakeBidiagReal(
      Vector<T>& Udiag, Vector<T>& Vdiag, 
      const GenVector<T>& cD, const GenVector<T>& cE,
      const VectorView<T>& D, const VectorView<T>& E, T& )
  {
    TMVAssert(Vdiag.size() == Udiag.size());
    TMVAssert(cD.size() == D.size());
    TMVAssert(cE.size() == E.size());
    TMVAssert(D.size() == Udiag.size());
    TMVAssert(E.size() == D.size()-1);
    TMVAssert(D.step() == 1);
    TMVAssert(E.step() == 1);

    Udiag.SetAllTo(T(1));
    Vdiag.SetAllTo(T(1));
    D = cD;
    E = cE;
  }

  template <class T> static void MakeBidiagReal(
      Vector<std::complex<T> >& Udiag, Vector<std::complex<T> >& Vdiag, 
      const GenVector<std::complex<T> >& cD, 
      const GenVector<std::complex<T> >& cE,
      const VectorView<T>& D, const VectorView<T>& E, 
      std::complex<T>& signdet)
  {
    TMVAssert(Vdiag.size() == Udiag.size());
    TMVAssert(cD.size() == D.size());
    TMVAssert(cE.size() == E.size());
    TMVAssert(D.size() == Udiag.size());
    TMVAssert(E.size() == D.size()-1);
    TMVAssert(D.step() == 1);
    TMVAssert(E.step() == 1);

    const int N = D.size();

    std::complex<T>* Uj = Udiag.ptr();
    std::complex<T>* Vj = Vdiag.ptr();
    T* Dj = D.ptr();
    T* Ej = E.ptr();
    Vector<std::complex<T> > xcD = cD;
    Vector<std::complex<T> > xcE = cE;
    const std::complex<T>* cDj = xcD.cptr();
    const std::complex<T>* cEj = xcE.cptr();
#ifdef TMVFLDEBUG
    TMVAssert(Vj >= Vdiag.first);
    TMVAssert(Vj < Vdiag.last);
#endif
    *Vj = T(1);
    std::complex<T> newcDj = *cDj;
    for(int j=0;j<N-1;++j,++Uj,++Dj,++Ej,++cEj) {
#ifdef TMVFLDEBUG
      TMVAssert(Dj >= D.first);
      TMVAssert(Dj < D.last);
      TMVAssert(Ej >= E.first);
      TMVAssert(Ej < E.last);
      TMVAssert(Uj >= Udiag.first);
      TMVAssert(Uj < Udiag.last);
#endif
      *Dj = ABS(newcDj);
      *Uj = SIGN(newcDj,*Dj);
      std::complex<T> newcEj = CONJ(*Uj)* *cEj;
      *Ej = ABS(newcEj);
      ++Vj; // Now Vdiag(j+1)
#ifdef TMVFLDEBUG
      TMVAssert(Vj >= Vdiag.first);
      TMVAssert(Vj < Vdiag.last);
#endif
      *Vj = SIGN(newcEj,*Ej);
      ++cDj; // Now cd(j+1)
      newcDj = CONJ(*Vj)* *cDj;
    }
#ifdef TMVFLDEBUG
    TMVAssert(Dj >= D.first);
    TMVAssert(Dj < D.last);
    TMVAssert(Uj >= Udiag.first);
    TMVAssert(Uj < Udiag.last);
#endif
    *Dj = ABS(newcDj);
    *Uj = SIGN(newcDj,*Dj);
    std::complex<T> su, sv;
    DiagMatrixViewOf(Udiag).LogDet(&su);
    DiagMatrixViewOf(Vdiag).LogDet(&sv);
    signdet *= su*sv;
  }

  template <class T> static void NonLapBidiagonalize(
      const GenBandMatrix<T>& A,
      MVP<T> U, const VectorView<RT>& D,
      const VectorView<RT>& E, MVP<T> V, RT& logdet, T& signdet)
  {
    // Decompose A into U B V
    // The Bidiagonal Matrix B is stored as two vectors: D, E
    // D is the diagonal, E is the super-diagonal
    // We use Householder reflections to reduce A to the bidiagonal form:

    TMVAssert(A.rowsize() <= A.colsize());
    TMVAssert(A.rowsize() > 0);
    if (U) {
      TMVAssert(U->colsize() == A.colsize());
      TMVAssert(U->rowsize() == A.rowsize());
    } 
    if (V) {
      TMVAssert(V->colsize() == A.rowsize());
      TMVAssert(V->rowsize() == A.rowsize());
    }
    TMVAssert(D.size() == A.rowsize());
    TMVAssert(D.size() == E.size()+1);

    const int M = A.colsize();
    const int N = A.rowsize();

    const int nlo = A.nlo();
    const int nhi = A.nhi();

    if (nlo == 0 && nhi == 1) {
      Vector<T> Ud(N);
      Vector<T> Vd(N);
      MakeBidiagReal(Ud,Vd,A.diag(),A.diag(1),D,E,signdet);
      if (U) {
	U->Zero();
	U->diag() = Ud;
      }
      if (V) {
	V->Zero();
	V->diag() = Vd;
      }
    } else if (A.IsSquare() && nlo == 1 && nhi == 0) {
      Vector<T> Ud(N);
      Vector<T> Vd(N);
      MakeBidiagReal(Ud,Vd,A.diag().Reverse(),A.diag(-1).Reverse(),D,E,signdet);
      if (U) {
	U->Zero();
	U->SubVector(N-1,0,-1,1,N) = Ud;
      }
      if (V) {
	V->Zero();
	V->SubVector(0,N-1,1,-1,N) = Vd;
      }
    } else {
      auto_ptr<Matrix<T,ColMajor> > UU(0);
      auto_ptr<MatrixView<T> > U1(0);
      if (U) {
	*U = A;
	U1.reset(new MatrixView<T>(U->View()));
      } else {
	UU.reset(new Matrix<T,ColMajor>(A));
	U1.reset(new MatrixView<T>(UU->View()));
      }

      std::vector<int> vec(N), ver(N-1);
      int endcol = nlo+1;
      Vector<T> Ubeta(N);
      Vector<T> Vbeta(N-1);

      T* Ubj = Ubeta.ptr();
      for(int j=0;j<N-1;++j,++Ubj) {
	vec[j] = endcol;
	int endrow = MIN(endcol+nhi,N);
	ver[j] = endrow;
#ifdef TMVFLDEBUG
	TMVAssert(Ubj >= Ubeta.first);
	TMVAssert(Ubj < Ubeta.last);
#endif
	*Ubj = Householder_Reflect(U1->SubMatrix(j,endcol,j,endrow),signdet);
	if (endcol < M) endcol = MIN(endrow+nlo,M);
	Vbeta(j) = Householder_Reflect(U1->Transpose().SubMatrix(
	      j+1,endrow,j,endcol),signdet);
      }
      vec[N-1] = endcol;
#ifdef TMVFLDEBUG
      TMVAssert(Ubj >= Ubeta.first);
      TMVAssert(Ubj < Ubeta.last);
#endif
      *Ubj = Householder_Reflect(U1->SubMatrix(N-1,endcol,N-1,N),signdet);

      // Now U stores Householder vectors for U in lower diagonal columns (HLi)
      // and Householder vectors for V in upper diagonal rows (HRi)
      // except for the bidiagonal which is the bidiagonal we want:
#ifdef XTEST
      if (IsComplex(T())) {
	TMVAssert(NormInf(U1->diag().Imag()) == RT(0));
	TMVAssert(NormInf(U1->diag(1).Imag()) == RT(0));
      }
#endif
      D = U1->diag().Real();
      E = U1->diag(1).Real();

      if (V) {
	V->SetToIdentity();
	for (int j=N-2;j>=0;--j) {
	  V->row(j+1,j+2,ver[j]) = U1->row(j,j+2,ver[j]);
	  Householder_Unpack(V->Transpose().SubMatrix(j+1,ver[j],j+1,N),
	      Vbeta(j));
	}
      }

      if (U) {
	U->diag().Zero();
	U->diag(1).Zero();
	// Ubj is currently &U(N-1)
	Householder_Unpack(U->SubMatrix(N-1,vec[N-1],N-1,N),*Ubj);
	for (int j=N-2;j>=0;--j) {
	  U->row(j,j,ver[j]).Zero();
	  Householder_Unpack(U->SubMatrix(j,vec[j],j,N),*(--Ubj));
	}
      }
    }
    if (signdet != T(0)) {
      RT s;
      logdet += DiagMatrixViewOf(D).LogDet(&s);
      signdet *= s;
    }
  }

#ifdef LAP
  template <class T> static inline void LapBidiagonalize(
      const GenBandMatrix<T>& A,
      MVP<T> U, const VectorView<RT>& D,
      const VectorView<RT>& E, MVP<T> V, RT& logdet, T& signdet)
  { NonLapBidiagonalize(A,U,D,E,V,logdet,signdet); }
#ifdef INST_DOUBLE
  template <> void LapBidiagonalize(const GenBandMatrix<double>& A,
      MVP<double> U, const VectorView<double>& D,
      const VectorView<double>& E, MVP<double> V, 
      double& logdet, double& signdet)
  {
    TMVAssert(A.rowsize() == A.colsize());
    // The Lap routines can do NonSquare matrices, but they want to
    // write out to a square (MxM) U matrix which is larger than
    // what we have stored here.
    TMVAssert(A.rowsize() > 0);
    if (U) {
      TMVAssert(U->colsize() == A.colsize());
      TMVAssert(U->rowsize() == A.rowsize());
      TMVAssert(U->iscm());
      TMVAssert(U->ct() == NonConj);
    }
    if (V) {
      TMVAssert(V->colsize() == A.rowsize());
      TMVAssert(V->rowsize() == A.rowsize());
      TMVAssert(V->iscm());
      TMVAssert(V->ct() == NonConj);
    }
    TMVAssert(D.size() == A.rowsize());
    TMVAssert(E.size() == D.size()-1);
    TMVAssert(D.step()==1);
    TMVAssert(E.step()==1);
    TMVAssert(D.ct() == NonConj);
    TMVAssert(E.ct() == NonConj);

    int m = A.colsize();
    int n = A.rowsize();
    int ncc = 0;
    int kl = A.nlo();
    int ku = A.nhi();
#ifndef LAPNOWORK
    int lwork = 2*MAX(m,n);
    double* work = LAP_DWork(lwork);
#endif
    // LAP version overwrites original BandMatrix with crap.
    // Hence, copy BandMatrix before running.
    BandMatrix<double,ColMajor> A2 = A;
    int lda = A2.diagstep();
    int ldu = U ? U->stepj() : 1;
    int ldv = V ? V->stepj() : 1;
    char vect = U ? V ? 'B' : 'Q' : V ? 'P' : 'N';
    double* VV = V ? V->ptr() : 0;
    double* UU = U ? U->ptr() : 0;

    LAPNAME(dgbbrd) (LAPCM LAPV(vect),LAPV(m),LAPV(n),LAPV(ncc),
	LAPV(kl),LAPV(ku),
	LAPP(A2.cptr()-A.nhi()),LAPV(lda),LAPP(D.ptr()),LAPP(E.ptr()),
	LAPP(UU),LAPV(ldu),LAPP(VV),LAPV(ldv),
	0,LAPV(n) LAPWK(work) LAPINFO LAP1 );

    if (signdet != double(0)) {
      double s;
      logdet += DiagMatrixViewOf(D).LogDet(&s);
      signdet *= s;
    }
    LAP_Results("dgbbrd");
  }
  template <> void LapBidiagonalize(
      const GenBandMatrix<std::complex<double> >& A,
      MVP<std::complex<double> > U, const VectorView<double>& D,
      const VectorView<double>& E, MVP<std::complex<double> > V, 
      double& logdet, std::complex<double>& signdet)
  {
    TMVAssert(A.rowsize() == A.colsize());
    TMVAssert(A.rowsize() > 0);
    if (U) {
      TMVAssert(U->colsize() == A.colsize());
      TMVAssert(U->rowsize() == A.rowsize());
      TMVAssert(U->iscm());
      TMVAssert(U->ct() == NonConj);
    }
    if (V) {
      TMVAssert(V->colsize() == A.rowsize());
      TMVAssert(V->rowsize() == A.rowsize());
      TMVAssert(V->iscm());
      TMVAssert(V->ct() == NonConj);
    }
    TMVAssert(D.size() == A.rowsize());
    TMVAssert(E.size() == D.size()-1);
    TMVAssert(D.step()==1);
    TMVAssert(E.step()==1);
    TMVAssert(D.ct() == NonConj);
    TMVAssert(E.ct() == NonConj);

    int m = A.colsize();
    int n = A.rowsize();
    int o = 0;
    int kl = A.nlo();
    int ku = A.nhi();
#ifndef LAPNOWORK
    int lwork = MAX(m,n);
    std::complex<double>* work = LAP_ZWork(lwork);
    double* rwork = LAP_DWork(lwork);
#endif
    BandMatrix<std::complex<double>,ColMajor> A2 = A;
    int lda = A2.diagstep();
    int ldu = U ? U->stepj() : 1;
    int ldv = V ? V->stepj() : 1;
    char vect = U ? V ? 'B' : 'Q' : V ? 'P' : 'N';
    std::complex<double>* VV = V ? V->ptr() : 0;
    std::complex<double>* UU = U ? U->ptr() : 0;

    LAPNAME(zgbbrd) (LAPCM LAPV(vect),LAPV(m),LAPV(n),LAPV(o),
	LAPV(kl),LAPV(ku),LAPP(A2.cptr()-A.nhi()),LAPV(lda),
	LAPP(D.ptr()),LAPP(E.ptr()),LAPP(UU),LAPV(ldu),
	LAPP(VV),LAPV(ldv),0,LAPV(n)
	LAPWK(work) LAPWK(rwork) LAPINFO LAP1);

    // OK, I lied, the output A2 isn't complete crap.  Its diagonal
    // holds a version of D which includes all the complex arguments
    // so it can be used to find the determinant.
    if (signdet != double(0)) {
      std::complex<double> s;
      logdet += DiagMatrixViewOf(A2.diag()).LogDet(&s);
      signdet *= s;
    }
    LAP_Results("zgbbrd");
  }
#endif
#ifdef INST_FLOAT
  template <> void LapBidiagonalize(const GenBandMatrix<float>& A,
      MVP<float> U, const VectorView<float>& D,
      const VectorView<float>& E, MVP<float> V, 
      float& logdet, float& signdet)
  {
    TMVAssert(A.rowsize() == A.colsize());
    // The Lap routines can do NonSquare matrices, but they want to
    // write out to a square (MxM) U matrix which is larger than
    // what we have stored here.
    TMVAssert(A.rowsize() > 0);
    if (U) {
      TMVAssert(U->colsize() == A.colsize());
      TMVAssert(U->rowsize() == A.rowsize());
      TMVAssert(U->iscm());
      TMVAssert(U->ct() == NonConj);
    }
    if (V) {
      TMVAssert(V->colsize() == A.rowsize());
      TMVAssert(V->rowsize() == A.rowsize());
      TMVAssert(V->iscm());
      TMVAssert(V->ct() == NonConj);
    }
    TMVAssert(D.size() == A.rowsize());
    TMVAssert(E.size() == D.size()-1);
    TMVAssert(D.step()==1);
    TMVAssert(E.step()==1);
    TMVAssert(D.ct() == NonConj);
    TMVAssert(E.ct() == NonConj);

    int m = A.colsize();
    int n = A.rowsize();
    int ncc = 0;
    int kl = A.nlo();
    int ku = A.nhi();
#ifndef LAPNOWORK
    int lwork = 2*MAX(m,n);
    float* work = LAP_SWork(lwork);
#endif
    BandMatrix<float,ColMajor> A2 = A;
    int lda = A2.diagstep();
    int ldu = U ? U->stepj() : 1;
    int ldv = V ? V->stepj() : 1;
    char vect = U ? V ? 'B' : 'Q' : V ? 'P' : 'N';
    float* VV = V ? V->ptr() : 0;
    float* UU = U ? U->ptr() : 0;

    LAPNAME(sgbbrd) (LAPCM LAPV(vect),LAPV(m),LAPV(n),LAPV(ncc),
	LAPV(kl),LAPV(ku),
	LAPP(A2.cptr()-A.nhi()),LAPV(lda),LAPP(D.ptr()),LAPP(E.ptr()),
	LAPP(UU),LAPV(ldu),LAPP(VV),LAPV(ldv),
	0,LAPV(n) LAPWK(work) LAPINFO LAP1 );

    if (signdet != float(0)) {
      float s;
      logdet += DiagMatrixViewOf(D).LogDet(&s);
      signdet *= s;
    }
    LAP_Results("sgbbrd");
  }
  template <> void LapBidiagonalize(
      const GenBandMatrix<std::complex<float> >& A,
      MVP<std::complex<float> > U, const VectorView<float>& D,
      const VectorView<float>& E, MVP<std::complex<float> > V, 
      float& logdet, std::complex<float>& signdet)
  {
    TMVAssert(A.rowsize() == A.colsize());
    TMVAssert(A.rowsize() > 0);
    if (U) {
      TMVAssert(U->colsize() == A.colsize());
      TMVAssert(U->rowsize() == A.rowsize());
      TMVAssert(U->iscm());
      TMVAssert(U->ct() == NonConj);
    }
    if (V) {
      TMVAssert(V->colsize() == A.rowsize());
      TMVAssert(V->rowsize() == A.rowsize());
      TMVAssert(V->iscm());
      TMVAssert(V->ct() == NonConj);
    }
    TMVAssert(D.size() == A.rowsize());
    TMVAssert(E.size() == D.size()-1);
    TMVAssert(D.step()==1);
    TMVAssert(E.step()==1);
    TMVAssert(D.ct() == NonConj);
    TMVAssert(E.ct() == NonConj);

    int m = A.colsize();
    int n = A.rowsize();
    int o = 0;
    int kl = A.nlo();
    int ku = A.nhi();
#ifndef LAPNOWORK
    int lwork = MAX(m,n);
    std::complex<float>* work = LAP_CWork(lwork);
    float* rwork = LAP_SWork(lwork);
#endif
    BandMatrix<std::complex<float>,ColMajor> A2 = A;
    int lda = A2.diagstep();
    int ldu = U ? U->stepj() : 1;
    int ldv = V ? V->stepj() : 1;
    char vect = U ? V ? 'B' : 'Q' : V ? 'P' : 'N';
    std::complex<float>* VV = V ? V->ptr() : 0;
    std::complex<float>* UU = U ? U->ptr() : 0;

    LAPNAME(cgbbrd) (LAPCM LAPV(vect),LAPV(m),LAPV(n),LAPV(o),
	LAPV(kl),LAPV(ku),LAPP(A2.cptr()-A.nhi()),LAPV(lda),
	LAPP(D.ptr()),LAPP(E.ptr()),LAPP(UU),LAPV(ldu),
	LAPP(VV),LAPV(ldv),0,LAPV(n)
	LAPWK(work) LAPWK(rwork) LAPINFO LAP1);

    if (signdet != float(0)) {
      std::complex<float> s;
      logdet += DiagMatrixViewOf(A2.diag()).LogDet(&s);
      signdet *= s;
    }
    LAP_Results("cgbbrd");
  }
#endif
#endif
  template <class T> static void Bidiagonalize(
      const GenBandMatrix<T>& A,
      MVP<T> U, const VectorView<RT>& D,
      const VectorView<RT>& E, MVP<T> V, RT& logdet, T& signdet)
  {
    TMVAssert(A.rowsize() <= A.colsize());
    TMVAssert(A.rowsize() > 0);
    if (U) {
      TMVAssert(U->colsize() == A.colsize());
      TMVAssert(U->rowsize() == A.rowsize());
      TMVAssert(U->ct() == NonConj);
    }
    if (V) {
      TMVAssert(V->colsize() == A.rowsize());
      TMVAssert(V->rowsize() == A.rowsize());
      TMVAssert(V->ct() == NonConj);
    }
    TMVAssert(D.size() == A.rowsize());
    TMVAssert(E.size() == D.size()-1);
    TMVAssert(D.step()==1);
    TMVAssert(E.step()==1);
    TMVAssert(D.ct() == NonConj);
    TMVAssert(E.ct() == NonConj);

#ifdef XDEBUG
    Matrix<T> A0(A);
    BandMatrix<T> A2(A);
    Vector<RT> D2(D);
    Vector<RT> E2(E);
    Matrix<T> U2(A.colsize(),A.rowsize());
    Matrix<T> V2(D.size(),D.size());
    RT logdet2(0);
    T signdet2(1);
    NonLapBidiagonalize<T>(A2,U2.View(),D2.View(),E2.View(),V2.View(),
	logdet2,signdet2);
#endif

    if (A.rowsize() > 0) {
      TMVAssert(E.size() == D.size()-1);
#ifdef LAP
      if (A.IsSquare() && (!U || U->iscm()) && (!V || V->iscm())) 
	LapBidiagonalize(A,U,D,E,V,logdet,signdet);
      else 
#endif
	NonLapBidiagonalize(A,U,D,E,V,logdet,signdet);
    }
#ifdef XDEBUG
    if (U && V) {
      Matrix<T> UBV = *U*UpperBiDiagMatrix(D,E)*(*V);
      if (Norm(UBV-A0) > 0.001*Norm(A0)) {
	cerr<<"Band Bidiagonalize:\n";
	cerr<<"A = "<<Type(A)<<"  "<<A0<<endl;
	cerr<<"-> D = "<<D<<endl;
	cerr<<"Nonlap D = "<<D2<<endl;
	cerr<<"Norm(diff) = "<<Norm(D-D2)<<endl;
	cerr<<"E = "<<E<<endl;
	cerr<<"Nonlap E = "<<E2<<endl;
	cerr<<"Norm(diff) = "<<Norm(E-E2)<<endl;
	cerr<<"U = "<<*U<<endl;
	cerr<<"V = "<<*V<<endl;
	cerr<<"U2 = "<<U2<<endl;
	cerr<<"V2 = "<<V2<<endl;
	cerr<<"A0 = "<<A0<<endl;
	cerr<<"UBV = "<<UBV<<endl;
	cerr<<"Norm(UBV-A0) = "<<Norm(UBV-A0)<<endl;
	abort();
      }
    }
#endif
  }

  template <class T> void SV_Decompose(
      const GenBandMatrix<T>& A,
      MVP<T> U, const DiagMatrixView<RT>& S, MVP<T> V, RT& logdet, T& signdet)
  {
    // Decompose A into U S V
    // where S is a diagonal real matrix, and U,V are unitary matrices.
    // All matrices are square N x N
    // The determinant is kept track of in det.
    //
    // Everything is identical to the regular SVD except for the 
    // Bidiagonal Step.

    TMVAssert(A.rowsize() <= A.colsize());
    TMVAssert(A.rowsize() > 0);
    if (U) {
      TMVAssert(U->rowsize() == A.rowsize());
      TMVAssert(U->colsize() == A.colsize());
      TMVAssert(U->ct() == NonConj);
    } 
    if (V) {
      TMVAssert(V->rowsize() == A.rowsize());
      TMVAssert(V->colsize() == A.rowsize());
      TMVAssert(V->ct() == NonConj);
    }
    TMVAssert(S.size() == A.rowsize());
    TMVAssert(S.diag().ct() == NonConj);

    if (A.nlo() == 0 && A.nhi() == 0) {
      if (U) U->SetToIdentity();
      if (V) V->SetToIdentity();

      if (signdet != T(0)) {
	T s;
	logdet += DiagMatrixViewOf(A.diag()).LogDet(&s);
	signdet *= s;
      }
      const int N = A.rowsize();
      const T* Ajj = A.cptr();
      const int Ads = A.stepi()+A.stepj();
      RT* Sj = S.diag().ptr();
      const int Ss = S.diag().step();
      T* Ujj = U ? U->ptr() : 0;
      const int Uds = U ? (U->stepi()+U->stepj()) : 0;

      if (A.isconj()) {
	for(int j=0;j<N;++j,Ajj+=Ads,Sj+=Ss) {
#ifdef TMVFLDEBUG
	  TMVAssert(Sj >= S.first);
	  TMVAssert(Sj < S.last);
#endif
	  *Sj = ABS(*Ajj);
	  if(U) {
#ifdef TMVFLDEBUG
	    TMVAssert(Ujj >= U->first);
	    TMVAssert(Ujj < U->last);
#endif
	    *Ujj = SIGN(CONJ(*Ajj),*Sj); Ujj += Uds; 
	  }
	}
      } else {
	for(int j=0;j<N;++j,Ajj+=Ads,Sj+=Ss) {
#ifdef TMVFLDEBUG
	  TMVAssert(Sj >= S.first);
	  TMVAssert(Sj < S.last);
#endif
	  *Sj = ABS(*Ajj);
	  if(U) { 
#ifdef TMVFLDEBUG
	    TMVAssert(Ujj >= U->first);
	    TMVAssert(Ujj < U->last);
#endif
	    *Ujj = SIGN(*Ajj,*Sj); Ujj += Uds; 
	  }
	}
      }
      auto_array<int> sortp(new int[N]);
      S.diag().Sort(sortp.get(),DESCEND);
      if (U) U->PermuteCols(sortp.get());
      if (V) V->PermuteRows(sortp.get());
    } else {
      Vector<RT> E(S.size()-1);
      Bidiagonalize(A,U,S.diag(),E.View(),V,logdet,signdet);

      SV_Decompose_From_Bidiagonal(U,S.diag(),E.View(),V);
    }
  }

  template <class T> void SV_Decompose(
      const GenBandMatrix<T>& A,
      const MatrixView<T>& U, const DiagMatrixView<RT>& S, 
      const MatrixView<T>& V)
  { 
    if (U.isconj()) {
      if (V.isconj()) {
	SV_Decompose(A.Conjugate(),U.Conjugate(),S,V.Conjugate());
      } else {
	SV_Decompose(A.Conjugate(),U.Conjugate(),S,V);
	V.ConjugateSelf();
      }
    } else {
      if (V.isconj()) {
	SV_Decompose(A,U,S,V.Conjugate());
	V.ConjugateSelf();
      } else {
	RT ld=0; T d=0; SV_Decompose<T>(A,U,S,V,ld,d); 
      }
    }
  }

  template <class T> void SV_Decompose(
      const GenBandMatrix<T>& A,
      const MatrixView<T>& U, const DiagMatrixView<RT>& S)
  {
    if (U.isconj()) {
      SV_Decompose(A.Conjugate(),U.Conjugate(),S);
    } else {
      RT ld=0; T d=0; SV_Decompose<T>(A,U,S,0,ld,d); 
    }
  }

  template <class T> void SV_Decompose(
      const GenBandMatrix<T>& A,
      const DiagMatrixView<RT>& S, const MatrixView<T>& V)
  {
    if (V.isconj()) {
      SV_Decompose(A.Conjugate(),S,V.Conjugate());
    } else {
      RT ld=0; T d=0; SV_Decompose<T>(A,0,S,V,ld,d); 
    }
  }

  template <class T> void SV_Decompose(
      const GenBandMatrix<T>& A, const DiagMatrixView<RT>& S)
  { RT ld=0; T d=0; SV_Decompose<T>(A,0,S,0,ld,d); }

#undef RT

#define InstFile "TMV_BandSVDecompose.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace mv


