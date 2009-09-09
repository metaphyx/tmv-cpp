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


#ifndef TMV_TriSymBandArith_H
#define TMV_TriSymBandArith_H

#include "TMV_TriBandArithFunc.h"

#define CT std::complex<T>

namespace tmv {

  //
  // TriMatrix + SymBandMatrix
  //

  template <class T, class T1, class T2> class SumUsB : 
    public BandMatrixComposite<T> 
  {
    public:
      inline SumUsB(T _x1, const GenUpperTriMatrix<T1>& _m1, 
	  T _x2, const GenSymBandMatrix<T2>& _m2) :
	x1(_x1),m1(_m1),x2(_x2),m2(_m2)
      { TMVAssert(m1.size() == m2.size()); }
      inline size_t colsize() const { return m1.size(); }
      inline size_t rowsize() const { return m1.size(); }
      inline int nlo() const { return m2.nlo(); }
      inline int nhi() const { return m1.size()-1; }
      inline StorageType stor() const { return BaseStorOf(m2); }
      inline T GetX1() const { return x1; }
      inline const GenUpperTriMatrix<T1>& GetM1() const { return m1; }
      inline T GetX2() const { return x2; }
      inline const GenSymBandMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToB(const BandMatrixView<RealType(T)>& m0) const
      { 
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nhi() >= nhi());
	TMVAssert(m0.nlo() >= nlo());
	if (SameStorage(m0,m2)) {
	  BandMatrix<T2> m2x = m2;
	  MultXM(x1,m0=m1);
	  AddMM(x2,m2x,m0);
	} else {
	  MultXM(x1,m0=m1);
	  AddMM(x2,m2,m0);
	}
      }
      inline void AssignToB(const BandMatrixView<ComplexType(T)>& m0) const
      { 
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nhi() >= nhi());
	TMVAssert(m0.nlo() >= nlo());
	if (SameStorage(m0,m2)) {
	  BandMatrix<T2> m2x = m2;
	  MultXM(x1,m0=m1);
	  AddMM(x2,m2x,m0);
	} else {
	  MultXM(x1,m0=m1);
	  AddMM(x2,m2,m0);
	}
      }
    private:
      const T x1;
      const GenUpperTriMatrix<T1>& m1;
      const T x2;
      const GenSymBandMatrix<T2>& m2;
  };

  template <class T, class T1, class T2> class SumLsB : 
    public BandMatrixComposite<T> 
  {
    public:
      inline SumLsB(T _x1, const GenLowerTriMatrix<T1>& _m1, 
	  T _x2, const GenSymBandMatrix<T2>& _m2) :
	x1(_x1),m1(_m1),x2(_x2),m2(_m2)
      { TMVAssert(m1.size() == m2.size()); }
      inline size_t colsize() const { return m1.size(); }
      inline size_t rowsize() const { return m1.size(); }
      inline int nlo() const { return m1.size()-1; }
      inline int nhi() const { return m2.nhi(); }
      inline StorageType stor() const { return BaseStorOf(m2); }
      inline T GetX1() const { return x1; }
      inline const GenLowerTriMatrix<T1>& GetM1() const { return m1; }
      inline T GetX2() const { return x2; }
      inline const GenSymBandMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToB(const BandMatrixView<RealType(T)>& m0) const
      { 
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nhi() >= nhi());
	TMVAssert(m0.nlo() >= nlo());
	if (SameStorage(m0,m2)) {
	  BandMatrix<T2> m2x = m2;
	  MultXM(x1,m0=m1);
	  AddMM(x2,m2x,m0);
	} else {
	  MultXM(x1,m0=m1);
	  AddMM(x2,m2,m0);
	}
      }
      inline void AssignToB(const BandMatrixView<ComplexType(T)>& m0) const
      { 
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nhi() >= nhi());
	TMVAssert(m0.nlo() >= nlo());
	if (SameStorage(m0,m2)) {
	  BandMatrix<T2> m2x = m2;
	  MultXM(x1,m0=m1);
	  AddMM(x2,m2x,m0);
	} else {
	  MultXM(x1,m0=m1);
	  AddMM(x2,m2,m0);
	}
      }
    private:
      const T x1;
      const GenLowerTriMatrix<T1>& m1;
      const T x2;
      const GenSymBandMatrix<T2>& m2;
  };


#define SUMMM SumUsB
#define GENMATRIX1 GenUpperTriMatrix
#define GENMATRIX2 GenSymBandMatrix
#define PRODXM1 ProdXU
#define PRODXM2 ProdXsB
#include "TMV_AuxSumMM.h"
#include "TMV_AuxSumMMb.h"
#undef SUMMM
#undef GENMATRIX1
#undef PRODXM1
#define SUMMM SumLsB
#define GENMATRIX1 GenLowerTriMatrix
#define PRODXM1 ProdXL
#include "TMV_AuxSumMM.h"
#include "TMV_AuxSumMMb.h"
#undef SUMMM
#undef GENMATRIX1
#undef GENMATRIX2
#undef PRODXM1
#undef PRODXM2


  //
  // TriMatrix * SymBandMatrix
  //

  template <class T, class T1, class T2> class ProdUsB : 
    public BandMatrixComposite<T>
  {
    public:
      inline ProdUsB(T _x, const GenUpperTriMatrix<T1>& _m1,
	  const GenSymBandMatrix<T2>& _m2) :
	x(_x), m1(_m1), m2(_m2)
      { TMVAssert(m1.size() == m2.size()); }
      inline size_t colsize() const { return m2.size(); }
      inline size_t rowsize() const { return m2.size(); }
      inline int nlo() const { return m2.nlo(); }
      inline int nhi() const { return int(m2.size())-1; }
      inline StorageType stor() const { return BaseStorOf(m2); }
      inline T GetX() const { return x; }
      inline const GenUpperTriMatrix<T1>& GetM1() const { return m1; }
      inline const GenSymBandMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToB(const BandMatrixView<RealType(T)>& m0) const
      {
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nhi() >= nhi());
	TMVAssert(m0.nlo() >= nlo());
	if (SameStorage(m0,m1)) {
	  BandMatrix<T1> m1x = m1;
	  MultXM(x,m0=m2);
	  MultMM<false>(T(1),m1x,m0,m0);
	} else {
	  MultXM(x,m0=m2);
	  MultMM<false>(T(1),m1,m0,m0);
	}
      }
      inline void AssignToB(const BandMatrixView<ComplexType(T)>& m0) const
      {
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nhi() >= nhi());
	TMVAssert(m0.nlo() >= nlo());
	if (SameStorage(m0,m1)) {
	  BandMatrix<T1> m1x = m1;
	  MultXM(x,m0=m2);
	  MultMM<false>(T(1),m1x,m0,m0);
	} else {
	  MultXM(x,m0=m2);
	  MultMM<false>(T(1),m1,m0,m0);
	}
      }
    protected:
      T x;
      const GenUpperTriMatrix<T1>& m1;
      const GenSymBandMatrix<T2>& m2;
  };

  template <class T, class T1, class T2> class ProdsBU : 
    public BandMatrixComposite<T>
  {
    public:
      inline ProdsBU(T _x, const GenSymBandMatrix<T1>& _m1,
	  const GenUpperTriMatrix<T2>& _m2) :
	x(_x), m1(_m1), m2(_m2)
      { TMVAssert(m1.size() == m2.size()); }
      inline size_t colsize() const { return m1.size(); }
      inline size_t rowsize() const { return m1.size(); }
      inline int nlo() const { return m1.nlo(); }
      inline int nhi() const { return int(m1.size())-1; }
      inline StorageType stor() const { return BaseStorOf(m1); }
      inline T GetX() const { return x; }
      inline const GenSymBandMatrix<T1>& GetM1() const { return m1; }
      inline const GenUpperTriMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToB(const BandMatrixView<RealType(T)>& m0) const
      {
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nhi() >= nhi());
	TMVAssert(m0.nlo() >= nlo());
	if (SameStorage(m0,m2)) {
	  BandMatrix<T2> m2x = m2;
	  MultXM(x,m0=m1);
	  MultMM<false>(T(1),m0,m2x,m0);
	} else {
	  MultXM(x,m0=m1);
	  MultMM<false>(T(1),m0,m2,m0);
	}
      }
      inline void AssignToB(const BandMatrixView<ComplexType(T)>& m0) const
      {
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nhi() >= nhi());
	TMVAssert(m0.nlo() >= nlo());
	if (SameStorage(m0,m2)) {
	  BandMatrix<T2> m2x = m2;
	  MultXM(x,m0=m1);
	  MultMM<false>(T(1),m0,m2x,m0);
	} else {
	  MultXM(x,m0=m1);
	  MultMM<false>(T(1),m0,m2,m0);
	}
      }
    protected:
      T x;
      const GenSymBandMatrix<T1>& m1;
      const GenUpperTriMatrix<T2>& m2;
  };

  template <class T, class T1, class T2> class ProdLsB : 
    public BandMatrixComposite<T>
  {
    public:
      inline ProdLsB(T _x, const GenLowerTriMatrix<T1>& _m1,
	  const GenSymBandMatrix<T2>& _m2) :
	x(_x), m1(_m1), m2(_m2)
      { TMVAssert(m1.size() == m2.size()); }
      inline size_t colsize() const { return m2.size(); }
      inline size_t rowsize() const { return m2.size(); }
      inline int nlo() const { return int(m2.size())-1; }
      inline int nhi() const { return m2.nhi(); }
      inline StorageType stor() const { return BaseStorOf(m2); }
      inline T GetX() const { return x; }
      inline const GenLowerTriMatrix<T1>& GetM1() const { return m1; }
      inline const GenSymBandMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToB(const BandMatrixView<RealType(T)>& m0) const
      {
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nhi() >= nhi());
	TMVAssert(m0.nlo() >= nlo());
	if (SameStorage(m0,m1)) {
	  BandMatrix<T1> m1x = m1;
	  MultXM(x,m0=m2);
	  MultMM<false>(T(1),m1x,m0,m0);
	} else {
	  MultXM(x,m0=m2);
	  MultMM<false>(T(1),m1,m0,m0);
	}
      }
      inline void AssignToB(const BandMatrixView<ComplexType(T)>& m0) const
      {
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nhi() >= nhi());
	TMVAssert(m0.nlo() >= nlo());
	if (SameStorage(m0,m1)) {
	  BandMatrix<T1> m1x = m1;
	  MultXM(x,m0=m2);
	  MultMM<false>(T(1),m1x,m0,m0);
	} else {
	  MultXM(x,m0=m2);
	  MultMM<false>(T(1),m1,m0,m0);
	}
      }
    protected:
      T x;
      const GenLowerTriMatrix<T1>& m1;
      const GenSymBandMatrix<T2>& m2;
  };

  template <class T, class T1, class T2> class ProdsBL : 
    public BandMatrixComposite<T>
  {
    public:
      inline ProdsBL(T _x, const GenSymBandMatrix<T1>& _m1,
	  const GenLowerTriMatrix<T2>& _m2) :
	x(_x), m1(_m1), m2(_m2)
      { TMVAssert(m1.size() == m2.size()); }
      inline size_t colsize() const { return m1.size(); }
      inline size_t rowsize() const { return m1.size(); }
      inline int nlo() const { return int(m1.size())-1; }
      inline int nhi() const { return m1.nhi(); }
      inline StorageType stor() const { return BaseStorOf(m1); }
      inline T GetX() const { return x; }
      inline const GenSymBandMatrix<T1>& GetM1() const { return m1; }
      inline const GenLowerTriMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToB(const BandMatrixView<RealType(T)>& m0) const
      {
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nhi() >= nhi());
	TMVAssert(m0.nlo() >= nlo());
	if (SameStorage(m0,m2)) {
	  BandMatrix<T2> m2x = m2;
	  MultXM(x,m0=m1);
	  MultMM<false>(T(1),m0,m2x,m0);
	} else {
	  MultXM(x,m0=m1);
	  MultMM<false>(T(1),m0,m2,m0);
	}
      }
      inline void AssignToB(const BandMatrixView<ComplexType(T)>& m0) const
      {
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nhi() >= nhi());
	TMVAssert(m0.nlo() >= nlo());
	if (SameStorage(m0,m2)) {
	  BandMatrix<T2> m2x = m2;
	  MultXM(x,m0=m1);
	  MultMM<false>(T(1),m0,m2x,m0);
	} else {
	  MultXM(x,m0=m1);
	  MultMM<false>(T(1),m0,m2,m0);
	}
      }
    protected:
      T x;
      const GenSymBandMatrix<T1>& m1;
      const GenLowerTriMatrix<T2>& m2;
  };


