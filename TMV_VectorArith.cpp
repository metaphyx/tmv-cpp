#include "TMV_VectorArith_Inline.h"

namespace tmv {

  //
  // MultXV
  //
  template <class T, class Tx> inline void CallMultXV(const Tx x, 
      const VectorView<T>& v)
  {
    TMVAssert(x!=Tx(1));
    TMVAssert(x!=Tx(0));
    TMVAssert(v.size()>0);
    TMVAssert(v.ct()==NonConj);
    if (v.step() == 1)
      DoMultXV(x,VIt<T,Unit,NonConj>(v.begin()),v.size()); 
    else 
      DoMultXV(x,VIt<T,Step,NonConj>(v.begin()),v.size()); 
  }
#ifdef BLAS
  template <> inline void CallMultXV(const double x,
      const VectorView<double>& v)
  { 
    TMVAssert(x!=double(1));
    TMVAssert(x!=double(0));
    TMVAssert(v.size()>0);
    TMVAssert(v.ct()==NonConj);
    DoMultXV(x,VIt<double,Step,NonConj>(v.begin()),v.size()); 
  }
  template <> inline void CallMultXV(const double x,
      const VectorView<complex<double> >& v)
  {
    TMVAssert(x!=double(1));
    TMVAssert(x!=double(0));
    TMVAssert(v.size()>0);
    TMVAssert(v.ct()==NonConj);
    DoMultXV(x,VIt<complex<double>,Step,NonConj>(v.begin()),v.size()); 
  }
  template <> inline void CallMultXV(const complex<double> x,
      const VectorView<complex<double> >& v)
  {
    TMVAssert(x!=double(1));
    TMVAssert(x!=double(0));
    TMVAssert(v.size()>0);
    TMVAssert(v.ct()==NonConj);
    DoMultXV(x,VIt<complex<double>,Step,NonConj>(v.begin()),v.size()); 
  }
#ifndef NOFLOAT
  template <> inline void CallMultXV(const float x,
      const VectorView<float>& v)
  { 
    TMVAssert(x!=float(1));
    TMVAssert(x!=float(0));
    TMVAssert(v.size()>0);
    TMVAssert(v.ct()==NonConj);
    DoMultXV(x,VIt<float,Step,NonConj>(v.begin()),v.size()); 
  }
  template <> inline void CallMultXV(const float x,
      const VectorView<complex<float> >& v)
  {
    TMVAssert(x!=float(1));
    TMVAssert(x!=float(0));
    TMVAssert(v.size()>0);
    TMVAssert(v.ct()==NonConj);
    DoMultXV(x,VIt<complex<float>,Step,NonConj>(v.begin()),v.size()); 
  }
  template <> inline void CallMultXV(const complex<float> x,
      const VectorView<complex<float> >& v)
  {
    TMVAssert(x!=float(1));
    TMVAssert(x!=float(0));
    TMVAssert(v.size()>0);
    TMVAssert(v.ct()==NonConj);
    DoMultXV(x,VIt<complex<float>,Step,NonConj>(v.begin()),v.size()); 
  }
#endif
#endif
  template <class T, class Tx> inline void MultXV(const Tx x, 
      const VectorView<T>& v)
  { 
    if (x == Tx(0)) v.Zero();
    else if (x != T(1) && v.size()>0) 
      if (v.isconj()) MultXV(CONJ(x),v.Conjugate()); 
      else if (IsComplex(x) && IMAG(x) == RealType(Tx)(0)) MultXV(REAL(x),v);
      else if (v.step()<0) MultXV(x,v.Reverse());
      else CallMultXV(x,v); 
  }

  // 
  // AddVV
  //
  template <class Tx, class T1, class T2> inline void CallAddVV(
      const Tx x, const GenVector<T1>& v1, const VectorView<T2>& v2)
  {
    TMVAssert(v1.size() == v2.size());
    TMVAssert(v2.size()>0);
    TMVAssert(x != Tx(0));
    TMVAssert(v2.ct()==NonConj);
    if (v1.isconj())
      if (v1.step() == 1) 
	if (v2.step() == 1)
	  NonBlasAddVV(x,CVIt<T1,Unit,Conj>(v1.begin()),
	      VIt<T2,Unit,NonConj>(v2.begin()),v1.size()); 
	else 
	  NonBlasAddVV(x,CVIt<T1,Unit,Conj>(v1.begin()),
	      VIt<T2,Step,NonConj>(v2.begin()),v1.size()); 
      else
	if (v2.step() == 1)
	  NonBlasAddVV(x,CVIt<T1,Step,Conj>(v1.begin()),
	      VIt<T2,Unit,NonConj>(v2.begin()),v1.size()); 
	else
	  NonBlasAddVV(x,CVIt<T1,Step,Conj>(v1.begin()),
	      VIt<T2,Step,NonConj>(v2.begin()),v1.size()); 
    else
      if (v1.step() == 1) 
	if (v2.step() == 1)
	  NonBlasAddVV(x,CVIt<T1,Unit,NonConj>(v1.begin()),
	      VIt<T2,Unit,NonConj>(v2.begin()),v1.size()); 
	else 
	  NonBlasAddVV(x,CVIt<T1,Unit,NonConj>(v1.begin()),
	      VIt<T2,Step,NonConj>(v2.begin()),v1.size()); 
      else
	if (v2.step() == 1)
	  NonBlasAddVV(x,CVIt<T1,Step,NonConj>(v1.begin()),
	      VIt<T2,Unit,NonConj>(v2.begin()),v1.size()); 
	else 
	  NonBlasAddVV(x,CVIt<T1,Step,NonConj>(v1.begin()),
	      VIt<T2,Step,NonConj>(v2.begin()),v1.size()); 
  }
#ifdef BLAS
  template <class Tx, class T1, class T2> inline void BlasCallAddVV(
      const Tx x, const GenVector<T1>& v1, const VectorView<T2>& v2)
  { CallAddVV(x,v1,v2); }
  template <> inline void BlasCallAddVV(const double x,
      const GenVector<double>& v1, const VectorView<double>& v2)
  { 
    TMVAssert(v1.size() == v1.size()); 
    TMVAssert(v2.size()>0);
    TMVAssert(x != double(0));
    TMVAssert(v1.ct()==NonConj);
    TMVAssert(v2.ct()==NonConj);
    TMVAssert(v1.step()>0);
    TMVAssert(v2.step()>0);
    DoAddVV(x,CVIt<double,Step,NonConj>(v1.begin()),
	VIt<double,Step,NonConj>(v2.begin()),v1.size()); 
  }
  template <> inline void BlasCallAddVV(const complex<double> x, 
      const GenVector<complex<double> >& v1, 
      const VectorView<complex<double> >& v2)
  { 
    TMVAssert(v1.size() == v1.size()); 
    TMVAssert(v2.size()>0);
    TMVAssert(x != double(0));
    TMVAssert(v1.ct()==NonConj);
    TMVAssert(v2.ct()==NonConj);
    TMVAssert(v1.step()>0);
    TMVAssert(v2.step()>0);
    DoAddVV(x,CVIt<complex<double>,Step,NonConj>(v1.begin()),
	VIt<complex<double>,Step,NonConj>(v2.begin()),v1.size()); 
  }
#ifdef FLOAT
  template <> inline void BlasCallAddVV(const float x,
      const GenVector<float>& v1, const VectorView<float>& v2)
  {
    TMVAssert(v1.size() == v1.size()); 
    TMVAssert(v2.size()>0);
    TMVAssert(x != float(0));
    TMVAssert(v1.ct()==NonConj);
    TMVAssert(v2.ct()==NonConj);
    TMVAssert(v1.step()>0);
    TMVAssert(v2.step()>0);
    DoAddVV(x,CVIt<float,Step,NonConj>(v1.begin()),
	VIt<float,Step,NonConj>(v2.begin()),v1.size()); 
  }
  template <> inline void BlasCallAddVV(const complex<float> x, 
      const GenVector<complex<float> >& v1, 
      const VectorView<complex<float> >& v2)
  { 
    TMVAssert(v1.size() == v1.size()); 
    TMVAssert(v2.size()>0);
    TMVAssert(x != float(0));
    TMVAssert(v1.ct()==NonConj);
    TMVAssert(v2.ct()==NonConj);
    TMVAssert(v1.step()>0);
    TMVAssert(v2.step()>0);
    DoAddVV(x,CVIt<complex<float>,Step,NonConj>(v1.begin()),
	VIt<complex<float>,Step,NonConj>(v2.begin()),v1.size()); 
  }
#endif
#endif
  template <class T, class Tx, class T1> inline void AddVV(
      const Tx x, const GenVector<T1>& v1, const VectorView<T>& v2)
  { 
    TMVAssert(v1.size() == v1.size()); 
    if(v1.size() > 0 && x != T(0))  
      if (v2.isconj()) AddVV(CONJ(x),v1.Conjugate(),v2.Conjugate());
      else if (IsComplex(x) && IMAG(x)==RealType(Tx)(0)) AddVV(REAL(x),v1,v2);
      else if ((v1.step()<0 && v2.step()<0) || (v1.step()==-1 && v2.step()!=1) 
	  || (v2.step()==-1 && v1.step()!=1))
	AddVV(x,v1.Reverse(),v2.Reverse());
      else 
#ifdef BLAS
	if (!v1.isconj() && v1.step()>0 && v2.step()>0) BlasCallAddVV(x,v1,v2);
	else
#endif
	  CallAddVV(x,v1,v2);
  }

