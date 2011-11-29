///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// The Template Matrix/Vector Library for C++ was created by Mike Jarvis     //
// Copyright (C) 2007                                                        //
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
// along with this program in the file gpl.txt.                              //
//                                                                           //
// If not, write to:                                                         //
// The Free Software Foundation, Inc.                                        //
// 51 Franklin Street, Fifth Floor,                                          //
// Boston, MA  02110-1301, USA.                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////



#include "TMV_Matrix.h"
#include "TMV_TriMatrix.h"
#include "TMV_SymMatrix.h"
#include "TMV_SymCHDiv.h"
#include "TMV_SymCHDiv_A.h"
#include "TMV_SymCHDiv_B.h"
#include "TMV_DiagMatrix.h"
#include "TMV_TriMatrixArith.h"
#include "TMV_SymMatrixArith.h"
#include "TMV_MatrixArith.h"
#include <ostream>

namespace tmv {

  template <class T> struct HermCHDiv<T>::HermCHDiv_Impl
  {
    HermCHDiv_Impl(const GenSymMatrix<T>& m, bool inplace);

    const bool inplace;
    auto_array<T> Aptr1;
    T* Aptr;
    SymMatrixView<T> LLx;
    mutable T det;
    mutable bool donedet;
  };

#define APTR1 inplace ? 0 : new T[A.size()*A.size()]
#define APTR inplace ? A.NonConst().ptr() : Aptr1.get()
#define LLX \
  inplace ? A.uplo()==Upper ? A.NonConst().Adjoint() : A.NonConst() : \
  HermMatrixViewOf(Aptr,A.size(),Lower,BaseStorOf(A.LowerTri()))

  template <class T> HermCHDiv<T>::HermCHDiv_Impl::HermCHDiv_Impl(
      const GenSymMatrix<T>& A, bool _inplace) :
    inplace(_inplace && (A.iscm() || A.isrm())), 
    Aptr1(APTR1), Aptr(APTR), LLx(LLX), 
    det(T(0)),donedet(false) {}

#undef APTR
#undef APTR1
#undef LLX

  template <class T> HermCHDiv<T>::HermCHDiv(const GenSymMatrix<T>& A,
      bool inplace) : pimpl(new HermCHDiv_Impl(A,inplace))
  {
#ifdef XTEST
    TMVAssert(A.HermOK());
#endif
    TMVAssert(IsReal(T()) || A.isherm());
    if (inplace) TMVAssert(A==pimpl->LLx); 
    else pimpl->LLx = A;
    try {
      HermCH_Decompose(pimpl->LLx);
    }
    catch (NonPosDef)
    {
      if (inplace) throw NonPosDefHermMatrix<T>(pimpl->LLx);
      else throw NonPosDefHermMatrix2<T>(pimpl->LLx,A);
    }
  }

  template <class T> HermCHDiv<T>::~HermCHDiv() { delete pimpl; }

#define CT std::complex<T>
  template <class T> inline void HermCH_LDivEq(
      const GenSymMatrix<CT>& , const MatrixView<T>& )
  { TMVAssert(FALSE); }
  template <class T> inline void HermCH_RDivEq(
      const GenSymMatrix<CT>& , const MatrixView<T>& )
  { TMVAssert(FALSE); }
  template <class T> inline void HermCH_Inverse(
      const GenSymMatrix<CT>& , const SymMatrixView<T>& )
  { TMVAssert(FALSE); }
#undef CT

  template <class T> template <class T1> void HermCHDiv<T>::DoLDivEq(
      const MatrixView<T1>& m) const
  {
    TMVAssert(pimpl->LLx.size() == m.colsize());
    HermCH_LDivEq(pimpl->LLx,m);
  }

  template <class T> template <class T1> void HermCHDiv<T>::DoRDivEq(
      const MatrixView<T1>& m) const
  {
    TMVAssert(pimpl->LLx.size() == m.rowsize());
    HermCH_RDivEq(pimpl->LLx,m);
  }

  template <class T> template <class T1, class T2> void HermCHDiv<T>::DoLDiv(
      const GenMatrix<T1>& m1, const MatrixView<T2>& m0) const
  {
    TMVAssert(m1.colsize() == m0.colsize());
    TMVAssert(m1.rowsize() == m0.rowsize());
    TMVAssert(pimpl->LLx.size() == m1.colsize());
    HermCH_LDivEq(pimpl->LLx,m0=m1);
  }

  template <class T> template <class T1, class T2> void HermCHDiv<T>::DoRDiv(
      const GenMatrix<T1>& m1, const MatrixView<T2>& m0) const
  {
    TMVAssert(m1.colsize() == m0.colsize());
    TMVAssert(m1.rowsize() == m0.rowsize());
    TMVAssert(pimpl->LLx.size() == m1.rowsize());
    HermCH_RDivEq(pimpl->LLx,m0=m1);
  }

  //
  // LDivEq
  //

  template <class T1, class T2> void HermCH_LDivEq(
      const GenSymMatrix<T1>& LL, const MatrixView<T2>& m)
  {
    TMVAssert(LL.size() == m.colsize());
    // m = (LLt)^-1 m
    //   = Lt^-1 L^-1 m
    m /= LL.LowerTri();
    m /= LL.UpperTri();
  }

  //
  // RDivEq Matrix
  //

  template <class T1, class T2> void HermCH_RDivEq(
      const GenSymMatrix<T1>& LL, const MatrixView<T2>& m)
  {
    TMVAssert(LL.size() == m.rowsize());
    // m = m (LLt)^-1 
    //   = m Lt^-1 L^-1
    m %= LL.UpperTri();
    m %= LL.LowerTri();
  }

  template <class T> T HermCHDiv<T>::Det() const
  {
    if (!pimpl->donedet) {
      pimpl->det = DiagMatrixViewOf(pimpl->LLx.diag()).Det();
      pimpl->det *= pimpl->det;
      pimpl->donedet = true;
    }
    return pimpl->det;
  }

  template <class T> template <class T1> void HermCHDiv<T>::DoInverse(
      const SymMatrixView<T1>& sinv) const
  {
    TMVAssert(sinv.size() == pimpl->LLx.size());
    TMVAssert(sinv.isherm());
    HermCH_Inverse(pimpl->LLx,sinv);
  }

  template <class T> template <class T1> void HermCHDiv<T>::DoInverse(
      const MatrixView<T1>& minv) const
  {
    TMVAssert(minv.colsize() == pimpl->LLx.size());
    TMVAssert(minv.rowsize() == pimpl->LLx.size());

    if (IsComplex(T1())) minv.diag().Imag().Zero();
    Inverse(HermMatrixViewOf(minv,Lower));
    if (minv.colsize() > 1)
      UpperTriMatrixViewOf(minv).OffDiag() =
	LowerTriMatrixViewOf(minv).OffDiag().Adjoint();
  }

  template <class T> void HermCHDiv<T>::DoInverseATA(
      const MatrixView<T>& ata) const
  {
    // ata = (At A)^-1 = A^-1 (A^-1)t
    //     = A^-1 A^-1
    SymMatrixView<T> hermata = HermMatrixViewOf(ata,Lower);
    DoInverse(hermata);
    SymATASquare<true>(ata);
  }

  template <class T> bool HermCHDiv<T>::Singular() const 
  { return Det() == T(0); }

  template <class T> 
    const ConstLowerTriMatrixView<T> HermCHDiv<T>::GetL() const 
    { return pimpl->LLx.LowerTri(); }

  template <class T> const GenSymMatrix<T>& HermCHDiv<T>::GetLL() const 
  { return pimpl->LLx; }

  template <class T> std::string HermCHDiv<T>::Type() const
  { return std::string("HermCHDiv<") + tmv::Type(T()) + ">"; }

  template <class T> DivType HermCHDiv<T>::GetDivType() const 
  { return CH; }


  template <class T> bool HermCHDiv<T>::CheckDecomp(
      const BaseMatrix<T>& m, std::ostream* fout) const
  {
    Matrix<T> mm = m;
    if (fout) {
      *fout << "HermCHDiv:\n";
      *fout << "M = "<<mm<<std::endl;
      *fout << "L = "<<GetL()<<std::endl;
    }
    Matrix<T> lu = GetL()*GetL().Adjoint();
    RealType(T) nm = Norm(lu-mm);
    nm /= SQR(Norm(GetL()));
    if (fout) {
      *fout << "LLt = "<<lu<<std::endl;
      *fout << "Norm(M-LLt)/Norm(LLt) = "<<nm<<std::endl;
    }
    mm.DivideUsing(SVS);
    mm.SetDiv();
    return nm < mm.Condition()*mm.colsize()*Epsilon<T>();
  }

  template <class T> size_t HermCHDiv<T>::colsize() const
  { return pimpl->LLx.size(); }

  template <class T> size_t HermCHDiv<T>::rowsize() const
  { return pimpl->LLx.size(); }

#define InstFile "TMV_SymCHDiv.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace mv

