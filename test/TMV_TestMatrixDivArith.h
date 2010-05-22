#define CT std::complex<T>

template <class SM1, class SM2> inline bool CanLDiv(
    const SM1& a, const SM2& b)
{ return a.colsize() == b.colsize(); }

template <class SM1, class SM2, class SM3> inline bool CanLDiv(
    const SM1& a, const SM2& b, const SM3& c)
{ 
    return a.colsize() == b.colsize() && a.rowsize() == c.colsize() 
        && b.rowsize() == c.rowsize(); 
}

template <class SM1, class SM2> inline bool CanLDivEq(
    const SM1& a, const SM2& b)
{ return CanLDiv(a,b) && b.isSquare(); }

template <class V1, class SM2> inline bool CanLDivVM(
    const V1& a, const SM2& b)
{ return a.size() == b.colsize(); }

template <class V1, class SM2, class V3> inline bool CanLDivVM(
    const V1& a, const SM2& b, const V3& c)
{ return a.size() == b.colsize() && c.size() == b.rowsize(); }

template <class V1, class SM2> inline bool CanLDivEqVM(
    const V1& a, const SM2& b)
{ return CanLDivVM(a,b) && b.isSquare(); }

template <class SM1, class SM2> inline bool CanRDiv(
    const SM1& a, const SM2& b)
{ return a.rowsize() == b.rowsize(); }

template <class SM1, class SM2, class SM3> inline bool CanRDiv(
    const SM1& a, const SM2& b, const SM3& c)
{ 
    return a.rowsize() == b.rowsize() && a.colsize() == c.colsize() 
        && b.colsize() == c.rowsize(); 
}

template <class SM1, class SM2> inline bool CanRDivEq(
    const SM1& a, const SM2& b)
{ return CanRDiv(a,b) && b.isSquare(); }

template <class V1, class SM2> inline bool CanRDivVM(
    const V1& a, const SM2& b)
{ return a.size() == b.rowsize(); }

template <class V1, class SM2, class V3> inline bool CanRDivVM(
    const V1& a, const SM2& b, const V3& c)
{ return a.size() == b.rowsize() && c.size() == b.colsize(); }

template <class V1, class SM2> inline bool CanRDivEqVM(
    const V1& a, const SM2& b)
{ return CanRDivVM(a,b) && b.isSquare(); }

template <class T1, class T2> struct ProdType
{ typedef T1 Tprod; };

template <class T> struct ProdType<T,std::complex<T> >
{ typedef std::complex<T> Tprod; };

#define RealType(T) typename tmv::Traits<T>::real_type
#define ComplexType(T) typename tmv::Traits<T>::complex_type

#define ProductType(T1,T2) typename ProdType<T1,T2>::Tprod

#ifdef NOMIX
#define VEC(T,v) (tmv::Vector<T>(v))
#define VEC2(T1,T2,v) (tmv::Vector<ProductType(T1,T2)>(v))
#define MAT(T,m) (tmv::Matrix<T>(m))
#define MAT2(T1,T2,m) (tmv::Matrix<ProductType(T1,T2)>(m))
#else
#define VEC(T,v) (v)
#define VEC2(T1,T2,v) (v)
#define MAT(T,m) (m)
#define MAT2(T1,T2,m) (m)
#endif

template <class T, class Tb, class V, class MM> 
static void DoTestLDivVM1a(
    tmv::DivType dt, const V& a, const MM& b, std::string label)
{
    if (showstartdone) std::cout<<"Start LDiv VM1a: "<<label<<std::endl;

    tmv::Vector<T> v = a;
    tmv::Matrix<Tb> m = b;
    m.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m.saveDiv();

    double eps = EPS*Norm(m)*Norm(tmv::Matrix<Tb>(m.inverse()))*v.size();

    if (CanLDivVM(a,b)) {
        tmv::Vector<ProductType(T,Tb)> frac = v/m;
        eps *= Norm(frac);
        if (XXDEBUG1) {
            std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
            std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
            std::cout<<"v = "<<tmv::TMV_Text(v)<<"  "<<v<<std::endl;
            std::cout<<"m = "<<tmv::TMV_Text(m)<<"  "<<m<<std::endl;
            std::cout<<"a/b = "<<a/b<<std::endl;
            std::cout<<"v/m = "<<frac<<std::endl;
            std::cout<<"Norm(diff) = "<<Norm(VEC2(T,Tb,a/b)-frac)<<std::endl;
            std::cout<<"eps = "<<eps<<std::endl;
        }
        Assert(Norm(VEC2(T,Tb,a/b)-frac) <= eps,label+" a/b");
        RealType(T) x(5);
        ComplexType(T) z(3,4);
        Assert(Norm(VEC2(T,Tb,x*a/b)-x*frac) <= x*eps,label+" x*a/b");
        if (XXDEBUG1) {
            std::cout<<"z*a/b = "<<VEC(ComplexType(T),z*a/b)<<std::endl;
            std::cout<<"z*frac = "<<z*frac<<std::endl;
            std::cout<<"Norm(diff) = "<<Norm(VEC(ComplexType(T),z*a/b)-z*frac)
                <<" x*eps = "<<x*eps<<std::endl;
        }
        Assert(Norm(VEC(ComplexType(T),z*a/b)-z*frac) <= x*eps,label+" z*a/b");
#if (XTEST & 16)
        Assert(Norm(VEC2(T,Tb,b.inverse()*a)-frac) <= eps,label+" b^-1*a");
#ifndef NOMIX
        Assert(Norm(VEC2(T,Tb,a/m)-frac) <= eps,label+" a/m");
        Assert(Norm(VEC2(T,Tb,m.inverse()*a)-frac) <= eps,label+" m^-1*a");
        Assert(Norm(VEC2(T,Tb,v/b)-frac) <= eps,label+" v/b");
        Assert(Norm(VEC2(T,Tb,b.inverse()*v)-frac) <= eps,label+" b^-1*v");
#endif

        Assert(Norm(VEC2(T,Tb,(x*a)/b)-x*frac) <= x*eps,label+" (x*a)/b");
        Assert(Norm(VEC2(T,Tb,x*(a/b))-x*frac) <= x*eps,label+" x*(a/b)");
        Assert(Norm(VEC2(T,Tb,a/(x*b))-frac/x) <= eps/x,label+" a/(x*b)");

        Assert(Norm(VEC(ComplexType(T),(z*a)/b)-z*frac) <= x*eps,label+" (z*a)/b");
        Assert(Norm(VEC(ComplexType(T),z*(a/b))-z*frac) <= x*eps,label+" z*(a/b)");
        Assert(Norm(VEC(ComplexType(T),a/(z*b))-frac/z) <= eps/x,label+" a/(z*b)");

        Assert(Norm(VEC2(T,Tb,(x*a)/(x*b))-frac) <= eps,
               label+" (x*a)/(x*b)");
        Assert(Norm(VEC(ComplexType(T),(z*a)/(x*b))-(z/x)*frac) <= eps,
               label+" (z*a)/(x*b)");
        Assert(Norm(VEC(ComplexType(T),(x*a)/(z*b))-(x/z)*frac) <= eps,
               label+" (x*a)/(z*b)");
        Assert(Norm(VEC(ComplexType(T),(z*a)/(z*b))-frac) <= eps,
               label+" (z*a)/(z*b)");
#endif
    }

    if (showstartdone) std::cout<<"Done LDiv VM1a"<<std::endl;
}

template <class T, class Tb, class V, class MM> 
static void DoTestRDivVM1a(
    tmv::DivType dt, const V& a, const MM& b, std::string label)
{
    if (showstartdone) std::cout<<"Start RDiv VM1a: "<<label<<std::endl;

    tmv::Vector<T> v = a;
    tmv::Matrix<Tb> m = b;
    m.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m.saveDiv();

    double eps = EPS*Norm(m)*Norm(tmv::Matrix<Tb>(m.inverse()))*v.size();

    if (CanRDivVM(a,b)) {
        tmv::Vector<ProductType(T,Tb)> frac = v%m;
        eps *= Norm(frac);
        if (XXDEBUG2) {
            std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
            std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
            std::cout<<"v = "<<tmv::TMV_Text(v)<<"  "<<v<<std::endl;
            std::cout<<"m = "<<tmv::TMV_Text(m)<<"  "<<m<<std::endl;
            std::cout<<"a%b = "<<a%b<<std::endl;
            std::cout<<"v%m = "<<frac<<std::endl;
            std::cout<<"Norm(diff) = "<<Norm(VEC2(T,Tb,a%b)-frac)<<std::endl;
            std::cout<<"eps = "<<eps<<std::endl;
        }
        Assert(Norm(VEC2(T,Tb,a%b)-frac) <= eps,label+" a%b");
        RealType(T) x(5);
        ComplexType(T) z(3,4);
        Assert(Norm(VEC2(T,Tb,x*a%b)-x*frac) <= x*eps,label+" x*a%b");
        Assert(Norm(VEC(ComplexType(T),z*a%b)-z*frac) <= x*eps,label+" z*a%b");
#if (XTEST & 16)
        Assert(Norm(VEC2(T,Tb,a*b.inverse())-frac) <= eps,label+" a*b^-1");
#ifndef NOMIX
        Assert(Norm(VEC2(T,Tb,a%m)-frac) <= eps,label+" a%m");
        Assert(Norm(VEC2(T,Tb,a*m.inverse())-frac) <= eps,label+" a*m^-1");
        Assert(Norm(VEC2(T,Tb,v%b)-frac) <= eps,label+" v%b");
        Assert(Norm(VEC2(T,Tb,v*b.inverse())-frac) <= eps,label+" v*b^-1");
#endif

        Assert(Norm(VEC2(T,Tb,(x*a)%b)-x*frac) <= x*eps,label+" (x*a)%b");
        Assert(Norm(VEC2(T,Tb,x*(a%b))-x*frac) <= x*eps,label+" x*(a%b)");
        Assert(Norm(VEC2(T,Tb,a%(x*b))-frac/x) <= eps/x,label+" a%(x*b)");

        Assert(Norm(VEC(ComplexType(T),(z*a)%b)-z*frac) <= x*eps,label+" (z*a)%b");
        Assert(Norm(VEC(ComplexType(T),z*(a%b))-z*frac) <= x*eps,label+" z*(a%b)");
        Assert(Norm(VEC(ComplexType(T),a%(z*b))-frac/z) <= eps/x,label+" a%(z*b)");

        Assert(Norm(VEC2(T,Tb,(x*a)%(x*b))-frac) <= eps,
               label+" (x*a)%(x*b)");
        Assert(Norm(VEC(ComplexType(T),(z*a)%(x*b))-(z/x)*frac) <= eps,
               label+" (z*a)%(x*b)");
        Assert(Norm(VEC(ComplexType(T),(x*a)%(z*b))-(x/z)*frac) <= eps,
               label+" (x*a)%(z*b)");
        Assert(Norm(VEC(ComplexType(T),(z*a)%(z*b))-frac) <= eps,
               label+" (z*a)%(z*b)");
#endif
    }

    if (showstartdone) std::cout<<"Done RDiv VM1a"<<std::endl;
}

template <class T, class Tb, class V, class MM> 
static void DoTestLDivVM1R(
    tmv::DivType dt, const V& a, const MM& b, std::string label)
{
    DoTestLDivVM1a<T,Tb>(dt,a,b,label);

#ifndef NOVIEWS
    DoTestLDivVM1a<T,Tb>(dt,a.reverse(),b,label+" RevA");
#endif
}

template <class T, class Tb, class V, class MM> 
static void DoTestLDivVM1C(
    tmv::DivType dt, const V& a, const MM& b, std::string label)
{
    DoTestLDivVM1a<T,Tb>(dt,a,b,label);

#ifndef NOVIEWS
    DoTestLDivVM1a<T,Tb>(dt,a.reverse(),b,label+" RevA");
    DoTestLDivVM1a<T,Tb>(dt,a.conjugate(),b,label+" ConjA");
#endif
}

template <class T, class Tb, class V, class MM> 
static void DoTestRDivVM1R(
    tmv::DivType dt, const V& a, const MM& b, std::string label)
{
    DoTestRDivVM1a<T,Tb>(dt,a,b,label);

#ifndef NOVIEWS
    DoTestRDivVM1a<T,Tb>(dt,a.reverse(),b,label+" RevA");
#endif
}

template <class T, class Tb, class V, class MM> 
static void DoTestRDivVM1C(
    tmv::DivType dt, const V& a, const MM& b, std::string label)
{
    DoTestRDivVM1a<T,Tb>(dt,a,b,label);

#ifndef NOVIEWS
    DoTestRDivVM1a<T,Tb>(dt,a.reverse(),b,label+" RevA");
    DoTestRDivVM1a<T,Tb>(dt,a.conjugate(),b,label+" ConjA");
#endif
}

template <class T> inline void SetZ(T& z)
{ z = T(5); }
template <class T> inline void SetZ(std::complex<T>& z)
{ z = std::complex<T>(3,4); }

#ifndef NOLDIVEQ
template <class T, class Tb, class V, class MM> 
static void DoTestLDivVM2a(
    tmv::DivType dt, V& a, const MM& b, std::string label)
{
    if (showstartdone) std::cout<<"Start LDiv VM2b: "<<label<<std::endl;

    tmv::Vector<T> v = a;
    tmv::Matrix<Tb> m = b;
    m.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m.saveDiv();

    double eps = EPS*Norm(m)*Norm(tmv::Matrix<Tb>(m.inverse()))*v.size();

    if (CanLDivEqVM(a,b)) {
        tmv::Vector<T> a0 = v;
        tmv::Vector<T> frac = v/m;
        eps *= Norm(frac);
        a /= b;
        Assert(Norm(VEC(T,a)-frac) <= eps,label+" a/=b");
        a = a0;
#ifdef ALIASOK
        a = a / b;
        Assert(Norm(VEC(T,a)-frac) <= eps,label+" a=a/b");
        a = a0;
        a = b.inverse() * a;
        Assert(Norm(VEC(T,a)-frac) <= eps,label+" a=b^-1*a");
        a = a0;
#if (XTEST & 16)
        RealType(T) x(5);
        T z;  SetZ(z);
        a = x * a / b;
        Assert(Norm(VEC(T,a)-x*frac) <= x*eps,label+" a=x*a/b");
        a = a0;
        a = x * b.inverse() * a;
        Assert(Norm(VEC(T,a)-x*frac) <= x*eps,label+" a=x*b^-1*a");
        a = a0;
        a = z * a / b;
        Assert(Norm(VEC(T,a)-z*frac) <= x*eps,label+" a=z*a/b");
        a = a0;
        a = z * b.inverse() * a;
        Assert(Norm(VEC(T,a)-z*frac) <= x*eps,label+" a=z*b^-1*a");
        a = a0;
#endif
#endif
    }
    if (showstartdone) std::cout<<"Done LDiv VM2a"<<std::endl;
}

template <class T, class Tb, class V, class MM> 
static void DoTestLDivVM2R(
    tmv::DivType dt, V& a, const MM& b, std::string label)
{
    DoTestLDivVM2a<T,Tb>(dt,a,b,label);
#ifndef NOVIEWS
    typename V::reverse_type ar = a.reverse();
    DoTestLDivVM2a<T,Tb>(dt,ar,b,label+" RevA");
#endif

#if (XTEST & 16)
    tmv::Vector<T> a0 = a;
    a.setZero();
    DoTestLDivVM2a<T,Tb>(dt,a,b,label+" 1");
    a = a0;

#ifndef NOVIEWS
    a.subVector(0,a.size()/2).setZero();
    DoTestLDivVM2a<T,Tb>(dt,a,b,label+" 2");
    a = a0;

    a.subVector(a.size()/2,a.size()).setZero();
    DoTestLDivVM2a<T,Tb>(dt,a,b,label+" 3");
    a = a0;

    a.subVector(0,a.size()/4).setZero();
    a.subVector(3*a.size()/4,a.size()).setZero();
    DoTestLDivVM2a<T,Tb>(dt,a,b,label+" 4");
    a = a0;
#endif
#endif
}

template <class T, class Tb, class V, class MM> 
static void DoTestLDivVM2C(
    tmv::DivType dt, V& a, const MM& b, std::string label)
{
    DoTestLDivVM2a<T,Tb>(dt,a,b,label);
#ifndef NOVIEWS
    typename V::reverse_type ar = a.reverse();
    typename V::conjugate_type ac = a.conjugate();
    DoTestLDivVM2a<T,Tb>(dt,ar,b,label+" RevA");
    DoTestLDivVM2a<T,Tb>(dt,ac,b,label+" ConjA");
#endif

#if (XTEST & 16)
    tmv::Vector<T> a0 = a;
    a.setZero();
    DoTestLDivVM2a<T,Tb>(dt,a,b,label+" 1");
    a = a0;

#ifndef NOVIEWS
    a.subVector(0,a.size()/2).setZero();
    DoTestLDivVM2a<T,Tb>(dt,a,b,label+" 2");
    a = a0;

    a.subVector(a.size()/2,a.size()).setZero();
    DoTestLDivVM2a<T,Tb>(dt,a,b,label+" 3");
    a = a0;

    a.subVector(0,a.size()/4).setZero();
    a.subVector(3*a.size()/4,a.size()).setZero();
    DoTestLDivVM2a<T,Tb>(dt,a,b,label+" 4");
    a = a0;
#endif
#endif
}
#endif

#ifndef NORDIVEQ
template <class T, class Tb, class V, class MM> 
static void DoTestRDivVM2a(
    tmv::DivType dt, V& a, const MM& b, std::string label)
{
    if (showstartdone) std::cout<<"Start RDiv VM2a: "<<label<<std::endl;

    tmv::Vector<T> v = a;
    tmv::Matrix<Tb> m = b;
    m.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m.saveDiv();

    double eps = EPS*Norm(m)*Norm(tmv::Matrix<Tb>(m.inverse()))*v.size();

    if (CanRDivEqVM(a,b)) {
        tmv::Vector<T> a0 = a;
        tmv::Vector<T> frac = v%m;
        eps *= Norm(frac);
        if (XXDEBUG2) {
            std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a.step()<<"  "<<a<<std::endl;
            std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
            std::cout<<"a % b = "<<frac<<std::endl;
        }
        a %= b;
        if (XXDEBUG2) {
            std::cout<<"a %= b = "<<a<<std::endl;
        }
        Assert(Norm(VEC(T,a)-frac) <= eps,label+" a%=b");
        a = a0;
        a *= b.inverse();
        if (XXDEBUG2) {
            std::cout<<"a *= b.inv = "<<a<<std::endl;
        }
        Assert(Norm(VEC(T,a)-frac) <= eps,label+" a*=b^-1");
        a = a0;
#ifdef ALIASOK
        a = a % b;
        Assert(Norm(VEC(T,a)-frac) <= eps,label+" a=a%b");
        a = a0;
        a = a * b.inverse();
        Assert(Norm(VEC(T,a)-frac) <= eps,label+" a=a*b^-1");
        a = a0;
#if (XTEST & 16)
        RealType(T) x(5);
        T z;  SetZ(z);
        a = x * a % b;
        Assert(Norm(VEC(T,a)-x*frac) <= x*eps,label+" a=x*a%b");
        a = a0;
        a = x * a * b.inverse();
        Assert(Norm(VEC(T,a)-x*frac) <= x*eps,label+" a=x*a*b^-1");
        a = a0;
        a = z * a % b;
        Assert(Norm(VEC(T,a)-z*frac) <= x*eps,label+" a=z*a%b");
        a = a0;
        a = z * a * b.inverse();
        Assert(Norm(VEC(T,a)-z*frac) <= x*eps,label+" a=z*a*b^-1");
        a = a0;
#endif
#endif
    }
    if (showstartdone) std::cout<<"Done RDiv VM2a"<<std::endl;
}

template <class T, class Tb, class V, class MM> 
static void DoTestRDivVM2R(
    tmv::DivType dt, V& a, const MM& b, std::string label)
{
    DoTestRDivVM2a<T,Tb>(dt,a,b,label);
#ifndef NOVIEWS
    typename V::reverse_type ar = a.reverse();
    DoTestRDivVM2a<T,Tb>(dt,ar,b,label+" RevA");
#endif

#if (XTEST & 16)
    tmv::Vector<T> a0 = a;
    a.setZero();
    DoTestRDivVM2a<T,Tb>(dt,a,b,label+" 1");
    a = a0;

#ifndef NOVIEWS
    a.subVector(0,a.size()/2).setZero();
    DoTestRDivVM2a<T,Tb>(dt,a,b,label+" 2");
    a = a0;

    a.subVector(a.size()/2,a.size()).setZero();
    DoTestRDivVM2a<T,Tb>(dt,a,b,label+" 3");
    a = a0;

    a.subVector(0,a.size()/4).setZero();
    a.subVector(3*a.size()/4,a.size()).setZero();
    DoTestRDivVM2a<T,Tb>(dt,a,b,label+" 4");
    a = a0;
#endif
#endif
}

template <class T, class Tb, class V, class MM> 
static void DoTestRDivVM2C(
    tmv::DivType dt, V& a, const MM& b, std::string label)
{
    DoTestRDivVM2a<T,Tb>(dt,a,b,label);
#ifndef NOVIEWS
    typename V::reverse_type ar = a.reverse();
    typename V::conjugate_type ac = a.conjugate();
    DoTestRDivVM2a<T,Tb>(dt,ar,b,label+" RevA");
    DoTestRDivVM2a<T,Tb>(dt,ac,b,label+" ConjA");
#endif

#if (XTEST & 16)
    tmv::Vector<T> a0 = a;
    a.setZero();
    DoTestRDivVM2a<T,Tb>(dt,a,b,label+" 1");
    a = a0;

#ifndef NOVIEWS
    a.subVector(0,a.size()/2).setZero();
    DoTestRDivVM2a<T,Tb>(dt,a,b,label+" 2");
    a = a0;

    a.subVector(a.size()/2,a.size()).setZero();
    DoTestRDivVM2a<T,Tb>(dt,a,b,label+" 3");
    a = a0;

    a.subVector(0,a.size()/4).setZero();
    a.subVector(3*a.size()/4,a.size()).setZero();
    DoTestRDivVM2a<T,Tb>(dt,a,b,label+" 4");
    a = a0;
#endif
#endif
}
#endif

template <class Ta, class Tb, class T, class V1, class MM, class V2>
static void DoTestLDivVM3a(
    tmv::DivType dt, const V1& a, const MM& b, V2& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start LDiv VM3a"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    tmv::Vector<Ta> v1 = a;
    tmv::Matrix<Tb> m = b;
    tmv::Vector<T> v2 = c;
    m.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m.saveDiv();

    double eps = EPS*Norm(m)*Norm(tmv::Matrix<Tb>(m.inverse()))*v1.size();

    if (XXDEBUG3) {
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a.step()<<"  "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<"  "<<c.step()<<"  "<<c<<std::endl;
    }

    if (CanLDivVM(a,b,c)) {
        v2 = v1/m;
        eps *= Norm(v2);
        c = a/b;
        if (XXDEBUG3) {
            std::cout<<"v/m = "<<v2<<std::endl;
            std::cout<<"a/b = "<<c<<std::endl;
        }
        Assert(Norm(VEC(T,c)-v2) <= eps,label+" c=a/b");
#if (XTEST & 16)
        c = -a/b;
        Assert(Norm(VEC(T,c)-(-v2)) <= eps,label+" c=-a/b");
        RealType(T) x(5);
        T z; SetZ(z);
        c = x*a/b;
        Assert(Norm(VEC(T,c)-(x*v2)) <= x*eps,label+" c=x*a/b");
        c = a/b*x;
        Assert(Norm(VEC(T,c)-(x*v2)) <= x*eps,label+" c=a/b*x");
        c = (x*a)/b;
        Assert(Norm(VEC(T,c)-(x*v2)) <= x*eps,label+" c=(x*a)/b");
        c = z*a/b;
        Assert(Norm(VEC(T,c)-(z*v2)) <= x*eps,label+" c=z*a/b");
        c = a/b*z;
        Assert(Norm(VEC(T,c)-(z*v2)) <= x*eps,label+" c=a/b*z");
        c = (z*a)/b;
        Assert(Norm(VEC(T,c)-(z*v2)) <= x*eps,label+" c=(z*a)/b");
#endif
    }

    if (showstartdone) std::cout<<"Done LDiv VM3a"<<std::endl;
}

template <class Ta, class Tb, class T, class V1, class MM, class V2>
static void DoTestRDivVM3a(
    tmv::DivType dt, const V1& a, const MM& b, V2& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start RDiv VM3a"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    tmv::Vector<Ta> v1 = a;
    tmv::Matrix<Tb> m = b;
    tmv::Vector<T> v2 = c;
    m.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m.saveDiv();

    double eps = EPS*Norm(m)*Norm(tmv::Matrix<Tb>(m.inverse()))*v1.size();

    if (XXDEBUG4) {
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a.step()<<"  "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<"  "<<c.step()<<"  "<<c<<std::endl;
    }

    if (CanRDivVM(a,b,c)) {
        v2 = v1%m;
        eps *= Norm(v2);
        c = a%b;
        if (XXDEBUG4) {
            std::cout<<"v%m = "<<v2<<std::endl;
            std::cout<<"a%b = "<<c<<std::endl;
        }
        Assert(Norm(VEC(T,c)-v2) <= eps,label+" c=a%b");
#if (XTEST & 16)
        c = -a%b;
        Assert(Norm(VEC(T,c)-(-v2)) <= eps,label+" c=-a%b");
        RealType(T) x(5);
        T z; SetZ(z);
        c = x*a%b;
        Assert(Norm(VEC(T,c)-(x*v2)) <= x*eps,label+" c=x*a%b");
        c = a%b*x;
        Assert(Norm(VEC(T,c)-(x*v2)) <= x*eps,label+" c=x*a%b");
        c = (x*a)%b;
        Assert(Norm(VEC(T,c)-(x*v2)) <= x*eps,label+" c=(x*a)%b");
        c = z*a%b;
        Assert(Norm(VEC(T,c)-(z*v2)) <= x*eps,label+" c=z*a%b");
        c = a%b*z;
        Assert(Norm(VEC(T,c)-(z*v2)) <= x*eps,label+" c=a%b*z");
        c = (z*a)%b;
        Assert(Norm(VEC(T,c)-(z*v2)) <= x*eps,label+" c=(z*a)%b");
#endif
    }

    if (showstartdone) std::cout<<"Done RDiv VM3a"<<std::endl;
}

template <class Ta, class Tb, class T, class V1, class MM, class V2>
static void DoTestLDivVM3RR(
    tmv::DivType dt, const V1& a, const MM& b, V2& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start LDiv VM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestLDivVM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
    typename V2::reverse_type cr = c.reverse();
    DoTestLDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,c,label+" RevA");
    DoTestLDivVM3a<Ta,Tb,T>(dt,a,b,cr,label+" RevC");
    DoTestLDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,cr,label+" RevAC");
#endif

    if (showstartdone) std::cout<<"Done LDiv VM3"<<std::endl;
}

template <class Ta, class Tb, class T, class V1, class MM, class V2>
static void DoTestLDivVM3RC(
    tmv::DivType dt, const V1& a, const MM& b, V2& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start LDiv VM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestLDivVM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
    typename V2::reverse_type cr = c.reverse();
    DoTestLDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,c,label+" RevA");
    DoTestLDivVM3a<Ta,Tb,T>(dt,a,b,cr,label+" RevC");
    DoTestLDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,cr,label+" RevAC");

#if (XTEST & 2)
    typename V2::conjugate_type cc = c.conjugate();
    DoTestLDivVM3a<Ta,Tb,T>(dt,a,b,cc,label+" ConjC");
#endif
#endif

    if (showstartdone) std::cout<<"Done LDiv VM3"<<std::endl;
}

template <class Ta, class Tb, class T, class V1, class MM, class V2>
static void DoTestLDivVM3CR(
    tmv::DivType dt, const V1& a, const MM& b, V2& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start LDiv VM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestLDivVM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
    typename V2::reverse_type cr = c.reverse();
    DoTestLDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,c,label+" RevA");
    DoTestLDivVM3a<Ta,Tb,T>(dt,a,b,cr,label+" RevC");
    DoTestLDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,cr,label+" RevAC");

#if (XTEST & 2)
    DoTestLDivVM3a<Ta,Tb,T>(dt,Conjugate(a),b,c,label+" ConjA");
#endif
#endif

    if (showstartdone) std::cout<<"Done LDiv VM3"<<std::endl;
}

template <class Ta, class Tb, class T, class V1, class MM, class V2>
static void DoTestLDivVM3CC(
    tmv::DivType dt, const V1& a, const MM& b, V2& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start LDiv VM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestLDivVM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
    typename V2::reverse_type cr = c.reverse();
    DoTestLDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,c,label+" RevA");
    DoTestLDivVM3a<Ta,Tb,T>(dt,a,b,cr,label+" RevC");
    DoTestLDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,cr,label+" RevAC");

#if (XTEST & 2)
    typename V2::conjugate_type cc = c.conjugate();
    DoTestLDivVM3a<Ta,Tb,T>(dt,Conjugate(a),b,c,label+" ConjA");
    DoTestLDivVM3a<Ta,Tb,T>(dt,a,b,cc,label+" ConjC");
    DoTestLDivVM3a<Ta,Tb,T>(dt,Conjugate(a),b,cc,label+" ConjAC");
#endif
#endif

    if (showstartdone) std::cout<<"Done LDiv VM3"<<std::endl;
}

template <class Ta, class Tb, class T, class V1, class MM, class V2>
static void DoTestRDivVM3RR(
    tmv::DivType dt, const V1& a, const MM& b, V2& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start RDiv VM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestRDivVM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
    typename V2::reverse_type cr = c.reverse();
    DoTestRDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,c,label+" RevA");
    DoTestRDivVM3a<Ta,Tb,T>(dt,a,b,cr,label+" RevC");
    DoTestRDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,cr,label+" RevAC");
#endif

    if (showstartdone) std::cout<<"Done RDiv VM3"<<std::endl;
}

template <class Ta, class Tb, class T, class V1, class MM, class V2>
static void DoTestRDivVM3RC(
    tmv::DivType dt, const V1& a, const MM& b, V2& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start RDiv VM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestRDivVM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
    typename V2::reverse_type cr = c.reverse();
    DoTestRDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,c,label+" RevA");
    DoTestRDivVM3a<Ta,Tb,T>(dt,a,b,cr,label+" RevC");
    DoTestRDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,cr,label+" RevAC");

#if (XTEST & 2)
    typename V2::conjugate_type cc = c.conjugate();
    DoTestRDivVM3a<Ta,Tb,T>(dt,a,b,cc,label+" ConjC");
#endif
#endif

    if (showstartdone) std::cout<<"Done RDiv VM3"<<std::endl;
}

template <class Ta, class Tb, class T, class V1, class MM, class V2>
static void DoTestRDivVM3CR(
    tmv::DivType dt, const V1& a, const MM& b, V2& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start RDiv VM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestRDivVM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
    typename V2::reverse_type cr = c.reverse();
    DoTestRDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,c,label+" RevA");
    DoTestRDivVM3a<Ta,Tb,T>(dt,a,b,cr,label+" RevC");
    DoTestRDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,cr,label+" RevAC");

#if (XTEST & 2)
    DoTestRDivVM3a<Ta,Tb,T>(dt,Conjugate(a),b,c,label+" ConjA");
#endif
#endif

    if (showstartdone) std::cout<<"Done RDiv VM3"<<std::endl;
}

template <class Ta, class Tb, class T, class V1, class MM, class V2>
static void DoTestRDivVM3CC(
    tmv::DivType dt, const V1& a, const MM& b, V2& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start RDiv VM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestRDivVM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
    typename V2::reverse_type cr = c.reverse();
    DoTestRDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,c,label+" RevA");
    DoTestRDivVM3a<Ta,Tb,T>(dt,a,b,cr,label+" RevC");
    DoTestRDivVM3a<Ta,Tb,T>(dt,a.reverse(),b,cr,label+" RevAC");

#if (XTEST & 2)
    typename V2::conjugate_type cc = c.conjugate();
    DoTestRDivVM3a<Ta,Tb,T>(dt,Conjugate(a),b,c,label+" ConjA");
    DoTestRDivVM3a<Ta,Tb,T>(dt,a,b,cc,label+" ConjC");
    DoTestRDivVM3a<Ta,Tb,T>(dt,Conjugate(a),b,cc,label+" ConjAC");
#endif
#endif

    if (showstartdone) std::cout<<"Done RDiv VM3"<<std::endl;
}

template <class T, class Tb, class M1, class M2> 
static void DoTestLDivMM1a(
    tmv::DivType dt, const M1& a, const M2& b, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start LDiv MM1b: "<<label<<std::endl;
    }

    tmv::Matrix<T> m1 = a;
    tmv::Matrix<Tb> m2 = b;
    m2.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m2.saveDiv();

    double eps = EPS*Norm(m2)*Norm(tmv::Matrix<Tb>(m2.inverse()))*m1.colsize();

    if (CanLDiv(a,b)) {
        tmv::Matrix<ProductType(T,Tb)> frac = m1/m2;
        eps *= Norm(frac);
        if (XXDEBUG5) {
            std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
            std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
            std::cout<<"v = "<<tmv::TMV_Text(m1)<<"  "<<m1<<std::endl;
            std::cout<<"m = "<<tmv::TMV_Text(m2)<<"  "<<m2<<std::endl;
            std::cout<<"a/b = "<<a/b<<std::endl;
            std::cout<<"m1/m2 = "<<frac<<std::endl;
            std::cout<<"Norm(diff) = "<<Norm(MAT2(T,Tb,a/b)-frac)<<std::endl;
            std::cout<<"eps = "<<eps<<std::endl;
        }
        Assert(Norm(MAT2(T,Tb,a/b)-frac) <= eps,label+" a/b");
        RealType(T) x(5);
        ComplexType(T) z(3,4);
        Assert(Norm(MAT2(T,Tb,x*a/b)-x*frac) <= x*eps,label+" x*a/b");
        if (XXDEBUG5) {
            std::cout<<"z*a/b = "<<VEC(ComplexType(T),z*a/b)<<std::endl;
            std::cout<<"z*frac = "<<z*frac<<std::endl;
            std::cout<<"Norm(diff) = "<<Norm(VEC(ComplexType(T),z*a/b)-z*frac)
                <<" x*eps = "<<x*eps<<std::endl;
        }
        Assert(Norm(MAT(ComplexType(T),z*a/b)-z*frac) <= x*eps,label+" z*a/b");
#if (XTEST & 16)
        Assert(Norm(MAT2(T,Tb,b.inverse()*a)-frac) <= eps,label+" b^-1*a");
#ifndef NOMIX
        Assert(Norm(MAT2(T,Tb,a/m2)-frac) <= eps,label+" a/m2");
        Assert(Norm(MAT2(T,Tb,m2.inverse()*a)-frac) <= eps,label+" m2^-1*a");
        Assert(Norm(MAT2(T,Tb,m1/b)-frac) <= eps,label+" m1/b");
        Assert(Norm(MAT2(T,Tb,b.inverse()*m1)-frac) <= eps,label+" b^-1*m1");
#endif

        Assert(Norm(MAT2(T,Tb,(x*a)/b)-x*frac) <= x*eps,label+" (x*a)/b");
        Assert(Norm(MAT2(T,Tb,x*(a/b))-x*frac) <= x*eps,label+" x*(a/b)");
        Assert(Norm(MAT2(T,Tb,a/(x*b))-frac/x) <= eps/x,label+" a/(x*b)");

        Assert(Norm(MAT(ComplexType(T),(z*a)/b)-z*frac) <= x*eps,label+" (z*a)/b");
        Assert(Norm(MAT(ComplexType(T),z*(a/b))-z*frac) <= x*eps,label+" z*(a/b)");
        Assert(Norm(MAT(ComplexType(T),a/(z*b))-frac/z) <= eps/x,label+" a/(z*b)");

        Assert(Norm(MAT2(T,Tb,(x*a)/(x*b))-frac) <= eps,label+" (x*a)/(x*b)");
        Assert(Norm(MAT(ComplexType(T),(z*a)/(x*b))-(z/x)*frac) <= eps,
               label+" (z*a)/(x*b)");
        Assert(Norm(MAT(ComplexType(T),(x*a)/(z*b))-(x/z)*frac) <= eps,
               label+" (x*a)/(z*b)");
        Assert(Norm(MAT(ComplexType(T),(z*a)/(z*b))-frac) <= eps,
               label+" (z*a)/(z*b)");
#endif
    }

    if (showstartdone) std::cout<<"Done LDiv MM1b"<<std::endl;
}

template <class T, class Tb, class M1, class M2> 
static void DoTestRDivMM1a(
    tmv::DivType dt, const M1& a, const M2& b, std::string label)
{
    if (showstartdone) std::cout<<"Start RDiv MM1b: "<<label<<std::endl;

    tmv::Matrix<T> m1 = a;
    tmv::Matrix<Tb> m2 = b;
    m2.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m2.saveDiv();

    double eps = EPS*Norm(m2)*Norm(tmv::Matrix<Tb>(m2.inverse()))*m1.colsize();

    if (CanRDiv(a,b)) {
        tmv::Matrix<ProductType(T,Tb)> frac = m1%m2;
        eps *= Norm(frac);
        if (XXDEBUG6) {
            std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
            std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
            std::cout<<"m1 = "<<tmv::TMV_Text(m1)<<"  "<<m1<<std::endl;
            std::cout<<"m2 = "<<tmv::TMV_Text(m2)<<"  "<<m2<<std::endl;
            std::cout<<"a%b = "<<a%b<<std::endl;
            std::cout<<"m1%m2 = "<<frac<<std::endl;
            std::cout<<"Norm(diff) = "<<Norm(MAT2(T,Tb,a%b)-frac)<<std::endl;
            std::cout<<"eps = "<<eps<<std::endl;
        }
        Assert(Norm(MAT2(T,Tb,a%b)-frac) <= eps,label+" a%b");
        RealType(T) x(5);
        ComplexType(T) z(3,4);
        Assert(Norm(MAT2(T,Tb,x*a%b)-x*frac) <= x*eps,label+" x*a%b");
        Assert(Norm(MAT(ComplexType(T),z*a%b)-z*frac) <= x*eps,label+" z*a%b");
#if (XTEST & 16)
        Assert(Norm(MAT2(T,Tb,a*b.inverse())-frac) <= eps,label+" a*b^-1");
#ifndef NOMIX
        Assert(Norm(MAT2(T,Tb,a%m2)-frac) <= eps,label+" a%m2");
        Assert(Norm(MAT2(T,Tb,a*m2.inverse())-frac) <= eps,label+" a*m2^-1");
        Assert(Norm(MAT2(T,Tb,m1%b)-frac) <= eps,label+" m1%b");
        Assert(Norm(MAT2(T,Tb,m1*b.inverse())-frac) <= eps,label+" m1*b^-1");
#endif

        Assert(Norm(MAT2(T,Tb,(x*a)%b)-x*frac) <= x*eps,label+" (x*a)%b");
        Assert(Norm(MAT2(T,Tb,x*(a%b))-x*frac) <= x*eps,label+" x*(a%b)");
        Assert(Norm(MAT2(T,Tb,a%(x*b))-frac/x) <= eps/x,label+" a%(x*b)");

        Assert(Norm(MAT(ComplexType(T),(z*a)%b)-z*frac) <= x*eps,label+" (z*a)%b");
        Assert(Norm(MAT(ComplexType(T),z*(a%b))-z*frac) <= x*eps,label+" z*(a%b)");
        Assert(Norm(MAT(ComplexType(T),a%(z*b))-frac/z) <= eps/x,label+" a%(z*b)");

        Assert(Norm(MAT2(T,Tb,(x*a)%(x*b))-frac) <= eps,label+" (x*a)%(x*b)");
        Assert(Norm(MAT(ComplexType(T),(z*a)%(x*b))-(z/x)*frac) <= eps,
               label+" (z*a)%(x*b)");
        Assert(Norm(MAT(ComplexType(T),(x*a)%(z*b))-(x/z)*frac) <= eps,
               label+" (x*a)%(z*b)");
        Assert(Norm(MAT(ComplexType(T),(z*a)%(z*b))-frac) <= eps,
               label+" (z*a)%(z*b)");
#endif
    }

    if (showstartdone) std::cout<<"Done RDiv MM1b"<<std::endl;
}

template <class T, class Tb, class M1, class M2> 
static void DoTestLDivMM1R(
    tmv::DivType dt, const M1& a, const M2& b, std::string label)
{
    DoTestLDivMM1a<T,Tb>(dt,a,b,label);
}

template <class T, class Tb, class M1, class M2> 
static void DoTestLDivMM1C(
    tmv::DivType dt, const M1& a, const M2& b, std::string label)
{
    DoTestLDivMM1a<T,Tb>(dt,a,b,label);

#ifndef NOVIEWS
    DoTestLDivMM1a<T,Tb>(dt,a.conjugate(),b,label+" ConjA");
#endif
}

template <class T, class Tb, class M1, class M2> 
static void DoTestRDivMM1R(
    tmv::DivType dt, const M1& a, const M2& b, std::string label)
{
    DoTestRDivMM1a<T,Tb>(dt,a,b,label);
}

template <class T, class Tb, class M1, class M2> 
static void DoTestRDivMM1C(
    tmv::DivType dt, const M1& a, const M2& b, std::string label)
{
    DoTestRDivMM1a<T,Tb>(dt,a,b,label);

#ifndef NOVIEWS
    DoTestRDivMM1a<T,Tb>(dt,a.conjugate(),b,label+" ConjA");
#endif
}

#ifndef NOLDIVEQ
template <class T, class Tb, class M0, class M1, class M2> 
static void DoTestLDivMM2a(
    tmv::DivType dt, M0& a0, M1& a, const M2& b, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start LDiv MM2b: "<<label<<std::endl;
        std::cout<<"a0 = "<<a0<<std::endl;
        std::cout<<"a = "<<a<<std::endl;
        std::cout<<"b = "<<b<<std::endl;
    }

    tmv::Matrix<T> v = a;
    tmv::Matrix<Tb> m = b;
    m.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m.saveDiv();

    double eps = EPS*Norm(m)*Norm(tmv::Matrix<Tb>(m.inverse()))*v.colsize();

    if (CanLDivEq(a,b)) {
        a0 = a;
        tmv::Matrix<T> frac = v/m;
        eps *= Norm(frac);
        a /= b;
        Assert(Norm(MAT(T,a)-frac) <= eps,label+" a/=b");
        a = a0;
#ifdef ALIASOK
        a = a / b;
        Assert(Norm(MAT(T,a)-frac) <= eps,label+" a=a/b");
        a = a0;
        a = b.inverse() * a;
        Assert(Norm(MAT(T,a)-frac) <= eps,label+" a=b^-1*a");
        a = a0;
#if (XTEST & 16)
        RealType(T) x(5);
        T z;  SetZ(z);
        a = x * a / b;
        Assert(Norm(MAT(T,a)-x*frac) <= x*eps,label+" a=x*a/b");
        a = a0;
        a = x * b.inverse() * a;
        Assert(Norm(MAT(T,a)-x*frac) <= x*eps,label+" a=x*b^-1*a");
        a = a0;
        a = z * a / b;
        Assert(Norm(MAT(T,a)-z*frac) <= x*eps,label+" a=z*a/b");
        a = a0;
        a = z * b.inverse() * a;
        Assert(Norm(MAT(T,a)-z*frac) <= x*eps,label+" a=z*b^-1*a");
        a = a0;
#endif
#endif
    }
    if (showstartdone) std::cout<<"Done LDiv MM2a"<<std::endl;
}

template <class T, class Tb, class M0, class M1, class M2> 
static void DoTestLDivMM2R(
    tmv::DivType dt, M0& a0, M1& a, const M2& b, std::string label)
{
    DoTestLDivMM2a<T,Tb>(dt,a0,a,b,label);
}

template <class T, class Tb, class M0, class M1, class M2> 
static void DoTestLDivMM2C(
    tmv::DivType dt, M0& a0, M1& a, const M2& b, std::string label)
{
    DoTestLDivMM2a<T,Tb>(dt,a0,a,b,label);
#ifndef NOVIEWS
    typename M1::conjugate_type ac = a.conjugate();
    DoTestLDivMM2a<T,Tb>(dt,a0,ac,b,label+" ConjA");
#endif
}
#endif

#ifndef NORDIVEQ
template <class T, class Tb, class M0, class M1, class M2> 
static void DoTestRDivMM2a(
    tmv::DivType dt, M0& a0, M1& a, const M2& b, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start RDiv MM2a: "<<label<<std::endl;
        std::cout<<"a0 = "<<a0<<std::endl;
        std::cout<<"a = "<<a<<std::endl;
        std::cout<<"b = "<<b<<std::endl;
    }

    tmv::Matrix<T> v = a;
    tmv::Matrix<Tb> m = b;
    m.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m.saveDiv();

    double eps = EPS*Norm(m)*Norm(tmv::Matrix<Tb>(m.inverse()));

    if (CanRDivEq(a,b)) {
        a0 = a;
        tmv::Matrix<T> frac = v%m;
        eps *= Norm(frac);
        a %= b;
        Assert(Norm(MAT(T,a)-frac) <= eps,label+" a%=b");
        a = a0;
        a *= b.inverse();
        Assert(Norm(MAT(T,a)-frac) <= eps,label+" a*=b^-1");
        a = a0;
#ifdef ALIASOK
        a = a % b;
        Assert(Norm(MAT(T,a)-frac) <= eps,label+" a=a%b");
        a = a0;
        a = a * b.inverse();
        Assert(Norm(MAT(T,a)-frac) <= eps,label+" a=a*b^-1");
        a = a0;
#if (XTEST & 16)
        RealType(T) x(5);
        T z;  SetZ(z);
        a = x * a % b;
        Assert(Norm(MAT(T,a)-x*frac) <= x*eps,label+" a=x*a%b");
        a = a0;
        a = x * a * b.inverse();
        Assert(Norm(MAT(T,a)-x*frac) <= x*eps,label+" a=x*a*b^-1");
        a = a0;
        a = z * a % b;
        Assert(Norm(MAT(T,a)-z*frac) <= x*eps,label+" a=z*a%b");
        a = a0;
        a = z * a * b.inverse();
        Assert(Norm(MAT(T,a)-z*frac) <= x*eps,label+" a=z*a*b^-1");
        a = a0;
#endif
#endif
    }
    if (showstartdone) std::cout<<"Done RDiv MM2a"<<std::endl;
}

template <class T, class Tb, class M0, class M1, class M2> 
static void DoTestRDivMM2R(
    tmv::DivType dt, M0& a0, M1& a, const M2& b, std::string label)
{
    DoTestRDivMM2a<T,Tb>(dt,a0,a,b,label);
}

template <class T, class Tb, class M0, class M1, class M2> 
static void DoTestRDivMM2C(
    tmv::DivType dt, M0& a0, M1& a, const M2& b, std::string label)
{
    DoTestRDivMM2a<T,Tb>(dt,a0,a,b,label);
#ifndef NOVIEWS
    typename M1::conjugate_type ac = a.conjugate();
    DoTestRDivMM2a<T,Tb>(dt,a0,ac,b,label+" ConjA");
#endif
}
#endif

template <class Ta, class Tb, class T, class M1, class M2, class M3>
static void DoTestLDivMM3a(
    tmv::DivType dt, const M1& a, const M2& b, M3& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start LDiv MM3a"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    tmv::Matrix<Ta> v1 = a;
    tmv::Matrix<Tb> m = b;
    tmv::Matrix<T> v2 = c;
    m.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m.saveDiv();

    double eps = EPS*Norm(m)*Norm(tmv::Matrix<Tb>(m.inverse()))*v1.colsize();

    if (XXDEBUG7) {
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<"  "<<c<<std::endl;
    }

    if (CanLDiv(a,b,c)) {
        v2 = v1/m;
        eps *= Norm(v2);
        c = a/b;
        if (XXDEBUG7) {
            std::cout<<"v/m = "<<v2<<std::endl;
            std::cout<<"a/b = "<<c<<std::endl;
        }
        Assert(Norm(MAT(T,c)-v2) <= eps,label+" c=a/b");
#if (XTEST & 16)
        c = -a/b;
        Assert(Norm(MAT(T,c)-(-v2)) <= eps,label+" c=-a/b");
        RealType(T) x(5);
        T z; SetZ(z);
        c = x*a/b;
        Assert(Norm(MAT(T,c)-(x*v2)) <= x*eps,label+" c=x*a/b");
        c = a/b*x;
        Assert(Norm(MAT(T,c)-(x*v2)) <= x*eps,label+" c=a/b*x");
        c = (x*a)/b;
        Assert(Norm(MAT(T,c)-(x*v2)) <= x*eps,label+" c=(x*a)/b");
        c = z*a/b;
        Assert(Norm(MAT(T,c)-(z*v2)) <= x*eps,label+" c=z*a/b");
        c = a/b*z;
        Assert(Norm(MAT(T,c)-(z*v2)) <= x*eps,label+" c=a/b*z");
        c = (z*a)/b;
        Assert(Norm(MAT(T,c)-(z*v2)) <= x*eps,label+" c=(z*a)/b");
#endif
    }

    if (showstartdone) std::cout<<"Done LDiv MM3a"<<std::endl;
}

template <class Ta, class Tb, class T, class M1, class M2, class M3>
static void DoTestRDivMM3a(
    tmv::DivType dt, const M1& a, const M2& b, M3& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start RDiv MM3a"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    tmv::Matrix<Ta> v1 = a;
    tmv::Matrix<Tb> m = b;
    tmv::Matrix<T> v2 = c;
    m.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m.saveDiv();

    double eps = EPS*Norm(m)*Norm(tmv::Matrix<Tb>(m.inverse()));

    if (XXDEBUG8) {
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<"  "<<c<<std::endl;
    }

    if (CanRDiv(a,b,c)) {
        v2 = v1%m;
        eps *= Norm(v2);
        c = a%b;
        if (XXDEBUG8) {
            std::cout<<"v%m = "<<v2<<std::endl;
            std::cout<<"a%b = "<<c<<std::endl;
            std::cout<<"c-v2 = "<<c-v2<<std::endl;
            std::cout<<"Norm(c-v2) = "<<Norm(MAT(T,c)-v2)<<std::endl;
            std::cout<<"eps = "<<EPS<<" * "<<Norm(m)<<" * "<<
                Norm(tmv::Matrix<Tb>(m.inverse()))<<" * "<<
                v1.colsize()<<" = "<<eps<<std::endl;
        }
        Assert(Norm(MAT(T,c)-v2) <= eps,label+" c=a%b");
#if (XTEST & 16)
        c = -a%b;
        Assert(Norm(MAT(T,c)-(-v2)) <= eps,label+" c=-a%b");
        RealType(T) x(5);
        T z; SetZ(z);
        c = x*a%b;
        Assert(Norm(MAT(T,c)-(x*v2)) <= x*eps,label+" c=x*a%b");
        c = a%b*x;
        Assert(Norm(MAT(T,c)-(x*v2)) <= x*eps,label+" c=x*a%b");
        c = (x*a)%b;
        Assert(Norm(MAT(T,c)-(x*v2)) <= x*eps,label+" c=(x*a)%b");
        c = z*a%b;
        Assert(Norm(MAT(T,c)-(z*v2)) <= x*eps,label+" c=z*a%b");
        c = a%b*z;
        Assert(Norm(MAT(T,c)-(z*v2)) <= x*eps,label+" c=a%b*z");
        c = (z*a)%b;
        Assert(Norm(MAT(T,c)-(z*v2)) <= x*eps,label+" c=(z*a)%b");
#endif
    }

    if (showstartdone) std::cout<<"Done RDiv MM3a"<<std::endl;
}

template <class Ta, class Tb, class T, class M1, class M2, class M3>
static void DoTestLDivMM3RR(
    tmv::DivType dt, const M1& a, const M2& b, M3& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start LDiv MM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestLDivMM3a<Ta,Tb,T>(dt,a,b,c,label);

    if (showstartdone) std::cout<<"Done LDiv MM3"<<std::endl;
}

template <class Ta, class Tb, class T, class M1, class M2, class M3>
static void DoTestLDivMM3RC(
    tmv::DivType dt, const M1& a, const M2& b, M3& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start LDiv MM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestLDivMM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
#if (XTEST & 2)
    typename M3::conjugate_type cc = c.conjugate();
    DoTestLDivMM3a<Ta,Tb,T>(dt,a,b,cc,label+" ConjC");
#endif
#endif

    if (showstartdone) std::cout<<"Done LDiv MM3"<<std::endl;
}

template <class Ta, class Tb, class T, class M1, class M2, class M3>
static void DoTestLDivMM3CR(
    tmv::DivType dt, const M1& a, const M2& b, M3& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start LDiv MM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestLDivMM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
#if (XTEST & 2)
    DoTestLDivMM3a<Ta,Tb,T>(dt,Conjugate(a),b,c,label+" ConjA");
#endif
#endif

    if (showstartdone) std::cout<<"Done LDiv MM3"<<std::endl;
}

template <class Ta, class Tb, class T, class M1, class M2, class M3>
static void DoTestLDivMM3CC(
    tmv::DivType dt, const M1& a, const M2& b, M3& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start LDiv MM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestLDivMM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
#if (XTEST & 2)
    typename M3::conjugate_type cc = c.conjugate();
    DoTestLDivMM3a<Ta,Tb,T>(dt,Conjugate(a),b,c,label+" ConjA");
    DoTestLDivMM3a<Ta,Tb,T>(dt,Conjugate(a),b,cc,label+" ConjA ConjC");
    DoTestLDivMM3a<Ta,Tb,T>(dt,a,b,cc,label+" ConjC");
#endif
#endif

    if (showstartdone) std::cout<<"Done LDiv MM3"<<std::endl;
}

template <class Ta, class Tb, class T, class M1, class M2, class M3>
static void DoTestRDivMM3RR(
    tmv::DivType dt, const M1& a, const M2& b, M3& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start RDiv MM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestRDivMM3a<Ta,Tb,T>(dt,a,b,c,label);

    if (showstartdone) std::cout<<"Done RDiv MM3"<<std::endl;
}

template <class Ta, class Tb, class T, class M1, class M2, class M3>
static void DoTestRDivMM3RC(
    tmv::DivType dt, const M1& a, const M2& b, M3& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start RDiv MM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestRDivMM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
#if (XTEST & 2)
    typename M3::conjugate_type cc = c.conjugate();
    DoTestRDivMM3a<Ta,Tb,T>(dt,a,b,cc,label+" ConjC");
#endif
#endif

    if (showstartdone) std::cout<<"Done RDiv MM3"<<std::endl;
}

template <class Ta, class Tb, class T, class M1, class M2, class M3>
static void DoTestRDivMM3CR(
    tmv::DivType dt, const M1& a, const M2& b, M3& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start RDiv MM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestRDivMM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
#if (XTEST & 2)
    DoTestRDivMM3a<Ta,Tb,T>(dt,Conjugate(a),b,c,label+" ConjA");
#endif
#endif

    if (showstartdone) std::cout<<"Done RDiv MM3"<<std::endl;
}

template <class Ta, class Tb, class T, class M1, class M2, class M3>
static void DoTestRDivMM3CC(
    tmv::DivType dt, const M1& a, const M2& b, M3& c, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start RDiv MM3"<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<" "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<" "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<" "<<c<<std::endl;
    }

    DoTestRDivMM3a<Ta,Tb,T>(dt,a,b,c,label);
#ifndef NOVIEWS
#if (XTEST & 2)
    typename M3::conjugate_type cc = c.conjugate();
    DoTestRDivMM3a<Ta,Tb,T>(dt,Conjugate(a),b,c,label+" ConjA");
    DoTestRDivMM3a<Ta,Tb,T>(dt,Conjugate(a),b,cc,label+" ConjA ConjC");
    DoTestRDivMM3a<Ta,Tb,T>(dt,a,b,cc,label+" ConjC");
#endif
#endif

    if (showstartdone) std::cout<<"Done RDiv MM3"<<std::endl;
}

template <class T, class MM> 
static void DoTestDivMX(
    tmv::DivType dt, const MM& a, std::string label)
{
    if (showstartdone) std::cout<<"Start Div MX: "<<label<<std::endl;

    tmv::Matrix<T> m = a;
    m.divideUsing(dt==tmv::CH?tmv::LU:dt);
    m.saveDiv();

    double eps = EPS*Norm(m)*Norm(m.inverse())*std::max(m.colsize(),m.rowsize());

    RealType(T) x(5);
    ComplexType(T) z(3,4);
    tmv::Matrix<T> xfrac = x/m;
    tmv::Matrix<ComplexType(T)> zfrac = z/m;
    double normfrac = Norm(xfrac);
    double normm = Norm(m);

    if (XXDEBUG9) {
        std::cout<<"eps = "<<eps<<std::endl;
        std::cout<<"x = "<<x<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
        std::cout<<"m = "<<tmv::TMV_Text(m)<<"  "<<m<<std::endl;
        std::cout<<"x/a = "<<x/a<<std::endl;
        std::cout<<"x/m = "<<xfrac<<std::endl;
        std::cout<<"a*(x/a) = "<<tmv::Matrix<T>(a*(x/a))<<std::endl;
        std::cout<<"m*(x/m) = "<<m*xfrac<<std::endl;
        std::cout<<"Norm(diff) = "<<Norm(MAT(T,x/a)-xfrac)<<std::endl;
        std::cout<<"eps*Norm(diff) = "<<eps*normfrac<<std::endl;
    }
    Assert(Norm(MAT(T,x/a)-xfrac) <= x*eps*normfrac,label+" x/a");
    Assert(Norm(MAT(ComplexType(T),z/a)-zfrac) <= x*eps*normfrac,label+" z/a");
#if (XTEST & 16)
    Assert(Norm(MAT(T,a.inverse()*x)-xfrac) <= x*eps*normfrac,
           label+" a^-1*x");
    Assert(Norm(MAT(ComplexType(T),a.inverse()*z)-zfrac) <= x*eps*normfrac,
           label+" a^-1*z");
#endif

    if (XXDEBUG9) {
        std::cout<<"x%a = "<<x%a<<std::endl;
        std::cout<<"x%m = "<<xfrac<<std::endl;
        std::cout<<"x%a*a = "<<tmv::Matrix<T>((x%a)*a)<<std::endl;
        std::cout<<"x%m*m = "<<xfrac*m<<std::endl;
        std::cout<<"Norm(diff) = "<<Norm(MAT(T,x%a)-xfrac)<<std::endl;
    }
    Assert(Norm(MAT(T,x%a)-xfrac) <= x*eps*normfrac,label+" x%a");
#if (XTEST & 16)
    Assert(Norm(MAT(T,x*a.inverse())-xfrac) <= x*eps*normfrac,
           label+" x*a^-1");
    Assert(Norm(MAT(ComplexType(T),z%a)-zfrac) <= x*eps*normfrac,label+" z%a");
    Assert(Norm(MAT(ComplexType(T),z*a.inverse())-zfrac) <= x*eps*normfrac,
           label+" z*a^-1");
#endif

    tmv::Matrix<T> temp = a.inverse();
    if (XXDEBUG9) {
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
        std::cout<<"1/a = "<<temp<<std::endl;
        std::cout<<"a*(1/a) = "<<tmv::Matrix<T>(a*temp)<<std::endl;
        std::cout<<"(1/a)*a = "<<tmv::Matrix<T>(temp*a)<<std::endl;
        std::cout<<"(1/a)*a-((1/a)*a)T = "<<
            tmv::Matrix<T>(temp*a)-Adjoint(tmv::Matrix<T>(temp*a))<<std::endl;
        std::cout<<"(a*(1/a))*a = "<<tmv::Matrix<T>(a*temp)*a<<std::endl;
        std::cout<<"(a*(1/a))*a-a = "<<tmv::Matrix<T>(a*temp)*a-a<<std::endl;
        std::cout<<Norm(tmv::Matrix<T>(a*temp)*a-a)<<"  "<<eps*normm<<std::endl;
        std::cout<<Norm(tmv::Matrix<T>(temp*a)-Adjoint(tmv::Matrix<T>(temp*a)))<<
            "  "<<eps<<std::endl;
    }
    Assert(Norm(tmv::Matrix<T>(a*temp)*a-MAT(T,a)) <= eps*normm,label+" a*(1/a)*a");
    Assert(Norm(tmv::Matrix<T>(temp*a)-Adjoint(tmv::Matrix<T>(temp*a))) <= eps,
           label+" (1/a)*a-((1/a)*a)T");

    if (showstartdone) std::cout<<"Done MX"<<std::endl;
}

template <class T, class SM0, class CSM0, class SM1, class SM2, class CSM1, class CSM2> 
static void TestMatrixDivArith1(
    tmv::DivType dt, SM0& b0, CSM0& cb0, const SM1& a, SM2& b,
    const CSM1& ca, CSM2& cb, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start Test Div 1: "<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
        std::cout<<"ca = "<<tmv::TMV_Text(ca)<<"  "<<ca<<std::endl;
        std::cout<<"cb = "<<tmv::TMV_Text(cb)<<"  "<<cb<<std::endl;
        std::cout<<"b0 = "<<tmv::TMV_Text(b0)<<"  "<<b0<<std::endl;
        std::cout<<"cb0 = "<<tmv::TMV_Text(cb0)<<"  "<<cb0<<std::endl;
    }

    tmv::Matrix<CT> cc(a.rowsize(),b.rowsize());
    tmv::Matrix<CT> cd(b.colsize(),a.colsize());
    tmv::Matrix<T> c(a.rowsize(),b.rowsize());
    tmv::Matrix<T> d(b.colsize(),a.colsize());
    DoTestLDivMM1R<T,T>(dt,b,a,label+" R,R");
    DoTestRDivMM1R<T,T>(dt,b,a,label+" R,R");
#if (XTEST & 4)
    DoTestLDivMM1R<T,CT>(dt,b,ca,label+" R,C");
    DoTestRDivMM1R<T,CT>(dt,b,ca,label+" R,C");
    DoTestLDivMM1C<CT,T>(dt,cb,a,label+" C,R");
    DoTestRDivMM1C<CT,T>(dt,cb,a,label+" C,R");
#endif
    DoTestLDivMM1C<CT,CT>(dt,cb,ca,label+" C,C");
    DoTestRDivMM1C<CT,CT>(dt,cb,ca,label+" C,C");

#ifndef NOLDIVEQ
    DoTestLDivMM2R<T,T>(dt,b0,b,a,label+" R,R");
#if (XTEST & 4)
    DoTestLDivMM2C<CT,T>(dt,cb0,cb,a,label+" C,R");
#endif
    DoTestLDivMM2C<CT,CT>(dt,cb0,cb,ca,label+" C,C");
#endif

#ifndef NORDIVEQ
    DoTestRDivMM2R<T,T>(dt,b0,b,a,label+" R,R");
#if (XTEST & 4)
    DoTestRDivMM2C<CT,T>(dt,cb0,cb,a,label+" C,R");
#endif
    DoTestRDivMM2C<CT,CT>(dt,cb0,cb,ca,label+" C,C");
#endif

    DoTestLDivMM3RR<T,T,T>(dt,b,a,c,label+" R,R,R");
    DoTestRDivMM3RR<T,T,T>(dt,b,a,d,label+" R,R,R");
#if (XTEST & 4)
    DoTestLDivMM3RC<T,T,CT>(dt,b,a,cc,label+" R,R,C");
    DoTestRDivMM3RC<T,T,CT>(dt,b,a,cd,label+" R,R,C");
    DoTestLDivMM3RC<T,CT,CT>(dt,b,ca,cc,label+" R,C,C");
    DoTestRDivMM3RC<T,CT,CT>(dt,b,ca,cd,label+" R,C,C");
    DoTestLDivMM3CC<CT,T,CT>(dt,cb,a,cc,label+" C,R,C");
    DoTestRDivMM3CC<CT,T,CT>(dt,cb,a,cd,label+" C,R,C");
#endif
    DoTestLDivMM3CC<CT,CT,CT>(dt,cb,ca,cc,label+" C,C,C");
    DoTestRDivMM3CC<CT,CT,CT>(dt,cb,ca,cd,label+" C,C,C");
}

template <class T, class SM0, class CSM0, class SM1, class SM2, class CSM1, class CSM2> 
static void TestMatrixDivArith2(
    tmv::DivType dt, SM0& b0, CSM0& cb0,
    const SM1& a, SM2& b, const CSM1& ca, CSM2& cb, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start Test Div 2: "<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
        std::cout<<"ca = "<<tmv::TMV_Text(ca)<<"  "<<ca<<std::endl;
        std::cout<<"cb = "<<tmv::TMV_Text(cb)<<"  "<<cb<<std::endl;
        std::cout<<"b0 = "<<tmv::TMV_Text(b0)<<"  "<<b0<<std::endl;
        std::cout<<"cb0 = "<<tmv::TMV_Text(cb0)<<"  "<<cb0<<std::endl;
    }

    DoTestDivMX<T>(dt,a,label+" R");
    DoTestDivMX<CT>(dt,ca,label+" C");

    tmv::Matrix<T> m2(b);
    tmv::Matrix<CT> cm2(cb);
    tmv::Vector<T> v(b.colsize());
    tmv::Vector<CT> cv(b.colsize());
    if (b.rowsize() > 0) {
        v = m2.col(0);
        cv = cm2.col(0);
    }
    else {
        v.setAllTo(1);
        cv.setAllTo(CT(1,2));
    }

    tmv::Vector<T> w(b.rowsize());
    tmv::Vector<CT> cw(b.rowsize());
    if (b.colsize() > 0) {
        w = m2.row(0);
        cw = cm2.row(0);
    }
    else {
        w.setAllTo(1);
        cw.setAllTo(CT(1,2));
    }
    tmv::Vector<T> x(a.rowsize());
    tmv::Vector<T> y(a.colsize());
    tmv::Vector<CT> cx(a.rowsize());
    tmv::Vector<CT> cy(a.colsize());
    tmv::VectorView<T> vv = v.view();
    tmv::VectorView<CT> cvv = cv.view();
    tmv::VectorView<T> wv = w.view();
    tmv::VectorView<CT> cwv = cw.view();
    tmv::VectorView<T> xv = x.view();
    tmv::VectorView<CT> cxv = cx.view();
    tmv::VectorView<T> yv = y.view();
    tmv::VectorView<CT> cyv = cy.view();

    DoTestLDivVM1R<T,T>(dt,vv,a,label+" R,R");
    DoTestRDivVM1R<T,T>(dt,wv,a,label+" R,R");
#if (XTEST & 4)
    DoTestLDivVM1R<T,CT>(dt,vv,ca,label+" R,C");
    DoTestRDivVM1R<T,CT>(dt,wv,ca,label+" R,C");
    DoTestLDivVM1C<CT,T>(dt,cvv,a,label+" C,R");
    DoTestRDivVM1C<CT,T>(dt,cwv,a,label+" C,R");
#endif
    DoTestLDivVM1C<CT,CT>(dt,cvv,ca,label+" C,C");
    DoTestRDivVM1C<CT,CT>(dt,cwv,ca,label+" C,C");

#ifndef NOLDIVEQ
    DoTestLDivVM2R<T,T>(dt,vv,a,label+" R,R");
#if (XTEST & 4)
    DoTestLDivVM2C<CT,T>(dt,cvv,a,label+" C,R");
#endif
    DoTestLDivVM2C<CT,CT>(dt,cvv,ca,label+" C,C");
#endif

#ifndef NORDIVEQ
    DoTestRDivVM2R<T,T>(dt,wv,a,label+" R,R");
#if (XTEST & 4)
    DoTestRDivVM2C<CT,T>(dt,cwv,a,label+" C,R");
#endif
    DoTestRDivVM2C<CT,CT>(dt,cwv,ca,label+" C,C");
#endif

    DoTestLDivVM3RR<T,T,T>(dt,vv,a,xv,label+" R,R,R");
    DoTestRDivVM3RR<T,T,T>(dt,wv,a,yv,label+" R,R,R");
#if (XTEST & 4)
    DoTestLDivVM3RC<T,T,CT>(dt,vv,a,cxv,label+" R,R,C");
    DoTestRDivVM3RC<T,T,CT>(dt,wv,a,cyv,label+" R,R,C");
    DoTestLDivVM3RC<T,CT,CT>(dt,vv,ca,cxv,label+" R,C,C");
    DoTestRDivVM3RC<T,CT,CT>(dt,wv,ca,cyv,label+" R,C,C");
    DoTestLDivVM3CC<CT,T,CT>(dt,cvv,a,cxv,label+" C,R,C");
    DoTestRDivVM3CC<CT,T,CT>(dt,cwv,a,cyv,label+" C,R,C");
#endif
    DoTestLDivVM3CC<CT,CT,CT>(dt,cvv,ca,cxv,label+" C,C,C");
    DoTestRDivVM3CC<CT,CT,CT>(dt,cwv,ca,cyv,label+" C,C,C");

    tmv::Matrix<CT> cc(a.rowsize(),b.rowsize());
    tmv::Matrix<CT> cd(b.colsize(),a.colsize());
    tmv::Matrix<T> c(a.rowsize(),b.rowsize());
    tmv::Matrix<T> d(b.colsize(),a.colsize());
    tmv::MatrixView<T> cV = c.view(); // cV, since cv is already taken.
    tmv::MatrixView<CT> ccv = cc.view();
    tmv::MatrixView<T> dv = d.view();
    tmv::MatrixView<CT> cdv = cd.view();

    DoTestLDivMM1R<T,T>(dt,b,a,label+" R,R");
    DoTestRDivMM1R<T,T>(dt,b,a,label+" R,R");
#if (XTEST & 4)
    DoTestLDivMM1R<T,CT>(dt,b,ca,label+" R,C");
    DoTestRDivMM1R<T,CT>(dt,b,ca,label+" R,C");
    DoTestLDivMM1C<CT,T>(dt,cb,a,label+" C,R");
    DoTestRDivMM1C<CT,T>(dt,cb,a,label+" C,R");
#endif
    DoTestLDivMM1C<CT,CT>(dt,cb,ca,label+" C,C");
    DoTestRDivMM1C<CT,CT>(dt,cb,ca,label+" C,C");

#ifndef NOLDIVEQ
    DoTestLDivMM2R<T,T>(dt,b0,b,a,label+" R,R");
#if (XTEST & 4)
    DoTestLDivMM2C<CT,T>(dt,cb0,cb,a,label+" C,R");
#endif
    DoTestLDivMM2C<CT,CT>(dt,cb0,cb,ca,label+" C,C");
#endif

#ifndef NORDIVEQ
    DoTestRDivMM2R<T,T>(dt,b0,b,a,label+" R,R");
#if (XTEST & 4)
    DoTestRDivMM2C<CT,T>(dt,cb0,cb,a,label+" C,R");
#endif
    DoTestRDivMM2C<CT,CT>(dt,cb0,cb,ca,label+" C,C");
#endif

    DoTestLDivMM3RR<T,T,T>(dt,b,a,cV,label+" R,R,R");
    DoTestRDivMM3RR<T,T,T>(dt,b,a,dv,label+" R,R,R");
#if (XTEST & 4)
    DoTestLDivMM3RC<T,T,CT>(dt,b,a,ccv,label+" R,R,C");
    DoTestRDivMM3RC<T,T,CT>(dt,b,a,cdv,label+" R,R,C");
    DoTestLDivMM3RC<T,CT,CT>(dt,b,ca,ccv,label+" R,C,C");
    DoTestRDivMM3RC<T,CT,CT>(dt,b,ca,cdv,label+" R,C,C");
    DoTestLDivMM3CC<CT,T,CT>(dt,cb,a,ccv,label+" C,R,C");
    DoTestRDivMM3CC<CT,T,CT>(dt,cb,a,cdv,label+" C,R,C");
#endif
    DoTestLDivMM3CC<CT,CT,CT>(dt,cb,ca,ccv,label+" C,C,C");
    DoTestRDivMM3CC<CT,CT,CT>(dt,cb,ca,cdv,label+" C,C,C");
}

template <class T, class SM1, class CSM1> 
static void TestMatrixDivArith3a(
    tmv::DivType dt, const SM1& a, const CSM1& ca, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start Test Div 3a: "<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
        std::cout<<"ca = "<<tmv::TMV_Text(ca)<<"  "<<ca<<std::endl;
    }

    DoTestDivMX<T>(dt,a,label+" R");
    DoTestDivMX<CT>(dt,ca,label+" C");
}

template <class T, class SM0, class CSM0, class SM1, class SM2, class SM3, class CSM1, class CSM2, class CSM3> 
static void TestMatrixDivArith3b(
    tmv::DivType dt, SM0& b0, CSM0& cb0, const SM1& a, SM2& b, SM3& c, 
    const CSM1& ca, CSM2& cb, CSM3& cc, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start Test Div 3b: "<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<"  "<<c<<std::endl;
        std::cout<<"ca = "<<tmv::TMV_Text(ca)<<"  "<<ca<<std::endl;
        std::cout<<"cb = "<<tmv::TMV_Text(cb)<<"  "<<cb<<std::endl;
        std::cout<<"cc = "<<tmv::TMV_Text(cc)<<"  "<<cc<<std::endl;
        std::cout<<"b0 = "<<tmv::TMV_Text(b0)<<"  "<<b0<<std::endl;
        std::cout<<"cb0 = "<<tmv::TMV_Text(cb0)<<"  "<<cb0<<std::endl;
    }

    DoTestLDivMM1R<T,T>(dt,b,a,label+" R,R");
#if (XTEST & 4)
    DoTestLDivMM1R<T,CT>(dt,b,ca,label+" R,C");
    DoTestLDivMM1C<CT,T>(dt,cb,a,label+" C,R");
#endif
    DoTestLDivMM1C<CT,CT>(dt,cb,ca,label+" C,C");

#ifndef NOLDIVEQ
    DoTestLDivMM2R<T,T>(dt,b0,b,a,label+" R,R");
#if (XTEST & 4)
    DoTestLDivMM2C<CT,T>(dt,cb0,cb,a,label+" C,R");
#endif
    DoTestLDivMM2C<CT,CT>(dt,cb0,cb,ca,label+" C,C");
#endif

    DoTestLDivMM3RR<T,T,T>(dt,b,a,c,label+" R,R,R");
#if (XTEST & 4)
    DoTestLDivMM3RC<T,T,CT>(dt,b,a,cc,label+" R,R,C");
    DoTestLDivMM3RC<T,CT,CT>(dt,b,ca,cc,label+" R,C,C");
    DoTestLDivMM3CC<CT,T,CT>(dt,cb,a,cc,label+" C,R,C");
#endif
    DoTestLDivMM3CC<CT,CT,CT>(dt,cb,ca,cc,label+" C,C,C");
}

template <class T, class SM0, class CSM0, class SM1, class SM2, class SM3, class CSM1, class CSM2, class CSM3> 
static void TestMatrixDivArith3c(
    tmv::DivType dt, SM0& b0, CSM0& cb0, const SM1& a, SM2& b, SM3& c, 
    const CSM1& ca, CSM2& cb, CSM3& cc, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start Test Div 3c: "<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
        std::cout<<"b = "<<tmv::TMV_Text(b)<<"  "<<b<<std::endl;
        std::cout<<"c = "<<tmv::TMV_Text(c)<<"  "<<c<<std::endl;
        std::cout<<"ca = "<<tmv::TMV_Text(ca)<<"  "<<ca<<std::endl;
        std::cout<<"cb = "<<tmv::TMV_Text(cb)<<"  "<<cb<<std::endl;
        std::cout<<"cc = "<<tmv::TMV_Text(cc)<<"  "<<cc<<std::endl;
        std::cout<<"b0 = "<<tmv::TMV_Text(b0)<<"  "<<b0<<std::endl;
        std::cout<<"cb0 = "<<tmv::TMV_Text(cb0)<<"  "<<cb0<<std::endl;
    }

    DoTestRDivMM1R<T,T>(dt,b,a,label+" R,R");
#if (XTEST & 4)
    DoTestRDivMM1R<T,CT>(dt,b,ca,label+" R,C");
    DoTestRDivMM1C<CT,T>(dt,cb,a,label+" C,R");
#endif
    DoTestRDivMM1C<CT,CT>(dt,cb,ca,label+" C,C");

#ifndef NORDIVEQ
    DoTestRDivMM2R<T,T>(dt,b0,b,a,label+" R,R");
#if (XTEST & 4)
    DoTestRDivMM2C<CT,T>(dt,cb0,cb,a,label+" C,R");
#endif
    DoTestRDivMM2C<CT,CT>(dt,cb0,cb,ca,label+" C,C");
#endif

    DoTestRDivMM3RR<T,T,T>(dt,b,a,c,label+" R,R,R");
#if (XTEST & 4)
    DoTestRDivMM3RC<T,T,CT>(dt,b,a,cc,label+" R,R,C");
    DoTestRDivMM3RC<T,CT,CT>(dt,b,ca,cc,label+" R,C,C");
    DoTestRDivMM3CC<CT,T,CT>(dt,cb,a,cc,label+" C,R,C");
#endif
    DoTestRDivMM3CC<CT,CT,CT>(dt,cb,ca,cc,label+" C,C,C");
}

template <class T, class SM1, class V1, class V2, class CSM1, class CV1, class CV2> 
static void TestMatrixDivArith3d(
    tmv::DivType dt, const SM1& a, const V1& v, V2& x,
    const CSM1& ca, const CV1& cv, CV2& cx, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start Test Div 3d: "<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
        std::cout<<"v = "<<tmv::TMV_Text(v)<<"  "<<v<<std::endl;
        std::cout<<"x = "<<tmv::TMV_Text(x)<<"  "<<x<<std::endl;
        std::cout<<"ca = "<<tmv::TMV_Text(ca)<<"  "<<ca<<std::endl;
        std::cout<<"cv = "<<tmv::TMV_Text(cv)<<"  "<<cv<<std::endl;
        std::cout<<"cx = "<<tmv::TMV_Text(cx)<<"  "<<cx<<std::endl;
    }

    DoTestLDivVM1R<T,T>(dt,v,a,label+" R,R");
#if (XTEST & 4)
    DoTestLDivVM1R<T,CT>(dt,v,ca,label+" R,C");
    DoTestLDivVM1C<CT,T>(dt,cv,a,label+" C,R");
#endif
    DoTestLDivVM1C<CT,CT>(dt,cv,ca,label+" C,C");

#ifndef NOLDIVEQ
    DoTestLDivVM2R<T,T>(dt,x,a,label+" R,R");
#if (XTEST & 4)
    DoTestLDivVM2C<CT,T>(dt,cx,a,label+" C,R");
#endif
    DoTestLDivVM2C<CT,CT>(dt,cx,ca,label+" C,C");
#endif

    DoTestLDivVM3RR<T,T,T>(dt,v,a,x,label+" R,R,R");
#if (XTEST & 4)
    DoTestLDivVM3RC<T,T,CT>(dt,v,a,cx,label+" R,R,C");
    DoTestLDivVM3RC<T,CT,CT>(dt,v,ca,cx,label+" R,C,C");
    DoTestLDivVM3CC<CT,T,CT>(dt,cv,a,cx,label+" C,R,C");
#endif
    DoTestLDivVM3CC<CT,CT,CT>(dt,cv,ca,cx,label+" C,C,C");
}

template <class T, class SM1, class V1, class V2, class CSM1, class CV1, class CV2> 
static void TestMatrixDivArith3e(
    tmv::DivType dt, const SM1& a, const V1& w, V2& y,
    const CSM1& ca, const CV1& cw, CV2& cy, std::string label)
{
    if (showstartdone) {
        std::cout<<"Start Test Div 3e: "<<label<<std::endl;
        std::cout<<"a = "<<tmv::TMV_Text(a)<<"  "<<a<<std::endl;
        std::cout<<"w = "<<tmv::TMV_Text(w)<<"  "<<w<<std::endl;
        std::cout<<"y = "<<tmv::TMV_Text(y)<<"  "<<y<<std::endl;
        std::cout<<"ca = "<<tmv::TMV_Text(ca)<<"  "<<ca<<std::endl;
        std::cout<<"cw = "<<tmv::TMV_Text(cw)<<"  "<<cw<<std::endl;
        std::cout<<"cy = "<<tmv::TMV_Text(cy)<<"  "<<cy<<std::endl;
    }

    DoTestRDivVM1R<T,T>(dt,w,a,label+" R,R");
#if (XTEST & 4)
    DoTestRDivVM1R<T,CT>(dt,w,ca,label+" R,C");
    DoTestRDivVM1C<CT,T>(dt,cw,a,label+" C,R");
#endif
    DoTestRDivVM1C<CT,CT>(dt,cw,ca,label+" C,C");

#ifndef NORDIVEQ
    DoTestRDivVM2R<T,T>(dt,y,a,label+" R,R");
#if (XTEST & 4)
    DoTestRDivVM2C<CT,T>(dt,cy,a,label+" C,R");
#endif
    DoTestRDivVM2C<CT,CT>(dt,cy,ca,label+" C,C");
#endif

    DoTestRDivVM3RR<T,T,T>(dt,w,a,y,label+" R,R,R");
#if (XTEST & 4)
    DoTestRDivVM3RC<T,T,CT>(dt,w,a,cy,label+" R,R,C");
    DoTestRDivVM3RC<T,CT,CT>(dt,w,ca,cy,label+" R,C,C");
    DoTestRDivVM3CC<CT,T,CT>(dt,cw,a,cy,label+" C,R,C");
#endif
    DoTestRDivVM3CC<CT,CT,CT>(dt,cw,ca,cy,label+" C,C,C");
}
