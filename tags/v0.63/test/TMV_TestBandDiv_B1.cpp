
#define START 0

#include "TMV.h"
#include "TMV_Band.h"
#include "TMV_Test.h"
#include "TMV_Test2.h"
#include "TMV_TestBandArith.h"

#include "TMV_TestMatrixDivArith.h"

template <class T> 
void TestBandDiv_B1(tmv::DivType dt)
{
    const int N = 10;

    std::vector<tmv::BandMatrixView<T> > b;
    std::vector<tmv::BandMatrixView<std::complex<T> > > cb;
    std::vector<tmv::BaseMatrix<T>*> B;
    std::vector<tmv::BaseMatrix<std::complex<T> >*> CB;
    MakeBandList(b,cb,B,CB);

    tmv::Matrix<T> a1(N,N);
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) a1(i,j) = T(3+i-2*j);
    a1.diag().addToAll(T(10)*N);
    a1 /= T(10);
    tmv::Matrix<std::complex<T> > ca1 = a1 * std::complex<T>(3,-4);

    tmv::Matrix<T> a1x = a1;
    tmv::Matrix<std::complex<T> > ca1x = ca1;

#ifdef XTEST
    tmv::Matrix<T> a3 = a1.colRange(0,N/2);
    tmv::Matrix<std::complex<T> > ca3 = ca1.colRange(0,N/2);
    tmv::Matrix<T> a4 = a1.rowRange(0,N/2);
    tmv::Matrix<std::complex<T> > ca4 = ca1.rowRange(0,N/2);
    tmv::Matrix<T> a5 = a1.colRange(0,0);
    tmv::Matrix<std::complex<T> > ca5 = ca1.colRange(0,0);
    tmv::Matrix<T> a6 = a1.rowRange(0,0);
    tmv::Matrix<std::complex<T> > ca6 = ca1.rowRange(0,0);

    tmv::Matrix<T> a3x = a3;
    tmv::Matrix<T> a4x = a4;
    tmv::Matrix<T> a5x = a5;
    tmv::Matrix<T> a6x = a6;
    tmv::Matrix<std::complex<T> > ca3x = ca3;
    tmv::Matrix<std::complex<T> > ca4x = ca4;
    tmv::Matrix<std::complex<T> > ca5x = ca5;
    tmv::Matrix<std::complex<T> > ca6x = ca6;
#endif

    for(size_t i=START;i<b.size();i++) {
        if (showstartdone) 
            std::cout<<"Start loop: i = "<<i<<"\nbi = "<<tmv::TMV_Text(b[i])<<
                "  "<<b[i]<<std::endl;
        const tmv::BandMatrixView<T>& bi = b[i];
        const tmv::BandMatrixView<std::complex<T> >& cbi = cb[i];
        if (dt == tmv::LU && !bi.isSquare()) continue;

        bi.saveDiv();
        cbi.saveDiv();

        TestMatrixDivArith1<T>(dt,a1x,ca1x,bi,a1.view(),cbi,ca1.view(),
                               "SquareMatrix/Band");
#ifdef XTEST
        TestMatrixDivArith1<T>(dt,a3x,ca3x,bi,a3.view(),cbi,ca3.view(),
                               "NonSquareMatrix/Band");
        TestMatrixDivArith1<T>(dt,a4x,ca4x,bi,a4.view(),cbi,ca4.view(),
                               "NonSquareMatrix/Band");
        TestMatrixDivArith1<T>(dt,a5x,ca5x,bi,a5.view(),cbi,ca5.view(),
                               "DegenerateMatrix/Band");
        TestMatrixDivArith1<T>(dt,a6x,ca6x,bi,a6.view(),cbi,ca6.view(),
                               "DegenerateMatrix/Band");
#endif
    }
    for(size_t i=0;i<B.size();++i) delete B[i];
    for(size_t i=0;i<CB.size();++i) delete CB[i];
}

#ifdef INST_DOUBLE
template void TestBandDiv_B1<double>(tmv::DivType dt);
#endif
#ifdef INST_FLOAT
template void TestBandDiv_B1<float>(tmv::DivType dt);
#endif
#ifdef INST_LONGDOUBLE
template void TestBandDiv_B1<long double>(tmv::DivType dt);
#endif