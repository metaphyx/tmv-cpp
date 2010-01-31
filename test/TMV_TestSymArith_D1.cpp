#define START 0

#include "TMV.h"
#include "TMV_Sym.h"
#include "TMV_Test.h"
#include "TMV_Test2.h"
#include "TMV_TestSymArith.h"

#define NOADDEQ
#define NOMULTEQ

#include "TMV_TestMatrixArith.h"

template <class T> 
void TestSymMatrixArith_D1()
{
    const int N = 10;

    std::vector<tmv::SymMatrixView<T> > s;
    std::vector<tmv::SymMatrixView<std::complex<T> > > cs;
    std::vector<tmv::BaseMatrix<T>*> B;
    std::vector<tmv::BaseMatrix<std::complex<T> >*> CB;
    MakeSymList(s,cs,B,CB,InDef);

    tmv::Matrix<T> a1(N,N);
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) a1(i,j) = T(3+i-5*j);
    tmv::Matrix<std::complex<T> > ca1(N,N);
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) ca1(i,j) = 
        std::complex<T>(3+i-5*j,2-3*i);

    tmv::UpperTriMatrix<T,tmv::NonUnitDiag,tmv::RowMajor> u1(a1);
    tmv::UpperTriMatrix<std::complex<T>,tmv::NonUnitDiag,tmv::RowMajor> cu1(ca1);
    tmv::UpperTriMatrixView<T> u1v = u1.view();
    tmv::UpperTriMatrixView<std::complex<T> > cu1v = cu1.view();
    tmv::UpperTriMatrix<T,tmv::NonUnitDiag> u1x = u1v;
    tmv::UpperTriMatrix<std::complex<T>,tmv::NonUnitDiag> cu1x = cu1v;

#ifdef XTEST
    tmv::UpperTriMatrix<T,tmv::UnitDiag,tmv::RowMajor> u2(a1);
    tmv::UpperTriMatrix<T,tmv::NonUnitDiag,tmv::ColMajor> u3(a1);
    tmv::UpperTriMatrix<T,tmv::UnitDiag,tmv::ColMajor> u4(a1);
    tmv::LowerTriMatrix<T,tmv::NonUnitDiag,tmv::RowMajor> l1(a1);
    tmv::LowerTriMatrix<T,tmv::UnitDiag,tmv::RowMajor> l2(a1);
    tmv::LowerTriMatrix<T,tmv::NonUnitDiag,tmv::ColMajor> l3(a1);
    tmv::LowerTriMatrix<T,tmv::UnitDiag,tmv::ColMajor> l4(a1);

    tmv::UpperTriMatrix<std::complex<T>,tmv::UnitDiag,tmv::RowMajor> cu2(ca1);
    tmv::UpperTriMatrix<std::complex<T>,tmv::NonUnitDiag,tmv::ColMajor> cu3(ca1);
    tmv::UpperTriMatrix<std::complex<T>,tmv::UnitDiag,tmv::ColMajor> cu4(ca1);
    tmv::LowerTriMatrix<std::complex<T>,tmv::NonUnitDiag,tmv::RowMajor> cl1(ca1);
    tmv::LowerTriMatrix<std::complex<T>,tmv::UnitDiag,tmv::RowMajor> cl2(ca1);
    tmv::LowerTriMatrix<std::complex<T>,tmv::NonUnitDiag,tmv::ColMajor> cl3(ca1);
    tmv::LowerTriMatrix<std::complex<T>,tmv::UnitDiag,tmv::ColMajor> cl4(ca1);

    tmv::UpperTriMatrixView<T> u2v = u2.view();
    tmv::UpperTriMatrixView<T> u3v = u3.view();
    tmv::UpperTriMatrixView<T> u4v = u4.view();
    tmv::LowerTriMatrixView<T> l1v = l1.view();
    tmv::LowerTriMatrixView<T> l2v = l2.view();
    tmv::LowerTriMatrixView<T> l3v = l3.view();
    tmv::LowerTriMatrixView<T> l4v = l4.view();
    tmv::UpperTriMatrixView<std::complex<T> > cu2v = cu2.view();
    tmv::UpperTriMatrixView<std::complex<T> > cu3v = cu3.view();
    tmv::UpperTriMatrixView<std::complex<T> > cu4v = cu4.view();
    tmv::LowerTriMatrixView<std::complex<T> > cl1v = cl1.view();
    tmv::LowerTriMatrixView<std::complex<T> > cl2v = cl2.view();
    tmv::LowerTriMatrixView<std::complex<T> > cl3v = cl3.view();
    tmv::LowerTriMatrixView<std::complex<T> > cl4v = cl4.view();

    tmv::UpperTriMatrix<T,tmv::UnitDiag> u2x = u2v;
    tmv::LowerTriMatrix<T,tmv::NonUnitDiag> l1x = l1v;
    tmv::LowerTriMatrix<T,tmv::UnitDiag> l2x = l2v;
    tmv::UpperTriMatrix<std::complex<T>,tmv::UnitDiag> cu2x = cu2v;
    tmv::LowerTriMatrix<std::complex<T>,tmv::NonUnitDiag> cl1x = cl1v;
    tmv::LowerTriMatrix<std::complex<T>,tmv::UnitDiag> cl2x = cl2v;
#endif

    for(size_t i=START;i<s.size();i++) {
        if (showstartdone) {
            std::cout<<"Start loop i = "<<i<<std::endl;
            std::cout<<"si = "<<s[i]<<std::endl;
        }

        tmv::SymMatrixView<T> si = s[i];
        tmv::SymMatrixView<std::complex<T> > csi = cs[i];

        if (csi.isherm()) {
            tmv::HermMatrix<T> sx = si;
            tmv::HermMatrix<std::complex<T> > csx = csi;

            TestMatrixArith456<T>(sx,csx,si,csi,u1v,cu1v,"Herm/UpperTri");
#ifdef XTEST
            TestMatrixArith456<T>(sx,csx,si,csi,l1v,cl1v,"Herm/LowerTri");
            TestMatrixArith456<T>(sx,csx,si,csi,u2v,cu2v,"Herm/UpperTri");
            TestMatrixArith456<T>(sx,csx,si,csi,l2v,cl2v,"Herm/LowerTri");
            TestMatrixArith456<T>(sx,csx,si,csi,u3v,cu3v,"Herm/UpperTri");
            TestMatrixArith456<T>(sx,csx,si,csi,l3v,cl3v,"Herm/LowerTri");
            TestMatrixArith456<T>(sx,csx,si,csi,u4v,cu4v,"Herm/UpperTri");
            TestMatrixArith456<T>(sx,csx,si,csi,l4v,cl4v,"Herm/LowerTri");
#endif
        } else {
            tmv::SymMatrix<T> sx = si;
            tmv::SymMatrix<std::complex<T> > csx = csi;

            TestMatrixArith456<T>(sx,csx,si,csi,u1v,cu1v,"Sym/UpperTri");
#ifdef XTEST
            TestMatrixArith456<T>(sx,csx,si,csi,l1v,cl1v,"Sym/LowerTri");
            TestMatrixArith456<T>(sx,csx,si,csi,u2v,cu2v,"Sym/UpperTri");
            TestMatrixArith456<T>(sx,csx,si,csi,l2v,cl2v,"Sym/LowerTri");
            TestMatrixArith456<T>(sx,csx,si,csi,u3v,cu3v,"Sym/UpperTri");
            TestMatrixArith456<T>(sx,csx,si,csi,l3v,cl3v,"Sym/LowerTri");
            TestMatrixArith456<T>(sx,csx,si,csi,u4v,cu4v,"Sym/UpperTri");
            TestMatrixArith456<T>(sx,csx,si,csi,l4v,cl4v,"Sym/LowerTri");
#endif
        }
    }
    for(size_t i=0;i<B.size();++i) delete B[i];
    for(size_t i=0;i<CB.size();++i) delete CB[i];
}

#ifdef INST_DOUBLE
template void TestSymMatrixArith_D1<double>();
#endif
#ifdef INST_FLOAT
template void TestSymMatrixArith_D1<float>();
#endif
#ifdef INST_LONGDOUBLE
template void TestSymMatrixArith_D1<long double>();
#endif
#ifdef INST_INT
template void TestSymMatrixArith_D1<int>();
#endif
