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


#ifndef TMV_DivMU_H
#define TMV_DivMU_H

#include "TMV_MultMM.h"
#include "TMV_MultUV.h"
#include "TMV_MultUM.h"
#include "TMV_CopyU.h"
#include "TMV_MultXU.h"
#include "TMV_DivVU.h"
#include "TMV_Rank1VVM.h"
#include "TMV_SmallTriMatrix.h"
#include "TMV_InvertU.h"

#ifdef _OPENMP
#include "omp.h"
#ifdef PRINTALGO_DIVU_OMP
#include <fstream>
#endif
#endif

#ifdef PRINTALGO_DIVU
#include <iostream>
#endif

// Use the specialized 1,2,3 sized algorithms for the end of the 
// recursive algorithm.
#if TMV_OPT >= 2
#define TMV_OPT_CLEANUP
#endif

// Check for small (<=5) values of cs or rs
// This leads to significant speed improvements for such matrices at little
// cost to larger matrices, but the large increase in code size and the fact
// that it only benefits a few particular sizes of matrices mean that
// we require TMV_OPT = 3 for it.
#if TMV_OPT >= 3
#define TMV_OPT_SMALL
#endif

// Q2 is the minimum size to keep recursing
#define TMV_Q2 4

// Q6 is the minimum value of (M^2*N / 16^3) to use multiple threads.
// (We also require that N > 64 so we have something to split.)
#define TMV_Q6 64

namespace tmv {

    // Defined below:
    template <class M1, class M2>
    inline void LDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const BaseMatrix_Tri<M2>& m2);
    template <class M1, class M2>
    inline void NoAliasLDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const BaseMatrix_Tri<M2>& m2);
    template <class M1, class M2>
    inline void InlineLDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const BaseMatrix_Tri<M2>& m2);
    template <class M1, class M2>
    inline void AliasLDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const BaseMatrix_Tri<M2>& m2);

    // Defined in TMV_DivMU.cpp
    template <class T1, class T2, bool C2>
    void InstLDivEq(
        MatrixView<T1> m1,
        const ConstUpperTriMatrixView<T2,UnknownDiag,UNKNOWN,UNKNOWN,C2>& m2);
    template <class T1, class T2, bool C2>
    void InstLDivEq(
        MatrixView<T1> m1,
        const ConstLowerTriMatrixView<T2,UnknownDiag,UNKNOWN,UNKNOWN,C2>& m2);


    //
    // m1 /= m2
    // m2 is UpperTri or LowerTri
    //

    template <int algo, int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper;

    // algo 0: Trivial, nothing to do.
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<0,cs,rs,M1,M2>
    { static void call(M1& , const M2& ) {} };

    // algo 1: cs == 1, so reduces to MultXV
    template <int rs, class M1, class M2>
    struct LDivEqMU_Helper<1,1,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
#ifdef PRINTALGO_DIVU
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 1: M,N,cs,rs = "<<1<<','<<N<<
                ','<<1<<','<<rs<<std::endl;
#endif
            const bool u2 = M2::munit;
            typedef typename M1::row_type M1r;
            typedef typename M2::real_type RT;
            typedef typename M2::value_type T2;
            typedef typename Maybe<u2>::template SelectType<RT,T2>::type XT2;
            const int ix2 = u2 ? 1 : 0;

            M1r m1r = m1.get_row(0);
            const Scaling<ix2,XT2> inv00(RT(1)/m2.cref(0,0));
            ScaleV_Helper<-1,rs,ix2,XT2,M1r>::call(inv00, m1r);
        }
    };

    // algo 2: rs == 1, so reduces to LDivVU
    template <int cs, class M1, class M2>
    struct LDivEqMU_Helper<2,cs,1,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
#ifdef PRINTALGO_DIVU
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
            std::cout<<"LDivEqMU algo 2: M,N,cs,rs = "<<M<<','<<1<<
                ','<<cs<<','<<1<<std::endl;
#endif
            typedef typename M1::col_type M1c;
            typedef typename M2::const_col_type M2c;

            M1c m1c = m1.get_col(0);
            LDivEqVU_Helper<-1,cs,M1c,M2>::call(m1c,m2);
        }
    };

    // algo 11: UpperTri loop over n
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<11,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
#ifdef PRINTALGO_DIVU
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
            std::cout<<"LDivEqMU algo 11: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            typedef typename M1::col_type M1c;
            for(int j=0;j<N;++j) {
                // m1.col(j) /= m2
                M1c m1j = m1.get_col(j);
                LDivEqVU_Helper<-4,cs,M1c,M2>::call(m1j,m2);
            }
        }
    };

    // algo 12: UpperTri loop over m
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<12,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
#ifdef PRINTALGO_DIVU
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 12: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            const bool u2 = M2::munit;
            typedef typename M2::real_type RT;
            typedef typename M2::value_type T2;
            typedef typename Maybe<u2>::template SelectType<RT,T2>::type XT2;
            typedef typename M2::const_row_sub_type M2r;
            typedef typename M1::row_type M1r;
            typedef typename M1::rowrange_type::const_transpose_type M1rrt;
            const int ix2 = u2 ? 1 : 0;
            const int xx = UNKNOWN;
            const Scaling<-1,RT> mone;

            for(int i=M;i--;) {
                // m1.row(i) -= m2.row(i,i+1,M) * m1.rowRange(i+1,M)
                // m1.row(i) /= m1(i,i)
                M1r m1i = m1.get_row(i);
                M1rrt m1rrt = m1.cRowRange(i+1,M).transpose();
                M2r m2i = m2.get_row(i,i+1,M);
                MultMV_Helper<-4,rs,xx,true,-1,RT,M1rrt,M2r,M1r>::call(
                    mone,m1rrt,m2i,m1i);
                const Scaling<ix2,XT2> invii(RT(1)/m2.cref(i,i));
                ScaleV_Helper<-4,rs,ix2,T2,M1r>::call(invii,m1i);
            }
        }
    };

    // algo 13: UpperTri loop over k
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<13,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
#ifdef PRINTALGO_DIVU
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 13: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            const bool u2 = M2::munit;
            typedef typename M2::real_type RT;
            typedef typename M2::value_type T2;
            typedef typename Maybe<u2>::template SelectType<RT,T2>::type XT2;
            typedef typename M1::row_type M1r;
            typedef typename M1::rowrange_type M1rr;
            typedef typename M1::const_row_type M1rc;
            typedef typename M2::const_col_sub_type M2c;
            const int ix2 = u2 ? 1 : 0;
            const int xx = UNKNOWN;
            const Scaling<-1,RT> mone;

            for(int k=M;k--;) {
                // m1.row(k) /= m2(k,k)
                // m1.rowRange(0,k) -= m2.col(k,0,k) ^ m1.row(k)
                M1r m1k = m1.get_row(k);
                M1rr m1rr = m1.cRowRange(0,k);
                M2c m2k = m2.get_col(k,0,k);
                const Scaling<ix2,XT2> invkk(RT(1)/m2.cref(k,k));
                ScaleV_Helper<-4,rs,ix2,XT2,M1r>::call(invkk,m1k);
                Rank1VVM_Helper<-4,xx,rs,true,-1,RT,M2c,M1rc,M1rr>::call(
                    mone,m2k,m1k,m1rr);
            }
        }
    };

    // algo 16: For small cs, create m2.inverse() explicitly
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<16,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
#ifdef PRINTALGO_DIVU
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 16: M,N,cs,rs = "<<2<<','<<N<<
                ','<<2<<','<<rs<<std::endl;
#endif
            typedef typename M2::real_type RT;
            typedef typename M2::value_type T2;
            const DiagType D2 = M2::munit ? UnitDiag : NonUnitDiag;
            SmallUpperTriMatrix<T2,cs,D2> m2inv = m2;
            m2inv.invertSelf();
            const Scaling<1,RT> one;
            NoAliasMultMM<false>(one,m2inv,m1,m1);
        }
    };
    template <int rs, class M1, class M2>
    struct LDivEqMU_Helper<16,UNKNOWN,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const int M = m1.colsize();
#ifdef PRINTALGO_DIVU
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 16: M,N,cs,rs = "<<M<<','<<N<<
                ','<<UNKNOWN<<','<<rs<<std::endl;
#endif
            const bool rr = M1::mrowmajor && M2::mrowmajor;
            const bool rc = M1::mrowmajor && M2::mcolmajor;
            const bool cx = M1::mcolmajor;
            const int algo2 = rr ? 12 : rc ? 13 : cx ? 11 : 13;
            switch (M) {
              case 0 :
                   // do nothing
                   break;
              case 1 :
                   LDivEqMU_Helper<1,1,rs,M1,M2>::call(m1,m2);
                   break;
              case 2 :
                   LDivEqMU_Helper<16,2,rs,M1,M2>::call(m1,m2);
                   break;
              case 3 :
                   LDivEqMU_Helper<16,3,rs,M1,M2>::call(m1,m2);
                   break;
              default :
                   LDivEqMU_Helper<algo2,UNKNOWN,rs,M1,M2>::call(m1,m2);
            }
        }
    };

    // algo 17: Split the UpperTriMatrix into 3 sections and recurse
    // the calculation on each of them:
    // ( A B ) ( D ) = ( F )
    // ( 0 C ) ( E )   ( G )
    // F = AD + BE
    // G = CE
    // Solving for D,E yields:
    // E /= C
    // D -= BE
    // D /= A
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<17,cs,rs,M1,M2>
    {
        template <int which, int dummy> 
        struct Helper2;

        template <int dummy> 
        struct Helper2<0,dummy> // cs == UNKNOWN
        {
            static void call(M1& m1, const M2& m2)
            {
                const int M = m1.colsize();
#ifdef TMV_OPT_CLEANUP
                const int algo2 = 16;
#else
                const bool rr = M1::mrowmajor && M2::mrowmajor;
                const bool rc = M1::mrowmajor && M2::mcolmajor;
                const bool cx = M1::mcolmajor;
                const int algo2 = rr ? 12 : rc ? 13 : cx ? 11 : 13;
#endif

                const int xx = UNKNOWN;
                if (M < TMV_Q2) {
                    LDivEqMU_Helper<algo2,xx,rs,M1,M2>::call(m1,m2);
                } else {
                    const int Mx = M > 16 ? ((((M-1)>>5)+1)<<4) : (M>>1);
                    // (If M > 16, round M/2 up to a multiple of 16.)

                    typedef typename M2::real_type RT;
                    typedef typename M2::const_subtrimatrix_type M2a;
                    typedef typename M2::const_submatrix_type M2b;
                    typedef typename M1::const_rowrange_type M1rc;
                    typedef typename M1::rowrange_type M1r;
                    const Scaling<-1,RT> mone;

                    M2a A = m2.cSubTriMatrix(0,Mx);
                    M2b B = m2.cSubMatrix(0,Mx,Mx,M);
                    M2a C = m2.cSubTriMatrix(Mx,M);
                    M1r D = m1.cRowRange(0,Mx);
                    M1r E = m1.cRowRange(Mx,M);

                    LDivEqMU_Helper<17,xx,rs,M1r,M2a>::call(E,C);
                    MultMM_Helper<-2,xx,rs,xx,true,-1,RT,M2b,M1rc,M1r>::call(
                        mone,B,E,D);
                    LDivEqMU_Helper<17,xx,rs,M1r,M2a>::call(D,A);
                }
            }
        };
        template <int dummy> 
        struct Helper2<1,dummy> // cs < TMV_Q2
        {
            static void call(M1& m1, const M2& m2)
            {
                const bool rr = M1::mrowmajor && M2::mrowmajor;
                const bool rc = M1::mrowmajor && M2::mcolmajor;
                const bool cx = M1::mcolmajor;
                const int algo2 = 
                    cs == 0 ? 0 :
                    cs == 1 ? 1 :
                    cs <= 5 ? 16 :
                    rr ? 12 : rc ? 13 : cx ? 11 : 13;
                LDivEqMU_Helper<algo2,cs,rs,M1,M2>::call(m1,m2);
            }
        };
        template <int dummy> 
        struct Helper2<2,dummy> // cs >= TMV_Q2
        {
            static void call(M1& m1, const M2& m2)
            {
                const int M = cs;
                const int Mx = M > 16 ? ((((M-1)>>5)+1)<<4) : (M>>1);
                const int My = M - Mx;

                typedef typename M2::real_type RT;
                typedef typename M2::const_subtrimatrix_type M2a;
                typedef typename M2::const_submatrix_type M2b;
                typedef typename M1::const_rowrange_type M1rc;
                typedef typename M1::rowrange_type M1r;
                const Scaling<-1,RT> mone;

                M2a A = m2.cSubTriMatrix(0,Mx);
                M2b B = m2.cSubMatrix(0,Mx,Mx,M);
                M2a C = m2.cSubTriMatrix(Mx,M);
                M1r D = m1.cRowRange(0,Mx);
                M1r E = m1.cRowRange(Mx,M);

                LDivEqMU_Helper<17,Mx,rs,M1r,M2a>::call(E,C);
                MultMM_Helper<-2,Mx,rs,My,true,-1,RT,M2b,M1rc,M1r>::call(
                    mone,B,E,D);
                LDivEqMU_Helper<17,Mx,rs,M1r,M2a>::call(D,A);
            }
        };
        static void call(M1& m1, const M2& m2)
        {
#ifdef PRINTALGO_DIVU
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 17: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            const int cs2 = (cs >= 0 && cs <= 64) ? cs : UNKNOWN;
            const int which = cs2 == UNKNOWN ? 0 : cs2 < TMV_Q2 ? 1 : 2;
            Helper2<which,1>::call(m1,m2);
        }
    };

    // algo 21: LowerTri loop over n
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<21,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
#ifdef PRINTALGO_DIVU
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
            std::cout<<"LDivEqMU algo 21: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            typedef typename M1::col_type M1c;
            for(int j=0;j<N;++j) {
                // m1.col(j) /= m2
                M1c m1j = m1.get_col(j);
                LDivEqVU_Helper<-4,cs,M1c,M2>::call(m1j,m2);
            }
        }
    };

    // algo 22: LowerTri loop over m
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<22,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
#ifdef PRINTALGO_DIVU
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 22: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            const bool u2 = M2::munit;
            typedef typename M2::real_type RT;
            typedef typename M2::value_type T2;
            typedef typename Maybe<u2>::template SelectType<RT,T2>::type XT2;
            typedef typename M2::const_row_sub_type M2r;
            typedef typename M1::rowrange_type::const_transpose_type M1rrt;
            typedef typename M1::row_type M1r;
            const int ix2 = u2 ? 1 : 0;
            const int xx = UNKNOWN;
            const Scaling<-1,RT> mone;

            for(int i=0;i<M;++i) {
                // m1.row(i) -= m2.row(i,0,i) * m1.rowRange(0,i)
                // m1.row(i) /= m2(i,i)
                M1r m1i = m1.get_row(i);
                M2r m2i = m2.get_row(i,0,i);
                M1rrt m1rrt = m1.cRowRange(0,i).transpose();
                MultMV_Helper<-4,rs,xx,true,-1,RT,M1rrt,M2r,M1r>::call(
                    mone,m1rrt,m2i,m1i);
                const Scaling<ix2,XT2> invii(RT(1)/m2.cref(i,i));
                ScaleV_Helper<-4,rs,ix2,XT2,M1r>::call(invii,m1i);
            }
        }
    };

    // algo 23: LowerTri loop over k
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<23,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
#ifdef PRINTALGO_DIVU
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 23: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            const bool u2 = M2::munit;
            typedef typename M2::real_type RT;
            typedef typename M2::value_type T2;
            typedef typename Maybe<u2>::template SelectType<RT,T2>::type XT2;
            typedef typename M1::row_type M1r;
            typedef typename M1::const_row_type M1rc;
            typedef typename M1::rowrange_type M1rr;
            typedef typename M2::const_col_sub_type M2c;
            const int ix2 = u2 ? 1 : 0;
            const int xx = UNKNOWN;
            const Scaling<-1,RT> mone;

            for(int k=0;k<M;++k) {
                // m1.row(k) /= m2(k,k)
                // m1.rowRange(k+1,M) -= m2.col(k,k+1,M) ^ m1.row(k)
                M1r m1k = m1.get_row(k);
                M1rr m1rr = m1.cRowRange(k+1,M);
                M2c m2k = m2.get_col(k,k+1,M);
                const Scaling<ix2,XT2> invkk(RT(1)/m2.cref(k,k));
                ScaleV_Helper<-4,rs,ix2,XT2,M1r>::call(invkk,m1k);
                Rank1VVM_Helper<-4,xx,rs,true,-1,RT,M2c,M1rc,M1rr>::call(
                    mone,m2k,m1k,m1rr);
            }
        }
    };

    // algo 26: For small cs, create m2.inverse() explicitly
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<26,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
#ifdef PRINTALGO_DIVU
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 26: M,N,cs,rs = "<<cs<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            typedef typename M2::real_type RT;
            typedef typename M2::value_type T2;
            const DiagType D2 = M2::munit ? UnitDiag : NonUnitDiag;
            SmallLowerTriMatrix<T2,cs,D2> m2inv = m2;
            m2inv.invertSelf();
            const Scaling<1,RT> one;
            NoAliasMultMM<false>(one,m2inv,m1,m1);
        }
    };
    template <int rs, class M1, class M2>
    struct LDivEqMU_Helper<26,UNKNOWN,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const int M = m1.colsize();
#ifdef PRINTALGO_DIVU
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 26: M,N,cs,rs = "<<M<<','<<N<<
                ','<<UNKNOWN<<','<<rs<<std::endl;
#endif
            const bool rr = M1::mrowmajor && M2::mrowmajor;
            const bool rc = M1::mrowmajor && M2::mcolmajor;
            const bool cx = M1::mcolmajor;
            const int algo2 = rr ? 22 : rc ? 23 : cx ? 21 : 23;
            switch (M) {
              case 0 :
                   // do nothing
                   break;
              case 1 :
                   LDivEqMU_Helper<1,1,rs,M1,M2>::call(m1,m2);
                   break;
              case 2 :
                   LDivEqMU_Helper<26,2,rs,M1,M2>::call(m1,m2);
                   break;
              case 3 :
                   LDivEqMU_Helper<26,3,rs,M1,M2>::call(m1,m2);
                   break;
              default :
                   LDivEqMU_Helper<algo2,UNKNOWN,rs,M1,M2>::call(m1,m2);
            }
        }
    };

    // algo 27: Split the LowerTriMatrix into 3 sections and recurse
    // the calculation on each of them:
    // ( A 0 ) ( D ) = ( F )
    // ( B C ) ( E )   ( G )
    // G = BD + CE
    // F = AD
    // Solving for D,E yields:
    // D /= A
    // E -= BD
    // E /= C
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<27,cs,rs,M1,M2>
    {
        template <int which, int dummy> 
        struct Helper2;

        template <int dummy> 
        struct Helper2<0,dummy> // cs == UNKNOWN
        {
            static void call(M1& m1, const M2& m2)
            {
                const int M = m1.colsize();
#ifdef TMV_OPT_CLEANUP
                const int algo2 = 26;
#else
                const bool rr = M1::mrowmajor && M2::mrowmajor;
                const bool rc = M1::mrowmajor && M2::mcolmajor;
                const bool cx = M1::mcolmajor;
                const int algo2 = rr ? 22 : rc ? 23 : cx ? 21 : 23;
#endif

                const int xx = UNKNOWN;
                if (M < TMV_Q2) {
                    LDivEqMU_Helper<algo2,xx,rs,M1,M2>::call(m1,m2);
                } else {
                    const int Mx = M > 16 ? ((((M-1)>>5)+1)<<4) : (M>>1);
                    // (If M > 16, round M/2 up to a multiple of 16.)

                    typedef typename M2::real_type RT;
                    typedef typename M2::const_subtrimatrix_type M2a;
                    typedef typename M2::const_submatrix_type M2b;
                    typedef typename M1::const_rowrange_type M1rc;
                    typedef typename M1::rowrange_type M1r;
                    const Scaling<-1,RT> mone;

                    M2a A = m2.cSubTriMatrix(0,Mx);
                    M2b B = m2.cSubMatrix(Mx,M,0,Mx);
                    M2a C = m2.cSubTriMatrix(Mx,M);
                    M1r D = m1.cRowRange(0,Mx);
                    M1r E = m1.cRowRange(Mx,M);

                    LDivEqMU_Helper<27,xx,rs,M1r,M2a>::call(D,A);
                    MultMM_Helper<-2,xx,rs,xx,true,-1,RT,M2b,M1rc,M1r>::call(
                        mone,B,D,E);
                    LDivEqMU_Helper<27,xx,rs,M1r,M2a>::call(E,C);
                }
            }
        };
        template <int dummy> 
        struct Helper2<1,dummy> // cs < TMV_Q2
        {
            static void call(M1& m1, const M2& m2)
            {
                const bool rr = M1::mrowmajor && M2::mrowmajor;
                const bool rc = M1::mrowmajor && M2::mcolmajor;
                const bool cx = M1::mcolmajor;
                const int algo2 = 
                    cs == 0 ? 0 :
                    cs == 1 ? 1 :
                    cs <= 5 ? 26 :
                    rr ? 22 : rc ? 23 : cx ? 21 : 23;
                LDivEqMU_Helper<algo2,cs,rs,M1,M2>::call(m1,m2);
            }
        };
        template <int dummy> 
        struct Helper2<2,dummy> // cs >= TMV_Q2
        {
            static void call(M1& m1, const M2& m2)
            {
                const int M = cs;
                const int Mx = M > 16 ? ((((M-1)>>5)+1)<<4) : (M>>1);
                const int My = M - Mx;

                typedef typename M2::real_type RT;
                typedef typename M2::const_subtrimatrix_type M2a;
                typedef typename M2::const_submatrix_type M2b;
                typedef typename M1::const_rowrange_type M1rc;
                typedef typename M1::rowrange_type M1r;
                const Scaling<-1,RT> mone;

                M2a A = m2.cSubTriMatrix(0,Mx);
                M2b B = m2.cSubMatrix(Mx,M,0,Mx);
                M2a C = m2.cSubTriMatrix(Mx,M);
                M1r D = m1.cRowRange(0,Mx);
                M1r E = m1.cRowRange(Mx,M);

                LDivEqMU_Helper<27,Mx,rs,M1r,M2a>::call(D,A);
                MultMM_Helper<-2,My,rs,Mx,true,-1,RT,M2b,M1rc,M1r>::call(
                    mone,B,D,E);
                LDivEqMU_Helper<27,My,rs,M1r,M2a>::call(E,C);
            }
        };
        static void call(M1& m1, const M2& m2)
        {
#ifdef PRINTALGO_DIVU
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 27: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            const int cs2 = (cs >= 0 && cs <= 64) ? cs : UNKNOWN;
            const int which = cs2 == UNKNOWN ? 0 : cs2 < TMV_Q2 ? 1 : 2;
            Helper2<which,1>::call(m1,m2);
        }
    };

    // algo 31: Determine which algorithm to use based on the runtime
    // knowledge of the sizes.
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<31,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
#ifdef PRINTALGO_DIVU
            std::cout<<"LDivEqMU algo 31: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            const bool upper2 = M2::mupper;
#ifdef TMV_OPT_CLEANUP
            const int algo2 = upper2 ? 16 : 26;
#else
            const bool rr = M1::mrowmajor && M2::mrowmajor;
            const bool rc = M1::mrowmajor && M2::mcolmajor;
            const bool cx = M1::mcolmajor;
            const int algo2 = 
                upper2 ? ( rr ? 22 : rc ? 23 : cx ? 21 : 23 ) :
                ( rr ? 22 : rc ? 23 : cx ? 21 : 23 );
#endif
            const int algo3 = upper2 ? 17 : 27;

#ifdef _OPENMP
            const int Mc = M < 16 ? 1 : M>>4; // M/16
            const int Nc = N < 16 ? 1 : N>>4; // N/16
#endif

            // Put the small matrix option first, so it doesn't have to 
            // go through a bunch of if/else statements.  For large matrices,
            // all these if/else's don't matter for the total time.
            if ((M <= 5 && N < 16) || (M <= 3))
                LDivEqMU_Helper<algo2,cs,rs,M1,M2>::call(m1,m2);
#ifdef _OPENMP
            else if (!omp_in_parallel() && N >= 64 && Mc*Mc*Nc > TMV_Q6)
                LDivEqMU_Helper<36,cs,rs,M1,M2>::call(m1,m2);
#endif
            else
                LDivEqMU_Helper<algo3,cs,rs,M1,M2>::call(m1,m2);
        }
    };

#ifdef _OPENMP
    // algo 36: Split problem into smaller parts with OpenMP for 
    // parallelization.
    // We take a pretty simple approach here, and just split up m2 and m1
    // matrices by columns and let each thread do a single matrix.
    // Then each thread calls algo 17 or 27 to calculate its product.
    // Also, we require that all but the last thread has a column width
    // that is a multiple of 16.  This way we get the maximum advantage from
    // our blocking structure while keeping the threads as balanced as 
    // possible.
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<36,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
#ifdef PRINTALGO_DIVU
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
            std::cout<<"LDivEqMU algo 36: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            const bool upper2 = M2::mupper;
            const int algo2 = upper2 ? 17 : 27;
            bool bad_alloc = false;
#ifdef PRINTALGO_DIVU_OMP
            std::ofstream fout("omp.out");
#endif
#pragma omp parallel
            {
                try {
                    int num_threads = omp_get_num_threads();
                    int mythread = omp_get_thread_num();
#ifdef PRINTALGO_DIVU_OMP
#pragma omp critical
                    {
                        fout<<"thread "<<mythread<<"/"<<num_threads<<std::endl;
                    }
#endif
                    if (num_threads == 1) {
#ifdef PRINTALGO_DIVU_OMP
#pragma omp critical
                        {
                            fout<<"thread "<<mythread<<"/"<<num_threads;
                            fout<<"\nonly 1 thread"<<std::endl;
                        }
#endif
                        LDivEqMU_Helper<algo2,cs,rs,M1,M2>::call(m1,m2);
                    } else {
                        int Nx = N / num_threads;
                        Nx = ((((Nx-1)>>4)+1)<<4); 
                        int j1 = mythread * Nx;
                        int j2 = (mythread+1) * Nx;
                        if (j2 > N || mythread == num_threads-1) j2 = N;
#ifdef PRINTALGO_DIVU_OMP
#pragma omp critical
                        {
                            fout<<"thread "<<mythread<<"/"<<num_threads;
                            fout<<"Nx = "<<Nx<<std::endl;
                            fout<<"j1 = "<<j1<<std::endl;
                            fout<<"j2 = "<<j2<<std::endl;
                        }
#endif
                        if (j1 < N)  {
                            typedef typename M1::colrange_type M1c;
                            const int rsx = UNKNOWN; 
                            M1c m1c = m1.cColRange(j1,j2);
#ifdef PRINTALGO_DIVU_OMP
#pragma omp critical
                            {
                                fout<<"thread "<<mythread<<"/"<<num_threads;
                                fout<<"\nm1 = "<<m1<<std::endl;
                                fout<<"m2c = "<<m2c<<std::endl;
                                fout<<"m1c = "<<m1c<<std::endl;
                            }
#endif
                            LDivEqMU_Helper<algo2,cs,rsx,M1c,M2>::call(m1c,m2);
#ifdef PRINTALGO_DIVU_OMP
#pragma omp critical
                            {
                                fout<<"thread "<<mythread<<"/"<<num_threads;
                                fout<<"\nm1c => "<<m1c<<std::endl;
                            }
#endif
                        }
                    }
                } catch (...) {
                    // should only be std::bad_alloc, but it's good form to 
                    // catch everything inside a parallel block
                    bad_alloc = true;
                }
            }
            if (bad_alloc)
                LDivEqMU_Helper<algo2,cs,rs,M1,M2>::call(m1,m2);
        }
    };
#endif

    // algo 38: Unknown cs, check if M is small
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<38,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
#ifdef TMV_OPT_SMALL
            TMVStaticAssert(cs == UNKNOWN);
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
#ifdef PRINTALGO_DIVU
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 38: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            const bool upper2 = M2::mupper;
            const int algo2 = upper2 ? 16 : 26;

            if (M <= 5) {
                // then it is worth figuring out what M is.
                switch (M) {
                  case 0 :
                       // do nothing
                       break;
                  case 1 :
                       LDivEqMU_Helper<1,1,rs,M1,M2>::call(m1,m2);
                       break;
                  case 2 :
                       LDivEqMU_Helper<algo2,2,rs,M1,M2>::call(m1,m2);
                       break;
                  case 3 :
                       LDivEqMU_Helper<algo2,3,rs,M1,M2>::call(m1,m2);
                       break;
                  case 4 :
                       LDivEqMU_Helper<algo2,4,rs,M1,M2>::call(m1,m2);
                       break;
                  case 5 :
                       LDivEqMU_Helper<algo2,5,rs,M1,M2>::call(m1,m2);
                }
            } else 
#endif
                LDivEqMU_Helper<31,cs,rs,M1,M2>::call(m1,m2);
        }
    };

    // algo 81: copy m2
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<81,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const int M = cs == UNKNOWN ? int(m1.colsize()) : cs;
#ifdef PRINTALGO_MV_MM
            const int N = rs == UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"LDivEqMU algo 81: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            typedef typename M2::value_type T2;
            const bool rm = false; // I don't think this matters..
            const int s2 = M2::mshape;
            typedef typename MCopyHelper<T2,s2,cs,cs,rm,false>::type M2c;
            M2c m2c(M);
            typedef typename M2c::view_type M2cv;
            typedef typename M2c::const_view_type M2ccv;
            M2cv m2cv = m2c.view();
            M2ccv m2ccv = m2c.view();
            CopyU_Helper<-2,cs,M2,M2cv>::call(m2,m2cv);
            LDivEqMU_Helper<-2,cs,rs,M1,M2ccv>::call(m1,m2ccv);
        }
    };

    // algo 84: copy m1
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<84,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const int M = cs == UNKNOWN ? int(m1.colsize()) : cs;
            const int N = rs == UNKNOWN ? int(m1.rowsize()) : rs;
#ifdef PRINTALGO_DIVU
            std::cout<<"LDivEqMU algo 84: M,N,cs,rs = "<<M<<','<<N<<
                ','<<cs<<','<<rs<<std::endl;
#endif
            typedef typename M1::value_type T1;
            // I think rowmajor is usually better, since it only inverts
            // the diagonal elements of m2 once each.
            const bool rm = true; 
            typedef typename MCopyHelper<T1,Rec,cs,rs,rm,false>::type M1c;
            M1c m1c(M,N);
            typedef typename M1c::view_type M1cv;
            typedef typename M1c::const_view_type M1ccv;
            M1cv m1cv = m1c.view();
            M1ccv m1ccv = m1c.view();
            CopyM_Helper<-2,cs,rs,M1,M1cv>::call(m1,m1cv);
            LDivEqMU_Helper<-2,cs,rs,M1cv,M2>::call(m1cv,m2);
            CopyM_Helper<-2,cs,rs,M1,M1ccv>::call(m1ccv,m1);
        }
    };

    // algo -4: No branches or copies
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<-4,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const bool upper2 = M2::mupper;
            const int algo = 
                ( cs == 0 || rs == 0 ) ? 0 :
                cs == 1 ? 1 :
                rs == 1 ? 2 :
                ( cs != UNKNOWN ) ? (
                    upper2 ? (cs <= 5 ? 16 : 17 ) :
                    (cs <= 5 ? 26 : 27 ) ) :
                upper2 ? 17 : 27;
            LDivEqMU_Helper<algo,cs,rs,M1,M2>::call(m1,m2);
        }
    };

    // algo -3: Determine which algorithm to use
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<-3,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            // Possible algorithms are:
            //
            // Trivial and special for small TriMatrix sizes.
            //  0 = cs or rs == 0, so nothing to do
            //  1 = cs == 1: reduces to trivial MultXV function
            //  2 = rs == 1: reduces to trivial MultMV function
            //
            // UpperTri:
            // 11 = loop over n: MultUV
            // 12 = loop over m: MultMV
            // 13 = loop over k: Rank1 
            // 16 = specialization for cs = 2,3,4,5
            // 17 = split trimatrix into 3 submatrices
            //
            // LowerTri:
            // 21 = loop over n: MultUV
            // 22 = loop over m: MultMV
            // 23 = loop over k: Rank1
            // 26 = specialization for cs = 2,3,4,5
            // 27 = split trimatrix into 3 submatrices
            // 
            // Overall drivers:
            // 31 = Choose algorithm based on the (runtime) size
            // 36 = Parallelize using openmp 
            // 38 = Check if M is small

            const bool upper2 = M2::mupper;
#if 1
            const int algo = upper2 ? 17 : 27;
#else
#if TMV_OPT == 0 
            const bool rr = M1::mrowmajor && M2::mrowmajor;
            const bool rc = M1::mrowmajor && M2::mcolmajor;
            const bool cx = M1::mcolmajor;
            const int algo = 
                upper2 ? 
                ( rr ? 12 : rc ? 13 : cx ? 11 : 13 ) :
                ( rr ? 22 : rc ? 23 : cx ? 21 : 23 );
#else
#ifdef _OPENMP
            const int Mc = cs == UNKNOWN ? UNKNOWN : (cs < 16) ? 1 : (cs>>4);
            const int Nc = rs == UNKNOWN ? UNKNOWN : (rs < 16) ? 1 : (rs>>4);
            const int McMcNc = IntTraits2<IntTraits2<Mc,Mc>::prod,Nc>::prod;
#endif
            const int algo = 
                ( cs == 0 || rs == 0 ) ? 0 :
                cs == 1 ? 1 :
                rs == 1 ? 2 :
                upper2 ? (
                    cs == UNKNOWN ? 38 : 
                    cs <= 5 ? 16 :
                    rs == UNKNOWN ? 31 :
#ifdef _OPENMP
                    (rs >= 64 && McMcNc >= TMV_Q6) ? 36 :
#endif
                    17 ) :
                ( // lowertri
                    cs == UNKNOWN ? 38 : 
                    cs <= 5 ? 26 :
                    rs == UNKNOWN ? 31 :
#ifdef _OPENMP
                    (rs >= 64 && McMcNc >= TMV_Q6) ? 36 :
#endif
                    27 );
#endif
#endif
#ifdef PRINTALGO_DIVU
            const int M = cs==UNKNOWN ? int(m1.colsize()) : cs;
            const int N = rs==UNKNOWN ? int(m1.rowsize()) : rs;
            std::cout<<"InlineLDivEqMU: \n";
            std::cout<<"m1 = "<<TMV_Text(m1)<<std::endl;
            std::cout<<"m2 = "<<TMV_Text(m2)<<std::endl;
            std::cout<<"M = "<<M<<"  N = "<<N<<std::endl;
            std::cout<<"cs = "<<cs<<"  rs = "<<rs<<std::endl;
            std::cout<<"algo = "<<algo<<std::endl;
#endif
            LDivEqMU_Helper<algo,cs,rs,M1,M2>::call(m1,m2);
        }
    };

    // algo 97: Conjugate
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<97,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        { 
            typedef typename M1::conjugate_type M1c;
            typedef typename M2::const_conjugate_type M2c;
            M1c m1c = m1.conjugate();
            M2c m2c = m2.conjugate();
            LDivEqMU_Helper<-2,cs,rs,M1c,M2c>::call(m1c,m2c);
        }
    };

    // algo 98: call InstMultMM
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<98,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        { InstLDivEq(m1.xView(),m2.xdView()); }
    };

    // algo -2: Check for inst
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<-2,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            typedef typename M1::value_type T1;
            typedef typename M2::value_type T2;
            const bool inst = 
                M1::mcolsize == UNKNOWN && M1::mrowsize == UNKNOWN &&
                M2::msize == UNKNOWN &&
#ifdef TMV_INST_MIX
                Traits2<T1,T2>::samebase &&
#else
                Traits2<T1,T2>::sametype &&
#endif
                Traits<T1>::isinst;
            const int algo = 
                ( cs == 0 || rs == 0 ) ? 0 :
                cs == 1 ? 1 :
                rs == 1 ? 2 :
                M1::mconj ? 97 :
                inst ? 98 : 
                -3;
            LDivEqMU_Helper<algo,cs,rs,M1,M2>::call(m1,m2);
        }
    };

    // algo 99: Check for aliases
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<99,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            if (!SameStorage(m1,m2)) {
                // No aliasing 
                LDivEqMU_Helper<-2,cs,rs,M1,M2>::call(m1,m2);
            } else if (m1.colsize() <= 2*m1.rowsize()) {
                // copy m2
                LDivEqMU_Helper<81,cs,rs,M1,M2>::call(m1,m2);
            } else { 
                // Use temporary for m1
                LDivEqMU_Helper<84,cs,rs,M1,M2>::call(m1,m2);
            }
        }
    };

    // algo -1: Check for aliases?
    template <int cs, int rs, class M1, class M2>
    struct LDivEqMU_Helper<-1,cs,rs,M1,M2>
    {
        static void call(M1& m1, const M2& m2)
        {
            const bool checkalias =
                M1::mcolsize == UNKNOWN && M1::mrowsize == UNKNOWN &&
                M2::msize == UNKNOWN;
            const int algo = 
                ( cs == 0 || rs == 0 ) ? 0 :
                cs == 1 ? 1 :
                rs == 1 ? 2 :
                checkalias ? 99 : 
                -2;
            LDivEqMU_Helper<algo,cs,rs,M1,M2>::call(m1,m2);
        }
    };

    template <int algo, class M1, class M2>
    inline void DoLDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const BaseMatrix_Tri<M2>& m2)
    {
        TMVStaticAssert((Sizes<M2::msize,M1::mcolsize>::same));
        TMVAssert(m2.size() == m1.colsize());

        const int cs = Sizes<M1::mcolsize,M2::msize>::size;
        const int rs = M1::mrowsize;
        typedef typename M1::cview_type M1v;
        typedef typename M2::const_cview_type M2v;
        M1v m1v = m1.cView();
        M2v m2v = m2.cView();
        if (m2.isSingular()) {
#ifdef TMV_NO_THROW
            std::cerr<<"Singular TriMatrix found in Division\n";
            exit(1);
#else
            throw SingularMatrix<M2>(m2.mat());
#endif
        }
        LDivEqMU_Helper<algo,cs,rs,M1v,M2v>::call(m1v,m2v);
    }
    template <class M1, class M2>
    inline void LDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const BaseMatrix_Tri<M2>& m2)
    { DoLDivEq<-1>(m1,m2); }
    template <class M1, class M2>
    inline void NoAliasLDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const BaseMatrix_Tri<M2>& m2)
    { DoLDivEq<-2>(m1,m2); }
    template <class M1, class M2>
    inline void InlineLDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const BaseMatrix_Tri<M2>& m2)
    { DoLDivEq<-3>(m1,m2); }
    template <class M1, class M2>
    inline void AliasLDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const BaseMatrix_Tri<M2>& m2)
    { DoLDivEq<99>(m1,m2); }

    //
    // m3 = m1 / m2
    //

    template <int algo, int ix, class T, class M1, class M2, class M3>
    struct LDivMU_Helper;

    // algo -2: NoAlias: Move along to LDivEq
    template <int ix, class T, class M1, class M2, class M3>
    struct LDivMU_Helper<-2,ix,T,M1,M2,M3>
    {
        static void call(
            const Scaling<ix,T>& x, const M1& m1, const M2& m2, M3& m3)
        {
            NoAliasMultXM<false>(x,m1,m3);
            NoAliasLDivEq(m3,m2);
        }
    };

    // algo 99: Check for aliases
    template <int ix, class T, class M1, class M2, class M3>
    struct LDivMU_Helper<99,ix,T,M1,M2,M3>
    {
        static void call(
            const Scaling<ix,T>& x, const M1& m1, const M2& m2, M3& m3)
        {
            if ( !SameStorage(m2,m3) ) {
                AliasMultXM<false>(x,m1,m3);
                NoAliasLDivEq(m3,m2);
            } else {
                typename M3::copy_type m3c(m3.colsize(),m3.rowsize());
                NoAliasMultXM<false>(x,m1,m3c);
                NoAliasLDivEq(m3c,m2);
                NoAliasCopy(m3c,m3);
            }
        }
    };

    // algo -1: Check for aliases?
    template <int ix, class T, class M1, class M2, class M3>
    struct LDivMU_Helper<-1,ix,T,M1,M2,M3>
    {
        static void call(
            const Scaling<ix,T>& x, const M1& m1, const M2& m2, M3& m3)
        {
            const bool checkalias =
                M1::mcolsize == UNKNOWN && M1::mrowsize == UNKNOWN &&
                M2::msize == UNKNOWN &&
                M3::mcolsize == UNKNOWN && M3::mrowsize == UNKNOWN;
            const int algo =
                checkalias ? 99 :
                -2;
            LDivMU_Helper<algo,ix,T,M1,M2,M3>::call(x,m1,m2,m3);
        }
    };

    template <int algo, int ix, class T, class M1, class M2, class M3>
    inline void DoLDiv(
        const Scaling<ix,T>& x, 
        const BaseMatrix_Rec<M1>& m1, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    {
        TMVStaticAssert((Sizes<M1::mrowsize,M3::mrowsize>::same));
        TMVStaticAssert((Sizes<M1::mcolsize,M2::msize>::same));
        TMVStaticAssert((Sizes<M2::msize,M3::mcolsize>::same));
        TMVAssert(m1.colsize() == m3.colsize());
        TMVAssert(m1.rowsize() == m2.size());
        TMVAssert(m3.rowsize() == m2.size());
        typedef typename M1::const_cview_type M1v;
        typedef typename M2::const_cview_type M2v;
        typedef typename M3::cview_type M3v;
        M1v m1v = m1.cView();
        M2v m2v = m2.cView();
        M3v m3v = m3.cView();
        LDivMU_Helper<algo,ix,T,M1v,M2v,M3v>::call(x,m1v,m2v,m3v);
    }
    template <int ix, class T, class M1, class M2, class M3>
    inline void LDiv(
        const Scaling<ix,T>& x, 
        const BaseMatrix_Rec<M1>& m1, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    { DoLDiv<-1>(x,m1,m2,m3); }
    template <int ix, class T, class M1, class M2, class M3>
    inline void NoAliasLDiv(
        const Scaling<ix,T>& x, 
        const BaseMatrix_Rec<M1>& m1, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    { DoLDiv<-2>(x,m1,m2,m3); }
    template <int ix, class T, class M1, class M2, class M3>
    inline void AliasLDiv(
        const Scaling<ix,T>& x, 
        const BaseMatrix_Rec<M1>& m1, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    { DoLDiv<99>(x,m1,m2,m3); }

    //
    // m1 %= m2
    //

    template <class M1, class M2>
    inline void RDivEq(
        BaseMatrix_Mutable<M1>& m1, const BaseMatrix_Tri<M2>& m2)
    { 
        typename M1::transpose_type m1t = m1.transpose();
        LDivEq(m1t,m2.transpose()); 
    }
    template <class M1, class M2>
    inline void NoAliasRDivEq(
        BaseMatrix_Mutable<M1>& m1, const BaseMatrix_Tri<M2>& m2)
    { 
        typename M1::transpose_type m1t = m1.transpose();
        NoAliasLDivEq(m1t,m2.transpose()); 
    }
    template <class M1, class M2>
    inline void AliasRDivEq(
        BaseMatrix_Mutable<M1>& m1, const BaseMatrix_Tri<M2>& m2)
    {
        typename M1::transpose_type m1t = m1.transpose();
        AliasLDivEq(m1t,m2.transpose()); 
    }

    //
    // m3 = m1 % m2
    //

    template <int ix, class T, class M1, class M2, class M3>
    inline void RDiv(
        const Scaling<ix,T>& x,
        const BaseMatrix_Calc<M1>& m1,
        const BaseMatrix_Tri<M2>& m2, BaseMatrix_Mutable<M3>& m3)
    { 
        typename M3::transpose_type m3t = m3.transpose();
        LDiv(x,m1.transpose(),m2.transpose(),m3t); 
    }
    template <int ix, class T, class M1, class M2, class M3>
    inline void NoAliasRDiv(
        const Scaling<ix,T>& x,
        const BaseMatrix_Calc<M1>& m1,
        const BaseMatrix_Tri<M2>& m2, BaseMatrix_Mutable<M3>& m3)
    { 
        typename M3::transpose_type m3t = m3.transpose();
        NoAliasLDiv(x,m1.transpose(),m2.transpose(),m3t); 
    }
    template <int ix, class T, class M1, class M2, class M3>
    inline void AliasRDiv(
        const Scaling<ix,T>& x,
        const BaseMatrix_Calc<M1>& m1,
        const BaseMatrix_Tri<M2>& m2, BaseMatrix_Mutable<M3>& m3)
    {
        typename M3::transpose_type m3t = m3.transpose();
        AliasLDiv(x,m1.transpose(),m2.transpose(),m3t); 
    }

#undef TMV_OPT_SMALL
#undef TMV_OPT_CLEANUP
#undef TMV_Q2
#undef TMV_Q6

} // namespace tmv

#endif 