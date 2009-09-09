// vim:et:ts=2:sw=2:ci:cino=f0,g0,t0,+0:

#include "TMV_TestSmallMatrixArith_7.h"

template <class T> void TestSmallMatrixArith_7b()
{
  TestSmallMatrixArith_7<T,3,3>("3 3");
}

#ifdef TEST_DOUBLE
template void TestSmallMatrixArith_7b<double>();
#endif
#ifdef TEST_FLOAT
template void TestSmallMatrixArith_7b<float>();
#endif
#ifdef TEST_LONGDOUBLE
template void TestSmallMatrixArith_7b<long double>();
#endif
#ifdef TEST_INT
template void TestSmallMatrixArith_7b<int>();
#endif
