
#include "TMV.h"
#include "TMV_Tri.h"

//#define XDEBUG

namespace tmv {

  //
  // MultXM
  //

  template <class T, class T1> void RowMajorMultXM(
      const T1 alpha, const UpperTriMatrixView<T>& A)
  {
    TMVAssert(A.isrm());
    TMVAssert(!A.isunit());
    TMVAssert(alpha != T1(1));
    TMVAssert(A.size() > 0);
    TMVAssert(A.ct() == NonConj);

    T* Aii = A.ptr();
    const int ds = A.stepi()+1;
    const size_t N = A.size();

    for(size_t len=N;len>0;--len,Aii+=ds) {
      // A.row(i,i,N) *= alpha;
      T* Aij = Aii;
      for(size_t j=len;j>0;--j,++Aij) *Aij *= alpha;
    }
  }

  template <class T, class T1> void ColMajorMultXM(
      const T1 alpha, const UpperTriMatrixView<T>& A)
  {
    TMVAssert(A.iscm());
    TMVAssert(!A.isunit());
    TMVAssert(alpha != T1(1));
    TMVAssert(A.size() > 0);
    TMVAssert(A.ct() == NonConj);

    T* A0j = A.ptr();
    const int Astepj = A.stepj();
    const size_t N = A.size();

    for(size_t j=N,len=1;j>0;--j,++len,A0j+=Astepj) {
      // A.col(j,0,j+1) *= alpha;
      T* Aij = A0j;
      for(size_t i=len;i>0;--i,++Aij) *Aij *= alpha;
    }
  }

  template <class T> void MultXM(const T alpha, const UpperTriMatrixView<T>& A)
    // A = alpha * A
  {
#ifdef XDEBUG
    Matrix<T> A2 = alpha * Matrix<T>(A);
    Matrix<T> A0 = A;
#endif
    TMVAssert(!A.isunit());

    if (A.size() > 0 && alpha != T(1)) {
      if (A.isconj()) MultXM(CONJ(alpha),A.Conjugate());
      else if (alpha == T(0)) A.Zero();
      else if (A.isrm())
	if (IMAG(alpha) == RealType(T)(0))
	  RowMajorMultXM(REAL(alpha),A);
	else
	  RowMajorMultXM(alpha,A);
      else if (A.iscm())
	if (IMAG(alpha) == RealType(T)(0))
	  ColMajorMultXM(REAL(alpha),A);
	else
	  ColMajorMultXM(alpha,A);
      else 
	for(size_t i=0;i<A.colsize();++i) 
	  A.row(i,i,A.rowsize()) *= alpha;
    }
#ifdef XDEBUG
    if (Norm(A-A2) > 0.001*max(RealType(T)(1),Norm(A))) {
      cerr<<"TriMultXM: alpha = "<<alpha<<endl;
      cerr<<"A = "<<Type(A)<<"  "<<A0<<endl;
      cerr<<"-> A = "<<A<<endl;
      cerr<<"A2 = "<<A2<<endl;
      abort();
    }
#endif
  }

#define InstFile "TMV_TriMatrixArith_B.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace mv
