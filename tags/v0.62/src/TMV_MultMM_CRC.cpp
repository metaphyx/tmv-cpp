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


//#include <iostream>

#include "TMV_Blas.h" // Sets BLAS if appropriate
#include "TMV_MultMM.h"
#include "tmv/TMV_Matrix.h"
#include "tmv/TMV_MatrixArith.h"

#ifdef _OPENMP
//#include <omp.h>
#endif

namespace tmv {

  template <bool ca, class T, class Ta, class Tb> 
  static void ColMult(int M, const Tb b00, const Ta* A, T* C)
  {
    for(int i=M/8;i;--i) {
#ifdef TMVFLDEBUG
      TMVAssert(C >= _first);
      TMVAssert(C+7 < _last);
#endif
      C[0] += (ca ? CONJ(A[0]) : A[0]) * b00;
      C[1] += (ca ? CONJ(A[1]) : A[1]) * b00;
      C[2] += (ca ? CONJ(A[2]) : A[2]) * b00;
      C[3] += (ca ? CONJ(A[3]) : A[3]) * b00;
      C[4] += (ca ? CONJ(A[4]) : A[4]) * b00;
      C[5] += (ca ? CONJ(A[5]) : A[5]) * b00;
      C[6] += (ca ? CONJ(A[6]) : A[6]) * b00;
      C[7] += (ca ? CONJ(A[7]) : A[7]) * b00;
      A += 8; C += 8;
    }
    M %= 8;
    if (M) {
      if (M >= 4) {
#ifdef TMVFLDEBUG
        TMVAssert(C >= _first);
        TMVAssert(C+3 < _last);
#endif
        C[0] += (ca ? CONJ(A[0]) : A[0]) * b00;
        C[1] += (ca ? CONJ(A[1]) : A[1]) * b00;
        C[2] += (ca ? CONJ(A[2]) : A[2]) * b00;
        C[3] += (ca ? CONJ(A[3]) : A[3]) * b00;
        M -= 4; A += 4; C += 4;
      }
      if (M >= 2) {
#ifdef TMVFLDEBUG
        TMVAssert(C >= _first);
        TMVAssert(C+1 < _last);
#endif
        C[0] += (ca ? CONJ(A[0]) : A[0]) * b00;
        C[1] += (ca ? CONJ(A[1]) : A[1]) * b00;
        M -= 2; A += 2; C += 2;
      }
      if (M) {
#ifdef TMVFLDEBUG
        TMVAssert(C >= _first);
        TMVAssert(C < _last);
#endif
        C[0] += (ca ? CONJ(A[0]) : A[0]) * b00;
      }
    }
  }

  template <bool ca, bool cb, class T, class Ta, class Tb> 
  extern void RecursiveCRCMultMM(
      const int M, const int N, const int K,
      const Ta* A, const Tb* B, T* C,
      const int Ask, const int Bsk, const int Csj)
  {
    if (K > N) {
      int K1 = K/2;
      RecursiveCRCMultMM<ca,cb>(M,N,K1,A,B,C,Ask,Bsk,Csj );
      RecursiveCRCMultMM<ca,cb>(M,N,K-K1,A+K1*Ask,B+K1*Bsk,C,Ask,Bsk,Csj );
    } else if (N > 1) {
      int N1 = N/2;
      RecursiveCRCMultMM<ca,cb>(M,N1,K,A,B,C,Ask,Bsk,Csj );
      RecursiveCRCMultMM<ca,cb>(M,N-N1,K,A,B+N1,C+N1*Csj,Ask,Bsk,Csj );
    } else {
      ColMult<ca>(M,(cb ? CONJ(B[0]) : B[0]),A,C);
    }
  }

  static void MakeTaskList(int M,int N, int i, int j, 
      std::vector<int>& ilist, std::vector<int>& jlist, int& index)
  {
    if (M > N) {
      int M1 = M/2;
      MakeTaskList(M1,N,i,j,ilist,jlist,index);
      MakeTaskList(M-M1,N,i+M1,j,ilist,jlist,index);
    } else if (N > 1) {
      int N1 = N/2;
      MakeTaskList(M,N1,i,j,ilist,jlist,index);
      MakeTaskList(M,N-N1,i,j+N1,ilist,jlist,index);
    } else {
      ilist[index] = i;
      jlist[index] = j;
      index++;
    }
  }

  template <bool add, bool ca, bool cb, class T, class Ta, class Tb> 
  extern void DoCRCMultMM(
      const T alpha, const GenMatrix<Ta>& A, const GenMatrix<Tb>& B,
      const MatrixView<T>& C)
  {
    //std::cout<<"Start DoCRCMultMM"<<std::endl;
    TMVAssert(A.colsize() == C.colsize());
    TMVAssert(A.rowsize() == B.colsize());
    TMVAssert(B.rowsize() == C.rowsize());
    TMVAssert(C.colsize() > 0);
    TMVAssert(C.rowsize() > 0);
    TMVAssert(A.rowsize() > 0);
    TMVAssert(alpha != T(0));
    TMVAssert(ca == A.isconj());
    TMVAssert(cb == B.isconj());
    TMVAssert(C.ct()==NonConj);
    TMVAssert(A.iscm());
    TMVAssert(B.isrm());
    TMVAssert(C.iscm());

    const int M = C.colsize();
    const int N = C.rowsize();
    const int K = A.rowsize();
    const int Ask = A.stepj();
    const int Bsk = B.stepi();

    const Ta* Ap = A.cptr();
    const Tb* Bp = B.cptr();

    const int MB = MMCRC_BLOCKSIZE_M;
    const int NB = MMCRC_BLOCKSIZE_N*sizeof(double)/sizeof(T);
    const int Mb = M/MB;
    const int Nb = N/NB;
    //std::cout<<"Mb = "<<Mb<<", Nb = "<<Nb<<std::endl;
    //std::cout<<"M = "<<M<<", N = "<<N<<std::endl;
    if (Mb || Nb) {
      const int M2 = M-Mb*MB;
      const int N2 = N-Nb*NB;
      const int M1 = M-M2;
      const int N1 = N-N2;

      // This is best done recursively to avoid cache misses as much 
      // as possible, but openmp doesn't do recursion very well (yet), 
      // so I just figure out the recursive order first and do a 
      // simple loop that openmp can divvy up appropriately.
      const int MbNb = Mb*Nb;
      std::vector<int> ilist(MbNb);
      std::vector<int> jlist(MbNb);
      if (MbNb) {
        int listindex=0;
        MakeTaskList(Mb,Nb,0,0,ilist,jlist,listindex);
      }
#ifdef _OPENMP
#pragma omp parallel
      {
        // OpenMP seems to have trouble with new [], so just use a 
        // static array.  
        // However, the compiler needs to know the size at compile time
        // so a little wasted memory here, always using the full MB*NB.
        T Ct[ MB * NB ];
        MatrixView<T> Ctemp = MatrixViewOf(Ct,MIN(M,MB),MIN(N,NB),ColMajor);
#else
        Matrix<T,ColMajor> Ctemp(MIN(M,MB),MIN(N,NB));
        T* Ct = Ctemp.ptr();
#endif
        const int Ctsj = Ctemp.stepj();

        // Full MBxNB blocks in C matrix
        if (Mb && Nb) {
#ifdef _OPENMP
#pragma omp for nowait schedule(guided)
#endif
          for(int ij=0;ij<int(MbNb);ij++) {
            const int i=ilist[ij];
            const int j=jlist[ij];
            const int ii = i*MB;
            const int jj = j*NB;

            Ctemp.Zero();
            RecursiveCRCMultMM<ca,cb>(MB,NB,K,Ap+ii,Bp+jj,Ct,Ask,Bsk,Ctsj);
            if (alpha != T(1)) Ctemp *= alpha;
#ifdef _OPENMP
#pragma omp critical
#endif
            {
              if (add) C.SubMatrix(ii,ii+MB,jj,jj+NB) += Ctemp;
              else C.SubMatrix(ii,ii+MB,jj,jj+NB) = Ctemp;
            }
          }
        } // Mb && Nb

        // MB x N2 partial blocks:
        if (Mb && N2) {
#ifdef _OPENMP
#pragma omp for nowait //schedule(dynamic,1)
#endif
          for(int i=0;i<int(Mb);i++) {
            const int ii = i*MB;
            Ctemp.Zero();
            RecursiveCRCMultMM<ca,cb>(MB,N2,K,Ap+ii,Bp+N1,Ct,Ask,Bsk,Ctsj);
            if (alpha != T(1)) Ctemp.Cols(0,N2) *= alpha;
#ifdef _OPENMP
#pragma omp critical
#endif
            {
              if (add) C.SubMatrix(ii,ii+MB,N1,N) += Ctemp.Cols(0,N2);
              else C.SubMatrix(ii,ii+MB,N1,N) = Ctemp.Cols(0,N2);
            }
          }
        } // Mb && N2

        // M2 x NB partial blocks:
        if (M2 && Nb) {
#ifdef _OPENMP
#pragma omp for nowait //schedule(dynamic,1)
#endif
          for(int j=0;j<int(Nb);j++) {
            const int jj = j*NB;
            Ctemp.Zero();
            RecursiveCRCMultMM<ca,cb>(M2,NB,K,Ap+M1,Bp+jj,Ct,Ask,Bsk,Ctsj);
            if (alpha != T(1)) Ctemp.Rows(0,M2) *= alpha;
#ifdef _OPENMP
#pragma omp critical
#endif
            {
              if (add) C.SubMatrix(M1,M,jj,jj+NB) += Ctemp.Rows(0,M2);
              else C.SubMatrix(M1,M,jj,jj+NB) = Ctemp.Rows(0,M2);
            }
          }
        } // M2 && Nb

        // Final M2 x N2 partial block
        if (M2 && N2) {
#ifdef _OPENMP
#pragma omp single
#endif
          {
            Ctemp.Zero();
            RecursiveCRCMultMM<ca,cb>(M2,N2,K,Ap+M1,Bp+N1,Ct,Ask,Bsk,Ctsj);
            if (alpha != T(1)) Ctemp.SubMatrix(0,M2,0,N2) *= alpha;
#ifdef _OPENMP
#pragma omp critical
#endif
            {
              if (add) C.SubMatrix(M1,M,N1,N) += Ctemp.SubMatrix(0,M2,0,N2);
              else C.SubMatrix(M1,M,N1,N) = Ctemp.SubMatrix(0,M2,0,N2);
            }
          }
        } // M2 && N2
#ifdef _OPENMP
      } // end parallel
#endif
    } // Mb || Nb
    else {
      Matrix<T,ColMajor> Ctemp(M,N,T(0));
      T* Ct = Ctemp.ptr();
      const int Ctsj = Ctemp.stepj();
      RecursiveCRCMultMM<ca,cb>(M,N,K,Ap,Bp,Ct, Ask,Bsk,Ctsj);
      if (alpha != T(1)) Ctemp *= alpha;
      if (add) C += Ctemp;
      else C = Ctemp;
    }
  }

  template <bool add, class T, class Ta, class Tb> void CRCMultMM(
      const T alpha, const GenMatrix<Ta>& A, const GenMatrix<Tb>& B,
      const MatrixView<T>& C)
  {
    //std::cout<<"CRCMultMM: "<<A.colsize()<<", "<<A.rowsize()<<", "<<C.rowsize()<<std::endl;
    if (A.isconj())
      if (B.isconj())
        DoCRCMultMM<add,true,true>(alpha,A,B,C);
      else
        DoCRCMultMM<add,true,false>(alpha,A,B,C);
    else
      if (B.isconj())
        DoCRCMultMM<add,false,true>(alpha,A,B,C);
      else
        DoCRCMultMM<add,false,false>(alpha,A,B,C);
    //std::cout<<"Done CRCMultMM"<<std::endl;
  }

#ifdef BLAS
#define INST_SKIP_BLAS
#endif

#define InstFile "TMV_MultMM_CRC.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv

