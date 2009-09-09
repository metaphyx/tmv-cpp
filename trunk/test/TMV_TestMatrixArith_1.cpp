// vim:et:ts=2:sw=2:ci:cino=f0,g0,t0,+0:

#include "TMV_Test.h"
#include "TMV_Test1.h"
#include "TMV_Mat.h"
#include <fstream>

// MJ: Remove this once it's ok to test Det()
#define NODIV

#include "TMV_TestMatrixArith.h"
#define CT std::complex<T>

template <class T> void TestMatrixArith_1()
{
  tmv::Matrix<T,tmv::RowMajor> a1x(4,4);
  for(int i=0;i<4;++i) for(int j=0;j<4;++j) {
    a1x(i,j) = T(2+4*i-5*j);
  }
  a1x(0,0) = 14; 
  a1x(1,0) = -2; 
  a1x(2,0) = 7; 
  a1x(3,0) = -10;
  a1x(2,2) = 30;

  tmv::Matrix<CT,tmv::RowMajor> ca1x = a1x;
  ca1x(2,3) += CT(2,3);
  ca1x(1,0) *= CT(0,2);
  ca1x.col(1) *= CT(-1,3);
  ca1x.row(3) += tmv::Vector<CT>(4,CT(1,9));

  tmv::Matrix<T,tmv::ColMajor> a2x = a1x.Transpose();
  a2x.row(1) *= T(3);
  a2x.col(2) -= tmv::Vector<T>(4,4.);
  tmv::Matrix<CT,tmv::ColMajor> ca2x = ca1x;
  ca2x -= a2x;
  ca2x *= CT(1,-2);

  tmv::Matrix<T> a3x(12,16);
  for(int i=0;i<12;++i) for(int j=0;j<16;++j) a3x(i,j) = T(1-2*i+3*j);
  a3x.diag().AddToAll(30);
  tmv::Matrix<CT> ca3x = a3x*CT(1,-2);
  ca3x.diag().AddToAll(CT(-22,15));

  tmv::MatrixView<T> a1 = a1x.View();
  tmv::MatrixView<CT> ca1 = ca1x.View();
  tmv::MatrixView<T> a2 = a2x.View();
  tmv::MatrixView<CT> ca2 = ca2x.View();
  tmv::MatrixView<T> a3 = a3x.SubMatrix(0,12,0,16,3,4);
  tmv::MatrixView<CT> ca3 = ca3x.SubMatrix(0,12,0,16,3,4);

  TestMatrixArith1<T>(a1,ca1,"Square 1");
  TestMatrixArith1<T>(a2,ca2,"Square 2");
  TestMatrixArith1<T>(a3,ca3,"Square 3");

#if 1
  tmv::Matrix<T,tmv::RowMajor> a4x(7,4);
  for(int i=0;i<7;++i) for(int j=0;j<4;++j) a4x(i,j) = T(1-3*i+2*j);
  tmv::Matrix<T,tmv::ColMajor> a5x = a4x.Transpose();
  a4x.SubMatrix(2,6,0,4) += a1x;
  a5x.SubMatrix(0,4,1,5) -= a2x;

  tmv::Matrix<CT,tmv::RowMajor> ca4x = a4x*CT(1,2);
  tmv::Matrix<CT,tmv::ColMajor> ca5x = ca4x.Adjoint();
  ca4x.SubMatrix(2,6,0,4) += ca1x;
  ca5x.SubMatrix(0,4,1,5) -= ca2x;
  ca4x.col(1) *= CT(2,1);
  ca4x.row(6).AddToAll(CT(-7,2));
  ca5x.col(3) *= CT(-1,3);
  ca5x.row(0).AddToAll(CT(1,9));

  tmv::MatrixView<T> a4 = a4x.View();
  tmv::MatrixView<CT> ca4 = ca4x.View();
  tmv::MatrixView<T> a5 = a5x.View();
  tmv::MatrixView<CT> ca5 = ca5x.View();
  TestMatrixArith1<T>(a4,ca4,"NonSquare 1");
  TestMatrixArith1<T>(a5,ca5,"NonSquare 2");

#ifdef XTEST
  tmv::Matrix<T> a6x(4,0,1);
  tmv::Matrix<T> a7x(0,4,1);
  tmv::Matrix<CT> ca6x = a6x;
  tmv::Matrix<CT> ca7x = a7x;

  tmv::MatrixView<T> a6 = a6x.View();
  tmv::MatrixView<CT> ca6 = ca6x.View();
  tmv::MatrixView<T> a7 = a7x.View();
  tmv::MatrixView<CT> ca7 = ca7x.View();
  TestMatrixArith1<T>(a6,ca6,"Degenerate 1");
  TestMatrixArith1<T>(a7,ca7,"Degenerate 2");
#endif
#endif
}

#ifdef TEST_DOUBLE
template void TestMatrixArith_1<double>();
#endif
#ifdef TEST_FLOAT
template void TestMatrixArith_1<float>();
#endif
#ifdef TEST_LONGDOUBLE
template void TestMatrixArith_1<long double>();
#endif
#ifdef TEST_INT
template void TestMatrixArith_1<int>();
#endif
