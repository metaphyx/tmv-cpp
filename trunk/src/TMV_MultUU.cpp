//////////////////////////////////////////////////////////////////////////////
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

//#define PRINTALGO_UU

#include "tmv/TMV_MultUU.h"
#include "tmv/TMV_CopyU.h"
#include "tmv/TMV_MultXU.h"
#include "tmv/TMV_ScaleU.h"
#include "tmv/TMV_TriMatrix.h"
#include "tmv/TMV_ProdXM.h"

namespace tmv {

    template <class M1, class M2, class M3>
    void DoMultUU(const M1& m1, const M2& m2, M3& m3)
    {
        TMVAssert(m1.isrm() || m1.iscm());
        TMVAssert(m2.isrm() || m2.iscm());
        TMVAssert(m3.isrm() || m3.iscm());

        Scaling<1,typename M3::real_type> one;
        if (m3.iscm()) {
            typename M3::cmview_type m3cm = m3.cmView();
            if (m1.iscm())
                if (m2.iscm())
                    InlineMultMM<false>(one,m1.cmView(),m2.cmView(),m3cm);
                else
                    InlineMultMM<false>(one,m1.cmView(),m2.rmView(),m3cm);
            else
                if (m2.iscm())
                    InlineMultMM<false>(one,m1.rmView(),m2.cmView(),m3cm);
                else
                    InlineMultMM<false>(one,m1.rmView(),m2.rmView(),m3cm);
        } else {
            typename M3::rmview_type m3rm = m3.rmView();
            if (m1.iscm())
                if (m2.iscm())
                    InlineMultMM<false>(one,m1.cmView(),m2.cmView(),m3rm);
                else
                    InlineMultMM<false>(one,m1.cmView(),m2.rmView(),m3rm);
            else
                if (m2.iscm())
                    InlineMultMM<false>(one,m1.rmView(),m2.cmView(),m3rm);
                else
                    InlineMultMM<false>(one,m1.rmView(),m2.rmView(),m3rm);
        }
    }

    template <class T1, bool C1, class T2, bool C2, class T3>
    void InstMultMM(
        const T3 x,
        const ConstUpperTriMatrixView<T1,UnknownDiag,UNKNOWN,UNKNOWN,C1>& m1,
        const ConstUpperTriMatrixView<T2,UnknownDiag,UNKNOWN,UNKNOWN,C2>& m2,
        UpperTriMatrixView<T3,UnknownDiag> m3)
    {
        if (!(m1.isrm() || m1.iscm())) 
            InstMultMM(x,m1.copy().constView().xdView(),m2,m3);
        else if (!(m2.isrm() || m2.iscm())) 
            InstMultMM(x,m1,m2.copy().constView().xdView(),m3);
        else if (!(m3.iscm() || m3.isrm())) {
            UpperTriMatrix<T3,NonUnitDiag,ColMajor> m3c(m3.size());
            UpperTriMatrixView<T3,UnknownDiag> m3v = m3c.xdView();
            DoMultUU(m1,m2,m3v);
            if (m3.isunit()) 
                InstCopy(m3v.constView().viewAsUnitDiag().xdView(),m3);
            else
                InstMultXM(x,m3v.constView(),m3.viewAsNonUnitDiag());
        } else {
            DoMultUU(m1,m2,m3);
            if (!m3.isunit()) InstScale(x,m3.viewAsNonUnitDiag());
        }
    }

    template <class T1, bool C1, class T2, bool C2, class T3>
    void InstAddMultMM(
        const T3 x,
        const ConstUpperTriMatrixView<T1,UnknownDiag,UNKNOWN,UNKNOWN,C1>& m1,
        const ConstUpperTriMatrixView<T2,UnknownDiag,UNKNOWN,UNKNOWN,C2>& m2,
        UpperTriMatrixView<T3,NonUnitDiag> m3)
    {
        if (!(m1.isrm() || m1.iscm()))
            InstAddMultMM(x,m1.copy().constView().xdView(),m2,m3);
        else if (!(m2.isrm() || m2.iscm()))
            InstAddMultMM(x,m1,m2.copy().constView().xdView(),m3);
        else {
            UpperTriMatrix<T3,NonUnitDiag,ColMajor> m3c(m3.size());
            UpperTriMatrixView<T3,UnknownDiag> m3v = m3c.xdView();
            DoMultUU(m1,m2,m3v);
            InstAddMultXM(x,m3v.constView(),m3);
        }
    }

    template <class T1, bool C1, class T2, bool C2, class T3>
    void InstMultMM(
        const T3 x,
        const ConstLowerTriMatrixView<T1,UnknownDiag,UNKNOWN,UNKNOWN,C1>& m1,
        const ConstLowerTriMatrixView<T2,UnknownDiag,UNKNOWN,UNKNOWN,C2>& m2,
        LowerTriMatrixView<T3,UnknownDiag> m3)
    {
        if (!(m1.isrm() || m1.iscm())) 
            InstMultMM(x,m1.copy().constView().xdView(),m2,m3);
        else if (!(m2.isrm() || m2.iscm())) 
            InstMultMM(x,m1,m2.copy().constView().xdView(),m3);
        else if (!(m3.iscm() || m3.isrm())) {
            LowerTriMatrix<T3,NonUnitDiag,ColMajor> m3c(m3.size());
            LowerTriMatrixView<T3,UnknownDiag> m3v = m3c.xdView();
            DoMultUU(m1,m2,m3v);
            if (m3.isunit()) 
                InstCopy(m3v.constView().viewAsUnitDiag().xdView(),m3);
            else
                InstMultXM(x,m3v.constView(),m3.viewAsNonUnitDiag());
        } else {
            DoMultUU(m1,m2,m3);
            if (!m3.isunit()) InstScale(x,m3.viewAsNonUnitDiag());
        }
    }

    template <class T1, bool C1, class T2, bool C2, class T3>
    void InstAddMultMM(
        const T3 x,
        const ConstLowerTriMatrixView<T1,UnknownDiag,UNKNOWN,UNKNOWN,C1>& m1,
        const ConstLowerTriMatrixView<T2,UnknownDiag,UNKNOWN,UNKNOWN,C2>& m2,
        LowerTriMatrixView<T3,NonUnitDiag> m3)
    {
        if (!(m1.isrm() || m1.iscm()))
            InstAddMultMM(x,m1.copy().constView().xdView(),m2,m3);
        else if (!(m2.isrm() || m2.iscm()))
            InstAddMultMM(x,m1,m2.copy().constView().xdView(),m3);
        else {
            LowerTriMatrix<T3,NonUnitDiag,ColMajor> m3c(m3.size());
            LowerTriMatrixView<T3,UnknownDiag> m3v = m3c.xdView();
            DoMultUU(m1,m2,m3v);
            InstAddMultXM(x,m3v.constView(),m3);
        }
    }


#define InstFile "TMV_MultUU.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv


