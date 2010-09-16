
#include "TMV.h"
#include "TMV_Sym.h"
#include "TMV_Test.h"
#include "TMV_Test2.h"
#include <fstream>
#include <cstdio>

template <class T, tmv::UpLoType U, tmv::StorageType S> 
static void TestBasicSymMatrix_1()
{
    const int N = 6;

    tmv::SymMatrix<std::complex<T>,U,S> s1(N);
    tmv::SymMatrix<std::complex<T>,U,S> s2(N);
    tmv::SymMatrix<std::complex<T>,U,S,tmv::FortranStyle> s1f(N);
    tmv::SymMatrix<std::complex<T>,U,S,tmv::FortranStyle> s2f(N);

    Assert(int(s1.colsize()) == N && int(s1.rowsize()) == N,
           "Creating SymMatrix(N)");

    for (int i=0, k=1; i<N; ++i) for (int j=0; j<N; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        if (i<=j) {
            s1(i,j) = value; 
            s1f(i+1,j+1) = value; 
        }
        if (j<=i) {
            s2(i,j) = value; 
            s2f(i+1,j+1) = value; 
        }
    }

    tmv::SymMatrixView<std::complex<T> > s1v = s1.view();
    tmv::ConstSymMatrixView<std::complex<T> > s1cv = s1.view();
    tmv::SymMatrixView<std::complex<T> > s2v = s2.view();
    tmv::ConstSymMatrixView<std::complex<T> > s2cv = s2.view();
    tmv::SymMatrixView<std::complex<T>,tmv::FortranStyle> s1fv = s1f.view();
    tmv::ConstSymMatrixView<std::complex<T>,tmv::FortranStyle> s1fcv =
        s1f.view();
    tmv::SymMatrixView<std::complex<T>,tmv::FortranStyle> s2fv = s2f.view();
    tmv::ConstSymMatrixView<std::complex<T>,tmv::FortranStyle> s2fcv =
        s2f.view();
    const tmv::SymMatrix<std::complex<T>,U,S>& s1x = s1;
    const tmv::SymMatrix<std::complex<T>,U,S>& s2x = s2;
    const tmv::SymMatrix<std::complex<T>,U,S,tmv::FortranStyle>& s1fx = s1f;
    const tmv::SymMatrix<std::complex<T>,U,S,tmv::FortranStyle>& s2fx = s2f;

    for (int i=0, k=1; i<N; ++i) for (int j=0; j<N; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        if (showtests) std::cout<<"i,j = "<<i<<','<<j<<std::endl;
        if (i<=j) {
            Assert(s1(i,j) == value,"Read/Write SymMatrix");
            Assert(s1x(i,j) == value,"Access const SymMatrix");
            Assert(s1v(i,j) == value,"Access SymMatrix V");
            Assert(s1cv(i,j) == value,"Access SymMatrix CV");
            Assert(s1(j,i) == value,"Access SymMatrix - opposite tri");
            Assert(s1x(j,i) == value,"Access const SymMatrix - opposite tri");
            Assert(s1v(j,i) == value,"Access SymMatrix V");
            Assert(s1cv(j,i) == value,"Access SymMatrix CV");
            Assert(s1f(i+1,j+1) == value,"Read/Write SymMatrixF");
            Assert(s1fx(i+1,j+1) == value,"Access const SymMatrixF");
            Assert(s1fv(i+1,j+1) == value,"Access SymMatrixF V");
            Assert(s1fcv(i+1,j+1) == value,"Access SymMatrixF CV");
            Assert(s1f(j+1,i+1) == value,"Access SymMatrixF - opposite tri");
            Assert(s1fx(j+1,i+1) == value,
                   "Access const SymMatrixF - opposite tri");
            Assert(s1fv(j+1,i+1) == value,"Access SymMatrixF V");
            Assert(s1fcv(j+1,i+1) == value,"Access SymMatrixF CV");
            Assert(s1.row(i,i,N)(j-i) == value,"SymMatrix.row");
            Assert(s1x.row(i,i,N)(j-i) == value,"SymMatrix.row");
            Assert(s1cv.row(i,i,N)(j-i) == value,"SymMatrix.row CV");
            Assert(s1v.row(i,i,N)(j-i) == value,"SymMatrix.row V");
            Assert(s1.col(i,i,N)(j-i) == value,"SymMatrix.col");
            Assert(s1x.col(i,i,N)(j-i) == value,"SymMatrix.col");
            Assert(s1cv.col(i,i,N)(j-i) == value,"SymMatrix.col CV");
            Assert(s1v.col(i,i,N)(j-i) == value,"SymMatrix.col V");
            Assert(s1.row(i,j,N)(0) == value,"SymMatrix.row2");
            Assert(s1x.row(i,j,N)(0) == value,"SymMatrix.row2");
            Assert(s1cv.row(i,j,N)(0) == value,"SymMatrix.row2 CV");
            Assert(s1v.row(i,j,N)(0) == value,"SymMatrix.row2 V");
            Assert(s1.col(i,j,N)(0) == value,"SymMatrix.col2");
            Assert(s1x.col(i,j,N)(0) == value,"SymMatrix.col2");
            Assert(s1cv.col(i,j,N)(0) == value,"SymMatrix.col2 CV");
            Assert(s1v.col(i,j,N)(0) == value,"SymMatrix.col2 V");
            Assert(s1f.row(i+1,i+1,N)(j-i+1) == value,"SymMatrixF.row");
            Assert(s1fx.row(i+1,i+1,N)(j-i+1) == value,"const SymMatrixF.row");
            Assert(s1fcv.row(i+1,i+1,N)(j-i+1) == value,"SymMatrixF.row CV");
            Assert(s1fv.row(i+1,i+1,N)(j-i+1) == value,"SymMatrixF.row V");
            Assert(s1f.col(i+1,i+1,N)(j-i+1) == value,"SymMatrixF.col");
            Assert(s1fx.col(i+1,i+1,N)(j-i+1) == value,"const SymMatrixF.col");
            Assert(s1fcv.col(i+1,i+1,N)(j-i+1) == value,"SymMatrixF.col CV");
            Assert(s1fv.col(i+1,i+1,N)(j-i+1) == value,"SymMatrixF.col V");
            Assert(s1f.row(i+1,j+1,N)(1) == value,"SymMatrixF.row2");
            Assert(s1fx.row(i+1,j+1,N)(1) == value,"const SymMatrixF.row2");
            Assert(s1fcv.row(i+1,j+1,N)(1) == value,"SymMatrixF.row2 CV");
            Assert(s1fv.row(i+1,j+1,N)(1) == value,"SymMatrixF.row2 V");
            Assert(s1f.col(i+1,j+1,N)(1) == value,"SymMatrixF.col2");
            Assert(s1fx.col(i+1,j+1,N)(1) == value,"const SymMatrixF.col2");
            Assert(s1fcv.col(i+1,j+1,N)(1) == value,"SymMatrixF.col2 CV");
            Assert(s1fv.col(i+1,j+1,N)(1) == value,"SymMatrixF.col2 V");
            int d = int(j)-int(i);
            if (d==0) {
                Assert(s1.diag()(i) == value,"SymMatrix.diag");
                Assert(s1x.diag()(i) == value,"const SymMatrix.diag");
                Assert(s1cv.diag()(i) == value,"SymMatrix.diag CV");
                Assert(s1v.diag()(i) == value,"SymMatrix.diag V");
                Assert(s1f.diag()(i+1) == value,"SymMatrixF.diag");
                Assert(s1fx.diag()(i+1) == value,"const SymMatrixF.diag");
                Assert(s1fcv.diag()(i+1) == value,"SymMatrixF.diag CV");
                Assert(s1fv.diag()(i+1) == value,"SymMatrixF.diag V");
            }
            Assert(s1.diag(d)(i) == value,"SymMatrix.diag1");
            Assert(s1x.diag(d)(i) == value,"const SymMatrix.diag1");
            Assert(s1cv.diag(d)(i) == value,"SymMatrix.diag1 CV");
            Assert(s1v.diag(d)(i) == value,"SymMatrix.diag1 V");
            Assert(s1.diag(d,i,N-d)(0) == value,"SymMatrix.diag2");
            Assert(s1x.diag(d,i,N-d)(0) == value,"const SymMatrix.diag2");
            Assert(s1cv.diag(d,i,N-d)(0) == value,"SymMatrix.diag2 CV");
            Assert(s1v.diag(d,i,N-d)(0) == value,"SymMatrix.diag2 V");
            Assert(s1f.diag(d)(i+1) == value,"SymMatrixF.diag1");
            Assert(s1fx.diag(d)(i+1) == value,"const SymMatrixF.diag1");
            Assert(s1fcv.diag(d)(i+1) == value,"SymMatrixF.diag1 CV");
            Assert(s1fv.diag(d)(i+1) == value,"SymMatrixF.diag1 V");
            Assert(s1f.diag(d,i+1,N-d)(1) == value,"SymMatrixF.diag2");
            Assert(s1fx.diag(d,i+1,N-d)(1) == value,"const SymMatrixF.diag2");
            Assert(s1fcv.diag(d,i+1,N-d)(1) == value,"SymMatrixF.diag2 CV");
            Assert(s1fv.diag(d,i+1,N-d)(1) == value,"SymMatrixF.diag2 V");
            Assert(s1.diag(-d)(i) == value,"SymMatrix.diag1");
            Assert(s1x.diag(-d)(i) == value,"const SymMatrix.diag1");
            Assert(s1cv.diag(-d)(i) == value,"SymMatrix.diag1 CV");
            Assert(s1v.diag(-d)(i) == value,"SymMatrix.diag1 V");
            Assert(s1.diag(-d,i,N-d)(0) == value,"SymMatrix.diag2");
            Assert(s1x.diag(-d,i,N-d)(0) == value,"const SymMatrix.diag2");
            Assert(s1cv.diag(-d,i,N-d)(0) == value,"SymMatrix.diag2 CV");
            Assert(s1v.diag(-d,i,N-d)(0) == value,"SymMatrix.diag2 V");
            Assert(s1f.diag(-d)(i+1) == value,"SymMatrixF.diag1");
            Assert(s1fx.diag(-d)(i+1) == value,"const SymMatrixF.diag1");
            Assert(s1fcv.diag(-d)(i+1) == value,"SymMatrixF.diag1 CV");
            Assert(s1fv.diag(-d)(i+1) == value,"SymMatrixF.diag1 V");
            Assert(s1f.diag(-d,i+1,N-d)(1) == value,"SymMatrixF.diag2");
            Assert(s1fx.diag(-d,i+1,N-d)(1) == value,"const SymMatrixF.diag2");
            Assert(s1fcv.diag(-d,i+1,N-d)(1) == value,"SymMatrixF.diag2 CV");
            Assert(s1fv.diag(-d,i+1,N-d)(1) == value,"SymMatrixF.diag2 V");
        }
        if (j<=i) {
            Assert(s2(j,i) == value,"Read/Write SymMatrix");
            Assert(s2x(j,i) == value,"Access const SymMatrix");
            Assert(s2v(j,i) == value,"Access SymMatrix V");
            Assert(s2cv(j,i) == value,"Access SymMatrix CV");
            Assert(s2(i,j) == value,"Access SymMatrix - opposite tri");
            Assert(s2x(i,j) == value,"Access const SymMatrix - opposite tri");
            Assert(s2v(i,j) == value,"Access SymMatrix V");
            Assert(s2cv(i,j) == value,"Access SymMatrix CV");
            Assert(s2f(j+1,i+1) == value,"Read/Write SymMatrixF");
            Assert(s2fx(j+1,i+1) == value,"Access const SymMatrixF");
            Assert(s2fv(j+1,i+1) == value,"Access SymMatrixF V");
            Assert(s2fcv(j+1,i+1) == value,"Access SymMatrixF CV");
            Assert(s2f(i+1,j+1) == value,"Access SymMatrixF - opposite tri");
            Assert(s2fx(i+1,j+1) == value,
                   "Access const SymMatrixF - opposite tri");
            Assert(s2fv(i+1,j+1) == value,"Access SymMatrixF V");
            Assert(s2fcv(i+1,j+1) == value,"Access SymMatrixF CV");
            Assert(s2.row(i,0,i+1)(j) == value,"SymMatrix.row");
            Assert(s2x.row(i,0,i+1)(j) == value,"const SymMatrix.row");
            Assert(s2cv.row(i,0,i+1)(j) == value,"SymMatrix.row CV");
            Assert(s2v.row(i,0,i+1)(j) == value,"SymMatrix.row V");
            Assert(s2.col(i,0,i+1)(j) == value,"SymMatrix.col");
            Assert(s2x.col(i,0,i+1)(j) == value,"const SymMatrix.col");
            Assert(s2cv.col(i,0,i+1)(j) == value,"SymMatrix.col CV");
            Assert(s2v.col(i,0,i+1)(j) == value,"SymMatrix.col V");
            Assert(s2.row(i,j,i+1)(0) == value,"SymMatrix.row2");
            Assert(s2x.row(i,j,i+1)(0) == value,"const SymMatrix.row2");
            Assert(s2cv.row(i,j,i+1)(0) == value,"SymMatrix.row2 CV");
            Assert(s2v.row(i,j,i+1)(0) == value,"SymMatrix.row2 V");
            Assert(s2.col(i,j,i+1)(0) == value,"SymMatrix.col2");
            Assert(s2x.col(i,j,i+1)(0) == value,"const SymMatrix.col2");
            Assert(s2cv.col(i,j,i+1)(0) == value,"SymMatrix.col2 CV");
            Assert(s2v.col(i,j,i+1)(0) == value,"SymMatrix.col2 V");
            Assert(s2f.row(i+1,1,i+1)(j+1) == value,"SymMatrixF.row");
            Assert(s2fx.row(i+1,1,i+1)(j+1) == value,"const SymMatrixF.row");
            Assert(s2fcv.row(i+1,1,i+1)(j+1) == value,"SymMatrixF.row CV");
            Assert(s2fv.row(i+1,1,i+1)(j+1) == value,"SymMatrixF.row V");
            Assert(s2f.col(i+1,1,i+1)(j+1) == value,"SymMatrixF.col");
            Assert(s2fx.col(i+1,1,i+1)(j+1) == value,"const SymMatrixF.col");
            Assert(s2fcv.col(i+1,1,i+1)(j+1) == value,"SymMatrixF.col CV");
            Assert(s2fv.col(i+1,1,i+1)(j+1) == value,"SymMatrixF.col V");
            Assert(s2f.row(i+1,j+1,i+1)(1) == value,"SymMatrixF.row2");
            Assert(s2fx.row(i+1,j+1,i+1)(1) == value,"const SymMatrixF.row2");
            Assert(s2fcv.row(i+1,j+1,i+1)(1) == value,"SymMatrixF.row2 CV");
            Assert(s2fv.row(i+1,j+1,i+1)(1) == value,"SymMatrixF.row2 V");
            Assert(s2f.col(i+1,j+1,i+1)(1) == value,"SymMatrixF.col2");
            Assert(s2fx.col(i+1,j+1,i+1)(1) == value,"const SymMatrixF.col2");
            Assert(s2fcv.col(i+1,j+1,i+1)(1) == value,"SymMatrixF.col2 CV");
            Assert(s2fv.col(i+1,j+1,i+1)(1) == value,"SymMatrixF.col2 V");
            int d = int(j)-int(i);
            if (d==0) {
                Assert(s2.diag()(i) == value,"SymMatrix.diag");
                Assert(s2x.diag()(i) == value,"const SymMatrix.diag");
                Assert(s2cv.diag()(i) == value,"SymMatrix.diag CV");
                Assert(s2v.diag()(i) == value,"SymMatrix.diag V");
                Assert(s2f.diag()(i+1) == value,"SymMatrixF.diag");
                Assert(s2fx.diag()(i+1) == value,"const SymMatrixF.diag");
                Assert(s2fcv.diag()(i+1) == value,"SymMatrixF.diag CV");
                Assert(s2fv.diag()(i+1) == value,"SymMatrixF.diag V");
            }
            Assert(s2.diag(d)(j) == value,"SymMatrix.diag1");
            Assert(s2x.diag(d)(j) == value,"const SymMatrix.diag1");
            Assert(s2cv.diag(d)(j) == value,"SymMatrix.diag1 CV");
            Assert(s2v.diag(d)(j) == value,"SymMatrix.diag1 V");
            Assert(s2.diag(d,j,N+d)(0) == value,"SymMatrix.diag2");
            Assert(s2x.diag(d,j,N+d)(0) == value,"const SymMatrix.diag2");
            Assert(s2cv.diag(d,j,N+d)(0) == value,"SymMatrix.diag2 CV");
            Assert(s2v.diag(d,j,N+d)(0) == value,"SymMatrix.diag2 V");
            Assert(s2f.diag(d)(j+1) == value,"SymMatrixF.diag1");
            Assert(s2fx.diag(d)(j+1) == value,"const SymMatrixF.diag1");
            Assert(s2fcv.diag(d)(j+1) == value,"SymMatrixF.diag1 CV");
            Assert(s2fv.diag(d)(j+1) == value,"SymMatrixF.diag1 V");
            Assert(s2f.diag(d,j+1,N+d)(1) == value,"SymMatrixF.diag2");
            Assert(s2fx.diag(d,j+1,N+d)(1) == value,"const SymMatrixF.diag2");
            Assert(s2fcv.diag(d,j+1,N+d)(1) == value,"SymMatrixF.diag2 CV");
            Assert(s2fv.diag(d,j+1,N+d)(1) == value,"SymMatrixF.diag2 V");
            Assert(s2.diag(-d)(j) == value,"SymMatrix.diag1");
            Assert(s2x.diag(-d)(j) == value,"const SymMatrix.diag1");
            Assert(s2cv.diag(-d)(j) == value,"SymMatrix.diag1 CV");
            Assert(s2v.diag(-d)(j) == value,"SymMatrix.diag1 V");
            Assert(s2.diag(-d,j,N+d)(0) == value,"SymMatrix.diag2");
            Assert(s2x.diag(-d,j,N+d)(0) == value,"const SymMatrix.diag2");
            Assert(s2cv.diag(-d,j,N+d)(0) == value,"SymMatrix.diag2 CV");
            Assert(s2v.diag(-d,j,N+d)(0) == value,"SymMatrix.diag2 V");
            Assert(s2f.diag(-d)(j+1) == value,"SymMatrixF.diag1");
            Assert(s2fx.diag(-d)(j+1) == value,"const SymMatrixF.diag1");
            Assert(s2fcv.diag(-d)(j+1) == value,"SymMatrixF.diag1 CV");
            Assert(s2fv.diag(-d)(j+1) == value,"SymMatrixF.diag1 V");
            Assert(s2f.diag(-d,j+1,N+d)(1) == value,"SymMatrixF.diag2");
            Assert(s2fx.diag(-d,j+1,N+d)(1) == value,"const SymMatrixF.diag2");
            Assert(s2fcv.diag(-d,j+1,N+d)(1) == value,"SymMatrixF.diag2 CV");
            Assert(s2fv.diag(-d,j+1,N+d)(1) == value,"SymMatrixF.diag2 V");
        }
    }

    Assert(s1 == s1f,"CStyle SymMatrix == FortranStyle SymMatrix");
    Assert(s1 == s1cv,"SymMatrix == ConstSymMatrixView");
    Assert(s1 == s1v,"SymMatrix == SymMatrixView");
    Assert(s1 == s1fcv,"SymMatrix == FortranStyle ConstSymMatrixView");
    Assert(s1 == s1fv,"SymMatrix == FortranStyle SymMatrixView");
    Assert(s2 == s2f,"CStyle SymMatrix == FortranStyle SymMatrix");
    Assert(s2 == s2cv,"SymMatrix == ConstSymMatrixView");
    Assert(s2 == s2v,"SymMatrix == SymMatrixView");
    Assert(s2 == s2fcv,"SymMatrix == FortranStyle ConstSymMatrixView");
    Assert(s2 == s2fv,"SymMatrix == FortranStyle SymMatrixView");

    s1.resize(3);
    Assert(s1.colsize() == 3 && s1.rowsize() == 3,
           "SymMatrix s1.resize(3)");
    s2.resize(3);
    Assert(s2.colsize() == 3 && s2.rowsize() == 3,
           "SymMatrix s2.resize(3)");
    for (int i=0, k=0; i<3; ++i) for (int j=0; j<3; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        std::complex<T> hvalue = i==j ? std::complex<T>(T(k),0) : value;
        if (i <= j) s1(i,j) = hvalue; 
        if (j <= i) s2(i,j) = hvalue; 
    }
    for (int i=0, k=0; i<3; ++i) for (int j=0; j<3; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        std::complex<T> hvalue = i==j ? std::complex<T>(T(k),0) : value;
        if (i<=j) {
            Assert(s1(i,j) == hvalue,"Read/Write resized SymMatrix");
            Assert(s1(j,i) == hvalue,
                   "Read/Write resized SymMatrix opp tri");
        }
        if (j<=i) {
            Assert(s2(j,i) == hvalue,"Read/Write resized SymMatrix");
            Assert(s2(i,j) == hvalue,
                   "Read/Write resized SymMatrix opp tri");
        }
    }

    s1.resize(2*N);
    Assert(int(s1.colsize()) == 2*N && int(s1.rowsize()) == 2*N,
           "SymMatrix s1.resize(2*N) sizes");
    s2.resize(2*N);
    Assert(int(s2.colsize()) == 2*N && int(s2.rowsize()) == 2*N,
           "SymMatrix s2.resize(2*N) sizes");
    for (int i=0, k=0; i<2*N; ++i) for (int j=0; j<2*N; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        if (i <= j) s1(i,j) = value; 
        if (j <= i) s2(i,j) = value; 
    }
    for (int i=0, k=0; i<2*N; ++i) for (int j=0; j<2*N; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        if (i<=j) {
            Assert(s1(i,j) == value,"Read/Write resized SymMatrix");
            Assert(s1(j,i) == value,
                   "Read/Write resized SymMatrix opp tri");
        }
        if (j<=i) {
            Assert(s2(j,i) == value,"Read/Write resized SymMatrix");
            Assert(s2(i,j) == value,
                   "Read/Write resized SymMatrix opp tri");
        }
    }
}

template <class T, tmv::UpLoType U, tmv::StorageType S> 
static void TestBasicHermMatrix_1()
{
    const int N = 6;

    tmv::HermMatrix<std::complex<T>,U,S> h1(N);
    tmv::HermMatrix<std::complex<T>,U,S> h2(N);
    tmv::HermMatrix<std::complex<T>,U,S,tmv::FortranStyle> h1f(N);
    tmv::HermMatrix<std::complex<T>,U,S,tmv::FortranStyle> h2f(N);

    Assert(int(h1.colsize()) == N && int(h1.rowsize()) == N,
           "Creating HermMatrix(N)");

    for (int i=0, k=1; i<N; ++i) for (int j=0; j<N; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        std::complex<T> hvalue = i==j ? std::complex<T>(T(k),0) : value;
        if (i<=j) {
            h1(i,j) = hvalue; 
            h1f(i+1,j+1) = hvalue;
        }
        if (j<=i) {
            h2(i,j) = hvalue; 
            h2f(i+1,j+1) = hvalue; 
        }
    }

    tmv::SymMatrixView<std::complex<T> > h1v = h1.view();
    tmv::ConstSymMatrixView<std::complex<T> > h1cv = h1.view();
    tmv::SymMatrixView<std::complex<T> > h2v = h2.view();
    tmv::ConstSymMatrixView<std::complex<T> > h2cv = h2.view();
    tmv::SymMatrixView<std::complex<T>,tmv::FortranStyle> h1fv = h1f.view();
    tmv::ConstSymMatrixView<std::complex<T>,tmv::FortranStyle> h1fcv =
        h1f.view();
    tmv::SymMatrixView<std::complex<T>,tmv::FortranStyle> h2fv = h2f.view();
    tmv::ConstSymMatrixView<std::complex<T>,tmv::FortranStyle> h2fcv =
        h2f.view();
    const tmv::HermMatrix<std::complex<T>,U,S>& h1x = h1;
    const tmv::HermMatrix<std::complex<T>,U,S>& h2x = h2;
    const tmv::HermMatrix<std::complex<T>,U,S,tmv::FortranStyle>& h1fx = h1f;
    const tmv::HermMatrix<std::complex<T>,U,S,tmv::FortranStyle>& h2fx = h2f;

    for (int i=0, k=1; i<N; ++i) for (int j=0; j<N; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        std::complex<T> hvalue = i==j ? std::complex<T>(T(k),0) : value;
        if (showtests) std::cout<<"i,j = "<<i<<','<<j<<std::endl;
        if (i<=j) {
            Assert(h1(i,j) == hvalue,"Read/Write HermMatrix");
            Assert(h1x(i,j) == hvalue,"Access const HermMatrix");
            Assert(h1v(i,j) == hvalue,"Access HermMatrix V");
            Assert(h1cv(i,j) == hvalue,"Access HermMatrix CV");
            Assert(h1(j,i) == conj(hvalue),"Access HermMatrix - opposite tri");
            Assert(h1x(j,i) == conj(hvalue),
                   "Access const HermMatrix - opposite tri");
            Assert(h1v(j,i) == conj(hvalue),"Access HermMatrix V");
            Assert(h1cv(j,i) == conj(hvalue),"Access HermMatrix CV");
            Assert(h1f(i+1,j+1) == hvalue,"Read/Write HermMatrixF");
            Assert(h1fx(i+1,j+1) == hvalue,"Access const HermMatrixF");
            Assert(h1fv(i+1,j+1) == hvalue,"Access HermMatrixF V");
            Assert(h1fcv(i+1,j+1) == hvalue,"Access HermMatrixF CV");
            Assert(h1f(j+1,i+1) == conj(hvalue),
                   "Access HermMatrixF - opposite tri");
            Assert(h1fx(j+1,i+1) == conj(hvalue),
                   "Access const HermMatrixF - opposite tri");
            Assert(h1fv(j+1,i+1) == conj(hvalue),
                   "Access HermMatrixF V");
            Assert(h1fcv(j+1,i+1) == conj(hvalue),"Access HermMatrixF CV");
            Assert(h1.row(i,i,N)(j-i) == hvalue,"HermMatrix.row");
            Assert(h1x.row(i,i,N)(j-i) == hvalue,"HermMatrix.row");
            Assert(h1cv.row(i,i,N)(j-i) == hvalue,"HermMatrix.row CV");
            Assert(h1v.row(i,i,N)(j-i) == hvalue,"HermMatrix.row V");
            Assert(h1.col(i,i,N)(j-i) == conj(hvalue),"HermMatrix.col");
            Assert(h1x.col(i,i,N)(j-i) == conj(hvalue),"HermMatrix.col");
            Assert(h1cv.col(i,i,N)(j-i) == conj(hvalue),"HermMatrix.col CV");
            Assert(h1v.col(i,i,N)(j-i) == conj(hvalue),"HermMatrix.col V");
            Assert(h1.row(i,j,N)(0) == hvalue,"HermMatrix.row2");
            Assert(h1x.row(i,j,N)(0) == hvalue,"HermMatrix.row2");
            Assert(h1cv.row(i,j,N)(0) == hvalue,"HermMatrix.row2 CV");
            Assert(h1v.row(i,j,N)(0) == hvalue,"HermMatrix.row2 V");
            Assert(h1.col(i,j,N)(0) == conj(hvalue),"HermMatrix.col2");
            Assert(h1x.col(i,j,N)(0) == conj(hvalue),"HermMatrix.col2");
            Assert(h1cv.col(i,j,N)(0) == conj(hvalue),"HermMatrix.col2 CV");
            Assert(h1v.col(i,j,N)(0) == conj(hvalue),"HermMatrix.col2 V");
            Assert(h1f.row(i+1,i+1,N)(j-i+1) == hvalue,"HermMatrixF.row");
            Assert(h1fx.row(i+1,i+1,N)(j-i+1) == hvalue,
                   "const HermMatrixF.row");
            Assert(h1fcv.row(i+1,i+1,N)(j-i+1) == hvalue,"HermMatrixF.row CV");
            Assert(h1fv.row(i+1,i+1,N)(j-i+1) == hvalue,"HermMatrixF.row V");
            Assert(h1f.col(i+1,i+1,N)(j-i+1) == conj(hvalue),"HermMatrixF.col");
            Assert(h1fx.col(i+1,i+1,N)(j-i+1) == conj(hvalue),
                   "const HermMatrixF.col");
            Assert(h1fcv.col(i+1,i+1,N)(j-i+1) == conj(hvalue),
                   "HermMatrixF.col CV");
            Assert(h1fv.col(i+1,i+1,N)(j-i+1) == conj(hvalue),
                   "HermMatrixF.col V");
            Assert(h1f.row(i+1,j+1,N)(1) == hvalue,"HermMatrixF.row2");
            Assert(h1fx.row(i+1,j+1,N)(1) == hvalue,"const HermMatrixF.row2");
            Assert(h1fcv.row(i+1,j+1,N)(1) == hvalue,"HermMatrixF.row2 CV");
            Assert(h1fv.row(i+1,j+1,N)(1) == hvalue,"HermMatrixF.row2 V");
            Assert(h1f.col(i+1,j+1,N)(1) == conj(hvalue),"HermMatrixF.col2");
            Assert(h1fx.col(i+1,j+1,N)(1) == conj(hvalue),
                   "const HermMatrixF.col2");
            Assert(h1fcv.col(i+1,j+1,N)(1) == conj(hvalue),
                   "HermMatrixF.col2 CV");
            Assert(h1fv.col(i+1,j+1,N)(1) == conj(hvalue),"HermMatrixF.col2 V");
            int d = int(j)-int(i);
            if (d==0) {
                Assert(h1.diag()(i) == hvalue,"HermMatrix.diag");
                Assert(h1x.diag()(i) == hvalue,"const HermMatrix.diag");
                Assert(h1cv.diag()(i) == hvalue,"HermMatrix.diag CV");
                Assert(h1v.diag()(i) == hvalue,"HermMatrix.diag V");
                Assert(h1f.diag()(i+1) == hvalue,"HermMatrixF.diag");
                Assert(h1fx.diag()(i+1) == hvalue,"const HermMatrixF.diag");
                Assert(h1fcv.diag()(i+1) == hvalue,"HermMatrixF.diag CV");
                Assert(h1fv.diag()(i+1) == hvalue,"HermMatrixF.diag V");
            }
            Assert(h1.diag(d)(i) == hvalue,"HermMatrix.diag1");
            Assert(h1x.diag(d)(i) == hvalue,"const HermMatrix.diag1");
            Assert(h1cv.diag(d)(i) == hvalue,"HermMatrix.diag1 CV");
            Assert(h1v.diag(d)(i) == hvalue,"HermMatrix.diag1 V");
            Assert(h1.diag(d,i,N-d)(0) == hvalue,"HermMatrix.diag2");
            Assert(h1x.diag(d,i,N-d)(0) == hvalue,"const HermMatrix.diag2");
            Assert(h1cv.diag(d,i,N-d)(0) == hvalue,"HermMatrix.diag2 CV");
            Assert(h1v.diag(d,i,N-d)(0) == hvalue,"HermMatrix.diag2 V");
            Assert(h1f.diag(d)(i+1) == hvalue,"HermMatrixF.diag1");
            Assert(h1fx.diag(d)(i+1) == hvalue,"const HermMatrixF.diag1");
            Assert(h1fcv.diag(d)(i+1) == hvalue,"HermMatrixF.diag1 CV");
            Assert(h1fv.diag(d)(i+1) == hvalue,"HermMatrixF.diag1 V");
            Assert(h1f.diag(d,i+1,N-d)(1) == hvalue,"HermMatrixF.diag2");
            Assert(h1fx.diag(d,i+1,N-d)(1) == hvalue,"const HermMatrixF.diag2");
            Assert(h1fcv.diag(d,i+1,N-d)(1) == hvalue,"HermMatrixF.diag2 CV");
            Assert(h1fv.diag(d,i+1,N-d)(1) == hvalue,"HermMatrixF.diag2 V");
            Assert(h1.diag(-d)(i) == conj(hvalue),"HermMatrix.diag1");
            Assert(h1x.diag(-d)(i) == conj(hvalue),"const HermMatrix.diag1");
            Assert(h1cv.diag(-d)(i) == conj(hvalue),"HermMatrix.diag1 CV");
            Assert(h1v.diag(-d)(i) == conj(hvalue),"HermMatrix.diag1 V");
            Assert(h1.diag(-d,i,N-d)(0) == conj(hvalue),"HermMatrix.diag2");
            Assert(h1x.diag(-d,i,N-d)(0) == conj(hvalue),
                   "const HermMatrix.diag2");
            Assert(h1cv.diag(-d,i,N-d)(0) == conj(hvalue),
                   "HermMatrix.diag2 CV");
            Assert(h1v.diag(-d,i,N-d)(0) == conj(hvalue),"HermMatrix.diag2 V");
            Assert(h1f.diag(-d)(i+1) == conj(hvalue),"HermMatrixF.diag1");
            Assert(h1fx.diag(-d)(i+1) == conj(hvalue),
                   "const HermMatrixF.diag1");
            Assert(h1fcv.diag(-d)(i+1) == conj(hvalue),"HermMatrixF.diag1 CV");
            Assert(h1fv.diag(-d)(i+1) == conj(hvalue),"HermMatrixF.diag1 V");
            Assert(h1f.diag(-d,i+1,N-d)(1) == conj(hvalue),"HermMatrixF.diag2");
            Assert(h1fx.diag(-d,i+1,N-d)(1) == conj(hvalue),
                   "const HermMatrixF.diag2");
            Assert(h1fcv.diag(-d,i+1,N-d)(1) == conj(hvalue),
                   "HermMatrixF.diag2 CV");
            Assert(h1fv.diag(-d,i+1,N-d)(1) == conj(hvalue),
                   "HermMatrixF.diag2 V");
        }
        if (j<=i) {
            Assert(h2(j,i) == conj(hvalue),"Read/Write HermMatrix");
            Assert(h2x(j,i) == conj(hvalue),"Access const HermMatrix");
            Assert(h2v(j,i) == conj(hvalue),"Access HermMatrix V");
            Assert(h2cv(j,i) == conj(hvalue),"Access HermMatrix CV");
            Assert(h2(i,j) == hvalue,"Access HermMatrix - opposite tri");
            Assert(h2x(i,j) == hvalue,"Access const HermMatrix - opposite tri");
            Assert(h2v(i,j) == hvalue,"Access HermMatrix V");
            Assert(h2cv(i,j) == hvalue,"Access HermMatrix CV");
            Assert(h2f(j+1,i+1) == conj(hvalue),"Read/Write HermMatrixF");
            Assert(h2fx(j+1,i+1) == conj(hvalue),"Access const HermMatrixF");
            Assert(h2fv(j+1,i+1) == conj(hvalue),"Access HermMatrixF V");
            Assert(h2fcv(j+1,i+1) == conj(hvalue),"Access HermMatrixF CV");
            Assert(h2f(i+1,j+1) == hvalue,"Access HermMatrixF - opposite tri");
            Assert(h2fx(i+1,j+1) == hvalue,
                   "Access const HermMatrixF - opposite tri");
            Assert(h2fv(i+1,j+1) == hvalue,"Access HermMatrixF V");
            Assert(h2fcv(i+1,j+1) == hvalue,"Access HermMatrixF CV");
            Assert(h2.row(i,0,i+1)(j) == hvalue,"HermMatrix.row");
            Assert(h2x.row(i,0,i+1)(j) == hvalue,"const HermMatrix.row");
            Assert(h2cv.row(i,0,i+1)(j) == hvalue,"HermMatrix.row CV");
            Assert(h2v.row(i,0,i+1)(j) == hvalue,"HermMatrix.row V");
            Assert(h2.col(i,0,i+1)(j) == conj(hvalue),"HermMatrix.col");
            Assert(h2x.col(i,0,i+1)(j) == conj(hvalue),"const HermMatrix.col");
            Assert(h2cv.col(i,0,i+1)(j) == conj(hvalue),"HermMatrix.col CV");
            Assert(h2v.col(i,0,i+1)(j) == conj(hvalue),"HermMatrix.col V");
            Assert(h2.row(i,j,i+1)(0) == hvalue,"HermMatrix.row2");
            Assert(h2x.row(i,j,i+1)(0) == hvalue,"const HermMatrix.row2");
            Assert(h2cv.row(i,j,i+1)(0) == hvalue,"HermMatrix.row2 CV");
            Assert(h2v.row(i,j,i+1)(0) == hvalue,"HermMatrix.row2 V");
            Assert(h2.col(i,j,i+1)(0) == conj(hvalue),"HermMatrix.col2");
            Assert(h2x.col(i,j,i+1)(0) == conj(hvalue),"const HermMatrix.col2");
            Assert(h2cv.col(i,j,i+1)(0) == conj(hvalue),"HermMatrix.col2 CV");
            Assert(h2v.col(i,j,i+1)(0) == conj(hvalue),"HermMatrix.col2 V");
            Assert(h2f.row(i+1,1,i+1)(j+1) == hvalue,"HermMatrixF.row");
            Assert(h2fx.row(i+1,1,i+1)(j+1) == hvalue,"const HermMatrixF.row");
            Assert(h2fcv.row(i+1,1,i+1)(j+1) == hvalue,"HermMatrixF.row CV");
            Assert(h2fv.row(i+1,1,i+1)(j+1) == hvalue,"HermMatrixF.row V");
            Assert(h2f.col(i+1,1,i+1)(j+1) == conj(hvalue),"HermMatrixF.col");
            Assert(h2fx.col(i+1,1,i+1)(j+1) == conj(hvalue),
                   "const HermMatrixF.col");
            Assert(h2fcv.col(i+1,1,i+1)(j+1) == conj(hvalue),
                   "HermMatrixF.col CV");
            Assert(h2fv.col(i+1,1,i+1)(j+1) == conj(hvalue),
                   "HermMatrixF.col V");
            Assert(h2f.row(i+1,j+1,i+1)(1) == hvalue,"HermMatrixF.row2");
            Assert(h2fx.row(i+1,j+1,i+1)(1) == hvalue,"const HermMatrixF.row2");
            Assert(h2fcv.row(i+1,j+1,i+1)(1) == hvalue,"HermMatrixF.row2 CV");
            Assert(h2fv.row(i+1,j+1,i+1)(1) == hvalue,"HermMatrixF.row2 V");
            Assert(h2f.col(i+1,j+1,i+1)(1) == conj(hvalue),"HermMatrixF.col2");
            Assert(h2fx.col(i+1,j+1,i+1)(1) == conj(hvalue),
                   "const HermMatrixF.col2");
            Assert(h2fcv.col(i+1,j+1,i+1)(1) == conj(hvalue),
                   "HermMatrixF.col2 CV");
            Assert(h2fv.col(i+1,j+1,i+1)(1) == conj(hvalue),
                   "HermMatrixF.col2 V");
            int d = int(j)-int(i);
            if (d==0) {
                Assert(h2.diag()(i) == hvalue,"HermMatrix.diag");
                Assert(h2x.diag()(i) == hvalue,"const HermMatrix.diag");
                Assert(h2cv.diag()(i) == hvalue,"HermMatrix.diag CV");
                Assert(h2v.diag()(i) == hvalue,"HermMatrix.diag V");
                Assert(h2f.diag()(i+1) == hvalue,"HermMatrixF.diag");
                Assert(h2fx.diag()(i+1) == hvalue,"const HermMatrixF.diag");
                Assert(h2fcv.diag()(i+1) == hvalue,"HermMatrixF.diag CV");
                Assert(h2fv.diag()(i+1) == hvalue,"HermMatrixF.diag V");
            }
            Assert(h2.diag(d)(j) == hvalue,"HermMatrix.diag1");
            Assert(h2x.diag(d)(j) == hvalue,"const HermMatrix.diag1");
            Assert(h2cv.diag(d)(j) == hvalue,"HermMatrix.diag1 CV");
            Assert(h2v.diag(d)(j) == hvalue,"HermMatrix.diag1 V");
            Assert(h2.diag(d,j,N+d)(0) == hvalue,"HermMatrix.diag2");
            Assert(h2x.diag(d,j,N+d)(0) == hvalue,"const HermMatrix.diag2");
            Assert(h2cv.diag(d,j,N+d)(0) == hvalue,"HermMatrix.diag2 CV");
            Assert(h2v.diag(d,j,N+d)(0) == hvalue,"HermMatrix.diag2 V");
            Assert(h2f.diag(d)(j+1) == hvalue,"HermMatrixF.diag1");
            Assert(h2fx.diag(d)(j+1) == hvalue,"const HermMatrixF.diag1");
            Assert(h2fcv.diag(d)(j+1) == hvalue,"HermMatrixF.diag1 CV");
            Assert(h2fv.diag(d)(j+1) == hvalue,"HermMatrixF.diag1 V");
            Assert(h2f.diag(d,j+1,N+d)(1) == hvalue,"HermMatrixF.diag2");
            Assert(h2fx.diag(d,j+1,N+d)(1) == hvalue,"const HermMatrixF.diag2");
            Assert(h2fcv.diag(d,j+1,N+d)(1) == hvalue,"HermMatrixF.diag2 CV");
            Assert(h2fv.diag(d,j+1,N+d)(1) == hvalue,"HermMatrixF.diag2 V");
            Assert(h2.diag(-d)(j) == conj(hvalue),"HermMatrix.diag1");
            Assert(h2x.diag(-d)(j) == conj(hvalue),"const HermMatrix.diag1");
            Assert(h2cv.diag(-d)(j) == conj(hvalue),"HermMatrix.diag1 CV");
            Assert(h2v.diag(-d)(j) == conj(hvalue),"HermMatrix.diag1 V");
            Assert(h2.diag(-d,j,N+d)(0) == conj(hvalue),"HermMatrix.diag2");
            Assert(h2x.diag(-d,j,N+d)(0) == conj(hvalue),
                   "const HermMatrix.diag2");
            Assert(h2cv.diag(-d,j,N+d)(0) == conj(hvalue),
                   "HermMatrix.diag2 CV");
            Assert(h2v.diag(-d,j,N+d)(0) == conj(hvalue),"HermMatrix.diag2 V");
            Assert(h2f.diag(-d)(j+1) == conj(hvalue),"HermMatrixF.diag1");
            Assert(h2fx.diag(-d)(j+1) == conj(hvalue),
                   "const HermMatrixF.diag1");
            Assert(h2fcv.diag(-d)(j+1) == conj(hvalue),"HermMatrixF.diag1 CV");
            Assert(h2fv.diag(-d)(j+1) == conj(hvalue),"HermMatrixF.diag1 V");
            Assert(h2f.diag(-d,j+1,N+d)(1) == conj(hvalue),"HermMatrixF.diag2");
            Assert(h2fx.diag(-d,j+1,N+d)(1) == conj(hvalue),
                   "const HermMatrixF.diag2");
            Assert(h2fcv.diag(-d,j+1,N+d)(1) == conj(hvalue),
                   "HermMatrixF.diag2 CV");
            Assert(h2fv.diag(-d,j+1,N+d)(1) == conj(hvalue),
                   "HermMatrixF.diag2 V");
        }
    }

    Assert(h1 == h1f,"CStyle HermMatrix == FortranStyle HermMatrix");
    Assert(h1 == h1cv,"HermMatrix == ConstSymMatrixView");
    Assert(h1 == h1v,"HermMatrix == SymMatrixView");
    Assert(h1 == h1fcv,"HermMatrix == FortranStyle ConstSymMatrixView");
    Assert(h1 == h1fv,"HermMatrix == FortranStyle SymMatrixView");
    Assert(h2 == h2f,"CStyle HermMatrix == FortranStyle HermMatrix");
    Assert(h2 == h2cv,"HermMatrix == ConstSymMatrixView");
    Assert(h2 == h2v,"HermMatrix == SymMatrixView");
    Assert(h2 == h2fcv,"HermMatrix == FortranStyle ConstSymMatrixView");
    Assert(h2 == h2fv,"HermMatrix == FortranStyle SymMatrixView");

    h1.resize(3);
    Assert(h1.colsize() == 3 && h1.rowsize() == 3,
           "HermBandMatrix h1.resize(3)");
    h2.resize(3);
    Assert(h2.colsize() == 3 && h2.rowsize() == 3,
           "HermBandMatrix h2.resize(3)");
    for (int i=0, k=0; i<3; ++i) for (int j=0; j<3; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        std::complex<T> hvalue = i==j ? std::complex<T>(T(k),0) : value;
        if (i <= j) h1(i,j) = hvalue; 
        if (j <= i) h2(i,j) = hvalue; 
    }
    for (int i=0, k=0; i<3; ++i) for (int j=0; j<3; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        std::complex<T> hvalue = i==j ? std::complex<T>(T(k),0) : value;
        if (i<=j) {
            Assert(h1(i,j) == hvalue,"Read/Write resized HermMatrix");
            Assert(h1(j,i) == std::conj(hvalue),
                   "Read/Write resized HermMatrix opp tri");
        }
        if (j<=i) {
            Assert(h2(i,j) == hvalue,"Read/Write resized HermMatrix");
            Assert(h2(j,i) == std::conj(hvalue),
                   "Read/Write resized HermMatrix opp tri");
        }
    }

    h1.resize(2*N);
    Assert(int(h1.colsize()) == 2*N && int(h1.rowsize()) == 2*N,
           "HermMatrix h1.resize(2*N) sizes");
    h2.resize(2*N);
    Assert(int(h2.colsize()) == 2*N && int(h2.rowsize()) == 2*N,
           "HermMatrix h2.resize(2*N) sizes");
    for (int i=0, k=0; i<2*N; ++i) for (int j=0; j<2*N; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        std::complex<T> hvalue = i==j ? std::complex<T>(T(k),0) : value;
        if (i <= j) h1(i,j) = hvalue; 
        if (j <= i) h2(i,j) = hvalue; 
    }
    for (int i=0, k=0; i<2*N; ++i) for (int j=0; j<2*N; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        std::complex<T> hvalue = i==j ? std::complex<T>(T(k),0) : value;
        if (i<=j) {
            Assert(h1(i,j) == hvalue,"Read/Write resized HermMatrix");
            Assert(h1(j,i) == std::conj(hvalue),
                   "Read/Write resized HermMatrix opp tri");
        }
        if (j<=i) {
            Assert(h2(i,j) == hvalue,"Read/Write resized HermMatrix");
            Assert(h2(j,i) == std::conj(hvalue),
                   "Read/Write resized HermMatrix opp tri");
        }
    }
}

template <class T, tmv::UpLoType U, tmv::StorageType S> 
static void TestBasicSymMatrix_2()
{
    const int N = 6;

    tmv::SymMatrix<std::complex<T>,U,S> s1(N);
    tmv::SymMatrix<std::complex<T>,U,S> s2(N);

    for (int i=0, k=1; i<N; ++i) for (int j=0; j<N; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        if (i<=j) {
            s1(i,j) = value; 
        }
        if (j<=i) {
            s2(i,j) = value; 
        }
    }

    std::vector<T> qv(16);
    if ((U == tmv::Upper) == (S == tmv::RowMajor)) {
        const T qvar[] = { T(0), T(-1), T(-2), T(-3),
            T(0), T(1),  T(0), T(-1),
            T(0), T(0),  T(2), T(1),
            T(0), T(0),  T(0), T(3)};
        for(int i=0;i<16;i++) qv[i] = qvar[i];
    } else {
        const T qvar[] = { T(0),  T(0), T(0), T(0),
            T(-1), T(1), T(0), T(0),
            T(-2), T(0), T(2), T(0),
            T(-3), T(-1), T(1), T(3) };
        for(int i=0;i<16;i++) qv[i] = qvar[i];
    }
    T qar[16];
    for(int i=0;i<16;i++) qar[i] = qv[i];

    const tmv::SymMatrix<T,U,S> q1(4,qar);
    const tmv::SymMatrix<T,U,S> q2(4,qv);
    const tmv::ConstSymMatrixView<T> q3 = tmv::SymMatrixViewOf(qar,4,U,S);
    for(int i=0;i<4;i++) for(int j=0;j<4;j++) {
        T v = T(2)*std::min(i,j)-std::max(i,j);
        Assert(q1(i,j) == v,"Create SymMatrix from T*");
        Assert(q2(i,j) == v,"Create SymMatrix from vector");
        Assert(q3(i,j) == v,"Create SymMatrixView of T*");
    }

    tmv::SymMatrix<std::complex<T>,U,S> s3(N);
    s3 = s1+s2;

    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) {
        Assert(s3(i,j) == s1(i,j)+s2(i,j),"Add SymMatrices1");
        Assert(s3(j,i) == s1(i,j)+s2(i,j),"Add SymMatrices2");
    }

    s3 = s1-s2;

    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) 
        if (i<=j) {
            Assert(s3(i,j) == s1(i,j)-s2(i,j),"Subtract SymMatrices1");
            Assert(s3(j,i) == s1(i,j)-s2(i,j),"Subtract SymMatrices2");
        }

    tmv::Matrix<std::complex<T> > m1 = s1;
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) {
        Assert(s1(i,j) == m1(i,j),"SymMatrix -> Matrix");
    }
    tmv::SymMatrix<std::complex<T>,U,S> sm1(m1);
    Assert(s1 == sm1,"Matrix -> SymMatrix1");
    tmv::SymMatrix<std::complex<T>,U,S> sm2 = m1;
    Assert(s1 == sm2,"Matrix -> SymMatrix2");

    tmv::SymMatrix<std::complex<T>,tmv::Upper,tmv::RowMajor> sur = s1;
    Assert(s1==sur,"SymMatrix == SymMatrix<U,R>");
    Assert(s1.view()==sur.view(),"SymMatrix.view == SymMatrix<U,R>.view");
    Assert(s1.transpose()==sur.transpose(),
           "SymMatrix.transpose == SymMatrix<U,R>.transpose");
    Assert(s1.conjugate()==sur.conjugate(),
           "SymMatrix.conjugate == SymMatrix<U,R>.conjugate");
    Assert(s1.adjoint()==sur.adjoint(),
           "SymMatrix.adjoint == SymMatrix<U,R>.adjoint");
    Assert(s1.upperTri()==sur.upperTri(),
           "SymMatrix.upperTri == SymMatrix<U,R>.upperTri");
    Assert(s1.lowerTri()==sur.lowerTri(),
           "SymMatrix.lowerTri == SymMatrix<U,R>.lowerTri");
    Assert(s1.realPart()==sur.realPart(),
           "SymMatrix.real == SymMatrix<U,R>.real");
    Assert(s1.imagPart()==sur.imagPart(),
           "SymMatrix.imag == SymMatrix<U,R>.imag");
    Assert(s1.subMatrix(N/2,N,0,N/2)==sur.subMatrix(N/2,N,0,N/2),
           "SymMatrix.subMatrix1 == SymMatrix<U,R>.subMatrix1");
    Assert(s1.subMatrix(0,N/2,N/2,N)==sur.subMatrix(0,N/2,N/2,N),
           "SymMatrix.subMatrix2 == SymMatrix<U,R>.subMatrix2");
    Assert(s1.subSymMatrix(0,N/2)==sur.subSymMatrix(0,N/2),
           "SymMatrix.subSymMatrix == SymMatrix<U,R>.subSymMatrix");
}

template <class T, tmv::UpLoType U, tmv::StorageType S> 
static void TestBasicHermMatrix_2()
{
    const int N = 6;

    tmv::HermMatrix<std::complex<T>,U,S> h1(N);
    tmv::HermMatrix<std::complex<T>,U,S> h2(N);

    for (int i=0, k=1; i<N; ++i) for (int j=0; j<N; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        std::complex<T> hvalue = i==j ? std::complex<T>(T(k),0) : value;
        if (i<=j) {
            h1(i,j) = hvalue; 
        }
        if (j<=i) {
            h2(i,j) = hvalue; 
        }
    }

    std::vector<T> qv(16);
    if ((U == tmv::Upper) == (S == tmv::RowMajor)) {
        const T qvar[] = { T(0), T(-1), T(-2), T(-3),
            T(0), T(1),  T(0), T(-1),
            T(0), T(0),  T(2), T(1),
            T(0), T(0),  T(0), T(3)};
        for(int i=0;i<16;i++) qv[i] = qvar[i];
    } else {
        const T qvar[] = { T(0),  T(0), T(0), T(0),
            T(-1), T(1), T(0), T(0),
            T(-2), T(0), T(2), T(0),
            T(-3), T(-1), T(1), T(3) };
        for(int i=0;i<16;i++) qv[i] = qvar[i];
    }
    T qar[16];
    for(int i=0;i<16;i++) qar[i] = qv[i];

    const tmv::HermMatrix<T,U,S> q4(4,qar);
    const tmv::HermMatrix<T,U,S> q5(4,qv);
    const tmv::ConstSymMatrixView<T> q6 = tmv::HermMatrixViewOf(qar,4,U,S);
    for(int i=0;i<4;i++) for(int j=0;j<4;j++) {
        T v = T(2)*std::min(i,j)-std::max(i,j);
        Assert(q4(i,j) == v,"Create HermMatrix from T*");
        Assert(q5(i,j) == v,"Create HermMatrix from vector");
        Assert(q6(i,j) == v,"Create HermMatrixView of T*");
    }

    tmv::HermMatrix<std::complex<T>,U,S> h3(N);
    h3 = h1+h2;

    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) {
        Assert(h3(i,j) == h1(i,j)+h2(i,j),"Add HermMatrices1");
        Assert(h3(j,i) == conj(h1(i,j)+h2(i,j)),"Add HermMatrices2");
    }

    h3 = h1-h2;

    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) 
        if (i<=j) {
            Assert(h3(i,j) == h1(i,j)-h2(i,j),"Subtract HermMatrices1");
            Assert(h3(j,i) == conj(h1(i,j)-h2(i,j)),"Subtract HermMatrices2");
        }

    tmv::Matrix<std::complex<T> > n1 = h1;
    for (int i=0; i<N; ++i) for (int j=0; j<N; ++j) {
        Assert(h1(i,j) == n1(i,j),"HermMatrix -> Matrix");
    }
    tmv::HermMatrix<std::complex<T>,U,S> hn1(n1);
    Assert(h1 == hn1,"Matrix -> HermMatrix1");
    tmv::HermMatrix<std::complex<T>,U,S> hn2 = n1;
    Assert(h1 == hn2,"Matrix -> HermMatrix2");

    tmv::HermMatrix<std::complex<T>,tmv::Upper,tmv::RowMajor> hur = h1;
    Assert(h1==hur,"HermMatrix == HermMatrix<U,R>");
    Assert(h1.view()==hur.view(),"HermMatrix.view == HermMatrix<U,R>.view");
    Assert(h1.transpose()==hur.transpose(),
           "HermMatrix.transpose == HermMatrix<U,R>.transpose");
    Assert(h1.conjugate()==hur.conjugate(),
           "HermMatrix.conjugate == HermMatrix<U,R>.conjugate");
    Assert(h1.adjoint()==hur.adjoint(),
           "HermMatrix.adjoint == HermMatrix<U,R>.adjoint");
    Assert(h1.upperTri()==hur.upperTri(),
           "HermMatrix.upperTri == HermMatrix<U,R>.upperTri");
    Assert(h1.lowerTri()==hur.lowerTri(),
           "HermMatrix.lowerTri == HermMatrix<U,R>.lowerTri");
    Assert(h1.realPart()==hur.realPart(),
           "HermMatrix.real == HermMatrix<U,R>.real");
    Assert(h1.subMatrix(N/2,N,0,N/2)==hur.subMatrix(N/2,N,0,N/2),
           "HermMatrix.subMatrix1 == HermMatrix<U,R>.subMatrix1");
    Assert(h1.subMatrix(0,N/2,N/2,N)==hur.subMatrix(0,N/2,N/2,N),
           "HermMatrix.subMatrix2 == HermMatrix<U,R>.subMatrix2");
    Assert(h1.subSymMatrix(0,N/2)==hur.subSymMatrix(0,N/2),
           "HermMatrix.subSymMatrix == HermMatrix<U,R>.subSymMatrix");
}

template <class T, tmv::UpLoType U, tmv::StorageType S> 
static void TestBasicSymMatrix_IO()
{
    const int N = 6;

    tmv::SymMatrix<std::complex<T>,U,S> s1(N);
    tmv::HermMatrix<std::complex<T>,U,S> h1(N);

    for (int i=0, k=1; i<N; ++i) for (int j=0; j<N; ++j, ++k) {
        std::complex<T> value(T(k),T(2*k));
        if (i==j) {
            h1(i,j) = T(k); s1(i,j) = value;
        } else {
            h1(i,j) = s1(i,j) = value;
        }
    }

    std::ofstream fout("tmvtest_symmatrix_io.dat");
    if (!fout) {
#ifdef NOTHROW
        std::cerr<<"Couldn't open tmvtest_symmatrix_io.dat for output\n"; 
        exit(1); 
#else
        throw std::runtime_error(
            "Couldn't open tmvtest_symmatrix_io.dat for output");
#endif
    }
    fout << s1 << std::endl << h1 << std::endl;
    s1.writeCompact(fout);
    h1.writeCompact(fout);
    fout.close();

    tmv::Matrix<std::complex<T>,tmv::RowMajor> xsm1(N,N);
    tmv::Matrix<std::complex<T>,tmv::RowMajor> xhm1(N,N);
    tmv::SymMatrix<std::complex<T>,tmv::Upper,tmv::RowMajor> xs1(N);
    tmv::HermMatrix<std::complex<T>,tmv::Upper,tmv::RowMajor> xh1(N);
    std::ifstream fin("tmvtest_symmatrix_io.dat");
    if (!fin) {
#ifdef NOTHROW
        std::cerr<<"Couldn't open tmvtest_symmatrix_io.dat for input\n"; 
        exit(1); 
#else
        throw std::runtime_error(
            "Couldn't open tmvtest_symmatrix_io.dat for input");
#endif
    }
    fin >> xsm1 >> xhm1;
    fin >> xs1 >> xh1;
    fin.close();
    Assert(tmv::Matrix<std::complex<T> >(s1) == xsm1,"SymMatrix I/O check #1");
    Assert(tmv::Matrix<std::complex<T> >(h1) == xhm1,"HermMatrix I/O check #1");
    Assert(s1 == xs1,"SymMatrix Compact I/O check #1");
    Assert(h1 == xh1,"HermMatrix Compact I/O check #1");

    tmv::Matrix<std::complex<T>,tmv::ColMajor> xsm2(N,N);
    tmv::Matrix<std::complex<T>,tmv::ColMajor> xhm2(N,N);
    tmv::SymMatrix<std::complex<T>,tmv::Upper,tmv::ColMajor> xs2(N);
    tmv::HermMatrix<std::complex<T>,tmv::Upper,tmv::ColMajor> xh2(N);
    fin.open("tmvtest_symmatrix_io.dat");
    if (!fin) {
#ifdef NOTHROW
        std::cerr<<"Couldn't open tmvtest_symmatrix_io.dat for input\n"; 
        exit(1); 
#else
        throw std::runtime_error(
            "Couldn't open tmvtest_symmatrix_io.dat for input");
#endif
    }
    fin >> xsm2 >> xhm2;
    fin >> xs2 >> xh2;
    fin.close();
    Assert(tmv::Matrix<std::complex<T> >(s1) == xsm2,"SymMatrix I/O check #2");
    Assert(tmv::Matrix<std::complex<T> >(h1) == xhm2,"HermMatrix I/O check #2");
    Assert(s1 == xs2,"SymMatrix Compact I/O check #2");
    Assert(h1 == xh2,"HermMatrix Compact I/O check #2");

    tmv::SymMatrix<std::complex<T>,tmv::Lower,tmv::RowMajor> xs3(N);
    tmv::HermMatrix<std::complex<T>,tmv::Lower,tmv::RowMajor> xh3(N);
    fin.open("tmvtest_symmatrix_io.dat");
    if (!fin) {
#ifdef NOTHROW
        std::cerr<<"Couldn't open tmvtest_symmatrix_io.dat for input\n"; 
        exit(1); 
#else
        throw std::runtime_error(
            "Couldn't open tmvtest_symmatrix_io.dat for input");
#endif
    }
    fin >> xsm1 >> xhm1;
    fin >> xs3 >> xh3;
    fin.close();
    Assert(s1 == xs3,"SymMatrix Compact I/O check #3");
    Assert(h1 == xh3,"HermMatrix Compact I/O check #3");

    tmv::SymMatrix<std::complex<T>,tmv::Lower,tmv::ColMajor> xs4(N);
    tmv::HermMatrix<std::complex<T>,tmv::Lower,tmv::ColMajor> xh4(N);
    fin.open("tmvtest_symmatrix_io.dat");
    if (!fin) {
#ifdef NOTHROW
        std::cerr<<"Couldn't open tmvtest_symmatrix_io.dat for input\n"; 
        exit(1); 
#else
        throw std::runtime_error(
            "Couldn't open tmvtest_symmatrix_io.dat for input");
#endif
    }
    fin >> xsm1 >> xhm1;
    fin >> xs4 >> xh4;
    fin.close();
    Assert(s1 == xs4,"SymMatrix Compact I/O check #4");
    Assert(h1 == xh4,"HermMatrix Compact I/O check #4");

    std::auto_ptr<tmv::Matrix<std::complex<T> > > xsm5;
    std::auto_ptr<tmv::Matrix<std::complex<T> > > xhm5;
    std::auto_ptr<tmv::SymMatrix<std::complex<T> > > xs5;
    std::auto_ptr<tmv::HermMatrix<std::complex<T> > > xh5;
    fin.open("tmvtest_symmatrix_io.dat");
    if (!fin) {
#ifdef NOTHROW
        std::cerr<<"Couldn't open tmvtest_symmatrix_io.dat for input\n"; 
        exit(1); 
#else
        throw std::runtime_error(
            "Couldn't open tmvtest_symmatrix_io.dat for input");
#endif
    }
    fin >> xsm5 >> xhm5;
    fin >> xs5 >> xh5;
    fin.close();
    Assert(tmv::Matrix<std::complex<T> >(s1) == *xsm5,"SymMatrix I/O check #5");
    Assert(tmv::Matrix<std::complex<T> >(h1) == *xhm5,"HermMatrix I/O check #5");
    Assert(s1 == *xs5,"SymMatrix Compact I/O check #5");
    Assert(h1 == *xh5,"HermMatrix Compact I/O check #5");

    std::remove("tmvtest_symmatrix_io.dat");
}

template <class T, tmv::UpLoType U, tmv::StorageType S> 
static void TestBasicSymMatrix()
{
    TestBasicSymMatrix_1<T,U,S>();
    TestBasicSymMatrix_2<T,U,S>();
    TestBasicHermMatrix_1<T,U,S>();
    TestBasicHermMatrix_2<T,U,S>();
    TestBasicSymMatrix_IO<T,U,S>();
}

template <class T> void TestSymMatrix() 
{
    TestBasicSymMatrix<T,tmv::Upper,tmv::ColMajor>();
    TestBasicSymMatrix<T,tmv::Lower,tmv::ColMajor>();
#if (XTEST & 2)
    TestBasicSymMatrix<T,tmv::Upper,tmv::RowMajor>();
    TestBasicSymMatrix<T,tmv::Lower,tmv::RowMajor>();
#endif

    std::cout<<"SymMatrix<"<<tmv::TMV_Text(T())<<"> passed all basic tests\n";

    TestSymMatrixArith_A<T>();
    std::cout<<"SymMatrix<"<<tmv::TMV_Text(T())<<
        "> (Sym/Sym) Arithmetic passed all tests\n";
    TestSymMatrixArith_B1<T>();
    TestSymMatrixArith_B2<T>();
    std::cout<<"SymMatrix<"<<tmv::TMV_Text(T())<<
        "> (Matrix/Sym) Arithmetic passed all tests\n";
    TestSymMatrixArith_C1<T>();
    TestSymMatrixArith_C2<T>();
    std::cout<<"SymMatrix<"<<tmv::TMV_Text(T())<<
        "> (Diag/Sym) Arithmetic passed all tests\n";
    TestSymMatrixArith_D1<T>();
    TestSymMatrixArith_D2<T>();
    std::cout<<"SymMatrix<"<<tmv::TMV_Text(T())<<
        "> (Tri/Sym) Arithmetic passed all tests\n";
    TestSymMatrixArith_E1<T>();
    TestSymMatrixArith_E2<T>();
    std::cout<<"SymMatrix<"<<tmv::TMV_Text(T())<<
        "> (Band/Sym) Arithmetic passed all tests\n";
}

#ifdef TEST_DOUBLE
template void TestSymMatrix<double>();
#endif
#ifdef TEST_FLOAT
template void TestSymMatrix<float>();
#endif
#ifdef TEST_LONGDOUBLE
template void TestSymMatrix<long double>();
#endif
#ifdef TEST_INT
template void TestSymMatrix<int>();
#endif
