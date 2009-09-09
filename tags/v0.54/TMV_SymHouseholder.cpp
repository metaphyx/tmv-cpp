
#include "TMV.h"
#include "TMV_Sym.h"
#include "TMV_SymHouseholder.h"

namespace tmv {

  template <class T1, class T2> void Householder_LRMult(
      const GenVector<T1>& vv, T1 beta, const SymMatrixView<T2>& m)
  {
    // The input vector, v, is taken to be the vector for a  
    // Householder matrix, H.  This routine takes m <- H m Ht
    // if m is Hermitian or H m HT if m is symmetric.
    TMVAssert(m.size() > 0);
    TMVAssert(vv.size() == m.size()-1);

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
    size_t N = m.size();
    if (N > 0 && beta != T1(0)) {
      // Normally, I take the unit first element of v to be implicit, 
      // but this calculation is more complicated, so for now I just 
      // copy the vector to a temporary Vector.  
      // Someday maybe I'll change this to not need this temporary.
      Vector<T1> v(N);
      v(0) = T1(1);
      v.SubVector(1,N) = vv;

      T2 betasqvtmv;
      Vector<T2> mv(N);
      if (m.isherm()) {
	mv = m*v;
        betasqvtmv = NORM(beta)*v.Conjugate()*mv;
      } else {
	mv = m*v.Conjugate();
        betasqvtmv = beta*beta*v.Conjugate()*mv;
      }
      Rank2Update(T2(-beta),v,mv,1,m);
      if (m.issym()) {
	m += betasqvtmv * (v^v);
      } else {
	// imag part is 0 - make it exact.
	m += REAL(betasqvtmv) * (v^v.Conjugate());
      }
    }
  }

#define InstFile "TMV_SymHouseholder.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace mv


