//---------------------------------------------------------------------------
//
// This file defines the TMV TriMatrix class.
//
// Constructors:
//
//    There are two TriMatrix classes: UpperTriMatrix<T> and LowerTriMatrix<T>
//    For these notes, I will just write TriMatrix, but for all uses,
//    you need to write "Upper" or "Lower" before the "Tri".
//
//    In addition to the type template parameter (T), TriMatrixes have two
//    additional template parameters:
//        DiagType dt = UnitDiag || NonUnitDiag 
//        StorageType stor = RowMajor || ColMajor
//
//        They both have default values, so you can omit both, or
//        just stor.  The default values are: {NonUnitDiag, RowMajor}
//
//        If dt is UnitDiag, then the diagonal elements are not
//        actually stored or referenced.  The are all taken to be = 1.
//
//        The storage follows the same meaning as for regular Matrices.
//
//    TriMatrix<T,dt,stor>(size_t n)
//        Makes a Triangular Matrix with column size = row size = n
//        with _uninitialized_ values.
//
//    TriMatrix<T,dt,stor>(size_t n, T x)
//        Makes a Triangular Matrix with column size = row size = n
//        with all values = x
//
//    TriMatrix<T,dt,stor>(const Matrix<T>& m)
//    TriMatrix<T,dt,stor>(const TriMatrix<T>& m)
//        Makes a TriMatrix which copies the corresponding elements of m.
//
//    ConstUpperTriMatrixView<T> UpperTriMatrixViewOf(const Matrix<T>& m, dt)
//    ConstLowerTriMatrixView<T> LowerTriMatrixViewOf(const Matrix<T>& m, dt)
//    ConstUpperTriMatrixView<T> UpperTriMatrixViewOf(const TriMatrix<T>& m, dt)
//    ConstLowerTriMatrixView<T> LowerTriMatrixViewOf(const TriMatrix<T>& m, dt)
//        Makes a constant TriMatrix view of the corresponding part of m.
//        While this view cannot be modified, changing the original m
//        will cause corresponding changes in this view of m.
//        The last two versions allows you to re-view a NonUnitDiag TriMatrix
//        as a UnitDiag TriMatrix.
//
//    UpperTriMatrixView<T> UpperTriMatrixViewOf(Matrix<T>& m, dt)
//    LowerTriMatrixView<T> LowerTriMatrixViewOf(Matrix<T>& m, dt)
//    UpperTriMatrixView<T> UpperTriMatrixViewOf(UpperTriMatrix<T>& m, dt)
//    LowerTriMatrixView<T> LowerTriMatrixViewOf(LowerTriMatrix<T>& m, dt)
//        Makes a modifiable TriMatrix view of the corresponding part of m.
//        Modifying this matrix will change the corresponding elements in
//        the original Matrix.
//
//    ConstUpperTriMatrixView<T> UpperTriMatrixViewOf(const T* m, size, 
//        dt, stor)
//    UpperTriMatrixView<T> UpperTriMatrixViewOf(T* m, size, dt, stor)
//    ConstLowerTriMatrixView<T> LowerTriMatrixViewOf(const T* m, size, 
//        dt, stor)
//    LowerTriMatrixView<T> LowerTriMatrixViewOf(T* m, size, dt, stor)
//
// Access Functions
//
//    size_t colsize() const
//    size_t rowsize() const
//    size_t size() const
//        Return the dimensions of the TriMatrix
//
//    T& operator()(size_t i, size_t j)
//    T operator()(size_t i, size_t j) const
//        Return the (i,j) element of the TriMatrix
//
//    Vector& row(size_t i, size_t j1, size_t j2)
//        Return a portion of the ith row 
//        This range must be a valid range for the requested row.
//
//    Vector& col(size_t j, size_t i1, size_t i2)
//        Return a portion of the jth column
//        This range must be a valid range for the requested column.
//
//    Vector& diag()
//        Return the main diagonal
//        The TriMatrix must be NonUnitDiag.
//
//    Vector& diag(int i, size_t j1, size_t j2)
//    Vector& diag(int i)
//        Return the super- or sub-diagonal i
//        If i > 0 return the super diagonal starting at m_0i
//        If i < 0 return the sub diagonal starting at m_|i|0
//        If j1,j2 are given, it returns the diagonal SubVector 
//        either from m_j1,i+j1 to m_j2,i+j2 (for i>0) 
//        or from m_|i|+j1,j1 to m_|i|+j2,j2 (for i<0)
//        i>0 will give an error for a LowerTriMatrix
//        i<0 will give an error for an UpperTriMatrix
//        i=0 will give an error for a UnitDiag TriMatrix
//
// Modifying Functions:
//
//    Zero()
//    SetAllTo(T x)
//    ConjugateSelf()
//    SetToIdentity(x = 1)
//    void Swap(TriMatrix& m1, TriMatrix& m2)
//        The TriMatrices must be the same size and shape (Upper or Lower).
//
// Views of a TriMatrix:
//
//    SubMatrix(int i1, int i2, int j1, int j2, int istep=1, int jstep=1)
//        This member function will return a submatrix using rows i1 to i2
//        and columns j1 to j2 which refers
//        to the same physical elements as the original.
//        The submatrix must be completely contained within the TriMatrix.
//
//    SubVector(int i, int j, int istep, int jstep, int size)
//        Returns a SubVector which starts at position (i,j) in the 
//        matrix, moves in the directions (istep,jstep) and has a length
//        of size.
//
//    SubTriMatrix(int i1, int i2, int istep)
//        Returns the TriMatrix which runs from i1 to i2 along the diagonal
//        (not including i2) with an optional step, and includes the 
//        off diagonal in the same rows/cols.
//
//        For example, with an UpperTriMatrix of size 10, the x's below
//        are the original data, the O's are the SubTriMatrix returned
//        with the command SubTriMatrix(3,11,2), and the #'s are the 
//        SubTriMatrix returned with SubTriMatrix(0,3)
//
//        ###xxxxxxx
//         ##xxxxxxx
//          #xxxxxxx
//           OxOxOxO
//            xxxxxx
//             OxOxO
//              xxxx
//               OxO
//                xx
//                 O
//
//    OffDiag()
//        Returns the (NonUnitDiag) TriMatrix of all the off-diagonal
//        elements of a TriMatrix.
//
//    MakeUnitDiag()
//        Recasts the TriMatrix to have unit diagonal
//
//    Transpose(m)
//    Adjoint(m)
//        Note that the Transpose or Adjoint of an UpperTriMatrix returns 
//        a view which is a LowerTriMatrix, and vice versa.
//    Conjugate(m)
//
//    Real(), Imag()
//        For a complex TriMatrix, returns the real or imaginary part
//        as a real TriMatrix.
//
//
// Functions of Matrixs:
//
//    Det(m)
//    Trace(m)
//    Norm(m) or NormF(m)
//    NormSq(m)
//    Norm1(m) 
//    Norm2(m) 
//    NormInf(m) 
//
//    m.Inverse()
//    Inverse(m)
//    m.Inverse(minv) // Takes either a TriMatrix or Matrix argument
//    m.InverseATA(invata)
//
//    m.NewTranspose()
//    m.NewConjugate()
//    m.NewAdjoint()
//    m.NewInverse()
//    m.NewCopy()
//
//
// I/O: 
//
//    os << m 
//        Writes m to ostream os in the usual Matrix format
//
//    m.WriteCompact(os)
//        Writes m to ostream os in the following compact format:
//        For an UpperTriMatrix:
//          size 
//          ( m(0,0) m(0,1) ... m(0,size) )
//          ( m(1,1) .. m(1,size) )
//          ...
//          ( m(size,size) )
//
//        For a LowerTriMatrix:
//          size 
//          ( m(0,0) )
//          ( m(1,0) m(1,1) )
//          ...
//          ( m(size,0) ... m(size,size) )
//
//    is >> m
//        Reads m from istream is in the compact format
//        m must already be the correct size for this to work.
//
//    is >> mptr
//        If you do not know the size of the TriMatrix to be read, you can
//        use this form where mptr is a pointer to an undefined TriMatrix.
//        Then *mptr will be created with the right size using new,
//        so you should subsequently delete it.
//        (Note: if the DiagType for the TriMatrix is UnitDiag, then
//        all of the diagonals read in must be = 1.)
//
//
// Division Control Functions:
//
//    Most of the point of using TriMatrices is that they are easy
//    to divide using either forward substitution or back substitution.
//    This form of division is essentially the LU variety.  As such,
//    this is the only option available for "DivideUsing".  (It is also
//    the default, so you do not need to specify it directly.)
//


#ifndef TMV_TriMatrix_H
#define TMV_TriMatrix_H

#include "TMV_BaseMatrix.h"

namespace tmv {

  template <class T> class GenUpperTriMatrix;
  template <class T> class GenLowerTriMatrix;
  template <class T> class ConstUpperTriMatrixView;
  template <class T> class ConstLowerTriMatrixView;
  template <class T> class UpperTriMatrixView;
  template <class T> class LowerTriMatrixView;
  template <class T, DiagType D=NonUnitDiag, StorageType S=RowMajor> 
    class UpperTriMatrix;
  template <class T, DiagType D=NonUnitDiag, StorageType S=RowMajor> 
    class LowerTriMatrix;
  
  template <class T> class UpperTriMatrixComposite; 
  template <class T> class LowerTriMatrixComposite; 
  template <class T> class UpperTriDiv;
  template <class T> class LowerTriDiv;
  template <class T, class Tm> class QuotXU;
  template <class T, class Tm> class QuotXL;

  template <class T1, class T2> void Copy(
      const GenUpperTriMatrix<T1>& m1, const UpperTriMatrixView<T2>& m2);

  template <class T> class GenUpperTriMatrix : 
    public BaseMatrix<T>
  {

    public:

      //
      // Constructors
      //

      GenUpperTriMatrix(DiagType d, StorageType s, ConjItType c) : 
	BaseMatrix<T>(LU), itsdiag(d), itsstor(s), itsct(c) {}
      GenUpperTriMatrix(const GenUpperTriMatrix<T>& rhs) : 
	BaseMatrix<T>(rhs), itsdiag(rhs.itsdiag), itsstor(rhs.itsstor),
	itsct(rhs.itsct) {}
      ~GenUpperTriMatrix() {}

      //
      // Access Functions
      //

      inline size_t colsize() const { return size(); }
      inline size_t rowsize() const { return size(); }

      inline ConstVectorView<T> row(size_t i, size_t j1, size_t j2) const 
      { 
	TMVAssert(i<size());
	TMVAssert(j1<=j2);
	TMVAssert(j2<=size());
	TMVAssert(j1==j2 || okij(i,j1));
	return ConstVectorView<T>(cptr()+i*stepi()+j1*stepj(),j2-j1,stepj(),
	    itsct); 
      }

      inline ConstVectorView<T> col(size_t j, size_t i1, size_t i2) const
      {
	TMVAssert(j<size());
	TMVAssert(i1<=i2);
	TMVAssert(i2<=size());
	TMVAssert(i1==i2 || okij(i2-1,j));
	return ConstVectorView<T>(cptr()+i1*stepi()+j*stepj(),i2-i1,stepi(),
	    itsct); 
      }

      inline ConstVectorView<T> diag() const
      {
	TMVAssert(!isunit());
	return ConstVectorView<T>(cptr(),size(),stepi()+stepj(),itsct); 
      }

      inline ConstVectorView<T> diag(int i) const
      {
	TMVAssert(i<=int(size())); 
	TMVAssert(isunit() ? i>0 : i>=0);
	return ConstVectorView<T>(cptr()+i*stepj(),size()-i,stepi()+stepj(),
	    itsct); 
      }

      inline ConstVectorView<T> diag(int i, size_t j1, size_t j2) const
      {
	TMVAssert(isunit() ? i>0 : i>=0);
	TMVAssert(j1 <= j2);
	TMVAssert(j2 <= size()-i);
	const int ds = stepi()+stepj();
	return ConstVectorView<T>(cptr()+i*stepj()+j1*ds,j2-j1,ds,itsct);
      }

      template <class T2> inline bool SameStorageAs(
	  const BaseMatrix<T2>& m2) const
      { return false; }

      inline bool SameStorageAs(const GenMatrix<T>& m2) const
      { return (cptr()==m2.cptr()); }

      inline bool SameStorageAs(const GenUpperTriMatrix<T>& m2) const
      { 
	if (cptr() == m2.cptr()) {
	  bool up1 = stepi()>stepj();
	  bool up2 = m2.stepi()>m2.stepj();
	  if (up1==up2) return true;
	  else return !isunit() && !m2.isunit();
	} else return false;
      }

      inline bool SameStorageAs(const GenLowerTriMatrix<T>& m2) const
      { return SameStorageAs(m2.Transpose()); }

      template <class T2> inline bool SameAs(const BaseMatrix<T2>& m2) const
      { return false; }

      inline bool SameAs(const GenUpperTriMatrix<T>& m2) const
      { 
	return (this == &m2 || (cptr()==m2.cptr() && size()==m2.size() && 
	    itsdiag == m2.itsdiag && itsct == m2.itsct &&
	    stepi()==m2.stepi() && stepj()==m2.stepj()));
      }

      template <class T2> inline void DoCopyToMatrix(
	  const MatrixView<T2>& m2) const
      {
	TMVAssert(m2.colsize() == size());
	TMVAssert(m2.rowsize() == size());
	Copy(*this,UpperTriMatrixViewOf(m2,itsdiag));
	if (isunit()) m2.diag().SetAllTo(T2(1));
	if (size() > 0) LowerTriMatrixViewOf(m2).OffDiag().Zero();
      }
      inline void CopyToMatrix(const MatrixView<RealType(T)>& m2) const
      { TMVAssert(IsReal(T())); DoCopyToMatrix(m2); }
      inline void CopyToMatrix(const MatrixView<ComplexType(T)>& m2) const
      { DoCopyToMatrix(m2); }

      //
      // SubMatrix
      //

      bool OKSubMatrix(int i1, int i2, int j1, int j2,
	  int istep, int jstep) const;

      inline ConstMatrixView<T> SubMatrix(int i1, int i2,
	  int j1, int j2) const
      {
	TMVAssert(OKSubMatrix(i1,i2,j1,j2,1,1));
	return ConstMatrixView<T>(cptr()+i1*stepi()+j1*stepj(),
	    i2-i1, j2-j1, stepi(), stepj(), itsstor, itsct);
      }

      inline ConstMatrixView<T> SubMatrix(int i1, int i2, int j1, int j2,
	  int istep, int jstep) const
      {
	const StorageType newstor =
	  iscm() ? (istep == 1 ? ColMajor : NoMajor) :
	    isrm() ? (jstep == 1 ? RowMajor : NoMajor) : NoMajor;
	TMVAssert(OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	return ConstMatrixView<T>(cptr()+i1*stepi()+j1*stepj(),
	    (i2-i1)/istep,(j2-j1)/jstep,istep*stepi(),jstep*stepj(), 
	    newstor, itsct);
      }

      bool OKSubVector(size_t i, size_t j, int istep, int jstep, 
	  size_t size) const;

      inline ConstVectorView<T> SubVector(size_t i, size_t j,
	  int istep, int jstep, size_t size) const
      {
	TMVAssert(OKSubVector(i,j,istep,jstep,size));
	return ConstVectorView<T>(cptr()+i*stepi()+j*stepj(),size,
	    istep*stepi()+jstep*stepj(),itsct);
      }

      bool OKSubTriMatrix(int i1, int i2, int istep) const;

      inline ConstUpperTriMatrixView<T> SubTriMatrix(int i1, int i2) const
      {
	TMVAssert(OKSubTriMatrix(i1,i2,1));
	return ConstUpperTriMatrixView<T>(cptr()+i1*(stepi()+stepj()),i2-i1,
	    stepi(),stepj(),itsdiag,itsstor,itsct);
      }

      inline ConstUpperTriMatrixView<T> SubTriMatrix(int i1, int i2,
	  int istep) const
      {
	TMVAssert(OKSubTriMatrix(i1,i2,istep));
	return ConstUpperTriMatrixView<T>(cptr()+i1*(stepi()+stepj()),
	    (i2-i1)/istep,istep*stepi(),istep*stepj(),itsdiag,
	    istep==1 ? itsstor : NoMajor,itsct);
      }

      inline ConstUpperTriMatrixView<T> OffDiag() const
      {
	TMVAssert(size() > 0);
	return ConstUpperTriMatrixView<T>(cptr()+stepj(),size()-1,
	    stepi(),stepj(),NonUnitDiag,itsstor,itsct);
      }

      inline ConstUpperTriMatrixView<T> MakeUnitDiag() const
      { 
	return ConstUpperTriMatrixView<T>(cptr(),size(),
	    stepi(),stepj(),UnitDiag,itsstor,itsct);
      }

      inline ConstUpperTriMatrixView<T> View() const
      { 
	return ConstUpperTriMatrixView<T>(cptr(),size(),
	    stepi(),stepj(),itsdiag,itsstor,itsct);
      }

      inline ConstLowerTriMatrixView<T> Transpose() const
      { 
	return ConstLowerTriMatrixView<T>(cptr(),size(),
	    stepj(),stepi(),itsdiag,TransOf(itsstor),itsct);
      }

      inline ConstUpperTriMatrixView<T> Conjugate() const
      { 
	return ConstUpperTriMatrixView<T>(cptr(),size(),
	    stepi(),stepj(),itsdiag,itsstor,ConjOf(T,itsct));
      }

      inline ConstLowerTriMatrixView<T> Adjoint() const
      { 
	return ConstLowerTriMatrixView<T>(cptr(),size(),
	    stepj(),stepi(),itsdiag,TransOf(itsstor),ConjOf(T,itsct));
      }

      inline ConstUpperTriMatrixView<RealType(T)> Real() const
      {
	return ConstUpperTriMatrixView<RealType(T)>(
	    reinterpret_cast<const RealType(T)*>(cptr()),size(),
	    IsReal(T()) ? stepi() : 2*stepi(),
	    IsReal(T()) ? stepj() : 2*stepj(),
	    itsdiag, IsReal(T()) ? itsstor : NoMajor,NonConj);
      }

      inline ConstUpperTriMatrixView<RealType(T)> Imag() const
      {
	TMVAssert(IsComplex(T()));
	return ConstUpperTriMatrixView<RealType(T)>(
	    reinterpret_cast<const RealType(T)*>(cptr())+1,
	    size(),2*stepi(),2*stepj(),itsdiag,NoMajor,NonConj);
      }

      //
      // Functions of Matrix
      //

      inline T Trace() const
      { return isunit() ? T(size()) : diag().SumElements(); }

      inline RealType(T) Norm() const 
      { return SQRT(NormSq()); }

      RealType(T) NormF() const;

      // NormF()^2
      RealType(T) NormSq() const;

      // 1-Norm = max_j (sum_i |a_ij|)
      RealType(T) Norm1() const;

      // 2-Norm defined in BaseMatrix.h won't work, since can't do SV
      RealType(T) Norm2() const
      { return Matrix<T>(*this).Norm2(); }

      RealType(T) Condition() const
      { return Matrix<T>(*this).Condition(); }

      // inf-Norm = max_i (sum_j |a_ij|)
      RealType(T) NormInf() const;

      // = max_i,j (|a_ij|)
      RealType(T) MaxAbsElement() const;

      inline QuotXU<T,T> Inverse() const
      { return QuotXU<T,T>(T(1),*this); }

      void Inverse(const UpperTriMatrixView<T>& minv) const;

      void Inverse(const MatrixView<T>& minv) const
      {
	TMVAssert(minv.IsSquare());
	TMVAssert(minv.colsize() == size());
	TMVAssert(minv.rowsize() == size());
	if (size() > 0) {
	  LowerTriMatrixViewOf(minv).OffDiag().Zero();
	  Inverse(UpperTriMatrixViewOf(minv));
	}
      }

      template <DiagType D, StorageType S> inline void Inverse(
	  UpperTriMatrix<T,D,S>& minv) const
      { 
	TMVAssert(D==NonUnitDiag || isunit());
	Inverse(minv.View()); 
      }

      template <StorageType S> inline void Inverse(Matrix<T,S>& minv) const
      { Inverse(minv.View()); }

      using BaseMatrix<T>::InverseATA;

      template <StorageType S> inline void InverseATA(Matrix<T,S>& minv) const
      { InverseATA(minv.View()); }

      inline BaseMatrix<T>* NewTranspose() const 
      { return new ConstLowerTriMatrixView<T>(Transpose()); }

      inline BaseMatrix<T>* NewConjugate() const
      { return new ConstUpperTriMatrixView<T>(Conjugate()); }

      inline BaseMatrix<T>* NewAdjoint() const
      { return new ConstLowerTriMatrixView<T>(Adjoint()); }

      inline BaseMatrix<T>* NewView() const
      { return new ConstUpperTriMatrixView<T>(View()); }

      inline BaseMatrix<T>* NewCopy() const
      { 
	if (isunit())
	  if (isrm()) return new UpperTriMatrix<T,UnitDiag,RowMajor>(*this); 
	  else return new UpperTriMatrix<T,UnitDiag,ColMajor>(*this); 
	else
	  if (isrm()) return new UpperTriMatrix<T,NonUnitDiag,RowMajor>(*this); 
	  else return new UpperTriMatrix<T,NonUnitDiag,ColMajor>(*this); 
      }

      // 
      // Division Control
      //

      inline void DivideUsing(DivType dt) const
      { TMVAssert(dt == LU); }

      //
      // I/O
      //

      void WriteCompact(ostream& os) const;
      void Write(ostream& os) const;
      void WriteCompact(ostream& os, RealType(T) thresh) const;
      void Write(ostream& os, RealType(T) thresh) const;

      //
      // Arithmetic Helpers
      //

      using BaseMatrix<T>::LDivEq;
      using BaseMatrix<T>::RDivEq;
      using BaseMatrix<T>::LDiv;
      using BaseMatrix<T>::RDiv;

      template <class T1> void LDivEq(
	  const UpperTriMatrixView<T1>& m) const;
      template <class T1> void RDivEq(
	  const UpperTriMatrixView<T1>& m) const;
      template <class T1, class T0> void LDiv(
	  const GenUpperTriMatrix<T1>& m1,
	  const UpperTriMatrixView<T0>& m0) const;
      template <class T1, class T0> void RDiv(
	  const GenUpperTriMatrix<T1>& m1,
	  const UpperTriMatrixView<T0>& m0) const;

      virtual const T* cptr() const = 0;
      virtual int stepi() const = 0;
      virtual int stepj() const = 0;
      virtual size_t size() const = 0;
      virtual inline StorageType stor() const { return itsstor; }
      virtual inline DiagType dt() const { return itsdiag; }
      virtual inline ConjItType ct() const { return itsct; }
      virtual inline bool isrm() const { return itsstor == RowMajor; }
      virtual inline bool iscm() const { return itsstor == ColMajor; }
      virtual inline bool isunit() const { return itsdiag == UnitDiag; }
      inline bool isconj() const
      {
	TMVAssert(IsComplex(T()) || itsct==NonConj);
	return IsComplex(T()) && itsct==Conj;
      }

    protected :

      inline bool okij(size_t i, size_t j) const
      {
	TMVAssert(i < size());
	TMVAssert(j < size());
	if (isunit()) return i<j; else return i<=j;
      }

      virtual T cref(size_t i, size_t j) const;

      void NewDivider() const;

    private :

      DiagType itsdiag;
      StorageType itsstor;
      ConjItType itsct;

      void operator=(const GenUpperTriMatrix<T>&) { TMVAssert(false); }

  }; // GenUpperTriMatrix

  template <class T> class GenLowerTriMatrix : 
    public BaseMatrix<T>
  {

    public:

      //
      // Constructors
      //

      GenLowerTriMatrix(DiagType d, StorageType s, ConjItType c) : 
	BaseMatrix<T>(LU), itsdiag(d), itsstor(s), itsct(c) {}
      GenLowerTriMatrix(const GenLowerTriMatrix<T>& rhs) : 
	BaseMatrix<T>(rhs), itsdiag(rhs.itsdiag), itsstor(rhs.itsstor),
	itsct(rhs.itsct) {}
      ~GenLowerTriMatrix() {}

      //
      // Access Functions
      //

      inline size_t colsize() const { return size(); }
      inline size_t rowsize() const { return size(); }

      inline ConstVectorView<T> row(size_t i, size_t j1, size_t j2) const 
      { 
	TMVAssert(i<size());
	TMVAssert(j1<=j2);
	TMVAssert(j2<=size());
	TMVAssert(j1==j2 || okij(i,j2-1));
	return ConstVectorView<T>(cptr()+i*stepi()+j1*stepj(),j2-j1,stepj(),
	    itsct); 
      }

      inline ConstVectorView<T> col(size_t j, size_t i1, size_t i2) const
      {
	TMVAssert(j<size());
	TMVAssert(i1<=i2);
	TMVAssert(i2<=size());
	TMVAssert(i1==i2 || okij(i1,j));
	return ConstVectorView<T>(cptr()+i1*stepi()+j*stepj(),i2-i1,stepi(),
	    itsct); 
      }

      inline ConstVectorView<T> diag() const
      {
	TMVAssert(!isunit());
	return ConstVectorView<T>(cptr(),size(),stepi()+stepj(),itsct); 
      }

      inline ConstVectorView<T> diag(int i) const
      {
	TMVAssert(-i<=int(size())); 
	TMVAssert(isunit() ? i<0 : i<=0);
	return ConstVectorView<T>(cptr()-i*stepi(),size()-i,stepi()+stepj(),
	    itsct);
      }

      inline ConstVectorView<T> diag(int i, size_t j1, size_t j2) const
      {
	TMVAssert(isunit() ? i<0 : i<=0);
	TMVAssert(j1 <= j2);
	TMVAssert(j2 <= size()+i);
	const int ds = stepi()+stepj();
	return ConstVectorView<T>(cptr()-i*stepi()+j1*ds,j2-j1,ds,itsct);
      }

      template <class T2> inline bool SameStorageAs(
	  const BaseMatrix<T2>& m2) const
      { return false; }

      inline bool SameStorageAs(const GenMatrix<T>& m2) const
      { return (cptr()==m2.cptr()); }

      inline bool SameStorageAs(const GenLowerTriMatrix<T>& m2) const
      { return Transpose().SameStorageAs(m2.Transpose()); }

      inline bool SameStorageAs(const GenUpperTriMatrix<T>& m2) const
      { return Transpose().SameStorageAs(m2); }

      template <class T2> inline bool SameAs(const BaseMatrix<T2>& m2) const
      { return false; }

      inline bool SameAs(const GenLowerTriMatrix<T>& m2) const
      { 
	if (this == &m2) return true;
	else return (cptr()==m2.cptr() && size()==m2.size() && 
	    dt() == m2.dt() && itsct == m2.itsct &&
	    stepi()==m2.stepi() && stepj()==m2.stepj());
      }

      inline void CopyToMatrix(const MatrixView<RealType(T)>& m2) const
      { Transpose().CopyToMatrix(m2.Transpose()); }
      inline void CopyToMatrix(const MatrixView<ComplexType(T)>& m2) const
      { Transpose().CopyToMatrix(m2.Transpose()); }

      //
      // SubMatrix
      //

      inline bool OKSubMatrix(int i1, int i2, int j1, int j2,
          int istep, int jstep) const
      { return Transpose().OKSubMatrix(j1,j2,i1,i2,jstep,istep); }

      inline ConstMatrixView<T> SubMatrix(int i1, int i2,
	  int j1, int j2) const
      {
	TMVAssert(OKSubMatrix(i1,i2,j1,j2,1,1));
	return ConstMatrixView<T>(cptr()+i1*stepi()+j1*stepj(),
	    i2-i1, j2-j1, stepi(), stepj(), stor(), itsct);
      }

      inline ConstMatrixView<T> SubMatrix(int i1, int i2, int j1, int j2,
	  int istep, int jstep) const
      {
	const StorageType newstor =
	  iscm() ? (istep == 1 ? ColMajor : NoMajor) :
	    isrm() ? (jstep == 1 ? RowMajor : NoMajor) : NoMajor;
	TMVAssert(OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	return ConstMatrixView<T>(cptr()+i1*stepi()+j1*stepj(),
	    (i2-i1)/istep,(j2-j1)/jstep,istep*stepi(),jstep*stepj(), 
	    newstor,itsct);
      }

      bool OKSubVector(size_t i, size_t j, int istep, int jstep, 
	  size_t size) const
      { return Transpose().OKSubVector(j,i,jstep,istep,size); }

      inline ConstVectorView<T> SubVector(size_t i, size_t j,
	  int istep, int jstep, size_t size) const
      {
	TMVAssert(OKSubVector(i,j,istep,jstep,size));
	return ConstVectorView<T>(cptr()+i*stepi()+j*stepj(),size,
	    istep*stepi()+jstep*stepj(),itsct);
      }

      bool OKSubTriMatrix(int i1, int i2, int istep) const
      { return Transpose().OKSubTriMatrix(i1,i2,istep); }

      inline ConstLowerTriMatrixView<T> SubTriMatrix(int i1, int i2) const
      {
	TMVAssert(OKSubTriMatrix(i1,i2,1));
	return ConstLowerTriMatrixView<T>(cptr()+i1*(stepi()+stepj()),i2-i1,
	    stepi(),stepj(),dt(),stor(),itsct);
      }

      inline ConstLowerTriMatrixView<T> SubTriMatrix(int i1, int i2,
	  int istep) const
      {
	TMVAssert(OKSubTriMatrix(i1,i2,istep));
	return ConstLowerTriMatrixView<T>(cptr()+i1*(stepi()+stepj()),
	    (i2-i1)/istep,istep*stepi(),istep*stepj(),dt(),
	    istep==1 ? stor() : NoMajor,itsct);
      }

      inline ConstLowerTriMatrixView<T> OffDiag() const
      {
	TMVAssert(size() > 0);
	return ConstLowerTriMatrixView<T>(cptr()+stepi(),size()-1,
	    stepi(),stepj(),NonUnitDiag,stor(),itsct);
      }

      inline ConstLowerTriMatrixView<T> MakeUnitDiag() const
      { 
	return ConstLowerTriMatrixView<T>(cptr(),size(),
	    stepi(),stepj(),UnitDiag,itsstor,itsct);
      }

      inline ConstLowerTriMatrixView<T> View() const
      { 
	return ConstLowerTriMatrixView<T>(cptr(),size(),
	    stepi(),stepj(),dt(),stor(),itsct);
      }

      inline ConstUpperTriMatrixView<T> Transpose() const
      { 
	return ConstUpperTriMatrixView<T>(cptr(),size(),
	    stepj(),stepi(),dt(),TransOf(stor()),itsct);
      }
  
      inline ConstLowerTriMatrixView<T> Conjugate() const
      { 
	return ConstLowerTriMatrixView<T>(cptr(),size(),
	    stepi(),stepj(),dt(),stor(),ConjOf(T,itsct));
      }

      inline ConstUpperTriMatrixView<T> Adjoint() const
      { 
	return ConstUpperTriMatrixView<T>(cptr(),size(),
	    stepj(),stepi(),dt(),TransOf(stor()),ConjOf(T,itsct));
      }

      inline ConstLowerTriMatrixView<RealType(T)> Real() const
      {
	return ConstLowerTriMatrixView<RealType(T)>(
	    reinterpret_cast<const RealType(T)*>(cptr()),size(),
	    IsReal(T()) ? stepi() : 2*stepi(),
	    IsReal(T()) ? stepj() : 2*stepj(),
	    dt(), IsReal(T()) ? stor() : NoMajor,NonConj);
      }

      inline ConstLowerTriMatrixView<RealType(T)> Imag() const
      {
	TMVAssert(IsComplex(T()));
	return ConstLowerTriMatrixView<RealType(T)>(
	    reinterpret_cast<const RealType(T)*>(cptr())+1,
	    size(),2*stepi(),2*stepj(),dt(),NoMajor,NonConj);
      }

      //
      // Functions of Matrix
      //

      inline T Trace() const
      { return isunit() ? T(size()) : diag().SumElements(); }

      inline QuotXL<T,T> Inverse() const
      { return QuotXL<T,T>(T(1),*this); }

      void Inverse(const LowerTriMatrixView<T>& minv) const;

      void Inverse(const MatrixView<T>& minv) const
      {
	TMVAssert(minv.IsSquare()); 
	TMVAssert(minv.colsize() == size());
	TMVAssert(minv.rowsize() == size());
	if (size() > 0) {
	  UpperTriMatrixViewOf(minv).OffDiag().Zero();
	  Inverse(LowerTriMatrixViewOf(minv)); 
	}
      }

      template <DiagType D, StorageType S> inline void Inverse(
	  LowerTriMatrix<T,D,S>& minv) const
      { 
	Assert(D==NonUnitDiag || isunit());
	Inverse(minv.View()); 
      }

      template <StorageType S> inline void Inverse(Matrix<T,S>& minv) const
      { Inverse(minv.View()); }

      using BaseMatrix<T>::InverseATA;

      template <StorageType S> inline void InverseATA(Matrix<T,S>& minv) const
      { InverseATA(minv.View()); }

      inline RealType(T) Norm() const 
      { return SQRT(NormSq()); }

      inline RealType(T) NormF() const 
      { return SQRT(NormSq()); }

      // NormF()^2
      inline RealType(T) NormSq() const 
      { return Transpose().NormSq(); }

      // 1-Norm = max_j (sum_i |a_ij|)
      inline RealType(T) Norm1() const
      { return Transpose().NormInf(); }

      // 2-Norm defined in BaseMatrix.h

      // inf-Norm = max_i (sum_j |a_ij|)
      inline RealType(T) NormInf() const
      { return Transpose().Norm1(); }

      // = max_i,j (|a_ij|)
      inline RealType(T) MaxAbsElement() const
      { return Transpose().MaxAbsElement(); }

      inline BaseMatrix<T>* NewTranspose() const 
      { return new ConstUpperTriMatrixView<T>(Transpose()); }

      inline BaseMatrix<T>* NewConjugate() const
      { return new ConstLowerTriMatrixView<T>(Conjugate()); }

      inline BaseMatrix<T>* NewAdjoint() const
      { return new ConstUpperTriMatrixView<T>(Adjoint()); }

      inline BaseMatrix<T>* NewView() const
      { return new ConstLowerTriMatrixView<T>(View()); }

      inline BaseMatrix<T>* NewCopy() const
      { 
	if (isunit())
	  if (isrm()) return new LowerTriMatrix<T,UnitDiag,RowMajor>(*this); 
	  else return new LowerTriMatrix<T,UnitDiag,ColMajor>(*this); 
	else
	  if (isrm()) return new LowerTriMatrix<T,NonUnitDiag,RowMajor>(*this); 
	  else return new LowerTriMatrix<T,NonUnitDiag,ColMajor>(*this); 
      }

      // 
      // Division Control
      //

      inline void DivideUsing(DivType dt) const
      { TMVAssert(dt == LU); }

      //
      // I/O
      //

      void WriteCompact(ostream& os) const;
      void Write(ostream& os) const;
      void WriteCompact(ostream& os, RealType(T) thresh) const;
      void Write(ostream& os, RealType(T) thresh) const;

      //
      // Arithmetic Helpers
      //

      using BaseMatrix<T>::LDivEq;
      using BaseMatrix<T>::RDivEq;
      using BaseMatrix<T>::LDiv;
      using BaseMatrix<T>::RDiv;

      template <class T1> void LDivEq(
	  const LowerTriMatrixView<T1>& m) const;
      template <class T1> void RDivEq(
	  const LowerTriMatrixView<T1>& m) const;
      template <class T1, class T0> void LDiv(
	  const GenLowerTriMatrix<T1>& m1,
	  const LowerTriMatrixView<T0>& m0) const;
      template <class T1, class T0> void RDiv(
	  const GenLowerTriMatrix<T1>& m1,
	  const LowerTriMatrixView<T0>& m0) const;

      virtual const T* cptr() const = 0;
      virtual int stepi() const = 0;
      virtual int stepj() const = 0;
      virtual size_t size() const = 0;
      virtual inline StorageType stor() const { return itsstor; }
      virtual inline DiagType dt() const { return itsdiag; }
      virtual inline ConjItType ct() const { return itsct; }
      virtual inline bool isrm() const { return itsstor == RowMajor; }
      virtual inline bool iscm() const { return itsstor == ColMajor; }
      virtual inline bool isunit() const { return itsdiag == UnitDiag; }
      inline bool isconj() const
      {
	TMVAssert(IsComplex(T()) || itsct==NonConj);
	return IsComplex(T()) && itsct==Conj;
      }

    protected :

      inline bool okij(size_t i, size_t j) const
      {
	TMVAssert(i < size());
	TMVAssert(j < size());
	if (isunit()) return i>j; else return i>=j;
      }

      virtual T cref(size_t i, size_t j) const;

      void NewDivider() const;

    private :

      DiagType itsdiag;
      StorageType itsstor;
      ConjItType itsct;

      void operator=(const GenLowerTriMatrix<T>&) { TMVAssert(false); }

  }; // GenLowerTriMatrix

  template <class T> class ConstUpperTriMatrixView : 
    public GenUpperTriMatrix<T>
  {
    public :

      ConstUpperTriMatrixView(const ConstUpperTriMatrixView<T>& rhs) :
	GenUpperTriMatrix<T>(rhs), itsm(rhs.itsm),
	itss(rhs.itss), itssi(rhs.itssi), itssj(rhs.itssj) {}

      ConstUpperTriMatrixView(const GenUpperTriMatrix<T>& rhs) :
	GenUpperTriMatrix<T>(rhs), itsm(rhs.cptr()),
	itss(rhs.size()), itssi(rhs.stepi()), itssj(rhs.stepj()) {}

      ConstUpperTriMatrixView(const GenUpperTriMatrix<T>& rhs, DiagType d) :
	GenUpperTriMatrix<T>(d,rhs.stor(),rhs.ct()), itsm(rhs.cptr()),
	itss(rhs.size()), itssi(rhs.stepi()), itssj(rhs.stepj())
      { TMVAssert(d!=NonUnitDiag || !rhs.isunit()); }

      ConstUpperTriMatrixView(const T* _m, size_t _s, int _si, int _sj,
	  DiagType indt, StorageType instor, ConjItType inct) : 
	GenUpperTriMatrix<T>(indt,instor,inct), itsm(_m), itss(_s),
	itssi(_si), itssj(_sj)
      { 
	TMVAssert(instor==RowMajor ? _sj == 1 : instor==ColMajor ?
	    _si==1 : true);
      }

      ConstUpperTriMatrixView(const GenMatrix<T>& rhs, DiagType dt) : 
	GenUpperTriMatrix<T>(dt,rhs.stor(),rhs.ct()),
	itsm(rhs.cptr()), itss(rhs.colsize()), 
	itssi(rhs.stepi()), itssj(rhs.stepj())
      { TMVAssert(rhs.IsSquare()); }

      ~ConstUpperTriMatrixView() {}

      inline size_t size() const { return itss; }
      inline const T* cptr() const { return itsm; }
      inline int stepi() const { return itssi; }
      inline int stepj() const { return itssj; }

    protected :

      const T*const itsm;
      const size_t itss;
      const int itssi;
      const int itssj;

    private :

      inline void operator=(const ConstUpperTriMatrixView<T>&) 
      { TMVAssert(false); }

  }; // ConstUpperTriMatrixView

  template <class T> class ConstLowerTriMatrixView : 
    public GenLowerTriMatrix<T>
  {
    public :

      ConstLowerTriMatrixView(const ConstLowerTriMatrixView<T>& rhs) :
	GenLowerTriMatrix<T>(rhs), itsm(rhs.itsm),
	itss(rhs.itss), itssi(rhs.itssi), itssj(rhs.itssj) {}

      ConstLowerTriMatrixView(const GenLowerTriMatrix<T>& rhs) :
	GenLowerTriMatrix<T>(rhs), itsm(rhs.cptr()),
	itss(rhs.size()), itssi(rhs.stepi()), itssj(rhs.stepj()) {}

      ConstLowerTriMatrixView(const GenLowerTriMatrix<T>& rhs, DiagType d) :
	GenLowerTriMatrix<T>(d,rhs.stor(),rhs.ct()), itsm(rhs.cptr()),
	itss(rhs.size()), itssi(rhs.stepi()), itssj(rhs.stepj())
      { TMVAssert(d!=NonUnitDiag || !rhs.isunit()); }

      ConstLowerTriMatrixView(const T* _m, size_t _s, int _si, int _sj,
	  DiagType indt, StorageType instor, ConjItType inct) : 
	GenLowerTriMatrix<T>(indt,instor,inct), itsm(_m), itss(_s),
	itssi(_si), itssj(_sj)
      { 
	TMVAssert(instor==RowMajor ? _sj == 1 : instor==ColMajor ?
	    _si==1 : true);
      }

      ConstLowerTriMatrixView(const GenMatrix<T>& rhs, DiagType dt) : 
	GenLowerTriMatrix<T>(dt,rhs.stor(),rhs.ct()),
	itsm(rhs.cptr()), itss(rhs.colsize()), 
	itssi(rhs.stepi()), itssj(rhs.stepj())
      { TMVAssert(rhs.IsSquare()); }

      ~ConstLowerTriMatrixView() {}

      inline size_t size() const { return itss; }
      inline const T* cptr() const { return itsm; }
      inline int stepi() const { return itssi; }
      inline int stepj() const { return itssj; }

    protected :

      const T*const itsm;
      const size_t itss;
      const int itssi;
      const int itssj;

    private :

      inline void operator=(const ConstLowerTriMatrixView<T>&) 
      { TMVAssert(false); }

  }; // ConstLowerTriMatrixView

  template <class T> class UpperTriMatrixView : 
    public GenUpperTriMatrix<T>
  {

    public:

      //
      // Constructors
      //

      UpperTriMatrixView(const UpperTriMatrixView<T>& rhs) : 
	GenUpperTriMatrix<T>(rhs), itsm(rhs.itsm), itss(rhs.itss), 
	itssi(rhs.itssi), itssj(rhs.itssj)
	  DEFFIRSTLAST(rhs.first,rhs.last) {}

      UpperTriMatrixView(const UpperTriMatrixView<T>& rhs, DiagType d) : 
	GenUpperTriMatrix<T>(d,rhs.stor(),rhs.ct()), itsm(rhs.itsm), 
	itss(rhs.itss), itssi(rhs.itssi), itssj(rhs.itssj)
	  DEFFIRSTLAST(rhs.first,rhs.last) 
      { TMVAssert(d!=NonUnitDiag || !rhs.isunit()); }

      UpperTriMatrixView(T* _m, size_t _s, int _si, int _sj,
	  DiagType indt, StorageType instor, ConjItType inct 
	  PARAMFIRSTLAST(T) ) :
	GenUpperTriMatrix<T>(indt,instor,inct), itsm(_m), itss(_s),
	itssi(_si), itssj(_sj) DEFFIRSTLAST(_first,_last)
      {
	TMVAssert(instor==RowMajor ? _sj==1 : instor==ColMajor ?
	  _si==1 : true); 
      }

      UpperTriMatrixView(const MatrixView<T>& rhs, DiagType dt) : 
	GenUpperTriMatrix<T>(dt,rhs.stor(),rhs.ct()),
	itsm(rhs.ptr()), itss(rhs.colsize()), 
	itssi(rhs.stepi()), itssj(rhs.stepj())
	  DEFFIRSTLAST(rhs.first,rhs.last) 
      { TMVAssert(rhs.IsSquare()); }

      ~UpperTriMatrixView() {} 

      //
      // Op=
      //

      template <class T2> inline void DoCopy(
	  const GenUpperTriMatrix<T2>& m2) const
      { 
	TMVAssert(!(dt()==UnitDiag && m2.dt()==NonUnitDiag));
	if (dt()==NonUnitDiag && m2.dt()==UnitDiag) {
	  if (size() > 0) Copy(m2.OffDiag(),OffDiag());
	  diag().SetAllTo(T(1));
	} else
	  Copy(m2,*this);
      }

      inline const UpperTriMatrixView<T>& operator=(
	  const UpperTriMatrixView<T>& m2) const
      { if (!SameAs(m2)) DoCopy(m2); return *this; }

      inline const UpperTriMatrixView<T>& operator=(
	  const GenUpperTriMatrix<T>& m2) const
      { if (!SameAs(m2)) DoCopy(m2); return *this; }

      template <class T2> inline const UpperTriMatrixView<T>& operator=(
	  const GenUpperTriMatrix<T2>& m2) const
      { DoCopy(m2); return *this; }

      inline const UpperTriMatrixView<T>& operator=(T x) const 
      { TMVAssert(!isunit() || x==T(1)); return SetToIdentity(x); }

      inline const UpperTriMatrixView<T>& operator=(
	  const UpperTriMatrixComposite<T>& mcomp) const
      { 
	TMVAssert(size() == mcomp.size());
	TMVAssert(!(mcomp.dt()==NonUnitDiag && dt()==UnitDiag));
	if (mcomp.dt()==UnitDiag && dt()==NonUnitDiag) {
	  mcomp.AssignTo(UpperTriMatrixViewOf(*this,UnitDiag));
	  diag().SetAllTo(T(1));
	} else 
	  mcomp.AssignTo(View());
	return *this;
      }

      //
      // Access
      //

      inline VectorView<T> row(size_t i, size_t j1, size_t j2) const 
      { 
	TMVAssert(i<size());
	TMVAssert(j1<=j2);
	TMVAssert(j2<=size());
	TMVAssert(j1==j2 || this->okij(i,j1));
	return VectorView<T>(
	    ptr()+i*stepi()+j1*stepj(),j2-j1,stepj(),ct() FIRSTLAST); 
      }

      inline VectorView<T> col(size_t j, size_t i1, size_t i2) const
      {
	TMVAssert(j<size());
	TMVAssert(i1<=i2);
	TMVAssert(i2<=size());
	TMVAssert(i1==i2 || this->okij(i2-1,j));
	return VectorView<T>(
	    ptr()+i1*stepi()+j*stepj(),i2-i1,stepi(),ct() FIRSTLAST); 
      }

      inline VectorView<T> diag() const
      {
	TMVAssert(!isunit());
	return VectorView<T>(ptr(),size(),stepi()+stepj(),ct() FIRSTLAST); 
      }

      inline VectorView<T> diag(int i) const
      {
	TMVAssert(i<=int(size())); 
	TMVAssert(isunit() ? i>0 : i>=0);
	return VectorView<T>(ptr()+i*stepj(),size()-i,(stepi()+stepj()),ct() 
	    FIRSTLAST);
      }

      inline VectorView<T> diag(int i, size_t j1, size_t j2) const
      {
	TMVAssert(isunit() ? i>0 : i>=0);
	TMVAssert(j1 <= j2);
	TMVAssert(j2 <= size()+i);
	const int ds = stepi()+stepj();
	return VectorView<T>(ptr()+i*stepj()+j1*ds,j2-j1,ds,ct() FIRSTLAST);
      }

      inline RefType(T) operator()(size_t i,size_t j) const 
      { return ref(i,j); }

      //
      // Modifying Functions
      //

      inline const UpperTriMatrixView<T>& Zero() const 
      { return SetAllTo(T(0)); }
      const UpperTriMatrixView<T>& SetAllTo(T x) const;
      const UpperTriMatrixView<T>& Clip(RealType(T) thresh) const;
      const UpperTriMatrixView<T>& ConjugateSelf() const;
      const UpperTriMatrixView<T>& SetToIdentity(T x=T(1)) const;

      //
      // SubMatrix
      //

      inline MatrixView<T> SubMatrix(int i1, int i2, int j1, int j2) const
      {
	TMVAssert(this->OKSubMatrix(i1,i2,j1,j2,1,1));
	return MatrixView<T>(ptr()+i1*stepi()+j1*stepj(),
	    i2-i1, j2-j1, stepi(),stepj(),stor(),ct() FIRSTLAST );
      }

      inline MatrixView<T> SubMatrix(int i1, int i2, int j1, int j2,
	  int istep, int jstep) const
      {
	const StorageType newstor =
	  iscm() ? (istep == 1 ? ColMajor : NoMajor) :
	  isrm() ? (jstep == 1 ? RowMajor : NoMajor) : NoMajor;
	TMVAssert(this->OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	return MatrixView<T>(ptr()+i1*stepi()+j1*stepj(),
	    (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
	    newstor,ct() FIRSTLAST );
      }

      inline VectorView<T> SubVector(size_t i, size_t j,
	  int istep, int jstep, size_t size) const
      {
	TMVAssert(this->OKSubVector(i,j,istep,jstep,size));
	return VectorView<T>(ptr()+i*stepi()+j*stepj(),size,
	    istep*stepi()+jstep*stepj(),ct() FIRSTLAST );
      }

      inline UpperTriMatrixView<T> SubTriMatrix(int i1, int i2) const
      {
	TMVAssert(this->OKSubTriMatrix(i1,i2,1));
	return UpperTriMatrixView<T>(ptr()+i1*(stepi()+stepj()),i2-i1,
	    stepi(),stepj(),dt(),stor(),ct() FIRSTLAST);
      }

      inline UpperTriMatrixView<T> SubTriMatrix(int i1, int i2,
	  int istep) const
      {
	TMVAssert(this->OKSubTriMatrix(i1,i2,istep));
	return UpperTriMatrixView<T>(ptr()+i1*(stepi()+stepj()),
	    (i2-i1)/istep,istep*stepi(),istep*stepj(),dt(),
	    istep==1 ? stor() : NoMajor,ct() FIRSTLAST);
      }

      inline UpperTriMatrixView<T> OffDiag() const
      {
	TMVAssert(size() > 0);
	return UpperTriMatrixView<T>(ptr()+stepj(),size()-1,
	    stepi(),stepj(),NonUnitDiag,stor(),ct() FIRSTLAST);
      }

      inline UpperTriMatrixView<T> MakeUnitDiag() const
      { 
	return UpperTriMatrixView<T>(ptr(),size(),
	    stepi(),stepj(),UnitDiag,stor(),ct() FIRSTLAST);
      }

      inline UpperTriMatrixView<T> View() const
      { return *this; }

      inline LowerTriMatrixView<T> Transpose() const
      {
	return LowerTriMatrixView<T>(ptr(),size(),
	    stepj(),stepi(),dt(),TransOf(stor()),ct() FIRSTLAST);
      }

      inline UpperTriMatrixView<T> Conjugate() const
      {
	return UpperTriMatrixView<T>(ptr(),size(),
	    stepi(),stepj(),dt(),stor(),ConjOf(T,ct()) FIRSTLAST);
      }

      inline LowerTriMatrixView<T> Adjoint() const
      {
	return LowerTriMatrixView<T>(ptr(),size(),
	    stepj(),stepi(),dt(),TransOf(stor()),ConjOf(T,ct()) 
	    FIRSTLAST);
      }

      inline UpperTriMatrixView<RealType(T)> Real() const
      {
	return UpperTriMatrixView<RealType(T)>(
	    reinterpret_cast<RealType(T)*>(ptr()),size(),
	    IsReal(T()) ? stepi() : 2*stepi(),
	    IsReal(T()) ? stepj() : 2*stepj(),
	    dt(), IsReal(T()) ? stor() : NoMajor, NonConj
#ifdef TMVFLDEBUG
	    ,reinterpret_cast<const RealType(T)*>(first)
	    ,reinterpret_cast<const RealType(T)*>(last)
#endif
	    );
      }

      inline UpperTriMatrixView<RealType(T)> Imag() const
      {
	TMVAssert(IsComplex(T()));
	return UpperTriMatrixView<RealType(T)>(
	    reinterpret_cast<RealType(T)*>(ptr())+1,
	    size(),2*stepi(),2*stepj(),dt(),NoMajor, NonConj
#ifdef TMVFLDEBUG
	    ,reinterpret_cast<const RealType(T)*>(first)+1
	    ,reinterpret_cast<const RealType(T)*>(last)+1
#endif
	    );
      }

      //
      // I/O
      //

      void Read(istream& is) const;

      inline size_t size() const { return itss; }
      inline const T* cptr() const { return itsm; }
      inline T* ptr() const { return itsm; }
      inline int stepi() const { return itssi; }
      inline int stepj() const { return itssj; }
      using GenUpperTriMatrix<T>::dt;
      using GenUpperTriMatrix<T>::stor;
      using GenUpperTriMatrix<T>::ct;
      using GenUpperTriMatrix<T>::isconj;
      using GenUpperTriMatrix<T>::isrm;
      using GenUpperTriMatrix<T>::iscm;
      using GenUpperTriMatrix<T>::isunit;

      // This makes it easier for some things to compile
      // The ones that work are overridden
      template <class T2> void operator=(
	  const BaseMatrix<T2>&) const { TMVAssert(false); }
      template <class T2> void operator+=(
	  const BaseMatrix<T2>&) const { TMVAssert(false); }
      template <class T2> void operator-=(
	  const BaseMatrix<T2>&) const { TMVAssert(false); }
      template <class T2> void operator*=(
	  const BaseMatrix<T2>&) const { TMVAssert(false); }
      template <class T2> void operator/=(
	  const BaseMatrix<T2>&) const { TMVAssert(false); }
      template <class T2> void operator%=(
	  const BaseMatrix<T2>&) const { TMVAssert(false); }

    protected :

      T*const itsm;
      const size_t itss;
      const int itssi;
      const int itssj;

#ifdef TMVFLDEBUG
    public:
      const T*const first;
      const T*const last;
    protected:
#endif

      using GenUpperTriMatrix<T>::okij;
      RefType(T) ref(size_t i, size_t j) const;

  }; // UpperTriMatrixView

  template <class T> class LowerTriMatrixView : 
    public GenLowerTriMatrix<T>
  {

    public:

      //
      // Constructors
      //

      LowerTriMatrixView(const LowerTriMatrixView<T>& rhs) : 
	GenLowerTriMatrix<T>(rhs), itsm(rhs.itsm), itss(rhs.itss), 
	itssi(rhs.itssi), itssj(rhs.itssj)
	  DEFFIRSTLAST(rhs.first,rhs.last) {}

      LowerTriMatrixView(const LowerTriMatrixView<T>& rhs, DiagType d) : 
	GenLowerTriMatrix<T>(d,rhs.stor(),rhs.ct()), itsm(rhs.itsm), 
	itss(rhs.itss), itssi(rhs.itssi), itssj(rhs.itssj)
	  DEFFIRSTLAST(rhs.first,rhs.last) 
      { TMVAssert(d!=NonUnitDiag || !rhs.isunit()); }

      LowerTriMatrixView(T* _m, size_t _s, int _si, int _sj,
	  DiagType indt, StorageType instor, ConjItType inct 
	  PARAMFIRSTLAST(T) ) :
	GenLowerTriMatrix<T>(indt,instor,inct), itsm(_m), itss(_s),
	itssi(_si), itssj(_sj) DEFFIRSTLAST(_first,_last)
      { 
	TMVAssert(instor==RowMajor ? _sj==1 : instor==ColMajor ?
	  _si==1 : true); 
      }

      LowerTriMatrixView(const MatrixView<T>& rhs, DiagType dt) : 
	GenLowerTriMatrix<T>(dt,rhs.stor(),rhs.ct()),
	itsm(rhs.ptr()), itss(rhs.colsize()), 
	itssi(rhs.stepi()), itssj(rhs.stepj())
	  DEFFIRSTLAST(rhs.first,rhs.last) 
      { TMVAssert(rhs.IsSquare()); }

      ~LowerTriMatrixView() {} 

      //
      // Op=
      //

      inline const LowerTriMatrixView<T>& operator=(
	  const LowerTriMatrixView<T>& m2) const
      { Transpose() = m2.Transpose(); return *this; }

      inline const LowerTriMatrixView<T>& operator=(
	  const GenLowerTriMatrix<T>& m2) const
      { Transpose() = m2.Transpose(); return *this; }

      template <class T2> inline const LowerTriMatrixView<T>& operator=(
	  const GenLowerTriMatrix<T2>& m2) const
      { Transpose() = m2.Transpose(); return *this; }

      inline const LowerTriMatrixView<T>& operator=(T x) const 
      { TMVAssert(!isunit()); return SetToIdentity(x); }

      inline const LowerTriMatrixView<T>& operator=(
	  const LowerTriMatrixComposite<T>& mcomp) const
      { 
	TMVAssert(size() == mcomp.size());
	TMVAssert(!(mcomp.dt()==NonUnitDiag && dt()==UnitDiag));
	if (mcomp.dt()==UnitDiag && dt()==NonUnitDiag) {
	  mcomp.AssignTo(LowerTriMatrixViewOf(*this,UnitDiag));
	  diag().SetAllTo(T(1));
	} else 
	  mcomp.AssignTo(View());
	return *this;
      }

      //
      // Access
      //

      inline VectorView<T> row(size_t i, size_t j1, size_t j2) const 
      { 
	TMVAssert(i<size());
	TMVAssert(j1<=j2);
	TMVAssert(j2<=size());
	TMVAssert(j1==j2 || okij(i,j2-1));
	return VectorView<T>(ptr()+i*stepi()+j1*stepj(),j2-j1,stepj(),ct() 
	    FIRSTLAST); 
      }

      inline VectorView<T> col(size_t j, size_t i1, size_t i2) const
      {
	TMVAssert(j<size());
	TMVAssert(i1<=i2);
	TMVAssert(i2<=size());
	TMVAssert(i1==i2 || okij(i1,j));
	return VectorView<T>(ptr()+i1*stepi()+j*stepj(),i2-i1,stepi(),ct() 
	    FIRSTLAST); 
      }

      inline VectorView<T> diag() const
      {
	TMVAssert(!isunit());
	return VectorView<T>(ptr(),size(),stepi()+stepj(),ct() FIRSTLAST); 
      }

      inline VectorView<T> diag(int i) const
      {
	TMVAssert(-i<=int(size())); 
	TMVAssert(isunit() ? i<0 : i<=0);
	return VectorView<T>(ptr()-i*stepi(),size()+i,stepi()+stepj(),ct() 
	    FIRSTLAST);
      }

      inline VectorView<T> diag(int i, size_t j1, size_t j2) const
      {
	TMVAssert(isunit() ? i<0 : i<=0);
	TMVAssert(j1 <= j2);
	TMVAssert(j2 <= size()-i);
	const int ds = stepi()+stepj();
	return VectorView<T>(ptr()-i*stepi()+j1*ds,j2-j1,ds,ct() FIRSTLAST);
      }

      inline RefType(T) operator()(size_t i,size_t j) const 
      { return ref(i,j); }

      //
      // Modifying Functions
      //

      inline const LowerTriMatrixView<T>& Zero() const 
      { return SetAllTo(T(0)); }
      const LowerTriMatrixView<T>& SetAllTo(T x) const
      { Transpose().SetAllTo(x); return *this; }
      const LowerTriMatrixView<T>& Clip(RealType(T) thresh) const
      { Transpose().Clip(thresh); return *this; }
      const LowerTriMatrixView<T>& ConjugateSelf() const
      { Transpose().ConjugateSelf(); return *this; }
      const LowerTriMatrixView<T>& SetToIdentity(T x=T(1)) const
      { Transpose().SetToIdentity(x); return *this; }

      //
      // SubMatrix
      //

      inline MatrixView<T> SubMatrix(int i1, int i2, int j1, int j2) const
      {
	TMVAssert(this->OKSubMatrix(i1,i2,j1,j2,1,1));
	return MatrixView<T>(ptr()+i1*stepi()+j1*stepj(),
	    i2-i1, j2-j1, stepi(),stepj(),stor(),ct() FIRSTLAST );
      }

      inline MatrixView<T> SubMatrix(int i1, int i2, int j1, int j2,
	  int istep, int jstep) const
      {
	const StorageType newstor =
	  iscm() ? (istep == 1 ? ColMajor : NoMajor) :
	  isrm() ? (jstep == 1 ? RowMajor : NoMajor) : NoMajor;
	TMVAssert(this->OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	return MatrixView<T>(ptr()+i1*stepi()+j1*stepj(),
	    (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
	    newstor,ct() FIRSTLAST );
      }

      inline VectorView<T> SubVector(size_t i, size_t j,
	  int istep, int jstep, size_t size) const
      {
	TMVAssert(this->OKSubVector(i,j,istep,jstep,size));
	return VectorView<T>(ptr()+i*stepi()+j*stepj(),size,
	    istep*stepi()+jstep*stepj(),ct() FIRSTLAST );
      }

      inline LowerTriMatrixView<T> SubTriMatrix(int i1, int i2) const
      {
	TMVAssert(this->OKSubTriMatrix(i1,i2,1));
	return LowerTriMatrixView<T>(ptr()+i1*(stepi()+stepj()),i2-i1,
	    stepi(),stepj(),dt(),stor(),ct() FIRSTLAST);
      }

      inline LowerTriMatrixView<T> SubTriMatrix(int i1, int i2,
	  int istep) const
      {
	TMVAssert(this->OKSubTriMatrix(i1,i2,istep));
	return LowerTriMatrixView<T>(ptr()+i1*(stepi()+stepj()),
	    (i2-i1)/istep,istep*stepi(),istep*stepj(),dt(),
	    istep==1 ? stor() : NoMajor,ct() FIRSTLAST);
      }

      inline LowerTriMatrixView<T> OffDiag() const
      {
	TMVAssert(size() > 0);
	return LowerTriMatrixView<T>(ptr()+stepi(),size()-1,
	    stepi(),stepj(),NonUnitDiag,stor(),ct() FIRSTLAST);
      }

      inline LowerTriMatrixView<T> MakeUnitDiag() const
      { 
	return LowerTriMatrixView<T>(ptr(),size(),
	    stepi(),stepj(),UnitDiag,stor(),ct() FIRSTLAST);
      }

      inline LowerTriMatrixView<T> View() const
      { return *this; }

      inline UpperTriMatrixView<T> Transpose() const
      {
	return UpperTriMatrixView<T>(ptr(),size(),
	    stepj(),stepi(),dt(),TransOf(stor()),ct() FIRSTLAST);
      }

      inline LowerTriMatrixView<T> Conjugate() const
      {
	return LowerTriMatrixView<T>(ptr(),size(),
	    stepi(),stepj(),dt(),stor(),ConjOf(T,ct()) FIRSTLAST);
      }

      inline UpperTriMatrixView<T> Adjoint() const
      {
	return UpperTriMatrixView<T>(ptr(),size(),
	    stepj(),stepi(),dt(),TransOf(stor()),ConjOf(T,ct()) 
	    FIRSTLAST);
      }

      inline LowerTriMatrixView<RealType(T)> Real() const
      {
	return LowerTriMatrixView<RealType(T)>(
	    reinterpret_cast<RealType(T)*>(ptr()),size(),
	    IsReal(T()) ? stepi() : 2*stepi(),
	    IsReal(T()) ? stepj() : 2*stepj(),
	    dt(), IsReal(T()) ? stor() : NoMajor,NonConj
#ifdef TMVFLDEBUG
	    ,reinterpret_cast<const RealType(T)*>(first)
	    ,reinterpret_cast<const RealType(T)*>(last)
#endif
	    );
      }

      inline LowerTriMatrixView<RealType(T)> Imag() const
      {
	TMVAssert(IsComplex(T()));
	return LowerTriMatrixView<RealType(T)>(
	    reinterpret_cast<RealType(T)*>(ptr())+1,
	    size(),2*stepi(),2*stepj(),dt(),NoMajor,NonConj
#ifdef TMVFLDEBUG
	    ,reinterpret_cast<const RealType(T)*>(first)+1
	    ,reinterpret_cast<const RealType(T)*>(last)+1
#endif
	    );
      }

      //
      // I/O
      //

      void Read(istream& is) const;

      inline size_t size() const { return itss; }
      inline const T* cptr() const { return itsm; }
      inline T* ptr() const { return itsm; }
      inline int stepi() const { return itssi; }
      inline int stepj() const { return itssj; }
      using GenLowerTriMatrix<T>::dt;
      using GenLowerTriMatrix<T>::stor;
      using GenLowerTriMatrix<T>::ct;
      using GenLowerTriMatrix<T>::isconj;
      using GenLowerTriMatrix<T>::isrm;
      using GenLowerTriMatrix<T>::iscm;
      using GenLowerTriMatrix<T>::isunit;

      // This makes it easier for some things to compile
      // The ones that work are overridden
      template <class T2> void operator=(
	  const BaseMatrix<T2>&) const { TMVAssert(false); }
      template <class T2> void operator+=(
	  const BaseMatrix<T2>&) const { TMVAssert(false); }
      template <class T2> void operator-=(
	  const BaseMatrix<T2>&) const { TMVAssert(false); }
      template <class T2> void operator*=(
	  const BaseMatrix<T2>&) const { TMVAssert(false); }
      template <class T2> void operator/=(
	  const BaseMatrix<T2>&) const { TMVAssert(false); }
      template <class T2> void operator%=(
	  const BaseMatrix<T2>&) const { TMVAssert(false); }

    protected :

      T*const itsm;
      const size_t itss;
      const int itssi;
      const int itssj;

#ifdef TMVFLDEBUG
    public:
      const T*const first;
      const T*const last;
    protected:
#endif

      using GenLowerTriMatrix<T>::okij;
      RefType(T) ref(size_t i, size_t j) const;

  }; // LowerTriMatrixView

  template <class T, DiagType D, StorageType S> class UpperTriMatrix : 
    public GenUpperTriMatrix<T>
  {

    public:

      //
      // Constructors
      //

#define NEW_SIZE(s) GenUpperTriMatrix<T>(D,S,NonConj), \
      itslen((s)*(s)), itsm(new T[itslen]), itss(s) \
      DEFFIRSTLAST(itsm,itsm+itslen)

      explicit UpperTriMatrix(size_t _size) : NEW_SIZE(_size) 
      { 
	TMVAssert(S==RowMajor || S==ColMajor); 
#ifdef TMVDEBUG
	SetAllTo(T(888));
#endif
      }

      UpperTriMatrix(size_t _size, T x) : NEW_SIZE(_size) 
      { 
	TMVAssert(S==RowMajor || S==ColMajor); 
	SetAllTo(x);
      }

      UpperTriMatrix(const UpperTriMatrix<T,D,S>& rhs) :
	GenUpperTriMatrix<T>(D,S,NonConj), 
	itslen(rhs.itslen), itsm(new T[itslen]), itss(rhs.itss) 
	  DEFFIRSTLAST(itsm,itsm+itslen)
      {
	TMVAssert(S==RowMajor || S==ColMajor); 
	memmove(itsm,rhs.itsm,itslen*sizeof(T));
      }

      template <DiagType D2> UpperTriMatrix(
	  const UpperTriMatrix<T,D2,S>& rhs) : NEW_SIZE(rhs.size())
      {
	TMVAssert(S==RowMajor || S==ColMajor); 
	memmove(itsm,rhs.cptr(),itslen*sizeof(T));
	if (D==NonUnitDiag && D2==UnitDiag) diag().SetAllTo(T(1));
      }

      template <DiagType D2, StorageType S2> UpperTriMatrix(
	  const UpperTriMatrix<T,D2,S2>& rhs) : NEW_SIZE(rhs.size())
      {
	TMVAssert(S==RowMajor || S==ColMajor); 
	if (D==D2) Copy(rhs,View());
	else {
	  if (size() > 0) Copy(rhs.OffDiag(),OffDiag());
	  if (D==NonUnitDiag && D2==UnitDiag) diag().SetAllTo(T(1));
	}
      }

      template <StorageType S2> UpperTriMatrix(const Matrix<T,S2>& rhs) :
	NEW_SIZE(rhs.rowsize())
      {
	TMVAssert(S==RowMajor || S==ColMajor); 
	if (S==S2 && rhs.IsSquare()) 
	  memmove(itsm,rhs.cptr(),itslen*sizeof(T));
	else 
	  Copy(UpperTriMatrixViewOf(rhs,D),View());
      }

      template <class T2> UpperTriMatrix(const GenMatrix<T2>& rhs) :
	NEW_SIZE(rhs.rowsize())
      { 
	TMVAssert(S==RowMajor || S==ColMajor); 
	Copy(UpperTriMatrixViewOf(rhs,D),View()); 
      }

      template <class T2> UpperTriMatrix(const GenUpperTriMatrix<T2>& rhs) :
	NEW_SIZE(rhs.size())
      { 
	TMVAssert(S==RowMajor || S==ColMajor); 
	if (D==rhs.dt()) Copy(rhs,View());
	else {
	  if (size() > 0) Copy(rhs.OffDiag(),OffDiag());
	  if (D==NonUnitDiag && rhs.dt()==UnitDiag) diag().SetAllTo(T(1));
	}
      }

      UpperTriMatrix(const UpperTriMatrixComposite<T>& mcomp) :
	NEW_SIZE(mcomp.size())
      {
	TMVAssert(S==RowMajor || S==ColMajor); 
	TMVAssert(!(mcomp.dt()==NonUnitDiag && D==UnitDiag));
	if (mcomp.dt()==UnitDiag && D==NonUnitDiag) {
	  mcomp.AssignTo(UpperTriMatrixViewOf(*this,UnitDiag));
	  diag().SetAllTo(T(1));
	} else 
	  mcomp.AssignTo(View());
      }

#undef NEW_SIZE

      ~UpperTriMatrix() { TMVAssert(itsm); delete[] itsm; }


      //
      // Op=
      //

      inline UpperTriMatrix<T,D,S>& operator=(const UpperTriMatrix<T,D,S>& m2)
      { 
	if (&m2 != this) memmove(itsm,m2.itsm,itslen*sizeof(T));
	return *this;
      }

      template <class T2> inline UpperTriMatrix<T,D,S>& operator=(
	  const GenUpperTriMatrix<T2>& m2)
      { 
	TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
	if (m2.dt()==UnitDiag && D==NonUnitDiag) {
	  if (size() > 0) Copy(m2.OffDiag(),OffDiag());
	  diag().SetAllTo(T(1));
	} else 
	  Copy(m2,View());
	return *this;
      }

      inline UpperTriMatrix<T,D,S>& operator=(T x) 
      { 
	TMVAssert(!this->isunit() || x==T(1));
	return SetToIdentity(x); 
      }

      inline UpperTriMatrix<T,D,S>& operator=(
	  const UpperTriMatrixComposite<T>& mcomp)
      { 
	TMVAssert(size() == mcomp.size());
	TMVAssert(!(mcomp.dt()==NonUnitDiag && D==UnitDiag));
	if (mcomp.dt()==UnitDiag && D==NonUnitDiag) {
	  mcomp.AssignTo(UpperTriMatrixViewOf(*this,UnitDiag));
	  diag().SetAllTo(T(1));
	} else 
	  mcomp.AssignTo(View());
	return *this;
      }

      //
      // Access
      //

      inline ConstVectorView<T> row(size_t i, size_t j1, size_t j2) const 
      { 
	TMVAssert(i<size());
	TMVAssert(j1<=j2);
	TMVAssert(j2<=size());
	TMVAssert(j1==j2 || okij(i,j1));
	return ConstVectorView<T>(cptr()+i*stepi()+j1*stepj(),j2-j1,stepj(),
	    NonConj);
      }

      inline ConstVectorView<T> col(size_t j, size_t i1, size_t i2) const
      {
	TMVAssert(j<size());
	TMVAssert(i1<=i2);
	TMVAssert(i2<=size());
	TMVAssert(i1==i2 || okij(i2-1,j));
	return ConstVectorView<T>(cptr()+i1*stepi()+j*stepj(),i2-i1,stepi(),
	    NonConj);
      }

      inline ConstVectorView<T> diag() const
      {
	TMVAssert(!isunit());
	return ConstVectorView<T>(cptr(),size(),stepi()+stepj(),NonConj); 
      }

      inline ConstVectorView<T> diag(int i) const
      {
	TMVAssert(i<=int(size())); 
	TMVAssert(isunit() ? i>0 : i>=0);
	return ConstVectorView<T>(cptr()+i*stepj(),size()-i,stepi()+stepj(),
	    NonConj);
      }

      inline ConstVectorView<T> diag(int i, size_t j1, size_t j2) const
      {
	TMVAssert(isunit() ? i>0 : i>=0);
	TMVAssert(j1 <= j2);
	TMVAssert(j2 <= size()+i);
	const int ds = stepi()+stepj();
	return ConstVectorView<T>(cptr()+i*stepj()+j1*ds,j2-j1,ds,NonConj);
      }

      inline VectorView<T> row(size_t i, size_t j1, size_t j2)
      { 
	TMVAssert(i<size());
	TMVAssert(j1<=j2);
	TMVAssert(j2<=size());
	TMVAssert(j1==j2 || okij(i,j1));
	return VectorView<T>(ptr()+i*stepi()+j1*stepj(),j2-j1,stepj(),NonConj 
	    FIRSTLAST);
      }

      inline VectorView<T> col(size_t j, size_t i1, size_t i2)
      {
	TMVAssert(j<size());
	TMVAssert(i1<=i2);
	TMVAssert(i2<=size());
	TMVAssert(i1==i2 || okij(i2-1,j));
	return VectorView<T>(ptr()+i1*stepi()+j*stepj(),i2-i1,stepi(),NonConj
	    FIRSTLAST);
      }

      inline VectorView<T> diag()
      {
	TMVAssert(!isunit());
	return VectorView<T>(ptr(),size(),stepi()+stepj(),NonConj FIRSTLAST); 
      }

      inline VectorView<T> diag(int i)
      {
	TMVAssert(i<=int(size())); 
	TMVAssert(isunit() ? i>0 : i>=0);
	return VectorView<T>(ptr()+i*stepj(),size()-i,stepi()+stepj(),NonConj 
	    FIRSTLAST);
      }

      inline VectorView<T> diag(int i, size_t j1, size_t j2)
      {
	TMVAssert(isunit() ? i>0 : i>=0);
	TMVAssert(j1 <= j2);
	TMVAssert(j2 <= size()+i);
	const int ds = stepi()+stepj();
	return VectorView<T>(ptr()+i*stepj()+j1*ds,j2-j1,ds,NonConj FIRSTLAST);
      }

      inline T operator()(size_t i, size_t j) const
      { return cref(i,j); }

      inline T& operator()(size_t i, size_t j) 
      { return ref(i,j); }

      //
      // Modifying Functions
      //

      inline UpperTriMatrix<T,D,S>& Zero() { return SetAllTo(0); }
      inline UpperTriMatrix<T,D,S>& SetAllTo(T x) 
      { VectorViewOf(itsm,itslen).SetAllTo(x); return *this; }
      inline UpperTriMatrix<T,D,S>& Clip(RealType(T) thresh)
      { VectorViewOf(itsm,itslen).Clip(thresh); return *this; }
      inline UpperTriMatrix<T,D,S>& ConjugateSelf() 
      { VectorViewOf(itsm,itslen).ConjugateSelf(); return *this; }
      inline UpperTriMatrix<T,D,S>& SetToIdentity(T x=T(1)) 
      {
	TMVAssert(!isunit() || x==T(1));
	Zero(); if (!isunit()) diag().SetAllTo(x);
	return *this;
      }

      //
      // SubMatrix
      //

      inline ConstMatrixView<T> SubMatrix(int i1, int i2, int j1, int j2) const
      {
	TMVAssert(this->OKSubMatrix(i1,i2,j1,j2,1,1));
	return ConstMatrixView<T>(cptr()+i1*stepi()+j1*stepj(),
	    i2-i1, j2-j1,stepi(),stepj(),S,NonConj);
      }

      inline ConstMatrixView<T> SubMatrix(int i1, int i2, int j1, int j2,
	  int istep, int jstep) const
      {
	StorageType newstor = S==RowMajor ?
	  jstep == 1 ? RowMajor : NoMajor :
	  istep == 1 ? ColMajor : NoMajor;
	TMVAssert(this->OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	return ConstMatrixView<T>(cptr()+i1*stepi()+j1*stepj(),
	    (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
	    newstor, NonConj);
      }

      inline ConstVectorView<T> SubVector(size_t i, size_t j,
	  int istep, int jstep, size_t size) const
      {
	TMVAssert(this->OKSubVector(i,j,istep,jstep,size));
	return ConstVectorView<T>(cptr()+i*stepi()+j*stepj(),size,
	    istep*stepi()+jstep*stepj(),NonConj);
      }

      inline ConstUpperTriMatrixView<T> SubTriMatrix(int i1, int i2) const
      {
	TMVAssert(this->OKSubTriMatrix(i1,i2,1));
	return ConstUpperTriMatrixView<T>(cptr()+i1*(stepi()+stepj()),i2-i1,
	    stepi(),stepj(),D,S,NonConj);
      }

      inline ConstUpperTriMatrixView<T> SubTriMatrix(int i1, int i2,
	  int istep) const
      {
	TMVAssert(this->OKSubTriMatrix(i1,i2,istep));
	return ConstUpperTriMatrixView<T>(cptr()+i1*(stepi()+stepj()),
	    (i2-i1)/istep, istep*stepi(),istep*stepj(), D,
	    istep==1 ? S : NoMajor, NonConj);
      }

      inline ConstUpperTriMatrixView<T> OffDiag() const
      {
	TMVAssert(size() > 0);
	return ConstUpperTriMatrixView<T>(cptr()+stepj(),size()-1,
	    stepi(),stepj(),NonUnitDiag,S,NonConj);
      }

      inline ConstUpperTriMatrixView<T> MakeUnitDiag() const
      {
	return ConstUpperTriMatrixView<T>(cptr(),size(),
	    stepi(),stepj(),UnitDiag,S,NonConj);
      }

      inline MatrixView<T> SubMatrix(int i1, int i2, int j1, int j2) 
      {
	TMVAssert(this->OKSubMatrix(i1,i2,j1,j2,1,1));
	return MatrixView<T>(ptr()+i1*stepi()+j1*stepj(),
	    i2-i1, j2-j1, stepi(),stepj(),S,NonConj FIRSTLAST );
      }

      inline MatrixView<T> SubMatrix(int i1, int i2, int j1, int j2,
	  int istep, int jstep)
      {
	StorageType newstor = S == RowMajor ?
	  jstep == 1 ? RowMajor : NoMajor :
	  istep == 1 ? ColMajor : NoMajor;
	TMVAssert(this->OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	return MatrixView<T>(ptr()+i1*stepi()+j1*stepj(),
	    (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
	    newstor,NonConj FIRSTLAST );
      }

      inline VectorView<T> SubVector(size_t i, size_t j,
	  int istep, int jstep, size_t size)
      {
	TMVAssert(this->OKSubVector(i,j,istep,jstep,size));
	return VectorView<T>(ptr()+i*stepi()+j*stepj(),size,
	    istep*stepi()+jstep*stepj(),NonConj FIRSTLAST );
      }

      inline UpperTriMatrixView<T> SubTriMatrix(int i1, int i2)
      {
	TMVAssert(this->OKSubTriMatrix(i1,i2,1));
	return UpperTriMatrixView<T>(ptr()+i1*(stepi()+stepj()),i2-i1,
	    stepi(),stepj(),D,S,NonConj FIRSTLAST);
      }

      inline UpperTriMatrixView<T> SubTriMatrix(int i1, int i2, int istep) 
      {
	TMVAssert(this->OKSubTriMatrix(i1,i2,istep));
	return UpperTriMatrixView<T>(ptr()+i1*(stepi()+stepj()),
	    (i2-i1)/istep,istep*stepi(),istep*stepj(),D,
	    istep==1 ? S : NoMajor,NonConj FIRSTLAST);
      }

      inline UpperTriMatrixView<T> OffDiag()
      {
	TMVAssert(size() > 0);
	return UpperTriMatrixView<T>(ptr()+stepj(),size()-1,
	    stepi(),stepj(),NonUnitDiag,S,NonConj FIRSTLAST);
      }

      inline UpperTriMatrixView<T> MakeUnitDiag()
      {
	return UpperTriMatrixView<T>(ptr(),size(),
	    stepi(),stepj(),UnitDiag,S,NonConj FIRSTLAST);
      }

      inline ConstUpperTriMatrixView<T> View() const
      { 
	return ConstUpperTriMatrixView<T>(cptr(),size(),
	    stepi(),stepj(),D,S,NonConj);
      }

      inline ConstLowerTriMatrixView<T> Transpose() const
      { 
	return ConstLowerTriMatrixView<T>(cptr(),size(),
	    stepj(),stepi(),D,TransOf(S),NonConj);
      }

      inline ConstUpperTriMatrixView<T> Conjugate() const
      { 
	return ConstUpperTriMatrixView<T>(cptr(),size(),
	    stepi(),stepj(),D,S,ConjOf(T,NonConj));
      }

      inline ConstLowerTriMatrixView<T> Adjoint() const
      { 
	return ConstLowerTriMatrixView<T>(cptr(),size(),
	    stepj(),stepi(),D,TransOf(S),ConjOf(T,NonConj));
      }

      inline UpperTriMatrixView<T> View() 
      { 
	return UpperTriMatrixView<T>(ptr(),size(),
	    stepi(),stepj(),D,S,NonConj FIRSTLAST);
      }

      inline LowerTriMatrixView<T> Transpose() 
      { 
	return LowerTriMatrixView<T>(ptr(),size(),
	    stepj(),stepi(),D,TransOf(S),NonConj FIRSTLAST);
      }

      inline UpperTriMatrixView<T> Conjugate() 
      { 
	return UpperTriMatrixView<T>(ptr(),size(),
	    stepi(),stepj(),D,S,ConjOf(T,NonConj) FIRSTLAST);
      }

      inline LowerTriMatrixView<T> Adjoint() 
      { 
	return LowerTriMatrixView<T>(ptr(),size(),
	    stepj(),stepi(),D,TransOf(S),ConjOf(T,NonConj) FIRSTLAST);
      }

      inline size_t size() const { return itss; }
      inline const T* cptr() const { return itsm; }
      inline T* ptr() { return itsm; }
      inline int stepi() const { return S==RowMajor ? itss : 1; }
      inline int stepj() const { return S==RowMajor ? 1 : itss; }
      inline DiagType dt() const { return D; }
      inline StorageType stor() const { return S; }
      inline ConjItType ct() const { return NonConj; }
      inline bool isrm() const { return S==RowMajor; }
      inline bool iscm() const { return S==ColMajor; }
      inline bool isunit() const { return D == UnitDiag; }
      inline bool isconj() const { return false; }

    protected :

      const size_t itslen;
      T*const itsm;
      const size_t itss;

#ifdef TMVFLDEBUG
    public :
      const T*const first;
      const T*const last;
    protected :
#endif

      inline bool okij(size_t i, size_t j) const
      {
	TMVAssert(i < size());
	TMVAssert(j < size());
	if (isunit()) return i<j; else return i<=j;
      }

      inline T& ref(size_t i, size_t j)
      {
	TMVAssert(i < size());
	TMVAssert(j < size());
	TMVAssert(okij(i,j));
	return *(ptr() + i*stepi() + j*stepj());
      }

      inline T cref(size_t i, size_t j) const 
      {
	TMVAssert(i < size());
	TMVAssert(j < size());
	return i==j && isunit() ? T(1) : i>j ? T(0) :
	  *(cptr() + i*stepi() + j*stepj());
      }

  }; // UpperTriMatrix - RowMajor

  template <class T, DiagType D, StorageType S> class LowerTriMatrix : 
    public GenLowerTriMatrix<T>
  {

    public:

      //
      // Constructors
      //

#define NEW_SIZE(s) GenLowerTriMatrix<T>(D,S,NonConj), \
      itslen((s)*(s)), itsm(new T[itslen]), itss(s) \
      DEFFIRSTLAST(itsm,itsm+itslen)

      explicit LowerTriMatrix(size_t _size) : NEW_SIZE(_size) 
      { 
	TMVAssert(S==RowMajor || S==ColMajor); 
#ifdef TMVDEBUG
	SetAllTo(T(888));
#endif
      }

      LowerTriMatrix(size_t _size, T x) : NEW_SIZE(_size) 
      { 
	TMVAssert(S==RowMajor || S==ColMajor); 
	SetAllTo(x);
      }

      LowerTriMatrix(const LowerTriMatrix<T,D,S>& rhs) :
	GenLowerTriMatrix<T>(D,S,NonConj), 
	itslen(rhs.itslen), itsm(new T[itslen]), itss(rhs.itss) 
	  DEFFIRSTLAST(itsm,itsm+itslen)
      {
	TMVAssert(S==RowMajor || S==ColMajor); 
	memmove(itsm,rhs.itsm,itslen*sizeof(T));
      }

      template <DiagType D2> LowerTriMatrix(
	  const LowerTriMatrix<T,D2,S>& rhs) : NEW_SIZE(rhs.size())
      {
	TMVAssert(S==RowMajor || S==ColMajor); 
	memmove(itsm,rhs.cptr(),itslen*sizeof(T));
	if (D==NonUnitDiag && D2==UnitDiag) diag().SetAllTo(T(1));
      }

      template <DiagType D2, StorageType S2> LowerTriMatrix(
	  const LowerTriMatrix<T,D2,S2>& rhs) : NEW_SIZE(rhs.size())
      {
	TMVAssert(S==RowMajor || S==ColMajor); 
	if (D==D2) Copy(rhs.Transpose(),Transpose());
	else {
	  if (size() > 0) Copy(rhs.OffDiag().Transpose(),
	      OffDiag().Transpose());
	  if (D==NonUnitDiag && D2==UnitDiag) diag().SetAllTo(T(1));
	}
      }

      template <StorageType S2> LowerTriMatrix(const Matrix<T,S2>& rhs) :
	NEW_SIZE(rhs.colsize())
      {
	TMVAssert(S==RowMajor || S==ColMajor); 
	if (S==S2 && rhs.IsSquare()) 
	  memmove(itsm,rhs.cptr(),itslen*sizeof(T));
	else 
	  Copy(LowerTriMatrixViewOf(rhs,D).Transpose(),Transpose());
      }

      template <class T2> LowerTriMatrix(const GenMatrix<T2>& rhs) :
	NEW_SIZE(rhs.colsize())
      { 
	TMVAssert(S==RowMajor || S==ColMajor); 
	Copy(LowerTriMatrixViewOf(rhs,D).Transpose(),Transpose()); 
      }

      template <class T2> LowerTriMatrix(const GenLowerTriMatrix<T2>& rhs) :
	NEW_SIZE(rhs.size())
      { 
	TMVAssert(S==RowMajor || S==ColMajor); 
	if (D==rhs.dt()) Copy(rhs.Transpose(),Transpose());
	else {
	  if (size() > 0) Copy(rhs.OffDiag().Transpose(),
	      OffDiag().Transpose());
	  if (D==NonUnitDiag && rhs.dt()==UnitDiag) diag().SetAllTo(T(1));
	}
      }

      LowerTriMatrix(const LowerTriMatrixComposite<T>& mcomp) :
	NEW_SIZE(mcomp.size())
      {
	TMVAssert(S==RowMajor || S==ColMajor); 
	TMVAssert(!(mcomp.dt()==NonUnitDiag && D==UnitDiag));
	if (mcomp.dt()==UnitDiag && D==NonUnitDiag) {
	  mcomp.AssignTo(LowerTriMatrixViewOf(*this,UnitDiag));
	  diag().SetAllTo(T(1));
	} else 
	  mcomp.AssignTo(View());
      }

#undef NEW_SIZE

      ~LowerTriMatrix() { TMVAssert(itsm); delete[] itsm; }


      //
      // Op=
      //

      inline LowerTriMatrix<T,D,S>& operator=(const LowerTriMatrix<T,D,S>& m2)
      { 
	if (&m2 != this) memmove(itsm,m2.itsm,itslen*sizeof(T));
	return *this;
      }

      template <class T2> inline LowerTriMatrix<T,D,S>& operator=(
	  const GenLowerTriMatrix<T2>& m2)
      { 
	TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
	if (m2.dt()==UnitDiag && D==NonUnitDiag) {
	  if (size() > 0) Copy(m2.OffDiag().Transpose(),OffDiag().Transpose());
	  diag().SetAllTo(T(1));
	} else Copy(m2.Transpose(),Transpose());
	return *this;
      }

      inline LowerTriMatrix<T,D,S>& operator=(T x) 
      { 
	TMVAssert(!this->isunit() || x==T(1));
	return SetToIdentity(x); 
      }

      inline LowerTriMatrix<T,D,S>& operator=(
	  const LowerTriMatrixComposite<T>& mcomp)
      { 
	TMVAssert(size() == mcomp.size());
	TMVAssert(!(mcomp.dt()==NonUnitDiag && D==UnitDiag));
	if (mcomp.dt()==UnitDiag && D==NonUnitDiag) {
	  mcomp.AssignTo(LowerTriMatrixViewOf(*this,UnitDiag));
	  diag().SetAllTo(T(1));
	} else mcomp.AssignTo(View());
	return *this;
      }

      //
      // Access
      //

      inline ConstVectorView<T> row(size_t i, size_t j1, size_t j2) const 
      { 
	TMVAssert(i<size());
	TMVAssert(j1<=j2);
	TMVAssert(j2<=size());
	TMVAssert(j1==j2 || okij(i,j2-1));
	return ConstVectorView<T>(cptr()+i*stepi()+j1*stepj(),j2-j1,stepj(),
	    NonConj);
      }

      inline ConstVectorView<T> col(size_t j, size_t i1, size_t i2) const
      {
	TMVAssert(j<size());
	TMVAssert(i1<=i2);
	TMVAssert(i2<=size());
	TMVAssert(i1==i2 || okij(i1,j));
	return ConstVectorView<T>(cptr()+i1*stepi()+j*stepj(),i2-i1,stepi(),
	    NonConj);
      }

      inline ConstVectorView<T> diag() const
      {
	TMVAssert(!isunit());
	return ConstVectorView<T>(cptr(),size(),stepi()+stepj(),NonConj); 
      }

      inline ConstVectorView<T> diag(int i) const
      {
	TMVAssert(-i<=int(size())); 
	TMVAssert(isunit() ? i<0 : i<=0);
	return ConstVectorView<T>(cptr()-i*stepi(),size()+i,stepi()+stepj(),
	    NonConj);
      }

      inline ConstVectorView<T> diag(int i, size_t j1, size_t j2) const
      {
	TMVAssert(isunit() ? i<0 : i<=0);
	TMVAssert(j1 <= j2);
	TMVAssert(j2 <= size()-i);
	const int ds = stepi()+stepj();
	return ConstVectorView<T>(cptr()-i*stepi()+j1*ds,j2-j1,ds,NonConj);
      }

      inline VectorView<T> row(size_t i, size_t j1, size_t j2)
      { 
	TMVAssert(i<size());
	TMVAssert(j1<=j2);
	TMVAssert(j2<=size());
	TMVAssert(j1==j2 || okij(i,j2-1));
	return VectorView<T>(ptr()+i*stepi()+j1*stepj(),j2-j1,stepj(),NonConj
	    FIRSTLAST);
      }

      inline VectorView<T> col(size_t j, size_t i1, size_t i2)
      {
	TMVAssert(j<size());
	TMVAssert(i1<=i2);
	TMVAssert(i2<=size());
	TMVAssert(i1==i2 || okij(i1,j));
	return VectorView<T>(ptr()+i1*stepi()+j*stepj(),i2-i1,stepi(),NonConj
	    FIRSTLAST);
      }

      inline VectorView<T> diag()
      {
	TMVAssert(!isunit());
	return VectorView<T>(ptr(),size(),stepi()+stepj(),NonConj FIRSTLAST); 
      }

      inline VectorView<T> diag(int i)
      {
	TMVAssert(i<=int(size())); 
	TMVAssert(isunit() ? i<0 : i<=0);
	return VectorView<T>(ptr()-i*stepi(),size()+i,stepi()+stepj(),NonConj 
	    FIRSTLAST);
      }

      inline VectorView<T> diag(int i, size_t j1, size_t j2)
      {
	TMVAssert(isunit() ? i<0 : i<=0);
	TMVAssert(j1 <= j2);
	TMVAssert(j2 <= size()-i);
	const int ds = stepi()+stepj();
	return VectorView<T>(ptr()-i*stepi()+j1*ds,j2-j1,ds,NonConj FIRSTLAST);
      }

      inline T operator()(size_t i, size_t j) const
      { return cref(i,j); }

      inline T& operator()(size_t i, size_t j) 
      { return ref(i,j); }

      //
      // Modifying Functions
      //

      inline LowerTriMatrix<T,D,S>& Zero() { return SetAllTo(0); }
      inline LowerTriMatrix<T,D,S>& SetAllTo(T x) 
      { VectorViewOf(itsm,itslen).SetAllTo(x); return *this; }
      inline LowerTriMatrix<T,D,S>& Clip(RealType(T) thresh)
      { VectorViewOf(itsm,itslen).Clip(thresh); return *this; }
      inline LowerTriMatrix<T,D,S>& ConjugateSelf() 
      { VectorViewOf(itsm,itslen).ConjugateSelf(); return *this; }
      inline LowerTriMatrix<T,D,S>& SetToIdentity(T x=T(1)) 
      { 
	TMVAssert(!isunit() || x == T(1));
	Zero(); if (!isunit()) diag().SetAllTo(x);
	return *this;
      }

      //
      // SubMatrix
      //

      inline ConstMatrixView<T> SubMatrix(int i1, int i2, int j1, int j2) const
      {
	TMVAssert(this->OKSubMatrix(i1,i2,j1,j2,1,1));
	return ConstMatrixView<T>(cptr()+i1*stepi()+j1*stepj(),
	    i2-i1, j2-j1,stepi(),stepj(),S,NonConj);
      }

      inline ConstMatrixView<T> SubMatrix(int i1, int i2, int j1, int j2,
	  int istep, int jstep) const
      {
	StorageType newstor = S==RowMajor ?
	  jstep == 1 ? RowMajor : NoMajor :
	  istep == 1 ? ColMajor : NoMajor;
	TMVAssert(this->OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	return ConstMatrixView<T>(cptr()+i1*stepi()+j1*stepj(),
	    (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
	    newstor,NonConj);
      }

      inline ConstVectorView<T> SubVector(size_t i, size_t j,
	  int istep, int jstep, size_t size) const
      {
	TMVAssert(this->OKSubVector(i,j,istep,jstep,size));
	return ConstVectorView<T>(cptr()+i*stepi()+j*stepj(),size,
	    istep*stepi()+jstep*stepj(),NonConj);
      }

      inline ConstLowerTriMatrixView<T> SubTriMatrix(int i1, int i2) const
      {
	TMVAssert(this->OKSubTriMatrix(i1,i2,1));
	return ConstLowerTriMatrixView<T>(cptr()+i1*(stepi()+stepj()),i2-i1,
	    stepi(),stepj(),D,S,NonConj);
      }

      inline ConstLowerTriMatrixView<T> SubTriMatrix(int i1, int i2,
	  int istep) const
      {
	TMVAssert(this->OKSubTriMatrix(i1,i2,istep));
	return ConstLowerTriMatrixView<T>(cptr()+i1*(stepi()+stepj()),
	    (i2-i1)/istep,istep*stepi(),istep*stepj(),D,
	    istep==1 ? S : NoMajor, NonConj);
      }

      inline ConstLowerTriMatrixView<T> OffDiag() const
      {
	TMVAssert(size() > 0);
	return ConstLowerTriMatrixView<T>(cptr()+stepi(),size()-1,
	    stepi(),stepj(),NonUnitDiag,S,NonConj);
      }

      inline ConstLowerTriMatrixView<T> MakeUnitDiag() const
      {
	return ConstLowerTriMatrixView<T>(cptr(),size(),
	    stepi(),stepj(),UnitDiag,S,NonConj);
      }

      inline MatrixView<T> SubMatrix(int i1, int i2, int j1, int j2) 
      {
	TMVAssert(this->OKSubMatrix(i1,i2,j1,j2,1,1));
	return MatrixView<T>(ptr()+i1*stepi()+j1*stepj(),
	    i2-i1, j2-j1, stepi(),stepj(),S,NonConj FIRSTLAST );
      }

      inline MatrixView<T> SubMatrix(int i1, int i2, int j1, int j2,
	  int istep, int jstep)
      {
	StorageType newstor = S==RowMajor ?
	  jstep == 1 ? RowMajor : NoMajor :
	  istep == 1 ? ColMajor : NoMajor;
	TMVAssert(this->OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	return MatrixView<T>(ptr()+i1*stepi()+j1*stepj(),
	    (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
	    newstor,NonConj FIRSTLAST );
      }

      inline VectorView<T> SubVector(size_t i, size_t j,
	  int istep, int jstep, size_t size)
      {
	TMVAssert(this->OKSubVector(i,j,istep,jstep,size));
	return VectorView<T>(ptr()+i*stepi()+j*stepj(),size,
	    istep*stepi()+jstep*stepj(),NonConj FIRSTLAST );
      }

      inline LowerTriMatrixView<T> SubTriMatrix(int i1, int i2)
      {
	TMVAssert(this->OKSubTriMatrix(i1,i2,1));
	return LowerTriMatrixView<T>(ptr()+i1*(stepi()+stepj()),i2-i1,
	    stepi(),stepj(),D,S,NonConj FIRSTLAST);
      }

      inline LowerTriMatrixView<T> SubTriMatrix(int i1, int i2, int istep) 
      {
	TMVAssert(this->OKSubTriMatrix(i1,i2,istep));
	return LowerTriMatrixView<T>(ptr()+i1*(stepi()+1),(i2-i1)/istep,
	    istep*stepi(),istep,D,
	    istep==1 ? S : NoMajor,NonConj FIRSTLAST);
      }

      inline LowerTriMatrixView<T> OffDiag()
      {
	TMVAssert(size() > 0);
	return LowerTriMatrixView<T>(ptr()+stepi(),size()-1,
	    stepi(),stepj(),NonUnitDiag,S,NonConj FIRSTLAST);
      }

      inline LowerTriMatrixView<T> MakeUnitDiag()
      {
	return LowerTriMatrixView<T>(ptr(),size(),
	    stepi(),stepj(),UnitDiag,S,NonConj FIRSTLAST);
      }

      inline ConstLowerTriMatrixView<T> View() const
      { 
	return ConstLowerTriMatrixView<T>(cptr(),size(),
	    stepi(),stepj(),D,S,NonConj);
      }

      inline ConstUpperTriMatrixView<T> Transpose() const
      { 
	return ConstUpperTriMatrixView<T>(cptr(),size(),
	    stepj(),stepi(),D,TransOf(S),NonConj);
      }

      inline ConstLowerTriMatrixView<T> Conjugate() const
      { 
	return ConstLowerTriMatrixView<T>(cptr(),size(),
	    stepi(),stepj(),D,S,ConjOf(T,NonConj));
      }

      inline ConstUpperTriMatrixView<T> Adjoint() const
      { 
	return ConstUpperTriMatrixView<T>(cptr(),size(),
	    stepj(),stepi(),D,TransOf(S),ConjOf(T,NonConj));
      }

      inline LowerTriMatrixView<T> View() 
      { 
	return LowerTriMatrixView<T>(ptr(),size(),
	    stepi(),stepj(),D,S,NonConj FIRSTLAST);
      }

      inline UpperTriMatrixView<T> Transpose() 
      { 
	return UpperTriMatrixView<T>(ptr(),size(),
	    stepj(),stepi(),D,TransOf(S),NonConj FIRSTLAST);
      }

      inline LowerTriMatrixView<T> Conjugate() 
      { 
	return LowerTriMatrixView<T>(ptr(),size(),
	    stepi(),stepj(),D,S,ConjOf(T,NonConj) FIRSTLAST);
      }

      inline UpperTriMatrixView<T> Adjoint() 
      { 
	return UpperTriMatrixView<T>(ptr(),size(),
	    stepj(),stepi(),D,TransOf(S),ConjOf(T,NonConj) FIRSTLAST);
      }

      inline size_t size() const { return itss; }
      inline const T* cptr() const { return itsm; }
      inline T* ptr() { return itsm; }
      inline int stepi() const { return S==RowMajor ? itss : 1; }
      inline int stepj() const { return S==RowMajor ? 1 : itss; }
      inline DiagType dt() const { return D; }
      inline StorageType stor() const { return S; }
      inline ConjItType ct() const { return NonConj; }
      inline bool isrm() const { return S==RowMajor; }
      inline bool iscm() const { return S==ColMajor; }
      inline bool isunit() const { return D == UnitDiag; }
      inline bool isconj() const { return false; }

    protected :

      const size_t itslen;
      T*const itsm;
      const size_t itss;

#ifdef TMVFLDEBUG
    public:
      const T*const first;
      const T*const last;
    protected :
#endif

      inline bool okij(size_t i, size_t j) const
      {
	TMVAssert(i < size());
	TMVAssert(j < size());
	return isunit() ? i>j : i>=j;
      }

      inline T& ref(size_t i, size_t j)
      {
	TMVAssert(i < size());
	TMVAssert(j < size());
	TMVAssert(okij(i,j));
	return *(ptr() + i*stepi() + j*stepj());
      }

      inline T cref(size_t i, size_t j) const 
      {
	TMVAssert(i < size());
	TMVAssert(j < size());
	return (i==j && isunit()) ? T(1) : (i<j) ? T(0) :
	  *(cptr() + i*stepi() + j*stepj());
      }

  }; // LowerTriMatrix - RowMajor

//---------------------------------------------------------------------------

  //
  // Special Creators: 
  //   UpperTriMatrixViewOf(m)
  //   LowerTriMatrixViewOf(m)
  //   UnitTriMatrixViewOf(t)
  //

  template <class T> inline ConstUpperTriMatrixView<T> UpperTriMatrixViewOf(
      const GenMatrix<T>& m, DiagType dt=NonUnitDiag)
  {
    TMVAssert(m.colsize()>=m.rowsize());
    return ConstUpperTriMatrixView<T>(m.Rows(0,m.rowsize()),dt); 
  }

  template <class T> inline UpperTriMatrixView<T> UpperTriMatrixViewOf(
      const MatrixView<T>& m, DiagType dt=NonUnitDiag)
  { 
    TMVAssert(m.colsize()>=m.rowsize());
    return UpperTriMatrixView<T>(m.Rows(0,m.rowsize()),dt); 
  }

  template <class T, StorageType S>
    inline UpperTriMatrixView<T> UpperTriMatrixViewOf(
	Matrix<T,S>& m, DiagType dt=NonUnitDiag)
    {
      TMVAssert(m.colsize()>=m.rowsize());
      return UpperTriMatrixView<T>(m.Rows(0,m.rowsize()),dt); 
    }

  template <class T> inline ConstUpperTriMatrixView<T> UpperTriMatrixViewOf(
      const GenUpperTriMatrix<T>& m, DiagType dt)
  {
    TMVAssert(m.colsize()>=m.rowsize());
    TMVAssert(!(m.dt()==UnitDiag && dt==NonUnitDiag));
    return ConstUpperTriMatrixView<T>(m,dt); 
  }

  template <class T> inline UpperTriMatrixView<T> UpperTriMatrixViewOf(
      const UpperTriMatrixView<T>& m, DiagType dt)
  { 
    TMVAssert(m.colsize()>=m.rowsize());
    TMVAssert(!(m.dt()==UnitDiag && dt==NonUnitDiag));
    return UpperTriMatrixView<T>(m,dt); 
  }

  template <class T, DiagType D, StorageType S>
    inline UpperTriMatrixView<T> UpperTriMatrixViewOf(
	UpperTriMatrix<T,D,S>& m, DiagType dt)
    {
      TMVAssert(m.colsize()>=m.rowsize());
      TMVAssert(!(D==UnitDiag && dt==NonUnitDiag));
      return UpperTriMatrixView<T>(m.View(),dt); 
    }

  template <class T> inline ConstUpperTriMatrixView<T> UpperTriMatrixViewOf(
      const T* m, size_t size, DiagType dt, StorageType stor)
  {
    TMVAssert(stor == RowMajor || stor == ColMajor);
    if (stor == RowMajor)
      return ConstUpperTriMatrixView<T>(m,size,size,1,
	  dt,RowMajor,NonConj);
    else
      return ConstUpperTriMatrixView<T>(m,size,1,size,
	  dt,ColMajor,NonConj);
  }

  template <class T> inline UpperTriMatrixView<T> UpperTriMatrixViewOf(
      T* m, size_t size, DiagType dt, StorageType stor)
  {
    TMVAssert(stor == RowMajor || stor == ColMajor);
    if (stor == RowMajor)
      return UpperTriMatrixView<T>(m,size,size,1,
	  dt,RowMajor,NonConj FIRSTLAST1(m,m+size*size));
    else
      return UpperTriMatrixView<T>(m,size,1,size,
	  dt,ColMajor,NonConj FIRSTLAST1(m,m+size*size));
  }

  template <class T> inline ConstLowerTriMatrixView<T> LowerTriMatrixViewOf(
      const GenMatrix<T>& m, DiagType dt=NonUnitDiag)
  {
    TMVAssert(m.colsize()<=m.rowsize());
    return ConstLowerTriMatrixView<T>(m.Cols(0,m.colsize()),dt); 
  }

  template <class T> inline LowerTriMatrixView<T> LowerTriMatrixViewOf(
      const MatrixView<T>& m, DiagType dt=NonUnitDiag)
  { 
    TMVAssert(m.colsize()<=m.rowsize());
    return LowerTriMatrixView<T>(m.Cols(0,m.colsize()),dt); 
  }

  template <class T,StorageType S> 
    inline LowerTriMatrixView<T> LowerTriMatrixViewOf(
	Matrix<T,S>& m, DiagType dt=NonUnitDiag)
    {
      TMVAssert(m.colsize()<=m.rowsize());
      return LowerTriMatrixView<T>(m.Cols(0,m.colsize()),dt); 
    }

  template <class T> inline ConstLowerTriMatrixView<T> LowerTriMatrixViewOf(
      const GenLowerTriMatrix<T>& m, DiagType dt)
  {
    TMVAssert(m.colsize()<=m.rowsize());
    TMVAssert(!(m.dt()==UnitDiag && dt==NonUnitDiag));
    return ConstLowerTriMatrixView<T>(m,dt); 
  }

  template <class T> inline LowerTriMatrixView<T> LowerTriMatrixViewOf(
      const LowerTriMatrixView<T>& m, DiagType dt)
  { 
    TMVAssert(m.colsize()<=m.rowsize());
    TMVAssert(!(m.dt()==UnitDiag && dt==NonUnitDiag));
    return LowerTriMatrixView<T>(m,dt); 
  }

  template <class T, DiagType D, StorageType S> 
    inline LowerTriMatrixView<T> LowerTriMatrixViewOf(
	LowerTriMatrix<T,D,S>& m, DiagType dt)
    {
      TMVAssert(m.colsize()<=m.rowsize());
      TMVAssert(!(D==UnitDiag && dt==NonUnitDiag));
      return LowerTriMatrixView<T>(m.View(),dt); 
    }

  template <class T> inline ConstLowerTriMatrixView<T> LowerTriMatrixViewOf(
      const T* m, size_t size, DiagType dt, StorageType stor)
  {
    TMVAssert(stor == RowMajor || stor == ColMajor);
    if (stor == RowMajor)
      return ConstLowerTriMatrixView<T>(m,size,size,1,
	  dt,RowMajor,NonConj);
    else
      return ConstLowerTriMatrixView<T>(m,size,1,size,
	  dt,ColMajor,NonConj);
  }

  template <class T> inline LowerTriMatrixView<T> LowerTriMatrixViewOf(
      T* m, size_t size, DiagType dt, StorageType stor)
  {
    TMVAssert(stor == RowMajor || stor == ColMajor);
    if (stor == RowMajor)
      return LowerTriMatrixView<T>(m,size,size,1,
	  dt,RowMajor,NonConj FIRSTLAST1(m,m+size*size));
    else
      return LowerTriMatrixView<T>(m,size,1,size,
	  dt,ColMajor,NonConj FIRSTLAST1(m,m+size*size));
  }

  //
  // Copy
  //

  template <class T1, class T2> inline void Copy(
      const GenUpperTriMatrix<T1>& m1, const UpperTriMatrixView<T2>& m2)
  {
    TMVAssert(m1.size() == m2.size());
    TMVAssert(m1.dt() == m2.dt());
    const size_t N = m1.size();

    if (m1.size() > 0)
      if (m1.dt() == UnitDiag)
	Copy(m1.OffDiag(),m2.OffDiag());
      else
	if (m1.iscm() && m2.iscm()) 
	  for(size_t j=0;j<N;++j) m2.col(j,0,j+1) = m1.col(j,0,j+1);
	else 
	  for(size_t i=0;i<N;++i) m2.row(i,i,N) = m1.row(i,i,N);
  }

  //
  // Swap Matrices
  //

  template <class T> void Swap(
      const UpperTriMatrixView<T>& m1, const UpperTriMatrixView<T>& m2);
  template <class T, DiagType D, StorageType S> inline void Swap(
      const UpperTriMatrixView<T>& m1, UpperTriMatrix<T,D,S>& m2)
  { Swap(m1,m2.View()); }
  template <class T, DiagType D, StorageType S> inline void Swap(
      UpperTriMatrix<T,D,S>& m1, const UpperTriMatrixView<T>& m2)
  { Swap(m1.View(),m2); }
  template <class T, DiagType D, StorageType S1, StorageType S2> 
    inline void Swap(UpperTriMatrix<T,D,S1>& m1, UpperTriMatrix<T,D,S2>& m2)
    { Swap(m1.View(),m2.View()); }

  template <class T> inline void Swap(
      const LowerTriMatrixView<T>& m1, const LowerTriMatrixView<T>& m2)
  { Swap(m1.Transpose(),m2.Transpose()); }
  template <class T, DiagType D, StorageType S> inline void Swap(
      const LowerTriMatrixView<T>& m1, LowerTriMatrix<T,D,S>& m2)
  { Swap(m1.Transpose(),m2.Transpose()); }
  template <class T, DiagType D, StorageType S> inline void Swap(
      LowerTriMatrix<T,D,S>& m1, const LowerTriMatrixView<T>& m2)
  { Swap(m1.Transpose(),m2.Transpose()); }
  template <class T, DiagType D, StorageType S1, StorageType S2> 
    inline void Swap(LowerTriMatrix<T,D,S1>& m1, LowerTriMatrix<T,D,S2>& m2)
    { Swap(m1.Transpose(),m2.Transpose()); }

  //
  // Functions of Matrices:
  //

  template <class T> inline T Det(const GenUpperTriMatrix<T>& m)
  { return m.Det(); }
  template <class T> inline T Det(const GenLowerTriMatrix<T>& m)
  { return m.Det(); }

  template <class T> inline T Trace(const GenUpperTriMatrix<T>& m)
  { return m.Trace(); }
  template <class T> inline T Trace(const GenLowerTriMatrix<T>& m)
  { return m.Trace(); }

  template <class T> inline RealType(T) Norm(const GenUpperTriMatrix<T>& m)
  { return m.Norm(); }
  template <class T> inline RealType(T) Norm(const GenLowerTriMatrix<T>& m)
  { return m.Norm(); }

  template <class T> inline RealType(T) NormF(const GenUpperTriMatrix<T>& m)
  { return m.NormF(); }
  template <class T> inline RealType(T) NormF(const GenLowerTriMatrix<T>& m)
  { return m.NormF(); }

  template <class T> inline RealType(T) Norm2(const GenUpperTriMatrix<T>& m)
  { return m.Norm2(); }
  template <class T> inline RealType(T) Norm2(const GenLowerTriMatrix<T>& m)
  { return m.Norm2(); }

  template <class T> inline RealType(T) NormInf(const GenUpperTriMatrix<T>& m)
  { return m.NormInf(); }
  template <class T> inline RealType(T) NormInf(const GenLowerTriMatrix<T>& m)
  { return m.NormInf(); }

  template <class T> inline ConstLowerTriMatrixView<T> Transpose(
      const GenUpperTriMatrix<T>& m)
  { return m.Transpose(); }
  template <class T> inline ConstUpperTriMatrixView<T> Transpose(
      const GenLowerTriMatrix<T>& m)
  { return m.Transpose(); }

  template <class T> inline LowerTriMatrixView<T> Transpose(
      const UpperTriMatrixView<T>& m)
  { return m.Transpose(); }
  template <class T> inline UpperTriMatrixView<T> Transpose(
      const LowerTriMatrixView<T>& m)
  { return m.Transpose(); }

  template <class T, DiagType D, StorageType S> 
    inline LowerTriMatrixView<T> Transpose(UpperTriMatrix<T,D,S>& m)
    { return m.Transpose(); }
  template <class T, DiagType D, StorageType S> 
    inline UpperTriMatrixView<T> Transpose(LowerTriMatrix<T,D,S>& m)
    { return m.Transpose(); }

  template <class T> inline ConstUpperTriMatrixView<T> Conjugate(
      const GenUpperTriMatrix<T>& m)
  { return m.Conjugate(); }
  template <class T> inline ConstLowerTriMatrixView<T> Conjugate(
      const GenLowerTriMatrix<T>& m)
  { return m.Conjugate(); }

  template <class T> inline UpperTriMatrixView<T> Conjugate(
      const UpperTriMatrixView<T>& m)
  { return m.Conjugate(); }
  template <class T> inline LowerTriMatrixView<T> Conjugate(
      const LowerTriMatrixView<T>& m)
  { return m.Conjugate(); }

  template <class T, DiagType D, StorageType S> 
    inline UpperTriMatrixView<T> Conjugate(UpperTriMatrix<T,D,S>& m)
    { return m.Conjugate(); }
  template <class T, DiagType D, StorageType S> 
    inline LowerTriMatrixView<T> Conjugate(LowerTriMatrix<T,D,S>& m)
    { return m.Conjugate(); }

  template <class T> inline ConstLowerTriMatrixView<T> Adjoint(
      const GenUpperTriMatrix<T>& m)
  { return m.Adjoint(); }
  template <class T> inline ConstUpperTriMatrixView<T> Adjoint(
      const GenLowerTriMatrix<T>& m)
  { return m.Adjoint(); }

  template <class T> inline LowerTriMatrixView<T> Adjoint(
      const UpperTriMatrixView<T>& m)
  { return m.Adjoint(); }
  template <class T> inline UpperTriMatrixView<T> Adjoint(
      const LowerTriMatrixView<T>& m)
  { return m.Adjoint(); }

  template <class T, DiagType D, StorageType S> 
    inline LowerTriMatrixView<T> Adjoint(UpperTriMatrix<T,D,S>& m)
    { return m.Adjoint(); }
  template <class T, DiagType D, StorageType S> 
    inline UpperTriMatrixView<T> Adjoint(LowerTriMatrix<T,D,S>& m)
    { return m.Adjoint(); }

  template <class T> inline QuotXU<T,T> Inverse(const GenUpperTriMatrix<T>& m)
  { return m.Inverse(); }
  template <class T> inline QuotXL<T,T> Inverse(const GenLowerTriMatrix<T>& m)
  { return m.Inverse(); }

  //
  // TriMatrix ==, != TriMatrix
  //

  template <class T1, class T2> bool operator==(
      const GenUpperTriMatrix<T1>& m1, const GenUpperTriMatrix<T2>& m2);
  template <class T1, class T2> inline bool operator==(
      const GenLowerTriMatrix<T1>& m1, const GenLowerTriMatrix<T2>& m2)
  { return m1.Transpose() == m2.Transpose(); }
  template <class T1, class T2> inline bool operator!=(
      const GenUpperTriMatrix<T1>& m1, const GenUpperTriMatrix<T2>& m2)
  { return !(m1 == m2); }
  template <class T1, class T2> inline bool operator!=(
      const GenLowerTriMatrix<T1>& m1, const GenLowerTriMatrix<T2>& m2)
  { return !(m1 == m2); }


  //
  // I/O
  //
 
  template <class T, DiagType D, StorageType S> istream& operator>>(
      istream& is, UpperTriMatrix<T,D,S>*& m);
  template <class T, DiagType D, StorageType S> istream& operator>>(
      istream& is, LowerTriMatrix<T,D,S>*& m);

  template <class T> istream& operator>>(
      istream& is, const UpperTriMatrixView<T>& m);
  template <class T> istream& operator>>(
      istream& is, const LowerTriMatrixView<T>& m);

  template <class T, DiagType D, StorageType S> istream& operator>>(
      istream& is, UpperTriMatrix<T,D,S>& m)
  { return is>>m.View(); }
  template <class T, DiagType D, StorageType S> istream& operator>>(
      istream& is, LowerTriMatrix<T,D,S>& m)
  { return is>>m.View(); }

  template <class T, DiagType D, StorageType S> inline string Type(
      const UpperTriMatrix<T,D,S>& m)
  { 
    return string("UpperTriMatrix<")+Type(T())+","
      + Text(D)+","+Text(S)+">"; 
  }
  template <class T, DiagType D, StorageType S> inline string Type(
      const LowerTriMatrix<T,D,S>& m)
  { 
    return string("LowerTriMatrix<")+Type(T())+","
      + Text(D)+","+Text(S)+">"; 
  }

  template <class T> inline string Type(const GenUpperTriMatrix<T>& m)
  { 
    return string("GenUpperTriMatrix<")+Type(T())+","
      + Text(m.dt())+","+Text(m.ct())+","+Text(m.stor())+">"; 
  }
  template <class T> inline string Type(const GenLowerTriMatrix<T>& m)
  { 
    return string("GenLowerTriMatrix<")+Type(T())+","
      + Text(m.dt())+","+Text(m.ct())+","+Text(m.stor())+">"; 
  }

  template <class T> inline string Type(
      const ConstUpperTriMatrixView<T>& m)
  {
    return string("ConstUpperTriMatrixView<")+Type(T())+","
      + Text(m.dt())+","+Text(m.ct())+","+Text(m.stor())+">"; 
  }
  template <class T> inline string Type(
      const ConstLowerTriMatrixView<T>& m)
  {
    return string("ConstLowerTriMatrixView<")+Type(T())+","
      + Text(m.dt())+","+Text(m.ct())+","+Text(m.stor())+">"; 
  }

  template <class T> inline string Type(const UpperTriMatrixView<T>& m)
  {
    return string("UpperTriMatrixView<")+Type(T())+","
      + Text(m.dt())+","+Text(m.ct())+","+Text(m.stor())+">"; 
  }
  template <class T> inline string Type(const LowerTriMatrixView<T>& m)
  {
    return string("LowerTriMatrixView<")+Type(T())+","
      + Text(m.dt())+","+Text(m.ct())+","+Text(m.stor())+">"; 
  }

}; // namespace tmv

#endif