  template <class T, class T1> inline void AddVV(
      const T x1, const GenVector<T1>& v1,
      const T x2, const GenVector<T>& v2, const VectorView<T>& v0)
  {
    if (x1 == T(0)) {
      v0 = v2;
      MultXV(x2,v0);
    } else if (x2 == T(0)) {
      v0 = v1;
      MultXV(x1,v0);
    } else {
      if (v0.SameStorageAs(v1)) {
	if (v0.SameStorageAs(v2)) {
	  if (v0.SameAs(v1) && v0.SameAs(v2)) MultXV(x1+x2,v0);
	  else {
	    Vector<T> temp = v1;
	    MultXV(x1,temp.View());
	    AddVV(x2,v2,temp.View());
	    v0 = temp;
	  }
	} else {
	  v0 = v1;
	  MultXV(x1,v0);
	  AddVV(x2,v2,v0);
	}
      } else {
	v0 = v2;
	MultXV(x2,v0);
	AddVV(x1,v1,v0);
      }
    }
  }

  //
  // MultVV
  //
  template <class T, class T2> inline T CallMultVV(
      const GenVector<T>& v1, const GenVector<T2>& v2) 
  {
    TMVAssert(v1.size() == v2.size());
    TMVAssert(v1.size() > 0);
    TMVAssert(v1.ct()==NonConj);
    T res;
    if (v1.step() == 1) 
      if (v2.step() == 1)
	if (v2.isconj())
	  NonBlasMultVV(res,CVIt<T,Unit,NonConj>(v1.begin()),
	      CVIt<T2,Unit,Conj>(v2.begin()),v1.size()); 
	else
	  NonBlasMultVV(res,CVIt<T,Unit,NonConj>(v1.begin()),
	      CVIt<T2,Unit,NonConj>(v2.begin()),v1.size()); 
      else 
	if (v2.isconj())
	  NonBlasMultVV(res,CVIt<T,Unit,NonConj>(v1.begin()),
	      CVIt<T2,Step,Conj>(v2.begin()),v1.size()); 
	else
	  NonBlasMultVV(res,CVIt<T,Unit,NonConj>(v1.begin()),
	      CVIt<T2,Step,NonConj>(v2.begin()),v1.size()); 
    else
      if (v2.step() == 1)
	if (v2.isconj())
	  NonBlasMultVV(res,CVIt<T,Step,NonConj>(v1.begin()),
	      CVIt<T2,Unit,Conj>(v2.begin()),v1.size()); 
	else
	  NonBlasMultVV(res,CVIt<T,Step,NonConj>(v1.begin()),
	      CVIt<T2,Unit,NonConj>(v2.begin()),v1.size()); 
      else
	if (v2.isconj())
	  NonBlasMultVV(res,CVIt<T,Step,NonConj>(v1.begin()),
	      CVIt<T2,Step,Conj>(v2.begin()),v1.size()); 
	else
	  NonBlasMultVV(res,CVIt<T,Step,NonConj>(v1.begin()),
	      CVIt<T2,Step,NonConj>(v2.begin()),v1.size()); 
    return res;
  }
#ifdef BLAS
  template <class T, class T2> inline T BlasCallMultVV(
      const GenVector<T>& v1, const GenVector<T2>& v2) 
  { return CallMultVV(v1,v2); }
  template <> inline double BlasCallMultVV(const GenVector<double>& v1,
      const GenVector<double>& v2) 
  {
    TMVAssert(v1.size() == v2.size());
    TMVAssert(v1.size() > 0);
    TMVAssert(v1.ct()==NonConj);
    TMVAssert(v2.ct()==NonConj);
    double res;
    DoMultVV(res,CVIt<double,Step,NonConj>(v1.begin()),
	CVIt<double,Step,NonConj>(v2.begin()),v1.size()); 
    return res; 
  }
  template <> inline complex<double> BlasCallMultVV(
      const GenVector<complex<double> >& v1,
      const GenVector<complex<double> >& v2) 
  {
    TMVAssert(v1.size() == v2.size());
    TMVAssert(v1.size() > 0);
    TMVAssert(v1.ct()==NonConj);
    complex<double> res;
    if (v2.isconj())
      DoMultVV(res,CVIt<complex<double>,Step,NonConj>(v1.begin()),
	  CVIt<complex<double>,Step,Conj>(v2.begin()),v1.size()); 
    else
      DoMultVV(res,CVIt<complex<double>,Step,NonConj>(v1.begin()),
	  CVIt<complex<double>,Step,NonConj>(v2.begin()),v1.size()); 
    return res; 
  }
#ifdef FLOAT
  template <> inline float BlasCallMultVV(const GenVector<float>& v1,
      const GenVector<float>& v2) 
  {
    TMVAssert(v1.size() == v2.size());
    TMVAssert(v1.size() > 0);
    float res;
    DoMultVV(res,CVIt<float,Step,NonConj>(v1.begin()),
	CVIt<float,Step,NonConj>(v2.begin()),v1.size()); 
    return res; 
  }
  template <> inline complex<float> BlasCallMultVV(
      const GenVector<complex<float> >& v1,
      const GenVector<complex<float> >& v2) 
  {
    TMVAssert(v1.size() == v2.size());
    TMVAssert(v1.size() > 0);
    complex<float> res;
    if (v2.isconj())
      DoMultVV(res,CVIt<complex<float>,Step,NonConj>(v1.begin()),
	  CVIt<complex<float>,Step,Conj>(v2.begin()),v1.size()); 
    else
      DoMultVV(res,CVIt<complex<float>,Step,NonConj>(v1.begin()),
	  CVIt<complex<float>,Step,NonConj>(v2.begin()),v1.size()); 
    return res; 
  }
#endif
#endif
  template <class T, class T2> inline T MultVV(
      const GenVector<T>& v1, const GenVector<T2>& v2) 
  { 
    TMVAssert(v1.size() == v1.size()); 
    if (v1.size() == 0) return T(0);
    else if (v1.isconj()) return CONJ(MultVV(v1.Conjugate(),v2.Conjugate()));
    else if ((v1.step()<0 && v2.step()<0) || (v1.step()==-1 && v2.step()!=1) 
	|| (v2.step()==-1 && v1.step()!=1))
      return MultVV(v1.Reverse(),v2.Reverse());
    else
#ifdef BLAS
      if (v1.step()>0 && v2.step()>0) return BlasCallMultVV(v1,v2);
      else 
#endif
	return CallMultVV(v1,v2);
  }

  template <class Ta, class Tx, class Ty, StepItType Sy, ConjItType Cy, class Tz, StepItType Sz>
    void CallAddElementProd3(const Ta alpha, const GenVector<Tx>& x,
	CVIt<Ty,Sy,Cy> yit, VIt<Tz,Sz,NonConj> zit)
    {
      TMVAssert(x.size() > 0);
      TMVAssert(alpha != Ta(0));

      if (x.step() == 1)
	if (x.isconj())
	  DoAddElementProd(alpha,CVIt<Tx,Unit,Conj>(x.begin()),yit,zit,
	      x.size());
	else
	  DoAddElementProd(alpha,CVIt<Tx,Unit,NonConj>(x.begin()),yit,zit,
	      x.size());
      else
	if (x.isconj())
	  DoAddElementProd(alpha,CVIt<Tx,Step,Conj>(x.begin()),yit,zit,
	      x.size());
	else
	  DoAddElementProd(alpha,CVIt<Tx,Step,NonConj>(x.begin()),yit,zit,
	      x.size());
    }

  template <class Ta, class Tx, class Ty, class T, StepItType S>
    void CallAddElementProd2(const Ta alpha, const GenVector<Tx>& x,
	const GenVector<Ty>& y, VIt<T,S,NonConj> zit)
    // zi = alpha * xi * yi + zi
    {
      TMVAssert(x.size() == y.size());
      TMVAssert(x.size() > 0);
      TMVAssert(alpha != Ta(0));

      if (y.step() == 1)
	if (y.isconj())
	  CallAddElementProd3(alpha,x,CVIt<Ty,Unit,Conj>(y.begin()),zit);
	else
	  CallAddElementProd3(alpha,x,CVIt<Ty,Unit,NonConj>(y.begin()),zit);
      else
	if (y.isconj())
	  CallAddElementProd3(alpha,x,CVIt<Ty,Step,Conj>(y.begin()),zit);
	else
	  CallAddElementProd3(alpha,x,CVIt<Ty,Step,NonConj>(y.begin()),zit);
    }

  template <class T, class Tx, class Ty> void CallAddElementProd1(
      const T alpha, const GenVector<Tx>& x, const GenVector<Ty>& y,
      const VectorView<T>& z)
  {
    TMVAssert(x.size() == y.size());
    TMVAssert(x.size() == z.size());
    TMVAssert(z.size() > 0);
    TMVAssert(alpha != T(0));
    TMVAssert(z.ct()==NonConj);

    if (IMAG(alpha) == RealType(T)(0))
      if (z.step()==1)
	CallAddElementProd2(REAL(alpha),x,y,VIt<T,Unit,NonConj>(z.begin()));
      else
	CallAddElementProd2(REAL(alpha),x,y,VIt<T,Step,NonConj>(z.begin()));
    else
      if (z.step()==1)
	CallAddElementProd2(alpha,x,y,VIt<T,Unit,NonConj>(z.begin()));
      else
	CallAddElementProd2(alpha,x,y,VIt<T,Step,NonConj>(z.begin()));
  }

  template <class T, class Tx, class Ty> void AddElementProd(
      const T alpha, const GenVector<Tx>& x, const GenVector<Ty>& y,
      const VectorView<T>& z)
  {
    TMVAssert(x.size() == y.size());
    TMVAssert(x.size() == z.size());
    if (z.size() > 0 && alpha != T(0)) {
      if (z.isconj()) 
	AddElementProd(CONJ(alpha),x.Conjugate(),y.Conjugate(),
	    z.Conjugate());
      else if ((z.step() < 0 && (y.step() < 0 || x.step() < 0)) ||
	  (x.step() < 0 && y.step() < 0))
	  CallAddElementProd1(alpha,x.Reverse(),y.Reverse(),z.Reverse());
      else 
	CallAddElementProd1(alpha,x,y,z);
    }
  }

  template <class Ta, class Tx, class Ty> void CallElementProd(
      const Ta alpha, const GenVector<Tx>& x, const VectorView<Ty>& y)
    // yi = alpha * xi * yi
  {
    TMVAssert(x.size() == y.size());
    TMVAssert(x.size() > 0);

    if (x.step() == 1)
      if (x.isconj())
	if (y.step() == 1)
	  DoElementProd(alpha, CVIt<Tx,Unit,Conj>(x.begin()),
	      VIt<Ty,Unit,NonConj>(y.begin()),y.size());
	else
	  DoElementProd(alpha, CVIt<Tx,Unit,Conj>(x.begin()),
	      VIt<Ty,Step,NonConj>(y.begin()),y.size());
      else
	if (y.step() == 1)
	  DoElementProd(alpha, CVIt<Tx,Unit,NonConj>(x.begin()),
	      VIt<Ty,Unit,NonConj>(y.begin()),y.size());
	else
	  DoElementProd(alpha, CVIt<Tx,Unit,NonConj>(x.begin()),
	      VIt<Ty,Step,NonConj>(y.begin()),y.size());
    else
      if (x.isconj())
	if (y.step() == 1)
	  DoElementProd(alpha, CVIt<Tx,Step,Conj>(x.begin()),
	      VIt<Ty,Unit,NonConj>(y.begin()),y.size());
	else
	  DoElementProd(alpha, CVIt<Tx,Step,Conj>(x.begin()),
	      VIt<Ty,Step,NonConj>(y.begin()),y.size());
      else
	if (y.step() == 1)
	  DoElementProd(alpha, CVIt<Tx,Step,NonConj>(x.begin()),
	      VIt<Ty,Unit,NonConj>(y.begin()),y.size());
	else
	  DoElementProd(alpha, CVIt<Tx,Step,NonConj>(x.begin()),
	      VIt<Ty,Step,NonConj>(y.begin()),y.size());
  }

  template <class T, class Tx> void ElementProd(const T alpha,
      const GenVector<Tx>& x, const VectorView<T>& y)
  {
    TMVAssert(x.size() == y.size());
    if (y.size() > 0 && alpha != T(0)) 
      if ((x.step() < 0 && y.step() < 0) || (x.step()==-1 && y.step()!=1) || 
	  (y.step()==-1 && x.step()!=1))
	ElementProd(alpha,x.Reverse(),y.Reverse());
      else if (IMAG(alpha) == 0) 
	if (y.isconj()) 
	  CallElementProd(REAL(alpha),x.Conjugate(),y.Conjugate());
	else 
	  CallElementProd(REAL(alpha),x,y);
      else
	if (y.isconj()) 
	  CallElementProd(alpha,x.Conjugate(),y.Conjugate());
	else 
	  CallElementProd(alpha,x,y);
  }

#define InstFile "TMV_VectorArith.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace mv


