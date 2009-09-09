// vim:et:ts=2:sw=2:ci:cino=f0,g0,t0,+0:

// If XTEST, do the regular tests for a large vector
// But turn off XTEST, so we don't do all the extra arithmetic
// combinations for it.
#ifdef XTEST
#define DoLargeTest
#undef XTEST
#endif

#include "TMV_Test.h"
#include "TMV_Test3.h"
#include "TMV_Vec.h"
#include <fstream>
#include <fstream>
#include <cstdio>

#include "TMV_TestVectorArith.h"

template <int N, class T> static void DoTestSmallVectorArith_2d()
{
  tmv::SmallVector<T,N> a;
  for(int i=0;i<N;++i) a(i) = T(i+10);
  tmv::SmallVector<T,N> b;
  for(int i=0;i<N;++i) b(i) = T(-3*i+2);

  tmv::SmallVector<std::complex<T>,N> ca = a*std::complex<T>(2,-1);;
  tmv::SmallVector<std::complex<T>,N> cb = b*std::complex<T>(-5,1);

  TestVectorArith2<T>(a,ca,b,cb,"SmallVector CC");
}

template <class T> void TestSmallVectorArith_2d()
{
#ifdef DoLargeTest
  DoTestSmallVectorArith_2d<280,T>(); // this requires heap alloc
#endif
}

#ifdef TEST_DOUBLE
template void TestSmallVectorArith_2d<double>();
#endif
#ifdef TEST_FLOAT
template void TestSmallVectorArith_2d<float>();
#endif
#ifdef TEST_LONGDOUBLE
template void TestSmallVectorArith_2d<long double>();
#endif
#ifdef TEST_INT
template void TestSmallVectorArith_2d<int>();
#endif
