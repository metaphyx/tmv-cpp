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


//---------------------------------------------------------------------------
//
// This file contains the driver code for doing division using 
// LU Decomposition.
//
// The basic idea of an LU decomposition is that any 
// square matrix A can be decomposed into a lower triangular
// matrix times an upper triangular matrix.
//
// For stability reasons, we actually decompose a permutation
// of A instead, so:
//
// A = P L U
//
// Only one of L or U needs a non-unit diagonal, so we choose L to 
// have unit diagonal, and U to have the non-unit diagonal.
//
// This means that we can store L and U both in a square matrix
// the same size as A, with L being the elements below the diagonal
// and U being the elements above and including the diagonal.
//
// The determinant of A can be calculated easily from the LU
// decomposition:
//
// det(A) = det(P) * det(L) * det(U)
// det(A) = (+-1) * 1 * det(U)
// As we calculate the decomposition, we keep track of whether
// det(P) is +-1 
// The determinant of U is just the product of the diagonal elements.
// So the determinant of A is just det(P) times the diagonal elements 
// of U.
//


#ifndef TMV_LU_H
#define TMV_LU_H

#include "TMV_Divider.h"
#include "TMV_BaseMatrix_Rec.h"
#include "TMV_BaseMatrix_Tri.h"
#include "TMV_Matrix.h"
#include "TMV_NormU.h"
#include "TMV_MultUL.h"
#include "TMV_Permutation.h"

#include "TMV_LUDiv.h"
#include "TMV_LUDecompose.h"
#include "TMV_LUInverse.h"

namespace tmv {

    template <bool small, class M> struct LUD_Impl;

    template <class M> 
    class LUD : public Divider<typename M::value_type> 
    {
    public :

        typedef typename M::value_type T;
        typedef typename M::real_type RT;
        typedef typename M::complex_type CT;

        // This next bit finds the storage type to use for the lu matrix
        // regardless of what kind of matrix M is.  e.g. this should
        // work even if M is a TriMatrix or a BandMatrix, etc.
        enum { cs = M::_colsize };
        enum { rs = M::_rowsize };
        typedef typename MCopyHelper<T,Rec,cs,rs,false,false>::type lu_type;

        typedef typename lu_type::const_view_type getlu_type;
        typedef typename lu_type::const_unit_lowertri_type getl_type;
        typedef typename lu_type::const_uppertri_type getu_type;
        typedef Permutation getp_type;

        //
        // Constructors
        //

        // Sets up the internal storage and does the decomposition.
        LUD(const M& A, bool _inplace=false);

        // The copy constructor has transfer of ownership semantics.
        // This way an LUD object can be returned by value, and the 
        // copy is cheap.  I don't think there is any reason to use
        // a more sophisticated technique like shared_ptr or something
        // similar, since there are no non-const methods.
        LUD(const LUD<M>& rhs);

        // Clean up the internal storage
        ~LUD();


        //
        // Perform the division in place
        //
        template <class M2> 
        void solveInPlace(BaseMatrix_Rec_Mutable<M2>& m2) const;
        template <class V2> 
        void solveInPlace(BaseVector_Mutable<V2>& v2) const;

        template <class M2> 
        void solveTransposeInPlace(BaseMatrix_Rec_Mutable<M2>& m2) const;
        template <class V2> 
        void solveTransposeInPlace(BaseVector_Mutable<V2>& v2) const;

        // These are the virtual functions from the Divider base class.
        inline void doSolveInPlace(MatrixView<RT> m2) const 
        { solveInPlace(m2);  }
        inline void doSolveInPlace(MatrixView<CT> m2) const 
        { solveInPlace(m2);  }
        inline void doSolveInPlace(
            MatrixView<CT,UNKNOWN,UNKNOWN,true> m2) const 
        { solveInPlace(m2);  }
        inline void doSolveInPlace(VectorView<RT> v2) const 
        { solveInPlace(v2);  }
        inline void doSolveInPlace(VectorView<CT> v2) const 
        { solveInPlace(v2);  }
        inline void doSolveInPlace(VectorView<CT,UNKNOWN,true> v2) const 
        { solveInPlace(v2);  }

        inline void doSolveTransposeInPlace(MatrixView<RT> m2) const 
        { solveTransposeInPlace(m2); } 
        inline void doSolveTransposeInPlace(MatrixView<CT> m2) const 
        { solveTransposeInPlace(m2); } 
        inline void doSolveTransposeInPlace(
            MatrixView<CT,UNKNOWN,UNKNOWN,true> m2) const 
        { solveTransposeInPlace(m2); } 
        inline void doSolveTransposeInPlace(VectorView<RT> v2) const 
        { solveTransposeInPlace(v2); } 
        inline void doSolveTransposeInPlace(VectorView<CT> v2) const 
        { solveTransposeInPlace(v2); } 
        inline void doSolveTransposeInPlace(
            VectorView<CT,UNKNOWN,true> v2) const 
        { solveTransposeInPlace(v2); } 


        //
        // Next the not-in-place division 
        // For LU, we just copy m1->m3 and do the in-place version.
        // 
        
        template <class M1, class M2> 
        inline void solve(
            const BaseMatrix<M1>& m1, BaseMatrix_Rec_Mutable<M2>& m2) const
        { solveInPlace(m2=m1); }
        template <class V1, class V2> 
        inline void solve(
            const BaseVector<V1>& v1, BaseVector_Mutable<V2>& v2) const
        { solveInPlace(v2=v1); }

        template <class M1, class M2> 
        inline void solveTranspose(
            const BaseMatrix<M1>& m1, BaseMatrix_Rec_Mutable<M2>& m2) const
        { solveTransposeInPlace(m2=m1); }
        template <class V1, class V2> 
        inline void solveTranspose(
            const BaseVector<V1>& v1, BaseVector_Mutable<V2>& v2) const
        { solveTransposeInPlace(v2=v1); }

        // These are the virtual functions from the Divider base class.
        inline void doSolve(
            const ConstMatrixView<RT>& m1, MatrixView<RT> m2) const 
        { solve(m1,m2); } 
        inline void doSolve(
            const ConstMatrixView<RT>& m1, MatrixView<CT> m2) const 
        { solve(m1,m2); } 
        inline void doSolve(
            const ConstMatrixView<RT>& m1, 
            MatrixView<CT,UNKNOWN,UNKNOWN,true> m2) const 
        { solve(m1,m2); } 
        inline void doSolve(
            const ConstMatrixView<CT>& m1, MatrixView<CT> m2) const 
        { solve(m1,m2); } 
        inline void doSolve(
            const ConstMatrixView<CT>& m1, 
            MatrixView<CT,UNKNOWN,UNKNOWN,true> m2) const 
        { solve(m1,m2); } 
        inline void doSolve(
            const ConstMatrixView<CT,UNKNOWN,UNKNOWN,true>& m1, 
            MatrixView<CT> m2) const 
        { solve(m1,m2); } 
        inline void doSolve(
            const ConstMatrixView<CT,UNKNOWN,UNKNOWN,true>& m1, 
            MatrixView<CT,UNKNOWN,UNKNOWN,true> m2) const 
        { solve(m1,m2); } 
        inline void doSolve(
            const ConstVectorView<RT>& v1, VectorView<RT> v2) const 
        { solve(v1,v2); } 
        inline void doSolve(
            const ConstVectorView<RT>& v1, VectorView<CT> v2) const 
        { solve(v1,v2); } 
        inline void doSolve(
            const ConstVectorView<RT>& v1, 
            VectorView<CT,UNKNOWN,true> v2) const 
        { solve(v1,v2); } 
        inline void doSolve(
            const ConstVectorView<CT>& v1, VectorView<CT> v2) const 
        { solve(v1,v2); } 
        inline void doSolve(
            const ConstVectorView<CT>& v1, 
            VectorView<CT,UNKNOWN,true> v2) const 
        { solve(v1,v2); } 
        inline void doSolve(
            const ConstVectorView<CT,UNKNOWN,true>& v1, 
            VectorView<CT> v2) const 
        { solve(v1,v2); } 
        inline void doSolve(
            const ConstVectorView<CT,UNKNOWN,true>& v1, 
            VectorView<CT,UNKNOWN,true> v2) const 
        { solve(v1,v2); } 

        inline void doSolveTranspose(
            const ConstMatrixView<RT>& m1, MatrixView<RT> m2) const 
        { solveTranspose(m1,m2); }
        inline void doSolveTranspose(
            const ConstMatrixView<RT>& m1, MatrixView<CT> m2) const 
        { solveTranspose(m1,m2); }
        inline void doSolveTranspose(
            const ConstMatrixView<RT>& m1, 
            MatrixView<CT,UNKNOWN,UNKNOWN,true> m2) const 
        { solveTranspose(m1,m2); }
        inline void doSolveTranspose(
            const ConstMatrixView<CT>& m1, MatrixView<CT> m2) const 
        { solveTranspose(m1,m2); }
        inline void doSolveTranspose(
            const ConstMatrixView<CT>& m1, 
            MatrixView<CT,UNKNOWN,UNKNOWN,true> m2) const 
        { solveTranspose(m1,m2); }
        inline void doSolveTranspose(
            const ConstMatrixView<CT,UNKNOWN,UNKNOWN,true>& m1, 
            MatrixView<CT> m2) const 
        { solveTranspose(m1,m2); }
        inline void doSolveTranspose(
            const ConstMatrixView<CT,UNKNOWN,UNKNOWN,true>& m1, 
            MatrixView<CT,UNKNOWN,UNKNOWN,true> m2) const 
        { solveTranspose(m1,m2); }
        inline void doSolveTranspose(
            const ConstVectorView<RT>& v1, VectorView<RT> v2) const 
        { solveTranspose(v1,v2); }
        inline void doSolveTranspose(
            const ConstVectorView<RT>& v1, VectorView<CT> v2) const 
        { solveTranspose(v1,v2); }
        inline void doSolveTranspose(
            const ConstVectorView<RT>& v1, 
            VectorView<CT,UNKNOWN,true> v2) const 
        { solveTranspose(v1,v2); }
        inline void doSolveTranspose(
            const ConstVectorView<CT>& v1, VectorView<CT> v2) const 
        { solveTranspose(v1,v2); }
        inline void doSolveTranspose(
            const ConstVectorView<CT>& v1, 
            VectorView<CT,UNKNOWN,true> v2) const 
        { solveTranspose(v1,v2); }
        inline void doSolveTranspose(
            const ConstVectorView<CT,UNKNOWN,true>& v1, 
            VectorView<CT> v2) const 
        { solveTranspose(v1,v2); }
        inline void doSolveTranspose(
            const ConstVectorView<CT,UNKNOWN,true>& v1, 
            VectorView<CT,UNKNOWN,true> v2) const 
        { solveTranspose(v1,v2); }


        //
        // Determinant
        //
        
        T det() const;
        RT logDet(T* sign) const;
        bool isSingular() const;

        
        //
        // Inverse
        //
        
        template <class M2> 
        void makeInverse(BaseMatrix_Rec_Mutable<M2>& minv) const;

        inline void doMakeInverse(MatrixView<RT> minv) const 
        { makeInverse(minv); } 
        inline void doMakeInverse(MatrixView<CT> minv) const 
        { makeInverse(minv); } 
        inline void doMakeInverse(
            MatrixView<CT,UNKNOWN,UNKNOWN,true> minv) const 
        { makeInverse(minv); } 


        //
        // InverseATA
        //
        
        template <class M2>
        void makeInverseATA(BaseMatrix_Rec_Mutable<M2>& ata) const;

        inline void doMakeInverseATA(MatrixView<RT> ata) const
        { makeInverseATA(ata); }
        inline void doMakeInverseATA(MatrixView<CT> ata) const
        { makeInverseATA(ata); }
        inline void doMakeInverseATA(
            MatrixView<CT,UNKNOWN,UNKNOWN,true> ata) const
        { makeInverseATA(ata); }


        // 
        // Condition (kappa_inf)
        //

        RT condition(RT normInf) const;


        //
        // Access Decomposition
        //

        bool isTrans() const;
        getl_type getL() const;
        getu_type getU() const;
        getlu_type getLU() const;
        getp_type getP() const;

        template <class M2>
        bool checkDecomp(
            const BaseMatrix<M2>& m2, std::ostream* fout) const;

        inline bool doCheckDecomp(
            const ConstMatrixView<T>& m2, std::ostream* fout) const
        { return checkDecomp(m2,fout); }
        inline bool doCheckDecomp(
            const ConstMatrixView<T,UNKNOWN,UNKNOWN,true>& m2,
            std::ostream* fout) const
        { return checkDecomp(Maybe<Traits<T>::isreal>::conjugate(m2),fout); }

        bool preferInPlace() const { return true; }

    private :

        enum { small = (
                M::_colsize != UNKNOWN && M::_rowsize != UNKNOWN &&
                M::_colsize <= 8 && M::_rowsize <= 8 ) };

        mutable std::auto_ptr<LUD_Impl<small,M> > pimpl;

        size_t colsize() const;
        size_t rowsize() const;

        // op= not allowed.
        LUD<M>& operator=(const LUD<M>&);
    };
    
    template <bool isvalid, bool istrans> struct LUHelper;

    template <>
    struct LUHelper<true,false>
    {
        template <class M1, class M2>
        static void copy(const M1& m1, M2& m2)
        { NoAliasCopy(m1,m2); }
        template <class M1, class M2>
        static void makeInverse(const M1& LUx, const int* P, M2& m2)
        {
            // This one might be same storage if LUx was done in place.
            // e.g. { A.divideInPlace(); A = A.inverse(); }
            // So go ahead and check. (i.e. Don't use NoAliasCopy.)
            Copy(LUx,m2);
            LU_Inverse(m2,P);
        }
        template <class M1, class M2>
        static void solveInPlace(const M1& LUx, const int* P, M2& m2)
        { LU_SolveInPlace(LUx,P,m2); }
        template <class M1>
        static typename M1::const_view_type view(const M1& m)
        { return m.view(); }
        template <class M1>
        static int stepj(const M1& m)
        { return m.stepj(); }
    };
    template <>
    struct LUHelper<true,true>
    {
        template <class M1, class M2>
        static void copy(const M1& m1, M2& m2)
        { NoAliasCopy(m1.transpose(),m2); }
        template <class M1, class M2>
        static void makeInverse(const M1& LUx, const int* P, M2& m2)
        {
            typename M2::transpose_type m2t = m2.transpose();
            Copy(LUx,m2t);
            LU_Inverse(m2t,P);
        }
        template <class M1, class M2>
        static void solveInPlace(const M1& LUx, const int* P, M2& m2)
        { LU_SolveTransposeInPlace(LUx,P,m2); }
        template <class M1>
        static typename M1::const_transpose_type view(const M1& m)
        { return m.transpose(); }
        template <class M1>
        static int stepj(const M1& m)
        { return m.stepi(); }
    };
    template <bool istrans>
    struct LUHelper<false,istrans>
    {
        template <class M1, class M2>
        static void copy(const M1& , M2& ) {}
        template <class M1, class M2>
        static void makeInverse(const M1& , const int* , M2& ) {}
        template <class M1, class M2>
        static void solveInPlace(const M1& , const int* , M2& ) {}
        template <class M1>
        static void view(const M1& ) {}
    };

    template <class M> 
    struct LUD_Impl<true,M>
    {
        enum { istrans = M::_rowmajor };
        enum { size = M::_colsize };
        typedef typename LUD<M>::lu_type::view_type lux_type;

        LUD_Impl(const M& A, bool ) : LUx( SmallLUx.view() ), detp(1) 
        { 
            TMVStaticAssert(M::_colsize == int(M::_rowsize));
            TMVStaticAssert(M::_colsize != UNKNOWN);
            TMVStaticAssert(M::_colsize == int(LUD<M>::lu_type::_colsize));
            TMVStaticAssert(M::_rowsize == int(LUD<M>::lu_type::_rowsize));
            LUHelper<true,istrans>::copy(A,LUx); 
        }

        typename LUD<M>::lu_type SmallLUx;
        lux_type LUx;
        StackArray<int,size> P;
        int detp;
    };
    
    template <class M> 
    struct LUD_Impl<false,M>
    {
        enum { istrans = M::_rowmajor };
        enum { rmorcm = M::_rowmajor || M::_colmajor };
        typedef typename LUD<M>::lu_type::view_type lux_type;

