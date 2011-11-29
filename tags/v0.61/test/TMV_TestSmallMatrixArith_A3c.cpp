#ifdef NDEBUG
#undef NDEBUG
#endif
#include "TMV_Test.h"
#include "TMV_Test3.h"
#include "TMV.h"
#include "TMV_Small.h"

#include "TMV_TestMatrixArith.h"

template <class T> void TestSmallMatrixArith_A3c()
{
  tmv::SmallMatrix<T,4,4,tmv::RowMajor> a1;
  for(int i=0;i<4;++i) for(int j=0;j<4;++j) {
    a1(i,j) = T(2+4*i-5*j);
  }
  a1(0,0) = 14.;
  a1(1,0) = -2.;
  a1(2,0) = 7.;
  a1(3,0) = -10.;
  a1(2,2) = 30.;

  tmv::SmallMatrix<std::complex<T>,4,4> ca1 = a1;
  ca1(2,3) += std::complex<T>(2,3);
  ca1(1,0) *= std::complex<T>(0,2);
  ca1.col(1) *= std::complex<T>(-1,3);
  ca1.row(3) += tmv::SmallVector<std::complex<T>,4>(std::complex<T>(1,9));

  tmv::SmallMatrix<T,12,16> a3x;
  for(int i=0;i<12;++i) for(int j=0;j<16;++j) a3x(i,j) = T(1-2*i+3*j);
  a3x.diag().AddToAll(30);
  tmv::SmallMatrix<std::complex<T>,12,16> ca3x = a3x*std::complex<T>(1,-2);
  ca3x.diag().AddToAll(std::complex<T>(-22,15));
  tmv::SmallMatrixView<T,4,4,48,4> a3v = a3x.SubMatrix(0,12,0,16,3,4);
  tmv::SmallMatrixView<std::complex<T>,4,4,48,4> ca3v = 
    ca3x.SubMatrix(0,12,0,16,3,4);

  tmv::SmallMatrix<T,4,4> a1x;
  tmv::SmallMatrix<std::complex<T>,4,4> ca1x;

  tmv::SmallVector<T,4> v1 = a1.row(2);
  tmv::SmallVectorView<T,4,1> v1v = v1.View();
  tmv::SmallVector<std::complex<T>,4> cv1 = ca1.row(2);
  tmv::SmallVectorView<std::complex<T>,4,1> cv1v = cv1.View();
  tmv::SmallVector<T,4> v2 = a1.col(2);
  tmv::SmallVectorView<T,4,1> v2v = v2.View();
  tmv::SmallVector<std::complex<T>,4> cv2 = ca1.col(2);
  tmv::SmallVectorView<std::complex<T>,4,1> cv2v = cv2.View();
#ifdef XTEST
  tmv::SmallVector<T,20> v15;
  tmv::SmallVector<std::complex<T>,20> cv15;
  tmv::SmallVector<T,20> v25;
  tmv::SmallVector<std::complex<T>,20> cv25;
  tmv::SmallVectorView<T,4,5> v1s = v15.SubVector(0,20,5);
  tmv::SmallVectorView<std::complex<T>,4,5> cv1s = cv15.SubVector(0,20,5);
  tmv::SmallVectorView<T,4,5> v2s = v25.SubVector(0,20,5);
  tmv::SmallVectorView<std::complex<T>,4,5> cv2s = cv25.SubVector(0,20,5);
  v1s = v1v;
  cv1s = cv1v;
  v2s = v2v;
  cv2s = cv2v;
#endif

  if (showstartdone) {
    std::cout<<"A3c\n";
  }
  TestMatrixArith3<T>(a3v,ca3v,v1v,cv1v,v2v,cv2v,"Square");
#ifdef XTEST
  TestMatrixArith3<T>(a3v,ca3v,v1s,cv1s,v2v,cv2v,"Square");
  TestMatrixArith3<T>(a3v,ca3v,v1v,cv1v,v2s,cv2s,"Square");
  TestMatrixArith3<T>(a3v,ca3v,v1s,cv1s,v2s,cv2s,"Square");
#endif
}

#ifdef INST_DOUBLE
template void TestSmallMatrixArith_A3c<double>();
#endif
#ifdef INST_FLOAT
template void TestSmallMatrixArith_A3c<float>();
#endif
#ifdef INST_LONGDOUBLE
template void TestSmallMatrixArith_A3c<long double>();
#endif
#ifdef INST_INT
template void TestSmallMatrixArith_A3c<int>();
#endif