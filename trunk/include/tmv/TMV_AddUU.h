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


#ifndef TMV_AddUU_H
#define TMV_AddUU_H

#include "TMV_BaseMatrix_Tri.h"

namespace tmv {

    //
    // U = x * U + x * U
    //

    // Q1 is the maximum nops to unroll.
#if TMV_OPT >= 3
#define TMV_Q1 200 
#elif TMV_OPT >= 2
#define TMV_Q1 25
#elif TMV_OPT >= 1
#define TMV_Q1 9
#else
#define TMV_Q1 0
#endif

    template <int algo, int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper;

    // algo 1: m1 and/or m2 is unitdiag
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<1,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            typedef typename M1::const_offdiag_type M1o;
            typedef typename M2::const_offdiag_type M2o;
            typedef typename M3::offdiag_type M3o;
            M1o m1o = m1.offDiag();
            M2o m2o = m2.offDiag();
            M3o m3o = m3.offDiag();
            const int sm1 = IntTraits2<s,-1>::sum;
            AddUU_Helper<-1,sm1,ix1,T1,M1o,ix2,T2,M2o,M3o>::call(
                x1,m1o,x2,m2o,m3o);
            if (m1.isunit()) m3.diag().setAllTo(T1(x1));
            else m3.diag() = x1*m1.diag();
            if (m2.isunit()) m3.diag().addToAll(T2(x2));
            else m3.diag() += x2*m2.diag();
        }
    };

    // algo 2: Loop over rows
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<2,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            int N = (s == UNKNOWN ? m2.size() : s);
            typedef typename M1::const_row_sub_type M1r;
            typedef typename M2::const_row_sub_type M2r;
            typedef typename M3::row_sub_type M3r;
            typedef typename M1r::const_nonconj_type::const_iterator IT1;
            typedef typename M2r::const_nonconj_type::const_iterator IT2;
            typedef typename M3r::iterator IT3;
            const int step1 = m1.diagstep();
            const int step2 = m2.diagstep();
            const int step3 = m3.diagstep();
            IT1 it1 = m1.get_row(0,0,N).nonConj().begin();
            IT2 it2 = m2.get_row(0,0,N).nonConj().begin();
            IT3 it3 = m3.get_row(0,0,N).begin();
            for(;N;--N) {
                AddVV_Helper<-3,UNKNOWN,ix1,T1,M1r,ix2,T2,M2r,M3r>::call2(
                    N,x1,it1,x2,it2,it3);
                it1.shiftP(step1);
                it2.shiftP(step2);
                it3.shiftP(step3);
            }
        }
    };

    // algo 3: Loop over columns
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<3,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            int N = (s == UNKNOWN ? m2.size() : s);
            typedef typename M1::const_col_sub_type M1c;
            typedef typename M2::const_col_sub_type M2c;
            typedef typename M3::col_sub_type M3c;
            typedef typename M1c::const_nonconj_type::const_iterator IT1;
            typedef typename M2c::const_nonconj_type::const_iterator IT2;
            typedef typename M3c::iterator IT3;
            const int step1 = m1.stepj();
            const int step2 = m2.stepj();
            const int step3 = m3.stepj();
            IT1 it1 = m1.get_col(0,0,1).nonConj().begin();
            IT2 it2 = m2.get_col(0,0,1).nonConj().begin();
            IT3 it3 = m3.get_col(0,0,1).begin();
            int M=1;
            for(;N;--N) {
                AddVV_Helper<-3,UNKNOWN,ix1,T1,M1c,ix2,T2,M2c,M3c>::call2(
                    M++,x1,it1,x2,it2,it3);
                it1.shiftP(step1);
                it2.shiftP(step2);
                it3.shiftP(step3);
            }
        }
    };

    // algo 5: Fully unroll by rows
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<5,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        template <int I, int M, int J, int N>
        struct Unroller
        {
            static inline void unroll(
                const Scaling<ix1,T1>& x1, const M1& m1, 
                const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
            {
                Unroller<I,M/2,J,N>::unroll(x1,m1,x2,m2,m3);
                Unroller<I+M/2,M-M/2,J+M/2,N-M/2>::unroll(x1,m1,x2,m2,m3);
            }
        };
        template <int I, int J, int N>
        struct Unroller<I,1,J,N>
        {
            static inline void unroll(
                const Scaling<ix1,T1>& x1, const M1& m1, 
                const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
            {
                Unroller<I,1,J,N/2>::unroll(x1,m1,x2,m2,m3);
                Unroller<I,1,J+N/2,N-N/2>::unroll(x1,m1,x2,m2,m3);
            }
        };
        template <int I, int J, int N>
        struct Unroller<I,0,J,N>
        {
            static inline void unroll(
                const Scaling<ix1,T1>& x1, const M1& m1, 
                const Scaling<ix2,T2>& x2, const M2& m2, M3& m3) {}
        };
        template <int I, int J>
        struct Unroller<I,1,J,1>
        {
            static inline void unroll(
                const Scaling<ix1,T1>& x1, const M1& m1, 
                const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
            { m3.ref(I,J) = x1 * m1.cref(I,J) + x2 * m2.cref(I,J); }
        };
        template <int I, int J>
        struct Unroller<I,1,J,0>
        {
            static inline void unroll(
                const Scaling<ix1,T1>& x1, const M1& m1, 
                const Scaling<ix2,T2>& x2, const M2& m2, M3& m3) {}
        };
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        { Unroller<0,s,0,s>::unroll(x1,m1,x2,m2,m3); }
    };

    // algo 6: Fully unroll by columns
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<6,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        template <int I, int M, int J, int N>
        struct Unroller
        {
            static inline void unroll(
                const Scaling<ix1,T1>& x1, const M1& m1, 
                const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
            {
                Unroller<I,M-(N-N/2),J,N/2>::unroll(x1,m1,x2,m2,m3);
                Unroller<I,M,J+N/2,N-N/2>::unroll(x1,m1,x2,m2,m3);
            }
        };
        template <int I, int M, int J>
        struct Unroller<I,M,J,1>
        {
            static inline void unroll(
                const Scaling<ix1,T1>& x1, const M1& m1, 
                const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
            {
                Unroller<I,M/2,J,1>::unroll(x1,m1,x2,m2,m3);
                Unroller<I+M/2,M-M/2,J,1>::unroll(x1,m1,x2,m2,m3);
            }
        };
        template <int I, int M, int J>
        struct Unroller<I,M,J,0>
        {
            static inline void unroll(
                const Scaling<ix1,T1>& x1, const M1& m1, 
                const Scaling<ix2,T2>& x2, const M2& m2, M3& m3) {}
        };
        template <int I, int J>
        struct Unroller<I,1,J,1>
        {
            static inline void unroll(
                const Scaling<ix1,T1>& x1, const M1& m1, 
                const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
            { m3.ref(I,J) = x1 * m1.cref(I,J) + x2 * m2.cref(I,J); }
        };
        template <int I, int J>
        struct Unroller<I,0,J,1>
        {
            static inline void unroll(
                const Scaling<ix1,T1>& x1, const M1& m1, 
                const Scaling<ix2,T2>& x2, const M2& m2, M3& m3) {}
        };
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        { Unroller<0,s,0,s>::unroll(x1,m1,x2,m2,m3); }
    };

    // algo 10: LowerTri, transpose:
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<10,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            typedef typename M1::const_transpose_type M1t;
            typedef typename M2::const_transpose_type M2t;
            typedef typename M3::transpose_type M3t;
            M1t m1t = m1.transpose();
            M2t m2t = m2.transpose();
            M3t m3t = m3.transpose();
            AddUU_Helper<-1,s,ix1,T1,M1t,ix2,T2,M2t,M3t>::call(
                x1,m1t,x2,m2t,m3t);
        }
    };

    // algo 90: UnknownDiag
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<90,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const int algo2 = 
                ( (M1::mrowmajor && M2::mrowmajor) ||
                  (M1::mrowmajor && M3::mrowmajor) ||
                  (M2::mrowmajor && M3::mrowmajor) ) ? 2 : 3;
            if (m1.isunit() || m2.isunit())
                AddUU_Helper<1,s,ix1,T1,M1,ix2,T2,M2,M3>::call(
                    x1,m1,x2,m2,m3);
            else
                AddUU_Helper<algo2,s,ix1,T1,M1,ix2,T2,M2,M3>::call(
                    x1,m1,x2,m2,m3);
        }
    };

    // algo -3: Determine which algorithm to use
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<-3,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            typedef typename M3::value_type T3;
            const int s2 = s > 20 ? UNKNOWN : s;
            const int s2p1 = IntTraits<s2>::Sp1;
            // nops = n(n+1)
            const int nops = IntTraits2<s2,s2p1>::prod;
            const bool unroll = 
                s == UNKNOWN ? false :
                nops > TMV_Q1 ? false :
                s <= 10;
            const int algo = 
                M3::mlower ? 10 :
                ( M1::munit || M2::munit ) ? 1 :
                unroll ? (
                    ( (M1::mrowmajor && M2::mrowmajor) ||
                      (M1::mrowmajor && M3::mrowmajor) ||
                      (M2::mrowmajor && M3::mrowmajor) ) ? 5 : 6 ) :
                (M1::munknowndiag || M2::munknowndiag) ? 90 :
                ( (M1::mrowmajor && M2::mrowmajor) ||
                  (M1::mrowmajor && M3::mrowmajor) ||
                  (M2::mrowmajor && M3::mrowmajor) ) ? 2 : 3;
            AddUU_Helper<algo,s,ix1,T1,M1,ix2,T2,M2,M3>::call(
                x1,m1,x2,m2,m3);
        }
    };

    // algo 97: Call inst
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<97,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            NoAliasMultXM<false>(x1,m1,m3);
            NoAliasMultXM<true>(x2,m2,m3);
        }
    };

    // algo -2: Check for inst
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<-2,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            typedef typename M1::value_type TM1;
            typedef typename M2::value_type TM2;
            typedef typename M3::value_type TM3;
            const bool inst =
                M1::msize == UNKNOWN &&
                M2::msize == UNKNOWN &&
                M3::msize == UNKNOWN &&
#ifdef TMV_INST_MIX
                Traits2<TM1,TM2>::samebase &&
                Traits2<TM1,TM3>::samebase &&
#else
                Traits2<TM1,TM2>::sametype &&
                Traits2<TM1,TM3>::sametype &&
#endif
                Traits<TM3>::isinst;
            const int algo = 
                inst ? 97 :
                -3;
            AddUU_Helper<algo,s,ix1,T1,M1,ix2,T2,M2,M3>::call(x1,m1,x2,m2,m3);
        }
    };

    // algo 98: Check for aliases when calling Inst functions
    // We don't have a separate Inst function for this.  We just
    // split the operation into two parts: 
    // m3 = x1*m1; 
    // m3 += x2*m2;
    // and let those operations call their Inst functions.
    // However, the alias requirements for this are different than the
    // normal ones, so we need to put some alias checking here.
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<98,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const bool s1 = 
                SameStorage(m1,m3) &&
                !OppositeStorage(m1,m3);
            const bool s2 = 
                SameStorage(m2,m3) &&
                !OppositeStorage(m2,m3);

            if (!s1 && !s2) {
                // No aliasing
                NoAliasMultXM<false>(x1,m1,m3);
                NoAliasMultXM<true>(x2,m2,m3);
            } else if (!s2) { 
                // Alias with m1 only, do m1 first
                AliasMultXM<false>(x1,m1,m3);
                NoAliasMultXM<true>(x2,m2,m3);
            } else if (!s1) { 
                // Alias with m2 only, do m2 first
                AliasMultXM<false>(x2,m2,m3);
                NoAliasMultXM<true>(x1,m1,m3);
            } else { 
                // Need a temporary
                typename M1::copy_type m1c = m1;
                AliasMultXM<false>(x2,m2,m3);
                NoAliasMultXM<true>(x1,m1c,m3);
            }
        }
    };

    // algo 99: Check for aliases when not calling Inst functions
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<99,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const bool ss1 = SameStorage(m1,m3);
            const bool ss2 = SameStorage(m2,m3);
            const bool s1 = ss1 &&
                !ExactSameStorage(m1,m3) &&
                !OppositeStorage(m1,m3);
            const bool s2 = ss2 &&
                !ExactSameStorage(m2,m3) &&
                !OppositeStorage(m2,m3);

            if (!s1 && !s2) {
                // No aliasing (or no clobbering)
                AddUU_Helper<-3,s,ix1,T1,M1,ix2,T2,M2,M3>::call(
                    x1,m1,x2,m2,m3);
            } else if (!ss2) { 
                // Alias with m1 only, do m1 first
                AliasMultXM<false>(x1,m1,m3);
                NoAliasMultXM<true>(x2,m2,m3);
            } else if (!ss1) { 
                // Alias with m2 only, do m2 first
                AliasMultXM<false>(x2,m2,m3);
                NoAliasMultXM<true>(x1,m1,m3);
            } else { 
                // Need a temporary
                typename M1::copy_type m1c = m1;
                AliasMultXM<false>(x2,m2,m3);
                NoAliasMultXM<true>(x1,m1c,m3);
            }
        }
    };

    // algo -1: Check for aliases?
    template <int s, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUU_Helper<-1,s,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const bool noclobber = 
                (MStepHelper<M1,M3>::same || MStepHelper<M1,M3>::opp) &&
                (MStepHelper<M2,M3>::same || MStepHelper<M2,M3>::opp);
            typedef typename M1::value_type TM1;
            typedef typename M2::value_type TM2;
            typedef typename M3::value_type TM3;
            const bool inst =
                M1::msize == UNKNOWN &&
                M2::msize == UNKNOWN &&
                M3::msize == UNKNOWN &&
#ifdef TMV_INST_MIX
                Traits2<TM1,TM2>::samebase &&
                Traits2<TM1,TM3>::samebase &&
#else
                Traits2<TM1,TM2>::sametype &&
                Traits2<TM1,TM3>::sametype &&
#endif
                Traits<TM3>::isinst;
            const bool checkalias =
                M1::msize == UNKNOWN &&
                M2::msize == UNKNOWN &&
                M3::msize == UNKNOWN &&
                !noclobber;
            const int algo = 
                // We do a different check alias with the Inst calls.
                inst ? 98 :
                checkalias ? 99 : 
                -3;
            AddUU_Helper<algo,s,ix1,T1,M1,ix2,T2,M2,M3>::call(x1,m1,x2,m2,m3);
        }
    };

    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void AddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Tri<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Tri_Mutable<M3>& m3)
    {
        TMVStaticAssert(M1::mupper == int(M3::mupper));
        TMVStaticAssert(M2::mupper == int(M3::mupper));
        TMVStaticAssert((Sizes<M1::msize,M2::msize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::msize>::same));
        TMVStaticAssert(!M3::munit);
        TMVAssert(m1.size() == m2.size());
        TMVAssert(m1.size() == m3.size());
        TMVAssert(!m3.isunit());
        const int s = Sizes<Sizes<M1::msize,M2::msize>::size,M3::msize>::size;
        typedef typename M1::const_cview_type M1v;
        typedef typename M2::const_cview_type M2v;
        typedef typename M3::cview_type M3v;
        M1v m1v = m1.cView();
        M2v m2v = m2.cView();
        M3v m3v = m3.cView();
        AddUU_Helper<-1,s,ix1,T1,M1v,ix2,T2,M2v,M3v>::call(x1,m1v,x2,m2v,m3v);
    }

    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void NoAliasAddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Tri<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Tri_Mutable<M3>& m3)
    {
        TMVStaticAssert(M1::mupper == int(M3::mupper));
        TMVStaticAssert(M2::mupper == int(M3::mupper));
        TMVStaticAssert((Sizes<M1::msize,M2::msize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::msize>::same));
        TMVStaticAssert(!M3::munit);
        TMVAssert(m1.size() == m2.size());
        TMVAssert(m1.size() == m3.size());
        TMVAssert(!m3.isunit());
        const int s = Sizes<Sizes<M1::msize,M2::msize>::size,M3::msize>::size;
        typedef typename M1::const_cview_type M1v;
        typedef typename M2::const_cview_type M2v;
        typedef typename M3::cview_type M3v;
        M1v m1v = m1.cView();
        M2v m2v = m2.cView();
        M3v m3v = m3.cView();
        AddUU_Helper<-2,s,ix1,T1,M1v,ix2,T2,M2v,M3v>::call(x1,m1v,x2,m2v,m3v);
    }

    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void InlineAddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Tri<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Tri_Mutable<M3>& m3)
    {
        TMVStaticAssert(M1::mupper == int(M3::mupper));
        TMVStaticAssert(M2::mupper == int(M3::mupper));
        TMVStaticAssert((Sizes<M1::msize,M2::msize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::msize>::same));
        TMVStaticAssert(!M3::munit);
        TMVAssert(m1.size() == m2.size());
        TMVAssert(m1.size() == m3.size());
        TMVAssert(!m3.isunit());
        const int s = Sizes<Sizes<M1::msize,M2::msize>::size,M3::msize>::size;
        typedef typename M1::const_cview_type M1v;
        typedef typename M2::const_cview_type M2v;
        typedef typename M3::cview_type M3v;
        M1v m1v = m1.cView();
        M2v m2v = m2.cView();
        M3v m3v = m3.cView();
        AddUU_Helper<-3,s,ix1,T1,M1v,ix2,T2,M2v,M3v>::call(x1,m1v,x2,m2v,m3v);
    }

    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void AliasAddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Tri<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Tri_Mutable<M3>& m3)
    {
        TMVStaticAssert(M1::mupper == int(M3::mupper));
        TMVStaticAssert(M2::mupper == int(M3::mupper));
        TMVStaticAssert((Sizes<M1::msize,M2::msize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::msize>::same));
        TMVStaticAssert(!M3::munit);
        TMVAssert(m1.size() == m2.size());
        TMVAssert(m1.size() == m3.size());
        TMVAssert(!m3.isunit());
        const int s = Sizes<Sizes<M1::msize,M2::msize>::size,M3::msize>::size;
        typedef typename M1::value_type TM1;
        typedef typename M2::value_type TM2;
        typedef typename M3::value_type TM3;
        const bool inst =
            M1::msize == UNKNOWN &&
            M2::msize == UNKNOWN &&
            M3::msize == UNKNOWN &&
#ifdef TMV_INST_MIX
            Traits2<TM1,TM2>::samebase &&
            Traits2<TM1,TM3>::samebase &&
#else
            Traits2<TM1,TM2>::sametype &&
            Traits2<TM1,TM3>::sametype &&
#endif
            Traits<TM3>::isinst;
        typedef typename M1::const_cview_type M1v;
        typedef typename M2::const_cview_type M2v;
        typedef typename M3::cview_type M3v;
        M1v m1v = m1.cView();
        M2v m2v = m2.cView();
        M3v m3v = m3.cView();
        AddUU_Helper<inst?98:99,s,ix1,T1,M1v,ix2,T2,M2v,M3v>::call(
            x1,m1v,x2,m2v,m3v);
    }

    //
    // M = x * U + x * U
    //

    template <int algo, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUUM_Helper;

    // algo 1: M = x * U + x * U  (no alias checking)
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUUM_Helper<1,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            typename M3::uppertri_type m3u = m3.upperTri();
            m3.setZero();
            NoAliasAddMM(x1,m1,x2,m2,m3u);
            m3.lowerTri().offDiag().setZero();
        }
    };

    // algo 2: M = x * U + x * L  (no alias checking)
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUUM_Helper<2,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            typename M3::uppertri_type::offdiag_type m3u = 
                m3.upperTri().offDiag();
            typename M3::lowertri_type::offdiag_type m3l = 
                m3.lowerTri().offDiag();
            typename M3::diag_type m3d = m3.diag();
            NoAliasMultXM<false>(x1,m1,m3u);
            NoAliasMultXM<false>(x2,m2,m3l);
            NoAliasAddVV(x1,m1.diag(),x2,m2.diag(),m3d);
        }
    };

    // algo 3: M = x * L + x * U  (no alias checking)
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUUM_Helper<3,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            typename M3::uppertri_type::offdiag_type m3u = 
                m3.upperTri().offDiag();
            typename M3::lowertri_type::offdiag_type m3l = 
                m3.lowerTri().offDiag();
            typename M3::diag_type m3d = m3.diag();
            NoAliasMultXM<false>(x1,m1,m3l);
            NoAliasMultXM<false>(x2,m2,m3u);
            NoAliasAddVV(x1,m1.diag(),x2,m2.diag(),m3d);
        }
    };

    // algo 4: M = x * L + x * L  (no alias checking)
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUUM_Helper<4,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            typename M3::lowertri_type m3l = m3.lowerTri();
            m3.setZero();
            NoAliasAddMM(x1,m1,x2,m2,m3l);
        }
    };

    // algo 11: M = x * U + x * U  (with alias checking)
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUUM_Helper<11,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            typename M3::uppertri_type m3u = m3.upperTri();
            AliasAddMM(x1,m1,x2,m2,m3u);
            m3.lowerTri().offDiag().setZero();
        }
    };

    // algo 12: M = x * U + x * L  (with alias checking)
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUUM_Helper<12,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const bool s1 = SameStorage(m1,m3);
            const bool s2 = SameStorage(m2,m3);

            if (!s1 && !s2) {
                // No aliasing
                AddUUM_Helper<2,ix1,T1,M1,ix2,T2,M2,M3>::call(x1,m1,x2,m2,m3);
            } else if (!s2) { 
                // Alias with m1 only, do m1 first
                typename M3::uppertri_type m3u = m3.upperTri();
                typename M3::lowertri_type m3l = m3.lowerTri();
                AliasMultXM<false>(x1,m1,m3u);
                NoAliasMultXM<true>(x2,m2,m3l);
            } else if (!s1) { 
                // Alias with m2 only, do m2 first
                typename M3::uppertri_type m3u = m3.upperTri();
                typename M3::lowertri_type m3l = m3.lowerTri();
                AliasMultXM<false>(x2,m2,m3l);
                NoAliasMultXM<true>(x1,m1,m3u);
            } else if (ExactSameStorage(m1,m3) && ExactSameStorage(m2,m3)) {
                // Aliases don't conflict with each other.
                typename M3::uppertri_type::offdiag_type m3u =
                    m3.upperTri().offDiag();
                typename M3::lowertri_type::offdiag_type m3l =
                    m3.lowerTri().offDiag();
                typename M3::diag_type m3d = m3.diag();
                AliasMultXM<false>(x1,m1,m3u);
                AliasMultXM<false>(x2,m2,m3l);
                AliasAddVV(x1,m1.diag(),x2,m2.diag(),m3d);
            } else {
                // Need a temporary
                typename M3::uppertri_type m3u = m3.upperTri();
                typename M3::lowertri_type m3l = m3.lowerTri();
                typename M1::copy_type m1c = m1;
                AliasMultXM<false>(x2,m2,m3l);
                NoAliasMultXM<true>(x1,m1c,m3u);
            }
        }
    };

    // algo 13: M = x * L + x * U  (with alias checking)
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUUM_Helper<13,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const bool s1 = SameStorage(m1,m3);
            const bool s2 = SameStorage(m2,m3);

            if (!s1 && !s2) {
                // No aliasing
                AddUUM_Helper<3,ix1,T1,M1,ix2,T2,M2,M3>::call(x1,m1,x2,m2,m3);
            } else if (!s2) { 
                // Alias with m1 only, do m1 first
                typename M3::uppertri_type m3u = m3.upperTri();
                typename M3::lowertri_type m3l = m3.lowerTri();
                AliasMultXM<false>(x1,m1,m3l);
                NoAliasMultXM<true>(x2,m2,m3u);
            } else if (!s1) { 
                // Alias with m2 only, do m2 first
                typename M3::uppertri_type m3u = m3.upperTri();
                typename M3::lowertri_type m3l = m3.lowerTri();
                AliasMultXM<false>(x2,m2,m3u);
                NoAliasMultXM<true>(x1,m1,m3l);
            } else if (ExactSameStorage(m1,m3) && ExactSameStorage(m2,m3)) {
                // Aliases don't conflict with each other.
                typename M3::uppertri_type::offdiag_type m3u =
                    m3.upperTri().offDiag();
                typename M3::lowertri_type::offdiag_type m3l =
                    m3.lowerTri().offDiag();
                typename M3::diag_type m3d = m3.diag();
                AliasMultXM<false>(x1,m1,m3l);
                AliasMultXM<false>(x2,m2,m3u);
                AliasAddVV(x1,m1.diag(),x2,m2.diag(),m3d);
            } else {
                // Need a temporary
                typename M3::uppertri_type m3u = m3.upperTri();
                typename M3::lowertri_type m3l = m3.lowerTri();
                typename M1::copy_type m1c = m1;
                AliasMultXM<false>(x2,m2,m3u);
                NoAliasMultXM<true>(x1,m1c,m3l);
            }
        }
    };

    // algo 14: M = x * L + x * L  (with alias checking)
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUUM_Helper<14,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            typename M3::lowertri_type m3l = m3.lowerTri();
            AliasAddMM(x1,m1,x2,m2,m3l);
            m3.upperTri().offDiag().setZero();
        }
    };

    // algo 99: Check for aliases
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUUM_Helper<99,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const int algo = 
                (M1::mupper && M2::mupper) ? 11 :
                (M1::mupper && M2::mlower) ? 12 :
                (M1::mlower && M2::mupper) ? 13 :
                14;
            AddUUM_Helper<algo,ix1,T1,M1,ix2,T2,M2,M3>::call(x1,m1,x2,m2,m3);
        }
    };
    // algo -2: No alias checking
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUUM_Helper<-2,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const int algo = 
                (M1::mupper && M2::mupper) ? 1 :
                (M1::mupper && M2::mlower) ? 2 :
                (M1::mlower && M2::mupper) ? 3 :
                4;
            AddUUM_Helper<algo,ix1,T1,M1,ix2,T2,M2,M3>::call(x1,m1,x2,m2,m3);
        }
    };

    // algo -1: Check for aliases?
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUUM_Helper<-1,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const bool checkalias =
                M1::msize == UNKNOWN &&
                M2::msize == UNKNOWN &&
                M3::mcolsize == UNKNOWN && M3::mcolsize == UNKNOWN;
            const int algo = 
                checkalias ? 99 :
                -2;
            AddUUM_Helper<algo,ix1,T1,M1,ix2,T2,M2,M3>::call(x1,m1,x2,m2,m3);
        }
    };

    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void AddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Tri<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    {
        TMVStaticAssert((Sizes<M1::msize,M2::msize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::mcolsize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::mrowsize>::same));
        TMVAssert(m1.size() == m2.size());
        TMVAssert(m1.size() == m3.colsize());
        TMVAssert(m1.size() == m3.rowsize());
        AddUUM_Helper<-1,ix1,T1,M1,ix2,T2,M2,M3>::call(
            x1,m1.mat(),x2,m2.mat(),m3.mat());
    }

    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void NoAliasAddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Tri<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    {
        TMVStaticAssert((Sizes<M1::msize,M2::msize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::mcolsize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::mrowsize>::same));
        TMVAssert(m1.size() == m2.size());
        TMVAssert(m1.size() == m3.colsize());
        TMVAssert(m1.size() == m3.rowsize());
        AddUUM_Helper<-2,ix1,T1,M1,ix2,T2,M2,M3>::call(
            x1,m1.mat(),x2,m2.mat(),m3.mat());
    }

    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void AliasAddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Tri<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    {
        TMVStaticAssert((Sizes<M1::msize,M2::msize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::mcolsize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::mrowsize>::same));
        TMVAssert(m1.size() == m2.size());
        TMVAssert(m1.size() == m3.colsize());
        TMVAssert(m1.size() == m3.rowsize());
        AddUUM_Helper<99,ix1,T1,M1,ix2,T2,M2,M3>::call(
            x1,m1.mat(),x2,m2.mat(),m3.mat());
    }


    //
    // M = x * U + x * M
    //

    template <int algo, int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUMM_Helper;

    // algo 1: M = x * U + x * M  (no alias checking)
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUMM_Helper<1,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            typename M3::uppertri_type m3u = m3.upperTri();
            MultXM<false>(x2,m2,m3);
            MultXM<true>(x1,m1,m3u);
        }
    };

    // algo 2: M = x * L + x * M  (no alias checking)
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUMM_Helper<4,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            typename M3::lowertri_type m3l = m3.lowerTri();
            MultXM<false>(x2,m2,m3);
            MultXM<true>(x1,m1,m3l);
        }
    };

    // algo 11: M = x * U + x * M  (with alias checking)
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUMM_Helper<11,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const bool s1 = SameStorage(m1,m3);
            const bool s2 = SameStorage(m2,m3);

            if (!s1 && !s2) {
                // No aliasing
                AddUMM_Helper<1,ix1,T1,M1,ix2,T2,M2,M3>::call(x1,m1,x2,m2,m3);
            } else if (!s2) { 
                // Alias with m1 only, do m1 first
                typename M3::uppertri_type m3u = m3.upperTri();
                AliasMultXM<false>(x1,m1,m3u);
                NoAliasMultXM<true>(x2,m2,m3);
            } else if (!s1) { 
                // Alias with m2 only, do m2 first
                typename M3::uppertri_type m3u = m3.upperTri();
                AliasMultXM<false>(x2,m2,m3);
                NoAliasMultXM<true>(x1,m1,m3u);
            } else if (ExactSameStorage(m1,m3) && ExactSameStorage(m2,m3)) {
                // Aliases don't conflict with each other.
                typename M3::uppertri_type::offdiag_type m3u =
                    m3.upperTri().offDiag();
                typename M3::lowertri_type::offdiag_type m3l =
                    m3.lowerTri().offDiag();
                typename M3::diag_type m3d = m3.diag();
                AliasAddMM(x1,m1,x2,m2.upperTri().offDiag(),m3u);
                AliasMultXM<false>(x2,m2.lowerTri().offDiag(),m3l);
                AliasAddVV(x1,m1.diag(),x2,m2.diag(),m3d);
            } else {
                // Need a temporary
                typename M3::uppertri_type m3u = m3.upperTri();
                typename M1::copy_type m1c = m1;
                AliasMultXM<false>(x2,m2,m3);
                NoAliasMultXM<true>(x1,m1c,m3u);
            }
        }
    };

    // algo 12: M = x * L + x * M  (with alias checking)
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUMM_Helper<12,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const bool s1 = SameStorage(m1,m3);
            const bool s2 = SameStorage(m2,m3);

            if (!s1 && !s2) {
                // No aliasing
                AddUMM_Helper<1,ix1,T1,M1,ix2,T2,M2,M3>::call(x1,m1,x2,m2,m3);
            } else if (!s2) { 
                // Alias with m1 only, do m1 first
                typename M3::lowertri_type m3l = m3.lowerTri();
                AliasMultXM<false>(x1,m1,m3l);
                NoAliasMultXM<true>(x2,m2,m3);
            } else if (!s1) { 
                // Alias with m2 only, do m2 first
                typename M3::lowertri_type m3l = m3.lowerTri();
                AliasMultXM<false>(x2,m2,m3);
                NoAliasMultXM<true>(x1,m1,m3l);
            } else if (ExactSameStorage(m1,m3) && ExactSameStorage(m2,m3)) {
                // Aliases don't conflict with each other.
                typename M3::uppertri_type::offdiag_type m3u =
                    m3.upperTri().offDiag();
                typename M3::lowertri_type::offdiag_type m3l =
                    m3.lowerTri().offDiag();
                typename M3::diag_type m3d = m3.diag();
                AliasAddMM(x1,m1,x2,m2.lowerTri().offDiag(),m3l);
                AliasMultXM<false>(x2,m2.upperTri().offDiag(),m3u);
                AliasAddVV(x1,m1.diag(),x2,m2.diag(),m3d);
            } else {
                // Need a temporary
                typename M3::lowertri_type m3l = m3.lowerTri();
                typename M1::copy_type m1c = m1;
                AliasMultXM<false>(x2,m2,m3);
                NoAliasMultXM<true>(x1,m1c,m3l);
            }
        }
    };

    // algo 99: Check for aliases
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUMM_Helper<99,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const int algo = M1::mupper ? 11 : 12;
            AddUMM_Helper<algo,ix1,T1,M1,ix2,T2,M2,M3>::call(x1,m1,x2,m2,m3);
        }
    };
    // algo -2: No alias checking
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUMM_Helper<-2,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const int algo = M1::mupper ? 1 : 2;
            AddUMM_Helper<algo,ix1,T1,M1,ix2,T2,M2,M3>::call(x1,m1,x2,m2,m3);
        }
    };

    // algo -1: Check for aliases?
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    struct AddUMM_Helper<-1,ix1,T1,M1,ix2,T2,M2,M3>
    {
        static inline void call(
            const Scaling<ix1,T1>& x1, const M1& m1, 
            const Scaling<ix2,T2>& x2, const M2& m2, M3& m3)
        {
            const bool checkalias =
                M1::msize == UNKNOWN &&
                M2::mcolsize == UNKNOWN && M2::mrowsize == UNKNOWN &&
                M3::mcolsize == UNKNOWN && M3::mcolsize == UNKNOWN;
            const int algo = 
                checkalias ? 99 :
                -2;
            AddUMM_Helper<algo,ix1,T1,M1,ix2,T2,M2,M3>::call(x1,m1,x2,m2,m3);
        }
    };

    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void AddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Tri<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Rec<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    {
        TMVStaticAssert((Sizes<M1::msize,M2::msize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::mcolsize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::mrowsize>::same));
        TMVAssert(m1.size() == m2.size());
        TMVAssert(m1.size() == m3.colsize());
        TMVAssert(m1.size() == m3.rowsize());
        AddUMM_Helper<-1,ix1,T1,M1,ix2,T2,M2,M3>::call(
            x1,m1.mat(),x2,m2.mat(),m3.mat());
    }

    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void NoAliasAddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Tri<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Rec<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    {
        TMVStaticAssert((Sizes<M1::msize,M2::msize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::mcolsize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::mrowsize>::same));
        TMVAssert(m1.size() == m2.size());
        TMVAssert(m1.size() == m3.colsize());
        TMVAssert(m1.size() == m3.rowsize());
        AddUMM_Helper<-2,ix1,T1,M1,ix2,T2,M2,M3>::call(
            x1,m1.mat(),x2,m2.mat(),m3.mat());
    }

    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void AliasAddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Tri<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Rec<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    {
        TMVStaticAssert((Sizes<M1::msize,M2::msize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::mcolsize>::same));
        TMVStaticAssert((Sizes<M1::msize,M3::mrowsize>::same));
        TMVAssert(m1.size() == m2.size());
        TMVAssert(m1.size() == m3.colsize());
        TMVAssert(m1.size() == m3.rowsize());
        AddUMM_Helper<99,ix1,T1,M1,ix2,T2,M2,M3>::call(
            x1,m1.mat(),x2,m2.mat(),m3.mat());
    }


    //
    // M = x * M + x * U
    //

    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void AddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Rec<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    { AddMM(x2,m2,x1,m1,m3); }
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void NoAliasAddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Rec<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    { NoAliasAddMM(x2,m2,x1,m1,m3); }
    template <int ix1, class T1, class M1,
              int ix2, class T2, class M2, class M3>
    inline void AliasAddMM(
        const Scaling<ix1,T1>& x1, const BaseMatrix_Rec<M1>& m1, 
        const Scaling<ix2,T2>& x2, const BaseMatrix_Tri<M2>& m2, 
        BaseMatrix_Rec_Mutable<M3>& m3)
    { AliasAddMM(x2,m2,x1,m1,m3); }


#undef TMV_Q1

} // namespace tmv

#endif 