        LUD_Impl(const M& A, bool _inplace) :
            // inplace only if matrix is rowmajor or colmajor
            inplace(rmorcm && _inplace),
            // Aptr is the pointer to new storage if any
            Aptr( inplace ? 0 : A.rowsize()*A.rowsize() ),
            // LUx views this memory as the LU matrix
            LUx(
                inplace ? A.nonConst().ptr() : Aptr.get() , // ptr
                // A is square, so no need to check istrans for the right
                // values of rowsize,colsize here.
                A.rowsize() ,  // colsize
                A.rowsize() ,  // rowsize
                1 ,  // stepi
                // Here we do need to check istrans for the right step.
                ( inplace ? LUHelper<true,istrans>::stepj(A) :
                  int(A.rowsize()) ) // stepj
            ),
            // allocate memory for the permutation
            P(A.rowsize()), detp(1) 
            {
                if (!inplace) {
                    LUHelper<true,istrans>::copy(A,LUx);
                } else {
                    Maybe<M::_conj>::conjself(LUx);
                }
            }

        const bool inplace;
        AlignedArray<typename M::value_type> Aptr;
        lux_type LUx;
        AlignedArray<int> P;
        int detp;
    };

    template <class M> 
    LUD<M>::LUD(const M& A, bool inplace) :
        pimpl(new LUD_Impl<small,M>(A,inplace)) 
    {
        TMVStaticAssert((Sizes<M::_colsize,M::_rowsize>::same));
        TMVAssert(A.isSquare());
        LU_Decompose(pimpl->LUx,pimpl->P,pimpl->detp);
    }

    template <class M> 
    LUD<M>::LUD(const LUD<M>& rhs) : pimpl(rhs.pimpl.release()) {}

    template <class M> 
    LUD<M>::~LUD() {}

    template <class M> template <class M2> 
    void LUD<M>::solveInPlace(BaseMatrix_Rec_Mutable<M2>& m2) const
    {
        TMVStaticAssert((Sizes<M2::_colsize,M::_colsize>::same));
        TMVAssert(m2.colsize() == colsize());
        const bool isvalid = M::isreal || M2::iscomplex;
        const bool istrans = LUD_Impl<small,M>::istrans;
        LUHelper<isvalid,istrans>::solveInPlace(pimpl->LUx,pimpl->P,m2);
    }

    template <class M> template <class M2> 
    void LUD<M>::solveTransposeInPlace(BaseMatrix_Rec_Mutable<M2>& m2) const
    {
        TMVStaticAssert((Sizes<M2::_colsize,M::_rowsize>::same));
        TMVAssert(m2.colsize() == rowsize());
        const bool isvalid = M::isreal || M2::iscomplex;
        const bool istrans = LUD_Impl<small,M>::istrans;
        LUHelper<isvalid,!istrans>::solveInPlace(pimpl->LUx,pimpl->P,m2);
    }

    template <class M> template <class V2> 
    void LUD<M>::solveInPlace(BaseVector_Mutable<V2>& v2) const
    {
        TMVStaticAssert((Sizes<V2::_size,M::_colsize>::same));
        TMVAssert(v2.size() == colsize());
        const bool isvalid = M::isreal || V2::iscomplex;
        const bool istrans = LUD_Impl<small,M>::istrans;
        LUHelper<isvalid,istrans>::solveInPlace(pimpl->LUx,pimpl->P,v2);
    }

    template <class M> template <class V2> 
    void LUD<M>::solveTransposeInPlace(BaseVector_Mutable<V2>& v2) const
    {
        TMVStaticAssert((Sizes<V2::_size,M::_rowsize>::same));
        TMVAssert(v2.size() == rowsize());
        const bool isvalid = M::isreal || V2::iscomplex;
        const bool istrans = LUD_Impl<small,M>::istrans;
        LUHelper<isvalid,!istrans>::solveInPlace(pimpl->LUx,pimpl->P,v2);
    }

    template <class M> 
    typename M::value_type LUD<M>::det() const
    { return typename M::real_type(pimpl->detp) * getU().det(); }                  

