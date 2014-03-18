///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// The Template Matrix/Vector Library for C++ was created by Mike Jarvis     //
// Copyright (C) 1998 - 2014                                                 //
// All rights reserved                                                       //
//                                                                           //
// The project is hosted at https://code.google.com/p/tmv-cpp/               //
// where you can find the current version and current documention.           //
//                                                                           //
// For concerns or problems with the software, Mike may be contacted at      //
// mike_jarvis17 [at] gmail.                                                 //
//                                                                           //
// This software is licensed under a FreeBSD license.  The file              //
// TMV_LICENSE should have bee included with this distribution.              //
// It not, you can get a copy from https://code.google.com/p/tmv-cpp/.       //
//                                                                           //
// Essentially, you can use this software however you want provided that     //
// you include the TMV_LICENSE file in any distribution that uses it.        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#ifndef TMV_SVDiv_H
#define TMV_SVDiv_H

#include "tmv/TMV_BaseMatrix.h"
#include "tmv/TMV_BaseDiagMatrix.h"

namespace tmv {

#define RT TMV_RealType(T)

    template <class T> 
    void SV_Decompose(
        MatrixView<T> U, DiagMatrixView<RT> S, 
        MatrixView<T> Vt, RT& logdet, T& signdet, bool StoreU=false);

    template <class T, class Tm, class Tx> 
    void SV_LDiv(
        const GenMatrix<T>& U, const GenDiagMatrix<RT>& S,
        const GenMatrix<T>& Vt, ptrdiff_t kmax,
        const GenMatrix<Tm>& m, MatrixView<Tx> x);
    template <class T, class Tm, class Tx> 
    void SV_RDiv(
        const GenMatrix<T>& U, const GenDiagMatrix<RT>& S,
        const GenMatrix<T>& Vt, ptrdiff_t kmax,
        const GenMatrix<Tm>& m, MatrixView<Tx> x);


    template <class T> 
    void Bidiagonalize(
        MatrixView<T> A, VectorView<T> Ubeta,
        VectorView<T> Vtbeta, VectorView<RT> D, VectorView<RT> E, T& signdet);

    template <class T> 
    void BidiagonalChopSmallElements(
        VectorView<T> D, VectorView<T> E, bool* zd=0);

    template <class T> 
    void BidiagonalZeroFirstRow(
        MatrixView<T> U, VectorView<RT> D, VectorView<RT> E);

    template <class T> 
    void BidiagonalZeroLastCol(
        VectorView<RT> D, VectorView<RT> E, MatrixView<T> Vt);

    template <class T> 
    void SV_DecomposeFromBidiagonal(
        MatrixView<T> U, VectorView<RT> D, VectorView<RT> E, MatrixView<T> Vt,
        bool SetUV=false);

    template <class T> 
    void DoSVDecomposeFromBidiagonal(
        MatrixView<T> U, VectorView<RT> D, VectorView<RT> E, MatrixView<T> Vt,
        bool UisI, bool VisI);

    template <class T> 
    void SV_DecomposeFromBidiagonal_QR(
        MatrixView<T> U, VectorView<RT> D, VectorView<RT> E, MatrixView<T> Vt);

    template <class T> 
    void SV_DecomposeFromBidiagonal_DC(
        MatrixView<T> U, VectorView<RT> D, VectorView<RT> E, MatrixView<T> Vt,
        bool UisI, bool VisI);

    template <class T> 
    void FindDCSingularValues(
        Vector<T>& S, const T rho, const GenVector<T>& D, const GenVector<T>& z,
        Matrix<T,ColMajor>& diff);

    template <class T> 
    void FindDCSingularValues(
        Vector<T>& S, const T rho, const GenVector<T>& D, const GenVector<T>& z);


#define CTx std::complex<Tx>

    // Some compilers (specifically MS Visual C++ at least) need this
    // dummy variable in the front to be able to resolve the overloads
    // between the allowed versions and the below disallowed versions.
    template <class T, class Tm, class Tx>
    inline void CallSV_LDiv(
        Tx , const GenMatrix<T>& U, const GenDiagMatrix<RT>& S,
        const GenMatrix<T>& Vt, ptrdiff_t kmax,
        const GenMatrix<Tm>& m, MatrixView<Tx> x)
    { SV_LDiv(U,S,Vt,kmax,m,x); }

    template <class T, class Tm, class Tx>
    inline void CallSV_LDiv(
        Tx , const GenMatrix<T>& U, const GenDiagMatrix<RT>& S,
        const GenMatrix<T>& Vt, ptrdiff_t kmax,
        const GenMatrix<Tm>& m, MatrixView<CTx> x)
    { SV_LDiv(U,S,Vt,kmax,m,x); }
   
    template <class T, class Tm, class Tx>
    inline void CallSV_RDiv(
        Tx , const GenMatrix<T>& U, const GenDiagMatrix<RT>& S,
        const GenMatrix<T>& Vt, ptrdiff_t kmax,
        const GenMatrix<Tm>& m, MatrixView<Tx> x)
    { SV_RDiv(U,S,Vt,kmax,m,x); }
   
    template <class T, class Tm, class Tx>
    inline void CallSV_RDiv(
        Tx , const GenMatrix<T>& U, const GenDiagMatrix<RT>& S,
        const GenMatrix<T>& Vt, ptrdiff_t kmax,
        const GenMatrix<Tm>& m, MatrixView<CTx> x)
    { SV_RDiv(U,S,Vt,kmax,m,x); }
   
#undef CTx
#undef RT

    // Specialize disallowed complex combinations:
#define CT std::complex<T>

    template <class T>
    inline void CallSV_LDiv(
        CT , const GenMatrix<CT>& , const GenDiagMatrix<T>& ,
        const GenMatrix<CT>& , ptrdiff_t ,
        const GenMatrix<CT>& , MatrixView<T> )
    { TMVAssert(TMV_FALSE); }
    template <class T>
    inline void CallSV_LDiv(
        CT , const GenMatrix<CT>& , const GenDiagMatrix<T>& ,
        const GenMatrix<CT>& , ptrdiff_t ,
        const GenMatrix<T>& , MatrixView<T> )
    { TMVAssert(TMV_FALSE); }
    template <class T>
    inline void CallSV_LDiv(
        CT , const GenMatrix<T>& , const GenDiagMatrix<T>& ,
        const GenMatrix<T>& , ptrdiff_t ,
        const GenMatrix<CT>& , MatrixView<T> )
    { TMVAssert(TMV_FALSE); }
    template <class T>
    inline void CallSV_RDiv(
        CT , const GenMatrix<CT>& , const GenDiagMatrix<T>& ,
        const GenMatrix<CT>& , ptrdiff_t ,
        const GenMatrix<CT>& , MatrixView<T> )
    { TMVAssert(TMV_FALSE); }
    template <class T>
    inline void CallSV_RDiv(
        CT , const GenMatrix<CT>& , const GenDiagMatrix<T>& ,
        const GenMatrix<CT>& , ptrdiff_t ,
        const GenMatrix<T>& , MatrixView<T> )
    { TMVAssert(TMV_FALSE); }
    template <class T>
    inline void CallSV_RDiv(
        CT , const GenMatrix<T>& , const GenDiagMatrix<T>& ,
        const GenMatrix<T>& , ptrdiff_t ,
        const GenMatrix<CT>& , MatrixView<T> )
    { TMVAssert(TMV_FALSE); }

#undef CT

} // namespace mv

#endif