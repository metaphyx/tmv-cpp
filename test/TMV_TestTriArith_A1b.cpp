
#include "TMV_Test.h"
#include "TMV_Test_1.h"
#include "TMV.h"

template <class M0, class M1>
inline void CopyBack(
    const tmv::BaseMatrix_Tri<M0>& m0, tmv::BaseMatrix_Tri_Mutable<M1>& m1)
{
#ifdef XXD
    if (showtests) {
        std::cout<<"Special CopyBackM:\n";
        std::cout<<"m0 = "<<tmv::TMV_Text(m0)<<std::endl;
        std::cout<<"m1 = "<<tmv::TMV_Text(m1)<<std::endl;
    }
#endif
    if (m1.isunit()) m1 = m0.viewAsUnitDiag();
    else m1 = m0;
}

template <class M1, class M2, class M3>
static inline bool CanElemMult(
    const tmv::BaseMatrix_Tri<M1>& a, const tmv::BaseMatrix_Tri<M2>& b,
    const tmv::BaseMatrix_Tri_Mutable<M3>& c)
{
#ifdef XXD
    if (showtests) {
        std::cout<<"Tri: CanElemMult:\n";
        std::cout<<"a = "<<tmv::TMV_Text(a)<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<std::endl;
    }
#endif
    return (a.size() == b.size()) && (a.size() == c.size()) &&
        (!c.isunit() || (a.isunit() && b.isunit()));
}

template <class M1, class M2, class M3>
static inline bool CanAddElemMult(
    const tmv::BaseMatrix_Tri<M1>& a, const tmv::BaseMatrix_Tri<M2>& b,
    const tmv::BaseMatrix_Tri_Mutable<M3>& c)
{
#ifdef XXD
    if (showtests) {
        std::cout<<"Tri: CanAddElemMult:\n";
        std::cout<<"a = "<<tmv::TMV_Text(a)<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<std::endl;
    }
#endif
    return (a.size() == b.size()) && (a.size() == c.size()) &&
        !c.isunit();
}

#define NOADDEQ
#define NOADDEQX
#define NOMULTEQX
#define BASIC_MULTMM_ONLY
#include "TMV_TestMatrixArith.h"

template <class T> void TestTriMatrixArith_A1b()
{
    typedef std::complex<T> CT;

    tmv::Matrix<T,tmv::RowMajor> a1x(4,4);
    for(int i=0;i<4;++i) for(int j=0;j<4;++j) {
        a1x(i,j) = T(3+4*i-6*j);
    }
    a1x(0,0) = 14; 
    a1x(1,0) = -2; 
    a1x(2,0) = 7; 
    a1x(3,0) = -10;
    a1x(2,2) = 30;

    tmv::Matrix<CT,tmv::RowMajor> ca1x = a1x;
    ca1x(2,3) += CT(2,3);
    ca1x(1,0) *= CT(0,2);
    ca1x.col(1) *= CT(-1,3);
    ca1x.row(3) += tmv::Vector<CT>(4,CT(1,9));

    tmv::Matrix<T,tmv::ColMajor> a2x = a1x.transpose();
    a2x.row(1) *= T(3);
    a2x.col(2) -= tmv::Vector<T>(4,4);
    tmv::Matrix<CT,tmv::ColMajor> ca2x = ca1x;
    ca2x -= a2x;
    ca2x *= CT(1,-2);
    ca2x(0,0) = CT(7,12);

    tmv::UpperTriMatrixView<T> u4 = a1x.unitUpperTri();
    tmv::UpperTriMatrixView<CT> cu4 = ca1x.unitUpperTri();
    tmv::UpperTriMatrixView<T> u5 = a2x.unitUpperTri();
    tmv::UpperTriMatrixView<CT> cu5 = ca2x.unitUpperTri();

    TestMatrixArith1(u4,cu4,"UpperTri 4");
    TestMatrixArith1(u5,cu5,"UpperTri 5");

#if (XTEST & 1)
    tmv::Matrix<T> a3x(12,16);
    for(int i=0;i<12;++i) for(int j=0;j<16;++j) a3x(i,j) = T(1-2*i+3*j);
    a3x.diag().addToAll(30);
    tmv::Matrix<CT> ca3x = a3x*CT(1,-2);
    ca3x.diag().addToAll(CT(-22,15));

    tmv::UpperTriMatrixView<T> u6 = a3x.subMatrix(0,12,0,16,3,4).unitUpperTri();
    tmv::UpperTriMatrixView<CT> cu6 = ca3x.subMatrix(0,12,0,16,3,4).unitUpperTri();
    TestMatrixArith1(u6,cu6,"UpperTri 6");
#endif

#if (XTEST & 2)
    tmv::LowerTriMatrixView<T> l4 = a1x.unitLowerTri();
    tmv::LowerTriMatrixView<CT> cl4 = ca1x.unitLowerTri();
    tmv::LowerTriMatrixView<T> l5 = a2x.unitLowerTri();
    tmv::LowerTriMatrixView<CT> cl5 = ca2x.unitLowerTri();

    TestMatrixArith1(l4,cl4,"LowerTri 4");
    TestMatrixArith1(l5,cl5,"LowerTri 5");
#if (XTEST & 1)
    tmv::LowerTriMatrixView<T> l6 = a3x.subMatrix(0,12,0,16,3,4).unitLowerTri();
    tmv::LowerTriMatrixView<CT> cl6 = ca3x.subMatrix(0,12,0,16,3,4).unitLowerTri();
    TestMatrixArith1(l6,cl6,"LowerTri 6");
#endif
#endif

}

#ifdef TEST_DOUBLE
template void TestTriMatrixArith_A1b<double>();
#endif
#ifdef TEST_FLOAT
template void TestTriMatrixArith_A1b<float>();
#endif
#ifdef TEST_LONGDOUBLE
template void TestTriMatrixArith_A1b<long double>();
#endif
#ifdef TEST_INT
template void TestTriMatrixArith_A1b<int>();
#endif