    template <class M> 
    typename M::real_type LUD<M>::logDet(typename M::value_type* sign) const
    {
        typename M::real_type ret = getU().logDet(sign);
        if (sign) *sign *= typename M::real_type(pimpl->detp);
        return ret;
    }                  

    template <class M> 
    bool LUD<M>::isSingular() const 
    { return getU().isSingular(); }

    template <class M> template <class M2> 
    void LUD<M>::makeInverse(BaseMatrix_Rec_Mutable<M2>& minv) const
    {
        TMVAssert(minv.colsize() == rowsize());
        TMVAssert(minv.rowsize() == colsize());
        const bool isvalid = M::isreal || M2::iscomplex;
        const bool istrans = LUD_Impl<small,M>::istrans;
        LUHelper<isvalid,istrans>::makeInverse(pimpl->LUx,pimpl->P,minv);
    }

    template <class M> template <class M2>
    void LUD<M>::makeInverseATA(BaseMatrix_Rec_Mutable<M2>& ata) const
    {
        TMVAssert(ata.rowsize() == rowsize());
        TMVAssert(ata.colsize() == rowsize());
        LU_InverseATA(pimpl->LUx,pimpl->P,pimpl->istrans,ata);
    }

    template <class M> 
    bool LUD<M>::isTrans() const 
    { return LUD_Impl<small,M>::istrans; }

    template <class M> 
    typename LUD<M>::getl_type LUD<M>::getL() const 
    { return pimpl->LUx.unitLowerTri(); }

    template <class M> 
    typename LUD<M>::getu_type LUD<M>::getU() const 
    { return pimpl->LUx.upperTri(); }

    template <class M> 
    typename LUD<M>::getlu_type LUD<M>::getLU() const 
    { return pimpl->LUx; }

    template <class M> 
    typename LUD<M>::getp_type LUD<M>::getP() const 
    { return Permutation(rowsize(),pimpl->P,true,pimpl->detp); }

    template <class M> 
    typename M::real_type LUD<M>::condition(RT normInf) const 
    {
        // TODO: This is a placeholder until I write the real function.
        // Make sure to do this before releasing the code!
        // See page 129 of Golub and van Loan.
        //
        // This produces the exact right answer, but it is way too slow!
        // The GvL algorithm is order n^2.  This is order n^3.
        Matrix<T> minv(rowsize(),colsize());
        makeInverse(minv);
        return normInf * minv.normInf();
    }

    template <class M> template <class M2>
    bool LUD<M>::checkDecomp(
        const BaseMatrix<M2>& m2, std::ostream* fout) const
    {
        const bool istrans = LUD_Impl<small,M>::istrans;
        typename M2::calc_type mm = m2.calc();
        bool printmat = fout && m2.colsize() < 100 && m2.rowsize() < 100;
        if (printmat) {
            *fout << "LU:\n";
            *fout << LUHelper<true,istrans>::view(mm) << std::endl;
            *fout << "L = "<<getL()<<std::endl;
            *fout << "U = "<<getU()<<std::endl;
        }
        typename M::copy_type lu = getL()*getU();
        if (printmat) {
            *fout << "LU = "<<lu<<std::endl;
        }
        lu = getP() * lu;
        if (printmat) {
            *fout << "PLU = "<<lu<<std::endl;
        }
        RT nm = Norm(lu-LUHelper<true,istrans>::view(mm));
        nm /= Norm(getL())*Norm(getU());
        RT kappa = condition(mm.normInf());
        if (fout) {
            *fout << "Norm(M-PLU)/Norm(PLU) = "<<nm<<" <? ";
            *fout << kappa<<"*"<<RT(mm.colsize())<<"*"<<TMV_Epsilon<T>();
            *fout << " = "<<kappa*RT(mm.colsize())*TMV_Epsilon<T>()<<std::endl;
        }
        return nm < kappa*RT(mm.colsize())*TMV_Epsilon<T>();
    }

    template <class M> size_t LUD<M>::colsize() const
    { return pimpl->LUx.colsize(); }

    template <class M> size_t LUD<M>::rowsize() const
    { return pimpl->LUx.rowsize(); }

} // namespace tmv

#endif