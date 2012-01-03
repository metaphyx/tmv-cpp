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


#ifndef TMV_TriMatrixArithFunc_H
#define TMV_TriMatrixArithFunc_H

#include "tmv/TMV_BaseTriMatrix.h"
#include "tmv/TMV_Array.h"

#define CT std::complex<T>

namespace tmv {

    // y (+)= alpha * A * x
    template <bool add, class T, class Ta, class Tx> 
    void MultMV(
        const T alpha, const GenUpperTriMatrix<Ta>& A, 
        const GenVector<Tx>& x, const VectorView<T>& y);
    template <bool add, class T, class Ta, class Tx> 
    void MultMV(
        const T alpha, const GenLowerTriMatrix<Ta>& A, 
        const GenVector<Tx>& x, const VectorView<T>& y);

    // A = alpha * A
    template <class T> 
    void MultXM(const T alpha, const UpperTriMatrixView<T>& A);
    template <class T> 
    inline void MultXM(const T alpha, const LowerTriMatrixView<T>& A)
    { MultXM(alpha,A.transpose()); }

    // B += alpha * A
    template <class T, class Ta> 
    void AddMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A,
        const UpperTriMatrixView<T>& B);
    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A,
        const LowerTriMatrixView<T>& B)
    { AddMM(alpha,A.transpose(),B.transpose()); }
    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A,
        const MatrixView<T>& B)
    { AddMM(alpha,A,B.upperTri()); }
    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A,
        const MatrixView<T>& B)
    { AddMM(alpha,A.transpose(),B.lowerTri().transpose()); }
    // C = alpha * A + beta * B
    template <class T, class Ta, class Tb> 
    void AddMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A,
        const T beta, const GenUpperTriMatrix<Tb>& B, 
        const UpperTriMatrixView<T>& C);
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A,
        const T beta, const GenLowerTriMatrix<Tb>& B, 
        const LowerTriMatrixView<T>& C)
    { AddMM(alpha,A.transpose(),beta,B.transpose(),C.transpose()); }
    template <class T, class Ta, class Tb> 
    void AddMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A,
        const T beta, const GenMatrix<Tb>& B, const MatrixView<T>& C);
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenMatrix<Ta>& A,
        const T beta, const GenUpperTriMatrix<Tb>& B, const MatrixView<T>& C)
    { AddMM(beta,B,alpha,A,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A,
        const T beta, const GenMatrix<Tb>& B, const MatrixView<T>& C)
    { AddMM(alpha,A.transpose(),beta,B.transpose(),C.transpose()); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenMatrix<Ta>& A,
        const T beta, const GenLowerTriMatrix<Tb>& B, const MatrixView<T>& C)
    { AddMM(beta,B.transpose(),alpha,A.transpose(),C.transpose()); }
    template <class T, class Ta, class Tb> 
    void AddMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A,
        const T beta, const GenLowerTriMatrix<Tb>& B, const MatrixView<T>& C);
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A,
        const T beta, const GenUpperTriMatrix<Tb>& B, const MatrixView<T>& C)
    { AddMM(beta,B,alpha,A,C); }

    // C (+)= alpha * A * B
    template <bool add, class T, class Ta, class Tb> 
    void MultMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A, 
        const GenMatrix<Tb>& B, const MatrixView<T>& C);
    template <bool add, class T, class Ta, class Tb> 
    void MultMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A, 
        const GenMatrix<Tb>& B, const MatrixView<T>& C);
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenMatrix<Ta>& A, 
        const GenUpperTriMatrix<Tb>& B, const MatrixView<T>& C)
    { MultMM<add>(alpha,B.transpose(),A.transpose(),C.transpose()); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenMatrix<Ta>& A, 
        const GenLowerTriMatrix<Tb>& B, const MatrixView<T>& C)
    { MultMM<add>(alpha,B.transpose(),A.transpose(),C.transpose()); }

    template <bool add, class T, class Ta, class Tb> 
    void MultMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A, 
        const GenUpperTriMatrix<Tb>& B, const UpperTriMatrixView<T>& C);
    template <bool add, class T, class Ta, class Tb> 
    void MultMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A, 
        const GenLowerTriMatrix<Tb>& B, const MatrixView<T>& C);
    template <bool add, class T, class Ta, class Tb> 
    void MultMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A, 
        const GenUpperTriMatrix<Tb>& B, const MatrixView<T>& C);
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A, 
        const GenLowerTriMatrix<Tb>& B, const LowerTriMatrixView<T>& C)
    { MultMM<add>(alpha,B.transpose(),A.transpose(),C.transpose()); }

    template <bool add, class T, class Ta, class Tb> 
    void ElemMultMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A,
        const GenUpperTriMatrix<Tb>& B, const UpperTriMatrixView<T>& C);
    template <bool add, class T, class Ta, class Tb> 
    inline void ElemMultMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A,
        const GenLowerTriMatrix<Tb>& B, const LowerTriMatrixView<T>& C)
    { ElemMultMM<add>(alpha,A.transpose(),B.transpose(),C.transpose()); }

    template <class T> 
    class UpperTriMatrixComposite : public GenUpperTriMatrix<T>
    {
    public:

        inline UpperTriMatrixComposite() {}
        inline UpperTriMatrixComposite(const UpperTriMatrixComposite<T>&) {}
        virtual inline ~UpperTriMatrixComposite() {}
        const T* cptr() const;
        int stepi() const;
        int stepj() const;
        inline ConjType ct() const { return NonConj; }

    private:
        mutable AlignedArray<T> itsm;
    };

    template <class T> 
    class LowerTriMatrixComposite : public GenLowerTriMatrix<T>
    {
    public:

        inline LowerTriMatrixComposite() {}
        inline LowerTriMatrixComposite(const LowerTriMatrixComposite<T>&) {}
        virtual inline ~LowerTriMatrixComposite() {}
        const T* cptr() const;
        int stepi() const;
        int stepj() const;
        inline ConjType ct() const { return NonConj; }

    private:
        mutable AlignedArray<T> itsm;
    };

    // Specialize allowed complex combinations:
    template <bool add, class T, class Ta, class Tx> 
    inline void MultMV(
        const T alpha, const GenUpperTriMatrix<Ta>& A, 
        const GenVector<Tx>& x, const VectorView<CT>& y)
    { MultMV<add>(CT(alpha),A,x,y); }
    template <bool add, class T, class Ta, class Tx> 
    inline void MultMV(
        const T alpha, const GenLowerTriMatrix<Ta>& A, 
        const GenVector<Tx>& x, const VectorView<CT>& y)
    { MultMV<add>(CT(alpha),A,x,y); }

    template <class T> 
    inline void MultXM(const T alpha, const UpperTriMatrixView<CT>& A)
    { MultXM(CT(alpha),A); }
    template <class T> 
    inline void MultXM(const T alpha, const LowerTriMatrixView<CT>& A)
    { MultXM(CT(alpha),A); }

    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A,
        const UpperTriMatrixView<CT>& B)
    { AddMM(CT(alpha),A,B); }
    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A,
        const LowerTriMatrixView<CT>& B)
    { AddMM(CT(alpha),A,B); }
    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A, const MatrixView<CT>& B)
    { AddMM(CT(alpha),A,B); }
    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A, const MatrixView<CT>& B)
    { AddMM(CT(alpha),A,B); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A,
        const T beta, const GenUpperTriMatrix<Tb>& B, 
        const UpperTriMatrixView<CT>& C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A,
        const T beta, const GenLowerTriMatrix<Tb>& B, 
        const LowerTriMatrixView<CT>& C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A,
        const T beta, const GenMatrix<Tb>& B, const MatrixView<CT>& C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenMatrix<Ta>& A,
        const T beta, const GenUpperTriMatrix<Tb>& B, const MatrixView<CT>& C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A,
        const T beta, const GenMatrix<Tb>& B, const MatrixView<CT>& C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenMatrix<Ta>& A,
        const T beta, const GenLowerTriMatrix<Tb>& B, const MatrixView<CT>& C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A,
        const T beta, const GenLowerTriMatrix<Tb>& B, const MatrixView<CT>& C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A,
        const T beta, const GenUpperTriMatrix<Tb>& B, const MatrixView<CT>& C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T> 
    inline void AddMM(
        const CT alpha, const GenUpperTriMatrix<CT>& A,
        const CT beta, const GenUpperTriMatrix<T>& B,
        const UpperTriMatrixView<CT>& C)
    { AddMM(beta,B,alpha,A,C); }

    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A, 
        const GenMatrix<Tb>& B, const MatrixView<CT>& C)
    { MultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A, 
        const GenMatrix<Tb>& B, const MatrixView<CT>& C)
    { MultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenMatrix<Ta>& A, 
        const GenUpperTriMatrix<Tb>& B, const MatrixView<CT>& C)
    { MultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenMatrix<Ta>& A, 
        const GenLowerTriMatrix<Tb>& B, const MatrixView<CT>& C)
    { MultMM<add>(CT(alpha),A,B,C); }

    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A, 
        const GenUpperTriMatrix<Tb>& B, const UpperTriMatrixView<CT>& C)
    { MultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A, 
        const GenLowerTriMatrix<Tb>& B, const MatrixView<CT>& C)
    { MultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A, 
        const GenUpperTriMatrix<Tb>& B, const MatrixView<CT>& C)
    { MultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A, 
        const GenLowerTriMatrix<Tb>& B, const LowerTriMatrixView<CT>& C)
    { MultMM<add>(CT(alpha),A,B,C); }

    template <bool add, class T, class Ta, class Tb> 
    inline void ElemMultMM(
        const T alpha, const GenUpperTriMatrix<Ta>& A,
        const GenUpperTriMatrix<Tb>& B, const UpperTriMatrixView<CT>& C)
    { ElemMultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T, class Ta, class Tb> 
    inline void ElemMultMM(
        const T alpha, const GenLowerTriMatrix<Ta>& A,
        const GenLowerTriMatrix<Tb>& B, const LowerTriMatrixView<CT>& C)
    { ElemMultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T> 
    inline void ElemMultMM(
        const CT alpha, const GenUpperTriMatrix<CT>& A,
        const GenUpperTriMatrix<T>& B, const UpperTriMatrixView<CT>& C)
    { ElemMultMM<add>(alpha,B,A,C); }

    // Specialize disallowed complex combinations:
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMV(
        const CT , const GenUpperTriMatrix<Ta>& , 
        const GenVector<Tb>& , const VectorView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMV(
        const CT , const GenLowerTriMatrix<Ta>& , 
        const GenVector<Tb>& , const VectorView<T>& )
    { TMVAssert(TMV_FALSE); }

    template <class T> 
    inline void MultXM(const CT , const UpperTriMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T> 
    inline void MultXM(const CT , const LowerTriMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }

    template <class T, class Ta> 
    inline void AddMM(
        const CT , const GenUpperTriMatrix<Ta>& , const UpperTriMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta> 
    inline void AddMM(
        const CT , const GenLowerTriMatrix<Ta>& , const LowerTriMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta> 
    inline void AddMM(
        const CT , const GenUpperTriMatrix<Ta>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta> 
    inline void AddMM(
        const CT , const GenLowerTriMatrix<Ta>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenUpperTriMatrix<Ta>& ,
        const CT , const GenUpperTriMatrix<Tb>& , const UpperTriMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenLowerTriMatrix<Ta>& ,
        const CT , const GenLowerTriMatrix<Tb>& , const LowerTriMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenMatrix<Ta>& ,
        const CT , const GenUpperTriMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenMatrix<Ta>& ,
        const CT , const GenLowerTriMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenUpperTriMatrix<Ta>& ,
        const CT , const GenMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenLowerTriMatrix<Ta>& ,
        const CT , const GenMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenUpperTriMatrix<Ta>& ,
        const CT , const GenLowerTriMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenLowerTriMatrix<Ta>& ,
        const CT , const GenUpperTriMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }

    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenUpperTriMatrix<Ta>& , 
        const GenMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenMatrix<Ta>& , 
        const GenUpperTriMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenUpperTriMatrix<Ta>& , 
        const GenUpperTriMatrix<Tb>& , const UpperTriMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenLowerTriMatrix<Ta>& , 
        const GenMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenMatrix<Ta>& , 
        const GenLowerTriMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenLowerTriMatrix<Ta>& , 
        const GenLowerTriMatrix<Tb>& , const LowerTriMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenUpperTriMatrix<Ta>& , 
        const GenLowerTriMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenLowerTriMatrix<Ta>& , 
        const GenUpperTriMatrix<Tb>& , const MatrixView<T>& )
    { TMVAssert(TMV_FALSE); }

    template <bool add, class T, class Ta, class Tb> 
    inline void ElemMultMM(
        const CT , const GenUpperTriMatrix<Ta>& , 
        const GenUpperTriMatrix<Tb>& , const UpperTriMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void ElemMultMM(
        const CT , const GenLowerTriMatrix<Ta>& , 
        const GenLowerTriMatrix<Tb>& , const LowerTriMatrixView<T>& )
    { TMVAssert(TMV_FALSE); }

} // namespace tmv

#undef CT

#endif
