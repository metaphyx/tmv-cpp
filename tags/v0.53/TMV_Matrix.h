//---------------------------------------------------------------------------
//
// This file defines the TMV Matrix class.
//
// The Matrix class and all associated functions are contained
// in the namespace tmv.  Alse, the Matrix class is a template, 
// so for a Matrix of long doubles, one would write 
// tmv::Matrix<long double>.  
//
// There is also an optional second template parameter which is 
// either RowMajor or ColMajor, referring to the storage pattern of 
// the matrix elements.
//
// Finally, a third optional template parameter is either CStyle or
// FortranStyle to indicate whether to use C-style or Fortran-style indexing.  
// With C-style (the default), the upper left corner of an MxN matrix is m(0,0), 
// and the lower right is m(M-1,N-1).  With Fortran-style, these are m(1,1) and
// m(M,N) respectively.  Also, when a function takes an index range, i1,i2, 
// then with C-style, this means elements from i1...i2-1 inclusive. 
// With Fortran-style, this means i1..i2 inclusive.
//
// Note that if you want to specify Fortran-style, then you must also 
// specify the storage pattern, since that parameter precedes it.  For example:
// Matrix<T,RowMajor,FortranStyle> m(10,10);
//
// Also, GenMatrix and MatrixComposite always use C-style indexing.
// However, they are both castable as a Fortran-style matrix.  Just remember
// to do so if you want to use Fortran-style indexing.
//
// Constructors:
//
//    Matrix<T,stor,I>(size_t colsize, size_t rowsize)
//        Makes a Matrix with column size = colsize and row size = rowsize
//        with _uninitialized_ values
//
//    Matrix<T,stor,I>(size_t colsize, size_t rowsize, T x)
//        Makes a Matrix of size n with all values = x
//
//    Matrix<T,stor,I>(const vector<vector<T> >& m)
//        Makes a Matrix with a_ij = m[i][j]
//
//    Matrix<T,stor,I>(size_t colsize, size_t rowsize, const valarray<T>& m)
//    Matrix<T,stor,I>(size_t colsize, size_t rowsize, const T* m)
//    Matrix<T,stor,I>(size_t colsize, size_t rowsize, const vector<T>& m)
//        Make a Matrix which copies the elements of m.
//        If stor is tmv::RowMajor then the elements are taken in row major
//        order (m00,m01,..m0n,m10,m11...).  If stor is tmv::ColMajor
//        then the elements are taken in column major order.
//        If stor is omitted, then tmv::RowMajor is assumed.
//        (stor is also an optional last parameter on the other above 
//        constructors as well.)
//
//
// Special Constructors
//
//    MatrixView RowVectorViewOf(Vector& v)
//    MatrixView RowVectorViewOf(const VectorView& v)
//    ConstMatrixView RowVectorViewOf(const Vector& v)
//    ConstMatrixView RowVectorViewOf(const ConstVectorView& v)
//        Returns a 1xn MatrixView with v in the (only) row.
//        Unlike creating a Matrix with RowVector(v), this uses the same
//        data storage as the actual Vector v.
//        For a const Vector or a ConstVectorView, this returns a 
//        ConstMatrixView.
//
//    MatrixView ColVectorViewOf(Vector& v)
//    MatrixView ColVectorViewOf(const VectorView& v)
//    ConstMatrixView ColVectorViewOf(const Vector& v)
//    ConstMatrixView ColVectorViewOf(const ConstVectorView& v)
//        Returns an nx1 MatrixView with v in the (only) column.
//
//    MatrixView MatrixViewOf(T* m, size_t colsize, size_t rowsize,
//            StorageType stor)
//    ConstMatrixView MatrixViewOf(const T* m, size_t colsize, size_t rowsize,
//            StorageType stor)
//        Returns a MatrixView of the elements in m, using the actual
//        elements m for the storage.  This is essentially the same as the 
//        constructor with (const T*m), except that the data isn't duplicated.
//
// Access Functions
//
//    size_t colsize() const
//    size_t rowsize() const
//        Return the dimensions of the Matrix
//
//    T& operator()(size_t i, size_t j)
//    T operator()(size_t i, size_t j) const
//        Return the (i,j) element of the Matrix
//
//    VectorView& operator[](size_t i)
//    ConstVectorView& operator[](size_t i) const
//        Return the ith row of the Matrix as a Vector.
//        This allows for m[i][j] style access.
//
//    VectorView& row(size_t i, size_t j1, size_t j2)
//    ConstVectorView& row(size_t i, size_t j1, size_t j2) const
//        Return the ith row of the Matrix as a Vector
//        If j1,j2 are given, it returns the SubVector from j1 to j2 
//        (not including j2) within the row.
//
//    VectorView& col(size_t j, size_t i1, size_t i2)
//    ConstVectorView& col(size_t j) const
//        Return the jth column of the Matrix as a Vector
//        If i1,i2 are given, it returns the SubVector from i1 to i2 
//        (not including i2) within the column.
//
//    VectorView& diag()
//    ConstVectorView& diag() const
//        Return the diagonal of the Matrix as a Vector
//
//    VectorView& diag(int i, size_t j1, size_t j2)
//    ConstVectorView& diag(i, j1, j2) const
//        Return the super- or sub-diagonal i
//        If i > 0 return the super diagonal starting at m_0i
//        If i < 0 return the sub diagonal starting at m_|i|0
//        If j1,j2 are given, it returns the diagonal SubVector 
//        either from m_j1,i+j1 to m_j2,i+j2 (for i>0) 
//        or from m_|i|+j1,j1 to m_|i|+j2,j2 (for i<0)
//
// Modifying Functions
//
//    Matrix& Zero()
//        Sets all elements to 0
//
//    Matrix& Clip(RT thresh)
//        Set to 0 all elements whose abolute value is < thresh
//
//    Matrix& SetAllTo(T x)
//        Sets all elements to x
//
//    Matrix<T>& TransposeSelf() 
//        Transposes the elements of a square Matrix or SubMatrix
//
//    Matrix& ConjugateSelf()
//        Sets all elements to its conjugate
//
//    Matrix& SetToIdentity(x = 1)
//        Set to Identity Matrix, or 
//        with a parameter, set to x times Identity Matrix
//
//    Matrix& SwapRows(i1, i2)
//        Swap two rows
//
//    Matrix& SwapCols(j1, j2)
//        Swap two columns
//
//    Matrix& PermuteRows(const size_t* p)
//    Matrix& PermuteRows(const size_t* p, size_t i1, size_t i2)
//        Perform a series of row swaps (0,p[0]), (1,p[1])...
//        In the second case, only do (i1,p[i1)...(i2-1,p[i2-1])
//    Matrix& ReversePermuteRows(const size_t* p)
//    Matrix& ReversePermuteRows(const size_t* p, size_t i1, size_t i2)
//        The same, but perform the swaps in reverse order
//
//    Matrix& PermuteCols(const size_t* p)
//    Matrix& PermuteCols(const size_t* p, size_t j1, size_t j2)
//        Perform a series of column swaps (0,p[0]), (1,p[1])...
//        In the second case, only do (j1,p[j1)...(j2-1,p[j2-1])
//    Matrix& ReversePermuteCols(const size_t* p)
//    Matrix& ReversePermuteCols(const size_t* p, size_t j1, size_t j2)
//        The same, but perform the swaps in reverse order
//
//    void Swap(Matrix& m1, Matrix& m2)
//        Swap the values of two Matrices
//        The Matrices must be the same size
//
// MatrixViews:
//
//    As with VectorViews, we have both constant and mutable views of Matrices.
//    A ConstMatrixView object can only view the underlying Matrix object
//    whereas a MatrixView object can modify it as well.
//    For the below routines, a ConstMatrixView is returned from 
//    ConstMatrixViews and const Matrix objects.
//    A MatrixView is returned from MatrixViews and (non-const) Matrix objects.
//
//    MatrixView SubMatrix(int i1, int i2, int j1, int j2,
//            int istep=1, int jstep=1)
//        This member function will return a submatrix using rows i1 to i2
//        and columns j1 to j2 (not inclusive of i2,j2) which refers
//        to the same physical elements as the original.
//        Thus, to make the matrix:
//           ( 0 0 1 0 )
//           ( 0 0 0 1 )
//           ( 2 2 0 0 )
//           ( 2 2 0 0 )
//        you could write:
//           Matrix<int> A(4,4,0);
//           A.SubMatrix(0,1,2,3).SetToIdentity();
//           A.SubMatrix(2,3,0,1).SetAllTo(2);
//        The substep values allow you to space the elements of 
//        the submatrix at steps larger than 1.
//        eg. To make an 8x8 checkerboard of 1's and 0's, you could write:
//           Matrix<int> A(8,8,0);
//           A.SubMatrix(0,8,1,9,2,2) = 1;
//           A.SubMatrix(1,9,0,8,2,2) = 1;
//        Note that in this case the i2,j2 values need to be the 
//        "one past the end" value given the step size, so 9 here when
//        starting at 1.
//
//    VectorView SubVector(int i, int j, int istep, int jstep, int size)
//        Returns a SubVector which starts at position (i,j) in the 
//        matrix, moves in the directions (istep,jstep) and has a length
//        of size.
//        For example, the cross-diagonal from the lower left to the upper
//        right of a 6x6 matrix could be accessed using:
//        m.SubVector(5,0,-1,1,6)
//
//    MatrixView ColPair(int j1, int j2)
//        This returns an Mx2 submatrix which consists of the 
//        columns j1 and j2.  This is useful for multiplying two 
//        (not necessarily adjacent) columns of a matrix by a 2x2 matrix.
//
//    MatrixView RowPair(int i1, int i2)
//        Same as above, but two rows.
//
//    MatrixView Cols(int j1, int j2)
//        This is shorthand for SubMatrix(0,m.colsize(),j1,j2)
//
//    MatrixView Rows(int i1, int i2)
//        This is shorthand for SubMatrix(i1,i2,0,m.rowsize())
//
//    MatrixView Real()
//    MatrixView Imag()
//        Returns a view to the real/imag elements of a complex Matrix.
//
//    MatrixView View()
//        Returns a view of a matrix.
//
//    MatrixView Conjugate()
//        Returns a view to the conjugate of a Matrix.
//
//    MatrixView Transpose()
//        Returns a view to the transpose of a Matrix.
//
//    MatrixView Adjoint()
//        Returns a view to the adjoint (conjugate transpose) of a Matrix.
//        Note: Some people define the adjoint as the cofactor matrix.
//              This is not the same as our definition of the Adjoint.
//
//    VectorView LinearView()
//        Returns a VectorView with all the elements of the Matrix.
//        This is mostly used internally for things like MaxElement
//        and ConjugateSelf, where the matrix structure is irrelevant,
//        and we just want to do something to all the elements.
//        The correlary function CanLinearize() returns whether this is 
//        allowed.
//
// Functions of Matrixs:
//
//    Det(m)
//    m.Det()
//        Returns the determinant of a Matrix.
//        Note: If the matrix is not square, the determinant is not
//              well defined.  The returned value is such that
//              conj(det) * det = Det(Adjoint(m) * m)
//              So for real nonsquare matrices, the sign is arbitrary,
//              and for complex nonsquare matrices, it is multiplied
//              by an arbitrary phase.
//
//    Trace(m)
//    m.Trace()
//        Returns the trace of a Matrix.
//        = sum_i ( a_ii )
//
//    Norm(m) or NormF(m)
//    m.Norm() or m.NormF()
//        Return the Frobenius norm of a Matrix.
//        = sqrt( sum_ij |a_ij|^2 )
//
//    NormSq(m)
//    m.NormSq()
//        Returns the square of Norm().
//
//    Norm1(m) 
//    m.Norm1() 
//        Returns the 1-norm of a Matrix.
//        = max_j (sum_i |a_ij|)
//
//    Norm2(m) 
//    m.Norm2() 
//        Returns the 2-norm of a Matrix.
//        = sqrt( Max Eigenvalue of (A.Adjoint * A) )
//        = Maximum singular value
//        Note: This norm is costly to calculate if one is not 
//              otherwise doing a singular value decomposition
//              of the Matrix.
//
//    NormInf(m) 
//    m.NormInf() 
//        Returns the infinity-norm of a Matrix.
//        = max_i (sum_j |a_ij|)
//
//    m.Inverse(minv)
//        Sets minv to the inverse of m if it exists.
//        If m is singular and square, and LU is set for dividing
//          (LU is default for square matrices)
//          then a run-time error will occur.
//        If m is singular or not square and SV is set 
//          then the returned matrix is the pseudo-inverse which satisfies:
//          MXM = M
//          XMX = X
//          (MX)T = MX
//          (XM)T = XM
//        [If dividing using QR or QRP, all but the last of these will 
//         be true.]
//
//    m.InverseATA(invata)
//        Sets invata to the Inverse of (A.Adjoint * A) for matrix m = A
//        If Ax=b is solved for x, then (AT A)^-1 is the 
//        covariance matrix of the least-squares solution x
//
//    m.Inverse()
//    Inverse(m)
//        Returns an auxiliary object that delays the calculation of the 
//        inverse until there is appropriate storage for it.
//        m.Inverse(minv) is equivalent to minv = m.Inverse().
//
//    m.NewTranspose()
//    m.NewConjugate()
//    m.NewAdjoint()
//    m.NewView()
//    m.NewInverse()
//    m.NewCopy()
//        These all return pointers to new BaseMatrix's equal to the 
//        Transpose, Conjugate, Adjoint, View, Inverse, or itself respectively.
//        Inverse and Copy allocate new memory, the others are view.
//
// Operators:
//        Here we use m for a Matrix, v for a Vector and x for a Scalar.
//
//        You can also mix real and complex Vectors of the same
//        underlying type.  eg. Matrix<double> and Matrix<complex<double> >
//
//    -m
//
//    m = m
//
//    m += m
//    m + m
//    m += x
//    m + x
//    x + m
//
//    m -= m
//    m - m
//    m -= x
//    m - x
//    x - m
//
//    m *= x
//    m *= m
//    m * x
//    x * m
//    m * v
//    v * m
//    v *= m
//    m * m
//
//    m /= x
//    v /= m
//    v %= m
//    m /= m
//    m %= m
//    m / x
//    x / m
//    v / m
//    v % m
//    m / m
//    m % m
//
//    m == m
//    m != m
//
//       Most of these behave in the logical way for dealing with matrices.
//       Two comments about behavior that might not be obvious:
//
//       1) Vectors are either row or column Vectors as appropriate.
//          eg. For m*v, v is a column Vector, but for v*m, v is a row Vector
//
//       2) Sometimes x should be thought of a x*I, where I is an appropriately
//          sized identity matrix.  For example m-1 means m-I,
//          and m += 3 means m += 3*I (or equivalently, m.diag().AddToAll(3)).
//
//       3) Division by a matrix can be from either the left or the right.
//          ie. v/m can mean either m^-1 v or v m^-1
//          The first case is the solution of mx=v, the second is of xm = v.
//          Since the first case is the way one generally poses a problem
//          for solving a set of equations, we take v/m to be left-division.
//          If you want right-division (v m^-1), then we supply the % operator
//          to do so.  
//          ie. v%m means v m^-1
//          If you want to be more explicit, you can write:
//          v/m as m.Inverse() * v and v%m as v * m.Inverse().
//          In all cases, the actual calculation is delayed until there is
//          storage to put it.  (Unless you string too many calculations 
//          together, in which case it will use a temporary.)
//
//
// I/O: 
//
//    os << m 
//        Writes m to ostream os in the following format:
//          colsize rowsize
//          m(0,0) m(0,1) m(0,2) ... m(0,rowsize)
//          m(1,0) m(1,1) m(1,2) ... m(1,rowsize)
//          ...
//          m(colsize,0) ... 
//
//    is >> m
//        Reads m from istream is in the same format
//        m must already be the correct size for this to work.
//
//    is >> mptr
//        If you do not know the size of the Matrix to be read, you can
//        use this form where mptr is a pointer to an undefined Matrix.
//        Then *mptr will be created with the right size using new,
//        so you should subsequently delete it.
//
//
// Division Control Functions:
//
//    There are a number of algorithms available for dividing
//    matrices.  We provide functions to allow you to 
//    change the algorithm used by the code on the fly.
//    In particular, you can write:
//    m.DivideUsing(dt)
//    where dt is LU, QR, QRP, SV, SVS, SVU or SVV
//    (ie. anything but CH)
//    Note: the last 3 cannot actually perform divisions.  They are 
//    available for when you want an SV decomposition, but don't need to 
//    store both of the U,V matrices.  SVS only stores S, 
//    SVU stores S,U, and SVV stores S,V.
//    Each of these also has an in-place version whcih overwrites the
//    current Matrix memory with the decomposition needed for 
//    doing the division.  Obviously, if you try to use the Matrix
//    after doing SetDiv (implicitly or explicitly), the results will
//    be invalid.
//
//    The default method is LU (LU Decomposition) for square
//    matrices and QR (QR Decomposition) for non-square.
//   
//    See the header comment to TMV_Divider.h for more info about
//    the different algorithms.
//
//    There are also shorthands for accessing the decomposition.
//    If dt = LU, and the decomposition has been set, then 
//    LUD() returns the LUDiv<T> class
//    It is shorthand for *dynamic_cast<const LUDiv<T>*>(GetDiv());
//    Likewise:
//    QRD(), QRPD(), SVD() return the corresponding
//    Divider classes.
//    (SVD() is appropriate for SV, SVS, SVU, and SVV.)
//


#ifndef TMV_Matrix_H
#define TMV_Matrix_H

#include "TMV_Vector.h"

namespace tmv {

  template <class T> class GenMatrix;
  template <class T, IndexStyle I=CStyle> class ConstMatrixView;
  template <class T, IndexStyle I=CStyle> class MatrixView;
  template <class T, StorageType S=RowMajor, IndexStyle I=CStyle> class Matrix;
  template <class T> class MatrixComposite; // Defined in TMV_MatrixArith.h

#define TransOf(s) \
  (s==RowMajor ? ColMajor : s==ColMajor ? RowMajor : s)

  template <class M> inline StorageType BaseStorOf(const M& m)
  {
    return (m.stor()==RowMajor || m.stor()==ColMajor) ? m.stor() : 
      m.stepi() > m.stepj() ? RowMajor : ColMajor;
  }

  template <class T> class LUDiv;
  template <class T> class QRDiv;
  template <class T> class QRPDiv;
  template <class T> class SVDiv;
  template <class T, class Tm> class QuotXM;

}

#include "TMV_BaseMatrix.h"

namespace tmv {

  template <class T1, class T2> inline void Copy(
      const GenMatrix<T1>& m1, const MatrixView<T2>& m2);

  template <class T> class GenMatrix : 
    public BaseMatrix<T>
  {

    public:

      //
      // Constructors
      //

      GenMatrix(StorageType s, ConjItType c, size_t ls) : 
	itsstor(s), itsct(c), linsize(ls) {}
      GenMatrix(const GenMatrix<T>& rhs) : 
	BaseMatrix<T>(rhs), itsstor(rhs.itsstor), itsct(rhs.itsct),
        linsize(rhs.linsize) {}
      ~GenMatrix() {}

      //
      // Access Functions
      //

      using BaseMatrix<T>::colsize;
      using BaseMatrix<T>::rowsize;
      using BaseMatrix<T>::IsSquare;

      inline ConstVectorView<T> row(size_t i) const 
      { 
	TMVAssert(i<colsize());
	return ConstVectorView<T>(cptr()+i*stepi(),rowsize(),stepj(),itsct); 
      }

      inline ConstVectorView<T> col(size_t j) const
      {
	TMVAssert(j<rowsize());
        return ConstVectorView<T>(cptr()+j*stepj(),colsize(),stepi(),itsct); 
      }

      inline ConstVectorView<T> diag() const
      {
	return ConstVectorView<T>(cptr(),min(colsize(),rowsize()),
	      stepi()+stepj(),itsct);
      }

      inline ConstVectorView<T> diag(int i) const
      {
	TMVAssert(i<=int(rowsize()));
	TMVAssert(-i<=int(colsize()));
	const size_t diagstep = stepi() + stepj();
	if (i >= 0) {
	  const size_t diagsize = min(colsize(),rowsize()-i);
	  return ConstVectorView<T>(cptr()+i*stepj(),diagsize,diagstep,itsct);
	} else {
	  const size_t diagsize = min(colsize()+i,rowsize());
	  return ConstVectorView<T>(cptr()-i*stepi(),diagsize,diagstep,itsct);
	}
      }

      inline ConstVectorView<T> row(size_t i, size_t j1, size_t j2) const 
      { 
	TMVAssert(i<colsize());
	TMVAssert(j1<=j2);
	TMVAssert(j2<=rowsize());
	return ConstVectorView<T>(cptr()+i*stepi()+j1*stepj(),
	    j2-j1,stepj(),itsct); 
      }

      inline ConstVectorView<T> col(size_t j, size_t i1, size_t i2) const
      {
	TMVAssert(j<rowsize());
	TMVAssert(i1<=i2);
	TMVAssert(i2<=colsize());
	return ConstVectorView<T>(cptr()+i1*stepi()+j*stepj(),
	    i2-i1,stepi(),itsct); 
      }

      inline ConstVectorView<T> diag(int i, size_t j1, size_t j2) const
      {
	TMVAssert(i<=int(rowsize()));
	TMVAssert(-i<=int(colsize()));
	const size_t diagstep = stepi() + stepj();
	if (i >= 0) {
	  TMVAssert(j1 <= j2);
	  TMVAssert(j2 <= min(colsize(),rowsize()-i));
	  return ConstVectorView<T>(cptr()+i*stepj()+j1*diagstep, 
	      j2-j1, diagstep,itsct);
	} else {
	  TMVAssert(j1 <= j2);
	  TMVAssert(j2 <= min(colsize()+i,rowsize()));
	  return ConstVectorView<T>(cptr()-i*stepi()+j1*diagstep, 
	      j2-j1, diagstep,itsct);
	}
      }

      inline T operator()(size_t i, size_t j) const
      { 
	TMVAssert(i<colsize());
	TMVAssert(j<rowsize());
	return cref(i,j);
      }

      inline ConstVectorView<T> operator[](size_t i) const
      { 
	TMVAssert(i<colsize());
	return row(i); 
      }

      template <class T2> inline bool SameStorageAs(
	  const BaseMatrix<T2>& m2) const
      { return false; }

      inline bool SameStorageAs(const GenMatrix<T>& m2) const
      { return (cptr()==m2.cptr()); }

      template <class T2> inline bool SameAs(
	  const BaseMatrix<T2>& m2) const
      { return false; }

      inline bool SameAs(const GenMatrix<T>& m2) const
      { 
	return (this==&m2 || (cptr()==m2.cptr() && 
	    rowsize()==m2.rowsize() && colsize()==m2.colsize() &&
	    stepi()==m2.stepi() && stepj()==m2.stepj() && itsct==m2.itsct));
      }

      inline void CopyToMatrix(const MatrixView<RealType(T)>& m) const
      { 
	TMVAssert(IsReal(T()));
	TMVAssert(m.colsize() == colsize());
	TMVAssert(m.rowsize() == rowsize());
	Copy(*this,m);
      }
      inline void CopyToMatrix(const MatrixView<ComplexType(T)>& m) const
      {
	TMVAssert(m.colsize() == colsize());
	TMVAssert(m.rowsize() == rowsize());
	Copy(*this,m);
      }

      //
      // SubMatrix
      //

      bool OKSubMatrix(
	  int i1, int i2, int j1, int j2, int istep, int jstep) const;

      inline ConstMatrixView<T> SubMatrix(
	  int i1, int i2, int j1, int j2) const
      {
	TMVAssert(OKSubMatrix(i1,i2,j1,j2,1,1));
	return ConstMatrixView<T>(cptr()+i1*stepi()+j1*stepj(),
	    i2-i1,j2-j1,stepi(),stepj(),stor(),itsct);
      }

      inline ConstMatrixView<T> SubMatrix(
	  int i1, int i2, int j1, int j2, int istep, int jstep) const
      {
	StorageType newstor = 
	  iscm() ? (istep == 1 ? ColMajor : NoMajor) :
	  isrm() ? (jstep == 1 ? RowMajor : NoMajor) : NoMajor;
	TMVAssert(OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	return ConstMatrixView<T>(cptr()+i1*stepi()+j1*stepj(),
	    (i2-i1)/istep,(j2-j1)/jstep,istep*stepi(),jstep*stepj(),
	    newstor,itsct);
      }

      bool OKSubVector(
	  size_t i, size_t j, int istep, int jstep, size_t s) const;

      inline ConstVectorView<T> SubVector(
	  size_t i, size_t j, int istep, int jstep, size_t s) const
      {
	TMVAssert(OKSubVector(i,j,istep,jstep,s));
	return ConstVectorView<T>(cptr()+i*stepi()+j*stepj(),s,
	    istep*stepi()+jstep*stepj(),itsct);
      }

      inline ConstMatrixView<T> ColPair(size_t j1, size_t j2) const
      {
	StorageType newstor = 
	  iscm() ? ColMajor : 
	  isrm() ? (j2==j1+1 ? RowMajor : NoMajor) : NoMajor;
	TMVAssert(j1<rowsize() && j2<rowsize());
	return ConstMatrixView<T>(cptr()+j1*stepj(),colsize(),2,
	    stepi(),(int(j2)-int(j1))*stepj(),newstor,itsct);
      }

      inline ConstMatrixView<T> RowPair(size_t i1, size_t i2) const
      {
	StorageType newstor = 
	  isrm() ? RowMajor : 
	  iscm() ? (i2==i1+1 ? ColMajor : NoMajor) : NoMajor;
	TMVAssert(i1<colsize() && i2<colsize());
	return ConstMatrixView<T>(cptr()+i1*stepi(),2,rowsize(),
	    (int(i2)-int(i1))*stepi(),stepj(),newstor,itsct);
      }

      inline ConstMatrixView<T> Cols(size_t j1, size_t j2) const
      {
	TMVAssert(j1<=j2);
	TMVAssert(j2<=rowsize());
	return ConstMatrixView<T>(cptr()+j1*stepj(),colsize(),j2-j1,
	    stepi(),stepj(),stor(),itsct,
	    (iscm()&&ls())?1:0);
      }

      inline ConstMatrixView<T> Rows(size_t i1, size_t i2) const
      {
	TMVAssert(i1<=i2);
	TMVAssert(i2<=colsize());
	return ConstMatrixView<T>(cptr()+i1*stepi(),i2-i1,rowsize(),
	    stepi(),stepj(),stor(),itsct,
	    (isrm()&&ls())?1:0);
      }

      inline ConstMatrixView<RealType(T)> Real() const
      {
	return ConstMatrixView<RealType(T)>(
	    reinterpret_cast<const RealType(T)*>(cptr()),colsize(),rowsize(),
	    IsReal(T()) ? stepi() : 2*stepi(),
	    IsReal(T()) ? stepj() : 2*stepj(),
	    IsReal(T()) ? stor() : NoMajor,NonConj);
      }

      inline ConstMatrixView<RealType(T)> Imag() const
      {
	TMVAssert(IsComplex(T()));
	TMVAssert(itsct==NonConj);
	return ConstMatrixView<RealType(T)>(
	    reinterpret_cast<const RealType(T)*>(cptr())+1,
	    colsize(),rowsize(),2*stepi(),2*stepj(),NoMajor,NonConj);
      }

      //
      // Views
      //

      inline ConstMatrixView<T> View() const
      { 
	return ConstMatrixView<T>(cptr(),colsize(),rowsize(),
	    stepi(),stepj(),stor(),itsct,linsize);
      }

      inline ConstMatrixView<T> Transpose() const
      { 
	return ConstMatrixView<T>(cptr(),rowsize(),colsize(),
	    stepj(),stepi(),TransOf(stor()),itsct,linsize);
      }
  
      inline ConstMatrixView<T> Conjugate() const
      { 
	return ConstMatrixView<T>(cptr(),colsize(),rowsize(),
	    stepi(),stepj(),stor(),ConjOf(T,itsct),linsize);
      }

      inline ConstMatrixView<T> Adjoint() const
      { 
	return ConstMatrixView<T>(cptr(),rowsize(),colsize(),
	    stepj(),stepi(),TransOf(stor()),ConjOf(T,itsct),linsize);
      }

      inline bool CanLinearize() const 
      { 
	if (linsize == 1 && (rowsize() != 1 || colsize() != 1))
	  linsize = rowsize() * colsize();
	TMVAssert(linsize == 0 || isrm() || iscm());
	TMVAssert(linsize == 0 || !isrm() || stepi() == int(rowsize()));
	TMVAssert(linsize == 0 || !iscm() || stepj() == int(colsize()));
	return linsize > 0; 
      }

      inline ConstVectorView<T> ConstLinearView() const
      {
	TMVAssert(linsize != 1 || (rowsize() == 1 && colsize() == 1));
	  // To assure that next Assert has no effect

	TMVAssert(CanLinearize());
	TMVAssert(linsize == colsize()*rowsize());
	return ConstVectorView<T>(cptr(),linsize,1,itsct);
      }

      inline MatrixView<T> NonConst() const
      {
	return MatrixView<T>(const_cast<T*>(cptr()),colsize(),rowsize(),
	    stepi(),stepj(),stor(),itsct,linsize
	    FIRSTLAST1(cptr(),row(colsize()-1).end().GetP()));
      }

      //
      // Functions of Matrix
      //

      inline T Trace() const
      { return diag().SumElements(); }

      inline RealType(T) Norm() const 
      { return NormF(); }

      RealType(T) NormF() const;

      // NormF()^2
      RealType(T) NormSq() const;

      // 1-Norm = max_j (sum_i |a_ij|)
      RealType(T) Norm1() const;

      // 2-Norm defined in BaseMatrix.h

      // inf-Norm = max_i (sum_j |a_ij|)
      inline RealType(T) NormInf() const
      { return Transpose().Norm1(); }

      // = max_i,j (|a_ij|)
      RealType(T) MaxAbsElement() const;

      inline QuotXM<T,T> Inverse() const
      { return QuotXM<T,T>(T(1),*this); }

      using BaseMatrix<T>::Inverse;
      using BaseMatrix<T>::InverseATA;

      template <StorageType S, IndexStyle I> inline void Inverse(
	  Matrix<T,S,I>& minv) const
      { BaseMatrix<T>::Inverse(minv.View()); }

      template <StorageType S, IndexStyle I> inline void InverseATA(
	  Matrix<T,S,I>& minv) const
      { BaseMatrix<T>::InverseATA(minv.View()); }

      inline BaseMatrix<T>* NewTranspose() const 
      { return new ConstMatrixView<T>(Transpose()); }

      inline BaseMatrix<T>* NewConjugate() const
      { return new ConstMatrixView<T>(Conjugate()); }

      inline BaseMatrix<T>* NewAdjoint() const
      { return new ConstMatrixView<T>(Adjoint()); }

      inline BaseMatrix<T>* NewView() const
      { return new ConstMatrixView<T>(View()); }

      inline BaseMatrix<T>* NewCopy() const
      { 
	if (isrm()) return new Matrix<T,RowMajor>(*this); 
	else return new Matrix<T,ColMajor>(*this);
      }

      // 
      // Division Control
      //

      using BaseMatrix<T>::GetDiv;

      inline void DivideUsing(DivType dt) const
      {
	TMVAssert(dt == LU || dt == QR || dt == QRP || 
	    SV_Type(dt));
	BaseMatrix<T>::DivideUsing(dt); 
      }

      inline const LUDiv<T>& LUD() const
      {
	TMVAssert(GetDiv());
	TMVAssert(dynamic_cast<const LUDiv<T>*>(GetDiv()));
	return *dynamic_cast<const LUDiv<T>*>(GetDiv());
      }

      inline const QRDiv<T>& QRD() const
      {
	TMVAssert(GetDiv());
	TMVAssert(dynamic_cast<const QRDiv<T>*>(GetDiv()));
	return *dynamic_cast<const QRDiv<T>*>(GetDiv());
      }

      inline const QRPDiv<T>& QRPD() const
      {
	TMVAssert(GetDiv());
	TMVAssert(dynamic_cast<const QRPDiv<T>*>(GetDiv()));
	return *dynamic_cast<const QRPDiv<T>*>(GetDiv());
      }

      inline const SVDiv<T>& SVD() const
      {
	TMVAssert(GetDiv());
	TMVAssert(dynamic_cast<const SVDiv<T>*>(GetDiv()));
	return *dynamic_cast<const SVDiv<T>*>(GetDiv());
      }


      //
      // I/O
      //

      void Write(ostream& os) const;
      void Write(ostream& os, RealType(T) minnonzero) const;

      virtual const T* cptr() const = 0;
      virtual int stepi() const = 0;
      virtual int stepj() const = 0;
      virtual inline StorageType stor() const { return itsstor; }
      virtual inline bool isrm() const { return itsstor == RowMajor; }
      virtual inline bool iscm() const { return itsstor == ColMajor; }
      virtual inline ConjItType ct() const { return itsct; }
      inline size_t ls() const { return linsize; }
      inline bool isconj() const 
      { 
	TMVAssert(IsComplex(T()) || itsct==NonConj);
	return IsComplex(T()) && itsct==Conj; 
      }

    protected :

      T cref(size_t i, size_t j) const;

      void NewDivider() const;

    private :

      const StorageType itsstor;
      const ConjItType itsct;
      mutable size_t linsize;

      void operator=(const GenMatrix<T>&) { TMVAssert(false); }

  }; // GenMatrix

  template <class T, IndexStyle I> class ConstMatrixView : 
    public GenMatrix<T>
  {
    public :

      ConstMatrixView(const ConstMatrixView<T,I>& rhs) :
	GenMatrix<T>(rhs),
	itsm(rhs.itsm), itscs(rhs.itscs), itsrs(rhs.itsrs), 
	itssi(rhs.itssi), itssj(rhs.itssj) {}

      ConstMatrixView(const GenMatrix<T>& rhs) :
	GenMatrix<T>(rhs),
	itsm(rhs.cptr()), itscs(rhs.colsize()), itsrs(rhs.rowsize()),
	itssi(rhs.stepi()), itssj(rhs.stepj()) {}

      ConstMatrixView(const T* _m, size_t _cs, size_t _rs, int _si, int _sj, 
	  StorageType instor, ConjItType inct, size_t linsize=0) : 
	GenMatrix<T>(instor,inct,linsize),
	itsm(_m), itscs(_cs), itsrs(_rs), itssi(_si), itssj(_sj) 
      { 
	TMVAssert(instor==RowMajor ? _sj==1 : instor==ColMajor ? _si==1 : true);
      }

      ~ConstMatrixView() {}

      inline size_t colsize() const { return itscs; }
      inline size_t rowsize() const { return itsrs; }
      inline const T* cptr() const { return itsm; }
      inline int stepi() const { return itssi; }
      inline int stepj() const { return itssj; }

    protected :

      const T*const itsm;
      const size_t itscs;
      const size_t itsrs;
      const int itssi;
      const int itssj;

      using GenMatrix<T>::cref;

    private :

      inline void operator=(const ConstMatrixView<T,I>&) { TMVAssert(false); }

  }; // ConstMatrixView

  template <class T> class ConstMatrixView<T,FortranStyle> : 
    public ConstMatrixView<T,CStyle>
  {
    public :

      ConstMatrixView(const ConstMatrixView<T>& rhs) :
	ConstMatrixView<T,CStyle>(rhs) {}

      ConstMatrixView(const GenMatrix<T>& rhs) :
	ConstMatrixView<T,CStyle>(rhs) {}

      ConstMatrixView(const T* _m, size_t _cs, size_t _rs, int _si, int _sj, 
	  StorageType instor, ConjItType inct, size_t linsize=0) : 
	ConstMatrixView<T,CStyle>(_m,_cs,_rs,_si,_sj,instor,inct,linsize) {}

      ~ConstMatrixView() {}


      // 
      // Access
      //
      
      inline T operator()(size_t i, size_t j)
      { 
	TMVAssert(i>0 && i<=colsize());
	TMVAssert(j>0 && j<=rowsize());
	return cref(i-1,j-1);
      }

      inline ConstVectorView<T,FortranStyle> row(size_t i) const 
      { 
	TMVAssert(i>0 && i<=colsize());
	return GenMatrix<T>::row(i-1);
      }

      inline ConstVectorView<T,FortranStyle> col(size_t j) const
      {
	TMVAssert(j>0 && j<=rowsize());
	return GenMatrix<T>::col(j-1);
      }

      inline ConstVectorView<T,FortranStyle> diag() const
      { return GenMatrix<T>::diag(); }

      inline ConstVectorView<T,FortranStyle> diag(int i) const
      { return GenMatrix<T>::diag(i); }

      inline ConstVectorView<T,FortranStyle> row(
	  size_t i, size_t j1, size_t j2) const 
      { 
	TMVAssert(i>0 && i<=colsize());
	TMVAssert(j1 > 0 && j1 <= j2);
	TMVAssert(j2 <= rowsize());
	return GenMatrix<T>::row(i-1,j1-1,j2);
      }

      inline ConstVectorView<T,FortranStyle> col(
	  size_t j, size_t i1, size_t i2) const
      {
	TMVAssert(j>0 && j<=rowsize());
	TMVAssert(i1 > 0 && i1 <= i2);
	TMVAssert(i2 <= colsize());
	return GenMatrix<T>::col(j-1,i1-1,i2);
      }

      inline ConstVectorView<T,FortranStyle> diag(int i, size_t j1, size_t j2) const
      {
	TMVAssert(j1 > 0);
	return GenMatrix<T>::diag(i,j1-1,j2);
      }

      inline ConstVectorView<T,FortranStyle> operator[](size_t i) const
      { return row(i); }

      //
      // SubMatrix
      //

      bool OKSubMatrix(
	  int i1, int i2, int j1, int j2, int istep, int jstep) const;

      inline ConstMatrixView<T,FortranStyle> SubMatrix(
	  int i1, int i2, int j1, int j2) const
      {
	TMVAssert(OKSubMatrix(i1,i2,j1,j2,1,1));
	return GenMatrix<T>::SubMatrix(i1-1,i2,j1-1,j2);
      }

      inline ConstMatrixView<T,FortranStyle> SubMatrix(
	  int i1, int i2, int j1, int j2, int istep, int jstep) const
      {
	TMVAssert(OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	return GenMatrix<T>::SubMatrix(
	    i1-1,i2-1+istep,j1-1,j2-1+jstep,istep,jstep);
      }

      bool OKSubVector(
	  size_t i, size_t j, int istep, int jstep, size_t s) const;

      inline ConstVectorView<T,FortranStyle> SubVector(
	  size_t i, size_t j, int istep, int jstep, size_t s) const
      {
	TMVAssert(OKSubVector(i,j,istep,jstep,s));
	return GenMatrix<T>::SubVector(i-1,j-1,istep,jstep,s);
      }

      inline ConstMatrixView<T,FortranStyle> ColPair(size_t j1, size_t j2) const
      {
	TMVAssert(j1 > 0 && j1 <= rowsize());
	TMVAssert(j2 > 0 && j2 <= rowsize());
	return GenMatrix<T>::ColPair(j1-1,j2-1);
      }

      inline ConstMatrixView<T,FortranStyle> RowPair(size_t i1, size_t i2) const
      {
	TMVAssert(i1 > 0 && i1 <= colsize());
	TMVAssert(i2 > 0 && i2 <= colsize());
	return GenMatrix<T>::RowPair(i1-1,i2-1);
      }

      inline ConstMatrixView<T,FortranStyle> Cols(size_t j1, size_t j2) const
      {
	TMVAssert(j1 > 0 && j1 <= j2);
	TMVAssert(j2 <= rowsize());
	return GenMatrix<T>::Cols(j1-1,j2);
      }

      inline ConstMatrixView<T,FortranStyle> Rows(size_t i1, size_t i2) const
      {
	TMVAssert(i1 > 0 && i1 <= i2);
	TMVAssert(i2 <= colsize());
	return GenMatrix<T>::Rows(i1-1,i2);
      }

      inline ConstMatrixView<RealType(T),FortranStyle> Real() const
      { return GenMatrix<T>::Real(); }

      inline ConstMatrixView<RealType(T),FortranStyle> Imag() const
      { return GenMatrix<T>::Imag(); }

      //
      // Views
      //

      inline ConstMatrixView<T,FortranStyle> View() const
      { return GenMatrix<T>::View(); }

      inline ConstMatrixView<T,FortranStyle> Transpose() const
      { return GenMatrix<T>::Transpose(); }
  
      inline ConstMatrixView<T,FortranStyle> Conjugate() const
      { return GenMatrix<T>::Conjugate(); }

      inline ConstMatrixView<T,FortranStyle> Adjoint() const
      { return GenMatrix<T>::Adjoint(); }

      inline ConstVectorView<T,FortranStyle> ConstLinearView() const
      { return GenMatrix<T>::ConstLinearView(); }

      inline MatrixView<T,FortranStyle> NonConst() const
      { return GenMatrix<T>::NonConst(); }

      using ConstMatrixView<T,CStyle>::colsize;
      using ConstMatrixView<T,CStyle>::rowsize;

    protected :

      using GenMatrix<T>::cref;

    private :

      inline void operator=(const ConstMatrixView<T,FortranStyle>&) 
      { TMVAssert(false); }

  }; // FortranStyle ConstMatrixView

  template <class T, IndexStyle I> class MatrixView : 
    public GenMatrix<T>
  {

    public:

      //
      // Constructors
      //

      MatrixView(const MatrixView<T,I>& rhs) : 
	GenMatrix<T>(rhs), itsm(rhs.itsm), itscs(rhs.itscs), itsrs(rhs.itsrs),
	itssi(rhs.itssi), itssj(rhs.itssj) DEFFIRSTLAST(rhs.first,rhs.last) {}

      MatrixView(T* _m, size_t _cs, size_t _rs, int _si, int _sj,
	  StorageType instor, ConjItType inct, size_t linsize
	  PARAMFIRSTLAST(T) ) :
	GenMatrix<T>(instor,inct,linsize), itsm(_m), itscs(_cs), itsrs(_rs), 
	itssi(_si), itssj(_sj) DEFFIRSTLAST(_first,_last)
      {
	TMVAssert(instor==RowMajor ? _sj==1 : instor==ColMajor ? _si==1 : true);
      }

      MatrixView(T* _m, size_t _cs, size_t _rs, int _si, int _sj,
	  StorageType instor, ConjItType inct
	  PARAMFIRSTLAST(T) ) :
	GenMatrix<T>(instor,inct,0), itsm(_m), itscs(_cs), itsrs(_rs), 
	itssi(_si), itssj(_sj) DEFFIRSTLAST(_first,_last)
      {
	TMVAssert(instor==RowMajor ? _sj==1 : instor==ColMajor ? _si==1 : true);
      }

      ~MatrixView() {} 

      //
      // Op=
      //

      inline const MatrixView<T,I>& operator=(const MatrixView<T,I>& m2) const
      { if (!SameAs(m2)) Copy(m2,*this); return *this; }

      inline const MatrixView<T,I>& operator=(const GenMatrix<T>& m2) const
      { if (!SameAs(m2)) Copy(m2,*this); return *this; }

      template <class T2> inline const MatrixView<T,I>& operator=(
	  const GenMatrix<T2>& m2) const
      { Copy(m2,*this); return *this; }

      template <class T2> inline const MatrixView<T,I>& operator=(
	  const BaseMatrix<T2>& m2) const
      { m2.CopyToMatrix(*this); return *this; }

      inline const MatrixView<T,I>& operator=(T x) const 
      { return SetToIdentity(x); }

      inline const MatrixView<T,I>& operator=(
	  const MatrixComposite<T>& mcomp) const
      { 
	TMVAssert(colsize() == mcomp.colsize());
	TMVAssert(rowsize() == mcomp.rowsize());
	mcomp.AssignTo(*this);
	return *this;
      }

      //
      // Access
      //
 
      inline RefType(T) operator()(size_t i,size_t j) const 
      { 
	TMVAssert(i<colsize());
	TMVAssert(j<rowsize());
	return ref(i,j); 
      }

      inline VectorView<T,I> operator[](size_t i) const 
      { 
	TMVAssert(i<colsize());
	return row(i); 
      }

      typedef RefType(T) reference;

      inline VectorView<T,I> row(size_t i) const
      {
	TMVAssert(i<colsize());
	return VectorView<T,I>(ptr()+i*stepi(),
	    rowsize(),stepj(),ct() FIRSTLAST ); 
      }

      inline VectorView<T,I> col(size_t j) const
      {
	TMVAssert(j<rowsize());
	return VectorView<T,I>(ptr()+j*stepj(),
	    colsize(),stepi(),ct() FIRSTLAST ); 
      }

      inline VectorView<T,I> diag() const
      {
	return VectorView<T,I>(ptr(),
	    min(colsize(),rowsize()),stepi()+stepj(),ct() FIRSTLAST);
      }

      inline VectorView<T,I> diag(int i) const
      {
	TMVAssert(i<=int(rowsize()));
	TMVAssert(-i<=int(colsize()));
	const size_t diagstep = stepi() + stepj();
	if (i >= 0) {
	  const size_t diagsize = min(colsize(),rowsize()-i);
	  return VectorView<T,I>(ptr()+i*stepj(),
	      diagsize,diagstep,ct() FIRSTLAST );
	} else {
	  const size_t diagsize = min(colsize()+i,rowsize());
	  return VectorView<T,I>(ptr()-i*stepi(),
	      diagsize,diagstep,ct() FIRSTLAST );
	}
      }

      inline VectorView<T,I> row(size_t i, size_t j1, size_t j2) const 
      { 
	TMVAssert(i<colsize());
	TMVAssert(j1<=j2);
	TMVAssert(j2<=rowsize());
	return VectorView<T,I>(ptr()+i*stepi()+j1*stepj(),
	    j2-j1,stepj(),ct() FIRSTLAST ); 
      }

      inline VectorView<T,I> col(size_t j, size_t i1, size_t i2) const
      {
	TMVAssert(j<rowsize());
	TMVAssert(i1<=i2);
	TMVAssert(i2<=colsize());
	return VectorView<T,I>(ptr()+i1*stepi()+j*stepj(),
	    i2-i1,stepi(),ct() FIRSTLAST ); 
      }

      inline VectorView<T,I> diag(int i, size_t j1, size_t j2) const
      {
	TMVAssert(i<=int(rowsize()));
	TMVAssert(-i<=int(colsize()));
	const size_t diagstep = stepi() + stepj();
	if (i >= 0) {
	  TMVAssert(j1 <= j2);
	  TMVAssert(j2 <= min(colsize(),rowsize()-i));
	  return VectorView<T,I>(ptr()+i*stepj()+j1*diagstep,
	      j2-j1,diagstep,ct() FIRSTLAST );
	} else {
	  TMVAssert(j1 <= j2);
	  TMVAssert(j2 <= min(colsize()+i,rowsize()));
	  return VectorView<T,I>(ptr()-i*stepi()+j1*diagstep,
	      j2-j1,diagstep,ct() FIRSTLAST );
	}
      }

      //
      // Modifying Functions
      //

      inline const MatrixView<T,I>& Zero() const { return SetAllTo(T(0)); }

      const MatrixView<T,I>& Clip(RealType(T) thresh) const;

      const MatrixView<T,I>& SetAllTo(T x) const;

      const MatrixView<T,I>& TransposeSelf() const;

      const MatrixView<T,I>& ConjugateSelf() const;

      const MatrixView<T,I>& SetToIdentity(T x=T(1)) const;

      const MatrixView<T,I>& SwapRows(size_t i1, size_t i2) const;

      const MatrixView<T,I>& SwapCols(size_t j1, size_t j2) const;

      const MatrixView<T,I>& PermuteRows(const size_t* p, 
	  size_t i1, size_t i2) const;

      inline const MatrixView<T,I>& PermuteRows(const size_t* p) const
      { return PermuteRows(p,0,colsize()); }

      inline const MatrixView<T,I>& PermuteCols(
	  const size_t* p, size_t j1, size_t j2) const
      { Transpose().PermuteRows(p,j1,j2); return *this; }

      inline const MatrixView<T,I>& PermuteCols(const size_t* p) const
      { return PermuteCols(p,0,rowsize()); }

      const MatrixView<T,I>& ReversePermuteRows(
	  const size_t* p, size_t i1, size_t i2) const;

      inline const MatrixView<T,I>& ReversePermuteRows(const size_t* p) const
      { return ReversePermuteRows(p,0,colsize()); }

      inline const MatrixView<T,I>& ReversePermuteCols(
	  const size_t* p, size_t j1, size_t j2) const
      { Transpose().ReversePermuteRows(p,j1,j2); return *this; }

      inline const MatrixView<T,I>& ReversePermuteCols(const size_t* p) const
      { return ReversePermuteCols(p,0,rowsize()); }

      //
      // SubMatrix
      //

      inline MatrixView<T,I> SubMatrix(int i1, int i2, int j1, int j2) const
      {
	TMVAssert(GenMatrix<T>::OKSubMatrix(i1,i2,j1,j2,1,1));
	return MatrixView<T,I>(ptr()+i1*stepi()+j1*stepj(),
	    i2-i1,j2-j1,stepi(),stepj(),stor(),ct() FIRSTLAST );
      }

      inline MatrixView<T,I> SubMatrix(
	  int i1, int i2, int j1, int j2, int istep, int jstep) const
      {
	StorageType newstor = 
	  iscm() ? (istep == 1 ? ColMajor : NoMajor) :
	  isrm() ? (jstep == 1 ? RowMajor : NoMajor) : NoMajor;
	TMVAssert(GenMatrix<T>::OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	return MatrixView<T,I>(ptr()+i1*stepi()+j1*stepj(),
	    (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
	    newstor,ct() FIRSTLAST );
      }

      inline VectorView<T,I> SubVector(
	  size_t i, size_t j, int istep, int jstep, size_t size) const
      {
	TMVAssert(GenMatrix<T>::OKSubVector(i,j,istep,jstep,size));
	return VectorView<T,I>(ptr()+i*stepi()+j*stepj(),size,
	    istep*stepi()+jstep*stepj(),ct() FIRSTLAST );
      }

      inline MatrixView<T,I> ColPair(size_t j1, size_t j2) const
      {
	StorageType newstor = 
	  iscm() ? ColMajor : 
	  isrm() ? (j2==j1+1 ? RowMajor : NoMajor) : NoMajor;
	TMVAssert(j1<rowsize() && j2<rowsize());
	return MatrixView<T,I>(ptr()+j1*stepj(),colsize(),2,
	    stepi(),(int(j2)-int(j1))*stepj(),newstor,ct() FIRSTLAST );
      }

      inline MatrixView<T,I> RowPair(size_t i1, size_t i2) const
      {
	StorageType newstor = 
	  isrm() ? RowMajor : 
	  iscm() ? (i2==i1+1 ? ColMajor : NoMajor) : NoMajor;
	TMVAssert(i1<colsize() && i2<colsize());
	return MatrixView<T,I>(ptr()+i1*stepi(),2,rowsize(),
	    (int(i2)-int(i1))*stepi(),stepj(),newstor,ct() FIRSTLAST );
      }

      inline MatrixView<T,I> Cols(size_t j1, size_t j2) const
      {
	TMVAssert(j1<=j2);
	TMVAssert(j2<=rowsize());
	return MatrixView<T,I>(ptr()+j1*stepj(),colsize(),j2-j1,
	    stepi(),stepj(),stor(),ct(),
	    (iscm()&&ls())?1:0 FIRSTLAST);
      }

      inline MatrixView<T,I> Rows(size_t i1, size_t i2) const
      {
	TMVAssert(i1<=i2);
	TMVAssert(i2<=colsize());
	return MatrixView<T,I>(ptr()+i1*stepi(),i2-i1,rowsize(),
	    stepi(),stepj(),stor(),ct(),
	    (isrm()&&ls())?1:0 FIRSTLAST);
      }

      inline MatrixView<RealType(T)> Real() const
      {
	return MatrixView<RealType(T)>(
	    reinterpret_cast<RealType(T)*>(ptr()),colsize(),rowsize(),
	    IsReal(T()) ? stepi() : 2*stepi(),
	    IsReal(T()) ? stepj() : 2*stepj(),
	    IsReal(T()) ? stor() : NoMajor,NonConj
#ifdef TMVFLDEBUG
	    ,reinterpret_cast<const RealType(T)*>(first)
	    ,reinterpret_cast<const RealType(T)*>(last)
#endif
	    );
      }

      inline MatrixView<RealType(T)> Imag() const
      {
	TMVAssert(IsComplex(T()));
	TMVAssert(ct()==NonConj);
	return MatrixView<RealType(T)>(
	    reinterpret_cast<RealType(T)*>(ptr())+1,
	    colsize(),rowsize(),2*stepi(),2*stepj(), NoMajor,NonConj
#ifdef TMVFLDEBUG
	    ,reinterpret_cast<const RealType(T)*>(first)+1
	    ,reinterpret_cast<const RealType(T)*>(last)+1
#endif
	    );
      }

      //
      // Views
      //

      inline MatrixView<T,I> View() const
      { return *this; }

      inline MatrixView<T,I> Transpose() const
      { 
	return MatrixView<T,I>(ptr(),rowsize(),colsize(),
	    stepj(),stepi(),TransOf(stor()),ct(),ls() FIRSTLAST);
      }
  
      inline MatrixView<T,I> Conjugate() const
      { 
	return MatrixView<T,I>(ptr(),colsize(),rowsize(),
	    stepi(),stepj(),stor(),ConjOf(T,ct()),ls() FIRSTLAST);
      }

      inline MatrixView<T,I> Adjoint() const
      { 
	return MatrixView<T,I>(ptr(),rowsize(),colsize(),
	    stepj(),stepi(),TransOf(stor()),ConjOf(T,ct()),ls() FIRSTLAST);
      }

      inline VectorView<T,I> LinearView() const
      {
	TMVAssert(ls() != 1 || (rowsize() == 1 && colsize() == 1));
	  // To assure that next Assert has no effect

	TMVAssert(GenMatrix<T>::CanLinearize());
	TMVAssert(ls() == colsize()*rowsize());
	return VectorView<T,I>(ptr(),ls(),1,ct() FIRSTLAST );
      }

      
      //
      // I/O
      //

      void Read(istream& is) const;

      inline size_t colsize() const { return itscs; }
      inline size_t rowsize() const { return itsrs; }
      using BaseMatrix<T>::IsSquare;
      inline const T* cptr() const { return itsm; }
      inline T* ptr() const { return itsm; }
      inline int stepi() const { return itssi; }
      inline int stepj() const { return itssj; }
      using GenMatrix<T>::stor;
      using GenMatrix<T>::isrm;
      using GenMatrix<T>::iscm;
      using GenMatrix<T>::ct;
      using GenMatrix<T>::isconj;
      using GenMatrix<T>::ls;

    protected:

      T*const itsm;
      const size_t itscs;
      const size_t itsrs;
      const int itssi;
      const int itssj;

#ifdef TMVFLDEBUG
    public:
      const T*const first;
      const T*const last;
    protected:
#endif

      RefType(T) ref(size_t i, size_t j) const;

  }; // MatrixView

  template <class T> class MatrixView<T,FortranStyle> : 
    public MatrixView<T,CStyle>
  {

    public:

      //
      // Constructors
      //

      MatrixView(const MatrixView<T,CStyle>& rhs) : 
	MatrixView<T,CStyle>(rhs) {}

      MatrixView(const MatrixView<T,FortranStyle>& rhs) : 
	MatrixView<T,CStyle>(rhs) {}

      MatrixView(T* _m, size_t _cs, size_t _rs, int _si, int _sj,
	  StorageType instor, ConjItType inct, size_t linsize
	  PARAMFIRSTLAST(T) ) :
	MatrixView<T,CStyle>(_m,_cs,_rs,_si,_sj,instor,inct,linsize 
	    FIRSTLAST1(_first,_last) ) {}

      MatrixView(T* _m, size_t _cs, size_t _rs, int _si, int _sj,
	  StorageType instor, ConjItType inct
	  PARAMFIRSTLAST(T) ) :
	MatrixView<T,CStyle>(_m,_cs,_rs,_si,_sj,instor,inct 
	    FIRSTLAST1(_first,_last) ) {}

      ~MatrixView() {} 

      //
      // Op=
      //

      inline const MatrixView<T,FortranStyle>& operator=(
	  const MatrixView<T,FortranStyle>& m2) const
      { MatrixView<T,CStyle>::operator=(m2); return *this; }

      inline const MatrixView<T,FortranStyle>& operator=(
	  const GenMatrix<T>& m2) const
      { MatrixView<T,CStyle>::operator=(m2); return *this; }

      template <class T2> inline const MatrixView<T,FortranStyle>& operator=(
	  const GenMatrix<T2>& m2) const
      { MatrixView<T,CStyle>::operator=(m2); return *this; }

      template <class T2> inline const MatrixView<T,FortranStyle>& operator=(
	  const BaseMatrix<T2>& m2) const
      { MatrixView<T,CStyle>::operator=(m2); return *this; }

      inline const MatrixView<T,FortranStyle>& operator=(T x) const 
      { MatrixView<T,CStyle>::operator=(x); return *this; }

      inline const MatrixView<T,FortranStyle>& operator=(
	  const MatrixComposite<T>& mcomp) const
      { MatrixView<T,CStyle>::operator=(mcomp); return *this; }

      //
      // Access
      //
 
      inline RefType(T) operator()(size_t i,size_t j) const 
      { 
	TMVAssert(i > 0 && i <= colsize());
	TMVAssert(j > 0 && j <= rowsize());
	return ref(i-1,j-1); 
      }

      inline VectorView<T,FortranStyle> operator[](size_t i) const 
      { 
	TMVAssert(i>0 && i<=colsize());
	return row(i); 
      }

      inline VectorView<T,FortranStyle> row(size_t i) const
      {
	TMVAssert(i>0 && i<=colsize());
	return MatrixView<T,CStyle>::row(i-1);
      }

      inline VectorView<T,FortranStyle> col(size_t j) const
      {
	TMVAssert(j>0 && j<=rowsize());
	return MatrixView<T,CStyle>::col(j-1);
      }

      inline VectorView<T,FortranStyle> diag() const
      { return MatrixView<T,CStyle>::diag(); }

      inline VectorView<T,FortranStyle> diag(int i) const
      { return MatrixView<T,CStyle>::diag(i); }

      inline VectorView<T,FortranStyle> row(
	  size_t i, size_t j1, size_t j2) const 
      { 
	TMVAssert(i>0 && i<=colsize());
	TMVAssert(j1>0 && j1<=j2 && j2<=rowsize());
	return MatrixView<T,CStyle>::row(i-1,j1-1,j2);
      }

      inline VectorView<T,FortranStyle> col(
	  size_t j, size_t i1, size_t i2) const
      {
	TMVAssert(j>0 && j<=rowsize());
	TMVAssert(i1>0 && i1<=i2 && i2<=colsize());
	return MatrixView<T,CStyle>::col(j-1,i1-1,i2);
      }

      inline VectorView<T,FortranStyle> diag(int i, size_t j1, size_t j2) const
      {
	TMVAssert(j1 > 0);
	return MatrixView<T,CStyle>::diag(i,j1-1,j2);
      }

      //
      // Modifying Functions
      //

      inline const MatrixView<T,FortranStyle>& Zero() const 
      { MatrixView<T,CStyle>::Zero(); return *this; }

      inline const MatrixView<T,FortranStyle>& Clip(RealType(T) thresh) const
      { MatrixView<T,CStyle>::Clip(thresh); return *this; }

      inline const MatrixView<T,FortranStyle>& SetAllTo(T x) const
      { MatrixView<T,CStyle>::SetAllTo(x); return *this; }

      inline const MatrixView<T,FortranStyle>& TransposeSelf() const
      { MatrixView<T,CStyle>::TransposeSelf(); return *this; }

      inline const MatrixView<T,FortranStyle>& ConjugateSelf() const
      { MatrixView<T,CStyle>::ConjugateSelf(); return *this; }

      inline const MatrixView<T,FortranStyle>& SetToIdentity(T x=T(1)) const
      { MatrixView<T,CStyle>::SetToIdentity(x); return *this; }

      inline const MatrixView<T,FortranStyle>& SwapRows(
	  size_t i1, size_t i2) const
      { 
	TMVAssert(i1 > 0 && i1 <= colsize());
	TMVAssert(i2 > 0 && i2 <= colsize());
	if (i1 != i2)
	  MatrixView<T,CStyle>::SwapRows(i1-1,i2-1); 
	return *this; 
      }

      inline const MatrixView<T,FortranStyle>& SwapCols(
	  size_t j1, size_t j2) const
      { 
	TMVAssert(j1 > 0 && j1 <= rowsize());
	TMVAssert(j2 > 0 && j2 <= rowsize());
	if (j1 != j2)
	  MatrixView<T,CStyle>::SwapCols(j1-1,j2-1); 
	return *this; 
      }

      inline const MatrixView<T,FortranStyle>& PermuteRows(
	  const size_t* p, size_t i1, size_t i2) const
      {
	TMVAssert(i1>0);
	MatrixView<T,CStyle>::PermuteRows(p,i1-1,i2);
	return *this; 
      }

      inline const MatrixView<T,FortranStyle>& PermuteRows(
	  const size_t* p) const
      { MatrixView<T,CStyle>::PermuteRows(p); return *this; }

      inline const MatrixView<T,FortranStyle>& PermuteCols(
	  const size_t* p, size_t j1, size_t j2) const
      { Transpose().PermuteRows(p,j1,j2); return *this; }

      inline const MatrixView<T,FortranStyle>& PermuteCols(
	  const size_t* p) const
      { Transpose().PermuteRows(p); return *this; }

      inline const MatrixView<T,FortranStyle>& ReversePermuteRows(
	  const size_t* p, size_t i1, size_t i2) const
      { 
	TMVAssert(i1>0);
	MatrixView<T,CStyle>::ReversePermuteRows(p,i1-1,i2);  
	return *this;
      }

      inline const MatrixView<T,FortranStyle>& ReversePermuteRows(
	  const size_t* p) const
      { MatrixView<T,CStyle>::ReversePermuteRows(p); return *this; }

      inline const MatrixView<T,FortranStyle>& ReversePermuteCols(
	  const size_t* p, size_t j1, size_t j2) const
      { Transpose().ReversePermuteRows(p,j1,j2); return *this; }

      inline const MatrixView<T,FortranStyle>& ReversePermuteCols(
	  const size_t* p) const
      { Transpose().ReversePermuteRows(p); return *this; }


      //
      // SubMatrix
      //

      inline bool OKSubMatrix(
	  int i1, int i2, int j1, int j2, int istep, int jstep) const
      {
	return ConstMatrixView<T,FortranStyle>(*this).OKSubMatrix(
	    i1,i2,j1,j2,istep,jstep); 
      }

      inline bool OKSubVector(
	  size_t i, size_t j, int istep, int jstep, size_t s) const
      {
	return ConstMatrixView<T,FortranStyle>(*this).OKSubVector(
	    i,j,istep,jstep,s); 
      }

      inline MatrixView<T,FortranStyle> SubMatrix(
	  int i1, int i2, int j1, int j2) const
      {
	TMVAssert(OKSubMatrix(i1,i2,j1,j2,1,1));
	return MatrixView<T,CStyle>::SubMatrix(i1-1,i2,j1-1,j2);
      }

      inline MatrixView<T,FortranStyle> SubMatrix(
	  int i1, int i2, int j1, int j2, int istep, int jstep) const
      {
	TMVAssert(OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	return MatrixView<T,CStyle>::SubMatrix(i1-1,i2-1+istep,j1-1,j2-1+jstep,
	      istep,jstep);
      }

      inline VectorView<T,FortranStyle> SubVector(
	  size_t i, size_t j, int istep, int jstep, size_t s) const
      {
	TMVAssert(OKSubVector(i,j,istep,jstep,s));
	return MatrixView<T,CStyle>::SubVector(i-1,j-1,istep,jstep,s);
      }

      inline MatrixView<T,FortranStyle> ColPair(size_t j1, size_t j2) const
      {
	TMVAssert(j1 > 0 && j1 <= rowsize());
	TMVAssert(j2 > 0 && j2 <= rowsize());
	return MatrixView<T,CStyle>::ColPair(j1-1,j2-1);
      }

      inline MatrixView<T,FortranStyle> RowPair(size_t i1, size_t i2) const
      {
	TMVAssert(i1 > 0 && i1 <= rowsize());
	TMVAssert(i2 > 0 && i2 <= rowsize());
	return MatrixView<T,CStyle>::RowPair(i1-1,i2-1);
      }

      inline MatrixView<T,FortranStyle> Cols(size_t j1, size_t j2) const
      {
	TMVAssert(j1 > 0 && j1 <= j2);
	TMVAssert(j2 <= rowsize());
	return MatrixView<T,CStyle>::Cols(j1-1,j2);
      }

      inline MatrixView<T,FortranStyle> Rows(size_t i1, size_t i2) const
      {
	TMVAssert(i1 > 0 && i1 <= i2);
	TMVAssert(i2 <= colsize());
	return MatrixView<T,CStyle>::Rows(i1-1,i2);
      }

      inline MatrixView<RealType(T),FortranStyle> Real() const
      { return MatrixView<T,CStyle>::Real(); }

      inline MatrixView<RealType(T),FortranStyle> Imag() const
      { return MatrixView<T,CStyle>::Imag(); }

      //
      // Views
      //

      inline MatrixView<T,FortranStyle> View() const
      { return MatrixView<T,CStyle>::View(); }

      inline MatrixView<T,FortranStyle> Transpose() const
      { return MatrixView<T,CStyle>::Transpose(); }
  
      inline MatrixView<T,FortranStyle> Conjugate() const
      { return MatrixView<T,CStyle>::Conjugate(); }

      inline MatrixView<T,FortranStyle> Adjoint() const
      { return MatrixView<T,CStyle>::Adjoint(); }

      inline VectorView<T,FortranStyle> LinearView() const
      { return MatrixView<T,CStyle>::LinearView(); }

      using MatrixView<T,CStyle>::colsize;
      using MatrixView<T,CStyle>::rowsize;

    protected:

      using MatrixView<T,CStyle>::ref;

  }; // FortranStyle MatrixView

  template <class T, StorageType S, IndexStyle I> class Matrix : 
    public GenMatrix<T> 
  {

    public:

      //
      // Constructors
      //

#define NEW_SIZE(cs,rs) GenMatrix<T>(S,NonConj,((cs)*(rs))), \
      itsm(new T[ls()]), itscs(cs), itsrs(rs) \
      DEFFIRSTLAST(itsm,itsm+ls())

      Matrix(size_t _colsize, size_t _rowsize) :
	NEW_SIZE(_colsize,_rowsize)
      {
	TMVAssert(S==RowMajor || S==ColMajor);
#ifdef TMVDEBUG
	SetAllTo(T(888));
#endif
      }

      Matrix(size_t _colsize, size_t _rowsize, T x) :
	NEW_SIZE(_colsize,_rowsize) 
      {
	TMVAssert(S==RowMajor || S==ColMajor);
	SetAllTo(x);
      }

      Matrix(size_t _colsize, size_t _rowsize, const valarray<T>& vv) : 
	NEW_SIZE(_colsize,_rowsize) 
      { 
	TMVAssert(S==RowMajor || S==ColMajor);
	TMVAssert(vv.size() == ls());
	T* vi=itsm;
	for(size_t i=0;i<ls();++i) *vi=vv[i];
      }

      Matrix(size_t _colsize,size_t _rowsize, const T* vv) : 
	NEW_SIZE(_colsize,_rowsize) 
      { 
	TMVAssert(S==RowMajor || S==ColMajor);
	memmove(itsm,vv,ls()*sizeof(T)); 
      }

      Matrix(size_t _colsize, size_t _rowsize, const vector<T>& vv) : 
	NEW_SIZE(_colsize,_rowsize) 
      { 
	TMVAssert(S==RowMajor || S==ColMajor);
	TMVAssert(vv.size() == ls());
	T* vi=itsm;
	typename vector<T>::const_iterator vvi = vv.begin();
	for(size_t i=ls();i>0;--i) *vi = *vvi;
      }

      explicit Matrix(const vector<vector<T> >& vv);

      Matrix(const Matrix<T,S,I>& rhs) :
	NEW_SIZE(rhs.colsize(),rhs.rowsize())
      { 
	TMVAssert(S==RowMajor || S==ColMajor);
	memmove(itsm,rhs.itsm,ls()*sizeof(T)); 
      }

      template <IndexStyle I2> Matrix(const Matrix<T,S,I2>& rhs) :
	NEW_SIZE(rhs.colsize(),rhs.rowsize())
      { 
	TMVAssert(S==RowMajor || S==ColMajor);
	memmove(itsm,rhs.cptr(),ls()*sizeof(T)); 
      }

      template <class T2> Matrix(const GenMatrix<T2>& rhs) :
	NEW_SIZE(rhs.colsize(),rhs.rowsize()) 
      { 
	TMVAssert(S==RowMajor || S==ColMajor);
	Copy(rhs,View()); 
      }

      template <class T2> Matrix(const BaseMatrix<T2>& rhs) : 
	  NEW_SIZE(rhs.colsize(),rhs.rowsize()) 
      { 
	TMVAssert(S==RowMajor || S==ColMajor);
	rhs.CopyToMatrix(View()); 
      }

      Matrix(const MatrixComposite<T>& mcomp) :
	NEW_SIZE(mcomp.colsize(),mcomp.rowsize())
      { 
	TMVAssert(S==RowMajor || S==ColMajor);
	mcomp.AssignTo(View()); 
      }

#undef NEW_SIZE

      ~Matrix() { TMVAssert(itsm); delete[] itsm; }

      //
      // Op=
      //

      inline Matrix<T,S,I>& operator=(const Matrix<T,S,I>& m2)
      { 
	TMVAssert(m2.colsize() == colsize() && m2.rowsize()==rowsize());
	if (&m2 != this) memmove(itsm,m2.itsm,ls()*sizeof(T));
	return *this; 
      }

      template <IndexStyle I2> inline Matrix<T,S,I>& operator=(
	  const Matrix<T,S,I2>& m2)
      { 
	TMVAssert(m2.colsize() == colsize() && m2.rowsize()==rowsize());
	memmove(itsm,m2.cptr(),ls()*sizeof(T));
	return *this; 
      }

      template <class T2> inline Matrix<T,S,I>& operator=(
	  const GenMatrix<T2>& m2)
      { Copy(m2,View()); return *this; }

      template <class T2> inline Matrix<T,S,I>& operator=(
	  const BaseMatrix<T2>& m2)
      { m2.CopyToMatrix(View()); return *this; }

      inline Matrix<T,S,I>& operator=(T x) { return SetToIdentity(x); }

      inline Matrix<T,S,I>& operator=(const MatrixComposite<T>& mcomp)
      { mcomp.AssignTo(View()); return *this; }


      //
      // Access
      //

      inline T operator()(size_t i,size_t j) const
      {
	if (I == CStyle) {
	  TMVAssert(i < colsize());
	  TMVAssert(j < rowsize());
	  return cref(i,j); 
	} else {
	  TMVAssert(i > 0 && i <= colsize());
	  TMVAssert(j > 0 && j <= rowsize());
	  return cref(i-1,j-1); 
	}
      }

      inline T& operator()(size_t i,size_t j) 
      {
	if (I == CStyle) {
	  TMVAssert(i < colsize());
	  TMVAssert(j < rowsize());
	  return ref(i,j); 
	} else {
	  TMVAssert(i > 0 && i <= colsize());
	  TMVAssert(j > 0 && j <= rowsize());
	  return ref(i-1,j-1); 
	}
      }

      inline ConstVectorView<T,I> row(size_t i) const 
      { 
	if (I == FortranStyle) { TMVAssert(i>0); }
	const size_t ix = (I == CStyle ? i : i-1);
	TMVAssert(ix<colsize());
	return ConstVectorView<T,I>(cptr()+ix*stepi(),
	    rowsize(),stepj(),NonConj); 
      }

      inline ConstVectorView<T,I> row(size_t i, size_t j1, size_t j2) const 
      { 
	if (I == FortranStyle) { TMVAssert(i>0); TMVAssert(j1>0); }
	const size_t ix = (I == CStyle ? i : i-1);
	const size_t j1x = (I == CStyle ? j1 : j1-1);
	TMVAssert(ix<colsize());
	TMVAssert(j1x<=j2 && j2<=rowsize());
	return ConstVectorView<T,I>(cptr()+ix*stepi()+j1x*stepj(),
	    j2-j1x,stepj(),NonConj); 
      }

      inline ConstVectorView<T,I> operator[](size_t i) const
      { return row(i); }

      inline ConstVectorView<T,I> col(size_t j) const
      {
	if (I == FortranStyle) { TMVAssert(j>0); }
	const size_t jx = (I == CStyle ? j : j-1);
	TMVAssert(jx<rowsize());
	return ConstVectorView<T,I>(cptr()+jx*stepj(),
	    colsize(),stepi(),NonConj); 
      }

      inline ConstVectorView<T,I> col(size_t j, size_t i1, size_t i2) const
      {
	if (I == FortranStyle) { TMVAssert(j>0); TMVAssert(i1>0); }
	const size_t jx = (I == CStyle ? j : j-1);
	const size_t i1x = (I == CStyle ? i1 : i1-1);
	TMVAssert(jx<rowsize());
	TMVAssert(i1x<=i2 && i2<=colsize());
	return ConstVectorView<T,I>(cptr()+i1x*stepi()+jx*stepj(),
	      i2-i1x,stepi(),NonConj); 
      }

      inline ConstVectorView<T,I> diag() const
      {
	return ConstVectorView<T,I>(cptr(),min(colsize(),rowsize()),
	      stepi()+stepj(),NonConj);
      }

      inline ConstVectorView<T,I> diag(int i) const
      {
	TMVAssert(i<=int(rowsize()));
	TMVAssert(-i<=int(colsize()));
	const size_t diagstep = stepi() + stepj();
	if (i >= 0) {
	  const size_t diagsize = min(colsize(),rowsize()-i);
	  return ConstVectorView<T,I>(cptr()+i*stepj(),
	      diagsize,diagstep,NonConj);
	} else {
	  const size_t diagsize = min(colsize()+i,rowsize());
	  return ConstVectorView<T,I>(cptr()-i*stepi(),
	      diagsize,diagstep,NonConj);
	}
      }

      inline ConstVectorView<T,I> diag(int i, size_t j1, size_t j2) const
      {
	if (I == FortranStyle) { TMVAssert(j1 > 0); }
	const size_t j1x = (I==CStyle ? j1 : j1-1);
	TMVAssert(i<=int(rowsize()));
	TMVAssert(-i<=int(colsize()));
	const size_t diagstep = stepi() + 1;
	if (i >= 0) {
	  TMVAssert(j1 <= j2);
	  TMVAssert(j2 <= min(colsize(),rowsize()-i));
	  return ConstVectorView<T,I>(cptr()+i*stepj() + j1x*diagstep,
	      j2-j1x, diagstep, NonConj);
	} else {
	  TMVAssert(j1 <= j2);
	  TMVAssert(j2 <= min(colsize()+i,rowsize()));
	  return ConstVectorView<T,I>(cptr()-i*stepi() + j1x*diagstep,
	      j2-j1x, diagstep, NonConj);
	}
      }

      inline VectorView<T,I> row(size_t i)
      { 
	if (I == FortranStyle) { TMVAssert(i>0); }
	const size_t ix = (I == CStyle ? i : i-1);
	TMVAssert(ix<colsize());
	return VectorView<T,I>(ptr()+ix*stepi(),
	    rowsize(),stepj(),NonConj FIRSTLAST); 
      }

      inline VectorView<T,I> row(size_t i, size_t j1, size_t j2)
      { 
	if (I == FortranStyle) { TMVAssert(i>0); TMVAssert(j1>0); }
	const size_t ix = (I == CStyle ? i : i-1);
	const size_t j1x = (I == CStyle ? j1 : j1-1);
	TMVAssert(ix<colsize());
	TMVAssert(j1x<=j2 && j2<=rowsize());
	return VectorView<T,I>(ptr()+ix*stepi()+j1x*stepj(),
	    j2-j1x,stepj(),NonConj FIRSTLAST); 
      }

      inline VectorView<T,I> operator[](size_t i)
      { return row(i); }

      inline VectorView<T,I> col(size_t j)
      {
	if (I == FortranStyle) { TMVAssert(j>0); }
	const size_t jx = (I == CStyle ? j : j-1);
	TMVAssert(jx<rowsize());
	return VectorView<T,I>(ptr()+jx*stepj(),
	    colsize(),stepi(),NonConj FIRSTLAST); 
      }

      inline VectorView<T,I> col(size_t j, size_t i1, size_t i2)
      {
	if (I == FortranStyle) { TMVAssert(j>0); TMVAssert(i1>0); }
	const size_t jx = (I == CStyle ? j : j-1);
	const size_t i1x = (I == CStyle ? i1 : i1-1);
	TMVAssert(jx<rowsize());
	TMVAssert(i1x<=i2 && i2<=colsize());
	return VectorView<T,I>(ptr()+i1x*stepi()+jx*stepj(),
	    i2-i1x,stepi(),NonConj FIRSTLAST); 
      }

      inline VectorView<T,I> diag()
      {
	return VectorView<T,I>(ptr(),
	    min(colsize(),rowsize()),stepi()+stepj(),NonConj FIRSTLAST);
      }

      inline VectorView<T,I> diag(int i)
      {
	TMVAssert(i<=int(rowsize()));
	TMVAssert(-i<=int(colsize()));
	const size_t diagstep = stepi() + stepj();
	if (i >= 0) {
	  const size_t diagsize = min(colsize(),rowsize()-i);
	  return VectorView<T,I>(ptr()+i*stepj(),
	      diagsize,diagstep,NonConj FIRSTLAST);
	} else {
	  const size_t diagsize = min(colsize()+i,rowsize());
	  return VectorView<T,I>(ptr()-i*stepi(),
	      diagsize,diagstep,NonConj FIRSTLAST);
	}
      }

      inline VectorView<T,I> diag(int i, size_t j1, size_t j2) 
      {
	if (I == FortranStyle) { TMVAssert(j1 > 0); }
	const size_t j1x = (I==CStyle ? j1 : j1-1);
	TMVAssert(i<=int(rowsize()));
	TMVAssert(-i<=int(colsize()));
	const size_t diagstep = stepi() + stepj();
	if (i >= 0) {
	  TMVAssert(j1 <= j2);
	  TMVAssert(j2 <= min(colsize(),rowsize()-i));
	  return VectorView<T,I>(ptr()+i*stepj() + j1x*diagstep,
	      j2-j1x, diagstep, NonConj FIRSTLAST);
	} else {
	  TMVAssert(j1 <= j2);
	  TMVAssert(j2 <= min(colsize()+i,rowsize()));
	  return VectorView<T,I>(ptr()-i*stepi() + j1x*diagstep,
	      j2-j1x, diagstep, NonConj FIRSTLAST);
	}
      }

      //
      // Modifying Functions
      //

      inline Matrix<T,S,I>& Zero() 
      { LinearView().SetAllTo(0); return *this; }

      inline Matrix<T,S,I>& Clip(RealType(T) thresh)
      { LinearView().Clip(thresh); return *this; }

      inline Matrix<T,S,I>& SetAllTo(T x) 
      { LinearView().SetAllTo(x); return *this; }

      inline Matrix<T,S,I>& TransposeSelf() 
      { View().TransposeSelf(); return *this; }

      inline Matrix<T,S,I>& ConjugateSelf() 
      { LinearView().ConjugateSelf(); return *this; }

      inline Matrix<T,S,I>& SetToIdentity(T x=T(1)) 
      { 
	TMVAssert(IsSquare());
	Zero(); diag().SetAllTo(x);
	return *this; 
      }

      inline Matrix<T,S,I>& SwapRows(size_t i1, size_t i2)
      { 
	if (I == CStyle) { TMVAssert(i1<colsize() && i2<colsize()); }
	else { TMVAssert(i1>0 && i1<=colsize() && i2>0 && i2<=colsize()); }
	if (i1!=i2) Swap(row(i1),row(i2));
	return *this; 
      }

      inline Matrix<T,S,I>& SwapCols(size_t j1, size_t j2)
      { 
	if (I == CStyle) { TMVAssert(j1<rowsize() && j2<rowsize()); } 
	else { TMVAssert(j1>0 && j1<=rowsize() && j2>0 && j2<=rowsize()); }
	if (j1!=j2) Swap(col(j1),col(j2));
	return *this; 
      }

      inline Matrix<T,S,I>& PermuteRows(const size_t* p, size_t i1, size_t i2)
      { View().PermuteRows(p,i1,i2); return *this; }

      inline Matrix<T,S,I>& PermuteRows(const size_t* p)
      { View().PermuteRows(p); return *this; }

      inline Matrix<T,S,I>& PermuteCols(const size_t* p, size_t j1, size_t j2)
      { View().PermuteCols(p,j1,j2); return *this; }

      inline Matrix<T,S,I>& PermuteCols(const size_t* p)
      { View().PermuteCols(p); return *this; }

      inline Matrix<T,S,I>& ReversePermuteRows(
	  const size_t* p, size_t i1, size_t i2)
      { View().ReversePermuteRows(p,i1,i2); return *this; }

      inline Matrix<T,S,I>& ReversePermuteRows(const size_t* p)
      { View().ReversePermuteRows(p); return *this; }

      inline Matrix<T,S,I>& ReversePermuteCols(
	  const size_t* p, size_t j1, size_t j2)
      { View().ReversePermuteCols(p,j1,j2); return *this; }

      inline Matrix<T,S,I>& ReversePermuteCols(const size_t* p)
      { View().ReversePermuteCols(p); return *this; }

      //
      // SubMatrix
      //

      inline ConstMatrixView<T,I> SubMatrix(
	  int i1, int i2, int j1, int j2) const
      {
	TMVAssert(View().OKSubMatrix(i1,i2,j1,j2,1,1));
	const int i1x = (I==CStyle ? i1 : i1-1);
	const int j1x = (I==CStyle ? j1 : j1-1);
	return ConstMatrixView<T,I>(cptr()+i1x*stepi()+j1x*stepj(),
	    i2-i1x,j2-j1x,stepi(),stepj(),S,NonConj);
      }

      inline ConstMatrixView<T,I> SubMatrix(
	  int i1, int i2, int j1, int j2, int istep, int jstep) const
      {
	TMVAssert(View().OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	StorageType newstor = S == RowMajor ?
	  jstep == 1 ? RowMajor : NoMajor :
	  istep == 1 ? ColMajor : NoMajor;
	const int i1x = (I==CStyle ? i1 : i1-1);
	const int j1x = (I==CStyle ? j1 : j1-1);
	const int i2x = (I==CStyle ? i2 : i2-1+istep);
	const int j2x = (I==CStyle ? j2 : j2-1+jstep);
	return ConstMatrixView<T,I>(cptr()+i1x*stepi()+j1x*stepj(),
	    (i2x-i1x)/istep,(j2x-j1x)/jstep,istep*stepi(),jstep*stepj(), 
	    newstor,NonConj);
      }

      inline ConstVectorView<T,I> SubVector(
	  size_t i, size_t j, int istep, int jstep, size_t s) const
      {
	TMVAssert(View().OKSubVector(i,j,istep,jstep,s));
	const int ix = (I==CStyle ? i : i-1);
	const int jx = (I==CStyle ? j : j-1);
	return ConstVectorView<T,I>(cptr()+ix*stepi()+jx*stepj(),s,
	    istep*stepi()+jstep*stepj(),NonConj);
      }

      inline ConstMatrixView<T,I> ColPair(size_t j1, size_t j2) const
      {
	if (I == CStyle) { TMVAssert(j1<rowsize() && j2<rowsize()); }
	else { TMVAssert(j1>0 && j1<=rowsize() && j2>0 && j2<=rowsize()); }
	StorageType newstor = S == RowMajor ?
	  j2==j1+1 ? RowMajor : NoMajor : ColMajor;
	const int j1x = (I==CStyle ? j1 : j1-1);
	return ConstMatrixView<T,I>(cptr()+j1x*stepj(),colsize(),2,
	    stepi(),(int(j2)-int(j1))*stepj(),newstor,NonConj);
      }

      inline ConstMatrixView<T,I> RowPair(size_t i1, size_t i2) const
      {
	if (I == CStyle) { TMVAssert(i1<colsize() && i2<colsize()); }
	else { TMVAssert(i1>0 && i1<=colsize() && i2>0 && i2<=colsize()); }
	StorageType newstor = S == RowMajor ?
	  RowMajor : i2==i1+1 ? ColMajor : NoMajor;
	const int i1x = (I==CStyle ? i1 : i1-1);
	return ConstMatrixView<T,I>(cptr()+i1x*stepi(),2,rowsize(),
	    (int(i2)-int(i1))*stepi(),stepj(),newstor,NonConj);
      }

      inline ConstMatrixView<T,I> Cols(size_t j1, size_t j2) const
      {
	if (I==FortranStyle) { TMVAssert(j1>0); }
	TMVAssert(j1<=j2);
	TMVAssert(j2<=rowsize());
	const int j1x = (I==CStyle ? j1 : j1-1);
	return ConstMatrixView<T,I>(cptr()+j1x*stepj(),colsize(),j2-j1x,
	    stepi(),stepj(),S,NonConj,iscm()?1:0);
      }

      inline ConstMatrixView<T,I> Rows(size_t i1, size_t i2) const
      {
	if (I==FortranStyle) { TMVAssert(i1>0); }
	TMVAssert(i1<=i2);
	TMVAssert(i2<=colsize());
	const int i1x = (I==CStyle ? i1 : i1-1);
	return ConstMatrixView<T,I>(cptr()+i1x*stepi(),i2-i1x,rowsize(),
	    stepi(),stepj(),S,NonConj,isrm()?1:0);
      }

      inline ConstMatrixView<RealType(T)> Real() const
      {
	return ConstMatrixView<RealType(T)>(
	    reinterpret_cast<const RealType(T)*>(cptr()),colsize(),rowsize(),
	    IsReal(T()) ? stepi() : 2*stepi(),
	    IsReal(T()) ? stepj() : 2*stepj(),
	    IsReal(T()) ? S : NoMajor,NonConj);
      }

      inline ConstMatrixView<RealType(T)> Imag() const
      {
	TMVAssert(IsComplex(T()));
	return ConstMatrixView<RealType(T)>(
	    reinterpret_cast<const RealType(T)*>(cptr())+1,
	    colsize(),rowsize(),2*stepi(),2*stepj(),NoMajor,NonConj);
      }
      inline MatrixView<T,I> SubMatrix(int i1, int i2, int j1, int j2)
      {
	TMVAssert(View().OKSubMatrix(i1,i2,j1,j2,1,1));
	const int i1x = (I==CStyle ? i1 : i1-1);
	const int j1x = (I==CStyle ? j1 : j1-1);
	return MatrixView<T,I>(ptr()+i1x*stepi()+j1x*stepj(),
	    i2-i1x,j2-j1x,stepi(),stepj(),S,NonConj FIRSTLAST);
      }

      inline MatrixView<T,I> SubMatrix(
	  int i1, int i2, int j1, int j2, int istep, int jstep) 
      {
	TMVAssert(View().OKSubMatrix(i1,i2,j1,j2,istep,jstep));
	StorageType newstor = S == RowMajor ?
	  jstep == 1 ? RowMajor : NoMajor :
	  istep == 1 ? ColMajor : NoMajor;
	const int i1x = (I==CStyle ? i1 : i1-1);
	const int j1x = (I==CStyle ? j1 : j1-1);
	const int i2x = (I==CStyle ? i2 : i2-1+istep);
	const int j2x = (I==CStyle ? j2 : j2-1+jstep);
	return MatrixView<T,I>(ptr()+i1x*stepi()+j1x*stepj(),
	    (i2x-i1x)/istep,(j2x-j1x)/jstep,istep*stepi(),jstep*stepj(), 
	    newstor,NonConj FIRSTLAST);
      }

      inline VectorView<T,I> SubVector(
	  size_t i, size_t j, int istep, int jstep, size_t s) 
      {
	TMVAssert(View().OKSubVector(i,j,istep,jstep,s));
	const int ix = (I==CStyle ? i : i-1);
	const int jx = (I==CStyle ? j : j-1);
	return VectorView<T,I>(ptr()+ix*stepi()+jx*stepj(),s,
	    istep*stepi()+jstep*stepj(),NonConj FIRSTLAST);
      }

      inline MatrixView<T,I> ColPair(size_t j1, size_t j2) 
      {
	if (I == CStyle) { TMVAssert(j1<rowsize() && j2<rowsize()); }
	else { TMVAssert(j1>0 && j1<=rowsize() && j2>0 && j2<=rowsize()); }
	StorageType newstor = S == RowMajor ?
	  j2==j1+1 ? RowMajor : NoMajor : ColMajor;
	const int j1x = (I==CStyle ? j1 : j1-1);
	return MatrixView<T,I>(ptr()+j1x*stepj(),colsize(),2,
	    stepi(),(int(j2)-int(j1))*stepj(),newstor,NonConj FIRSTLAST);
      }

      inline MatrixView<T,I> RowPair(size_t i1, size_t i2) 
      {
	if (I == CStyle) { TMVAssert(i1<colsize() && i2<colsize()); }
	else { TMVAssert(i1>0 && i1<=colsize() && i2>0 && i2<=colsize()); }
	StorageType newstor = S == RowMajor ?
	  RowMajor : i2==i1+1 ? ColMajor : NoMajor;
	const int i1x = (I==CStyle ? i1 : i1-1);
	return MatrixView<T,I>(ptr()+i1x*stepi(),2,rowsize(),
	    (int(i2)-int(i1))*stepi(),stepj(),newstor,NonConj FIRSTLAST);
      }

      inline MatrixView<T,I> Cols(size_t j1, size_t j2) 
      {
	if (I==FortranStyle) { TMVAssert(j1>0); }
	TMVAssert(j1<=j2);
	TMVAssert(j2<=rowsize());
	const int j1x = (I==CStyle ? j1 : j1-1);
	return MatrixView<T,I>(ptr()+j1x*stepj(),colsize(),j2-j1x,
	    stepi(),stepj(),S,NonConj,iscm()?1:0 FIRSTLAST);
      }

      inline MatrixView<T,I> Rows(size_t i1, size_t i2) 
      {
	if (I==FortranStyle) { TMVAssert(i1>0); }
	TMVAssert(i1<=i2);
	TMVAssert(i2<=colsize());
	const int i1x = (I==CStyle ? i1 : i1-1);
	return MatrixView<T,I>(ptr()+i1x*stepi(),i2-i1x,rowsize(),
	    stepi(),stepj(),S,NonConj,isrm()?1:0 FIRSTLAST);
      }

      inline MatrixView<RealType(T)> Real() 
      {
	return MatrixView<RealType(T)>(
	    reinterpret_cast<RealType(T)*>(ptr()),colsize(),rowsize(),
	    IsReal(T()) ? stepi() : 2*stepi(),
	    IsReal(T()) ? stepj() : 2*stepj(),
	    IsReal(T()) ? S : NoMajor,NonConj
#ifdef TMVFLDEBUG
	    ,reinterpret_cast<const RealType(T)*>(first)+1
	    ,reinterpret_cast<const RealType(T)*>(last)+1
#endif
	    );
      }

      inline MatrixView<RealType(T)> Imag() 
      {
	TMVAssert(IsComplex(T()));
	return MatrixView<RealType(T)>(
	    reinterpret_cast<RealType(T)*>(ptr())+1,
	    colsize(),rowsize(),2*stepi(),2*stepj(),NoMajor,NonConj
#ifdef TMVFLDEBUG
	    ,reinterpret_cast<const RealType(T)*>(first)+1
	    ,reinterpret_cast<const RealType(T)*>(last)+1
#endif
	    );
      }

      //
      // Views
      //

      inline ConstMatrixView<T,I> View() const
      { 
	return ConstMatrixView<T,I>(cptr(),colsize(),rowsize(),
	    stepi(),stepj(),S,NonConj,ls());
      }

      inline ConstMatrixView<T,I> Transpose() const
      { 
	return ConstMatrixView<T,I>(cptr(),rowsize(),colsize(),
	    stepj(),stepi(),TransOf(S),NonConj,ls());
      }
  
      inline ConstMatrixView<T,I> Conjugate() const
      { 
	return ConstMatrixView<T,I>(cptr(),colsize(),rowsize(),
	    stepi(),stepj(),S,ConjOf(T,NonConj),ls());
      }

      inline ConstMatrixView<T,I> Adjoint() const
      { 
	return ConstMatrixView<T,I>(cptr(),rowsize(),colsize(),
	    stepj(),stepi(),TransOf(S),ConjOf(T,NonConj),ls());
      }

      inline ConstVectorView<T,I> ConstLinearView() const
      {
	TMVAssert(ls() == colsize()*rowsize());
	return ConstVectorView<T,I>(cptr(),ls(),1,NonConj); 
      }

      inline MatrixView<T,I> View()
      { 
	return MatrixView<T,I>(ptr(),colsize(),rowsize(),
	    stepi(),stepj(),S,NonConj,ls() FIRSTLAST);
      }

      inline MatrixView<T,I> Transpose()
      { 
	return MatrixView<T,I>(ptr(),rowsize(),colsize(),
	    stepj(),stepi(),TransOf(S),NonConj,ls() FIRSTLAST);
      }
  
      inline MatrixView<T,I> Conjugate()
      { 
	return MatrixView<T,I>(ptr(),colsize(),rowsize(),
	    stepi(),stepj(),S,ConjOf(T,NonConj),ls() FIRSTLAST);
      }

      inline MatrixView<T,I> Adjoint()
      { 
	return MatrixView<T,I>(ptr(),rowsize(),colsize(),
	    stepj(),stepi(),TransOf(S),ConjOf(T,NonConj),ls() FIRSTLAST);
      }

      inline VectorView<T,I> LinearView()
      {
	TMVAssert(ls() == colsize()*rowsize());
	return VectorView<T,I>(ptr(),ls(),1,NonConj FIRSTLAST); 
      }

      inline size_t colsize() const { return itscs; }
      inline size_t rowsize() const { return itsrs; }
      using BaseMatrix<T>::IsSquare;
      inline const T* cptr() const { return itsm; }
      inline T* ptr() { return itsm; }
      inline int stepi() const 
      { return S == RowMajor ? itsrs : 1; }
      inline int stepj() const 
      { return S == RowMajor ? 1 : itscs; }
      inline StorageType stor() const { return S; }
      inline bool isrm() const { return S==RowMajor; }
      inline bool iscm() const { return S==ColMajor; }
      inline ConjItType ct() const { return NonConj; }
      inline bool isconj() const { return false; }

    protected :

      using GenMatrix<T>::ls;
      T*const itsm;
      const size_t itscs;
      const size_t itsrs;

#ifdef TMVFLDEBUG
    public:
      const T*const first;
      const T*const last;
    protected:
#endif

      inline T cref(size_t i, size_t j) const
      { 
	TMVAssert(i<colsize());
	TMVAssert(j<rowsize());
	return *(cptr() + int(i)*stepi() + int(j)*stepj());
      }

      inline T& ref(size_t i, size_t j)
      { 
	TMVAssert(i<colsize());
	TMVAssert(j<rowsize());
	T*const mi = ptr() + int(i)*stepi() + int(j)*stepj();
#ifdef TMVFLDEBUG
	TMVAssert(mi >= first);
	TMVAssert(mi < last);
#endif
	return *mi;
      }

  }; // Matrix

//---------------------------------------------------------------------------

  //
  // Special Creators: 
  //   RowVectorViewOf(v) = 1xn Matrix with v in only row - Same Storage
  //   ColVectorViewOf(v) = nx1 Matrix with v in only col - Same Storage
  //   MatrixViewOf(m,colsize,rowsize,storage) = MatrixView of m 
  //

  template <class T> inline ConstMatrixView<T> RowVectorViewOf(
      const GenVector<T>& v)
  {
    return ConstMatrixView<T>(v.cptr(),1,v.size(),v.size(),v.step(),
	v.step()==1?RowMajor:NoMajor,v.ct(),v.size());
  }

  template <class T, IndexStyle I> inline ConstMatrixView<T,I> RowVectorViewOf(
      const ConstVectorView<T,I>& v)
  {
    return ConstMatrixView<T,I>(v.cptr(),1,v.size(),v.size(),v.step(),
	v.step()==1?RowMajor:NoMajor,v.ct(),v.size());
  }

  template <class T, IndexStyle I> inline ConstMatrixView<T,I> RowVectorViewOf(
      const Vector<T,I>& v)
  {
    return ConstMatrixView<T,I>(v.cptr(),1,v.size(),v.size(),1,
	RowMajor,v.ct(),v.size());
  }

  template <class T, IndexStyle I> inline MatrixView<T,I> RowVectorViewOf(
      const VectorView<T,I>& v)
  {
    return MatrixView<T,I>(v.ptr(),1,v.size(),v.size(),v.step(),
	v.step()==1?RowMajor:NoMajor,v.ct(),v.size()
	FIRSTLAST1(v.ptr(),v.ptr()+v.size()) );
  }

  template <class T, IndexStyle I> inline MatrixView<T,I> RowVectorViewOf(
      Vector<T,I>& v)
  {
    return MatrixView<T,I>(v.ptr(),1,v.size(),v.size(),1,
	RowMajor,v.ct(),v.size()
	FIRSTLAST1(v.ptr(),v.ptr()+v.size()) );
  }

  template <class T> inline ConstMatrixView<T> ColVectorViewOf(
      const GenVector<T>& v)
  { 
    return ConstMatrixView<T>(v.cptr(),v.size(),1,v.step(),v.size(),
	v.step()==1?ColMajor:NoMajor, v.ct(),v.size());
  }

  template <class T, IndexStyle I> inline ConstMatrixView<T,I> ColVectorViewOf(
      const ConstVectorView<T,I>& v)
  { 
    return ConstMatrixView<T,I>(v.cptr(),v.size(),1,v.step(),v.size(),
	v.step()==1?ColMajor:NoMajor,v.ct(),v.size());
  }

  template <class T, IndexStyle I> inline ConstMatrixView<T,I> ColVectorViewOf(
      const Vector<T,I>& v)
  { 
    return ConstMatrixView<T,I>(v.cptr(),v.size(),1,1,v.size(),
	ColMajor,v.ct(),v.size());
  }

  template <class T, IndexStyle I> inline MatrixView<T,I> ColVectorViewOf(
      const VectorView<T,I>& v)
  { 
    return MatrixView<T,I>(v.ptr(),v.size(),1,v.step(),v.size(),
	v.step()==1?ColMajor:NoMajor,v.ct(),v.size()
	FIRSTLAST1(v.ptr(),v.ptr()+v.size()) );
  }

  template <class T, IndexStyle I> inline MatrixView<T,I> ColVectorViewOf(
      Vector<T,I>& v)
  { 
    return MatrixView<T,I>(v.ptr(),v.size(),1,1,v.size(),
	ColMajor,v.ct(),v.size() FIRSTLAST1(v.ptr(),v.ptr()+v.size()) );
  }

  template <class T> inline MatrixView<T> MatrixViewOf(
      T* m, size_t colsize, size_t rowsize, StorageType stor)
  {
    TMVAssert(stor == RowMajor || stor == ColMajor);
    size_t linsize = colsize * rowsize;
    if (stor == RowMajor) 
      return MatrixView<T>(m,colsize,rowsize,rowsize,1,RowMajor,NonConj,
	  linsize FIRSTLAST1(m,m+linsize));
    else 
      return MatrixView<T>(m,colsize,rowsize,1,colsize,ColMajor,NonConj,
	  linsize FIRSTLAST1(m,m+linsize));
  }

  template <class T> inline ConstMatrixView<T> MatrixViewOf(
      const T* m, size_t colsize, size_t rowsize, StorageType stor)
  {
    TMVAssert(stor == RowMajor || stor == ColMajor);
    size_t linsize = colsize*rowsize;
    if (stor == RowMajor)
      return ConstMatrixView<T>(m,colsize,rowsize,rowsize,1,RowMajor,NonConj,
	  linsize);
    else 
      return ConstMatrixView<T>(m,colsize,rowsize,1,colsize,ColMajor,NonConj,
	  linsize);
  }

  //
  // Copy Matrices
  //

  template <bool c1, class T> void DoCopySameType(
      const GenMatrix<T>& m1, const MatrixView<T>& m2);

  template <class T> inline void DoCopy(
      const GenMatrix<T>& m1, const MatrixView<T>& m2)
  {
    if (m1.isconj()) DoCopySameType<true>(m1,m2);
    else DoCopySameType<false>(m1,m2);
  }

  template <bool c1, class T, class T1> inline void DoCopyDiffType(
      const GenMatrix<T1>& m1, const MatrixView<T>& m2)
  {
    TMVAssert(m2.rowsize() == m1.rowsize());
    TMVAssert(m2.colsize() == m1.colsize());
    TMVAssert(m2.rowsize() > 0);
    TMVAssert(m2.colsize() > 0);
    TMVAssert(m2.ct()==NonConj);
    TMVAssert(!(m1.isrm() && m2.isrm()));
    TMVAssert(!m2.SameAs(m1));
    TMVAssert(c1 == m1.isconj());

    if (m1.iscm() && m2.iscm() && m1.colsize() > 1) 
      for(size_t j=0;j<m2.rowsize();++j)
	DoCopyDiffType<c1>(m1.col(j),m2.col(j));
    else if (m2.colsize() < m2.rowsize())
      for(size_t i=0;i<m2.colsize();++i) 
	DoCopyDiffType<c1>(m1.row(i),m2.row(i));
    else
      for(size_t j=0;j<m2.rowsize();++j) 
	DoCopyDiffType<c1>(m1.col(j),m2.col(j));
  }

  template <class T, class T1> inline void DoCopy(
      const GenMatrix<T1>& m1, const MatrixView<T>& m2)
  {
    if (m1.isconj()) DoCopyDiffType<true>(m1,m2);
    else DoCopyDiffType<false>(m1,m2);
  }

  template <class T> inline void DoCopy(
      const GenMatrix<complex<T> >& m1, const MatrixView<T>& m2)
  { TMVAssert(false); }

  template <class T1, class T2> inline void Copy(
      const GenMatrix<T1>& m1, const MatrixView<T2>& m2)
  {
    TMVAssert(m2.rowsize() == m1.rowsize());
    TMVAssert(m2.colsize() == m1.colsize());
    if (m2.rowsize() > 0 && m2.colsize() > 0 && !m2.SameAs(m1))  {
      if (m1.stor() == m2.stor() && m1.CanLinearize() && m2.CanLinearize()) {
	TMVAssert(m1.ConstLinearView().size() == m2.LinearView().size());
	TMVAssert( (m1.stepi()==m2.stepi() && m1.stepj()==m2.stepj()) );
	m2.LinearView() = m1.ConstLinearView();
      }
      else {
	if (m2.isrm() || (m1.isrm() && !m2.iscm()))
	  if (m2.isconj()) DoCopy(m1.Adjoint(),m2.Adjoint());
	  else DoCopy(m1.Transpose(),m2.Transpose());
	else
	  if (m2.isconj()) DoCopy(m1.Conjugate(),m2.Conjugate());
	  else DoCopy(m1,m2);
      }
    }
  }

  //
  // Swap Matrices
  //

  template <class T> void Swap(
      const MatrixView<T>& m1, const MatrixView<T>& m2);

  template <class T, StorageType S, IndexStyle I> inline void Swap(
      const MatrixView<T>& m1, Matrix<T,S,I>& m2)
  { Swap(m1,m2.View()); }

  template <class T, StorageType S, IndexStyle I> inline void Swap(
      Matrix<T,S,I>& m1, const MatrixView<T>& m2)
  { Swap(m1.View(),m2); }

  template <class T, StorageType S1, IndexStyle I1, StorageType S2, IndexStyle I2> 
    inline void Swap(Matrix<T,S1,I1>& m1, Matrix<T,S2,I2>& m2)
    { Swap(m1.View(),m2.View()); }

  //
  // Functions of Matrices:
  //

  template <class T> inline T Det(const GenMatrix<T>& m)
  { return m.Det(); }

  template <class T> inline T Trace(const GenMatrix<T>& m)
  { return m.Trace(); }

  template <class T> inline RealType(T) Norm(const GenMatrix<T>& m)
  { return m.Norm(); }

  template <class T> inline RealType(T) NormSq(const GenMatrix<T>& m)
  { return m.NormSq(); }

  template <class T> inline RealType(T) NormF(const GenMatrix<T>& m)
  { return m.NormF(); }

  template <class T> RealType(T) Norm1(const GenMatrix<T>& m)
  { return m.Norm1(); }

  template <class T> inline RealType(T) Norm2(const GenMatrix<T>& m)
  { return m.Norm2(); }

  template <class T> inline RealType(T) NormInf(const GenMatrix<T>& m)
  { return m.NormInf(); }

  template <class T> RealType(T) MaxAbsElement(const GenMatrix<T>& m)
  { return m.MaxAbsElement(); }

  template <class T> inline ConstMatrixView<T> Transpose(const GenMatrix<T>& m)
  { return m.Transpose(); }

  template <class T, IndexStyle I> inline ConstMatrixView<T,I> Transpose(
      const ConstMatrixView<T,I>& m)
  { return m.Transpose(); }

  template <class T, StorageType S, IndexStyle I> 
    inline ConstMatrixView<T> Transpose(const Matrix<T,S,I>& m)
    { return m.Transpose(); }

  template <class T, IndexStyle I> inline MatrixView<T,I> Transpose(
      const MatrixView<T,I>& m)
  { return m.Transpose(); }

  template <class T, StorageType S, IndexStyle I> 
    inline MatrixView<T,I> Transpose(Matrix<T,S,I>& m)
    { return m.Transpose(); }

  template <class T> inline ConstMatrixView<T> Conjugate(const GenMatrix<T>& m)
  { return m.Conjugate(); }

  template <class T, IndexStyle I> inline ConstMatrixView<T,I> Conjugate(
      const ConstMatrixView<T,I>& m)
  { return m.Conjugate(); }

  template <class T, StorageType S, IndexStyle I> 
    inline ConstMatrixView<T,I> Conjugate(const Matrix<T,S,I>& m)
    { return m.Conjugate(); }

  template <class T, IndexStyle I> inline MatrixView<T,I> Conjugate(
      const MatrixView<T,I>& m)
  { return m.Conjugate(); }

  template <class T, StorageType S, IndexStyle I> 
    inline MatrixView<T,I> Conjugate(Matrix<T,S,I>& m)
    { return m.Conjugate(); }

  template <class T> inline ConstMatrixView<T> Adjoint(const GenMatrix<T>& m)
  { return m.Adjoint(); }

  template <class T, IndexStyle I> inline ConstMatrixView<T,I> Adjoint(
      const ConstMatrixView<T,I>& m)
  { return m.Adjoint(); }

  template <class T, StorageType S, IndexStyle I> 
    inline ConstMatrixView<T,I> Adjoint(const Matrix<T,S,I>& m)
    { return m.Adjoint(); }

  template <class T, IndexStyle I> inline MatrixView<T,I> Adjoint(
      const MatrixView<T,I>& m)
  { return m.Adjoint(); }

  template <class T, StorageType S, IndexStyle I> 
    inline MatrixView<T,I> Adjoint(Matrix<T,S,I>& m)
    { return m.Adjoint(); }

  template <class T> inline QuotXM<T,T> Inverse(const GenMatrix<T>& m)
  { return m.Inverse(); }

  //
  // Matrix ==, != Matrix
  //

  template <class T1, class T2> bool operator==(const GenMatrix<T1>& m1,
      const GenMatrix<T2>& m2);
  template <class T1, class T2> inline bool operator!=(const GenMatrix<T1>& m1,
      const GenMatrix<T2>& m2)
  { return !(m1 == m2); }


  //
  // I/O
  //
 
  template <class T, StorageType S, IndexStyle I> istream& operator>>(
      istream& is, Matrix<T,S,I>*& m);

  template <class T> istream& operator>>(istream& is, const MatrixView<T>& m);

  template <class T, StorageType S, IndexStyle I> inline istream& operator>>(
      istream& is, Matrix<T,S,I>& m)
  { return is>>m.View(); }

  inline string Text(StorageType s)
  { 
    return s == RowMajor ? "RowMajor" :
      s == ColMajor ? "ColMajor" :
      s == DiagMajor ? "DiagMajor" :
      s == NoMajor ? "NoMajor" : "Unknown";
  }

  template <class T, StorageType S, IndexStyle I> inline string Type(
      const Matrix<T,S,I>& m)
  { 
    return string("Matrix<")+Type(T())+","+Text(S)+","+Text(I)+">"; 
  }
  template <class T> inline string Type(const GenMatrix<T>& m)
  {
    return string("GenMatrix<")+Type(T())+","+Text(m.stor())+","+Text(m.ct())+">";
  }
  template <class T, IndexStyle I> inline string Type(const ConstMatrixView<T,I>& m)
  { 
    return string("ConstMatrixView<")+Type(T())+","+Text(m.stor())+","
      +Text(I)+","+Text(m.ct())+">"; 
  }
  template <class T, IndexStyle I> inline string Type(const MatrixView<T,I>& m)
  { 
    return string("MatrixView<")+Type(T())+","+Text(m.stor())+","
      +Text(I)+","+Text(m.ct())+">";
  }

}; // namespace tmv

#endif