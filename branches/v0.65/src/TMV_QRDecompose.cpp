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
#include "TMV_QRDiv.h"
#include "tmv/TMV_QRD.h"
#include "tmv/TMV_Matrix.h"
#include "tmv/TMV_TriMatrix.h"
#include "TMV_Householder.h"
#include "tmv/TMV_TriMatrixArith.h"
#include "tmv/TMV_VectorArith.h"
#include "tmv/TMV_MatrixArith.h"

#ifdef XDEBUG
#include "tmv/TMV_MatrixArith.h"
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#endif

namespace tmv {

#ifdef TMV_BLOCKSIZE
#define QR_BLOCKSIZE TMV_BLOCKSIZE
#else
#define QR_BLOCKSIZE 64
#endif

    //
    // QR Decompose
    //

    template <class T> 
    static void NonBlockQRDecompose(
        const MatrixView<T>& A, const VectorView<T>& beta, T& det)
    {
#ifdef XDEBUG
        cout<<"Start NonBlockQRDecompose\n";
        cout<<"Norm(A) = "<<Norm(A)<<endl;
        cout<<"Norm(beta) = "<<Norm(A)<<endl;
        Matrix<T> A0(A);
#endif

        TMVAssert(A.colsize() >= A.rowsize());
        TMVAssert(A.rowsize() == beta.size());
        TMVAssert(A.rowsize() > 0);
        TMVAssert(A.ct() == NonConj);
        TMVAssert(A.isrm() || A.iscm());
        TMVAssert(!beta.isconj());
        TMVAssert(beta.step()==1);
        // Decompose A into A = Q R 
        // where Q is unitary, and R is upper triangular
        // Q and R are stored in the same matrix (output of A), 
        // with the beta's for the Householder matrices returned in beta.
        const int M = A.colsize();
        const int N = A.rowsize();

        T* bj = beta.ptr();
        for(int j=0;j<N;++j,++bj) {
            // Apply the Householder Reflection for this column
#ifdef TMVFLDEBUG
            TMVAssert(bj >= beta.first);
            TMVAssert(bj < beta.last);
#endif
            *bj = HouseholderReflect(A.subMatrix(j,M,j,N),det);
        }
#ifdef XDEBUG
        Matrix<T> R = A.upperTri();
        Matrix<T> Q(A);
        GetQFromQR(Q.view(),beta);
        Matrix<T> AA = Q*R;
        cout<<"Done NonBlockQRDecompose\n";
        cout<<"Norm(A) = "<<Norm(A)<<endl;
        cout<<"Norm(beta) = "<<Norm(A)<<endl;
        cout<<"Norm(AA-A0) = "<<Norm(AA-A0)<<endl;
        if (!(Norm(AA-A0) < 0.0001*Norm(Q)*Norm(R))) {
            cerr<<"NonBlockQRDecompose: A = "<<TMV_Text(A)<<"  "<<A0<<endl;
            cerr<<"-> "<<A<<endl;
            cerr<<"beta = "<<beta<<endl;
            cerr<<"Q = "<<Q<<endl;
            cerr<<"R = "<<R<<endl;
            cerr<<"Q*R = "<<Q*R<<endl;
            cerr<<"Norm(diff) = "<<Norm(AA-A0)<<endl;
            abort(); 
        }
#endif
    }

