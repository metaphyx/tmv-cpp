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


#include "TMV_Blas.h"
#include "TMV_BandLUDiv.h"
#include "tmv/TMV_BandMatrix.h"
#include "tmv/TMV_Vector.h"
#include "tmv/TMV_VectorArith.h"
#include "tmv/TMV_DiagMatrix.h"
#include "tmv/TMV_DiagMatrixArith.h"

#ifdef NOTHROW
#include <iostream>
#endif

#ifdef XDEBUG
#include "tmv/TMV_TriDiv.h"
#include "tmv/TMV_MatrixArith.h"
#include "tmv/TMV_TriMatrixArith.h"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#endif

namespace tmv {

  //
  // TriLDivEq
  //

  template <bool rm, bool ca, bool ua, class T, class Ta> 
  static void DoRowUpperTriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b)
  {
    TMVAssert(b.step()==1);
    TMVAssert(A.IsSquare());
    TMVAssert(b.size() == A.colsize());
    TMVAssert(b.size() > 0);
    TMVAssert(b.ct() == NonConj);
    TMVAssert(A.nlo() == 0);
    TMVAssert(A.nhi() > 0);
    TMVAssert(rm == A.isrm());
    TMVAssert(ca == A.isconj());

    const int N = b.size();

    const int sj = (rm?1:A.stepj());
    const int ds = A.diagstep();
    const Ta* Aii = A.cptr() + (ua ? N-2 : N-1)*ds;
    T* bi = b.ptr() + (ua ? N-2 : N-1);

    if (!ua) {
      if (*Aii==Ta(0)) 
#ifdef NOTHROW
      { std::cerr<<"Singular BandUpperTriMatrix found\n"; exit(1); }
#else
      throw SingularBandLU<Ta>(A);
#endif
#ifdef TMVFLDEBUG
      TMVAssert(bi >= b.first);
      TMVAssert(bi < b.last);
#endif
      *bi /= (ca ? CONJ(*Aii) : *Aii);
      Aii -= ds;
      --bi;
    }
    if (N==1) return;

    int k = A.nhi()-1;
    for(int i=N-1,len=1;i>0;--i,Aii-=ds,--bi) {
      // Actual row being done is i-1, not i

      // *bi -= A.row(i,i+1,j2) * b.SubVector(i+1,j2);
      const T* bj = bi+1;
      const Ta* Aij = Aii+sj;
      for(int j=len;j>0;--j,++bj,(rm?++Aij:Aij+=sj)) {
#ifdef TMVFLDEBUG
        TMVAssert(bi >= b.first);
        TMVAssert(bi < b.last);
#endif
        *bi -= *bj * (ca ? CONJ(*Aij) : *Aij);
      }

      if (!ua) {
        if (*Aii==Ta(0)) 
#ifdef NOTHROW
        { std::cerr<<"Singular BandUpperTriMatrix found\n"; exit(1); }
#else
        throw SingularBandLU<Ta>(A);
#endif
#ifdef TMVFLDEBUG
        TMVAssert(bi >= b.first);
        TMVAssert(bi < b.last);
#endif
        *bi /= (ca ? CONJ(*Aii) : *Aii);
      }
      if (k > 0) { --k; ++len; }
    } 
  }

  template <bool rm, bool ua, class T, class Ta> 
  static inline void RowUpperTriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b)
  {
    if (A.isconj())
      DoRowUpperTriLDivEq<rm,true,ua>(A,b);
    else
      DoRowUpperTriLDivEq<rm,false,ua>(A,b);
  }

  template <bool cm, bool ca, bool ua, class T, class Ta> 
  static void DoColUpperTriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b)
  {
    TMVAssert(b.step()==1);
    TMVAssert(A.IsSquare());
    TMVAssert(b.size() == A.colsize());
    TMVAssert(b.size() > 0);
    TMVAssert(b.ct() == NonConj);
    TMVAssert(A.nlo() == 0);
    TMVAssert(A.nhi() > 0);
    TMVAssert(cm == A.iscm());
    TMVAssert(ca == A.isconj());

    const int N = b.size();

    const int si = (cm ? 1 : A.stepi());
    const int sj = A.stepj();
    const int ds = A.diagstep();
    const int hi = A.nhi();

    int i1 = N-1;
    if (i1 > hi) i1 -= hi;
    else i1 = 0;

    const Ta* Ai1j = A.cptr()+(N-1)*sj+i1*si;
    const Ta* Ajj = (ua ? 0 : A.cptr()+(N-1)*ds); // if unit, this isn't used
    T* bi1 = b.ptr()+i1;
    T* bj = b.ptr()+N-1;

    for(int len=N-1-i1;len>0;--bj) {
      if (*bj != T(0)) {
        if (!ua) {
          if (*Ajj==Ta(0)) 
#ifdef NOTHROW
          { std::cerr<<"Singular BandUpperTriMatrix found\n"; exit(1); }
#else
          throw SingularBandLU<Ta>(A);
#endif
#ifdef TMVFLDEBUG
          TMVAssert(bj >= b.first);
          TMVAssert(bj < b.last);
#endif
          *bj /= (ca ? CONJ(*Ajj) : *Ajj);
          Ajj -= ds;
        }

        // b.SubVector(i1,j) -= (*bj) * A.col(j,i1,j);
        T* bi = bi1;
        const Ta* Aij = Ai1j;
        for(int i=len;i>0;--i,++bi,(cm?++Aij:Aij+=si)) {
#ifdef TMVFLDEBUG
          TMVAssert(bi >= b.first);
          TMVAssert(bi < b.last);
#endif
          *bi -= *bj * (ca ? CONJ(*Aij) : *Aij);
        }
      }
      else if (!ua) Ajj -= ds;

      if (i1 > 0) { --i1; --bi1; Ai1j-=ds; } 
      else { --len; Ai1j-=sj; }
    } 
    if (!ua && *bj != T(0)) {
      if (*Ajj==Ta(0)) 
#ifdef NOTHROW
      { std::cerr<<"Singular BandUpperTriMatrix found\n"; exit(1); }
#else
      throw SingularBandLU<Ta>(A);
#endif
#ifdef TMVFLDEBUG
      TMVAssert(bj >= b.first);
      TMVAssert(bj < b.last);
#endif
      *bj /= (ca ? CONJ(*Ajj) : *Ajj);
    }
  }

  template <bool cm, bool ua, class T, class Ta> 
  static inline void ColUpperTriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b)
  {
    if (A.isconj())
      DoColUpperTriLDivEq<cm,true,ua>(A,b);
    else
      DoColUpperTriLDivEq<cm,false,ua>(A,b);
  }

  template <bool rm, bool ca, bool ua, class T, class Ta> 
  static void DoRowLowerTriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b)
  {
    TMVAssert(b.step()==1);
    TMVAssert(A.IsSquare());
    TMVAssert(b.size() == A.colsize());
    TMVAssert(b.size() > 0);
    TMVAssert(b.ct() == NonConj);
    TMVAssert(A.nhi() == 0);
    TMVAssert(A.nlo() > 0);
    TMVAssert(rm == A.isrm());
    TMVAssert(ca == A.isconj());

    const int N = A.colsize();

    const int sj = (rm ? 1 : A.stepj());
    const int si = A.stepi();
    const int ds = A.diagstep();

    const Ta* Aij1 = A.cptr();
    const T* bj1 = b.cptr();
    T* bi = b.ptr();

    if (!ua) {
      if (*Aij1==Ta(0)) 
#ifdef NOTHROW
      { std::cerr<<"Singular BandLowerTriMatrix found\n"; exit(1); }
#else
      throw SingularBandLU<Ta>(A);
#endif
#ifdef TMVFLDEBUG
      TMVAssert(bi >= b.first);
      TMVAssert(bi < b.last);
#endif
      *bi /= (ca ? CONJ(*Aij1) : *Aij1);
    }

    ++bi;
    Aij1 += si;
    int k=A.nlo()-1;

    for(int i=1,len=1;i<N;++i,++bi) {
      // *bi -= A.row(i,j1,i) * b.SubVector(j1,i);
      const Ta* Aij = Aij1;
      const T* bj = bj1;
      for(int j=len;j>0;--j,++bj,(rm?++Aij:Aij+=sj)) {
#ifdef TMVFLDEBUG
        TMVAssert(bj >= b.first);
        TMVAssert(bj < b.last);
#endif
        *bi -= *bj * (ca ? CONJ(*Aij) : *Aij);
      }
      if (!ua) {
        // Aij is Aii after the above for loop
        if (*Aij == Ta(0))
#ifdef NOTHROW
        { std::cerr<<"Singular BandLowerTriMatrix found\n"; exit(1); }
#else
        throw SingularBandLU<Ta>(A);
#endif
#ifdef TMVFLDEBUG
        TMVAssert(bi >= b.first);
        TMVAssert(bi < b.last);
#endif
        *bi /= (ca ? CONJ(*Aij) : *Aij);
      }
      if (k>0) { --k; ++len; Aij1+=A.stepi(); } 
      else { ++bj1; Aij1+=ds; }
    }
  }

  template <bool rm, bool ua, class T, class Ta> 
  static inline void RowLowerTriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b)
  {
    if (A.isconj())
      DoRowLowerTriLDivEq<rm,true,ua>(A,b);
    else
      DoRowLowerTriLDivEq<rm,false,ua>(A,b);
  }

  template <bool cm, bool ca, bool ua, class T, class Ta> 
  static void DoColLowerTriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b)
  {
    TMVAssert(b.step()==1);
    TMVAssert(A.IsSquare());
    TMVAssert(b.size() == A.colsize());
    TMVAssert(b.size() > 0);
    TMVAssert(b.ct() == NonConj);
    TMVAssert(A.nhi() == 0);
    TMVAssert(A.nlo() > 0);
    TMVAssert(cm == A.iscm());
    TMVAssert(ca == A.isconj());

    const int N = A.colsize();

    const int si = (cm ? 1 : A.stepi());
    const int ds = A.diagstep();

    const Ta* Ajj= A.cptr();
    T* bj = b.ptr();

    int i2=MIN(A.nlo()+1,int(A.colsize()));

    for(int len=i2-1;len>0;++bj,Ajj+=ds) {
      if (*bj != T(0)) {
        if (!ua) {
          if (*Ajj==Ta(0)) 
#ifdef NOTHROW
          { std::cerr<<"Singular BandLowerTriMatrix found\n"; exit(1); }
#else
          throw SingularBandLU<Ta>(A);
#endif
#ifdef TMVFLDEBUG
          TMVAssert(bj >= b.first);
          TMVAssert(bj < b.last);
#endif
          *bj /= (ca ? CONJ(*Ajj) : *Ajj);
        }
        // b.SubVector(j+1,i2) -= *bj * A.col(j,j+1,i2)
        T* bi = bj+1;
        const Ta* Aij = Ajj+si;
        for(int i=len;i>0;--i,++bi,(cm?++Aij:Aij+=si)) {
#ifdef TMVFLDEBUG
          TMVAssert(bi >= b.first);
          TMVAssert(bi < b.last);
#endif
          *bi -= *bj * (ca ? CONJ(*Aij) : *Aij);
        }
      }
      if (i2 < N) ++i2; else --len;
    }
    if (!ua && *bj != T(0)) {
      if (*Ajj==Ta(0)) 
#ifdef NOTHROW
      { std::cerr<<"Singular BandLowerTriMatrix found\n"; exit(1); }
#else
      throw SingularBandLU<Ta>(A);
#endif
#ifdef TMVFLDEBUG
      TMVAssert(bj >= b.first);
      TMVAssert(bj < b.last);
#endif
      *bj /= (ca ? CONJ(*Ajj) : *Ajj);
    }
  }

  template <bool rm, bool ua, class T, class Ta> 
  static inline void ColLowerTriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b)
  {
    if (A.isconj())
      DoColLowerTriLDivEq<rm,true,ua>(A,b);
    else
      DoColLowerTriLDivEq<rm,false,ua>(A,b);
  }

  template <class T, class Ta> static void UpperTriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b, DiagType dt)
  {
    TMVAssert(A.nlo() == 0);
    if (A.nhi() == 0) {
      if (dt == NonUnitDiag) b /= DiagMatrixViewOf(A.diag());
    } else {
      if (dt == UnitDiag)
        if (A.isrm()) RowUpperTriLDivEq<true,true>(A,b);
        else if (A.iscm()) ColUpperTriLDivEq<true,true>(A,b);
        else RowUpperTriLDivEq<false,true>(A,b);
      else
        if (A.isrm()) RowUpperTriLDivEq<true,false>(A,b);
        else if (A.iscm()) ColUpperTriLDivEq<true,false>(A,b);
        else RowUpperTriLDivEq<false,false>(A,b);
    }
  }

  template <class T, class Ta> static void LowerTriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b, DiagType dt)
  {
    TMVAssert(A.nhi() == 0);
    if (A.nlo() == 0) {
      if (dt == NonUnitDiag) b /= DiagMatrixViewOf(A.diag());
    } else {
      if (dt == UnitDiag)
        if (A.isrm()) RowLowerTriLDivEq<true,true>(A,b);
        else if (A.iscm()) ColLowerTriLDivEq<true,true>(A,b);
        else RowLowerTriLDivEq<false,true>(A,b);
      else
        if (A.isrm()) RowLowerTriLDivEq<true,false>(A,b);
        else if (A.iscm()) ColLowerTriLDivEq<true,false>(A,b);
        else RowLowerTriLDivEq<false,false>(A,b);
    }
  }

  template <class T, class Ta> static void NonBlasTriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b, DiagType dt)
  {
    //cout<<"Start NonBlasTriLDvEq\n";
    // Solve A x = y  where A is an upperor lower band triangle matrix
    TMVAssert(A.IsSquare());
    TMVAssert(b.size() == A.colsize());
    TMVAssert(b.size() > 0);
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(b.ct() == NonConj);

    const int N = b.size();
    if (b.step() == 1) {
      if (A.nlo() == 0) {
        int i2 = N;
        for(const T* b2 = b.cptr()+i2-1; i2>0 && *b2==T(0); --i2,--b2);
        if (i2==0) return;
        else if (i2 == N) 
          UpperTriLDivEq(A,b,dt);
        else if (A.nhi() < i2)
          UpperTriLDivEq(A.SubBandMatrix(0,i2,0,i2,0,A.nhi()),
              b.SubVector(0,i2),dt);
        else
          UpperTriLDivEq(A.SubBandMatrix(0,i2,0,i2,0,i2-1),
              b.SubVector(0,i2),dt);
      } else {
        int i1 = 0;
        for(const T* b1 = b.ptr(); i1<N && *b1==T(0); ++i1,++b1);
        if (i1==N) return;
        else if (i1 == 0) 
          LowerTriLDivEq(A,b,dt);
        else if (A.nlo() < (N-i1))
          LowerTriLDivEq(A.SubBandMatrix(i1,N,i1,N,A.nlo(),0),
              b.SubVector(i1,N),dt);
        else
          LowerTriLDivEq(A.SubBandMatrix(i1,N,i1,N,N-i1-1,0),
              b.SubVector(i1,N),dt);
      }
    } else {
      Vector<T> bb = b;
      NonBlasTriLDivEq(A,bb.View(),dt);
      b = bb;
    }
  }

#ifdef BLAS
  template <class T, class Ta> static inline void BlasTriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b, DiagType dt)
  { NonBlasTriLDivEq(A,b,dt); }
#ifdef INST_DOUBLE
  template <> void BlasTriLDivEq(
      const GenBandMatrix<double>& A, const VectorView<double>& b, DiagType dt)
  {
    TMVAssert(A.IsSquare());
    TMVAssert(b.size() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.rowsize() > 0);
    TMVAssert(A.iscm() || A.isrm());
    TMVAssert(A.ct() == NonConj);
    TMVAssert(b.ct() == NonConj);
    int n = A.colsize();
    int kd = A.nlo()==0 ? A.nhi() : A.nlo();
    int aoffset = A.isrm() ? A.nlo() : A.nhi();
    int ds = A.diagstep();
    int s = b.step();
    double* bp = b.ptr();
    if (s < 0) bp += (n-1)*s;
    BLASNAME(dtbsv) (BLASCM 
        ((A.nlo()==0) == A.isrm()) ?BLASCH_LO:BLASCH_UP, 
        A.isrm() ? BLASCH_T : BLASCH_NT, 
        dt==UnitDiag ? BLASCH_U : BLASCH_NU,
        BLASV(n), BLASV(kd), BLASP(A.cptr()-aoffset), BLASV(ds),
        BLASP(bp), BLASV(s) BLAS1 BLAS1 BLAS1);
  }
  template <> void BlasTriLDivEq(
      const GenBandMatrix<std::complex<double> >& A,
      const VectorView<std::complex<double> >& b, DiagType dt)
  {
    TMVAssert(A.IsSquare());
    TMVAssert(b.size() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.rowsize() > 0);
    TMVAssert(A.iscm() || A.isrm());
    TMVAssert(b.ct() == NonConj);

    int n = A.colsize();
    int kd = A.nlo()==0 ? A.nhi() : A.nlo();
    int aoffset = A.isrm() ? A.nlo() : A.nhi();
    int ds = A.diagstep();
    int s = b.step();
    std::complex<double>* bp = b.ptr();
    if (s < 0) bp += (n-1)*s;
    if (A.iscm() && A.isconj()) {
#ifdef CBLAS
      BLASNAME(ztbsv) (BLASRM
          A.nlo()==0 ? BLASCH_LO : BLASCH_UP, BLASCH_CT,
          dt==UnitDiag ? BLASCH_U : BLASCH_NU, 
          BLASV(n), BLASV(kd), BLASP(A.cptr()-aoffset), BLASV(ds),
          BLASP(bp), BLASV(s) BLAS1 BLAS1 BLAS1);
#else
      b.ConjugateSelf();
      BLASNAME(ztbsv) (BLASCM A.nlo()==0?BLASCH_UP:BLASCH_LO, BLASCH_NT, 
          dt==UnitDiag ? BLASCH_U : BLASCH_NU,
          BLASV(n), BLASV(kd), BLASP(A.cptr()-aoffset), BLASV(ds),
          BLASP(bp), BLASV(s) BLAS1 BLAS1 BLAS1);
      b.ConjugateSelf();
#endif
    } else {
      BLASNAME(ztbsv) (BLASCM 
          ((A.nlo()==0) == A.isrm()) ?BLASCH_LO:BLASCH_UP, 
          A.isrm() ? A.isconj() ? BLASCH_CT : BLASCH_T : BLASCH_NT, 
          dt==UnitDiag ? BLASCH_U : BLASCH_NU,
          BLASV(n), BLASV(kd), BLASP(A.cptr()-aoffset), BLASV(ds),
          BLASP(bp), BLASV(s) BLAS1 BLAS1 BLAS1);
    }
  }
  template <> void BlasTriLDivEq(
      const GenBandMatrix<double>& A, 
      const VectorView<std::complex<double> >& b, DiagType dt)
  {
    TMVAssert(A.IsSquare());
    TMVAssert(b.size() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.rowsize() > 0);
    TMVAssert(A.iscm() || A.isrm());
    TMVAssert(A.ct() == NonConj);
    TMVAssert(b.ct() == NonConj);
    int n = A.colsize();
    int kd = A.nlo()==0 ? A.nhi() : A.nlo();
    int aoffset = A.isrm() ? A.nlo() : A.nhi();
    int ds = A.diagstep();
    int s = 2*b.step();
    double* bp = (double*) b.ptr();
    if (s < 0) bp += (n-1)*s;
    BLASNAME(dtbsv) (BLASCM 
        ((A.nlo()==0) == A.isrm()) ?BLASCH_LO:BLASCH_UP, 
        A.isrm() ? BLASCH_T : BLASCH_NT, 
        dt==UnitDiag ? BLASCH_U : BLASCH_NU,
        BLASV(n), BLASV(kd), BLASP(A.cptr()-aoffset), BLASV(ds),
        BLASP(bp), BLASV(s) BLAS1 BLAS1 BLAS1);
    BLASNAME(dtbsv) (BLASCM 
        ((A.nlo()==0) == A.isrm()) ?BLASCH_LO:BLASCH_UP, 
        A.isrm() ? BLASCH_T : BLASCH_NT, 
        dt==UnitDiag ? BLASCH_U : BLASCH_NU,
        BLASV(n), BLASV(kd), BLASP(A.cptr()-aoffset), BLASV(ds),
        BLASP(bp+1), BLASV(s) BLAS1 BLAS1 BLAS1);
  }
#endif
#ifdef INST_FLOAT
  template <> void BlasTriLDivEq(
      const GenBandMatrix<float>& A, const VectorView<float>& b, DiagType dt)
  {
    TMVAssert(A.IsSquare());
    TMVAssert(b.size() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.rowsize() > 0);
    TMVAssert(A.iscm() || A.isrm());
    TMVAssert(A.ct() == NonConj);
    TMVAssert(b.ct() == NonConj);
    int n = A.colsize();
    int kd = A.nlo()==0 ? A.nhi() : A.nlo();
    int aoffset = A.isrm() ? A.nlo() : A.nhi();
    int ds = A.diagstep();
    int s = b.step();
    float* bp = b.ptr();
    if (s < 0) bp += (n-1)*s;
    BLASNAME(stbsv) (BLASCM 
        ((A.nlo()==0) == A.isrm()) ?BLASCH_LO:BLASCH_UP, 
        A.isrm() ? BLASCH_T : BLASCH_NT, 
        dt==UnitDiag ? BLASCH_U : BLASCH_NU,
        BLASV(n), BLASV(kd), BLASP(A.cptr()-aoffset), BLASV(ds),
        BLASP(bp), BLASV(s) BLAS1 BLAS1 BLAS1);
  }
  template <> void BlasTriLDivEq(
      const GenBandMatrix<std::complex<float> >& A,
      const VectorView<std::complex<float> >& b, DiagType dt)
  {
    TMVAssert(A.IsSquare());
    TMVAssert(b.size() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.rowsize() > 0);
    TMVAssert(A.iscm() || A.isrm());
    TMVAssert(b.ct() == NonConj);

    int n = A.colsize();
    int kd = A.nlo()==0 ? A.nhi() : A.nlo();
    int aoffset = A.isrm() ? A.nlo() : A.nhi();
    int ds = A.diagstep();
    int s = b.step();
    std::complex<float>* bp = b.ptr();
    if (s < 0) bp += (n-1)*s;
    if (A.iscm() && A.isconj()) {
#ifdef CBLAS
      BLASNAME(ctbsv) (BLASRM
          A.nlo()==0 ? BLASCH_LO : BLASCH_UP, BLASCH_CT,
          dt==UnitDiag ? BLASCH_U : BLASCH_NU, 
          BLASV(n), BLASV(kd), BLASP(A.cptr()-aoffset), BLASV(ds),
          BLASP(bp), BLASV(s) BLAS1 BLAS1 BLAS1);
#else
      b.ConjugateSelf();
      BLASNAME(ctbsv) (BLASCM A.nlo()==0?BLASCH_UP:BLASCH_LO, BLASCH_NT, 
          dt==UnitDiag ? BLASCH_U : BLASCH_NU,
          BLASV(n), BLASV(kd), BLASP(A.cptr()-aoffset), BLASV(ds),
          BLASP(bp), BLASV(s) BLAS1 BLAS1 BLAS1);
      b.ConjugateSelf();
#endif
    } else {
      BLASNAME(ctbsv) (BLASCM 
          ((A.nlo()==0) == A.isrm()) ?BLASCH_LO:BLASCH_UP, 
          A.isrm() ? A.isconj() ? BLASCH_CT : BLASCH_T : BLASCH_NT, 
          dt==UnitDiag ? BLASCH_U : BLASCH_NU,
          BLASV(n), BLASV(kd), BLASP(A.cptr()-aoffset), BLASV(ds),
          BLASP(bp), BLASV(s) BLAS1 BLAS1 BLAS1);
    }
  }
  template <> void BlasTriLDivEq(
      const GenBandMatrix<float>& A, 
      const VectorView<std::complex<float> >& b, DiagType dt)
  {
    TMVAssert(A.IsSquare());
    TMVAssert(b.size() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.rowsize() > 0);
    TMVAssert(A.iscm() || A.isrm());
    TMVAssert(A.ct() == NonConj);
    TMVAssert(b.ct() == NonConj);
    int n = A.colsize();
    int kd = A.nlo()==0 ? A.nhi() : A.nlo();
    int aoffset = A.isrm() ? A.nlo() : A.nhi();
    int ds = A.diagstep();
    int s = 2*b.step();
    float* bp = (float*) b.ptr();
    if (s < 0) bp += (n-1)*s;
    BLASNAME(stbsv) (BLASCM 
        ((A.nlo()==0) == A.isrm()) ?BLASCH_LO:BLASCH_UP, 
        A.isrm() ? BLASCH_T : BLASCH_NT, 
        dt==UnitDiag ? BLASCH_U : BLASCH_NU,
        BLASV(n), BLASV(kd), BLASP(A.cptr()-aoffset), BLASV(ds),
        BLASP(bp), BLASV(s) BLAS1 BLAS1 BLAS1);
    BLASNAME(stbsv) (BLASCM 
        ((A.nlo()==0) == A.isrm()) ?BLASCH_LO:BLASCH_UP, 
        A.isrm() ? BLASCH_T : BLASCH_NT, 
        dt==UnitDiag ? BLASCH_U : BLASCH_NU,
        BLASV(n), BLASV(kd), BLASP(A.cptr()-aoffset), BLASV(ds),
        BLASP(bp+1), BLASV(s) BLAS1 BLAS1 BLAS1);
  }
#endif 
#endif // BLAS

  template <class T, class Ta> void TriLDivEq(
      const GenBandMatrix<Ta>& A, const VectorView<T>& b, DiagType dt)
  {
#ifdef XDEBUG
    Vector<T> b0 = b;
    Matrix<T> A0 = A;
    if (dt == UnitDiag) A0.diag().SetAllTo(T(1));
#endif
    TMVAssert(A.IsSquare());
    TMVAssert(b.size() == A.colsize());
    TMVAssert(A.nlo() == 0 || A.nhi() == 0);
    TMVAssert(A.rowsize() > 0);
    TMVAssert(A.nhi() < int(A.colsize()));
    TMVAssert(A.nlo() < int(A.colsize()));
    if (b.isconj())
      TriLDivEq(A.Conjugate(),b.Conjugate(),dt);
    else {
#ifdef BLAS
      if (A.isrm() || A.iscm())
        BlasTriLDivEq(A,b,dt);
      else {
        BandMatrix<Ta,ColMajor> AA = A;
        BlasTriLDivEq(AA,b,dt);
      }
#else
      NonBlasTriLDivEq(A,b,dt);
#endif
    }

#ifdef XDEBUG
    Vector<T> bb = A0*b;
    if (Norm(bb-b0) > 0.001*Norm(b0)*Norm(A0)*Norm(A0.Inverse())) {
      cerr<<"TriLDivEq Vector:\n";
      cerr<<"A = "<<TypeText(A)<<A0<<endl;
      cerr<<"b = "<<TypeText(b)<<b0<<endl;
      cerr<<"--> b = "<<b<<endl;
      cerr<<"A*b = "<<bb<<endl;
      abort();
    }
#endif
  }

#define InstFile "TMV_BandTriDiv_V.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv

