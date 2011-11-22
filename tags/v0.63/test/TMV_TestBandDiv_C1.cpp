
#define START 0

#include "TMV.h"
#include "TMV_Band.h"
#include "TMV_Test.h"
#include "TMV_Test2.h"
#include "TMV_TestBandArith.h"

#define NOLDIVEQ
#define NORDIVEQ
#include "TMV_TestMatrixDivArith.h"

template <class T> 
void TestBandDiv_C1(tmv::DivType dt)
{
    const int N = 10;

    std::vector<tmv::BandMatrixView<T> > b;
    std::vector<tmv::BandMatrixView<std::complex<T> > > cb;
    std::vector<tmv::BaseMatrix<T>*> B;
    std::vector<tmv::BaseMatrix<std::complex<T> >*> CB;
    MakeBandList(b,cb,B,CB);

    tmv::Matrix<T> a1(N,N);
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) a1(i,j) = T(1-3*i+j);
    a1.diag().addToAll(T(10)*N);
    a1 /= T(10);
    tmv::Matrix<std::complex<T> > ca1 = a1 * std::complex<T>(3,-4);

    tmv::DiagMatrix<T> d(a1);
    tmv::DiagMatrix<std::complex<T> > cd(ca1);
    tmv::DiagMatrix<T> dx = d;
    tmv::DiagMatrix<std::complex<T> > cdx = cd;

    for(size_t i=START;i<b.size();i++) {
        if (showstartdone) 
            std::cout<<"Start loop: i = "<<i<<"\nbi = "<<tmv::TMV_Text(b[i])<<
                "  "<<b[i]<<std::endl;
        const tmv::BandMatrixView<T>& bi = b[i];
        const tmv::BandMatrixView<std::complex<T> >& cbi = cb[i];
        if (dt == tmv::LU && !bi.isSquare()) continue;

        bi.saveDiv();
        cbi.saveDiv();

        TestMatrixDivArith1<T>(dt,dx,cdx,bi,d.view(),cbi,cd.view(),
                               "DiagMatrix/Band");
    }
    for(size_t i=0;i<B.size();++i) delete B[i];
    for(size_t i=0;i<CB.size();++i) delete CB[i];
}

#ifdef INST_DOUBLE
template void TestBandDiv_C1<double>(tmv::DivType dt);
#endif
#ifdef INST_FLOAT
template void TestBandDiv_C1<float>(tmv::DivType dt);
#endif
#ifdef INST_LONGDOUBLE
template void TestBandDiv_C1<long double>(tmv::DivType dt);
#endif