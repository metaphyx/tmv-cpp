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



#include "tmv/TMV_QRD.h"
#include "TMV_QRDiv.h"
#include "tmv/TMV_Matrix.h"
#include "tmv/TMV_TriMatrix.h"
#include "tmv/TMV_DiagMatrix.h"
#include "tmv/TMV_TriMatrixArith.h"
#include "tmv/TMV_MatrixArith.h"
#include "tmv/TMV_PackedQ.h"
#include <ostream>

namespace tmv {

    template <class T> 
    struct QRDiv<T>::QRDiv_Impl
    {
    public :
        QRDiv_Impl(const GenMatrix<T>& A, bool inplace);

        const bool istrans;
        const bool inplace;
        AlignedArray<T> Aptr1;
        T* Aptr;
        MatrixView<T> QRx;
        Vector<T> beta;
        mutable TMV_RealType(T) logdet;
        mutable T signdet;
        mutable bool donedet;
    };

#define APTR1 (inplace ? 0 : (A.colsize()*A.rowsize()))
#define APTR (inplace ? A.nonConst().ptr() : Aptr1.get())
#define QRX (istrans ? \
             (inplace ? A.nonConst().transpose() : \
              MatrixViewOf(Aptr,A.rowsize(),A.colsize(),ColMajor)) : \
             (inplace ? A.nonConst().view() : \
              MatrixViewOf(Aptr,A.colsize(),A.rowsize(),ColMajor)))

    template <class T> 
    QRDiv<T>::QRDiv_Impl::QRDiv_Impl(const GenMatrix<T>& A, bool _inplace) :
        istrans(A.colsize()<A.rowsize()),
        inplace(_inplace && (A.iscm() || A.isrm())), 
        Aptr1(APTR1), Aptr(APTR), QRx(QRX), 
        beta(QRx.rowsize()), logdet(0), signdet(1), donedet(false) {}

#undef QRX
#undef APTR

    template <class T> 
    QRDiv<T>::QRDiv(const GenMatrix<T>& A, bool inplace) :
        pimpl(new QRDiv_Impl(A,inplace))
    {
        //std::cout<<"Start QRDiv Constructor"<<std::endl;
        if (pimpl->istrans) {
            if (inplace) TMVAssert(A.transpose() == pimpl->QRx);
            else pimpl->QRx = A.transpose();
        }
        else {
            if (inplace) TMVAssert(A == pimpl->QRx); 
            else pimpl->QRx = A;
        }
        //std::cout<<"Before QR_Decompose"<<std::endl;
        QR_Decompose(pimpl->QRx,pimpl->beta.view(),pimpl->signdet);
        //std::cout<<"End QRDiv Constructor"<<std::endl;
    }

    template <class T> QRDiv<T>::~QRDiv() {}

    template <class T> template <class T1> 
    void QRDiv<T>::doLDivEq(const MatrixView<T1>& m) const
    {
        if (pimpl->istrans) 
            QR_LDivEq(pimpl->QRx,pimpl->beta,0,m.transpose(),
                     pimpl->QRx.rowsize());
        else 
            QR_LDivEq(pimpl->QRx,pimpl->beta,0,m,pimpl->QRx.rowsize());
    }

    template <class T> template <class T1> 
    void QRDiv<T>::doRDivEq(const MatrixView<T1>& m) const
    {
        if (pimpl->istrans) 
            QR_RDivEq(pimpl->QRx,pimpl->beta,0,m.transpose(),
                     pimpl->QRx.rowsize());
        else 
            QR_RDivEq(pimpl->QRx,pimpl->beta,0,m,pimpl->QRx.rowsize());
    }

    template <class T> template <class T1, class T2> 
    void QRDiv<T>::doLDiv(const GenMatrix<T1>& m, const MatrixView<T2>& x) const
    {
        TMVAssert(m.colsize() == (pimpl->istrans ?  pimpl->QRx.rowsize() :
                                  pimpl->QRx.colsize()));
        TMVAssert(x.colsize() == (pimpl->istrans ?  pimpl->QRx.colsize() :
                                  pimpl->QRx.rowsize()));
        if (pimpl->istrans) 
            QR_RDiv(pimpl->QRx,pimpl->beta,0,m.transpose(),x.transpose(),
                    pimpl->QRx.rowsize());
        else QR_LDiv(pimpl->QRx,pimpl->beta,0,m,x,pimpl->QRx.rowsize());
    }

    template <class T> template <class T1, class T2> 
    void QRDiv<T>::doRDiv(const GenMatrix<T1>& m, const MatrixView<T2>& x) const
    {
        TMVAssert(m.colsize() == x.colsize());
        TMVAssert(m.rowsize() == (pimpl->istrans ? pimpl->QRx.colsize() :
                                  pimpl->QRx.rowsize()));
        TMVAssert(x.rowsize() == (pimpl->istrans ? pimpl->QRx.rowsize() :
                                  pimpl->QRx.colsize()));
        if (pimpl->istrans) 
            QR_LDiv(pimpl->QRx,pimpl->beta,0,m.transpose(),x.transpose(),
                    pimpl->QRx.rowsize());
        else 
            QR_RDiv(pimpl->QRx,pimpl->beta,0,m,x,pimpl->QRx.rowsize());
    }

    template <class T> 
    T QRDiv<T>::det() const
    {
        if (!pimpl->donedet) {
            T s;
            pimpl->logdet = DiagMatrixViewOf(pimpl->QRx.diag()).logDet(&s);
            pimpl->signdet *= s;
            pimpl->donedet = true;
        }         
        if (pimpl->signdet == T(0)) return T(0);
        else return pimpl->signdet * TMV_EXP(pimpl->logdet);  
    }                  

    template <class T> 
    TMV_RealType(T) QRDiv<T>::logDet(T* sign) const
    {
        if (!pimpl->donedet) {
            T s;
            pimpl->logdet = DiagMatrixViewOf(pimpl->QRx.diag()).logDet(&s);
            pimpl->signdet *= s;
            pimpl->donedet = true;
        }
        if (sign) *sign = pimpl->signdet;
        return pimpl->logdet;  
    }                  
    template <class T> template <class T1> 
    void QRDiv<T>::doMakeInverse(const MatrixView<T1>& minv) const
    {
        //std::cout<<"Start QRDiv doMakeInverse"<<std::endl;
        if (pimpl->istrans)
            QR_Inverse(pimpl->QRx,pimpl->beta,minv.transpose()); 
        else
            QR_Inverse(pimpl->QRx,pimpl->beta,minv); 
        //std::cout<<"End QRDiv doMakeInverse"<<std::endl;
    }

    template <class T> 
    void QRDiv<T>::doMakeInverseATA(const MatrixView<T>& ata) const
    {
        // At A = Rt Qt Q R = Rt R
        // (At A)^-1 = (Rt R)^-1 = R^-1 * Rt^-1

        UpperTriMatrixView<T> rinv = ata.upperTri();
        rinv = pimpl->QRx.upperTri().inverse();
        ata = rinv * rinv.adjoint();
    }

    template <class T> 
    bool QRDiv<T>::isSingular() const 
    { return det() == T(0); }

    template <class T> 
    bool QRDiv<T>::isTrans() const 
    { return pimpl->istrans; }

    template <class T> 
    PackedQ<T> QRDiv<T>::getQ() const
    { return PackedQ<T>(pimpl->QRx,pimpl->beta); }

    template <class T> 
    ConstUpperTriMatrixView<T> QRDiv<T>::getR() const
    { return pimpl->QRx.upperTri(); }

    template <class T> 
    const GenMatrix<T>& QRDiv<T>::getQR() const 
    { return pimpl->QRx; }

    template <class T> const GenVector<T>& QRDiv<T>::getBeta() const 
    { return pimpl->beta; }

    template <class T> 
    bool QRDiv<T>::checkDecomp(const BaseMatrix<T>& m, std::ostream* fout) const
    {
        Matrix<T> mm = m;
        bool printmat = fout && m.colsize() < 100 && m.rowsize() < 100;
        if (printmat) {
            *fout << "QRDiv:\n";
            *fout << "M = "<<
                (pimpl->istrans?mm.transpose():mm.view())<<std::endl;
            *fout << "Q = "<<getQ()<<std::endl;
            *fout << "R = "<<getR()<<std::endl;
        }
        Matrix<T> qr = getQ()*getR();
        TMV_RealType(T) nm = 
            Norm(qr-(pimpl->istrans ? mm.transpose() : mm.view()));
        nm /= Norm(getQ())*Norm(getR());
        if (printmat) {
            *fout << "QR = "<<qr<<std::endl;
        }
        if (fout) {
            *fout << "Norm(M-QR)/Norm(QR) = "<<nm<<"  ";
            *fout << pimpl->QRx.rowsize()*TMV_Epsilon<T>()<<std::endl;
        }
        return nm < mm.doCondition()*mm.colsize()*TMV_Epsilon<T>();
    }

    template <class T> 
    size_t QRDiv<T>::colsize() const
    { return pimpl->istrans ? pimpl->QRx.rowsize() : pimpl->QRx.colsize(); }

    template <class T> 
    size_t QRDiv<T>::rowsize() const
    { return pimpl->istrans ? pimpl->QRx.colsize() : pimpl->QRx.rowsize(); }

#define InstFile "TMV_QRD.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv


