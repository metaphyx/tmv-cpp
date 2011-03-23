
#ifndef TMV_MultPM_H
#define TMV_MultPM_H

#include "TMV_BaseMatrix_Rec.h"
#include "TMV_Permutation.h"
#include "TMV_MultXM_Funcs.h"

namespace tmv {

    //
    // P * m
    // TODO: break out add option at compile time rather than using a 
    //       runtime if statement.
    //

    template <bool add, int ix, class T, class M2, class M3>
    static inline void MultMM(
        const Scaling<ix,T>& x, const Permutation& m1, 
        const BaseMatrix<M2>& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    {
        if (add) {
            m3 += (x*m1*m2).calc();
        } else {
            MultXM<false>(x,m2.mat(),m3.mat());
            m1.applyOnLeft(m3);
        }
    }
    template <bool add, int ix, class T, class M2, class M3>
    static inline void NoAliasMultMM(
        const Scaling<ix,T>& x, const Permutation& m1, 
        const BaseMatrix<M2>& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    {
        if (add) {
            m3 += (x*m1*m2).calc();
        } else {
            NoAliasMultXM<false>(x,m2.mat(),m3.mat());
            m1.applyOnLeft(m3);
        }
    }
    template <bool add, int ix, class T, class M2, class M3>
    static inline void AliasMultMM(
        const Scaling<ix,T>& x, const Permutation& m1, 
        const BaseMatrix<M2>& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    {
        if (add) {
            m3 += (x*m1*m2).calc();
        } else {
            AliasMultXM<false>(x,m2.mat(),m3.mat());
            m1.applyOnLeft(m3);
        }
    }


    //
    // m * P
    //
    
    template <bool add, int ix, class T, class M1, class M3>
    static inline void MultMM(
        const Scaling<ix,T>& x, const BaseMatrix<M1>& m1, 
        const Permutation& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    {
        if (add) {
            m3 += (x*m1*m2).calc();
        } else {
            MultXM<false>(x,m1.mat(),m3.mat());
            m2.applyOnRight(m3);
        }
    }
    template <bool add, int ix, class T, class M1, class M3>
    static inline void NoAliasMultMM(
        const Scaling<ix,T>& x, const BaseMatrix<M1>& m1, 
        const Permutation& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    {
        if (add) {
            m3 += (x*m1*m2).calc();
        } else {
            NoAliasMultXM<false>(x,m1.mat(),m3.mat());
            m2.applyOnRight(m3);
        }
    }
    template <bool add, int ix, class T, class M1, class M3>
    static inline void AliasMultMM(
        const Scaling<ix,T>& x, const BaseMatrix<M1>& m1, 
        const Permutation& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    {
        if (add) {
            m3 += (x*m1*m2).calc();
        } else {
            AliasMultXM<false>(x,m1.mat(),m3.mat());
            m2.applyOnRight(m3);
        }
    }

    //
    // P * P
    //

    template <bool add, int ix, class T, class M3>
    static inline void MultMM(
        const Scaling<ix,T>& x, const Permutation& m1, 
        const Permutation& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    {
        if (add) {
            m3 += (x*m1*m2).calc();
        } else {
            m3 = x*m1*m2.calc();
        }
    }
    template <bool add, int ix, class T, class M3>
    static inline void NoAliasMultMM(
        const Scaling<ix,T>& x, const Permutation& m1, 
        const Permutation& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    { MultMM<add>(x,m1,m2,m3); }
    template <bool add, int ix, class T, class M3>
    static inline void AliasMultMM(
        const Scaling<ix,T>& x, const Permutation& m1, 
        const Permutation& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    { MultMM<add>(x,m1,m2,m3); }


    //
    // m / P
    // 

    template <int ix, class T, class M1, class M3>
    static inline void LDiv(
        const Scaling<ix,T>& x, const BaseMatrix<M1>& m1,
        const Permutation& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    { MultMM<false>(x,m2.inverse(),m1.mat(),m3.mat()); }
    template <int ix, class T, class M1, class M2, class M3>
    static inline void NoAliasLDiv(
        const Scaling<ix,T>& x, const BaseMatrix<M1>& m1,
        const Permutation& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    { NoAliasMultMM<false>(x,m2.inverse(),m1.mat(),m3.mat()); }
    template <int ix, class T, class M1, class M2, class M3>
    static inline void AliasLDiv(
        const Scaling<ix,T>& x, const BaseMatrix<M1>& m1,
        const Permutation& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    { AliasMultMM<false>(x,m2.inverse(),m1.mat(),m3.mat()); }


    //
    // m % P
    // 

    template <int ix, class T, class M1, class M3>
    static inline void RDiv(
        const Scaling<ix,T>& x, const BaseMatrix<M1>& m1,
        const Permutation& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    { MultMM<false>(x,m1.mat(),m2.inverse(),m3.mat()); }
    template <int ix, class T, class M1, class M2, class M3>
    static inline void NoAliasRDiv(
        const Scaling<ix,T>& x, const BaseMatrix<M1>& m1,
        const Permutation& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    { NoAliasMultMM<false>(x,m1.mat(),m2.inverse(),m3.mat()); }
    template <int ix, class T, class M1, class M2, class M3>
    static inline void AliasRDiv(
        const Scaling<ix,T>& x, const BaseMatrix<M1>& m1,
        const Permutation& m2, BaseMatrix_Rec_Mutable<M3>& m3)
    { AliasMultMM<false>(x,m1.mat(),m2.inverse(),m3.mat()); }


    // 
    // m *= P
    //

    template <class M1, int ix, class T>
    static inline void MultEqMM(
        BaseMatrix_Rec_Mutable<M1>& m1,
        const Scaling<ix,T>& x, const Permutation& m2)
    {
        m2.applyOnRight(m1); 
        Scale(x,m1.mat());
    }
    template <class M1, int ix, class T>
    static inline void NoAliasMultEqMM(
        BaseMatrix_Rec_Mutable<M1>& m1,
        const Scaling<ix,T>& x, const Permutation& m2)
    { MultEqMM(m1,x,m2); }
    template <class M1, int ix, class T>
    static inline void AliasMultEqMM(
        BaseMatrix_Rec_Mutable<M1>& m1,
        const Scaling<ix,T>& x, const Permutation& m2)
    { MultEqMM(m1,x,m2); }

    //
    // m /= P
    //

    template <class M1>
    static inline void LDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const Permutation& m2)
    { m2.inverse().applyOnLeft(m1); }
    template <class M1>
    static inline void NoAliasLDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const Permutation& m2)
    { LDivEq(m1,m2); }
    template <class M1>
    static inline void AliasLDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const Permutation& m2)
    { LDivEq(m1,m2); }

    //
    // m %= P
    //

    template <class M1>
    static inline void RDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const Permutation& m2)
    { m2.inverse().applyOnRight(m1); }
    template <class M1>
    static inline void NoAliasRDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const Permutation& m2)
    { RDivEq(m1,m2); }
    template <class M1>
    static inline void AliasRDivEq(
        BaseMatrix_Rec_Mutable<M1>& m1, const Permutation& m2)
    { RDivEq(m1,m2); }



} // namespace tmv

#endif
