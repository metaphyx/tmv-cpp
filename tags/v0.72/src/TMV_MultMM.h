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

#ifndef TMV_MultMM_H
#define TMV_MultMM_H

#include "tmv/TMV_Matrix.h"

namespace tmv {

    template <bool add, class T, class Ta, class Tb> 
    void CCCMultMM(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, MatrixView<T> C);

    template <bool add, class T, class Ta, class Tb> 
    void CRCMultMM(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, MatrixView<T> C);

    template <bool add, class T, class Ta, class Tb> 
    void RCCMultMM(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, MatrixView<T> C);

    template <bool add, class T, class Ta, class Tb> 
    void OpenMPMultMM(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, MatrixView<T> C);

    template <bool add, class T, class Ta, class Tb> 
    void BlockMultMM(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, MatrixView<T> C);

    template <bool add, class T, class Ta, class Tb> 
    void RecursiveBlockMultMM(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, MatrixView<T> C);


}

#endif