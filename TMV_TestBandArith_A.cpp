#define START1 0
#define START2 0

#include "TMV_Test.h"
#include "TMV.h"
#include "TMV_Band.h"
#include "TMV_TestMatrixArith.h"

using tmv::Matrix;
using tmv::BandMatrix;
using tmv::BandMatrixView;
using tmv::ConstBandMatrixView;
using tmv::ConjItType;
using tmv::RowMajor;
using tmv::ColMajor;
using tmv::DiagMajor;
using tmv::IndexStyle;

template <class T1, IndexStyle I1, class T2, IndexStyle I2> bool CanAddEq(
    const BandMatrixView<T1,I1>& a, const BandMatrixView<T2,I2>& b)
{ 
  return a.colsize() == b.colsize() && a.rowsize() == b.rowsize() &&
    a.nhi() >= b.nhi() && a.nlo() >= b.nlo();
}

template <class T1, IndexStyle I1, class T2, IndexStyle I2> bool CanMultEq(
    const BandMatrixView<T1,I1>& a, const BandMatrixView<T2,I2>& b)
{ 
  return a.rowsize() == b.colsize() && b.IsSquare() && 
    (b.nhi()==int(0) || a.nhi() == int(a.rowsize())-1) && 
    (b.nlo()==int(0) || a.nlo() == int(a.colsize())-1);
}

template <class T1, IndexStyle I1, class T2, IndexStyle I2> bool CanMultEq2(
    const BandMatrixView<T1,I1>& a, const BandMatrixView<T2,I2>& b)
{ 
  return a.rowsize() == b.colsize() && a.IsSquare() && 
    (a.nhi()==int(0) || b.nhi() == int(b.rowsize())-1) && 
    (a.nlo()==int(0) || b.nlo() == int(b.colsize())-1);
}

template <class T> void TestBandMatrixArith_A()
{
  const int N = 8;

  vector<BandMatrixView<T> > b;

  Matrix<T> a1(N,N);
  for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) a1(i,j) = 3.+i-5*j;
  Matrix<T> a2(2*N,2*N);
  for (int i=0; i<2*N; ++i) for (int j=0; j<2*N; ++j) a2(i,j) = 1.-3*i+6*j;
  Vector<T> v1(N);
  Vector<T> v2(N-1);
  for (int i=0; i<N; ++i) v1(i) = 16.-3*i; 
  for (int i=0; i<N-1; ++i) v2(i) = -6.+i; 

  BandMatrix<T,RowMajor> B1(a1,3,1);
  b.push_back(B1.View());
  BandMatrix<T,DiagMajor> B3(a1,3,1);
  b.push_back(B3.View());
  BandMatrix<T,DiagMajor> B5(B1,1,1);
  b.push_back(B5.View());
  BandMatrix<T> B6(B1,3,0);
  b.push_back(B6.View());
#ifdef XTEST
  BandMatrix<T> B4(a2,6,6);
  b.push_back(B4.SubBandMatrix(0,2*N,0,N,3,3));
  BandMatrix<T> B4a(B4);
  b.push_back(B4a.SubBandMatrix(0,N+2,0,N,4,4));
  BandMatrix<T> B4b(B4);
  b.push_back(B4b.SubBandMatrix(0,2*N,0,2*N,3,3,2,2));
  BandMatrix<T,ColMajor> B2(a1,3,1);
  BandMatrix<T,DiagMajor> B3b(a1,1,3);
  BandMatrix<T> B7(a1,0,3);
  BandMatrix<T,DiagMajor> B8 = tmv::UpperBiDiagMatrix(v1,v2);
  BandMatrix<T,DiagMajor> B9 = tmv::LowerBiDiagMatrix(v2,v1);
  BandMatrix<T,DiagMajor> B10 = tmv::TriDiagMatrix(v2,v1,v2);
  BandMatrix<T,DiagMajor> B11 = tmv::UpperBiDiagMatrix(v1,v1);
  BandMatrix<T,DiagMajor> B12 = tmv::LowerBiDiagMatrix(v1,v1);
  BandMatrix<T,DiagMajor> B13 = tmv::TriDiagMatrix(v1,v1,v2);
  BandMatrix<T,DiagMajor> B14 = tmv::TriDiagMatrix(v2,v1,v1);
  BandMatrix<T> B4c(B4);
  BandMatrix<T> B4d(B4);
  BandMatrix<T,DiagMajor> B15(a1,1,N-1);
  BandMatrix<T,DiagMajor> B16(a1,3,N-2);
  BandMatrix<T> B17(a1,0,0);
  if (doallarith) {
    b.push_back(B2.View());
    b.push_back(B3b.View());
    b.push_back(B7.View());
    b.push_back(B8.View());
    b.push_back(B9.View());
    b.push_back(B10.View());
    b.push_back(B11.View());
    b.push_back(B12.View());
    b.push_back(B13.View());
    b.push_back(B14.View());
    b.push_back(B4c.SubBandMatrix(0,N,0,2*N,3,3));
    b.push_back(B4d.SubBandMatrix(0,N,0,N+2,4,4));
    b.push_back(BandMatrixViewOf(a1,3,N-1));
    b.push_back(B15.View());
    b.push_back(B16.View());
    b.push_back(B17.View());
  }
#endif

  for(size_t i=START1;i<b.size();i++) 
    for(size_t j=START2;j<b.size();j++) if (i!=j) {
      if (showstartdone) {
	cerr<<"Start loop "<<i<<','<<j<<endl;
	cerr<<"bi = "<<b[i]<<endl;
	cerr<<"bj = "<<b[j]<<endl;
      }
      BandMatrixView<T> bi = b[i];
      BandMatrixView<T> bj = b[j];
      BandMatrix<complex<T> > cbi = bi * complex<T>(2.,1.);
      BandMatrix<complex<T> > cbj = bj * complex<T>(-4.,3.);
      TestMatrixArith<T,BandMatrix<T>,BandMatrix<complex<T> > >(
	  bi,bj,cbi.View(),cbj.View(),"Band/Band");
    }
}

template void TestBandMatrixArith_A<double>();
#ifndef NOFLOAT
template void TestBandMatrixArith_A<float>();
#endif
#ifdef LONGDOUBLE
template void TestBandMatrixArith_A<long double>();
#endif