    template <class T> 
    static void RecursiveQRDecompose(
        const MatrixView<T>& A, const UpperTriMatrixView<T>& Z, T& det,
        bool makeZ)
    {
#ifdef XDEBUG
        cout<<"Start RecursiveQRDecompose\n";
        cout<<"Norm(A) = "<<Norm(A)<<endl;
        cout<<"Norm(beta) = "<<Norm(A)<<endl;
        Matrix<T> A0(A);
#endif
        TMVAssert(A.colsize() >= A.rowsize());
        TMVAssert(A.rowsize() == Z.size());
        TMVAssert(A.rowsize() > 0);
        TMVAssert(A.isrm() || A.iscm());
        TMVAssert(Z.iscm());
        TMVAssert(!A.isconj());
        TMVAssert(!Z.isconj());
        // This is very similar to the BlockHouseholder_MakeZ function
        // in Householder.cpp.  The difference is the addition of the 
        // Householder_Reflects.
        // The makeZ parameter should be set to true if you want the Z
        // matrix to be correct on output.  If you don't need the Z matrix
        // after this call, setting makeZ to false will speed it up slightly.
        // (In either case, the diagonal of Z is correctly set to be 
        // beta.conjugate().)
        const int M = A.colsize();
        const int N = A.rowsize();

        if (N==1) {
            T b = HouseholderReflect(A.col(0),det);
            *Z.ptr() = TMV_CONJ(b);
        } else if (N==2) {
            T* Z00 = Z.ptr();
            T* Z01 = Z00 + Z.stepj();
            T* Z11 = Z01 + 1;
            //cout<<"Z = "<<*Z00<<"  "<<*Z01<<"  "<<*Z11<<endl;

            T b0 = HouseholderReflect(A,det);
            //cout<<"b0 = "<<b0<<endl;
            //cout<<"A -> "<<A<<endl;
            *Z00 = TMV_CONJ(b0);
            //cout<<"A.col(1,1,M) = "<<A.col(1,1,M)<<endl;
            T b1 = HouseholderReflect(A.col(1,1,M),det);
            //cout<<"b1 = "<<b1<<endl;
            *Z11 = TMV_CONJ(b1);
            //cout<<"Z => "<<*Z00<<"  "<<*Z01<<"  "<<*Z11<<endl;

            if (makeZ) {
                const T* A10 = A.cptr()+A.stepi();
                T temp = A.col(0,2,M).conjugate()*A.col(1,2,M);
                temp += TMV_CONJ(*A10);
                *Z01 = -TMV_CONJ(b0*b1)*temp;
                //cout<<"temp = "<<temp<<endl;
                //cout<<"Z => "<<*Z00<<"  "<<*Z01<<"  "<<*Z11<<endl;
            }
#ifdef TMV_INITIALIZE_NAN
            else {
                *Z01 = T(0);
                //cout<<"Z => "<<*Z00<<"  "<<*Z01<<"  "<<*Z11<<endl;
            }
#endif
        } else {
            //cout<<"N = "<<N<<endl;
            int j1 = (N+1)/2;
            //cout<<"j1 = "<<j1<<endl;
            MatrixView<T> A1 = A.colRange(0,j1);
            UpperTriMatrixView<T> Z1 = Z.subTriMatrix(0,j1);
            //cout<<"Z1 = "<<Z1<<endl;
            RecursiveQRDecompose(A1,Z1,det,true);
            //cout<<"Z1 => "<<Z1<<endl;
            //cout<<"A1 => "<<A1<<endl;

            BlockHouseholderLDiv(A1,Z1,A.colRange(j1,N));
            //cout<<"Z1 => "<<Z1<<endl;
            //cout<<"A1 => "<<A1<<endl;
            //cout<<"A(j1:N) = "<<A.colRange(j1,N)<<endl;

            MatrixView<T> A2 = A.subMatrix(j1,M,j1,N);
            UpperTriMatrixView<T> Z2 = Z.subTriMatrix(j1,N);
            //cout<<"Z2 = "<<Z2<<endl;
            RecursiveQRDecompose(A2,Z2,det,makeZ);
            //cout<<"Z2 => "<<Z2<<endl;
            //cout<<"A2 => "<<A2<<endl;

            if (makeZ) {
                MatrixView<T> Z3 = Z.subMatrix(0,j1,j1,N);
                Z3 = A1.rowRange(j1,N).adjoint() *
                    A.subMatrix(j1,N,j1,N).lowerTri(UnitDiag);
                Z3 += A1.rowRange(N,M).adjoint() * A.subMatrix(N,M,j1,N);
                Z3 = -Z1*Z3;
                Z3 *= Z2;
                //cout<<"Z => "<<Z<<endl;
            }
#ifdef TMV_INITIALIZE_NAN
            else {
                MatrixView<T> Z3 = Z.subMatrix(0,j1,j1,N);
                Z3.setZero();
                //cout<<"Z => "<<Z<<endl;
            }
#endif
        }
#ifdef XDEBUG
        Matrix<T> R = A.upperTri();
        Matrix<T> Q(A);
        GetQFromQR(Q.view(),Z.diag().conjugate());
        Matrix<T> AA = Q*R;
        cout<<"Done RecursiveQRDecompose\n";
        cout<<"Norm(A) = "<<Norm(A)<<endl;
        cout<<"Norm(beta) = "<<Norm(A)<<endl;
        cout<<"Norm(AA-A0) = "<<Norm(AA-A0)<<endl;
        cout<<"Norm(Z) = "<<Norm(Z)<<endl;
        if (!(Norm(AA-A0) < 0.0001*Norm(Q)*Norm(R))) {
            cerr<<"RecursiveQRDecompose: A = "<<TMV_Text(A)<<"  "<<A0<<endl;
            cerr<<"-> "<<A<<endl;
            cerr<<"Z = "<<Z<<endl;
            cerr<<"beta = "<<Z.diag().conjugate()<<endl;
            cerr<<"Q = "<<Q<<endl;
            cerr<<"R = "<<R<<endl;
            cerr<<"Q*R = "<<Q*R<<endl;
            cerr<<"Norm(diff) = "<<Norm(AA-A0)<<endl;
            Matrix<T> A2 = A0;
            Vector<T> beta2(Z.size());
            T det2(0);
            NonBlockQRDecompose(A2.view(),beta2.view(),det2);
            cerr<<"NonBlock "<<A2<<endl;
            cerr<<"beta = "<<beta2<<endl;
            abort(); 
        }
#endif
    }

    template <class T> 
    static void BlockQRDecompose(
        const MatrixView<T>& A, const VectorView<T>& beta, T& det)
    {
        TMVAssert(A.colsize() >= A.rowsize());
        TMVAssert(A.rowsize() == beta.size());
        TMVAssert(A.rowsize() > 0);
        TMVAssert(A.ct() == NonConj);
        TMVAssert(A.isrm() || A.iscm());
        TMVAssert(beta.ct() == NonConj);
        TMVAssert(beta.step()==1);

#ifdef XDEBUG
        cout<<"Start BlockQRDecompose\n";
        cout<<"Norm(A) = "<<Norm(A)<<endl;
        cout<<"Norm(beta) = "<<Norm(A)<<endl;
        Matrix<T> A0(A);
#endif
        const int M = A.colsize();
        const int N = A.rowsize();

        UpperTriMatrix<T,NonUnitDiag,ColMajor> BaseZ(
            TMV_MIN(QR_BLOCKSIZE,N));
        for(int j1=0;j1<N;) {
            int j2 = TMV_MIN(N,j1+QR_BLOCKSIZE);
            MatrixView<T> A1 = A.subMatrix(j1,M,j1,j2);
            UpperTriMatrixView<T> Z = BaseZ.subTriMatrix(0,j2-j1);

            RecursiveQRDecompose(A1,Z,det,j2<N);
            beta.subVector(j1,j2) = Z.diag().conjugate();

            if (j2 < N) 
                BlockHouseholderLDiv(A1,Z,A.subMatrix(j1,M,j2,N));
            j1 = j2;
        }
#ifdef XDEBUG
        Matrix<T> R = A.upperTri();
        Matrix<T> Q(A);
        GetQFromQR(Q.view(),beta);
        Matrix<T> AA = Q*R;
        cout<<"Done BlockQRDecompose\n";
        cout<<"Norm(A) = "<<Norm(A)<<endl;
        cout<<"Norm(beta) = "<<Norm(A)<<endl;
        cout<<"Norm(AA-A0) = "<<Norm(AA-A0)<<endl;
        cout<<"Norm(BaseZ) = "<<Norm(BaseZ)<<endl;
        if (!(Norm(AA-A0) < 0.0001*Norm(Q)*Norm(R))) {
            cerr<<"BlockQRDecompose: A = "<<TMV_Text(A)<<"  "<<A0<<endl;
            cerr<<"-> "<<A<<endl;
            cerr<<"beta = "<<beta<<endl;
            cerr<<"Q*R = "<<Q*R<<endl;
            cerr<<"Norm(diff) = "<<Norm(AA-A0)<<endl;
            Matrix<T> A2 = A0;
            Vector<T> beta2(beta.size());
            T det2(0);
            NonBlockQRDecompose(A2.view(),beta2.view(),det2);
            cerr<<"NonBlock "<<A2<<endl;
            cerr<<"beta = "<<beta2<<endl;
            abort(); 
        }
#endif
    }

    template <class T> 
    static void NonLapQRDecompose(
        const MatrixView<T>& A, const VectorView<T>& beta, T& det)
    {
        TMVAssert(A.colsize() >= A.rowsize());
        TMVAssert(A.rowsize() == beta.size());
        TMVAssert(A.rowsize() > 0);
        TMVAssert(A.ct() == NonConj);
        TMVAssert(A.isrm() || A.iscm());
        TMVAssert(beta.ct() == NonConj);
        TMVAssert(beta.step()==1);

        if (A.rowsize() > QR_BLOCKSIZE)
            BlockQRDecompose(A,beta,det);
        else {
            UpperTriMatrix<T,NonUnitDiag,ColMajor> Z(A.rowsize());
            RecursiveQRDecompose(A,Z.view(),det,false);
            beta = Z.diag().conjugate();
        }
    }

#ifdef LAP
    template <class T> 
    static inline void LapQRDecompose(
        const MatrixView<T>& A, const VectorView<T>& beta, T& det)
    { NonLapQRDecompose(A,beta,det); }
#ifdef INST_DOUBLE
    template <> 
    void LapQRDecompose(
        const MatrixView<double>& A,
        const VectorView<double>& beta, double& det)
    {
        TMVAssert(A.colsize() >= A.rowsize());
        TMVAssert(A.rowsize() == beta.size());
        TMVAssert(A.rowsize() > 0);
        TMVAssert(A.ct() == NonConj);
        TMVAssert(A.isrm() || A.iscm());
        TMVAssert(beta.ct() == NonConj);
        TMVAssert(beta.step()==1);
        int m = A.colsize();
        int n = A.rowsize();
        beta.setZero();
        if (A.isrm()) {
            int lda = A.stepi();
#ifndef LAPNOWORK
#ifdef NOWORKQUERY
            int lwork = 2*n*LAP_BLOCKSIZE;
            AlignedArray<double> work(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#else
            int lwork = -1;
            AlignedArray<double> work(1);
            work.get()[0] = 0.;
            LAPNAME(dgelqf) (
                LAPCM LAPV(n),LAPV(m),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
            lwork = int(work[0]);
            work.resize(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#endif
#endif
            LAPNAME(dgelqf) (
                LAPCM LAPV(n),LAPV(m),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
#ifdef LAPNOWORK
            LAP_Results("dgelqf");
#else
            LAP_Results(int(work[0]),m,n,lwork,"dgelqf");
#endif
        } else {
            int lda = A.stepj();
#ifndef LAPNOWORK
#ifdef NOWORKQUERY
            int lwork = 2*n*LAP_BLOCKSIZE;
            AlignedArray<double> work(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#else
            int lwork = -1;
            AlignedArray<double> work(1);
            work.get()[0] = 0.;
            LAPNAME(dgeqrf) (
                LAPCM LAPV(m),LAPV(n),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
            lwork = int(work[0]);
            work.resize(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#endif
#endif
            LAPNAME(dgeqrf) (
                LAPCM LAPV(m),LAPV(n),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
#ifdef LAPNOWORK
            LAP_Results("dgeqrf");
#else
            LAP_Results(int(work[0]),m,n,lwork,"dgeqrf");
#endif
        }
        double* bi = beta.ptr();
        for(int i=0;i<n;++i,++bi)  {
            if (TMV_ABS(*bi-1.) > 1.01) *bi = 0.;
            if (det && *bi != 0.) det = -det;
        }
    }
    template <> 
    void LapQRDecompose(
        const MatrixView<std::complex<double> >& A,
        const VectorView<std::complex<double> >& beta,
        std::complex<double>& det)
    {
        TMVAssert(A.colsize() >= A.rowsize());
        TMVAssert(A.rowsize() == beta.size());
        TMVAssert(A.rowsize() > 0);
        TMVAssert(A.ct() == NonConj);
        TMVAssert(A.isrm() || A.iscm());
        TMVAssert(beta.ct() == NonConj);
        TMVAssert(beta.step()==1);
        int m = A.colsize();
        int n = A.rowsize();
        beta.setZero();
        if (A.isrm()) {
            int lda = A.stepi();
#ifndef LAPNOWORK
#ifdef NOWORKQUERY
            int lwork = 2*n*LAP_BLOCKSIZE;
            AlignedArray<std::complex<double> > work(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#else
            int lwork = -1;
            AlignedArray<std::complex<double> > work(1);
            work.get()[0] = 0.;
            LAPNAME(zgelqf) (
                LAPCM LAPV(n),LAPV(m),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
            lwork = int(TMV_REAL(work[0]));
            work.resize(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#endif
#endif
            LAPNAME(zgelqf) (
                LAPCM LAPV(n),LAPV(m),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
#ifdef LAPNOWORK
            LAP_Results("zgelqf");
#else
            LAP_Results(int(TMV_REAL(work[0])),m,n,lwork,"zgelqf");
#endif
        } else {
            int lda = A.stepj();
#ifndef LAPNOWORK
#ifdef NOWORKQUERY
            int lwork = 2*n*LAP_BLOCKSIZE;
            AlignedArray<std::complex<double> > work(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#else
            int lwork = -1;
            AlignedArray<std::complex<double> > work(1);
            work.get()[0] = 0.;
            LAPNAME(zgeqrf) (
                LAPCM LAPV(m),LAPV(n),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
            lwork = int(TMV_REAL(work[0]));
            work.resize(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#endif
#endif
            LAPNAME(zgeqrf) (
                LAPCM LAPV(m),LAPV(n),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
#ifdef LAPNOWORK
            LAP_Results("zgeqrf");
#else
            LAP_Results(int(TMV_REAL(work[0])),m,n,lwork,"zgeqrf");
#endif
            beta.conjugateSelf();
        }
        std::complex<double>* bi = beta.ptr();
        for(int i=0;i<n;++i,++bi) {
            // Sometimes (very rarely!) LAPACK comes back with an 
            // impossible beta value with |beta - 1| > 1.  e.g. (2,-1) (?!?)
            // This seems to only happen when beta should really be 0.  
            // So just set it to 0 now.
            if (TMV_ABS(*bi-1.) > 1.01) *bi = 0.;
            if (TMV_IMAG(*bi) != 0.) 
                det *= -TMV_CONJ(*bi * *bi)/norm(*bi);
            else if (TMV_REAL(*bi) != 0.)
                det = -det;
        }
    }
#endif
#ifdef INST_FLOAT
    template <> 
    void LapQRDecompose(
        const MatrixView<float>& A,
        const VectorView<float>& beta, float& det)
    {
        TMVAssert(A.colsize() >= A.rowsize());
        TMVAssert(A.rowsize() == beta.size());
        TMVAssert(A.rowsize() > 0);
        TMVAssert(A.ct() == NonConj);
        TMVAssert(A.isrm() || A.iscm());
        TMVAssert(beta.ct() == NonConj);
        TMVAssert(beta.step()==1);
        int m = A.colsize();
        int n = A.rowsize();
        beta.setZero();
        if (A.isrm()) {
            int lda = A.stepi();
#ifndef LAPNOWORK
#ifdef NOWORKQUERY
            int lwork = 2*n*LAP_BLOCKSIZE;
            AlignedArray<float> work(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#else
            int lwork = -1;
            AlignedArray<float> work(1);
            work.get()[0] = 0.F;
            LAPNAME(sgelqf) (
                LAPCM LAPV(n),LAPV(m),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
            lwork = int(work[0]);
            work.resize(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#endif
#endif
            LAPNAME(sgelqf) (
                LAPCM LAPV(n),LAPV(m),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
#ifdef LAPNOWORK
            LAP_Results("sgelqf");
#else
            LAP_Results(int(work[0]),m,n,lwork,"sgelqf");
#endif
        } else {
            int lda = A.stepj();
#ifndef LAPNOWORK
#ifdef NOWORKQUERY
            int lwork = 2*n*LAP_BLOCKSIZE;
            AlignedArray<float> work(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#else
            int lwork = -1;
            AlignedArray<float> work(1);
            work.get()[0] = 0.F;
            LAPNAME(sgeqrf) (
                LAPCM LAPV(m),LAPV(n),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
            lwork = int(work[0]);
            work.resize(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#endif
#endif
            LAPNAME(sgeqrf) (
                LAPCM LAPV(m),LAPV(n),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
#ifdef LAPNOWORK
            LAP_Results("sgeqrf");
#else
            LAP_Results(int(work[0]),m,n,lwork,"sgeqrf");
#endif
        }
        float* bi = beta.ptr();
        for(int i=0;i<n;++i,++bi) {
            if (TMV_ABS(*bi-1.F) > 1.01F) *bi = 0.F;
            if (*bi != 0.F) det = -det;
        }
    }
    template <> 
    void LapQRDecompose(
        const MatrixView<std::complex<float> >& A,
        const VectorView<std::complex<float> >& beta, std::complex<float>& det)
    {
        TMVAssert(A.colsize() >= A.rowsize());
        TMVAssert(A.rowsize() == beta.size());
        TMVAssert(A.rowsize() > 0);
        TMVAssert(A.ct() == NonConj);
        TMVAssert(A.isrm() || A.iscm());
        TMVAssert(beta.ct() == NonConj);
        TMVAssert(beta.step()==1);
        int m = A.colsize();
        int n = A.rowsize();
        beta.setZero();
        if (A.isrm()) {
            int lda = A.stepi();
#ifndef LAPNOWORK
#ifdef NOWORKQUERY
            int lwork = 2*n*LAP_BLOCKSIZE;
            AlignedArray<std::complex<float> > work(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#else
            int lwork = -1;
            AlignedArray<std::complex<float> > work(1);
            work.get()[0] = 0.F;
            LAPNAME(cgelqf) (
                LAPCM LAPV(n),LAPV(m),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
            lwork = int(TMV_REAL(work[0]));
            work.resize(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#endif
#endif
            LAPNAME(cgelqf) (
                LAPCM LAPV(n),LAPV(m),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
#ifdef LAPNOWORK
            LAP_Results("cgelqf");
#else
            LAP_Results(int(TMV_REAL(work[0])),m,n,lwork,"cgelqf");
#endif
        } else {
            int lda = A.stepj();
#ifndef LAPNOWORK
#ifdef NOWORKQUERY
            int lwork = 2*n*LAP_BLOCKSIZE;
            AlignedArray<std::complex<float> > work(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#else
            int lwork = -1;
            AlignedArray<std::complex<float> > work(1);
            work.get()[0] = 0.F;
            LAPNAME(cgeqrf) (
                LAPCM LAPV(m),LAPV(n),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
            lwork = int(TMV_REAL(work[0]));
            work.resize(lwork);
            VectorViewOf(work.get(),lwork).setZero();
#endif
#endif
            LAPNAME(cgeqrf) (
                LAPCM LAPV(m),LAPV(n),LAPP(A.ptr()),LAPV(lda),
                LAPP(beta.ptr()) LAPWK(work.get()) LAPVWK(lwork) LAPINFO);
#ifdef LAPNOWORK
            LAP_Results("cgeqrf");
#else
            LAP_Results(int(TMV_REAL(work[0])),m,n,lwork,"cgeqrf");
#endif
            beta.conjugateSelf();
        }
        std::complex<float>* bi = beta.ptr();
        for(int i=0;i<n;++i,++bi) {
            if (TMV_ABS(*bi-1.F) > 1.01F) *bi = 0.F;
            if (TMV_IMAG(*bi) != 0.F) 
                det *= -TMV_CONJ(*bi * *bi)/norm(*bi);
            else if (TMV_REAL(*bi) != 0.F)
                det = -det;
        }
    }
#endif
#endif

    template <class T> 
    void QR_Decompose(const MatrixView<T>& A, const VectorView<T>& beta, T& det)
    {
#ifdef XDEBUG
        cout<<"Start QR_Decompose\n";
        cout<<"A = "<<TMV_Text(A)<<endl;
        cout<<"Norm(A) = "<<Norm(A)<<endl;
        cout<<"beta = "<<TMV_Text(beta)<<endl;
        cout<<"Norm(beta) = "<<Norm(beta)<<endl;
        Matrix<T> A0(A);
#endif

        TMVAssert(A.colsize() >= A.rowsize());
        TMVAssert(A.rowsize() == beta.size());
        TMVAssert(A.ct() == NonConj);
        TMVAssert(A.isrm() || A.iscm());
        TMVAssert(beta.ct() == NonConj);
        TMVAssert(beta.step()==1);
        if (A.rowsize() > 0) {
#ifdef LAP
            LapQRDecompose(A,beta,det);
#else
            NonLapQRDecompose(A,beta,det);
#endif
        }
#ifdef XDEBUG
        cout<<"Done QR_Decompose:\n";
        cout<<"Norm(A) = "<<Norm(A)<<endl;
        cout<<"beta = "<<beta<<endl;
        cout<<"Norm(beta) = "<<Norm(beta)<<endl;
        Matrix<T> R = A.upperTri();
        Matrix<T> Q(A);
        GetQFromQR(Q.view(),beta);
        Matrix<T> AA = Q*R;
        cout<<"Norm(AA-A0) = "<<Norm(AA-A0)<<endl;
        if (!(Norm(AA-A0) <= 0.0001*Norm(Q)*Norm(R))) {
            cerr<<"QRDecompose: A = "<<TMV_Text(A)<<"  "<<A0<<endl;
            cerr<<"-> "<<A<<endl;
            cerr<<"beta = "<<beta<<endl;
            cerr<<"Q*R = "<<Q*R<<endl;
            cerr<<"Norm(diff) = "<<Norm(AA-A0)<<endl;
            Matrix<T> A2 = A0;
            Vector<T> beta2(beta.size());
            T det2(0);
            NonBlockQRDecompose(A2.view(),beta2.view(),det2);
            cerr<<"NonBlock "<<A2<<endl;
            cerr<<"beta = "<<beta2<<endl;
            abort(); 
        }
#endif
    }


    //
    // QR Decompose - Unpacked
    //

    template <class T> 
    void QR_Decompose(
        const MatrixView<T>& Q, const UpperTriMatrixView<T>& R, T& det)
    {
        // Decompose A (input as Q) into A = Q R 
        // where Q is unitary and R is upper triangular

        TMVAssert(Q.colsize() >= Q.rowsize());
        TMVAssert(R.colsize() == Q.rowsize());
        TMVAssert(R.rowsize() == Q.rowsize());
        TMVAssert(Q.ct() == NonConj);
        TMVAssert(R.ct() == NonConj);

        Vector<T> beta(Q.rowsize());
        QR_Decompose(Q,beta.view(),det);
        R = Q.upperTri();
        GetQFromQR(Q,beta.view());
    }

    template <class T> 
    void QR_Decompose(const MatrixView<T>& Q, const UpperTriMatrixView<T>& R)
    {
        T d(0);
        if (Q.isconj()) {
            if (R.isconj()) {
                QR_Decompose(Q.conjugate(),R.conjugate(),d);
            } else {
                QR_Decompose(Q.conjugate(),R,d);
                R.conjugateSelf();
            }
        } else {
            if (R.isconj()) {
                QR_Decompose(Q,R.conjugate(),d);
                R.conjugateSelf();
            } else {
                QR_Decompose(Q,R,d);
            }
        }
    }

    template <class T> 
    void QR_Decompose(const MatrixView<T>& A)
    {
        // Decompose A into Q R, but ignore Q.
        // On output, R is A.upperTri()

        TMVAssert(A.colsize() >= A.rowsize());

        Vector<T> beta(A.rowsize());
        T d(0);
        if (A.isconj()) 
            QR_Decompose(A.conjugate(),beta.view(),d);
        else
            QR_Decompose(A,beta.view(),d);
    }

#ifdef INST_INT
#undef INST_INT
#endif

#define InstFile "TMV_QRDecompose.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv


