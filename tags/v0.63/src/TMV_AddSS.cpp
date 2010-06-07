///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// The Template Matrix/Vector Library for C++ was created by Mike Jarvis     //
// Copyright (C) 1998 - 2009                                                 //
//                                                                           //
// The project is hosted at http://sourceforge.net/projects/tmv-cpp/         //
// where you can find the current version and current documention.           //
//                                                                           //
// For concerns or problems with the software, Mike may be contacted at      //
// mike_jarvis@users.sourceforge.net                                         //
//                                                                           //
// This program is free software; you can redistribute it and/or             //
// modify it under the terms of the GNU General Public License               //
// as published by the Free Software Foundation; either version 2            //
// of the License, or (at your option) any later version.                    //
//                                                                           //
// This program is distributed in the hope that it will be useful,           //
// but WITHOUT ANY WARRANTY; without even the implied warranty of            //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             //
// GNU General Public License for more details.                              //
//                                                                           //
// You should have received a copy of the GNU General Public License         //
// along with this program in the file LICENSE.                              //
//                                                                           //
// If not, write to:                                                         //
// The Free Software Foundation, Inc.                                        //
// 51 Franklin Street, Fifth Floor,                                          //
// Boston, MA  02110-1301, USA.                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


//#define XDEBUG


#include "TMV_Blas.h"
#include "tmv/TMV_SymMatrixArithFunc.h"
#include "tmv/TMV_SymMatrix.h"
#include "tmv/TMV_MatrixArith.h"
#include "tmv/TMV_TriMatrixArith.h"
#include "tmv/TMV_SymMatrixArith.h"

#ifdef XDEBUG
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#endif

namespace tmv {

    template <class T, class Ta> 
    void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A, const MatrixView<T>& B)
    {
#ifdef XTEST
        TMVAssert(A.isHermOK());
#endif

#ifdef XDEBUG
        Matrix<Ta> A0 = A;
        Matrix<T> B0 = B;
        Matrix<T> B2 = alpha*A0 + B0;
        //cout<<"Start AddMM: alpha = "<<alpha<<endl;
        //cout<<"A = "<<TMV_Text(A)<<"  "<<A<<endl;
        //cout<<"B = "<<TMV_Text(B)<<"  "<<B0<<endl;
#endif

        TMVAssert(A.size() == B.colsize());
        TMVAssert(A.size() == B.rowsize());
        if (A.size() > 0) {
            if (B.isconj()) {
                AddMM(TMV_CONJ(alpha),A.conjugate(),B.conjugate());
            } else {
                if (SameStorage(A,B)) {
                    if (B.isrm()) {
                        Matrix<Ta,RowMajor> tempA = alpha * A;
                        B += tempA;
                    } else {
                        Matrix<Ta,ColMajor> tempA = alpha * A;
                        B += tempA;
                    }
                }
                else {
                    B.upperTri() += alpha * A.upperTri();
                    if (A.size() > 1) {
                        B.lowerTri().offDiag() += 
                            alpha * A.lowerTri().offDiag();
                    }
                }
            }
        }
#ifdef XDEBUG
        //cout<<"Done\n";
        if (Norm(B-B2) > 0.001*Norm(B)) {
            cerr<<"AddMM: alpha = "<<alpha<<endl;
            cerr<<"A = "<<TMV_Text(A)<<"  "<<A<<endl;
            cerr<<"B = "<<TMV_Text(B)<<"  "<<B0<<endl;
            cerr<<"->B = "<<B<<endl;
            cerr<<"B2 = "<<B2<<endl;
            abort();
        }
#endif
    }


    template <class T, class Ta, class Tb> 
    void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A,
        const T beta, const GenSymMatrix<Tb>& B, const MatrixView<T>& C)
    { 
#ifdef XTEST
        TMVAssert(A.isHermOK());
        TMVAssert(B.isHermOK());
#endif
#ifdef XDEBUG
        Matrix<Ta> A0 = A;
        Matrix<Tb> B0 = B;
        Matrix<T> C2 = alpha*A0 + beta*B0;
        //cout<<"Start AddMM: alpha,beta = "<<alpha<<"  "<<beta<<endl;
        //cout<<"A = "<<TMV_Text(A)<<"  "<<A0<<endl;
        //cout<<"B = "<<TMV_Text(B)<<"  "<<B0<<endl;
#endif

        TMVAssert(A.size() == B.size());
        TMVAssert(C.rowsize() == A.size());
        TMVAssert(C.colsize() == A.size());

        if (A.size() > 0) {
            if (SameStorage(A,C)) {
                if (SameStorage(B,C)) {
                    if (C.isrm()) {
                        Matrix<Ta,RowMajor> tempA = alpha * A;
                        C = beta*B;
                        C += tempA;
                    } else {
                        Matrix<Ta,ColMajor> tempA = alpha * A;
                        C = beta*B;
                        C += tempA;
                    }
                } else {
                    C = alpha*A;
                    AddMM(beta,B,C);
                }
            } else {
                C = beta*B;
                AddMM(alpha,A,C);
            }
        }

#ifdef XDEBUG
        //cout<<"Done\n";
        if (Norm(C-C2) > 0.001*(TMV_ABS(alpha)*Norm(A0) +
                                TMV_ABS(beta)*Norm(B0))) {
            cerr<<"Start AddMM: alpha,beta = "<<alpha<<"  "<<beta<<endl;
            cerr<<"A = "<<TMV_Text(A)<<"  "<<A0<<endl;
            cerr<<"B = "<<TMV_Text(B)<<"  "<<B0<<endl;
            cerr<<"->C = "<<TMV_Text(C)<<"  "<<C<<endl;
            cerr<<"C2 = "<<C2<<endl;
            abort();
        }
#endif
    }

    template <class T, class Ta, class Tb> 
    void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A,
        const T beta, const GenMatrix<Tb>& B, const MatrixView<T>& C)
    { 
#ifdef XTEST
        TMVAssert(A.isHermOK());
#endif
#ifdef XDEBUG
        Matrix<Ta> A0 = A;
        Matrix<Tb> B0 = B;
        Matrix<T> C2 = alpha*A0 + beta*B0;
        //cout<<"Start AddMM: alpha,beta = "<<alpha<<"  "<<beta<<endl;
        //cout<<"A = "<<TMV_Text(A)<<"  "<<A0<<endl;
        //cout<<"B = "<<TMV_Text(B)<<"  "<<B0<<endl;
#endif

        TMVAssert(A.size() == B.colsize());
        TMVAssert(A.size() == B.rowsize());
        TMVAssert(C.rowsize() == A.size());
        TMVAssert(C.colsize() == A.size());

        if (A.size() > 0) {
            if (SameStorage(A,C)) {
                if (SameStorage(B,C)) {
                    if (C.isrm()) {
                        Matrix<Ta,RowMajor> tempA = alpha * A;
                        C = beta * B;
                        C += tempA;
                    } else {
                        Matrix<Ta,ColMajor> tempA = alpha * A;
                        C = beta * B;
                        C += tempA;
                    }
                } else {
                    C = alpha*A;
                    C += beta*B;
                }
            } else {
                C = beta * B;
                AddMM(alpha,A,C);
            }
        }

#ifdef XDEBUG
        //cout<<"Done\n";
        if (Norm(C-C2) > 0.001*(TMV_ABS(alpha)*Norm(A0) +
                                TMV_ABS(beta)*Norm(B0))) {
            cerr<<"AddMM: alpha,beta = "<<alpha<<"  "<<beta<<endl;
            cerr<<"A = "<<TMV_Text(A)<<"  "<<A0<<endl;
            cerr<<"B = "<<TMV_Text(B)<<"  "<<B0<<endl;
            cerr<<"->C = "<<TMV_Text(C)<<"  "<<C<<endl;
            cerr<<"C2 = "<<C2<<endl;
            abort();
        }
#endif
    }

#define InstFile "TMV_AddSS.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv

