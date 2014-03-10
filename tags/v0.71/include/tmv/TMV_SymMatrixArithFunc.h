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


#ifndef TMV_SymMatrixArithFunc_H
#define TMV_SymMatrixArithFunc_H

#include "tmv/TMV_BaseSymMatrix.h"
#include "tmv/TMV_VectorArithFunc.h"
#include "tmv/TMV_MatrixArithFunc.h"
#include "tmv/TMV_TriMatrixArithFunc.h"
#include "tmv/TMV_Array.h"

#define CT std::complex<T>

namespace tmv {

    // y (+)= alpha * A * x
    template <bool add, class T, class Ta, class Tx> 
    void MultMV(
        const T alpha, const GenSymMatrix<Ta>& A, 
        const GenVector<Tx>& x, VectorView<T> y);

    // A = alpha * A
    template <class T> 
    inline void MultXM(const T alpha, SymMatrixView<T> A)
    { MultXM(alpha,A.upperTri()); }

    // B += alpha * A
    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A,
        SymMatrixView<T> B)
    { AddMM(alpha,A.upperTri(),B.upperTri()); }
    template <class T, class Ta> 
    void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A, MatrixView<T> B);

    // C = alpha * A + beta * B
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A, 
        const T beta, const GenSymMatrix<Tb>& B, SymMatrixView<T> C)
    { AddMM(alpha,A.upperTri(),beta,B.upperTri(),C.upperTri()); }
    template <class T, class Ta, class Tb> 
    void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A, 
        const T beta, const GenSymMatrix<Tb>& B, MatrixView<T> C);
    template <class T, class Ta, class Tb> 
    void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A, 
        const T beta, const GenMatrix<Tb>& B, MatrixView<T> C);
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenMatrix<Ta>& A, 
        const T beta, const GenSymMatrix<Tb>& B, MatrixView<T> C)
    { AddMM(beta,B,alpha,A,C); }

    // C (+)= alpha * A * B
    template <bool add, class T, class Ta, class Tb> 
    void MultMM(
        const T alpha, const GenSymMatrix<Ta>& A, 
        const GenMatrix<Tb>& B, MatrixView<T> C);
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenMatrix<Ta>& A, 
        const GenSymMatrix<Tb>& B, MatrixView<T> C)
    { MultMM<add>(alpha,B.transpose(),A.transpose(),C.transpose()); }
    template <bool add, class T, class Ta, class Tb> 
    void MultMM(
        const T alpha, const GenSymMatrix<Ta>& A, 
        const GenSymMatrix<Tb>& B, MatrixView<T> );

    // A (+)= alpha * (x^xT) (or x* if A is Herm)
    template <bool add, class T, class Tx> 
    void Rank1Update(
        const T alpha, const GenVector<Tx>& x, SymMatrixView<T> A);

    // B (+)= alpha * (A * AT) (or At if B is Herm)
    template <bool add, class T, class Ta> 
    void RankKUpdate(
        const T alpha, const GenMatrix<Ta>& A, SymMatrixView<T> B);

    template <bool add, class T, class Ta> 
    void RankKUpdate(
        const T alpha, const GenLowerTriMatrix<Ta>& A, 
        SymMatrixView<T> B);
    template <bool add, class T, class Ta> 
    void RankKUpdate(
        const T alpha, const GenUpperTriMatrix<Ta>& A, 
        SymMatrixView<T> B);

    // These two don't have += forms: they must called explicitly
    // A (+)= alpha * (x^y + y^x) (or x^y* + y^x* is A is Herm)
    template <bool add, class T, class Tx, class Ty> 
    void Rank2Update(
        const T alpha, const GenVector<Tx>& x,
        const GenVector<Ty>& y, SymMatrixView<T> A);
    template <bool add, class T, class Tx, class Ty, int A3> 
    inline void Rank2Update(
        const T alpha, const GenVector<Tx>& x,
        const GenVector<Ty>& y, SymMatrix<T,A3>& A)
    { Rank2Update(alpha,x,y,A.view()); }
    template <bool add, class T, class Tx, class Ty, int A3> 
    inline void Rank2Update(
        const T alpha, const GenVector<Tx>& x,
        const GenVector<Ty>& y, HermMatrix<T,A3>& A)
    { Rank2Update(alpha,x,y,A.view()); }

    // C (+)= alpha * (A * BT + B*AT) (or A*Bt + B*At if C is Herm)
    template <bool add, class T, class Ta, class Tb> 
    void Rank2KUpdate(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, SymMatrixView<T> C);
    template <bool add, class T, class Ta, class Tb, int A3> 
    inline void Rank2KUpdate(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, SymMatrix<T,A3>& C)
    { Rank2KUpdate(alpha,A,B,C.view()); }
    template <bool add, class T, class Ta, class Tb, int A3> 
    inline void Rank2KUpdate(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, HermMatrix<T,A3>& C)
    { Rank2KUpdate(alpha,A,B,C.view()); }

    // C (+)= alpha * A * B
    // This also needs to be called explicitly.
    // This is to prevent the programmer from doing this accidentally
    // when alpha * A * B is not necessarily symmetrix/hermitian.
    template <bool add, class T, class Ta, class Tb> 
    void SymMultMM(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, SymMatrixView<T> C);
    template <bool add, class T, class Ta, class Tb, int A3> 
    inline void SymMultMM(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, SymMatrix<T,A3>& C)
    { SymMultMM(alpha,A,B,C.view()); }
    template <bool add, class T, class Ta, class Tb, int A3> 
    inline void SymMultMM(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, HermMatrix<T,A3>& C)
    { SymMultMM(alpha,A,B,C.view()); }

    template <class T> 
    class SymMatrixComposite : public GenSymMatrix<T>
    {
    public :
        inline SymMatrixComposite() {}
        inline SymMatrixComposite(const SymMatrixComposite<T>&) {}
        virtual inline ~SymMatrixComposite() {}

        // Definitions are in TMV_MultSV.cpp
        const T* cptr() const;
        ptrdiff_t stepi() const;
        ptrdiff_t stepj() const;

        inline SymType sym() const { return Sym; }
        inline UpLoType uplo() const { return Lower; }
        inline ConjType ct() const { return NonConj; }

    private :
        mutable AlignedArray<T> itsm;

        SymMatrixComposite<T>& operator=(const SymMatrixComposite<T>&);
    };

    template <class T> 
    class SymMatrixComposite<CT> :
        public MatrixComposite<CT>,
        virtual public AssignableToSymMatrix<CT>
    {
    public :

        inline ptrdiff_t colsize() const { return this->size(); }
        inline ptrdiff_t rowsize() const { return this->size(); }

        inline void assignToM(MatrixView<TMV_RealType(T)> m0) const
        { MatrixComposite<CT>::assignToM(m0); }
        inline void assignToM(MatrixView<TMV_ComplexType(T)> m0) const
        { MatrixComposite<CT>::assignToM(m0); }
    };

    // Specialize allowed complex combinations:
    template <bool add, class T, class Ta, class Tx> 
    inline void MultMV(
        const T alpha, const GenSymMatrix<Ta>& A, 
        const GenVector<Tx>& x, VectorView<CT> y)
    { MultMV<add>(CT(alpha),A,x,y); }

    template <class T> 
    inline void MultXM(const T alpha, SymMatrixView<CT> A)
    { MultXM(CT(alpha),A); }

    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A, SymMatrixView<CT> B)
    { AddMM(CT(alpha),A,B); }
    template <class T, class Ta> 
    inline void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A, MatrixView<CT> B)
    { AddMM(CT(alpha),A,B); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A, 
        const T beta, const GenSymMatrix<Tb>& B, SymMatrixView<CT> C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A, 
        const T beta, const GenSymMatrix<Tb>& B, MatrixView<CT> C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenSymMatrix<Ta>& A, 
        const T beta, const GenMatrix<Tb>& B, MatrixView<CT> C)
    { AddMM(CT(alpha),A,CT(beta),B,C); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const T alpha, const GenMatrix<Ta>& A, 
        const T beta, const GenSymMatrix<Tb>& B, MatrixView<CT> C)
    { AddMM(beta,B,alpha,A,C); }
    template <class T> 
    inline void AddMM(
        const CT alpha, const GenSymMatrix<CT>& A, 
        const CT beta, const GenSymMatrix<T>& B, MatrixView<CT> C)
    { AddMM(beta,B,alpha,A,C); }

    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenSymMatrix<Ta>& A, 
        const GenMatrix<Tb>& B, MatrixView<CT> C)
    { MultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenMatrix<Ta>& A, 
        const GenSymMatrix<Tb>& B, MatrixView<CT> C)
    { MultMM<add>(CT(alpha),A,B,C); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const T alpha, const GenSymMatrix<Ta>& A, 
        const GenSymMatrix<Tb>& B, MatrixView<CT> C)
    { MultMM<add>(CT(alpha),A,B,C); }

    template <bool add, class T, class Tx> 
    inline void Rank1Update(
        const T alpha, const GenVector<Tx>& x, SymMatrixView<CT> A)
    { Rank1Update<add>(CT(alpha),x,A); }
    template <bool add, class T, class Ta> 
    inline void RankKUpdate(
        const T alpha, const GenMatrix<Ta>& A, SymMatrixView<CT> B)
    { RankKUpdate<add>(CT(alpha),A,B); }
    template <bool add, class T, class Ta> 
    inline void RankKUpdate(
        const T alpha, const GenLowerTriMatrix<Ta>& A, 
        SymMatrixView<CT> B)
    { RankKUpdate<add>(CT(alpha),A,B); }
    template <bool add, class T, class Ta> 
    inline void RankKUpdate(
        const T alpha, const GenUpperTriMatrix<Ta>& A, 
        SymMatrixView<CT> B)
    { RankKUpdate<add>(CT(alpha),A,B); }

    template <bool add, class T, class Tx, class Ty> 
    inline void Rank2Update(
        const T alpha, const GenVector<Tx>& x,
        const GenVector<Ty>& y, SymMatrixView<CT> A)
    { Rank2Update<add>(CT(alpha),x,y,A); }
    template <bool add, class T, class Ta, class Tb> 
    inline void Rank2KUpdate(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, SymMatrixView<CT> C)
    { Rank2KUpdate<add>(CT(alpha),A,B,C); }

    template <bool add, class T, class Ta, class Tb> 
    inline void SymMultMM(
        const T alpha, const GenMatrix<Ta>& A,
        const GenMatrix<Tb>& B, SymMatrixView<CT> C)
    { SymMultMM<add>(CT(alpha),A,B,C); }

    // Specialize disallowed complex combinations:
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMV(
        const CT , const GenSymMatrix<Ta>& , 
        const GenVector<Tb>& , VectorView<T> )
    { TMVAssert(TMV_FALSE); }

    template <class T> 
    inline void MultXM(const CT , SymMatrixView<T> )
    { TMVAssert(TMV_FALSE); }

    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenSymMatrix<Ta>& , 
        const CT , const GenSymMatrix<Tb>& , SymMatrixView<T> )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenSymMatrix<Ta>& , 
        const CT , const GenSymMatrix<Tb>& , MatrixView<T> )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenSymMatrix<Ta>& , 
        const CT , const GenMatrix<Tb>& , MatrixView<T> )
    { TMVAssert(TMV_FALSE); }
    template <class T, class Ta, class Tb> 
    inline void AddMM(
        const CT , const GenMatrix<Ta>& , 
        const CT , const GenSymMatrix<Tb>& , MatrixView<T> )
    { TMVAssert(TMV_FALSE); }

    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenSymMatrix<Ta>& , 
        const GenMatrix<Tb>& , MatrixView<T> )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenMatrix<Ta>& , 
        const GenSymMatrix<Tb>& , MatrixView<T> )
    { TMVAssert(TMV_FALSE); }
    template <bool add, class T, class Ta, class Tb> 
    inline void MultMM(
        const CT , const GenSymMatrix<Ta>& , 
        const GenSymMatrix<Tb>& , MatrixView<T> )
    { TMVAssert(TMV_FALSE); }


} // namespace tmv

#undef CT

#endif