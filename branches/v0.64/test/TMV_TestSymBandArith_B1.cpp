#define START 0

#include "TMV.h"
#include "TMV_SymBand.h"
#include "TMV_Test.h"
#include "TMV_Test2.h"
#include "TMV_TestSymBandArith.h"

#define NOADDEQ
#define NOMULTEQ

#include "TMV_TestMatrixArith.h"

template <class T> 
void TestSymBandMatrixArith_B1()
{
    const int N = 10;

    std::vector<tmv::SymBandMatrixView<T> > sb;
    std::vector<tmv::SymBandMatrixView<std::complex<T> > > csb;
    std::vector<tmv::BaseMatrix<T>*> B;
    std::vector<tmv::BaseMatrix<std::complex<T> >*> CB;
    MakeSymBandList(sb,csb,B,CB,InDef);

    tmv::Matrix<T> a1(N,N);
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) a1(i,j) = T(3+i-5*j);
    tmv::Matrix<std::complex<T> > ca1(N,N);
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) ca1(i,j) =
        std::complex<T>(3+i-5*j,2-3*i);

    tmv::MatrixView<T> a1v = a1.view();
    tmv::MatrixView<std::complex<T> > ca1v = ca1.view();

#ifdef XTEST
    tmv::Matrix<T> a2(2*N,2*N);
    for (int i=0; i<2*N; ++i) for (int j=0; j<2*N; ++j) a2(i,j) = T(1-3*i+6*j);
    tmv::Matrix<std::complex<T> > ca2(2*N,2*N);
    for (int i=0; i<2*N; ++i) for (int j=0; j<2*N; ++j) ca2(i,j) =
        std::complex<T>(1-3*i+6*j,-4+2*j);

    tmv::Matrix<T,tmv::RowMajor> a3 = a2.rowRange(0,N);
    tmv::Matrix<std::complex<T> > ca3 = a3 * std::complex<T>(-3,4);
    tmv::Matrix<T,tmv::RowMajor> a4 = a1.colRange(0,0);
    tmv::Matrix<std::complex<T> > ca4 = a4;

    tmv::MatrixView<T> a3v = a3.view();
    tmv::MatrixView<T> a4v = a4.view();
    tmv::MatrixView<std::complex<T> > ca3v = ca3.view();
    tmv::MatrixView<std::complex<T> > ca4v = ca4.view();
#endif

    for(size_t i=START;i<sb.size();i++) {
        if (showstartdone) {
            std::cout<<"Start loop i = "<<i<<std::endl;
            std::cout<<"si = "<<sb[i]<<std::endl;
        }

        tmv::SymBandMatrixView<T> si = sb[i];
        tmv::SymBandMatrixView<std::complex<T> > csi = csb[i];

        TestMatrixArith4<T>(si,csi,a1v,ca1v,"SymBand/SquareM");
        TestMatrixArith5<T>(si,csi,a1v,ca1v,"SymBand/SquareM");
        TestMatrixArith6x<T>(si,csi,a1v,ca1v,"SymBand/SquareM");
#ifdef XTEST
        TestMatrixArith4<T>(si,csi,a3v,ca3v,"SymBand/NonSquareM");
        TestMatrixArith5<T>(si,csi,a3v,ca3v,"SymBand/NonSquareM");
        TestMatrixArith6x<T>(si,csi,a3v,ca3v,"SymBand/NonSquareM");
        TestMatrixArith4<T>(si,csi,a4v,ca4v,"SymBand/DegenerateM");
        TestMatrixArith5<T>(si,csi,a4v,ca4v,"SymBand/DegenerateM");
        TestMatrixArith6x<T>(si,csi,a4v,ca4v,"SymBand/DegenerateM");
#endif
    }
    for(size_t i=0;i<B.size();++i) delete B[i];
    for(size_t i=0;i<CB.size();++i) delete CB[i];
}

#ifdef TEST_DOUBLE
template void TestSymBandMatrixArith_B1<double>();
#endif
#ifdef TEST_FLOAT
template void TestSymBandMatrixArith_B1<float>();
#endif
#ifdef TEST_LONGDOUBLE
template void TestSymBandMatrixArith_B1<long double>();
#endif
#ifdef TEST_INT
template void TestSymBandMatrixArith_B1<int>();
#endif