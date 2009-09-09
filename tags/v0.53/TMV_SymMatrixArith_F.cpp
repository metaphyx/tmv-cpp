
#include "TMV.h"
#include "TMV_Sym.h"

//#define XDEBUG

namespace tmv {

  // 
  // Rank2Update
  //

  template <bool cx, bool cy, bool ha, bool rm, bool add, class T, class Tx, class Ty> 
    void RowRank2Update(
	const GenVector<Tx>& x, const GenVector<Ty>& y,
	const SymMatrixView<T>& A)
    {
#ifdef XDEBUG
      //cerr<<"Row Rank2\n";
#endif
      TMVAssert(A.size() == x.size());
      TMVAssert(A.size() == y.size());
      TMVAssert(A.size() > 0);
      TMVAssert(A.ct() == NonConj);
      TMVAssert(A.uplo() == Lower);
      TMVAssert(x.step()==1);
      TMVAssert(y.step()==1);
      TMVAssert(rm == A.isrm());
      TMVAssert(cx == x.isconj());
      TMVAssert(cy == y.isconj());
      TMVAssert(ha == A.isherm());

      const size_t si = A.stepi();
      const size_t sj = A.stepj();
      const size_t N = A.size();
      const Tx*const x0 = x.cptr();
      const Ty*const y0 = y.cptr();

      const Tx* xi = x0;
      const Ty* yi = y0;

      T A00;
      if (*xi == Tx(0) || *yi == Ty(0)) A00 = T(0);
      else if (ha) A00 = RealType(T)(2) * REAL(*xi * CONJ(*yi));
      else A00 = RealType(T)(2) * (cx?CONJ(*xi):*xi) * (cy?CONJ(*yi):*yi);
      ++xi; ++yi;
      T* Arowi = A.ptr()+si;

      for (size_t i=1;i<N;++i,++xi,++yi,Arowi+=si) {
	// A.row(i,0,i+1) += ax * y.SubVector(0,i+1) + ay * x.SubVector(0,i+1);
	if (*xi != Tx(0)) {
	  const Tx xival = cx ? CONJ(*xi) : *xi;
	  T* Aij = Arowi;
	  const Ty* yj = y0;
	  if (*yi != Ty(0)) {
	    const Ty yival = cy ? CONJ(*yi) : *yi;
	    const Tx* xj = x0;
	    for(size_t j=i+1;j>0;--j,++yj,++xj,(rm?++Aij:Aij+=sj)) {
	      T temp = xival * (ha==cy ? *yj : CONJ(*yj));
	      temp += yival * (ha==cx ? *xj : CONJ(*xj));
	      if (add) *Aij += temp;
	      else *Aij = temp;
	    }
	  } else {
	    for(size_t j=i+1;j>0;--j,++yj,(rm?++Aij:Aij+=sj)) {
	      const T temp = xival * (ha==cy ? *yj : CONJ(*yj));
	      if (add) *Aij += temp;
	      else *Aij = temp;
	    }
	  }
	} else if (*yi != Ty(0)) {
	  const Ty yival = cy ? CONJ(*yi) : *yi;
	  T* Aij = Arowi;
	  const Tx* xj = x0;
	  for(size_t j=i+1;j>0;--j,++xj,(rm?++Aij:Aij+=sj)) {
	    const T temp = yival * (ha==cx ? *xj : CONJ(*xj));
	    if (add) *Aij += temp;
	    else *Aij = temp;
	  }
	}
      }
      if (add) *A.ptr() += A00;
      else *A.ptr() = A00;
    }

  template <bool cx, bool cy, bool ha, bool cm, bool add, class T, class Tx, class Ty> 
    void ColRank2Update(
	const GenVector<Tx>& x, const GenVector<Ty>& y,
	const SymMatrixView<T>& A)
    {
#ifdef XDEBUG
      //cerr<<"Col Rank2\n";
#endif
      TMVAssert(A.size() == x.size());
      TMVAssert(A.size() == y.size());
      TMVAssert(A.size() > 0);
      TMVAssert(A.ct() == NonConj);
      TMVAssert(A.uplo() == Lower);
      TMVAssert(x.step()==1);
      TMVAssert(y.step()==1);
      TMVAssert(cm == A.iscm());
      TMVAssert(cx == x.isconj());
      TMVAssert(cy == y.isconj());
      TMVAssert(ha == A.isherm());

      const size_t si = cm ? 1 : A.stepi();
      const size_t ds = A.stepj() + si;
      const size_t N = A.size();
      const Tx* xj = x.cptr()+N-1;
      const Ty* yj = y.cptr()+N-1;
      T* Ajj = A.ptr()+(N-1)*ds;

      for (size_t jj=N,Nmj=1;jj>0;--jj,++Nmj,--xj,--yj,Ajj-=ds) {
	// Nmj = N-j
	// A.col(j,j,N) += (A.isherm() ? CONJ(*yj) : *yj) * x.SubVector(j,N);
	// A.col(j,j,N) += (A.isherm() ? CONJ(*xj) : *xj) * y.SubVector(j,N);
	if (*yj!=Tx(0)) {
	  const Ty yjval = (ha==cy) ? *yj : CONJ(*yj);
	  T* Aij = Ajj;
	  const Tx* xi = xj;
	  if (*xj!=Tx(0)) {
	    const Tx xjval = (ha==cx) ? *xj : CONJ(*xj);
	    const Ty* yi = yj;
	    for(size_t i=Nmj;i>0;--i,++xi,++yi,(cm?++Aij:Aij+=si)) {
	      T temp = (cx ? CONJ(*xi) : *xi) * yjval;
	      temp += (cy ? CONJ(*yi) : *yi) * xjval;
	      if (add) *Aij += temp;
	      else *Aij = temp;
	    }
	  } else {
	    for(size_t i=Nmj;i>0;--i,++xi,(cm?++Aij:Aij+=si)) {
	      const T temp = (cx ? CONJ(*xi) : *xi) * yjval;
	      if (add) *Aij += temp;
	      else *Aij = temp;
	    }
	  }
	} else if (*xj!=Tx(0)) {
	  const Tx xjval = (ha==cx) ? *xj : CONJ(*xj);
	  T* Aij = Ajj;
	  const Ty* yi = yj;
	  for(size_t i=Nmj;i>0;--i,++yi,(cm?++Aij:Aij+=si)) {
	    const T temp = (cy ? CONJ(*yi) : *yi) * xjval;
	    if (add) *Aij += temp;
	    else *Aij = temp;
	  }
	}
      }
    }

  template <bool cx, bool cy, bool add, class T, class Tx, class Ty> 
    inline void UnitARank2Update(const GenVector<Tx>& x,
	const GenVector<Ty>& y, const SymMatrixView<T>& A)
    {
      TMVAssert(A.size() == x.size());
      TMVAssert(A.size() == y.size());
      TMVAssert(A.size() > 0);
      TMVAssert(A.ct() == NonConj);
      TMVAssert(A.uplo() == Lower);
      TMVAssert(x.step() == 1);
      TMVAssert(y.step() == 1);
      TMVAssert(cx == x.isconj());
      TMVAssert(cy == y.isconj());

      if (A.isherm())
	if (A.isrm()) RowRank2Update<cx,cy,true,true,add>(x,y,A);
	else if (A.iscm()) ColRank2Update<cx,cy,true,true,add>(x,y,A);
	else RowRank2Update<cx,cy,true,false,add>(x,y,A);
      else
	if (A.isrm()) RowRank2Update<cx,cy,false,true,add>(x,y,A);
	else if (A.iscm()) ColRank2Update<cx,cy,false,true,add>(x,y,A);
	else RowRank2Update<cx,cy,false,false,add>(x,y,A);
    }

  template <bool add, class T, class Tx, class Ty> void NonBlasRank2Update(
      const T alpha, const GenVector<Tx>& x, const GenVector<Ty>& y,
      const SymMatrixView<T>& A)
  {
    TMVAssert(A.size() == x.size());
    TMVAssert(A.size() == y.size());
    TMVAssert(alpha != T(0));
    TMVAssert(A.size() > 0);
    TMVAssert(A.ct() == NonConj);
    TMVAssert(A.uplo() == Lower);

    if (x.step() != 1 || alpha != T(1)) {
      if (x.step() == 1 && y.step() != 1) {
	if (IMAG(alpha) == RealType(T)(0)) {
	  Vector<Ty> yy = REAL(alpha)*y;
	  if (x.isconj())
	    UnitARank2Update<true,false,add>(x,yy,A);
	  else
	    UnitARank2Update<false,false,add>(x,yy,A);
	} else {
	  Vector<T> yy = alpha*y;
	  if (x.isconj())
	    UnitARank2Update<true,false,add>(x,yy,A);
	  else
	    UnitARank2Update<false,false,add>(x,yy,A);
	}
      } else {
	if (IMAG(alpha) == RealType(T)(0)) {
	  Vector<Tx> xx = REAL(alpha)*x;
	  if (y.step() == 1)
	    if (y.isconj())
	      UnitARank2Update<false,true,add>(xx,y,A);
	    else
	      UnitARank2Update<false,false,add>(xx,y,A);
	  else {
	    Vector<Ty> yy = y;
	    UnitARank2Update<false,false,add>(xx,yy,A);
	  }
	} else {
	  Vector<T> xx = alpha*x;
	  if (y.step() == 1)
	    if (y.isconj())
	      UnitARank2Update<false,true,add>(xx,y,A);
	    else
	      UnitARank2Update<false,false,add>(xx,y,A);
	  else {
	    Vector<Ty> yy = y;
	    UnitARank2Update<false,false,add>(xx,yy,A);
	  }
	}
      }
    } else {
      if (x.isconj())
	if (y.isconj())
	  UnitARank2Update<true,true,add>(x,y,A);
	else
	  UnitARank2Update<true,false,add>(x,y,A);
      else
	if (y.isconj())
	  UnitARank2Update<false,true,add>(x,y,A);
	else
	  UnitARank2Update<false,false,add>(x,y,A);
    }
  }

#ifdef BLAS
  template <class T, class Tx, class Ty> void BlasRank2Update(
      const T alpha, const GenVector<Tx>& x, const GenVector<Ty>& y,
      const SymMatrixView<T>& A)
  { NonBlasRank2Update<true>(alpha,x,y,A); }
  template <> void BlasRank2Update(
      const double alpha, const GenVector<double>& x,
      const GenVector<double>& y, const SymMatrixView<double>& A)
  {
#ifdef XDEBUG
    //cerr<<"Blas Rank2 double\n";
#endif
    TMVAssert(A.size() == x.size());
    TMVAssert(A.size() == y.size());
    TMVAssert(alpha != double(0));
    TMVAssert(x.size() > 0);
    TMVAssert(x.step() > 0);
    TMVAssert(y.step() > 0);
    TMVAssert(x.ct() == NonConj);
    TMVAssert(y.ct() == NonConj);
    TMVAssert(A.ct() == NonConj);
    TMVAssert(A.uplo() == Lower);
    TMVAssert(A.isherm());

    if (A.isrm())
      cblas_dsyr2(CblasRowMajor,CblasLower,A.size(),alpha,
	  x.cptr(),x.step(),y.cptr(),y.step(),A.ptr(),A.stepi()); 
    else
      cblas_dsyr2(CblasColMajor,CblasLower,A.size(),alpha,
	  x.cptr(),x.step(),y.cptr(),y.step(),A.ptr(),A.stepj()); 
  }
  template <> void BlasRank2Update(
      const complex<double> alpha, const GenVector<complex<double> >& x, 
      const GenVector<double>& y, const SymMatrixView<complex<double> >& A)
  {
#ifdef XDEBUG
    //cerr<<"Blas Rank2 c double\n";
#endif
    TMVAssert(A.size() == x.size());
    TMVAssert(A.size() == y.size());
    TMVAssert(alpha != double(0));
    TMVAssert(x.size() > 0);
    TMVAssert(x.step() > 0);
    TMVAssert(y.step() > 0);
    TMVAssert(A.ct() == NonConj);
    TMVAssert(x.ct() == NonConj);
    TMVAssert(y.ct() == NonConj);
    TMVAssert(A.uplo() == Lower);
    TMVAssert(A.isherm());

    if (A.isrm())
      cblas_zher2(CblasRowMajor,CblasLower,A.size(),&alpha,
	  x.cptr(),x.step(),y.cptr(),y.step(),A.ptr(),A.stepi()); 
    else
      cblas_zher2(CblasColMajor,CblasLower,A.size(),&alpha,
	  x.cptr(),x.step(),y.cptr(),y.step(),A.ptr(),A.stepj()); 
  }

#ifndef NOFLOAT
  template <> void BlasRank2Update(
      const float alpha, const GenVector<float>& x,
      const GenVector<float>& y, const SymMatrixView<float>& A)
  {
#ifdef XDEBUG
    //cerr<<"Blas Rank2 float\n";
#endif
    TMVAssert(A.size() == x.size());
    TMVAssert(A.size() == y.size());
    TMVAssert(alpha != float(0));
    TMVAssert(x.size() > 0);
    TMVAssert(x.step() > 0);
    TMVAssert(y.step() > 0);
    TMVAssert(x.ct() == NonConj);
    TMVAssert(y.ct() == NonConj);
    TMVAssert(A.ct() == NonConj);
    TMVAssert(A.uplo() == Lower);
    TMVAssert(A.isherm());

    if (A.isrm())
      cblas_ssyr2(CblasRowMajor,CblasLower,A.size(),alpha,
	  x.cptr(),x.step(),y.cptr(),y.step(),A.ptr(),A.stepi()); 
    else
      cblas_ssyr2(CblasColMajor,CblasLower,A.size(),alpha,
	  x.cptr(),x.step(),y.cptr(),y.step(),A.ptr(),A.stepj()); 
  }
  template <> void BlasRank2Update(
      const complex<float> alpha, const GenVector<complex<float> >& x, 
      const GenVector<float>& y, const SymMatrixView<complex<float> >& A)
  {
#ifdef XDEBUG
    //cerr<<"Blas Rank2 c float\n";
#endif
    TMVAssert(A.size() == x.size());
    TMVAssert(A.size() == y.size());
    TMVAssert(alpha != float(0));
    TMVAssert(x.size() > 0);
    TMVAssert(x.step() > 0);
    TMVAssert(y.step() > 0);
    TMVAssert(A.ct() == NonConj);
    TMVAssert(x.ct() == NonConj);
    TMVAssert(y.ct() == NonConj);
    TMVAssert(A.uplo() == Lower);
    TMVAssert(A.isherm());

    if (A.isrm())
      cblas_zher2(CblasRowMajor,CblasLower,A.size(),&alpha,
	  x.cptr(),x.step(),y.cptr(),y.step(),A.ptr(),A.stepi()); 
    else
      cblas_zher2(CblasColMajor,CblasLower,A.size(),&alpha,
	  x.cptr(),x.step(),y.cptr(),y.step(),A.ptr(),A.stepj()); 
  }
#endif // NOFLOAT
#endif // BLAS

  template <class T, class Tx, class Ty> void Rank2Update(
      const T alpha, const GenVector<Tx>& x, const GenVector<Ty>& y,
      const int beta, const SymMatrixView<T>& A)
    // if A is sym:  A = A + alpha * (x ^ y + y ^ x)
    // if A is herm: A = A + alpha * x ^ y* + conj(alpha) * y ^ x*
  {
#ifdef XDEBUG
    Matrix<T> A2 = Matrix<T>(A);
    if (A.isherm()) {
      if (beta == 0) A2 = (alpha*x^y.Conjugate());
      else A2 += (alpha*x^y.Conjugate());
      A2 += (CONJ(alpha)*y^x.Conjugate());
    }
    else {
      if (beta == 0) A2 = alpha*(x^y);
      else A2 += alpha*(x^y);
      A2 += alpha*(y^x);
    }
    Matrix<T> A0 = A;
#endif

    TMVAssert(A.size() == x.size());
    TMVAssert(A.size() == y.size());
    TMVAssert(beta == 0 || beta == 1);
    if (alpha != T(0) && A.size() > 0) {
      if (A.isconj()) 
	Rank2Update(CONJ(alpha),x.Conjugate(),y.Conjugate(),beta,
	    A.Conjugate());
      else if (A.uplo() == Upper) {
	if (A.isherm()) Rank2Update(alpha,x,y,beta,A.Adjoint());
	else Rank2Update(alpha,x,y,beta,A.Transpose());
      }
#ifdef BLAS
      else if ( ((A.isrm() && A.stepi()>0) || (A.iscm()&&A.stepj()>0)) ) {
	if (x.isconj() || x.step() < 0 || x.cptr() == (Tx*)(A.ptr())) {
	  if (y.isconj() || y.step() < 0 || y.cptr() == (Ty*)(A.ptr())) {
	    if (x.size() <= y.size()) {
	      Vector<Tx> xx = alpha*x;
	      Vector<Ty> yy = y;
	      if (beta == 0) A.Zero();
	      BlasRank2Update(T(1),xx,yy,A);
	    } else {
	      Vector<Tx> xx = x;
	      Vector<Ty> yy = CONJ(alpha)*y;
	      if (beta == 0) A.Zero();
	      BlasRank2Update(T(1),xx,yy,A);
	    }
	  } else {
	    Vector<Tx> xx = alpha*x;
	    if (beta == 0) A.Zero();
	    BlasRank2Update(T(1),xx,y,A);
	  }
	} else {
	  if (y.isconj() || y.step() < 0 || y.cptr() == (Ty*)(A.ptr())) {
	    Vector<Ty> yy = CONJ(alpha)*y;
	    if (beta == 0) A.Zero();
	    BlasRank2Update(T(1),x,yy,A);
	  } else {
	    if (beta == 0) A.Zero();
	    BlasRank2Update(alpha,x,y,A);
	  }
	}
      }
#endif
      else if (beta == 0)
	NonBlasRank2Update<false>(alpha,x,y,A);
      else
	NonBlasRank2Update<true>(alpha,x,y,A);
    }
  
#ifdef XDEBUG
    if (Norm(A-A2) > 0.001*max(RealType(T)(1),Norm(A))) {
      cerr<<"Rank2Update: alpha = "<<alpha<<','<<beta<<endl;
      cerr<<"x = "<<Type(x)<<"  step = "<<x.step()<<"  "<<x<<endl;
      cerr<<"y = "<<Type(y)<<"  step = "<<y.step()<<"  "<<y<<endl;
      cerr<<"A = "<<Type(A)<<"  "<<A0<<endl;
      cerr<<"-> A = "<<A<<endl;
      cerr<<"A2 = "<<A2<<endl;
      abort();
    }
#endif
  }

#define InstFile "TMV_SymMatrixArith_F.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace mv


