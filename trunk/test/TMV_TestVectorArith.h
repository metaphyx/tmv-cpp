// vim:et:ts=2:sw=2:ci:cino=f0,g0,t0,+0:
#ifndef TMV_TESTVECTOR_H
#define TMV_TESTVECTOR_H

#include "TMV_Test.h"

//template <class T1, class T2> struct ProdType
//{ typedef T1 Tprod; };

//template <class T> struct ProdType<T,std::complex<T> >
//{ typedef std::complex<T> Tprod; };

//#define ProductType(T1,T2) typename ProdType<T1,T2>::Tprod

template <class T, class V> inline void TestV(const V& a, std::string label)
{
  if (showstartdone) {
    std::cout<<"Start V "<<label<<std::endl;
    std::cout<<"a = "<<tmv::TypeText(a)<<std::endl;
  }

  tmv::Vector<T> v = a;
  RealType(T) eps = EPS*v.size();

  if (XXDEBUG1) {
    std::cout<<"a = "<<tmv::TypeText(a)<<" = "<<a<<std::endl;
    std::cout<<"v = "<<tmv::TypeText(v)<<" = "<<v<<std::endl;
    std::cout<<"a-v = "<<a-v<<std::endl;
    std::cout<<"Norm(a-v) = "<<Norm(a-v)<<std::endl;
    std::cout<<"Norm1(a) = "<<Norm1(a)<<"  "<<Norm1(v)<<std::endl;
    std::cout<<"Norm2(a) = "<<Norm2(a)<<"  "<<Norm2(v)<<std::endl;
    std::cout<<"NormInf(a) = "<<NormInf(a)<<"  "<<NormInf(v)<<std::endl;
    std::cout<<"NormSq(a) = "<<NormSq(a)<<"  "<<NormSq(v)<<std::endl;
    std::cout<<"abs(diff) = "<<tmv::TMV_ABS(NormSq(a)-NormSq(v))<<std::endl;
    std::cout<<"eps*normsq = "<<eps*NormSq(v)<<std::endl;
  }

  Assert(Norm(a-v) <= eps*Norm(v),label+" a != v");

  Assert(tmv::TMV_ABS(Norm1(a)-Norm1(v)) <= eps*tmv::TMV_ABS(Norm1(v)),label+" Norm1");
  Assert(tmv::TMV_ABS(Norm2(a)-Norm2(v)) <= eps*tmv::TMV_ABS(Norm2(v)),label+" Norm2");
  Assert(tmv::TMV_ABS(NormInf(a)-NormInf(v)) <= eps*tmv::TMV_ABS(NormInf(v)),
      label+" NormInf");
  Assert(tmv::TMV_ABS(NormSq(a)-NormSq(v)) <= eps*tmv::TMV_ABS(NormSq(v)),
      label+" NormSq");

  if (showstartdone) {
    std::cout<<"Done V "<<std::endl;
  }
}

template <class T, class V, class T2> inline void TestVX(
    const V& a, T2 x, std::string label)
{
  if (showstartdone) {
    std::cout<<"Start VX "<<label<<std::endl;
    std::cout<<"a = "<<tmv::TypeText(a)<<std::endl;
    std::cout<<"x "<<x<<std::endl;
  }

  tmv::Vector<T> v = a;
  RealType(T) eps = EPS*v.size();

  if (XXDEBUG2) {
    std::cout<<"a = "<<tmv::TypeText(a)<<" = "<<a<<std::endl;
    std::cout<<"v = "<<tmv::TypeText(v)<<" = "<<v<<std::endl;
    std::cout<<"a-v = "<<a-v<<std::endl;
    std::cout<<"Norm(a-v) = "<<Norm(a-v)<<std::endl;
    std::cout<<"x*a = "<<(x*a)<<std::endl;
    std::cout<<"x*v = "<<(x*v)<<std::endl;
    std::cout<<"Norm(diff) = "<<Norm((x*a)-(x*v))<<std::endl;
    std::cout<<"eps*norm = "<<eps*Norm(v)*tmv::TMV_ABS(x)<<std::endl;
  }
  Assert(Norm(a-v) <= eps*Norm(v),label+" a != v");

  Assert(Norm((x*a)-(x*v)) <= eps*Norm(v)*tmv::TMV_ABS(x),label+" x*a");
  Assert(Norm((a*x)-(x*v)) <= eps*Norm(v)*tmv::TMV_ABS(x),label+" a*x");
  Assert(Norm((a/x)-(v/x)) <= eps*Norm(v)*tmv::TMV_ABS(x),label+" a/x");

  if (showstartdone) {
    std::cout<<"Done VX "<<std::endl;
  }
}

template <class T, class V, class T2> inline void TestVX2(
    V& a, T2 x, std::string label)
{
  if (showstartdone) {
    std::cout<<"Start VX2 "<<label<<std::endl;
    std::cout<<"a = "<<tmv::TypeText(a)<<std::endl;
    std::cout<<"x "<<x<<std::endl;
  }

  tmv::Vector<T> v = a;
  RealType(T) eps = EPS*v.size();
  Assert(Norm(a-v) <= eps*Norm(v),label+" a != v");
  double normv = tmv::TMV_ABS(x)*Norm(v);
  typename V::copy_type a0 = a;

  if (XXDEBUG3) {
    std::cout<<"a = "<<tmv::TypeText(a)<<" = "<<a<<std::endl;
    std::cout<<"v = "<<tmv::TypeText(v)<<" = "<<v<<std::endl;
    std::cout<<"x = "<<x<<std::endl;
    std::cout<<"a*=x = "<<(a*=x)<<std::endl;
    std::cout<<"x*v = "<<(x*v)<<std::endl;
    std::cout<<"Norm(diff) = "<<Norm(a-(x*v))<<std::endl;
    std::cout<<"eps*norm = "<<eps*normv<<std::endl;
    a = a0;
  }

  a *= x;
  v = tmv::Vector<T>(v*x);
  Assert(Norm(a-v) <= eps*normv,label+" a *= x");
  Assert(Norm((a*=x)-(v*=x)) <= eps*normv,label+" a *= x (2)");
  a = v = a0;

  a /= x;
  v = tmv::Vector<T>(v/x);
  Assert(Norm(a-v) <= eps*normv,label+" a /= x");
  a = v = a0;

#ifdef ALIASOK
  a = a*x;
  v = tmv::Vector<T>(v*x);
  Assert(Norm(a-v) <= eps*normv,label+" a = a*x");
  a = v = a0;

  a = x*a;
  v = tmv::Vector<T>(v*x);
  Assert(Norm(a-v) <= eps*normv,label+" a = x*a");
  a = v = a0;

  a = a/x;
  v = tmv::Vector<T>(v/x);
  Assert(Norm(a-v) <= eps*normv,label+" a = a/x");
  a = a0;
#endif

  if (showstartdone) {
    std::cout<<"Done VX2 "<<std::endl;
  }
}

template <class T, class T2, class V1, class V2> inline void TestVV(
    const V1& a, const V2& b, std::string label)
{
  if (showstartdone) {
    std::cout<<"Start VV "<<label<<std::endl;
    std::cout<<"a = "<<tmv::TypeText(a)<<std::endl;
    std::cout<<"b = "<<tmv::TypeText(b)<<std::endl;
  }

  tmv::Vector<T> v1 = a;
  tmv::Vector<T2> v2 = b;
  RealType(T) eps = EPS * (Norm(v1) + Norm(v2));
  RealType(T) eps2 = EPS * Norm(v1) * Norm(v2);
  Assert(Norm(a-v1) <= EPS*Norm(v1),label+" a != v1");
  Assert(Norm(b-v2) <= EPS*Norm(v2),label+" b != v2");

  if (XXDEBUG4) {
    std::cout<<"a = "<<tmv::TypeText(a)<<" = "<<a<<std::endl;
    std::cout<<"b = "<<tmv::TypeText(b)<<" = "<<b<<std::endl;
    std::cout<<"v1 = "<<tmv::TypeText(v1)<<" = "<<v1<<std::endl;
    std::cout<<"v2 = "<<tmv::TypeText(v2)<<" = "<<v2<<std::endl;
    std::cout<<"a+b = "<<(a+b)<<std::endl;
    std::cout<<"v1+v2 = "<<(v1+v2)<<std::endl;
    std::cout<<"Norm(diff) = "<<Norm((a+b)-(v1+v2))<<std::endl;
    std::cout<<"eps = "<<eps<<std::endl;
  }

  Assert(Norm((a+b)-(v1+v2)) <= eps,label+" a+b");
  Assert(Norm((a-b)-(v1-v2)) <= eps,label+" a-b");
  Assert(tmv::TMV_ABS((a*b)-(v1*v2)) <= eps2,label+" a*b");
  Assert(Norm((a+v2)-(v1+v2)) <= eps,label+" a+v");
  Assert(Norm((v1+b)-(v1+v2)) <= eps,label+" v+b");
  Assert(Norm((a-v2)-(v1-v2)) <= eps,label+" a-v");
  Assert(Norm((v1-b)-(v1-v2)) <= eps,label+" v-b");
  Assert(tmv::TMV_ABS((a*v2)-(v1*v2)) <= eps2,label+" a*v");
  Assert(tmv::TMV_ABS((v1*b)-(v1*v2)) <= eps2,label+" v*b");

  RealType(T) x(5);
  ComplexType(T) z(3,4);
  if (XXDEBUG4) {
    std::cout<<"a-x*b = "<<(a-x*b)<<std::endl;
    std::cout<<"v1-x*v2 = "<<(v1-x*v2)<<std::endl;
    std::cout<<"Norm(diff) = "<<Norm((a-x*b)-(v1-x*v2))<<std::endl;
    std::cout<<"eps*x = "<<x*eps<<std::endl;
  }
  Assert(Norm((a-x*b)-(v1-x*v2)) <= x*eps,label+" a-x*b");
  Assert(Norm((a+x*b)-(v1+x*v2)) <= x*eps,label+" a+x*b");
  Assert(Norm((x*a-b)-(x*v1-v2)) <= x*eps,label+" x*a-b");
  Assert(Norm((x*a+b)-(x*v1+v2)) <= x*eps,label+" x*a+b");
  Assert(Norm((x*a-x*b)-(x*v1-x*v2)) <= x*eps,label+" x*a-x*b");
  Assert(Norm((x*a+x*b)-(x*v1+x*v2)) <= x*eps,label+" x*a+x*b");

  Assert(Norm((a-z*b)-(v1-z*v2)) <= x*eps,label+" a-z*b");
  Assert(Norm((a+z*b)-(v1+z*v2)) <= x*eps,label+" a+z*b");
  if (XXDEBUG4) {
    std::cout<<"z*a-b = "<<(z*a-b)<<std::endl;
    std::cout<<"z*v1-v2 = "<<(z*v1-v2)<<std::endl;
    std::cout<<"Norm(diff) = "<<Norm((z*a-b)-(z*v1-v2))<<std::endl;
    std::cout<<"eps*x = "<<x*eps<<std::endl;
  }
  Assert(Norm((z*a-b)-(z*v1-v2)) <= x*eps,label+" z*a-b");
  Assert(Norm((z*a+b)-(z*v1+v2)) <= x*eps,label+" z*a+b");
  Assert(Norm((z*a-z*b)-(z*v1-z*v2)) <= x*eps,label+" z*a-z*b");
  Assert(Norm((z*a+z*b)-(z*v1+z*v2)) <= x*eps,label+" z*a+z*b");

  Assert(Norm((z*a-x*b)-(z*v1-x*v2)) <= x*eps,label+" z*a-x*b");
  Assert(Norm((z*a+x*b)-(z*v1+x*v2)) <= x*eps,label+" z*a+x*b");
  Assert(Norm((x*a-z*b)-(x*v1-z*v2)) <= x*eps,label+" x*a-z*b");
  Assert(Norm((x*a+z*b)-(x*v1+z*v2)) <= x*eps,label+" x*a+z*b");

  Assert(tmv::TMV_ABS(((x*a)*b)-(x*v1*v2)) <= x*eps2,label+" (x*a)*b");
  Assert(tmv::TMV_ABS((a*(x*b))-(x*v1*v2)) <= x*eps2,label+" a*(x*b)");
  Assert(tmv::TMV_ABS((x*(a*b))-(x*v1*v2)) <= x*eps2,label+" x*(a*b)");

  Assert(tmv::TMV_ABS(((z*a)*b)-(z*v1*v2)) <= x*eps2,label+" (z*a)*b");
  Assert(tmv::TMV_ABS((a*(z*b))-(z*v1*v2)) <= x*eps2,label+" a*(z*b)");
  Assert(tmv::TMV_ABS((z*(a*b))-(z*v1*v2)) <= x*eps2,label+" z*(a*b)");

  Assert(tmv::TMV_ABS(((x*a)*(x*b))-(x*x*v1*v2)) <= x*x*eps2,label+" (x*a)*(x*b)");
  Assert(tmv::TMV_ABS(((z*a)*(x*b))-(z*x*v1*v2)) <= x*x*eps2,label+" (z*a)*(x*b)");
  Assert(tmv::TMV_ABS(((x*a)*(z*b))-(x*z*v1*v2)) <= x*x*eps2,label+" (x*a)*(z*b)");
  Assert(tmv::TMV_ABS(((z*a)*(z*b))-(z*z*v1*v2)) <= x*x*eps2,label+" (z*a)*(z*b)");

  if (showstartdone) {
    std::cout<<"Done VV "<<std::endl;
  }
}

template <class T, class T2, class V1, class V2> inline void TestVV2(
    V1& a, const V2& b, std::string label)
{
  if (showstartdone) {
    std::cout<<"Start VV2 "<<label<<std::endl;
    std::cout<<"a = "<<tmv::TypeText(a)<<std::endl;
    std::cout<<"b = "<<tmv::TypeText(b)<<std::endl;
  }

  tmv::Vector<T> v1 = a;
  tmv::Vector<T2> v2 = b;
  Assert(Norm(a-v1) <= EPS*Norm(v1),label+" a != v1");
  Assert(Norm(b-v2) <= EPS*Norm(v2),label+" b != v2");

  double normv = Norm(v1)+Norm(v2);
  RealType(T) eps = EPS*normv;

  tmv::Vector<T> v4 = v1;

#if 1
  tmv::Vector<T> v3 = v1;
  if (XXDEBUG5) {
    std::cout<<"a = "<<tmv::TypeText(a)<<" = "<<a<<std::endl;
    std::cout<<"b = "<<tmv::TypeText(b)<<" = "<<b<<std::endl;
    std::cout<<"v3+=b = "<<(v3+=b)<<std::endl;
    std::cout<<"v1+v2 = "<<(v1+v2)<<std::endl;
    std::cout<<"Norm(diff) = "<<Norm(v3-(v1+v2))<<std::endl;
    std::cout<<"eps = "<<eps<<std::endl;
    v3 = v1;
  }

  v3 += b;
  v4 = v1+v2;
  Assert(Norm(v3-v4) <= eps,label+" v += b");
  v3 = v4 = v1;
  v3 -= b;
  v4 = v1-v2;
  Assert(Norm(v3-v4) <= eps,label+" v -= b");
  v3 = v4 = v1;
  v3 = v3+b;
  v4 = v1+v2;
  Assert(Norm(v3-v4) <= eps,label+" v = v+b");
  v3 = v4 = v1;
  v3 = v3-b;
  v4 = v1-v2;
  Assert(Norm(v3-v4) <= eps,label+" v = v-b");
  v3 = v4 = v1;
  v3 = b+v3;
  v4 = v1+v2;
  Assert(Norm(v3-v4) <= eps,label+" v = b+v");
  v3 = v4 = v1;
  v3 = b-v3;
  v4 = v2-v1;
  Assert(Norm(v3-v4) <= eps,label+" v = b-v");
  v3 = v4 = v1;
  a += v2;
  v4 = v1+v2;
  Assert(Norm(a-v4) <= eps,label+" a += v");
  a = v4 = v1;
  a -= v2;
  v4 = v1-v2;
  Assert(Norm(a-v4) <= eps,label+" a -= v");
  a = v4 = v1;
#ifdef ALIASOK
  a = a+v2;
  v4 = v1+v2;
  Assert(Norm(a-v4) <= eps,label+" a = a+v");
  a = v4 = v1;
  a = a-v2;
  v4 = v1-v2;
  Assert(Norm(a-v4) <= eps,label+" a = a-v");
  a = v4 = v1;
  a = v2+a;
  v4 = v1+v2;
  Assert(Norm(a-v4) <= eps,label+" a = v+a");
  a = v4 = v1;
  a = v2-a;
  v4 = v2-v1;
  Assert(Norm(a-v4) <= eps,label+" a = v-a");
  a = v4 = v1;
#endif
#endif

  if (XXDEBUG5) {
    std::cout<<"a = "<<tmv::TypeText(a)<<" = "<<a<<std::endl;
    std::cout<<"b = "<<tmv::TypeText(b)<<" = "<<b<<std::endl;
    std::cout<<"a+=b = "<<(a+=b)<<std::endl;
    std::cout<<"a-=b = "<<(a-=b)<<std::endl;
    std::cout<<"v1+v2 = "<<(v1+v2)<<std::endl;
    std::cout<<"Norm(diff) = "<<Norm(a-(v1+v2))<<std::endl;
    std::cout<<"eps = "<<eps<<std::endl;
    a = v1;
  }

  a += b;
  v4 = v1+v2;
  Assert(Norm(a-v4) <= eps,label+" a += b");
  a = v4 = v1;
  a -= b;
  v4 = v1-v2;
  Assert(Norm(a-v4) <= eps,label+" a -= b");
  a = v4 = v1;
  a += -b;
  v4 = v1-v2;
  Assert(Norm(a-v4) <= eps,label+" a += -b");
  a = v4 = v1;
  a -= -b;
  v4 = v1+v2;
  Assert(Norm(a-v4) <= eps,label+" a -= -b");
  a = v4 = v1;
#ifdef ALIASOK
  a = a+b;
  v4 = v1+v2;
  Assert(Norm(a-v4) <= eps,label+" a = a+b");
  a = v4 = v1;
  a = a-b;
  v4 = v1-v2;
  Assert(Norm(a-v4) <= eps,label+" a = a-b");
  a = v4 = v1;
  a = b+a;
  v4 = v1+v2;
  Assert(Norm(a-v4) <= eps,label+" a = b+a");
  a = v4 = v1;
  a = b-a;
  v4 = v2-v1;
  Assert(Norm(a-v4) <= eps,label+" a = b-a");
  a = v4 = v1;
#endif

  if (showstartdone) {
    std::cout<<"Done VV2 "<<std::endl;
  }
}

template <class T, class V, class CV> inline void TestVectorArith1(
    V& a, CV& ca, std::string label)
{
  if (showstartdone) {
    std::cout<<"Start VectorArith1 "<<label<<std::endl;
    std::cout<<"a = "<<tmv::TypeText(a)<<"  "<<a<<std::endl;
    std::cout<<"ca = "<<tmv::TypeText(ca)<<"  "<<ca<<std::endl;
  }

  TestV<T>(a,label+" R");
  TestV<std::complex<T> >(ca,label+" C");

  T x = 12;
  std::complex<T> z(9.-2);

  TestVX<T>(a,x,label+" R,R");
  TestVX2<T>(a,x,label+" R,R");
  TestVX<T>(a,z,label+" R,C");
  TestVX<std::complex<T> >(ca,x,label+" C,R");
  TestVX2<std::complex<T> >(ca,x,label+" C,R");
  TestVX<std::complex<T> >(ca,z,label+" C,C");
  TestVX2<std::complex<T> >(ca,z,label+" C,C");
#ifdef ALIASOK
  TestVV<T,T>(a,a,label+" R,R");
  TestVV2<T,T>(a,a,label+" R,R");
  TestVV<std::complex<T>,std::complex<T> >(ca,ca,label+" C,C");
  TestVV2<std::complex<T>,std::complex<T> >(ca,ca,label+" C,C");
#endif

}
template <class T, class V1, class CV1, class V2, class CV2> 
inline void TestVectorArith2(V1& a, CV1& ca, const V2& b, const CV2& cb,
    std::string label)
{
  if (showstartdone) {
    std::cout<<"Start VectorArith2 "<<label<<std::endl;
    std::cout<<"a = "<<tmv::TypeText(a)<<"  "<<a<<std::endl;
    std::cout<<"ca = "<<tmv::TypeText(ca)<<"  "<<ca<<std::endl;
    std::cout<<"b = "<<tmv::TypeText(b)<<"  "<<b<<std::endl;
    std::cout<<"cb = "<<tmv::TypeText(cb)<<"  "<<cb<<std::endl;
  }

  TestVV<T,T>(a,b,label+" R,R");
  TestVV2<T,T>(a,b,label+" R,R");
  TestVV<T,std::complex<T> >(a,cb,label+" R,C");
  TestVV<std::complex<T>,T>(ca,b,label+" C,R");
  TestVV2<std::complex<T>,T>(ca,b,label+" C,R");
  TestVV<std::complex<T>,std::complex<T> >(ca,cb,label+" C,C");
  TestVV2<std::complex<T>,std::complex<T> >(ca,cb,label+" C,C");
}

#endif
