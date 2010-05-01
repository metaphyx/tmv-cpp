
#include "TMV.h"
#include "TMV_Test.h"
#include "TMV_Test1.h"
#include <fstream>

#define NOADDEQ
#define NOMULTEQ
#define NOSV
#include "TMV_TestMatrixArith.h"

template <class T> 
void TestDiagMatrixArith_B1()
{
    const int N = 10;

    tmv::DiagMatrix<T> a(N);
    tmv::DiagMatrix<T> b(N);
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) 
        if (i == j) {
            a(i,j) = T(3+i+5*j);
            b(i,j) = T(5+2*i+4*j);
        }

    tmv::DiagMatrix<std::complex<T> > ca = a*std::complex<T>(1,2);
    tmv::DiagMatrix<std::complex<T> > cb = b*std::complex<T>(-5,-1);

    tmv::Matrix<T> p(N,N,5);
    p.diag().addToAll(N*10);
    tmv::Matrix<std::complex<T> > cp = p*std::complex<T>(2,3);
    cp += ca;

    tmv::Matrix<T> q(2*N,N,-2);
    q.rowRange(0,N) += p;
    q.rowRange(N,2*N) -= p;
    q.rowRange(N/2,3*N/2) += T(4)*p;
    tmv::Matrix<std::complex<T> > cq = q*std::complex<T>(-1,4);
    cq.rowRange(0,N) -= cp;
    cq.rowRange(N,2*N) += cp;
    cq.rowRange(N/2,3*N/2) -= T(4)*cp;

    tmv::Matrix<T> r(N,0,T(1));
    tmv::Matrix<std::complex<T> > cr(N,0,T(1));

    tmv::Matrix<T> ax = a;
    tmv::Matrix<std::complex<T> > cax = ca;

    tmv::DiagMatrixView<T> av = a.view();
    tmv::DiagMatrixView<std::complex<T> > cav = ca.view();
    tmv::MatrixView<T> pv = p.view();
    tmv::MatrixView<std::complex<T> > cpv = cp.view();

    TestMatrixArith456<T>(ax,cax,av,cav,pv,cpv, "Diag/SquareM");

    tmv::MatrixView<T> qv = q.view();
    tmv::MatrixView<std::complex<T> > cqv = cq.view();
    tmv::MatrixView<T> rv = r.view();
    tmv::MatrixView<std::complex<T> > crv = cr.view();

    TestMatrixArith456<T>(ax,cax,av,cav,qv,cqv, "Diag/NonSquareM");
    TestMatrixArith456<T>(ax,cax,av,cav,rv,crv, "Diag/DegenM");
}

#ifdef INST_DOUBLE
template void TestDiagMatrixArith_B1<double>();
#endif
#ifdef INST_FLOAT
template void TestDiagMatrixArith_B1<float>();
#endif
#ifdef INST_LONGDOUBLE
template void TestDiagMatrixArith_B1<long double>();
#endif
#ifdef INST_INT
template void TestDiagMatrixArith_B1<int>();
#endif