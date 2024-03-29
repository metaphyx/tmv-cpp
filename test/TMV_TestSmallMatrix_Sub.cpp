
#include "TMV_Test.h"
#include "TMV_Test_3.h"
#include "TMV.h"
#include <fstream>

template <class T, size_t M, size_t N, tmv::StorageType S> 
static void DoTestSmallMatrix_Sub()
{
    tmv::SmallMatrix<T,M,N,S> m;
    tmv::SmallMatrix<T,M,N,S|tmv::FortranStyle> mf;
    Assert(m.colsize() == size_t(M) && m.rowsize() == size_t(N),
           "Creating SmallMatrix(M,N)");
    Assert(m.colsize() == size_t(M) && m.rowsize() == size_t(N),
           "Creating SmallMatrixF(M,N)");

    for (size_t i=0, k=0; i<M; ++i) for (size_t j=0; j<N; ++j, ++k) {
        m(i,j) = T(k);
        mf(i+1,j+1) = T(k);
    }

#define Si (S==tmv::RowMajor ? N : 1)
#define Sj (S==tmv::RowMajor ? 1 : M)

    Assert(m.subMatrix(2,5,1,4) == m.subMatrix(2,5,1,4,1,1),"subMatrix");
    Assert(m.subVector(2,5,4,2,3) == m.subMatrix(2,14,5,11,4,2).diag(),
           "subVector");

    Assert(mf.subMatrix(3,5,2,4) == mf.subMatrix(3,5,2,4,1,1),"subMatrixFF");
    Assert(mf.subVector(3,6,4,2,3) == mf.subMatrix(3,11,6,10,4,2).diag(),
           "subVectorFF");

    Assert(m.subMatrix(2,5,1,4) == mf.subMatrix(3,5,2,4),"subMatrixF");
    Assert(m.subMatrix(2,8,1,10,2,3) == mf.subMatrix(3,7,2,8,2,3),"subMatrixF");

    Assert(m.subVector(2,5,4,2,3) == m.subMatrix(2,14,5,11,4,2).diag(),
           "subVector");

    Assert(mf.subVector(3,6,4,2,3) == mf.subMatrix(3,11,6,10,4,2).diag(),
           "subVectorFF");

    Assert(m.subVector(2,5,4,2,3) == mf.subVector(3,6,4,2,3),"subVectorF");
    Assert(m.subVector(8,1,-1,2,4) == mf.subVector(9,2,-1,2,4),"subVector2F");
    Assert(m.subVector(12,8,-4,-2,2) == mf.subVector(13,9,-4,-2,2),
           "subVector3F");

    Assert(m.colPair(2,5) == m.subMatrix(0,M,2,8,1,3),"colPair");
    Assert(m.colPair(7,2) == m.subMatrix(0,M,7,-3,1,-5),"colPair");

    Assert(mf.colPair(3,6) == mf.subMatrix(1,M,3,6,1,3),"colPairFF");
    Assert(mf.colPair(8,3) == mf.subMatrix(1,M,8,3,1,-5),"colPairFF");

    Assert(m.colPair(2,5) == mf.colPair(3,6),"colPairF");
    Assert(m.colPair(7,2) == mf.colPair(8,3),"colPairF");


    Assert(m.rowPair(3,7) == m.subMatrix(3,11,0,N,4,1),"rowPair");
    Assert(m.rowPair(2,0) == m.subMatrix(2,-2,0,N,-2,1),"rowPair");

    Assert(mf.rowPair(4,8) == mf.subMatrix(4,8,1,N,4,1),"rowPairFF");
    Assert(mf.rowPair(3,1) == mf.subMatrix(3,1,1,N,-2,1),"rowPairFF");

    Assert(m.rowPair(3,7) == mf.rowPair(4,8),"rowPairF");
    Assert(m.rowPair(2,0) == mf.rowPair(3,1),"rowPairF");


    Assert(m.colRange(2,5) == m.subMatrix(0,M,2,5),"colRange");
    Assert(m.rowRange(3,7) == m.subMatrix(3,7,0,N),"rowRange");

    Assert(mf.colRange(3,5) == mf.subMatrix(1,M,3,5),"colRangeFF");
    Assert(mf.rowRange(4,7) == mf.subMatrix(4,7,1,N),"rowRangeFF");

    Assert(m.colRange(2,5) == mf.colRange(3,5),"colRangeF");
    Assert(m.rowRange(3,7) == mf.rowRange(4,7),"rowRangeF");

#undef Si
#undef Sj
}

template <class T> void TestSmallMatrix_Sub()
{
    DoTestSmallMatrix_Sub<T,15,10,tmv::RowMajor>();
    DoTestSmallMatrix_Sub<T,15,10,tmv::ColMajor>();
}

#ifdef TEST_DOUBLE
template void TestSmallMatrix_Sub<double>();
#endif
#ifdef TEST_FLOAT
template void TestSmallMatrix_Sub<float>();
#endif
#ifdef TEST_LONGDOUBLE
template void TestSmallMatrix_Sub<long double>();
#endif
#ifdef TEST_INT
template void TestSmallMatrix_Sub<int>();
#endif
