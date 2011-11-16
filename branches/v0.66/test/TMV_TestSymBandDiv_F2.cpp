
#define START 0

#include "TMV.h"
#include "TMV_SymBand.h"
#include "TMV_Test.h"
#include "TMV_Test_2.h"
#include "TMV_TestSymBandArith.h"

#define NOLDIVEQ
#define NORDIVEQ
#include "TMV_TestMatrixDivArith.h"

template <class T> 
void TestSymBandDiv_F2(tmv::DivType dt, PosDefCode pdc)
{
    const int N = 10;

    std::vector<tmv::SymBandMatrixView<T> > sb;
    std::vector<tmv::SymBandMatrixView<std::complex<T> > > csb;
    MakeSymBandList(sb,csb,pdc);

    tmv::Matrix<T> a1(N,N);
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) a1(i,j) = T(1-3*i+j);
    a1.diag().addToAll(T(10)*N);
    a1 /= T(10);
    tmv::Matrix<std::complex<T> > ca1 = a1 * std::complex<T>(3,-4);

    tmv::HermMatrix<T> h(a1);
    tmv::HermMatrix<std::complex<T> > ch(ca1);
    tmv::SymMatrix<T> s(a1);
    tmv::SymMatrix<std::complex<T> > cs(ca1);

    tmv::SymMatrixView<T> hv = h.view();
    tmv::SymMatrixView<std::complex<T> > chv = ch.view();
    tmv::SymMatrixView<T> sv = s.view();
    tmv::SymMatrixView<std::complex<T> > csv = cs.view();

    for(size_t i=START;i<sb.size();i++) {
        if (showstartdone)
            std::cout<<"Start loop: i = "<<i<<", si = "<<tmv::TMV_Text(sb[i])<<
                "  "<<sb[i]<<std::endl;
        tmv::SymBandMatrixView<T> si = sb[i];
        tmv::SymBandMatrixView<std::complex<T> > csi = csb[i];
        if (dt == tmv::CH && csi.issym()) continue;

        TestMatrixDivArith1<T>(dt,hv,si,chv,csi,"SymBand/HermMatrix");
        if (dt != tmv::CH)
            TestMatrixDivArith1<T>(dt,sv,si,csv,csi,"SymBand/SymMatrix");
    }
}

#ifdef TEST_DOUBLE
template void TestSymBandDiv_F2<double>(tmv::DivType dt, PosDefCode pdc);
#endif
#ifdef TEST_FLOAT
template void TestSymBandDiv_F2<float>(tmv::DivType dt, PosDefCode pdc);
#endif
#ifdef TEST_LONGDOUBLE
template void TestSymBandDiv_F2<long double>(tmv::DivType dt, PosDefCode pdc);
#endif
