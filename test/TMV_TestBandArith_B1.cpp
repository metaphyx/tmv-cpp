#define START 0

#include "TMV_Test.h"
#include "TMV_Test_2.h"
#include "TMV.h"
#include "TMV_Band.h"
#include "TMV_TestBandArith.h"

#if 0
template <class M1, class M2>
inline bool CanAddEq(
    const tmv::BaseMatrix_Band_Mutable<M1>& , const tmv::BaseMatrix_Rec<M2>& )
{ return false; }

template <class M1, class M2, class M3> 
inline bool CanMult(
    const tmv::BaseMatrix_Band<M1>& , const tmv::BaseMatrix_Rec<M2>& ,
    const tmv::BaseMatrix_Band_Mutable<M3>& )
{ return false; }

template <class M1, class M2, class M3> 
inline bool CanMult(
    const tmv::BaseMatrix_Rec<M1>& , const tmv::BaseMatrix_Band<M2>& ,
    const tmv::BaseMatrix_Band_Mutable<M3>& )
{ return false; }

template <class M1, class M2, class M3> 
inline bool CanMult(
    const tmv::BaseMatrix_Rec<M1>& , const tmv::BaseMatrix_Rec<M2>& ,
    const tmv::BaseMatrix_Band_Mutable<M3>& )
{ return false; }
#endif

template <class M1, class M2, class M3> 
inline bool CanElemMult(
    const tmv::BaseMatrix_Band<M1>& a, const tmv::BaseMatrix_Rec<M2>& b,
    const tmv::BaseMatrix_Band_Mutable<M3>& c)
{ 
    return a.rowsize() == c.rowsize() && a.colsize() == c.colsize() &&
        b.rowsize() == c.rowsize() && b.colsize() == c.colsize() &&
        c.nlo() >= a.nlo() && c.nhi() >= a.nhi();
}

#define NOADDEQ
#define NOMULTEQ
#include "TMV_TestMatrixArith.h"

template <class T> 
void TestBandMatrixArith_B1()
{
    std::vector<tmv::BandMatrixView<T> > b;
    std::vector<tmv::BandMatrixView<std::complex<T> > > cb;
    MakeBandList(b,cb);

    const int N = b[0].rowsize();

    tmv::Matrix<T> a1(N,N);
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) a1(i,j) = T(3+i-5*j);

    tmv::Matrix<std::complex<T> > ca1(N,N);
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j)
        ca1(i,j) = std::complex<T>(3+i-5*j,4-8*i-j);

    tmv::MatrixView<T> a1v = a1.view();
    tmv::MatrixView<std::complex<T> > ca1v = ca1.view();

#if (XTEST & 2)
    tmv::Matrix<T> a2(2*N,2*N);
    for (int i=0; i<2*N; ++i) for (int j=0; j<2*N; ++j) a2(i,j) = T(1-3*i+3*j);
    tmv::Matrix<std::complex<T> > ca2(2*N,2*N);
    for (int i=0; i<2*N; ++i) for (int j=0; j<2*N; ++j)
        ca2(i,j) = std::complex<T>(1-3*i+6*j,8+2*i-6*j);
    tmv::Matrix<T,tmv::RowMajor> a3 = a2.rowRange(0,N);
    tmv::Matrix<std::complex<T> > ca3 = ca2.rowRange(0,N);
    tmv::Matrix<T,tmv::RowMajor> a4 = a1.colRange(0,0);
    tmv::Matrix<std::complex<T> > ca4 = ca1.colRange(0,0);

    tmv::MatrixView<T> a3v = a3.view();
    tmv::MatrixView<T> a4v = a4.view();
    tmv::MatrixView<std::complex<T> > ca3v = ca3.view();
    tmv::MatrixView<std::complex<T> > ca4v = ca4.view();
#endif

    for(size_t i=START;i<b.size();i++) {
        if (showstartdone) {
            std::cerr<<"Start loop "<<i<<std::endl;
            std::cerr<<"bi = "<<b[i]<<std::endl;
        }
        tmv::BandMatrixView<T> bi = b[i];
        tmv::BandMatrixView<std::complex<T> > cbi = cb[i];
        tmv::Matrix<T> mx = bi;
        tmv::Matrix<std::complex<T> > cmx = cbi;

        TestMatrixArith4(bi,cbi,a1v,ca1v,"Band/SquareM");
        TestMatrixArith5(bi,cbi,a1v,ca1v,"Band/SquareM");
        TestMatrixArith6x(bi,cbi,a1v,ca1v,"Band/SquareM");
#if (XTEST & 2)
        TestMatrixArith4(bi,cbi,a3v,ca3v,"Band/NonSquareM");
        TestMatrixArith5(bi,cbi,a3v,ca3v,"Band/NonSquareM");
        TestMatrixArith6x(bi,cbi,a3v,ca3v,"Band/NonSquareM");
        TestMatrixArith4(bi,cbi,a4v,ca4v,"Band/DegenerateM");
        TestMatrixArith5(bi,cbi,a4v,ca4v,"Band/DegenerateM");
        TestMatrixArith6x(bi,cbi,a4v,ca4v,"Band/DegenerateM");
#endif
    }
}

#ifdef TEST_DOUBLE
template void TestBandMatrixArith_B1<double>();
#endif
#ifdef TEST_FLOAT
template void TestBandMatrixArith_B1<float>();
#endif
#ifdef TEST_LONGDOUBLE
template void TestBandMatrixArith_B1<long double>();
#endif
#ifdef TEST_INT
template void TestBandMatrixArith_B1<int>();
#endif
