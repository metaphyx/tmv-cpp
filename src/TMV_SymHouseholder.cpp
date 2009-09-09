///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// The Template Matrix/Vector Library for C++ was created by Mike Jarvis     //
// Copyright (C) 2008                                                        //
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



#include "TMV_SymHouseholder.h"
#include "TMV_Vector.h"
#include "TMV_SymMatrix.h"
#include "TMV_SymMatrixArith.h"
#include "TMV_MatrixArith.h"
#include "TMV_VectorArith.h"

namespace tmv {

  template <class T1, class T2> void Householder_LRMult(
      const GenVector<T1>& v, T1 beta, const SymMatrixView<T2>& m)
  {
    // The input vector, v, is taken to be the vector for a  
    // Householder matrix, H.  This routine takes m <- H m Ht
    // if m is Hermitian or H m HT if m is symmetric.
    TMVAssert(m.size() > 0);
    TMVAssert(v.size() == m.size()-1);

    // If m is Hermitian:
    //
    // H m Ht = (I - beta v vt) m (I - beta* v vt)
    //        = m - beta v vt m - beta* m v vt + |beta|^2 v vt m v vt
    //        = m - beta v (mv)t - beta* (mv) vt + (|beta|^2 vt (mv)) v vt
    //
    // If m is symmetric:
    //
    // H m HT = (I - beta v vt) m (I - beta v* vT)
    //        = m - beta v vt m - beta m v* vT + beta^2 v vt m v* vT
    //        = m - beta v (mv*)T - beta (mv*) vT + (beta^2 vt (mv*)) v vT
    //
    int N = m.size();
    if (N > 0 && beta != T1(0)) {
      // Normally, I take the unit first element of v to be implicit, 
      // but this calculation is more complicated, so for now I just 
      // copy the vector to a temporary Vector (vv).  
      // Someday maybe I'll change this to not need this temporary.
      Vector<T1> vv(N);
      vv(0) = T1(1);
      vv.SubVector(1,N) = v;

      T2 betasqvtmv;
      Vector<T2> mv(N);
      if (m.isherm()) {
	mv = m*vv;
        betasqvtmv = NORM(beta)*vv.Conjugate()*mv;
      } else {
	mv = m*vv.Conjugate();
        betasqvtmv = beta*beta*vv.Conjugate()*mv;
      }
      Rank2Update<true>(T2(-beta),vv,mv,m);
      if (m.issym()) {
	m += betasqvtmv * (vv^vv);
      } else {
	// imag part is 0 - make it exact.
	m += REAL(betasqvtmv) * (vv^vv.Conjugate());
      }
    }
  }

#define InstFile "TMV_SymHouseholder.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace mv


