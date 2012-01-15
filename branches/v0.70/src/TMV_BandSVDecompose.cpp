///////////////////////////////////////////////////////////////////////////////
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


#include "TMV_Blas.h"
#include "TMV_BandSVDiv.h"
#include "tmv/TMV_BandSVD.h"
#include "tmv/TMV_BandMatrix.h"
#include "tmv/TMV_DiagMatrix.h"
#include "TMV_Householder.h"

#ifdef XDEBUG
#include "tmv/TMV_VectorArith.h"
#include "tmv/TMV_MatrixArith.h"
#include "tmv/TMV_BandMatrixArith.h"
#include <iostream>
using std::cerr;
using std::endl;
#endif

namespace tmv {

#define RT TMV_RealType(T)

    template <class T> 
    static void MakeBidiagReal(
        Vector<T>& Udiag, Vector<T>& Vtdiag, 
        const GenVector<T>& cD, const GenVector<T>& cE,
        const VectorView<T>& D, const VectorView<T>& E, T& )
    {
        TMVAssert(Vtdiag.size() == Udiag.size());
        TMVAssert(cD.size() == D.size());
        TMVAssert(cE.size() == E.size());
        TMVAssert(D.size() == Udiag.size());
        TMVAssert(E.size() == D.size()-1);
        TMVAssert(D.step() == 1);
        TMVAssert(E.step() == 1);

        Udiag.setAllTo(T(1));
        Vtdiag.setAllTo(T(1));
        D = cD;
        E = cE;
    }

    template <class T> 
    static void MakeBidiagReal(
        Vector<std::complex<T> >& Udiag, Vector<std::complex<T> >& Vtdiag, 
        const GenVector<std::complex<T> >& cD, 
        const GenVector<std::complex<T> >& cE,
        const VectorView<T>& D, const VectorView<T>& E, 
        std::complex<T>& signdet)
    {
        TMVAssert(Vtdiag.size() == Udiag.size());
        TMVAssert(cD.size() == D.size());
        TMVAssert(cE.size() == E.size());
        TMVAssert(D.size() == Udiag.size());
        TMVAssert(E.size() == D.size()-1);
        TMVAssert(D.step() == 1);
        TMVAssert(E.step() == 1);

        const int N = D.size();

        std::complex<T>* Uj = Udiag.ptr();
        std::complex<T>* Vtj = Vtdiag.ptr();
        T* Dj = D.ptr();
        T* Ej = E.ptr();
        Vector<std::complex<T> > xcD = cD;
        Vector<std::complex<T> > xcE = cE;
        const std::complex<T>* cDj = xcD.cptr();
        const std::complex<T>* cEj = xcE.cptr();
#ifdef TMVFLDEBUG
        TMVAssert(Vtj >= Vtdiag._first);
        TMVAssert(Vtj < Vtdiag._last);
#endif
        *Vtj = T(1);
        std::complex<T> newcDj = *cDj;
        for(int j=0;j<N-1;++j,++Uj,++Dj,++Ej,++cEj) {
#ifdef TMVFLDEBUG
            TMVAssert(Dj >= D._first);
            TMVAssert(Dj < D._last);
            TMVAssert(Ej >= E._first);
            TMVAssert(Ej < E._last);
            TMVAssert(Uj >= Udiag._first);
            TMVAssert(Uj < Udiag._last);
#endif
            *Dj = TMV_ABS(newcDj);
            *Uj = TMV_SIGN(newcDj,*Dj);
            std::complex<T> newcEj = TMV_CONJ(*Uj) * (*cEj);
            *Ej = TMV_ABS(newcEj);
            ++Vtj; // Now Vdiag(j+1)
#ifdef TMVFLDEBUG
            TMVAssert(Vtj >= Vtdiag._first);
            TMVAssert(Vtj < Vtdiag._last);
#endif
            *Vtj = TMV_SIGN(newcEj,*Ej);
            ++cDj; // Now cd(j+1)
            newcDj = TMV_CONJ(*Vtj) * (*cDj);
        }
#ifdef TMVFLDEBUG
        TMVAssert(Dj >= D._first);
        TMVAssert(Dj < D._last);
        TMVAssert(Uj >= Udiag._first);
        TMVAssert(Uj < Udiag._last);
#endif
        *Dj = TMV_ABS(newcDj);
        *Uj = TMV_SIGN(newcDj,*Dj);
        std::complex<T> su, sv;
        DiagMatrixViewOf(Udiag).logDet(&su);
        DiagMatrixViewOf(Vtdiag).logDet(&sv);
        signdet *= su*sv;
    }

    template <class T> 
    static void NonLapBidiagonalize(
        const GenBandMatrix<T>& A,
        MVP<T> U, const VectorView<RT>& D,
        const VectorView<RT>& E, MVP<T> Vt, RT& logdet, T& signdet)
    {
        // Decompose A into U B Vt
        // The Bidiagonal Matrix B is stored as two vectors: D, E
        // D is the diagonal, E is the super-diagonal
        // We use Householder reflections to reduce A to the bidiagonal form:

        TMVAssert(A.rowsize() <= A.colsize());
        TMVAssert(A.rowsize() > 0);
        if (U) {
            TMVAssert(U->colsize() == A.colsize());
            TMVAssert(U->rowsize() == A.rowsize());
        } 
        if (Vt) {
            TMVAssert(Vt->colsize() == A.rowsize());
            TMVAssert(Vt->rowsize() == A.rowsize());
        }
        TMVAssert(D.size() == A.rowsize());
        TMVAssert(D.size() == E.size()+1);

        const int M = A.colsize();
        const int N = A.rowsize();

        const int nlo = A.nlo();
        const int nhi = A.nhi();

        if (nlo == 0 && nhi == 1) {
            Vector<T> Ud(N);
            Vector<T> Vtd(N);
            MakeBidiagReal(Ud,Vtd,A.diag(),A.diag(1),D,E,signdet);
            if (U) {
                U->setZero();
                U->diag() = Ud;
            }
            if (Vt) {
                Vt->setZero();
                Vt->diag() = Vtd;
            }
        } else if (A.isSquare() && nlo == 1 && nhi == 0) {
            Vector<T> Ud(N);
            Vector<T> Vtd(N);
            MakeBidiagReal(Ud,Vtd,A.diag().reverse(),A.diag(-1).reverse(),
                           D,E,signdet);
            if (U) {
                U->setZero();
                U->subVector(N-1,0,-1,1,N) = Ud;
            }
            if (Vt) {
                Vt->setZero();
                Vt->subVector(0,N-1,1,-1,N) = Vtd;
            }
        } else {
            std::auto_ptr<Matrix<T,ColMajor> > UU(0);
            std::auto_ptr<MatrixView<T> > U1(0);
            if (U) {
                *U = A;
                U1.reset(new MatrixView<T>(U->view()));
            } else {
                UU.reset(new Matrix<T,ColMajor>(A));
                U1.reset(new MatrixView<T>(UU->view()));
            }

            std::vector<int> vec(N), ver(N-1);
            int endcol = nlo+1;
            Vector<T> Ubeta(N);
            Vector<T> Vtbeta(N-1);

            T* Ubj = Ubeta.ptr();
            for(int j=0;j<N-1;++j,++Ubj) {
                vec[j] = endcol;
                int endrow = TMV_MIN(endcol+nhi,N);
                ver[j] = endrow;
#ifdef TMVFLDEBUG
                TMVAssert(Ubj >= Ubeta._first);
                TMVAssert(Ubj < Ubeta._last);
#endif
                *Ubj = HouseholderReflect(
                    U1->subMatrix(j,endcol,j,endrow),signdet);
                if (endcol < M) endcol = TMV_MIN(endrow+nlo,M);
                Vtbeta(j) = HouseholderReflect(
                    U1->transpose().subMatrix(j+1,endrow,j,endcol),signdet);
            }
            vec[N-1] = endcol;
#ifdef TMVFLDEBUG
            TMVAssert(Ubj >= Ubeta._first);
            TMVAssert(Ubj < Ubeta._last);
#endif
            *Ubj = HouseholderReflect(U1->subMatrix(N-1,endcol,N-1,N),signdet);

            // Now U stores Householder vectors for U in lower diagonal columns (HLi)
            // and Householder vectors for Vt in upper diagonal rows (HRi)
            // except for the bidiagonal which is the bidiagonal we want:
            D = U1->diag().realPart();
            E = U1->diag(1).realPart();
            if (isComplex(T())) {
                TMVAssert(NormInf(U1->diag().imagPart()) == RT(0));
                TMVAssert(NormInf(U1->diag(1).imagPart()) == RT(0));
            }

            if (Vt) {
                Vt->setToIdentity();
                for (int j=N-2;j>=0;--j) {
                    Vt->row(j+1,j+2,ver[j]) = U1->row(j,j+2,ver[j]);
                    HouseholderUnpack(
                        Vt->transpose().subMatrix(j+1,ver[j],j+1,N),Vtbeta(j));
                }
            }

            if (U) {
                U->diag().setZero();
                U->diag(1).setZero();
                // Ubj is currently &U(N-1)
                HouseholderUnpack(U->subMatrix(N-1,vec[N-1],N-1,N),*Ubj);
                for (int j=N-2;j>=0;--j) {
                    U->row(j,j,ver[j]).setZero();
                    HouseholderUnpack(U->subMatrix(j,vec[j],j,N),*(--Ubj));
                }
            }
        }
        if (signdet != T(0)) {
            RT s;
            logdet += DiagMatrixViewOf(D).logDet(&s);
            signdet *= s;
        }
    }

#ifdef LAP
    template <class T> 
    static inline void LapBidiagonalize(
        const GenBandMatrix<T>& A,
        MVP<T> U, const VectorView<RT>& D,
        const VectorView<RT>& E, MVP<T> Vt, RT& logdet, T& signdet)
    { NonLapBidiagonalize(A,U,D,E,Vt,logdet,signdet); }
#ifdef INST_DOUBLE
    template <> 
    void LapBidiagonalize(const GenBandMatrix<double>& A,
              MVP<double> U, const VectorView<double>& D,
              const VectorView<double>& E, MVP<double> Vt, 
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
        if (Vt) {
            TMVAssert(Vt->colsize() == A.rowsize());
            TMVAssert(Vt->rowsize() == A.rowsize());
            TMVAssert(Vt->iscm());
            TMVAssert(Vt->ct() == NonConj);
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
        int lwork = 2*TMV_MAX(m,n);
        AlignedArray<double> work(lwork);
        VectorViewOf(work.get(),lwork).setZero();
#endif
        // LAP version overwrites original BandMatrix with crap.
        // Hence, copy BandMatrix before running.
        BandMatrix<double,ColMajor> A2 = A;
        if (U) U->setZero();
        if (Vt) Vt->setZero();
        D.setZero();
        E.setZero();
        int lda = A2.diagstep();
        int ldu = U ? U->stepj() : 1;
        int ldv = Vt ? Vt->stepj() : 1;
        char vect = U ? Vt ? 'B' : 'Q' : Vt ? 'P' : 'N';
        double* VV = Vt ? Vt->ptr() : 0;
        double* UU = U ? U->ptr() : 0;

        LAPNAME(dgbbrd) (
            LAPCM LAPV(vect),LAPV(m),LAPV(n),LAPV(ncc),
            LAPV(kl),LAPV(ku),
            LAPP(A2.cptr()-A.nhi()),LAPV(lda),LAPP(D.ptr()),LAPP(E.ptr()),
            LAPP(UU),LAPV(ldu),LAPP(VV),LAPV(ldv),
            0,LAPV(n) LAPWK(work.get()) LAPINFO LAP1 );

        if (signdet != 0.) {
            double s;
            logdet += DiagMatrixViewOf(D).logDet(&s);
            signdet *= s;
        }
        LAP_Results("dgbbrd");
    }
    template <> 
    void LapBidiagonalize(
        const GenBandMatrix<std::complex<double> >& A,
        MVP<std::complex<double> > U, const VectorView<double>& D,
        const VectorView<double>& E, MVP<std::complex<double> > Vt, 
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
        if (Vt) {
            TMVAssert(Vt->colsize() == A.rowsize());
            TMVAssert(Vt->rowsize() == A.rowsize());
            TMVAssert(Vt->iscm());
            TMVAssert(Vt->ct() == NonConj);
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
        int lwork = TMV_MAX(m,n);
        AlignedArray<std::complex<double> > work(lwork);
        AlignedArray<double> rwork(lwork);
        VectorViewOf(work.get(),lwork).setZero();
        VectorViewOf(rwork.get(),lwork).setZero();
#endif
        BandMatrix<std::complex<double>,ColMajor> A2 = A;
        if (U) U->setZero();
        if (Vt) Vt->setZero();
        D.setZero();
        E.setZero();
        int lda = A2.diagstep();
        int ldu = U ? U->stepj() : 1;
        int ldv = Vt ? Vt->stepj() : 1;
        char vect = U ? Vt ? 'B' : 'Q' : Vt ? 'P' : 'N';
        std::complex<double>* VV = Vt ? Vt->ptr() : 0;
        std::complex<double>* UU = U ? U->ptr() : 0;

        LAPNAME(zgbbrd) (
            LAPCM LAPV(vect),LAPV(m),LAPV(n),LAPV(o),
            LAPV(kl),LAPV(ku),LAPP(A2.cptr()-A.nhi()),LAPV(lda),
            LAPP(D.ptr()),LAPP(E.ptr()),LAPP(UU),LAPV(ldu),
            LAPP(VV),LAPV(ldv),0,LAPV(n)
            LAPWK(work.get()) LAPWK(rwork.get()) LAPINFO LAP1);

        // OK, I lied, the output A2 isn't complete crap.  Its diagonal
        // holds a version of D which includes all the complex arguments
        // so it can be used to find the determinant.
        if (signdet != 0.) {
            std::complex<double> s;
            logdet += DiagMatrixViewOf(A2.diag()).logDet(&s);
            signdet *= s;
        }
        LAP_Results("zgbbrd");
    }
#endif
#ifdef INST_FLOAT
    template <> 
    void LapBidiagonalize(const GenBandMatrix<float>& A,
              MVP<float> U, const VectorView<float>& D,
              const VectorView<float>& E, MVP<float> Vt, 
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
        if (Vt) {
            TMVAssert(Vt->colsize() == A.rowsize());
            TMVAssert(Vt->rowsize() == A.rowsize());
            TMVAssert(Vt->iscm());
            TMVAssert(Vt->ct() == NonConj);
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
        int lwork = 2*TMV_MAX(m,n);
        AlignedArray<float> work(lwork);
        VectorViewOf(work.get(),lwork).setZero();
#endif
        BandMatrix<float,ColMajor> A2 = A;
        if (U) U->setZero();
        if (Vt) Vt->setZero();
        D.setZero();
        E.setZero();
        int lda = A2.diagstep();
        int ldu = U ? U->stepj() : 1;
        int ldv = Vt ? Vt->stepj() : 1;
        char vect = U ? Vt ? 'B' : 'Q' : Vt ? 'P' : 'N';
        float* VV = Vt ? Vt->ptr() : 0;
        float* UU = U ? U->ptr() : 0;

        LAPNAME(sgbbrd) (
            LAPCM LAPV(vect),LAPV(m),LAPV(n),LAPV(ncc),
            LAPV(kl),LAPV(ku),
            LAPP(A2.cptr()-A.nhi()),LAPV(lda),LAPP(D.ptr()),LAPP(E.ptr()),
            LAPP(UU),LAPV(ldu),LAPP(VV),LAPV(ldv),
            0,LAPV(n) LAPWK(work.get()) LAPINFO LAP1 );

        if (signdet != 0.F) {
            float s;
            logdet += DiagMatrixViewOf(D).logDet(&s);
            signdet *= s;
        }
        LAP_Results("sgbbrd");
    }
    template <> 
    void LapBidiagonalize(
        const GenBandMatrix<std::complex<float> >& A,
        MVP<std::complex<float> > U, const VectorView<float>& D,
        const VectorView<float>& E, MVP<std::complex<float> > Vt, 
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
        if (Vt) {
            TMVAssert(Vt->colsize() == A.rowsize());
            TMVAssert(Vt->rowsize() == A.rowsize());
            TMVAssert(Vt->iscm());
            TMVAssert(Vt->ct() == NonConj);
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
        int lwork = TMV_MAX(m,n);
        AlignedArray<std::complex<float> > work(lwork);
        AlignedArray<float> rwork(lwork);
        VectorViewOf(work.get(),lwork).setZero();
        VectorViewOf(rwork.get(),lwork).setZero();
#endif
        BandMatrix<std::complex<float>,ColMajor> A2 = A;
        if (U) U->setZero();
        if (Vt) Vt->setZero();
        D.setZero();
        E.setZero();
        int lda = A2.diagstep();
        int ldu = U ? U->stepj() : 1;
        int ldv = Vt ? Vt->stepj() : 1;
        char vect = U ? Vt ? 'B' : 'Q' : Vt ? 'P' : 'N';
        std::complex<float>* VV = Vt ? Vt->ptr() : 0;
        std::complex<float>* UU = U ? U->ptr() : 0;

        LAPNAME(cgbbrd) (
            LAPCM LAPV(vect),LAPV(m),LAPV(n),LAPV(o),
            LAPV(kl),LAPV(ku),LAPP(A2.cptr()-A.nhi()),LAPV(lda),
            LAPP(D.ptr()),LAPP(E.ptr()),LAPP(UU),LAPV(ldu),
            LAPP(VV),LAPV(ldv),0,LAPV(n)
            LAPWK(work.get()) LAPWK(rwork.get()) LAPINFO LAP1);

        if (signdet != 0.F) {
            std::complex<float> s;
            logdet += DiagMatrixViewOf(A2.diag()).logDet(&s);
            signdet *= s;
        }
        LAP_Results("cgbbrd");
    }
#endif
#endif
    template <class T> 
    static void Bidiagonalize(
        const GenBandMatrix<T>& A,
        MVP<T> U, const VectorView<RT>& D,
        const VectorView<RT>& E, MVP<T> Vt, RT& logdet, T& signdet)
    {
        TMVAssert(A.rowsize() <= A.colsize());
        TMVAssert(A.rowsize() > 0);
        if (U) {
            TMVAssert(U->colsize() == A.colsize());
            TMVAssert(U->rowsize() == A.rowsize());
            TMVAssert(U->ct() == NonConj);
        }
        if (Vt) {
            TMVAssert(Vt->colsize() == A.rowsize());
            TMVAssert(Vt->rowsize() == A.rowsize());
            TMVAssert(Vt->ct() == NonConj);
        }
        TMVAssert(D.size() == A.rowsize());
        TMVAssert(E.size() == D.size()-1);
        TMVAssert(D.step()==1);
        TMVAssert(E.step()==1);
        TMVAssert(D.ct() == NonConj);
        TMVAssert(E.ct() == NonConj);

#ifdef XDEBUG
        Matrix<T> A0(A);
#ifdef LAP
        BandMatrix<T> A2(A);
        Vector<RT> D2(D);
        Vector<RT> E2(E);
        Matrix<T> U2(A.colsize(),A.rowsize());
        Matrix<T> Vt2(D.size(),D.size());
        RT logdet2(0);
        T signdet2(1);
        NonLapBidiagonalize<T>(
            A2,U2.view(),D2.view(),E2.view(),Vt2.view(),logdet2,signdet2);
#endif
#endif

        if (A.rowsize() > 0) {
            TMVAssert(E.size() == D.size()-1);
#ifdef LAP
            if (A.isSquare() && (!U || U->iscm()) && (!Vt || Vt->iscm())) 
                LapBidiagonalize(A,U,D,E,Vt,logdet,signdet);
            else 
#endif
                NonLapBidiagonalize(A,U,D,E,Vt,logdet,signdet);
        }
#ifdef XDEBUG
        if (U && Vt) {
            std::cout<<"Done Band Bidiagonalize:\n";
            std::cout<<"U = "<<*U<<std::endl;
            std::cout<<"D = "<<D<<std::endl;
            std::cout<<"E = "<<E<<std::endl;
            std::cout<<"Vt = "<<*Vt<<std::endl;
            Matrix<T> UBVt = *U*UpperBiDiagMatrix(D,E)*(*Vt);
            std::cout<<"UBVt = "<<UBVt<<std::endl;
            std::cout<<"Norm(UBVt-A0) = "<<Norm(UBVt-A0)<<std::endl;
            if (!(Norm(UBVt-A0) < 0.001*Norm(A0))) {
                cerr<<"Bidiagonalize:\n";
                cerr<<"A = "<<TMV_Text(A)<<"  "<<A0<<endl;
                cerr<<"-> D = "<<D<<endl;
                cerr<<"E = "<<E<<endl;
                cerr<<"U = "<<*U<<endl;
                cerr<<"Vt = "<<*Vt<<endl;
#ifdef LAP
                cerr<<"Nonlap D = "<<D2<<endl;
                cerr<<"Norm(diff) = "<<Norm(D-D2)<<endl;
                cerr<<"Nonlap E = "<<E2<<endl;
                cerr<<"Norm(diff) = "<<Norm(E-E2)<<endl;
                cerr<<"U2 = "<<U2<<endl;
                cerr<<"Vt2 = "<<Vt2<<endl;
#endif
                cerr<<"UBVt = "<<UBVt<<endl;
                cerr<<"Norm(UBVt-A0) = "<<Norm(UBVt-A0)<<endl;
                abort();
            }
        }
#endif
    }

    template <class T> 
    void SV_Decompose(
        const GenBandMatrix<T>& A,
        MVP<T> U, const DiagMatrixView<RT>& S, MVP<T> Vt, RT& logdet, T& signdet)
    {
        // Decompose A into U S Vt
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
        if (Vt) {
            TMVAssert(Vt->rowsize() == A.rowsize());
            TMVAssert(Vt->colsize() == A.rowsize());
            TMVAssert(Vt->ct() == NonConj);
        }
        TMVAssert(S.size() == A.rowsize());
        TMVAssert(S.diag().ct() == NonConj);

        if (A.nlo() == 0 && A.nhi() == 0) {
            if (U) U->setToIdentity();
            if (Vt) Vt->setToIdentity();

            if (signdet != T(0)) {
                T s;
                logdet += DiagMatrixViewOf(A.diag()).logDet(&s);
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
                    TMVAssert(Sj >= S.diag()._first);
                    TMVAssert(Sj < S.diag()._last);
#endif
                    *Sj = TMV_ABS(*Ajj);
                    if(U) {
#ifdef TMVFLDEBUG
                        TMVAssert(Ujj >= U->_first);
                        TMVAssert(Ujj < U->_last);
#endif
                        *Ujj = TMV_SIGN(TMV_CONJ(*Ajj),*Sj); Ujj += Uds; 
                    }
                }
            } else {
                for(int j=0;j<N;++j,Ajj+=Ads,Sj+=Ss) {
#ifdef TMVFLDEBUG
                    TMVAssert(Sj >= S.diag()._first);
                    TMVAssert(Sj < S.diag()._last);
#endif
                    *Sj = TMV_ABS(*Ajj);
                    if(U) { 
#ifdef TMVFLDEBUG
                        TMVAssert(Ujj >= U->_first);
                        TMVAssert(Ujj < U->_last);
#endif
                        *Ujj = TMV_SIGN(*Ajj,*Sj); Ujj += Uds; 
                    }
                }
            }
            AlignedArray<int> sortp(N);
            S.diag().sort(sortp.get(),Descend);
            if (U) U->permuteCols(sortp.get());
            if (Vt) Vt->permuteRows(sortp.get());
        } else {
            Vector<RT> E(S.size()-1);
            Bidiagonalize(A,U,S.diag(),E.view(),Vt,logdet,signdet);

            SV_DecomposeFromBidiagonal(U,S.diag(),E.view(),Vt);
        }
    }

    template <class T> 
    void SV_Decompose(
        const GenBandMatrix<T>& A,
        const MatrixView<T>& U, const DiagMatrixView<RT>& S, 
        const MatrixView<T>& Vt)
    { 
        if (U.isconj()) {
            if (Vt.isconj()) {
                SV_Decompose(A.conjugate(),U.conjugate(),S,Vt.conjugate());
            } else {
                SV_Decompose(A.conjugate(),U.conjugate(),S,Vt);
                Vt.conjugateSelf();
            }
        } else {
            if (Vt.isconj()) {
                SV_Decompose(A,U,S,Vt.conjugate());
                Vt.conjugateSelf();
            } else {
                RT ld=0; T d=0; SV_Decompose<T>(A,U,S,Vt,ld,d); 
            }
        }
    }

    template <class T> 
    void SV_Decompose(
        const GenBandMatrix<T>& A,
        const MatrixView<T>& U, const DiagMatrixView<RT>& S)
    {
        if (U.isconj()) {
            SV_Decompose(A.conjugate(),U.conjugate(),S);
        } else {
            RT ld=0; T d=0; SV_Decompose<T>(A,U,S,0,ld,d); 
        }
    }

    template <class T> 
    void SV_Decompose(
        const GenBandMatrix<T>& A,
        const DiagMatrixView<RT>& S, const MatrixView<T>& Vt)
    {
        if (Vt.isconj()) {
            SV_Decompose(A.conjugate(),S,Vt.conjugate());
        } else {
            RT ld=0; T d=0; SV_Decompose<T>(A,0,S,Vt,ld,d); 
        }
    }

    template <class T> 
    void SV_Decompose(
        const GenBandMatrix<T>& A, const DiagMatrixView<RT>& S)
    { RT ld=0; T d=0; SV_Decompose<T>(A,0,S,0,ld,d); }

#undef RT

#ifdef INST_INT
#undef INST_INT
#endif

#define InstFile "TMV_BandSVDecompose.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv

