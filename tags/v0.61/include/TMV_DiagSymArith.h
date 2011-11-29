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


#ifndef TMV_DiagSymArith_H
#define TMV_DiagSymArith_H

#define CT std::complex<T>

namespace tmv {

  //
  // DiagMatrix + SymMatrix
  //

  template <class T, class T1, class T2> class SumDS : 
    public SymMatrixComposite<T> 
  {
    public:
      inline SumDS(T _x1, const GenDiagMatrix<T1>& _m1, 
	  T _x2, const GenSymMatrix<T2>& _m2) :
	x1(_x1),m1(_m1),x2(_x2),m2(_m2)
      { TMVAssert(m1.size() == m2.size()); }
      inline size_t size() const { return m1.size(); }
      inline SymType sym() const { return m2.sym(); }
      inline StorageType stor() const { return BaseStorOf(m2); }
      inline T GetX1() const { return x1; }
      inline const GenDiagMatrix<T1>& GetM1() const { return m1; }
      inline T GetX2() const { return x2; }
      inline const GenSymMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToM(const MatrixView<RealType(T)>& m0) const
      { 
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == size());
	TMVAssert(m0.rowsize() == size());
	if (SameStorage(m0,m1.diag())) {
	  DiagMatrix<T1> m1x = m1;
	  MultXM(x2,m0=m2);
	  AddVV(x1,m1x.diag(),m0.diag());
	} else {
	  MultXM(x2,m0=m2);
	  AddVV(x1,m1.diag(),m0.diag());
	}
      }
      inline void AssignToM(const MatrixView<ComplexType(T)>& m0) const
      { 
	TMVAssert(m0.colsize() == size());
	TMVAssert(m0.rowsize() == size());
	if (SameStorage(m0,m1.diag())) {
	  DiagMatrix<T1> m1x = m1;
	  MultXM(x2,m0=m2);
	  AddVV(x1,m1.diag(),m0.diag());
	} else {
	  MultXM(x2,m0=m2);
	  AddVV(x1,m1.diag(),m0.diag());
	}
      }
      inline void AssignToS(const SymMatrixView<RealType(T)>& m0) const
      { 
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == size());
	TMVAssert(m0.rowsize() == size());
	if (SameStorage(m0,m1.diag())) {
	  DiagMatrix<T1> m1x = m1;
	  MultXM(x2,m0=m2);
	  AddVV(x1,m1x.diag(),m0.diag());
	} else {
	  MultXM(x2,m0=m2);
	  AddVV(x1,m1.diag(),m0.diag());
	}
      }
      inline void AssignToS(const SymMatrixView<ComplexType(T)>& m0) const
      { 
	TMVAssert(m0.colsize() == size());
	TMVAssert(m0.rowsize() == size());
	if (!m2.issym()) {
	  TMVAssert(IsReal(T1()) && IMAG(x1) == T1(0) && IMAG(x2) == T1(0));
	}
	if (SameStorage(m0,m1.diag())) {
	  DiagMatrix<T1> m1x = m1;
	  MultXM(x2,m0=m2);
	  AddVV(x1,m1x.diag(),m0.diag());
	} else {
	  MultXM(x2,m0=m2);
	  AddVV(x1,m1.diag(),m0.diag());
	}
      }
    private:
      const T x1;
      const GenDiagMatrix<T1>& m1;
      const T x2;
      const GenSymMatrix<T2>& m2;
  };

  template <class T> inline const SymMatrixView<T>& operator+=(
      const SymMatrixView<T>& m1, const GenDiagMatrix<T>& m2) 
  {
    TMVAssert(m1.size() == m2.size());
    TMVAssert(m1.issym());
    AddVV(T(1),m2.diag(),m1.diag());
    return m1; 
  }

  template <class T> inline const SymMatrixView<CT>& operator+=(
      const SymMatrixView<CT>& m1, const GenDiagMatrix<T>& m2) 
  { 
    TMVAssert(m1.size() == m2.size());
    AddVV(T(1),m2.diag(),m1.diag());
    return m1; 
  }

  template <class T> inline const SymMatrixView<T>& operator-=(
      const SymMatrixView<T>& m1, const GenDiagMatrix<T>& m2) 
  { 
    TMVAssert(m1.size() == m2.size());
    TMVAssert(m1.issym());
    AddVV(T(-1),m2.diag(),m1.diag());
    return m1; 
  }

  template <class T> inline const SymMatrixView<CT>& operator-=(
      const SymMatrixView<CT>& m1, const GenDiagMatrix<T>& m2) 
  { 
    TMVAssert(m1.size() == m2.size());
    AddVV(T(-1),m2.diag(),m1.diag());
    return m1; 
  }

  template <class T, class T2> inline const SymMatrixView<T>& operator+=(
      const SymMatrixView<T>& m, const ProdXD<T,T2>& pxm)
  {
    TMVAssert(m.size() == pxm.size());
    TMVAssert(m.issym());
    AddVV(pxm.GetX(),pxm.GetM().diag(),m.diag());
    return m;
  }

  template <class T> inline const SymMatrixView<CT>& operator+=(
      const SymMatrixView<CT>& m, const ProdXD<T,T>& pxm)
  {
    TMVAssert(m.size() == pxm.size());
    AddVV(pxm.GetX(),pxm.GetM().diag(),m.diag());
    return m;
  }

  template <class T, class T2> inline const SymMatrixView<T>& operator-=(
      const SymMatrixView<T>& m, const ProdXD<T,T2>& pxm)
  {
    TMVAssert(m.size() == pxm.size());
    TMVAssert(m.issym());
    AddVV(-pxm.GetX(),pxm.GetM().diag(),m.diag());
    return m;
  }

  template <class T> inline const SymMatrixView<CT>& operator-=(
      const SymMatrixView<CT>& m, const ProdXD<T,T>& pxm)
  {
    TMVAssert(m.size() == pxm.size());
    AddVV(-pxm.GetX(),pxm.GetM().diag(),m.diag());
    return m;
  }

#define SUMMM SumDS
#define GENMATRIX1 GenDiagMatrix
#define GENMATRIX2 GenSymMatrix
#define PRODXM1 ProdXD
#define PRODXM2 ProdXS
#include "TMV_AuxSumMM.h"
#include "TMV_AuxSumMMb.h"
#undef SUMMM
#undef GENMATRIX1
#undef GENMATRIX2
#undef PRODXM1
#undef PRODXM2

  //
  // DiagMatrix * SymMatrix
  //

  template <class T, class T1, class T2> class ProdDS : 
    public MatrixComposite<T>
  {
    public:
      inline ProdDS(T _x, const GenDiagMatrix<T1>& _m1,
	  const GenSymMatrix<T2>& _m2) :
	x(_x), m1(_m1), m2(_m2)
      { TMVAssert(m1.size() == m2.size()); }
      inline size_t colsize() const { return m2.size(); }
      inline size_t rowsize() const { return m2.size(); }
      inline StorageType stor() const { return BaseStorOf(m2); }
      inline T GetX() const { return x; }
      inline const GenDiagMatrix<T1>& GetM1() const { return m1; }
      inline const GenSymMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToM(const MatrixView<RealType(T)>& m0) const
      {
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	if (SameStorage(m0,m1.diag())) {
	  DiagMatrix<T1> m1x = m1;
	  MultXM(x,m0=m2);
	  MultMM<false>(T(1),m1x,m0,m0);
	} else {
	  MultXM(x,m0=m2);
	  MultMM<false>(T(1),m1,m0,m0);
	}
      }
      inline void AssignToM(const MatrixView<ComplexType(T)>& m0) const
      {
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	if (SameStorage(m0,m1.diag())) {
	  DiagMatrix<T1> m1x = m1;
	  MultXM(x,m0=m2);
	  MultMM<false>(T(1),m1x,m0,m0);
	} else {
	  MultXM(x,m0=m2);
	  MultMM<false>(T(1),m1,m0,m0);
	}
      }
    protected:
      T x;
      const GenDiagMatrix<T1>& m1;
      const GenSymMatrix<T2>& m2;
  };

  template <class T, class T1, class T2> class ProdSD : 
    public MatrixComposite<T>
  {
    public:
      inline ProdSD(T _x, const GenSymMatrix<T1>& _m1,
	  const GenDiagMatrix<T2>& _m2) :
	x(_x), m1(_m1), m2(_m2)
      { TMVAssert(m1.size() == m2.size()); }
      inline size_t colsize() const { return m1.size(); }
      inline size_t rowsize() const { return m1.size(); }
      inline StorageType stor() const { return BaseStorOf(m1); }
      inline T GetX() const { return x; }
      inline const GenSymMatrix<T1>& GetM1() const { return m1; }
      inline const GenDiagMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToM(const MatrixView<RealType(T)>& m0) const
      {
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	if (SameStorage(m0,m2.diag())) {
	  DiagMatrix<T2> m2x = m2;
	  MultXM(x,m0=m1);
	  MultMM<false>(T(1),m0,m2x,m0);
	} else {
	  MultXM(x,m0=m1);
	  MultMM<false>(T(1),m0,m2,m0);
	}
      }
      inline void AssignToM(const MatrixView<ComplexType(T)>& m0) const
      {
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	if (SameStorage(m0,m2.diag())) {
	  DiagMatrix<T2> m2x = m2;
	  MultXM(x,m0=m1);
	  MultMM<false>(T(1),m0,m2x,m0);
	} else {
	  MultXM(x,m0=m1);
	  MultMM<false>(T(1),m0,m2,m0);
	}
      }
    protected:
      T x;
      const GenSymMatrix<T1>& m1;
      const GenDiagMatrix<T2>& m2;
  };

#define PRODMM ProdSD
#define GENMATRIX1 GenSymMatrix
#define GENMATRIX2 GenDiagMatrix
#define PRODXM1 ProdXS
#define PRODXM2 ProdXD
#include "TMV_AuxProdMM.h"
#include "TMV_AuxProdMMa.h"
#undef PRODMM
#undef GENMATRIX1
#undef GENMATRIX2
#undef PRODXM1
#undef PRODXM2
  
#define PRODMM ProdDS
#define GENMATRIX1 GenDiagMatrix
#define GENMATRIX2 GenSymMatrix
#define PRODXM1 ProdXD
#define PRODXM2 ProdXS
#include "TMV_AuxProdMM.h"
#include "TMV_AuxProdMMa.h"
#undef PRODMM
#undef GENMATRIX1
#undef GENMATRIX2
#undef PRODXM1
#undef PRODXM2
  
  //
  // SymMatrix / % DiagMatrix
  //

  template <class T, class T1, class T2> class QuotSD : 
    public MatrixComposite<T>
  {
    public:
      inline QuotSD(const T _x, const GenSymMatrix<T1>& _m1,
	  const GenDiagMatrix<T2>& _m2) :
	x(_x), m1(_m1), m2(_m2)
      { TMVAssert( m1.size() == m2.size() ); }
      inline size_t colsize() const { return m1.colsize(); }
      inline size_t rowsize() const { return m1.rowsize(); }
      inline StorageType stor() const { return BaseStorOf(m1); }
      inline T GetX() const { return x; }
      inline const GenSymMatrix<T1>& GetM1() const { return m1; }
      inline const GenDiagMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToM(const MatrixView<RealType(T)>& m0) const
      {
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	MultXM(x,m0=m1);
	m2.LDivEq(m0);
      }
      inline void AssignToM(const MatrixView<ComplexType(T)>& m0) const
      {
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	MultXM(x,m0=m1);
	m2.LDivEq(m0);
      }
    protected:
      const T x;
      const GenSymMatrix<T1>& m1;
      const GenDiagMatrix<T2>& m2;
  };

  template <class T, class T1, class T2> class RQuotSD : 
    public MatrixComposite<T>
  {
    public:
      inline RQuotSD(const T _x, const GenSymMatrix<T1>& _m1,
	  const GenDiagMatrix<T2>& _m2) :
	x(_x), m1(_m1), m2(_m2)
      { TMVAssert( m1.size() == m2.size() ); }
      inline size_t colsize() const { return m1.size(); }
      inline size_t rowsize() const { return m1.size(); }
      inline StorageType stor() const { return BaseStorOf(m1); }
      inline T GetX() const { return x; }
      inline const GenSymMatrix<T1>& GetM1() const { return m1; }
      inline const GenDiagMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToM(const MatrixView<RealType(T)>& m0) const
      {
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	MultXM(x,m0=m1);
	m2.RDivEq(m0);
      }
      inline void AssignToM(const MatrixView<ComplexType(T)>& m0) const
      {
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	MultXM(x,m0=m1);
	m2.RDivEq(m0);
      }
    protected:
      const T x;
      const GenSymMatrix<T1>& m1;
      const GenDiagMatrix<T2>& m2;
  };

#define QUOTMM QuotSD
#define QUOTXM QuotXD
#define RQUOTMM RQuotSD
#define GENMATRIX1 GenSymMatrix
#define GENMATRIX2 GenDiagMatrix
#define PRODXM1 ProdXS
#define PRODXM2 ProdXD
#include "TMV_AuxQuotMM.h"
#include "TMV_AuxQuotMMa.h"
#undef QUOTMM
#undef QUOTXM
#undef RQUOTMM
#undef GENMATRIX1
#undef GENMATRIX2
#undef PRODXM1
#undef PRODXM2
    
#define GENMATRIX1 GenDiagMatrix
#define GENMATRIX2 GenSymMatrix
#define PRODXM1 ProdXD
#define PRODXM2 ProdXS
#define QUOTXM QuotXS
#define TQUOTMM TransientQuotMS
#define TRQUOTMM TransientRQuotMS
#include "TMV_AuxTQuotMM.h"
#undef GENMATRIX1
#undef GENMATRIX2
#undef PRODXM1
#undef PRODXM2
#undef QUOTXM
#undef TQUOTMM
#undef TRQUOTMM

} // namespace tmv

#undef CT

#endif