
#include "tmv/TMV_Base.h"

#define NOVIEWS
#define NOSV
#define USETEMP

#ifdef NOMIX_SMALL
#define NOMIX
#endif

template <class T> void TestAllSmallVector();
template <class T> void TestAllSmallMatrix();
template <class T> void TestAllSmallMatrixA();
template <class T> void TestAllSmallMatrixB();
template <class T> void TestSmallMatrix_Sub();
template <class T> void TestSmallMatrixArith_A1();
template <class T> void TestSmallMatrixArith_A2a();
template <class T> void TestSmallMatrixArith_A2b();
template <class T> void TestSmallMatrixArith_A3a();
template <class T> void TestSmallMatrixArith_A3b();
template <class T> void TestSmallMatrixArith_A4a();
template <class T> void TestSmallMatrixArith_A4b();
template <class T> void TestSmallMatrixArith_A5a();
template <class T> void TestSmallMatrixArith_A5b();
template <class T> void TestSmallMatrixArith_A6a();
template <class T> void TestSmallMatrixArith_A6b();
template <class T> void TestSmallMatrixArith_A7();
template <class T> void TestSmallMatrixArith_B1();
template <class T> void TestSmallMatrixArith_B2a();
template <class T> void TestSmallMatrixArith_B2b();
template <class T> void TestSmallMatrixArith_B3a();
template <class T> void TestSmallMatrixArith_B3b();
template <class T> void TestSmallMatrixArith_B4a();
template <class T> void TestSmallMatrixArith_B4b();
template <class T> void TestSmallMatrixArith_B4c();
template <class T> void TestSmallMatrixArith_B4d();
template <class T> void TestSmallMatrixArith_B5a();
template <class T> void TestSmallMatrixArith_B5b();
template <class T> void TestSmallMatrixArith_B5c();
template <class T> void TestSmallMatrixArith_B5d();
template <class T> void TestSmallMatrixArith_B6a();
template <class T> void TestSmallMatrixArith_B6b();
template <class T> void TestSmallMatrixArith_B6c();
template <class T> void TestSmallMatrixArith_B6d();
template <class T> void TestSmallMatrixArith_B7();
template <class T> void TestAllSmallMatrixDiv();
template <class T> void TestAllSmallMatrixDivA();
template <class T> void TestAllSmallMatrixDivB();
template <class T> void TestSmallMatrixDiv_A1a();
template <class T> void TestSmallMatrixDiv_A1b();
template <class T> void TestSmallMatrixDiv_A1c();
template <class T> void TestSmallMatrixDiv_A2a();
template <class T> void TestSmallMatrixDiv_A2b();
template <class T> void TestSmallMatrixDiv_A3a();
template <class T> void TestSmallMatrixDiv_A3b();
template <class T> void TestSmallMatrixDiv_A4();
template <class T> void TestSmallMatrixDiv_A5();
template <class T> void TestSmallMatrixDiv_B1a();
template <class T> void TestSmallMatrixDiv_B1b();
template <class T> void TestSmallMatrixDiv_B1c();
template <class T> void TestSmallMatrixDiv_B2a();
template <class T> void TestSmallMatrixDiv_B2b();
template <class T> void TestSmallMatrixDiv_B3a();
template <class T> void TestSmallMatrixDiv_B3b();
template <class T> void TestSmallMatrixDiv_B4a();
template <class T> void TestSmallMatrixDiv_B4b();
template <class T> void TestSmallMatrixDiv_B5a();
template <class T> void TestSmallMatrixDiv_B5b();
