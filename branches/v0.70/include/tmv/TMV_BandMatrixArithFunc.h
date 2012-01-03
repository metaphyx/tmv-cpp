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


#ifndef TMV_BandMatrixArithFunc_H
#define TMV_BandMatrixArithFunc_H

#include "tmv/TMV_BaseBandMatrix.h"
#include "tmv/TMV_Array.h"

#define CT std::complex<T>

namespace tmv {

    // y (+)= alpha * A * x
    template <bool add, class T, class Ta, class Tx> 
    void MultMV(
        const T alpha, const GenBandMatrix<Ta>& A,
        const GenVector<Tx>& x, const VectorView<T>& y);

    // A *= alpha
    template <class T> 
    void MultXM(const T alpha, const BandMatrixView<T>& A);

    // B += alpha * A
    template <class T, class Ta> 
    void AddMM(
        const T alpha, const GenBandMatrix<Ta>& A, const BandMatrixView<T>& B);
    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenBandMatrix<Ta>& A, const MatrixView<T>& B)
    { AddMM(alpha,A,BandMatrixView<T>(B,A.nlo(),A.nhi())); }
    // C = alpha * A + beta * B
    template <class T, class Ta, class Tb> 
    void AddMM(
        const T alpha, const GenBandMatrix<Ta>& A,
        const T beta, const GenBandMatrix<Tb>& B, const BandMatrixView<T>& C);
    template <class T, class Ta, class Tb> 
    void AddMM(
        const T alpha, const GenBandMatrix<Ta>& A,
        const T beta, const GenMatrix<Tb>& B, const MatrixView<T>& C);
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenMatrix<Ta>& A,
        const T beta, const GenBandMatrix<Tb>& B, const MatrixView<T>& C)
    { AddMM(beta,B,alpha,A,C); }

    // C (+)= alpha * A * B
    template <bool add, class T, class Ta, class Tb> 
    void MultMM(
        const T alpha, const GenBandMatrix<Ta>& A,
        const GenMatrix<Tb>& B, const MatrixView<T>& C);
    template <bool add, class T, class Ta, class Tb> 
    void MultMM(
        const T alpha, const GenBandMatrix<Ta>& A,
        const GenBandMatrix<Tb>& B, const BandMatrixView<T>& C);
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenMatrix<Ta>& A,
        const GenBandMatrix<Tb>& B, const MatrixView<T>& C)
    { MultMM<add>(alpha,B.transpose(),A.transpose(),C.transpose()); }

    template <bool add, class T, class Ta, class Tb> 
    void ElemMultMM(
        const T alpha, const GenBandMatrix<Ta>& A,
        const GenBandMatrix<Tb>& B, const BandMatrixView<T>& C);

    template <class T> 
    class BandMatrixComposite : public GenBandMatrix<T>
    {
    public:

        inline BandMatrixComposite() : itsm(0) {}
        inline BandMatrixComposite(const BandMatrixComposite<T>&) : itsm(0) {}
        virtual inline ~BandMatrixComposite() {}

        // Definitions are in TMV_MultBV.cpp
        const T* cptr() const;
        int stepi() const;
        int stepj() const;
        int diagstep() const;

        inline ConjType ct() const { return NonConj; }
        inline bool isconj() const { return false; }
        int ls() const;

        ConstVectorView<T> constLinearView() const;
        inline bool canLinearize() const { return true; }

    private:

        mutable AlignedArray<T> itsm1;
        mutable T* itsm;

        BandMatrixComposite<T>& operator=(const BandMatrixComposite<T>&);
    };

    // Specialize allowed complex combinations:
    template <bool add, class T, class Ta, class Tx> 
    inline void MultMV(
        const T alpha, const GenBandMatrix<Ta>& A, const GenVector<Tx>& x, 
        const VectorView<CT>& y)
    { MultMV<add>(CT(alpha),A,x,y); }

    template <class T> 
    inline void MultXM(
        const T x, const BandMatrixView<CT>& m)
    { MultXM(CT(x),m); }

    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenBandMatrix<Ta>& A, const BandMatrixView<CT>& B)
    { AddMM(CT(alpha),A,B); }
    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenBandMatrix<Ta>& A, const MatrixView<CT>& B)
    { AddMM(CT(alpha),A,B); }

    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenBandMatrix<Ta>& A, const T beta,
        const GenBandMatrix<Tb>& B, const BandMatrixView<CT>& C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenBandMatrix<Ta>& A, const T beta,
        const GenMatrix<Tb>& B, const MatrixView<CT>& C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenMatrix<Ta>& A, const T beta,
        const GenBandMatrix<Tb>& B, const MatrixView<CT>& C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T> 
    inline void AddMM(
        const CT alpha, const GenBandMatrix<CT>& A, const CT beta,
        const GenBandMatrix<T>& B, const BandMatrixView<CT>& C)
    { AddMM(beta,B,alpha,A,C); }

    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenBandMatrix<Ta>& A, const GenMatrix<Tb>& B, 
        const MatrixView<CT>& C)
    { MultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenBandMatrix<Ta>& A, const GenBandMatrix<Tb>& B, 
        const BandMatrixView<CT>& C)
    { MultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenMatrix<Ta>& A, const GenBandMatrix<Tb>& B, 
        const MatrixView<CT>& C)
    { MultMM<add>(CT(alpha),A,B,C); }

    template <bool add, class T, class Ta, class Tb> 
    inline void ElemMultMM(
        const T alpha, const GenBandMatrix<Ta>& A, const GenBandMatrix<Tb>& B,
        const BandMatrixView<CT>& C)
    { ElemMultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T> 
    inline void ElemMultMM(
        const CT alpha, const GenBandMatrix<CT>& A, const GenBandMatrix<T>& B,
        const BandMatrixView<CT>& C)
    { ElemMultMM<add>(alpha,B,A,C); }

    // Specialize disallowed complex combinations:
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMV(
        const CT , const GenBandMatrix<Ta>& ,
        const GenVector<Tb>& , const VectorView<T>& )
    { TMVAssert(TMV_FALSE); }

    template <class T> 
    inline void MultXM(const CT , const BandMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }

    template <class T, class Ta> 
    inline void AddMM(
        const CT , const GenBandMatrix<Ta>& , const BandMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta> 
    inline void AddMM(
        const CT , const GenBandMatrix<Ta>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenBandMatrix<Ta>& ,
        const CT , const GenBandMatrix<Tb>& , const BandMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenBandMatrix<Ta>& ,
        const CT , const GenMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenMatrix<Ta>& ,
        const CT , const GenBandMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }

    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenBandMatrix<Ta>& ,
        const GenMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenBandMatrix<Ta>& ,
        const GenBandMatrix<Tb>& , const BandMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenMatrix<Ta>& ,
        const GenBandMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }

    template <bool add, class T, class Ta, class Tb> 
    inline void ElemMultMM(
        const CT , const GenBandMatrix<Ta>& ,
        const GenBandMatrix<Tb>& , const BandMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }

}

#undef CT

#endif