#define PRODMM ProdsBU
#define GENMATRIX1 GenSymBandMatrix
#define GENMATRIX2 GenUpperTriMatrix
#define PRODXM1 ProdXsB
#define PRODXM2 ProdXU
#include "TMV_AuxProdMM.h"
#include "TMV_AuxProdMMa.h"
#undef PRODMM
#undef GENMATRIX2
#undef PRODXM2
#define PRODMM ProdsBL
#define GENMATRIX2 GenLowerTriMatrix
#define PRODXM2 ProdXL
#include "TMV_AuxProdMM.h"
#include "TMV_AuxProdMMa.h"
#undef PRODMM
#undef GENMATRIX1
#undef GENMATRIX2
#undef PRODXM1
#undef PRODXM2
  
#define PRODMM ProdUsB
#define GENMATRIX1 GenUpperTriMatrix
#define GENMATRIX2 GenSymBandMatrix
#define PRODXM1 ProdXU
#define PRODXM2 ProdXsB
#include "TMV_AuxProdMM.h"
#include "TMV_AuxProdMMa.h"
#undef PRODMM
#undef GENMATRIX1
#undef PRODXM1
#define PRODMM ProdLsB
#define GENMATRIX1 GenLowerTriMatrix
#define PRODXM1 ProdXL
#include "TMV_AuxProdMM.h"
#include "TMV_AuxProdMMa.h"
#undef PRODMM
#undef GENMATRIX1
#undef GENMATRIX2
#undef PRODXM1
#undef PRODXM2
  
  
  //
  // SymBandMatrix / % TriMatrix
  //

  template <class T, class T1, class T2> class QuotsBU : 
    public BandMatrixComposite<T>
  {
    public:
      inline QuotsBU(const T _x, const GenSymBandMatrix<T1>& _m1,
	  const GenUpperTriMatrix<T2>& _m2) :
	x(_x), m1(_m1), m2(_m2)
      { TMVAssert( m1.size() == m2.size() ); }
      inline size_t colsize() const { return m1.colsize(); }
      inline size_t rowsize() const { return m1.rowsize(); }
      inline int nlo() const { return m1.nlo(); }
      inline int nhi() const { return m2.size()-1; }
      inline StorageType stor() const { return BaseStorOf(m1); }
      inline T GetX() const { return x; }
      inline const GenSymBandMatrix<T1>& GetM1() const { return m1; }
      inline const GenUpperTriMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToB(const BandMatrixView<RealType(T)>& m0) const
      {
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nlo() >= nlo());
	TMVAssert(m0.nhi() >= nhi());
	MultXM(x,m0=m1);
	MultMM<false>(T(1),BandMatrixViewOf(UpperTriMatrix<T2>(m2.Inverse())),
	      m0,m0);
      }
      inline void AssignToB(const BandMatrixView<ComplexType(T)>& m0) const
      {
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nlo() >= nlo());
	TMVAssert(m0.nhi() >= nhi());
	MultXM(x,m0=m1);
	MultMM<false>(T(1),BandMatrixViewOf(UpperTriMatrix<T2>(m2.Inverse())),
	      m0,m0);
      }
    protected:
      const T x;
      const GenSymBandMatrix<T1>& m1;
      const GenUpperTriMatrix<T2>& m2;
  };

  template <class T, class T1, class T2> class RQuotsBU : 
    public BandMatrixComposite<T>
  {
    public:
      inline RQuotsBU(const T _x, const GenSymBandMatrix<T1>& _m1,
	  const GenUpperTriMatrix<T2>& _m2) :
	x(_x), m1(_m1), m2(_m2)
      { TMVAssert( m1.rowsize() == m2.size() ); }
      inline size_t colsize() const { return m1.colsize(); }
      inline size_t rowsize() const { return m1.rowsize(); }
      inline int nlo() const { return m1.nlo(); }
      inline int nhi() const { return m2.size()-1; }
      inline StorageType stor() const { return BaseStorOf(m1); }
      inline T GetX() const { return x; }
      inline const GenSymBandMatrix<T1>& GetM1() const { return m1; }
      inline const GenUpperTriMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToB(const BandMatrixView<RealType(T)>& m0) const
      {
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nlo() >= nlo());
	TMVAssert(m0.nhi() >= nhi());
	MultXM(x,m0=m1);
	MultMM<false>(T(1),m0,
	    BandMatrixViewOf(UpperTriMatrix<T2>(m2.Inverse())),m0);
      }
      inline void AssignToB(const BandMatrixView<ComplexType(T)>& m0) const
      {
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nlo() >= nlo());
	TMVAssert(m0.nhi() >= nhi());
	MultXM(x,m0=m1);
	MultMM<false>(T(1),m0,
	    BandMatrixViewOf(UpperTriMatrix<T2>(m2.Inverse())),m0);
      }
    protected:
      const T x;
      const GenSymBandMatrix<T1>& m1;
      const GenUpperTriMatrix<T2>& m2;
  };

  template <class T, class T1, class T2> class QuotsBL : 
    public BandMatrixComposite<T>
  {
    public:
      inline QuotsBL(const T _x, const GenSymBandMatrix<T1>& _m1,
	  const GenLowerTriMatrix<T2>& _m2) :
	x(_x), m1(_m1), m2(_m2)
      { TMVAssert( m1.size() == m2.size() ); }
      inline size_t colsize() const { return m1.size(); }
      inline size_t rowsize() const { return m1.size(); }
      inline int nlo() const { return m2.size()-1; }
      inline int nhi() const { return m1.nhi(); }
      inline StorageType stor() const { return BaseStorOf(m1); }
      inline T GetX() const { return x; }
      inline const GenSymBandMatrix<T1>& GetM1() const { return m1; }
      inline const GenLowerTriMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToB(const BandMatrixView<RealType(T)>& m0) const
      {
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nlo() >= nlo());
	TMVAssert(m0.nhi() >= nhi());
	MultXM(x,m0=m1);
	MultMM<false>(T(1),BandMatrixViewOf(LowerTriMatrix<T2>(m2.Inverse())),
	      m0,m0);
      }
      inline void AssignToB(const BandMatrixView<ComplexType(T)>& m0) const
      {
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nlo() >= nlo());
	TMVAssert(m0.nhi() >= nhi());
	MultXM(x,m0=m1);
	MultMM<false>(T(1),BandMatrixViewOf(LowerTriMatrix<T2>(m2.Inverse())),
	      m0,m0);
      }
    protected:
      const T x;
      const GenSymBandMatrix<T1>& m1;
      const GenLowerTriMatrix<T2>& m2;
  };

  template <class T, class T1, class T2> class RQuotsBL : 
    public BandMatrixComposite<T>
  {
    public:
      inline RQuotsBL(const T _x, const GenSymBandMatrix<T1>& _m1,
	  const GenLowerTriMatrix<T2>& _m2) :
	x(_x), m1(_m1), m2(_m2)
      { TMVAssert( m1.rowsize() == m2.size() ); }
      inline size_t colsize() const { return m1.colsize(); }
      inline size_t rowsize() const { return m1.rowsize(); }
      inline int nlo() const { return m2.size()-1; }
      inline int nhi() const { return m1.nhi(); }
      inline StorageType stor() const { return BaseStorOf(m1); }
      inline T GetX() const { return x; }
      inline const GenSymBandMatrix<T1>& GetM1() const { return m1; }
      inline const GenLowerTriMatrix<T2>& GetM2() const { return m2; }
      inline void AssignToB(const BandMatrixView<RealType(T)>& m0) const
      {
	TMVAssert(IsReal(T()));
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nlo() >= nlo());
	TMVAssert(m0.nhi() >= nhi());
	MultXM(x,m0=m1);
	MultMM<false>(T(1),m0,
	    BandMatrixViewOf(LowerTriMatrix<T2>(m2.Inverse())),m0);
      }
      inline void AssignToB(const BandMatrixView<ComplexType(T)>& m0) const
      {
	TMVAssert(m0.colsize() == colsize());
	TMVAssert(m0.rowsize() == rowsize());
	TMVAssert(m0.nlo() >= nlo());
	TMVAssert(m0.nhi() >= nhi());
	MultXM(x,m0=m1);
	MultMM<false>(T(1),m0,
	    BandMatrixViewOf(LowerTriMatrix<T2>(m2.Inverse())),m0);
      }
    protected:
      const T x;
      const GenSymBandMatrix<T1>& m1;
      const GenLowerTriMatrix<T2>& m2;
  };

#define QUOTMM QuotsBL
#define QUOTXM QuotXL
#define RQUOTMM RQuotsBL
#define GENMATRIX1 GenSymBandMatrix
#define GENMATRIX2 GenLowerTriMatrix
#define PRODXM1 ProdXsB
#define PRODXM2 ProdXL
#include "TMV_AuxQuotMM.h"
#include "TMV_AuxQuotMMa.h"
#undef QUOTMM
#undef QUOTXM
#undef RQUOTMM
#undef GENMATRIX2
#undef PRODXM2
#define QUOTMM QuotsBU
#define QUOTXM QuotXU
#define RQUOTMM RQuotsBU
#define GENMATRIX2 GenUpperTriMatrix
#define PRODXM2 ProdXU
#include "TMV_AuxQuotMM.h"
#include "TMV_AuxQuotMMa.h"
#undef QUOTMM
#undef QUOTXM
#undef RQUOTMM
#undef GENMATRIX1
#undef GENMATRIX2
#undef PRODXM1
#undef PRODXM2
    
#define GENMATRIX1 GenUpperTriMatrix
#define GENMATRIX2 GenSymBandMatrix
#define PRODXM1 ProdXU
#define PRODXM2 ProdXsB
#define QUOTXM QuotXsB
#define TQUOTMM TransientQuotMsB
#define TRQUOTMM TransientRQuotMsB
#include "TMV_AuxTQuotMM.h"
#undef GENMATRIX1
#undef PRODXM1
#define GENMATRIX1 GenLowerTriMatrix
#define PRODXM1 ProdXL
#include "TMV_AuxTQuotMM.h"
#undef GENMATRIX1
#undef GENMATRIX2
#undef PRODXM1
#undef PRODXM2
#undef QUOTXM
#undef TQUOTMM
#undef TRQUOTMM

}

#undef CT

#endif
