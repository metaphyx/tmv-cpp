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



#include "tmv/TMV_QRPD.h"
#include "TMV_QRPDiv.h"
#include "tmv/TMV_Matrix.h"
#include "tmv/TMV_TriMatrix.h"
#include "tmv/TMV_TriMatrixArith.h"
#include "tmv/TMV_DiagMatrix.h"
#include "TMV_QRDiv.h"
#include "tmv/TMV_MatrixArith.h"
#include "tmv/TMV_PackedQ.h"
#include <ostream>

namespace tmv {

  template <class T> bool QRPDiv<T>::StrictQRP = false;

  template <class T> struct QRPDiv<T>::QRPDiv_Impl
  {
  public :
    QRPDiv_Impl(const GenMatrix<T>& m, bool inplace);

    const bool istrans;
    const bool inplace;
    auto_array<T> Aptr1;
    T* Aptr;
    MatrixView<T> QRx;
    Vector<T> beta;
    auto_array<int> P;
    mutable RealType(T) logdet;
    mutable T signdet;
    mutable bool donedet;
    int N1;
  };

#define APTR1 (inplace ? 0 : new T[A.colsize()*A.rowsize()])
#define APTR (inplace ? A.NonConst().ptr() : Aptr1.get())
#define QRX (istrans ? \
    (inplace ? A.NonConst().Transpose() : \
     MatrixViewOf(Aptr,A.rowsize(),A.colsize(),ColMajor)) : \
    (inplace ? A.NonConst().View() : \
     MatrixViewOf(Aptr,A.colsize(),A.rowsize(),ColMajor)))

  template <class T> QRPDiv<T>::QRPDiv_Impl::QRPDiv_Impl(
      const GenMatrix<T>& A, bool _inplace) :
    istrans(A.colsize()<A.rowsize()),
    inplace(_inplace && (A.iscm() || A.isrm())), 
    Aptr1(APTR1), Aptr(APTR), QRx(QRX), 
    beta(QRx.rowsize()), P(new int[beta.size()]),
    logdet(0), signdet(1), donedet(false), N1(beta.size()) {}

#undef QRX
#undef APTR

  template <class T> QRPDiv<T>::QRPDiv(const GenMatrix<T>& A,
      bool inplace) :
    pimpl(new QRPDiv_Impl(A,inplace)) 
  {
    if (pimpl->istrans) {
      if (inplace) TMVAssert(A.Transpose() == pimpl->QRx); 
      else pimpl->QRx = A.Transpose();
    }
    else {
      if (inplace) TMVAssert(A == pimpl->QRx); 
      else pimpl->QRx = A;
    }
    QRP_Decompose(pimpl->QRx,pimpl->beta.View(),pimpl->P.get(),pimpl->signdet,
        StrictQRP);
    int sd = pimpl->QRx.diag().step();
    TMVAssert(pimpl->N1>0);
    const T* endofdiag = pimpl->QRx.diag().cptr() + (pimpl->N1-1)*sd;
    while(pimpl->N1>0 && *endofdiag==T(0)) { 
      --pimpl->N1;
      endofdiag -= sd; 
    }
  }

  template <class T> QRPDiv<T>::~QRPDiv() {}

  template <class T> template <class T1> void QRPDiv<T>::DoLDivEq(
      const MatrixView<T1>& m) const
  {
    TMVAssert(pimpl->QRx.IsSquare());
    TMVAssert(m.colsize() == pimpl->QRx.colsize());
    if (pimpl->istrans) 
      QR_LDivEq(pimpl->QRx,pimpl->beta,pimpl->P.get(),m.Transpose(),pimpl->N1);
    else 
      QR_LDivEq(pimpl->QRx,pimpl->beta,pimpl->P.get(),m,pimpl->N1);
  }

  template <class T> template <class T1> void QRPDiv<T>::DoRDivEq(
      const MatrixView<T1>& m) const
  {
    TMVAssert(pimpl->QRx.IsSquare());
    TMVAssert(m.rowsize() == pimpl->QRx.rowsize());
    if (pimpl->istrans) 
      QR_RDivEq(pimpl->QRx,pimpl->beta,pimpl->P.get(),m.Transpose(),pimpl->N1);
    else 
      QR_RDivEq(pimpl->QRx,pimpl->beta,pimpl->P.get(),m,pimpl->N1);
  }

  template <class T> template <class T1, class T2> void QRPDiv<T>::DoLDiv(
      const GenMatrix<T1>& m, const MatrixView<T2>& x) const
  {
    TMVAssert(m.rowsize() == x.rowsize());
    TMVAssert(m.colsize() == (pimpl->istrans ? pimpl->QRx.rowsize() :
          pimpl->QRx.colsize()));
    TMVAssert(x.colsize() == (pimpl->istrans ? pimpl->QRx.colsize() : 
          pimpl->QRx.rowsize()));
    if (pimpl->istrans) 
      QR_RDiv(pimpl->QRx,pimpl->beta,pimpl->P.get(),
          m.Transpose(),x.Transpose(),pimpl->N1);
    else 
      QR_LDiv(pimpl->QRx,pimpl->beta,pimpl->P.get(),m,x,pimpl->N1);
  }

  template <class T> template <class T1, class T2> void QRPDiv<T>::DoRDiv(
      const GenMatrix<T1>& m, const MatrixView<T2>& x) const
  {
    TMVAssert(m.colsize() == x.colsize());
    TMVAssert(m.rowsize() == (pimpl->istrans ? pimpl->QRx.colsize() :
          pimpl->QRx.rowsize()));
    TMVAssert(x.rowsize() == (pimpl->istrans ? pimpl->QRx.rowsize() :
          pimpl->QRx.colsize()));
    if (pimpl->istrans) QR_LDiv(pimpl->QRx,pimpl->beta,pimpl->P.get(),
        m.Transpose(),x.Transpose(),pimpl->N1);
    else QR_RDiv(pimpl->QRx,pimpl->beta,pimpl->P.get(),m,x,pimpl->N1);
  }


  template <class T> T QRPDiv<T>::Det() const
  {
    if (!pimpl->donedet) {
      T s;
      pimpl->logdet = DiagMatrixViewOf(pimpl->QRx.diag()).LogDet(&s);
      pimpl->signdet *= s;
      pimpl->donedet = true;
    }         
    if (pimpl->signdet == T(0)) return T(0);
    else return pimpl->signdet * EXP(pimpl->logdet);  
  }                  

  template <class T> RealType(T) QRPDiv<T>::LogDet(T* sign) const
  {
    if (!pimpl->donedet) {
      T s;
      pimpl->logdet = DiagMatrixViewOf(pimpl->QRx.diag()).LogDet(&s);
      pimpl->signdet *= s;
      pimpl->donedet = true;
    }
    if (sign) *sign = pimpl->signdet;
    return pimpl->logdet;  
  }                  

  template <class T> template <class T1> void QRPDiv<T>::DoInverse(
      const MatrixView<T1>& minv) const
  {
    MatrixView<T1> minv2 = pimpl->istrans ? minv.Transpose() : minv;
    // minv2 = I Pt R^-1 Q^-1
    TMVAssert(minv2.colsize() == pimpl->QRx.rowsize());
    TMVAssert(minv2.rowsize() == pimpl->QRx.colsize());

    const int N = pimpl->QRx.rowsize();
    const int M = pimpl->QRx.colsize();

    minv2.Cols(0,N).SetToIdentity();
    minv2.Cols(0,N).PermuteCols(pimpl->P.get());
    minv2.Cols(pimpl->N1,M).Zero();
    minv2.Cols(0,pimpl->N1) %= 
    pimpl->QRx.UpperTri().SubTriMatrix(0,pimpl->N1);
    Q_RDivEq(pimpl->QRx,pimpl->beta,minv2);
  }

  template <class T> void QRPDiv<T>::DoInverseATA(
      const MatrixView<T>& ata) const
  {
    // At A = Pt Rt R P
    // (At A)^-1 = (Pt Rt R P)^-1 = Pt R^-1 R^-1t P
    UpperTriMatrix<T> rinv = pimpl->QRx.UpperTri().Inverse();
    ata = rinv * rinv.Adjoint();
    ata.ReversePermuteRows(pimpl->P.get());
    ata.ReversePermuteCols(pimpl->P.get());
  }

  template <class T> bool QRPDiv<T>::Singular() const
  { return Det() == T(0); }

  template <class T> bool QRPDiv<T>::IsTrans() const
  { return pimpl->istrans; }

  template <class T> PackedQ<T> QRPDiv<T>::GetQ() const
  { return PackedQ<T>(pimpl->QRx,pimpl->beta); }

  template <class T> ConstUpperTriMatrixView<T> QRPDiv<T>::GetR() const
  { return pimpl->QRx.UpperTri(); }

  template <class T> const GenMatrix<T>& QRPDiv<T>::GetQRx() const 
  { return pimpl->QRx; }

  template <class T> const GenVector<T>& QRPDiv<T>::Getbeta() const 
  { return pimpl->beta; }

  template <class T> const int* QRPDiv<T>::GetP() const 
  { return pimpl->P.get(); }

  template <class T> bool QRPDiv<T>::CheckDecomp(
      const BaseMatrix<T>& m, std::ostream* fout) const
  {
    Matrix<T> mm = m;
    bool printmat = fout && m.colsize() < 100 && m.rowsize() < 100;
    if (printmat) {
      *fout << "QRPDiv:\n";
      *fout << "M = "<<(pimpl->istrans?mm.Transpose():mm.View())<<std::endl;
      *fout << "Q = "<<GetQ()<<std::endl;
      *fout << "R = "<<GetR()<<std::endl;
      *fout << "P = ";
      for(int i=0;i<int(pimpl->QRx.rowsize());i++)
        *fout<<(pimpl->P.get())[i]<<" ";
      *fout<<std::endl;
    }
    Matrix<T> qr = GetQ()*GetR();
    qr.ReversePermuteCols(GetP());
    RealType(T) nm = Norm(qr-(pimpl->istrans ? mm.Transpose() : mm.View()));
    nm /= Norm(GetQ())*Norm(GetR());
    if (printmat) {
      *fout << "QRP = "<<qr<<std::endl;
    }
    if (fout) {
      *fout << "Norm(M-QR)/Norm(QR) = "<<nm<<"  ";
      *fout <<pimpl->QRx.rowsize()*Epsilon<T>()<<std::endl;
    }
    return nm < mm.DoCondition()*Epsilon<T>();
  }

  template <class T> size_t QRPDiv<T>::colsize() const
  { return pimpl->istrans ? pimpl->QRx.rowsize() : pimpl->QRx.colsize(); }

  template <class T> size_t QRPDiv<T>::rowsize() const
  { return pimpl->istrans ? pimpl->QRx.colsize() : pimpl->QRx.rowsize(); }

#define InstFile "TMV_QRPD.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv

