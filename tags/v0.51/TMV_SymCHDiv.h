//---------------------------------------------------------------------------
//
// This file contains the code for doing division using 
// Cholesky Decomposition.
// 
// The algorithm is much like the LU decomposition, but we don't do
// any pivoting, and since the source matrix is symmetric, L = LT
// (or for Hermition, L = Lt).
//


#ifndef TMV_SymCHDiv_H
#define TMV_SymCHDiv_H

#include "TMV_SymMatrix.h"
#include "TMV_Divider.h"

namespace tmv {

  extern bool RecursiveCH;

  template <class T> void HermCH_Decompose(const SymMatrixView<T>& A);
  // Decompose A into L*Lt
  
  template <class T1, class T2> void HermCH_LDivEq(
      const GenSymMatrix<T1>& L, const MatrixView<T2>& m);
  template <class T1, class T2> void HermCH_RDivEq(
      const GenSymMatrix<T1>& L, const MatrixView<T2>& m);

#define CT complex<T>
  template <class T> inline void HermCH_LDivEq(
      const GenSymMatrix<CT>& L, const MatrixView<T>& m)
  { TMVAssert(false); }
  template <class T> inline void HermCH_RDivEq(
      const GenSymMatrix<CT>& L, const MatrixView<T>& m)
  { TMVAssert(false); }
#undef CT

  template <class T> class HermCHDiv : 
    virtual public Divider<T> 
  {

    public :

      //
      // Constructors
      //

      HermCHDiv(const GenSymMatrix<T>& A, bool _inplace);
      ~HermCHDiv();

      //
      // Divider Versions of DivEq and Div
      //

      template <class T1> inline void DoLDivEq(const MatrixView<T1>& m) const 
      { 
	TMVAssert(LLx.size() == m.colsize());
	HermCH_LDivEq(LLx,m); 
      } 

      template <class T1> inline void DoRDivEq(const MatrixView<T1>& m) const 
      {
	TMVAssert(LLx.size() == m.rowsize());
	HermCH_RDivEq(LLx,m); 
      } 

      template <class T1, class T2> inline void DoLDiv(
	  const GenMatrix<T1>& m1, const MatrixView<T2>& m0) const 
      { 
	TMVAssert(m1.colsize() == m0.colsize()); 
	TMVAssert(m1.rowsize() == m0.rowsize());
	TMVAssert(LLx.size() == m1.colsize());
	HermCH_LDivEq(LLx,m0=m1); 
      }

      template <class T1, class T2> inline void DoRDiv(
	  const GenMatrix<T1>& m1, const MatrixView<T2>& m0) const 
      { 
	TMVAssert(m1.colsize() == m0.colsize()); 
	TMVAssert(m1.rowsize() == m0.rowsize());
	TMVAssert(LLx.size() == m1.rowsize());
	HermCH_RDivEq(LLx,m0=m1); 
      } 

#include "TMV_AuxAllDiv.h"

      //
      // Determinant, Inverse
      //

      T Det() const;
      void Inverse(const MatrixView<T>& minv) const;
      void InverseATA(const MatrixView<T>& minv) const;
      inline bool Singular() const { return Det() == T(0); }

      //
      // Access Decomposition
      //

      const ConstLowerTriMatrixView<T> GetL() const { return LLx.LowerTri(); }
      const GenSymMatrix<T>& GetLL() const { return LLx; }

      inline string Type() const
      { return string("HermCHDiv<") + tmv::Type(T()) + ">"; }
      inline DivType GetDivType() const { return CH; }

      bool CheckDecomp(const BaseMatrix<T>& m, ostream* fout) const;

    private :
      bool inplace;
      T* Aptr;
      SymMatrixView<T> LLx;
      mutable T det;
      mutable bool donedet;
  };

} // namespace tmv

#endif
