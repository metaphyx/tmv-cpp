// vim:et:ts=2:sw=2:ci:cino=f0,g0,t0,+0:
#define STARTI 0
#define STARTJ 0

//#define XXDEBUG8

#include "TMV_Test.h"
#include "TMV_Test2.h"
#include "TMV.h"
#include "TMV_SymBand.h"
#include "TMV_TestSymBandArith.h"
#include "TMV_TestSymArith.h"

template <class T> inline bool CanAddEq(
    const tmv::SymMatrixView<std::complex<T> >& a, 
    const tmv::SymBandMatrixView<std::complex<T> >& b)
{ return a.size() == b.size() && a.sym() == b.sym(); }

#define NOMULTEQ

#include "TMV_TestMatrixArith.h"

template <class T> void TestSymBandMatrixArith_F2()
{
#ifdef XTEST
  std::vector<tmv::SymBandMatrixView<T> > b;
  std::vector<tmv::SymBandMatrixView<std::complex<T> > > cb;
  std::vector<tmv::BaseMatrix<T>*> B;
  std::vector<tmv::BaseMatrix<std::complex<T> >*> CB;
  MakeSymBandList(b,cb,B,CB,InDef);

  std::vector<tmv::SymMatrixView<T> > s;
  std::vector<tmv::SymMatrixView<std::complex<T> > > cs;
  MakeSymList(s,cs,B,CB,InDef);

  for(size_t i=STARTI;i<b.size();i++) {
    if (showstartdone) {
      std::cout<<"Start loop i = "<<i<<std::endl;
      std::cout<<"bi = "<<b[i]<<std::endl;
    }

    tmv::SymBandMatrixView<T> bi = b[i];
    tmv::SymBandMatrixView<std::complex<T> > cbi = cb[i];

    for(size_t j=STARTJ;j<s.size();j++) {
      if (showstartdone) {
        std::cerr<<"Start sub-loop "<<j<<std::endl;
        std::cerr<<"sj = "<<s[j]<<std::endl;
      }
      tmv::SymMatrixView<T> sj = s[j];
      tmv::SymMatrixView<std::complex<T> > csj = cs[j];

      if (cbi.isherm()) {
        tmv::HermBandMatrix<T> bx = bi;
        tmv::HermBandMatrix<std::complex<T> > cbx = cbi;
        if (csj.isherm()) {
          tmv::HermMatrix<T> sx = sj;
          tmv::HermMatrix<std::complex<T> > csx = csj;
          TestMatrixArith456<T>(sx,csx,sj,csj,bi,cbi,"Herm/HermBand");
        } else {
          tmv::SymMatrix<T> sx = sj;
          tmv::SymMatrix<std::complex<T> > csx = csj;
          TestMatrixArith456<T>(sx,csx,sj,csj,bi,cbi,"Sym/HermBand");
        }
      } else {
        tmv::SymBandMatrix<T> bx = bi;
        tmv::SymBandMatrix<std::complex<T> > cbx = cbi;
        if (csj.isherm()) {
          tmv::HermMatrix<T> sx = sj;
          tmv::HermMatrix<std::complex<T> > csx = csj;
          TestMatrixArith456<T>(sx,csx,sj,csj,bi,cbi,"Herm/SymBand");
        } else {
          tmv::SymMatrix<T> sx = sj;
          tmv::SymMatrix<std::complex<T> > csx = csj;
          TestMatrixArith456<T>(sx,csx,sj,csj,bi,cbi,"Sym/SymBand");
        }
      }
    }
  }
  for(size_t i=0;i<B.size();++i) delete B[i];
  for(size_t i=0;i<CB.size();++i) delete CB[i];
#endif
}

#ifdef TEST_DOUBLE
template void TestSymBandMatrixArith_F2<double>();
#endif
#ifdef TEST_FLOAT
template void TestSymBandMatrixArith_F2<float>();
#endif
#ifdef TEST_LONGDOUBLE
template void TestSymBandMatrixArith_F2<long double>();
#endif
#ifdef TEST_INT
template void TestSymBandMatrixArith_F2<int>();
#endif
