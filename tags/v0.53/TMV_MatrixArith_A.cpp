
#include "TMV.h"

//#define XDEBUG

namespace tmv {

  // 
  //
  // MultMV
  //

  // Most of the optimizations here are copied from ATLAS.
  // While the particular values of 4,8,32 used herein may not be 
  // optimal for a particular machine/data type, they are likely to be 
  // better than not blocking at all for almost all machines.
  // MJ: Haven't done the optimizing here yet.
  
  template <bool b0, bool cx, bool ca, bool rm, class T, class Ta, class Tx>
    void RowMultMV(
	const GenMatrix<Ta>& A, const GenVector<Tx>& x,
	const VectorView<T>& y)
    {
#ifdef XDEBUG
      //cerr<<"RowMultMV\n";
      //cerr<<"A = "<<A<<endl;
      //cerr<<"x = "<<x<<endl;
      //cerr<<"y = "<<y<<endl;
#endif
      TMVAssert(A.rowsize() == x.size());
      TMVAssert(A.colsize() == y.size());
      TMVAssert(x.size() > 0);
      TMVAssert(y.size() > 0);
      TMVAssert(y.ct()==NonConj);
      TMVAssert(x.step() == 1);
      TMVAssert(y.step() == 1);
      TMVAssert(!x.SameStorageAs(y));
      TMVAssert(cx == x.isconj());
      TMVAssert(ca == A.isconj());
      TMVAssert(rm == A.isrm());

      const size_t M = A.colsize();
      const size_t N = A.rowsize();
      const int si = A.stepi();
      const int sj = (rm ? 1 : A.stepj());

      const Ta* Ai0 = A.cptr();
      const Tx*const x0 = x.cptr();
      T* yi = y.ptr();
      //cerr<<"Aptr = "<<A.cptr()<<endl;
      //cerr<<"xptr = "<<x.cptr()<<endl;
      //cerr<<"yptr = "<<y.cptr()<<endl;

      for(size_t i=M; i>0; --i,++yi,Ai0+=si) {
	//cerr<<"i = "<<i<<endl;
	// *yi += A.row(i) * x

	const Ta* Aij = Ai0;
	const Tx* xj = x0;
	register T temp(0);
	for(size_t j=N; j>0; --j,++xj,(rm?++Aij:Aij+=sj))
	  temp += (cx ? CONJ(*xj) : *xj) * (ca ? CONJ(*Aij) : *Aij);

	if (b0) *yi = temp;
	else *yi += temp;
	//cerr<<"y("<<M-i<<") = "<<*yi<<endl;
      }
      //cerr<<"Done: y = "<<y<<endl;
      //abort();
    }

  template <bool b0, bool cx, bool ca, bool cm, class T, class Ta, class Tx> 
    void ColMultMV(
	const GenMatrix<Ta>& A, const GenVector<Tx>& x,
	const VectorView<T>& y)
    {
#ifdef XDEBUG
      //cerr<<"ColMultMV\n";
#endif
      TMVAssert(A.rowsize() == x.size());
      TMVAssert(A.colsize() == y.size());
      TMVAssert(x.size() > 0);
      TMVAssert(y.size() > 0);
      TMVAssert(y.ct()==NonConj);
      TMVAssert(x.step() == 1);
      TMVAssert(y.step() == 1);
      TMVAssert(!x.SameStorageAs(y));
      TMVAssert(cx == x.isconj());
      TMVAssert(ca == A.isconj());
      TMVAssert(cm == A.iscm());

      const size_t M = A.colsize();
      size_t N = A.rowsize();
      const int si = (cm ? 1 : A.stepi());
      const int sj = A.stepj();

      const Ta* A0j = A.cptr();
      const Tx* xj = x.cptr();
      T*const y0 = y.ptr();

      if (b0) {
	if (*xj == Tx(0)) {
	  y.Zero();
	} else {
	  const Ta* Aij = A0j;
	  T* yi = y0;
	  const Tx xjval = (cx ? CONJ(*xj) : *xj);
	  for(size_t i=M; i>0; --i,++yi,(cm?++Aij:Aij+=si))
	    *yi = xjval * (ca ? CONJ(*Aij) : *Aij);
	}
	++xj; A0j+=sj; --N;
      }

      for(; N>0; --N,++xj,A0j+=sj) {
	// y += *xj * A.col(j)
	if (*xj != Tx(0)) {
	  const Ta* Aij = A0j;
	  T* yi = y0;
	  const Tx xjval = (cx ? CONJ(*xj) : *xj);
	  for(size_t i=M; i>0; --i,++yi,(cm?++Aij:Aij+=si))
	    *yi += xjval * (ca ? CONJ(*Aij) : *Aij);
	}
      }
    }

  template <bool b0, bool cx, class T, class Ta, class Tx> 
    void UnitAMultMV1(
	const GenMatrix<Ta>& A, const GenVector<Tx>& x,
	const VectorView<T>& y)
    {
      TMVAssert(A.rowsize() == x.size());
      TMVAssert(A.colsize() == y.size());
      TMVAssert(x.size() > 0);
      TMVAssert(y.size() > 0);
      TMVAssert(y.ct() == NonConj);
      TMVAssert(x.step() == 1);
      TMVAssert(y.step() == 1);
      TMVAssert(!x.SameStorageAs(y));
      TMVAssert(cx == x.isconj());

      if (A.isrm()) 
	if (A.isconj())
	  RowMultMV<b0,cx,true,true>(A,x,y);
	else
	  RowMultMV<b0,cx,false,true>(A,x,y);
      else if (A.iscm())
	if (A.isconj())
	  ColMultMV<b0,cx,true,true>(A,x,y);
	else
	  ColMultMV<b0,cx,false,true>(A,x,y);
      else if ( A.rowsize() >= A.colsize() )
	if (A.isconj())
	  RowMultMV<b0,cx,true,false>(A,x,y);
	else
	  RowMultMV<b0,cx,false,false>(A,x,y);
      else 
	if (A.isconj())
	  ColMultMV<b0,cx,true,false>(A,x,y);
	else
	  ColMultMV<b0,cx,false,false>(A,x,y);
    }

  template <bool b0, bool cx, class T, class Ta, class Tx> void UnitAMultMV(
      const GenMatrix<Ta>& A, const GenVector<Tx>& x,
      const VectorView<T>& y)
  {
    // Check for 0's in beginning or end of x:
    // y += [ A1 A2 A3 ] [ 0 ]  -->  y += A2 x
    //                   [ x ]
    //                   [ 0 ]

    const size_t N = x.size(); // = A.rowsize()
    size_t j2 = N;
    for(const Tx* x2=x.cptr()+N-1; j2>0 && *x2==Tx(0); --j2,--x2);
    if (j2 == 0) {
      if (b0) y.Zero();
      return;
    }
    size_t j1 = 0;
    for(const Tx* x1=x.cptr(); *x1==Tx(0); ++j1,++x1);
    TMVAssert(j1 !=j2);
    if (j1 == 0 && j2 == N) UnitAMultMV1<b0,cx>(A,x,y);
    else UnitAMultMV1<b0,cx>(A.Cols(j1,j2),x.SubVector(j1,j2),y);
  }

  template <class T, class Ta, class Tx> void NonBlasMultMV(
      const T alpha, const GenMatrix<Ta>& A, const GenVector<Tx>& x,
      const T beta, const VectorView<T>& y)
    // y = alpha * A * x + beta * y
  {
    TMVAssert(A.rowsize() == x.size());
    TMVAssert(A.colsize() == y.size());
    TMVAssert(alpha != T(0));
    TMVAssert(x.size() > 0);
    TMVAssert(y.size() > 0);
    TMVAssert(y.ct() == NonConj);

    const RealType(T) one(1);
    const RealType(T) zero(0);
    const size_t M = A.colsize();
    const size_t N = A.rowsize();

    if (x.step() != 1 || x.SameStorageAs(y) ||
/*	(A.Real().cptr() == y.Real().cptr() && 
	 (A.isrm() || !A.iscm() && A.rowsize()<A.colsize())) || */
	(alpha != one && y.step() == 1 && M/4 >= N)) {
      // This last check is taken from the ATLAS version of this code.
      // Apparently M = 4N is the dividing line between applying alpha
      // here versus at the end when adding Ax to y
      if (IMAG(alpha) == zero) {
	Vector<Tx> xx = REAL(alpha)*x;
	if (y.step()!=1) {
	  Vector<T> yy(y.size());
	  UnitAMultMV<true,false>(A,xx,yy.View());
	  AddVV(T(1),yy,beta,y);
	} 
	else if (beta == zero)
	  UnitAMultMV<true,false>(A,xx,y);
	else {
	  if (beta != one) y *= beta;
	  UnitAMultMV<false,false>(A,xx,y);
	}
      } else {
	Vector<T> xx = alpha*x;
	if (y.step() != 1) {
	  Vector<T> yy(y.size());
	  UnitAMultMV<true,false>(A,xx,yy.View());
	  AddVV(T(1),yy,beta,y);
	} 
	else if (beta == zero)
	  UnitAMultMV<true,false>(A,xx,y);
	else {
	  if (beta != one) y *= beta;
	  UnitAMultMV<false,false>(A,xx,y);
	}
      }
    } else if (y.step() != 1 || alpha != one) {
      Vector<T> yy(y.size());
      if (x.isconj())
	UnitAMultMV<true,true>(A,x,yy.View());
      else
	UnitAMultMV<true,false>(A,x,yy.View());
      AddVV(alpha,yy,beta,y);
    } else {
      TMVAssert(alpha == T(1));
      TMVAssert(y.step() == 1);
      TMVAssert(x.step() == 1);
      TMVAssert(!x.SameStorageAs(y));
      if (x.isconj())
	if (beta == zero)
	  UnitAMultMV<true,true>(A,x,y);
	else {
	  if (beta != one) y *= beta;
	  UnitAMultMV<false,true>(A,x,y);
	}
      else
	if (beta == zero)
	  UnitAMultMV<true,false>(A,x,y);
	else {
	  if (beta != one) y *= beta;
	  UnitAMultMV<false,false>(A,x,y);
	}
    } 
  }

#ifdef BLAS
  template <class T, class Ta, class Tx> void BlasMultMV(
      const T alpha, const GenMatrix<Ta>& A,
      const GenVector<Tx>& x, const T beta, const VectorView<T>& y)
  { NonBlasMultMV(alpha,A,x,beta,y); }
  template <> void BlasMultMV(const double alpha,
      const GenMatrix<double>& A, const GenVector<double>& x,
      const double beta, const VectorView<double>& y)
  {
    TMVAssert(A.rowsize() == x.size());
    TMVAssert(A.colsize() == y.size());
    TMVAssert(A.isrm() || A.iscm());
    TMVAssert(x.size() > 0);
    TMVAssert(y.size() > 0);
    TMVAssert(A.ct() == NonConj);
    TMVAssert(x.ct() == NonConj);
    TMVAssert(y.ct() == NonConj);
    TMVAssert(!x.SameStorageAs(y));
    if (A.isrm())
      cblas_dgemv(CblasRowMajor, CblasNoTrans,
	  A.colsize(),A.rowsize(),alpha,A.cptr(), A.stepi(),
	  x.cptr(),x.step(),beta,y.ptr(),y.step()); 
    else
      cblas_dgemv(CblasColMajor, CblasNoTrans,
	  A.colsize(),A.rowsize(),alpha,A.cptr(), A.stepj(),
	  x.cptr(),x.step(),beta,y.ptr(),y.step()); 
  }
  template <> void BlasMultMV(
      const complex<double> alpha, const GenMatrix<complex<double> >& A,
      const GenVector<complex<double> >& x,
      const complex<double> beta, const VectorView<complex<double> >& y)
  {
    TMVAssert(A.rowsize() == x.size());
    TMVAssert(A.colsize() == y.size());
    TMVAssert(A.isrm() || A.iscm());
    TMVAssert(x.size() > 0);
    TMVAssert(y.size() > 0);
    TMVAssert(y.ct() == NonConj);
    TMVAssert(!x.SameStorageAs(y));
    if (x.isconj()) {
      Vector<complex<double> > xx = alpha*x;
      BlasMultMV(complex<double>(1),A,xx,beta,y);
    }
    else if (A.isconj())
      if (A.isrm()) 
	cblas_zgemv(CblasColMajor, CblasConjTrans,
	    A.rowsize(),A.colsize(),&alpha,A.cptr(), A.stepi(),
	    x.cptr(),x.step(),&beta,y.ptr(),y.step()); 
      else
	cblas_zgemv(CblasRowMajor, CblasConjTrans,
	    A.rowsize(),A.colsize(),&alpha,A.cptr(), A.stepj(),
	    x.cptr(),x.step(),&beta,y.ptr(),y.step()); 
    else
      if (A.isrm())
	cblas_zgemv(CblasRowMajor, CblasNoTrans,
	    A.colsize(),A.rowsize(),&alpha,A.cptr(), A.stepi(),
	    x.cptr(),x.step(),&beta,y.ptr(),y.step()); 
      else
	cblas_zgemv(CblasColMajor, CblasNoTrans,
	    A.colsize(),A.rowsize(),&alpha,A.cptr(), A.stepj(),
	    x.cptr(),x.step(),&beta,y.ptr(),y.step()); 
  }
#ifndef NOFLOAT
  template <> void BlasMultMV( 
      const float alpha,
      const GenMatrix<float>& A, const GenVector<float>& x,
      const float beta, const VectorView<float>& y)
  {
    TMVAssert(A.rowsize() == x.size());
    TMVAssert(A.colsize() == y.size());
    TMVAssert(A.isrm() || A.iscm());
    TMVAssert(x.size() > 0);
    TMVAssert(y.size() > 0);
    TMVAssert(A.ct() == NonConj);
    TMVAssert(x.ct() == NonConj);
    TMVAssert(y.ct() == NonConj);
    TMVAssert(!x.SameStorageAs(y));
    if (A.isrm()) 
      cblas_sgemv(CblasRowMajor, CblasNoTrans,
	  A.colsize(),A.rowsize(),alpha,A.cptr(), A.stepi(),
	  x.cptr(),x.step(),beta,y.ptr(),y.step()); 
    else
      cblas_sgemv(CblasColMajor, CblasNoTrans,
	  A.colsize(),A.rowsize(),alpha,A.cptr(), A.stepj(),
	  x.cptr(),x.step(),beta,y.ptr(),y.step()); 
  }
  template <> void BlasMultMV(
      const complex<float> alpha, const GenMatrix<complex<float> >& A,
      const GenVector<complex<float> >& x,
      const complex<float> beta, const VectorView<complex<float> >& y)
  {
    TMVAssert(A.rowsize() == x.size());
    TMVAssert(A.colsize() == y.size());
    TMVAssert(A.isrm() || A.iscm());
    TMVAssert(x.size() > 0);
    TMVAssert(y.size() > 0);
    TMVAssert(y.ct() == NonConj);
    TMVAssert(!x.SameStorageAs(y));
    if (x.isconj()) {
      Vector<complex<float> > xx = alpha*x;
      BlasMultMV(complex<float>(1),A,xx,beta,y);
    }
    else if (A.isconj())
      if (A.isrm()) 
	cblas_cgemv(CblasColMajor, CblasConjTrans,
	    A.rowsize(),A.colsize(),&alpha,A.cptr(), A.stepi(),
	    x.cptr(),x.step(),&beta,y.ptr(),y.step()); 
      else
	cblas_cgemv(CblasRowMajor, CblasConjTrans,
	    A.rowsize(),A.colsize(),&alpha,A.cptr(), A.stepj(),
	    x.cptr(),x.step(),&beta,y.ptr(),y.step()); 
    else
      if (A.isrm())
	cblas_cgemv(CblasRowMajor, CblasNoTrans,
	    A.colsize(),A.rowsize(),&alpha,A.cptr(), A.stepi(),
	    x.cptr(),x.step(),&beta,y.ptr(),y.step()); 
      else
	cblas_cgemv(CblasColMajor, CblasNoTrans,
	    A.colsize(),A.rowsize(),&alpha,A.cptr(), A.stepj(),
	    x.cptr(),x.step(),&beta,y.ptr(),y.step()); 
  }
#endif // NOFLOAT
#endif // BLAS

  template <class T, class Ta, class Tx> void DoMultMV(
      const T alpha, const GenMatrix<Ta>& A,
      const GenVector<Tx>& x, const T beta, const VectorView<T>& y)
  {
    TMVAssert(A.rowsize() == x.size());
    TMVAssert(A.colsize() == y.size());
    TMVAssert(alpha != T(0));
    TMVAssert(x.size() > 0);
    TMVAssert(y.size() > 0);
    if (y.isconj()) DoMultMV(CONJ(alpha),A.Conjugate(),x.Conjugate(),
	CONJ(beta),y.Conjugate());
    else 
#ifdef BLAS
      if ((A.isrm()&&A.stepi()>0) || (A.iscm()&&A.stepj()>0)) {
	if (y.step() > 0) {
	  if (x.step() > 0 && !x.SameStorageAs(y)) 
	    BlasMultMV(alpha,A,x,beta,y);
	  else {
	    Vector<T> xx = alpha*x;
	    BlasMultMV(T(1),A,xx,beta,y);
	  }
	} else {
	  Vector<T> yy(y.size());
	  if (x.step() > 0) {
	    BlasMultMV(T(1),A,x,T(0),yy.View());
	    AddVV(alpha,yy,beta,y);
	  } else {
	    Vector<T> xx = alpha*x;
	    BlasMultMV(T(1),A,xx,T(0),yy.View());
	    AddVV(T(1),yy,beta,y);
	  }
	}
      }
      else {
	if (IMAG(alpha) == T(0)) {
	  Matrix<Ta,RowMajor> A2 = REAL(alpha)*A;
	  DoMultMV(T(1),A2,x,beta,y);
	} else {
	  Matrix<T,RowMajor> A2 = alpha*A;
	  DoMultMV(T(1),A2,x,beta,y);
	}
      }
#else
    NonBlasMultMV(alpha,A,x,beta,y);
#endif
  }

  template <class T, class Ta, class Tx> void MultMV(
      const T alpha, const GenMatrix<Ta>& A, const GenVector<Tx>& x,
      const T beta, const VectorView<T>& y)
    // y = alpha * A * x + beta * y    
  { 
    TMVAssert(A.rowsize() == x.size());
    TMVAssert(A.colsize() == y.size());
#ifdef XDEBUG
    //cerr<<"Start MultMV: alpha, beta = "<<alpha<<"  "<<beta<<endl;
    //cerr<<"A = "<<Type(A)<<"  "<<A<<endl;
    //cerr<<"x = "<<Type(x)<<" step "<<x.step()<<"  "<<x<<endl;
    //cerr<<"y = "<<Type(y)<<" step "<<y.step()<<"  "<<y<<endl;
    Vector<T> x0 = x;
    Vector<T> y0 = y;
    Matrix<T> A0 = A;
    Vector<T> y2 = y;
    for(size_t i=0;i<y.size();i++) {
      //cerr<<"y2("<<i<<") = "<<y2(i)<<endl;
      y2(i) *= beta;
      //cerr<<"y2("<<i<<") => "<<y2(i)<<endl;
      y2(i) += alpha * (A.row(i) * x0);
      //cerr<<"y2("<<i<<") => "<<y2(i)<<endl;
    }
#endif

    if (y.size() > 0) {
      if (x.size()==0 || alpha==T(0)) y *= beta; 
      else DoMultMV(alpha,A,x,beta,y);
    }
      
#ifdef XDEBUG
    if (Norm(y-y2) > 0.001*max(RealType(T)(1),
	  abs(alpha)*Norm(A0)*Norm(x0)+abs(beta)*Norm(y0))) {
      cerr<<"MultMV: alpha,beta = "<<alpha<<"  "<<beta<<endl;
      cerr<<"A = "<<Type(A);
      if (A.rowsize() < 30 && A.colsize() < 30) cerr<<"  "<<A0;
      else cerr<<"  "<<A.colsize()<<" x "<<A.rowsize();
      cerr<<endl<<"x = "<<Type(x)<<" step "<<x.step();
      if (x.size() < 30) cerr<<"  "<<x0;
      cerr<<endl<<"y = "<<Type(y)<<" step "<<y.step();
      if (y.size() < 30) cerr<<"  "<<y0;
      cerr<<endl<<"Aptr = "<<A.cptr();
      cerr<<", xptr = "<<x.cptr()<<", yptr = "<<y.cptr()<<endl;
      if (y.size() < 200) {
	cerr<<"--> y = "<<y<<endl;
	cerr<<"y2 = "<<y2<<endl;
      } else {
	size_t imax;
	MaxAbsElement(y-y2,&imax);
	cerr<<"y("<<imax<<") = "<<y(imax)<<endl;
	cerr<<"y2("<<imax<<") = "<<y2(imax)<<endl;
      }
      cerr<<"Norm(y-y2) = "<<Norm(y-y2)<<endl;
      cerr<<"NormInf(y-y2) = "<<NormInf(y-y2)<<endl;
      cerr<<"Norm1(y-y2) = "<<Norm1(y-y2)<<endl;
      abort();
    }
#endif
  }

#define InstFile "TMV_MatrixArith_A.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace mv


