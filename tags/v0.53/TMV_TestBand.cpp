
#include "TMV_Test.h"
#include "TMV.h"
#include "TMV_Band.h"
#include "TMV_TestMatrixArith.h"

using tmv::Matrix;
using tmv::BandMatrix;
using tmv::RowMajor;
using tmv::FortranStyle;

template <class T> extern void TestBandMatrixArith_A();
template <class T> extern void TestBandMatrixArith_B();

template <class T, tmv::StorageType S> void TestBasicBandMatrix()
{
  const int N = 10;
  const int nhi = 1;
  const int nlo = 3;

  BandMatrix<T> a1(N,N,nlo,nhi);
  BandMatrix<T,RowMajor,FortranStyle> a1f(N,N,nlo,nhi);

  Assert(a1.colsize() == size_t(N) && a1.rowsize() == size_t(N),
      "Creating BandMatrix(N)");
  Assert(a1.nlo() == nlo && a1.nhi() == nhi,"Creating BandMatrix(nlo,nhi)");
  Assert(a1f.colsize() == size_t(N) && a1f.rowsize() == size_t(N),
      "Creating BandMatrixF(N)");
  Assert(a1f.nlo() == nlo && a1f.nhi() == nhi,"Creating BandMatrixF(nlo,nhi)");

  for (int i=0, k=0; i<N; ++i) for (int j=0; j<N; ++j, ++k)
    if ( j <= i + nhi && i <= j + nlo) {
      a1(i,j) = k;
      a1f(i+1,j+1) = k;
    }

  tmv::ConstBandMatrixView<T> a1cv = a1.View();
  tmv::BandMatrixView<T> a1v = a1.View();
  tmv::ConstBandMatrixView<T,FortranStyle> a1fcv = a1f.View();
  tmv::BandMatrixView<T,FortranStyle> a1fv = a1f.View();

  for (int i=0, k=0; i<N; ++i) {
    int j1 = 0;
    if (i > nlo) j1 = i-nlo;
    int j2 = i+nhi+1;
    if (j2 >= N) j2 = N;
    for (int j=0; j<N; ++j, ++k) {
      int i1 = 0;
      if (j > nhi) i1 = j-nhi;
      int i2 = j+nlo+1;
      if (i2 >= N) i2 = N;
      if ( j <= i + nhi && i <= j + nlo) {
	Assert(a1(i,j) == k,"Read/Write BandMatrix");
	Assert(a1cv(i,j) == k,"Access BandMatrix CV");
	Assert(a1v(i,j) == k,"Access BandMatrix V");
	Assert(a1f(i+1,j+1) == k,"Read/Write BandMatrixF");
	Assert(a1fcv(i+1,j+1) == k,"Access BandMatrixF CV");
	Assert(a1fv(i+1,j+1) == k,"Access BandMatrixF V");
	Assert(a1.row(i,j1,j2)(j-j1) == k,"BandMatrix.row");
	Assert(a1cv.row(i,j1,j2)(j-j1) == k,"BandMatrix.row CV");
	Assert(a1v.row(i,j1,j2)(j-j1) == k,"BandMatrix.row V");
	Assert(a1f.row(i+1,j1+1,j2)(j+1-j1) == k,"BandMatrixF.row");
	Assert(a1fcv.row(i+1,j1+1,j2)(j+1-j1) == k,"BandMatrixF.row CV");
	Assert(a1fv.row(i+1,j1+1,j2)(j+1-j1) == k,"BandMatrixF.row V");
	Assert(a1.col(j,i1,i2)(i-i1) == k,"BandMatrix.col");
	Assert(a1cv.col(j,i1,i2)(i-i1) == k,"BandMatrix.col CV");
	Assert(a1v.col(j,i1,i2)(i-i1) == k,"BandMatrix.col V");
	Assert(a1f.col(j+1,i1+1,i2)(i+1-i1) == k,"BandMatrixF.col");
	Assert(a1fcv.col(j+1,i1+1,i2)(i+1-i1) == k,"BandMatrixF.col CV");
	Assert(a1fv.col(j+1,i1+1,i2)(i+1-i1) == k,"BandMatrixF.col V");
	int d = j-i;
	if (d>0) {
	  Assert(a1.diag(d)(i) == k,"BandMatrix.diag1");
	  Assert(a1cv.diag(d)(i) == k,"BandMatrix.diag1 CV");
	  Assert(a1v.diag(d)(i) == k,"BandMatrix.diag1 V");
	  Assert(a1.diag(d,i,N-d)(0) == k,"BandMatrix.diag2");
	  Assert(a1cv.diag(d,i,N-d)(0) == k,"BandMatrix.diag2 CV");
	  Assert(a1v.diag(d,i,N-d)(0) == k,"BandMatrix.diag2 V");
	  Assert(a1f.diag(d)(i+1) == k,"BandMatrixF.diag1");
	  Assert(a1fcv.diag(d)(i+1) == k,"BandMatrixF.diag1 CV");
	  Assert(a1fv.diag(d)(i+1) == k,"BandMatrixF.diag1 V");
	  Assert(a1f.diag(d,i+1,N-d)(1) == k,"BandMatrixF.diag2");
	  Assert(a1fcv.diag(d,i+1,N-d)(1) == k,"BandMatrixF.diag2 CV");
	  Assert(a1fv.diag(d,i+1,N-d)(1) == k,"BandMatrixF.diag2 V");
	} else {
	  if (d==0) {
	    Assert(a1.diag()(j) == k,"BandMatrix.diag");
	    Assert(a1cv.diag()(j) == k,"BandMatrix.diag CV");
	    Assert(a1v.diag()(j) == k,"BandMatrix.diag V");
	    Assert(a1f.diag()(j+1) == k,"BandMatrixF.diag");
	    Assert(a1fcv.diag()(j+1) == k,"BandMatrixF.diag CV");
	    Assert(a1fv.diag()(j+1) == k,"BandMatrixF.diag V");
	  }
	  Assert(a1.diag(d)(j) == k,"BandMatrix.diag1");
	  Assert(a1cv.diag(d)(j) == k,"BandMatrix.diag1 CV");
	  Assert(a1v.diag(d)(j) == k,"BandMatrix.diag1 V");
	  Assert(a1.diag(d,j,N+d)(0) == k,"BandMatrix.diag2");
	  Assert(a1cv.diag(d,j,N+d)(0) == k,"BandMatrix.diag2 CV");
	  Assert(a1v.diag(d,j,N+d)(0) == k,"BandMatrix.diag2 V");
	  Assert(a1f.diag(d)(j+1) == k,"BandMatrixF.diag1");
	  Assert(a1fcv.diag(d)(j+1) == k,"BandMatrixF.diag1 CV");
	  Assert(a1fv.diag(d)(j+1) == k,"BandMatrixF.diag1 V");
	  Assert(a1f.diag(d,j+1,N+d)(1) == k,"BandMatrixF.diag2");
	  Assert(a1fcv.diag(d,j+1,N+d)(1) == k,"BandMatrixF.diag2 CV");
	  Assert(a1fv.diag(d,j+1,N+d)(1) == k,"BandMatrixF.diag2 V");
	}
      }
    }
  }
  Assert(a1 == a1f,"CStyle BandMatrix == FortranStyle BandMatrix");
  Assert(a1 == a1cv,"BandMatrix == ConstBandMatrixView");
  Assert(a1 == a1v,"BandMatrix == BandMatrixView");
  Assert(a1 == a1fcv,"BandMatrix == FortranStyle ConstBandMatrixView");
  Assert(a1 == a1fv,"BandMatrix == FortranStyle BandMatrixView");

  BandMatrix<T> a2(N,N,nlo,nhi);
  for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) 
    if ( j <= i + nhi && i <= j + nlo) {
      a1(i,j) = 3.+i-5*j;
      a2(i,j) = 5.-2*i+4*j;
    }
  a1f = a1;
  Assert(a1f == a1,"Copy CStyle BandMatrix to FortranStyle");

  BandMatrix<T> c(N,N,nlo,nhi);
  c = a1+a2;
  for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) 
    if ( j <= i + nhi && i <= j + nlo) 
      Assert(c(i,j) == 8.-i-j,"Add BandMatrices");

  c = a1-a2;
  for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) 
    if ( j <= i + nhi && i <= j + nlo) 
      Assert(c(i,j) == -2.+3.*i-9.*j,"Subtract BandMatrices");

  Matrix<T> m1 = a1;
  for (int i=0, k=0; i<N; ++i) for (int j=0; j<N; ++j, ++k)
    if ( j <= i + nhi && i <= j + nlo) 
      Assert(a1(i,j) == m1(i,j),"BandMatrix -> Matrix");
  Assert(a1 == BandMatrix<T>(m1,nlo,nhi),"Matrix -> BandMatrix");
}

template <class T> void TestBandMatrix()
{
  TestBasicBandMatrix<T,tmv::RowMajor>();
  TestBasicBandMatrix<T,tmv::ColMajor>();
  TestBasicBandMatrix<T,tmv::DiagMajor>();

  cout<<"BandMatrix<"<<tmv::Type(T())<<"> passed all basic tests\n";

  TestBandMatrixArith_A<T>();
  cout<<"BandMatrix<"<<tmv::Type(T())<<"> (Band/Band) Arithmetic passed all tests\n";
  TestBandMatrixArith_B<T>();
  cout<<"BandMatrix<"<<tmv::Type(T())<<"> (Matrix/Band) Arithmetic passed all tests\n";
}

template void TestBandMatrix<double>();
#ifndef NOFLOAT
template void TestBandMatrix<float>();
#endif
#ifdef LONGDOUBLE
template void TestBandMatrix<long double>();
#endif
