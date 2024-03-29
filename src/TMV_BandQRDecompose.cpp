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


#include "TMV_BandQRDiv.h"
#include "tmv/TMV_BandQRD.h"
#include "tmv/TMV_BandMatrix.h"
#include "TMV_Householder.h"

#ifdef XDEBUG
#include "tmv/TMV_MatrixArith.h"
#include "tmv/TMV_BandMatrixArith.h"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#endif

namespace tmv {

    template <class T> 
    void QR_Decompose(
        const BandMatrixView<T>& QRx,
        const VectorView<T>& Qbeta, T& det)
    {
        // Decompose A (input as QRx) into A = Q R 
        // where Q is unitary, and R is upper triangular
        // Q and R are stored in the same matrix (QRx)
        TMVAssert(QRx.colsize() >= QRx.rowsize());
        TMVAssert(Qbeta.size() == QRx.rowsize());
        TMVAssert(!Qbeta.isconj());
        TMVAssert(Qbeta.step() == 1);

#ifdef XDEBUG
        cout<<"Start Band QR_Decompose\n";
        cout<<"QRx = "<<TMV_Text(QRx)<<endl;
        cout<<"= "<<QRx<<endl;
        cout<<"M N lo hi = "<<QRx.colsize()<<"  "<<QRx.rowsize()<<"  "<<QRx.nlo()<<"  "<<QRx.nhi()<<endl;
        Matrix<T> A0(QRx);
#endif
        const ptrdiff_t M = QRx.colsize();
        const ptrdiff_t N = QRx.rowsize();
        if (QRx.nlo() == 0) {
            Qbeta.setZero();
        } else {
            ptrdiff_t endcol = QRx.nlo()+1;
            ptrdiff_t endrow = QRx.nhi()+1;
            T* Qbj = Qbeta.ptr();
            for(ptrdiff_t j=0;j<N;++j,++Qbj) {
                // Apply the Householder Reflection for this column
#ifdef TMVFLDEBUG
                TMVAssert(Qbj >= Qbeta.first);
                TMVAssert(Qbj < Qbeta.last);
#endif
                *Qbj = HouseholderReflect(QRx.subMatrix(j,endcol,j,endrow),det);
                if (endcol < M) ++endcol;
                if (endrow < N) ++endrow;
            }
        }
#ifdef XDEBUG
        cout<<"Done Band QR_Decompose\n";
        cout<<"QRx => "<<QRx<<endl;
        cout<<"beta => "<<Qbeta<<endl;
        Matrix<T> Q(M,N);
        Q = QRx;
        GetQFromBandQR(Q.view(),Qbeta,QRx.nlo());
        cout<<"Q = "<<Q<<endl;
        Matrix<T> R(QRx.diagRange(0,QRx.nhi()+1));
        cout<<"R = "<<R<<endl;
        Matrix<T> QR = Q*R;
        cout<<"QR = "<<QR<<endl;
        cout<<"Norm(QR-A0) = "<<Norm(QR-A0)<<endl;
        cout<<"Norm(AtA-RtR) = "<<Norm(A0.adjoint()*A0-R.adjoint()*R)<<endl;
        if (!(Norm(QR-A0) < 0.00001*Norm(A0))) {
            cerr<<"QR_Decompose: \n";
            cerr<<"A = "<<TMV_Text(QRx)<<"  "<<A0<<endl;
            cerr<<"QRx = "<<QRx<<endl;
            cerr<<"Q = "<<Q<<endl;
            cerr<<"R = "<<R<<endl;
            cerr<<"QR = "<<QR<<endl;
            abort();
        }
#endif
    }

    template <class T> 
    void QR_Decompose(
        const GenBandMatrix<T>& A,
        const MatrixView<T>& Q, const BandMatrixView<T>& R)
    {
        // Decompose A = Q R 
        // where Q is unitary and R is upper banded

        TMVAssert(A.colsize() >= A.rowsize());
        TMVAssert(Q.colsize() == A.colsize());
        TMVAssert(Q.rowsize() == A.rowsize());
        TMVAssert(R.colsize() == A.rowsize());
        TMVAssert(R.rowsize() == A.rowsize());
        TMVAssert(R.nlo() >= 0);
        TMVAssert(R.nhi() == R.rowsize()-1 || 
                  R.nhi() >= A.nlo() + A.nhi());

        if (Q.isconj()) {
            QR_Decompose(A.conjugate(),Q.conjugate(),R.conjugate());
        } else {
            Vector<T> beta(A.rowsize());
            T d(0);
            Q.setZero();
            BandMatrixView<T>(Q,A.nlo(),A.nhi()) = A;
            ptrdiff_t newnhi = TMV_MIN(A.nlo()+A.nhi(),A.rowsize()-1);
            QR_Decompose(BandMatrixViewOf(Q,A.nlo(),newnhi),
                        beta.view(),d);
            R = BandMatrixViewOf(Q,0,newnhi);
            GetQFromBandQR(Q,beta.view(),A.nlo());
        }
#ifdef XDEBUG
        cout<<"Norm(AtA-RtR) = "<<Norm(A.adjoint()*A-R.adjoint()*R)<<endl;
#endif
    }

    template <class T> 
    void QR_Decompose(
        const GenBandMatrix<T>& A, const BandMatrixView<T>& R)
    {
        // Decompose A = Q R 
        // where Q is unitary and R is upper banded

        TMVAssert(A.colsize() >= A.rowsize());
        TMVAssert(R.colsize() == A.rowsize());
        TMVAssert(R.rowsize() == A.rowsize());
        TMVAssert(R.nlo() >= 0);
        TMVAssert(R.nhi() == R.rowsize()-1 || R.nhi() >= A.nlo() + A.nhi());

        Vector<T> beta(A.rowsize());
        T d(0);
        ptrdiff_t newnhi = TMV_MIN(A.nlo()+A.nhi(),A.rowsize()-1);
        BandMatrix<T> QR(
            TMV_MIN(A.colsize(),A.rowsize()+A.nlo()),A.rowsize(),
            A.nlo(),newnhi,T(0));
        QR.setZero();
        BandMatrixViewOf(QR,A.nlo(),A.nhi()) = A.rowRange(0,QR.colsize());
        QR_Decompose(QR.view(),beta.view(),d);
        R = BandMatrixViewOf(QR,0,newnhi);
#ifdef XDEBUG
        cout<<"Norm(AtA-RtR) = "<<Norm(A.adjoint()*A-R.adjoint()*R)<<endl;
#endif
    }

#ifdef INST_INT
#undef INST_INT
#endif

#define InstFile "TMV_BandQRDecompose.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv


