///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// The Template Matrix/Vector Library for C++ was created by Mike Jarvis     //
// Copyright (C) 2007                                                        //
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
// along with this program in the file gpl.txt.                              //
//                                                                           //
// If not, write to:                                                         //
// The Free Software Foundation, Inc.                                        //
// 51 Franklin Street, Fifth Floor,                                          //
// Boston, MA  02110-1301, USA.                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
//
// This file defines all the iterator classes, as well as the classes
// to deal with conjugate vectors/matrices.
//
// The vector iterators are similar to the standard library's 
// iterator and const_iterator types.  
//
// VIt and CVIt are iterators along a mutable vector (or view) 
// and constant vector respectively.
//

#ifndef TMV_It_H
#define TMV_It_H

#include "TMV_Base.h"

namespace tmv {

  // Start with real version so Conj isn't an issue.
  // Specialize for complex below.
  template <class T> class VIter
  { 
    public :

      VIter() : p(0), s(0) DEFFIRSTLAST(0,0) {}
      VIter(T* inp, int instep, ConjType DEBUGPARAM(inc) PARAMFIRSTLAST(T) ) : 
	p(inp), s(instep) DEFFIRSTLAST(_first,_last) 
      { TMVAssert(inc==NonConj); }
      VIter(const VIter<T>& rhs) : 
	p(rhs.p), s(rhs.s) DEFFIRSTLAST(rhs.first,rhs.last) {}
      VIter<T>& operator=(const VIter<T>& rhs) 
      { 
	TMVAssert(s == rhs.s);
	p=rhs.p; 
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }
      ~VIter() {}

      T* GetP() const { return p; }
      int step() const { return s; }
      ConjType GetC() const { return NonConj; }

      inline bool operator==(const VIter<T>& rhs) const 
      { return p == rhs.p; }
      inline bool operator!=(const VIter<T>& rhs) const 
      { return p != rhs.p; }
      inline bool operator<(const VIter<T>& rhs) const 
      { return (s > 0 ? p < rhs.p : p > rhs.p); }

      inline T& operator*() const
      { 
#ifdef TMVFLDEBUG
	if (!(p>=first && p<last)) {
	  std::cerr<<"p = "<<p<<std::endl;
	  std::cerr<<"first,last = "<<first<<"  "<<last<<std::endl;
	}
	TMVAssert(p>=first);
	TMVAssert(p<last);
#endif
	return *p; 
      }

      inline VIter<T>& operator++() { p += s; return *this; }
      inline VIter<T>& operator--() { p -= s; return *this; }
      inline VIter<T> operator++(int) 
      { VIter<T> p2 = *this; p+=s; return p2; }
      inline VIter<T> operator--(int) 
      { VIter<T> p2 = *this; p-=s; return p2; }

      inline VIter<T>& operator+=(size_t n) { p += n*s; return *this; }
      inline VIter<T>& operator-=(size_t n) { p -= n*s; return *this; }
      inline VIter<T> operator+(size_t n) const 
      { return VIter<T>(p+n*s,s,NonConj FIRSTLAST ); }
      inline VIter<T> operator-(size_t n) const 
      { return VIter<T>(p-n*s,s,NonConj FIRSTLAST ); }

      inline ptrdiff_t operator-(const VIter<T>& rhs) const 
      { return (p-rhs.p)/s; }

      inline T& operator[](size_t n) const
      {
#ifdef TMVFLDEBUG
	T* pn = p+n*s;
	if (!(p>=first && p<last)) {
	  std::cerr<<"p = "<<p<<std::endl;
	  std::cerr<<"first,last = "<<first<<"  "<<last<<std::endl;
	}
	TMVAssert(pn >= first);
	TMVAssert(pn < first);
	return *pn; 
#else
	return *(p+n*s); 
#endif
      }

      typedef std::random_access_iterator_tag iterator_category;
      typedef T                               value_type;
      typedef ptrdiff_t                       difference_type;
      typedef T*                              pointer;
      typedef T&                              reference;

    private :

      T* p;
      const int s;

#ifdef TMVFLDEBUG
    public :
      const T* first;
      const T* last;
#endif

  };

  template <class T> class CVIter
  { 
    public :

      CVIter() : p(0), s(0) {}
      CVIter(const T* inp, int instep, ConjType DEBUGPARAM(inc)) : 
	p(inp), s(instep) 
      { TMVAssert(inc==NonConj); }
      CVIter(const CVIter<T>& rhs) : p(rhs.p), s(rhs.s) {}
      CVIter(const VIter<T>& rhs) : p(rhs.GetP()), s(rhs.step()) {}
      CVIter<T>& operator=(const CVIter<T>& rhs) 
      { 
	TMVAssert(s == rhs.s);
	p=rhs.p; 
	return *this; 
      }
      ~CVIter() {}

      const T* GetP() const { return p; }
      int step() const { return s; }
      ConjType GetC() const { return NonConj; }

      inline bool operator==(const CVIter<T>& rhs) const 
      { return p == rhs.p; }
      inline bool operator!=(const CVIter<T>& rhs) const 
      { return p != rhs.p; }
      inline bool operator<(const CVIter<T>& rhs) const 
      { return (s > 0 ? p < rhs.p : p > rhs.p); }

      inline T operator*() const { return *p; }

      inline CVIter<T>& operator++() { p += s; return *this; }
      inline CVIter<T>& operator--() { p -= s; return *this; }
      inline CVIter<T> operator++(int) 
      { CVIter<T> p2 = *this; p+=s; return p2; }
      inline CVIter<T> operator--(int) 
      { CVIter<T> p2 = *this; p-=s; return p2; }

      inline CVIter<T>& operator+=(size_t n) { p += n*s; return *this; }
      inline CVIter<T>& operator-=(size_t n) { p -= n*s; return *this; }
      inline CVIter<T> operator+(size_t n) const 
      { return CVIter<T>(p+n*s,s,NonConj); }
      inline CVIter<T> operator-(size_t n) const 
      { return CVIter<T>(p-n*s,s,NonConj); }

      inline ptrdiff_t operator-(const CVIter<T>& rhs) const 
      { return (p-rhs.p)/s; }

      inline T operator[](size_t n) const { return *(p+n*s); }

      typedef std::random_access_iterator_tag iterator_category;
      typedef T                               value_type;
      typedef ptrdiff_t                       difference_type;
      typedef const T*                        pointer;
      typedef const T&                        reference;

    private :

      const T* p;
      const int s;

  };

  // These (C)VIt, rather than (C)VIter have the StepType and 
  // ConjType specified as templates.  When these can be known,
  // it is faster to let the compiler know which variety
  // of iterator you are dealing with.  
  // For example, if the step is 1, p++ is much faster than p+=s
  // (where s==1), so the S=Unit iterator does this.
  // Likewise when you know you do not have a Conjugate iterator, 
  // you can specify NonConj for the ConjType and it doesn't have
  // to deal with the complications of the VarConjRef type.
  
  // The general template specification is really that of
  // T = real, S = Unit, C = irrelevant
  // T = complex and S = Step are specialized below.
  template <class T, StepType S, ConjType C> class VIt 
  { 
    public :

      VIt() : p(0) DEFFIRSTLAST(0,0) {}

      VIt(T* inp, int DEBUGPARAM(step) PARAMFIRSTLAST(T) ) : 
	p(inp) DEFFIRSTLAST(_first,_last)
	{ TMVAssert(step==1); }

      VIt(const VIt<T,S,C>& rhs) : 
	p(rhs.p)  DEFFIRSTLAST(rhs.first,rhs.last) {}

      VIt(const VIter<T>& rhs) : 
	p(rhs.GetP())  DEFFIRSTLAST(rhs.first,rhs.last) 
      { TMVAssert(rhs.step()==1); }

      template <StepType S2> VIt(const VIt<T,S2,C>& rhs) : 
	p(rhs.GetP())  DEFFIRSTLAST(rhs.first,rhs.last) 
      { TMVAssert(rhs.step()==1); }

      VIt<T,S,C>& operator=(const VIt<T,S,C>& rhs) 
      { 
	p=rhs.p; 
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }

      VIt<T,S,C>& operator=(const VIter<T>& rhs) 
      { 
	p=rhs.GetP(); 
	TMVAssert(rhs.step()==1);
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }

      template <StepType S2> VIt<T,S,C>& operator=(const VIt<T,S2,C>& rhs) 
      { 
	p=rhs.GetP(); 
	TMVAssert(rhs.step()==1);
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }

      ~VIt() {}

      T* GetP() const { return p; }
      int step() const { return 1; }

      inline bool operator==(const VIt<T,S,C>& rhs) const 
      { return p == rhs.p; }
      inline bool operator!=(const VIt<T,S,C>& rhs) const 
      { return p != rhs.p; }
      inline bool operator<(const VIt<T,S,C>& rhs) const 
      { return (p < rhs.p); }

      inline T& operator*() const
      { 
#ifdef TMVFLDEBUG
	if (!(p>=first && p<last)) {
	  std::cerr<<"p = "<<p<<std::endl;
	  std::cerr<<"first,last = "<<first<<"  "<<last<<std::endl;
	}
	TMVAssert(p>=first);
	TMVAssert(p<last);
#endif
	return *p; 
      }

      inline VIt<T,S,C>& operator++() { ++p; return *this; }
      inline VIt<T,S,C>& operator--() { --p; return *this; }
      inline VIt<T,S,C> operator++(int) 
      { VIt<T,S,C> p2 = *this; ++p; return p2; }
      inline VIt<T,S,C> operator--(int) 
      { VIt<T,S,C> p2 = *this; --p; return p2; }

      inline VIt<T,S,C>& operator+=(size_t n) { p += n; return *this; }
      inline VIt<T,S,C>& operator-=(size_t n) { p -= n; return *this; }
      inline VIt<T,S,C> operator+(size_t n) const 
      { return VIt<T,S,C>(p+n,1 FIRSTLAST ); }
      inline VIt<T,S,C> operator-(size_t n) const 
      { return VIt<T,S,C>(p-n,1 FIRSTLAST ); }

      inline ptrdiff_t operator-(const VIt<T,S,C>& rhs) const 
      { return p-rhs.p; }

      inline T& operator[](size_t n) const
      {
#ifdef TMVFLDEBUG
	T* pn = p+n;
	if (!(p>=first && p<last)) {
	  std::cerr<<"p = "<<p<<std::endl;
	  std::cerr<<"first,last = "<<first<<"  "<<last<<std::endl;
	}
	TMVAssert(pn >= first);
	TMVAssert(pn < first);
	return *pn; 
#else
	return *(p+n);
#endif
      }

      typedef std::random_access_iterator_tag iterator_category;
      typedef T                               value_type;
      typedef ptrdiff_t                       difference_type;
      typedef T*                              pointer;
      typedef T&                              reference;

    private :

      T* p;

#ifdef TMVFLDEBUG
    public :
      const T* first;
      const T* last;
#endif

  };
  
  template <class T, StepType S, ConjType C> class CVIt 
  { 
    public :

      CVIt() : p(0) {}
      CVIt(const T* inp, int DEBUGPARAM(step)) : p(inp) 
      { TMVAssert(step==1); }
      CVIt(const CVIt<T,S,C>& rhs) : p(rhs.p) { TMVAssert(rhs.step()==1); }
      CVIt(const VIt<T,S,C>& rhs) : p(rhs.GetP()) { TMVAssert(rhs.step()==1); }
      CVIt(const CVIter<T>& rhs) : p(rhs.GetP()) { TMVAssert(rhs.step()==1); }
      CVIt(const VIter<T>& rhs) : p(rhs.GetP()) { TMVAssert(rhs.step()==1); }
      template <StepType S2> CVIt(const CVIt<T,S2,C>& rhs) : p(rhs.GetP())
      { TMVAssert(rhs.step()==1); }
      template <StepType S2> CVIt(const VIt<T,S2,C>& rhs) : p(rhs.GetP())
      { TMVAssert(rhs.step()==1); }

      CVIt<T,S,C>& operator=(const CVIt<T,S,C>& rhs)
      { TMVAssert(rhs.step()==1); p = rhs.p; return *this; }
      CVIt<T,S,C>& operator=(const VIt<T,S,C>& rhs)
      { TMVAssert(rhs.step()==1); p = rhs.GetP(); return *this; }
      CVIt<T,S,C>& operator=(const CVIter<T>& rhs)
      { TMVAssert(rhs.step()==1); p = rhs.GetP(); return *this; }
      CVIt<T,S,C>& operator=(const VIter<T>& rhs)
      { TMVAssert(rhs.step()==1); p = rhs.GetP(); return *this; }
      template <StepType S2> CVIt<T,S,C>& operator=(const CVIt<T,S2,C>& rhs)
      { TMVAssert(rhs.step()==1); p = rhs.GetP(); return *this; }
      template <StepType S2> CVIt<T,S,C>& operator=(const VIt<T,S2,C>& rhs)
      { TMVAssert(rhs.step()==1); p = rhs.GetP(); return *this; }
      ~CVIt() {}

      const T* GetP() const { return p; }
      int step() const { return 1; }

      inline bool operator==(const CVIt<T,S,C>& rhs) const 
      { return p == rhs.p; }
      inline bool operator!=(const CVIt<T,S,C>& rhs) const 
      { return p != rhs.p; }
      inline bool operator<(const CVIt<T,S,C>& rhs) const 
      { return (p < rhs.p); }

      inline T operator*() const { return *p; }

      inline CVIt<T,S,C>& operator++() { ++p; return *this; }
      inline CVIt<T,S,C>& operator--() { --p; return *this; }
      inline CVIt<T,S,C> operator++(int) 
      { CVIt<T,S,C> p2 = *this; ++p; return p2; }
      inline CVIt<T,S,C> operator--(int) 
      { CVIt<T,S,C> p2 = *this; --p; return p2; }

      inline CVIt<T,S,C>& operator+=(size_t n) { p += n; return *this; }
      inline CVIt<T,S,C>& operator-=(size_t n) { p -= n; return *this; }
      inline CVIt<T,S,C> operator+(size_t n) const 
      { return CVIt<T,S,C>(p+n,1); }
      inline CVIt<T,S,C> operator-(size_t n) const 
      { return CVIt<T,S,C>(p-n,1); }

      inline ptrdiff_t operator-(const CVIt<T,S,C>& rhs) const 
      { return (p-rhs.p); }

      inline T operator[](size_t n) const { return *(p+n); }

      typedef std::random_access_iterator_tag iterator_category;
      typedef T                               value_type;
      typedef ptrdiff_t                       difference_type;
      typedef const T*                        pointer;
      typedef const T&                        reference;

    private :

      const T* p;
  };

  template <class T, ConjType C> class VIt<T,Step,C> 
  { 
    public :

      VIt() : p(0), s(0) DEFFIRSTLAST(0,0) {}
      VIt(T* inp, int instep PARAMFIRSTLAST(T) ) : 
	p(inp), s(instep) DEFFIRSTLAST(_first,_last) {}
      VIt(const VIt<T,Step,C>& rhs) : 
	p(rhs.p), s(rhs.s) DEFFIRSTLAST(rhs.first,rhs.last) {}
      VIt(const VIt<T,Unit,C>& rhs) : 
	p(rhs.GetP()), s(1) DEFFIRSTLAST(rhs.first,rhs.last) {}
      VIt(const VIter<T>& rhs) : 
	p(rhs.GetP()), s(rhs.step()) DEFFIRSTLAST(rhs.first,rhs.last) {}
      VIt<T,Step,C>& operator=(const VIt<T,Step,C>& rhs) 
      { 
	TMVAssert(s==rhs.s);
	p=rhs.p; 
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }
      VIt<T,Step,C>& operator=(const VIt<T,Unit,C>& rhs) 
      { 
	TMVAssert(s==1);
	p=rhs.GetP(); 
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }
      VIt<T,Step,C>& operator=(const VIter<T>& rhs) 
      { 
	TMVAssert(s==rhs.step());
	p=rhs.GetP(); 
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }
      ~VIt() {}

      T* GetP() const { return p; }
      int step() const { return s; }

      inline bool operator==(const VIt<T,Step,C>& rhs) const 
      { return p == rhs.p; }
      inline bool operator!=(const VIt<T,Step,C>& rhs) const 
      { return p != rhs.p; }
      inline bool operator<(const VIt<T,Step,C>& rhs) const 
      { return (s > 0 ? p < rhs.p : p > rhs.p); }

      inline T& operator*() const
      { 
#ifdef TMVFLDEBUG
	if (!(p>=first && p<last)) {
	  std::cerr<<"p = "<<p<<std::endl;
	  std::cerr<<"first,last = "<<first<<"  "<<last<<std::endl;
	}
	TMVAssert(p>=first);
	TMVAssert(p<last);
#endif
	return *p; 
      }

      inline VIt<T,Step,C>& operator++() { p += s; return *this; }
      inline VIt<T,Step,C>& operator--() { p -= s; return *this; }
      inline VIt<T,Step,C> operator++(int) 
      { VIt<T,Step,C> p2 = *this; p+=s; return p2; }
      inline VIt<T,Step,C> operator--(int) 
      { VIt<T,Step,C> p2 = *this; p-=s; return p2; }

      inline VIt<T,Step,C>& operator+=(size_t n) { p += n*s; return *this; }
      inline VIt<T,Step,C>& operator-=(size_t n) { p -= n*s; return *this; }
      inline VIt<T,Step,C> operator+(size_t n) const 
      { return VIt<T,Step,C>(p+n*s,s FIRSTLAST ); }
      inline VIt<T,Step,C> operator-(size_t n) const 
      { return VIt<T,Step,C>(p-n*s,s FIRSTLAST ); }

      inline ptrdiff_t operator-(const VIt<T,Step,C>& rhs) const 
      { return (p-rhs.p)/s; }

      inline T& operator[](size_t n) const
      {
#ifdef TMVFLDEBUG
	T* pn = p+n*s;
	if (!(pn>=first && pn<last)) {
	  std::cerr<<"pn = "<<pn<<std::endl;
	  std::cerr<<"first,last = "<<first<<"  "<<last<<std::endl;
	}
	TMVAssert(pn >= first);
	TMVAssert(pn < first);
	return *pn; 
#else
	return *(p+n*s); 
#endif
      }

      typedef std::random_access_iterator_tag iterator_category;
      typedef T                               value_type;
      typedef ptrdiff_t                       difference_type;
      typedef T*                              pointer;
      typedef T&                              reference;

    private :

      T* p;
      const int s;

#ifdef TMVFLDEBUG
    public :
      const T* first;
      const T* last;
#endif

  };

  template <class T, ConjType C> class CVIt<T,Step,C> 
  { 
    public :

      CVIt() : p(0), s(0) {}
      CVIt(const T* inp, int instep) : p(inp), s(instep) {}
      CVIt(const CVIt<T,Step,C>& rhs) : p(rhs.p), s(rhs.s) {}
      CVIt(const VIt<T,Step,C>& rhs) : p(rhs.GetP()), s(rhs.step()) {}
      CVIt(const CVIt<T,Unit,C>& rhs) : p(rhs.GetP()), s(1) {}
      CVIt(const VIt<T,Unit,C>& rhs) : p(rhs.GetP()), s(1) {}
      CVIt(const CVIter<T>& rhs) : p(rhs.GetP()), s(rhs.step()) {}
      CVIt(const VIter<T>& rhs) : p(rhs.GetP()), s(rhs.step()) {}
      CVIt<T,Step,C>& operator=(const CVIt<T,Step,C>& rhs)
      { TMVAssert(s==rhs.s); p = rhs.p; return *this; }
      CVIt<T,Step,C>& operator=(const VIt<T,Step,C>& rhs)
      { TMVAssert(s==rhs.step()); p = rhs.GetP(); return *this; }
      CVIt<T,Step,C>& operator=(const CVIt<T,Unit,C>& rhs)
      { TMVAssert(s==1); p = rhs.GetP(); return *this; }
      CVIt<T,Step,C>& operator=(const VIt<T,Unit,C>& rhs)
      { TMVAssert(s==1); p = rhs.GetP(); return *this; }
      CVIt<T,Step,C>& operator=(const CVIter<T>& rhs)
      { TMVAssert(s==rhs.step()); p = rhs.GetP(); return *this; }
      CVIt<T,Step,C>& operator=(const VIter<T>& rhs)
      { TMVAssert(s==rhs.step()); p = rhs.GetP(); return *this; }
      ~CVIt() {}

      const T* GetP() const { return p; }
      int step() const { return s; }

      inline bool operator==(const CVIt<T,Step,C>& rhs) const 
      { return p == rhs.p; }
      inline bool operator!=(const CVIt<T,Step,C>& rhs) const 
      { return p != rhs.p; }
      inline bool operator<(const CVIt<T,Step,C>& rhs) const 
      { return (s > 0 ? p < rhs.p : p > rhs.p); }

      inline T operator*() const { return *p; }

      inline CVIt<T,Step,C>& operator++() { p += s; return *this; }
      inline CVIt<T,Step,C>& operator--() { p -= s; return *this; }
      inline CVIt<T,Step,C> operator++(int) 
      { CVIt<T,Step,C> p2 = *this; p+=s; return p2; }
      inline CVIt<T,Step,C> operator--(int) 
      { CVIt<T,Step,C> p2 = *this; p-=s; return p2; }

      inline CVIt<T,Step,C>& operator+=(size_t n) 
      { p += n*s; return *this; }
      inline CVIt<T,Step,C>& operator-=(size_t n) 
      { p -= n*s; return *this; }
      inline CVIt<T,Step,C> operator+(size_t n) const 
      { return CVIt<T,Step,C>(p+n*s,s); }
      inline CVIt<T,Step,C> operator-(size_t n) const 
      { return CVIt<T,Step,C>(p-n*s,s); }

      inline ptrdiff_t operator-(const CVIt<T,Step,C>& rhs) const 
      { return (p-rhs.p)/s; }

      inline T operator[](size_t n) const { return *(p+n*s); }

      typedef std::random_access_iterator_tag iterator_category;
      typedef T                               value_type;
      typedef ptrdiff_t                       difference_type;
      typedef const T*                        pointer;
      typedef const T&                        reference;

    private :

      const T* p;
      const int s;
  };

  template <class T> class ConjRef
  {
    public:

      explicit ConjRef(T& _val) : val(_val) {}
      ConjRef(const ConjRef<T>& rhs) : val(rhs.val) {}
      ~ConjRef() {}

      inline operator T() const { return std::conj(val); }
      inline T& GetRef() { return val; }
      inline T conj() const { return val; }
      inline RealType(T) real() const { return val.real(); }
      inline RealType(T) imag() const { return -val.imag(); }
      inline T operator-() const { return -std::conj(val); }

      inline ConjRef<T> operator=(ConjRef<T> rhs) const
      { val = rhs.val; return *this; }
      inline ConjRef<T> operator=(T rhs) const
      { val = std::conj(rhs); return *this; }
      inline ConjRef<T> operator=(RealType(T) rhs) const
      { val = rhs; return *this; }

      inline ConjRef<T> operator+=(ConjRef<T> x2) const
      { val += x2.val; return *this; }
      inline ConjRef<T> operator+=(T x2) const
      { val += std::conj(x2); return *this; }
      inline ConjRef<T> operator+=(RealType(T) x2) const
      { val += x2; return *this; }
      inline T operator+(ConjRef<T> x2)
      { return std::conj(val+x2.val); }
      inline friend T operator+(ConjRef<T> x1, T x2)
      { return std::conj(x1.val)+x2; }
      inline friend T operator+(ConjRef<T> x1, RealType(T) x2)
      { return std::conj(x1.val)+x2; }
      inline friend T operator+(T x1, ConjRef<T> x2)
      { return x1+std::conj(x2.val); }
      inline friend T operator+(RealType(T) x1, ConjRef<T> x2)
      { return x1+std::conj(x2.val); }
      inline friend T& operator+=(T& x1, ConjRef<T> x2)
      { return x1+=std::conj(x2.val); }

      inline ConjRef<T> operator-=(ConjRef<T> x2) const
      { val -= x2.val; return *this; }
      inline ConjRef<T> operator-=(T x2) const
      { val -= std::conj(x2); return *this; }
      inline ConjRef<T> operator-=(RealType(T) x2) const
      { val -= x2; return *this; }
      inline T operator-(ConjRef<T> x2)
      { return std::conj(val-x2.val); }
      inline friend T operator-(ConjRef<T> x1, T x2)
      { return std::conj(x1.val)-x2; }
      inline friend T operator-(ConjRef<T> x1, RealType(T) x2)
      { return std::conj(x1.val)-x2; }
      inline friend T operator-(T x1, ConjRef<T> x2)
      { return x1-std::conj(x2.val); }
      inline friend T operator-(RealType(T) x1, ConjRef<T> x2)
      { return x1-std::conj(x2.val); }
      inline friend T& operator-=(T& x1, ConjRef<T> x2)
      { return x1-=std::conj(x2.val); }

      inline ConjRef<T> operator*=(ConjRef<T> x2) const
      { val *= x2.val; return *this; }
      inline ConjRef<T> operator*=(T x2) const
      { val *= std::conj(x2); return *this; }
      inline ConjRef<T> operator*=(RealType(T) x2) const
      { val *= x2; return *this; }
      inline T operator*(ConjRef<T> x2)
      { return std::conj(val*x2.val); }
      inline friend T operator*(ConjRef<T> x1, T x2)
      { return std::conj(x1.val)*x2; }
      inline friend T operator*(ConjRef<T> x1, RealType(T) x2)
      { return std::conj(x1.val)*x2; }
      inline friend T operator*(T x1, ConjRef<T> x2)
      { return x1*std::conj(x2.val); }
      inline friend T operator*(RealType(T) x1, ConjRef<T> x2)
      { return x1*std::conj(x2.val); }
      inline friend T& operator*=(T& x1, ConjRef<T> x2)
      { return x1*=std::conj(x2.val); }

      inline ConjRef<T> operator/=(ConjRef<T> x2) const
      { val /= x2.val; return *this; }
      inline ConjRef<T> operator/=(T x2) const
      { val /= std::conj(x2); return *this; }
      inline ConjRef<T> operator/=(RealType(T) x2) const
      { val /= x2; return *this; }
      inline T operator/(ConjRef<T> x2)
      { return std::conj(val/x2.val); }
      inline friend T operator/(ConjRef<T> x1, T x2)
      { return std::conj(x1.val)/x2; }
      inline friend T operator/(ConjRef<T> x1, RealType(T) x2)
      { return std::conj(x1.val)/x2; }
      inline friend T operator/(T x1, ConjRef<T> x2)
      { return x1/std::conj(x2.val); }
      inline friend T operator/(RealType(T) x1, ConjRef<T> x2)
      { return x1/std::conj(x2.val); }
      inline friend T& operator/=(T& x1, ConjRef<T> x2)
      { return x1/=std::conj(x2.val); }

      inline bool operator==(ConjRef<T> x2) const
      { return val == x2.val; }
      inline bool operator==(T x2) const 
      { return std::conj(val) == x2; }
      inline bool operator==(RealType(T) x2) const 
      { return std::real(val) == x2 && std::imag(val) == T(0); }
      inline friend bool operator==(T x1, ConjRef<T> x2)
      { return x2==x1; }
      inline friend bool operator==(RealType(T) x1, ConjRef<T> x2)
      { return x2==x1; }
      inline bool operator!=(ConjRef<T> x2) const
      { return !(operator==(x2)); }
      inline bool operator!=(T x2) const 
      { return !(operator==(x2)); }
      inline bool operator!=(RealType(T) x2) const 
      { return !(operator==(x2)); }
      inline friend bool operator!=(T x1, ConjRef<T> x2)
      { return !(x2==x1); }
      inline friend bool operator!=(RealType(T) x1, ConjRef<T> x2)
      { return !(x2==x1); }

      inline void SwapWith(T& x2)
      { 
	TMVAssert(&val != &x2);
	T temp = x2; x2 = std::conj(val); val = std::conj(temp); 
      }
      inline void SwapWith(ConjRef<T> x2)
      { 
	TMVAssert(&val != &x2);
	T temp = x2.val; x2.val = val; val = temp; 
      }

      inline friend std::ostream& operator<<(std::ostream& os, ConjRef<T> x)
      { os << std::conj(x.val); return os; }
      inline friend std::istream& operator>>(std::istream& is, ConjRef<T> x)
      { is >> x.val; x.val = std::conj(x.val); return is; }

    private:

      T& val;
  };

  template <class T> inline T CONJ(ConjRef<T> x) { return x.conj(); }
  template <class T> inline T conj(ConjRef<T> x) { return x.conj(); }
  template <class T> inline RealType(T) NORM(ConjRef<T> x) 
  { return norm(x.conj()); }
  template <class T> inline RealType(T) norm(ConjRef<T> x) 
  { return norm(x.conj()); }
  template <class T> inline RealType(T) ABS(ConjRef<T> x) 
  { return std::abs(x.conj()); }
  template <class T> inline T SQR(ConjRef<T> x) { return SQR(T(x)); }
  template <class T> inline T SQRT(ConjRef<T> x) { return SQRT(T(x)); }
  template <class T> inline RealType(T) REAL(ConjRef<T> x) 
  { return x.real(); }
  template <class T> inline RealType(T) IMAG(ConjRef<T> x) 
  { return x.imag(); }
  template <class T> inline RealType(T) real(ConjRef<T> x) 
  { return x.real(); }
  template <class T> inline RealType(T) imag(ConjRef<T> x) 
  { return x.imag(); }
}

namespace std {
  template <class T> inline void swap(tmv::ConjRef<std::complex<T> > x1,
      tmv::ConjRef<std::complex<T> > x2)
  { return x1.SwapWith(x2); }
  template <class T> inline void swap(std::complex<T>& x1,
      tmv::ConjRef<std::complex<T> > x2)
  { return x2.SwapWith(x1); }
  template <class T> inline void swap(tmv::ConjRef<std::complex<T> > x1,
      std::complex<T>& x2)
  { return x1.SwapWith(x2); }
}

namespace tmv {

  template <class T> class VIt<std::complex<T>,Unit,Conj> 
  { 
    public :

      VIt() : p(0) DEFFIRSTLAST(0,0) {}
      VIt(std::complex<T>* inp, int DEBUGPARAM(step) 
	  PARAMFIRSTLAST(std::complex<T>)) :
	p(inp) DEFFIRSTLAST(_first,_last)
      { TMVAssert(step==1); }
      VIt(const VIt<std::complex<T>,Unit,Conj>& rhs) : 
	p(rhs.p) DEFFIRSTLAST(rhs.first,rhs.last) {}
      VIt(const VIt<std::complex<T>,Step,Conj>& rhs) : 
	p(rhs.GetP())  DEFFIRSTLAST(rhs.first,rhs.last) 
      { TMVAssert(rhs.step() == 1); }
      VIt(const VIter<std::complex<T> >& rhs) : 
	p(rhs.GetP())  DEFFIRSTLAST(rhs.first,rhs.last) 
      { 
	TMVAssert(rhs.step() == 1); 
	TMVAssert(rhs.GetC() == Conj);
      }
      VIt& operator=(const VIt<std::complex<T>,Unit,Conj>& rhs) 
      { 
	p=rhs.p; 
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }
      VIt& operator=(const VIt<std::complex<T>,Step,Conj>& rhs) 
      { 
	TMVAssert(rhs.step()==1);
	p=rhs.GetP(); 
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }
      VIt& operator=(const VIter<std::complex<T> >& rhs) 
      { 
	TMVAssert(rhs.step()==1);
	TMVAssert(rhs.GetC()==Conj);
	p=rhs.GetP(); 
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }
      ~VIt() {}

      std::complex<T>* GetP() const { return p; }
      int step() const { return 1; }

      inline bool operator==(const VIt<std::complex<T>,Unit,Conj>& rhs) const 
      { return p == rhs.p; }
      inline bool operator!=(const VIt<std::complex<T>,Unit,Conj>& rhs) const 
      { return p != rhs.p; }
      inline bool operator<(const VIt<std::complex<T>,Unit,Conj>& rhs) const 
      { return (p < rhs.p); }

      inline ConjRef<std::complex<T> > operator*() const
      { 
#ifdef TMVFLDEBUG
	if (!(p>=first && p<last)) {
	  std::cerr<<"p = "<<p<<std::endl;
	  std::cerr<<"first,last = "<<first<<"  "<<last<<std::endl;
	}
	TMVAssert(p>=first);
	TMVAssert(p<last);
#endif
	return ConjRef<std::complex<T> >(*p);
      }

      inline VIt<std::complex<T>,Unit,Conj>& operator++() { ++p; return *this; }
      inline VIt<std::complex<T>,Unit,Conj>& operator--() { --p; return *this; }
      inline VIt<std::complex<T>,Unit,Conj> operator++(int) 
      { VIt<std::complex<T>,Unit,Conj> p2 = *this; ++p; return p2; }
      inline VIt<std::complex<T>,Unit,Conj> operator--(int) 
      { VIt<std::complex<T>,Unit,Conj> p2 = *this; --p; return p2; }

      inline VIt<std::complex<T>,Unit,Conj>& operator+=(size_t n) 
      { p += n; return *this; }
      inline VIt<std::complex<T>,Unit,Conj>& operator-=(size_t n) 
      { p -= n; return *this; }
      inline VIt<std::complex<T>,Unit,Conj> operator+(size_t n) const 
      { return VIt<std::complex<T>,Unit,Conj>(p+n,1 FIRSTLAST ); }
      inline VIt<std::complex<T>,Unit,Conj> operator-(size_t n) const 
      { return VIt<std::complex<T>,Unit,Conj>(p-n,1 FIRSTLAST ); }

      inline ptrdiff_t operator-(const VIt<std::complex<T>,Unit,Conj>& rhs) const 
      { return (p-rhs.p); }

      inline std::complex<T>& operator[](size_t n) const
      {
#ifdef TMVFLDEBUG
	std::complex<T>* pn = p+n;
	if (!(pn>=first && pn<last)) {
	  std::cerr<<"pn = "<<pn<<std::endl;
	  std::cerr<<"first,last = "<<first<<"  "<<last<<std::endl;
	}
	TMVAssert(pn >= first);
	TMVAssert(pn < first);
	return ConjRef<std::complex<T> >(*pn); 
#else
	return ConjRef<std::complex<T> >(*(p+n)); 
#endif
      }

      typedef std::random_access_iterator_tag iterator_category;
      typedef std::complex<T>                      value_type;
      typedef ptrdiff_t                       difference_type;
      typedef VIt<std::complex<T>,Unit,Conj>       pointer;
      typedef ConjRef<std::complex<T> >            reference;

    private :

      std::complex<T>* p;

#ifdef TMVFLDEBUG
    public :
      const std::complex<T>* first;
      const std::complex<T>* last;
#endif
  };

  template <class T> class CVIt<std::complex<T>,Unit,Conj> 
  { 
    public :

      CVIt() : p(0) {}
      CVIt(const std::complex<T>* inp, int DEBUGPARAM(step)) : p(inp) 
      { TMVAssert(step==1); }
      CVIt(const CVIt<std::complex<T>,Unit,Conj>& rhs) : p(rhs.p) {}
      CVIt(const VIt<std::complex<T>,Unit,Conj>& rhs) : p(rhs.GetP()) {}
      CVIt(const CVIt<std::complex<T>,Step,Conj>& rhs) : p(rhs.GetP())
      { TMVAssert(rhs.step() == 1); }
      CVIt(const VIt<std::complex<T>,Step,Conj>& rhs) : p(rhs.GetP())
      { TMVAssert(rhs.step() == 1); }
      CVIt(const CVIter<std::complex<T> >& rhs) : p(rhs.GetP())
      { 
	TMVAssert(rhs.GetC() == Conj);
	TMVAssert(rhs.step() == 1); 
      }
      CVIt(const VIter<std::complex<T> >& rhs) : p(rhs.GetP())
      { 
	TMVAssert(rhs.GetC() == Conj);
	TMVAssert(rhs.step() == 1); 
      }
      CVIt<std::complex<T>,Unit,Conj>& operator=(
	  const CVIt<std::complex<T>,Unit,Conj>& rhs)
      { p = rhs.p; return *this; }
      CVIt<std::complex<T>,Unit,Conj>& operator=(
	  const VIt<std::complex<T>,Unit,Conj>& rhs)
      { p = rhs.GetP(); return *this; }
      CVIt<std::complex<T>,Unit,Conj>& operator=(
	  const CVIt<std::complex<T>,Step,Conj>& rhs)
      { TMVAssert(rhs.step()==1); p = rhs.GetP(); return *this; }
      CVIt<std::complex<T>,Unit,Conj>& operator=(
	  const VIt<std::complex<T>,Step,Conj>& rhs)
      { TMVAssert(rhs.step()==1); p = rhs.GetP(); return *this; }
      CVIt<std::complex<T>,Unit,Conj>& operator=(const CVIter<std::complex<T> >& rhs)
      { 
	TMVAssert(rhs.GetC() == Conj);
	TMVAssert(rhs.step()==1); 
	p = rhs.GetP(); 
	return *this; 
      }
      CVIt<std::complex<T>,Unit,Conj>& operator=(const VIter<std::complex<T> >& rhs)
      { 
	TMVAssert(rhs.GetC() == Conj);
	TMVAssert(rhs.step()==1); 
	p = rhs.GetP(); 
	return *this; 
      }
      ~CVIt() {}

      const std::complex<T>* GetP() const { return p; }
      int step() const { return 1; }

      inline bool operator==(const CVIt<std::complex<T>,Unit,Conj>& rhs) const 
      { return p == rhs.p; }
      inline bool operator!=(const CVIt<std::complex<T>,Unit,Conj>& rhs) const 
      { return p != rhs.p; }
      inline bool operator<(const CVIt<std::complex<T>,Unit,Conj>& rhs) const 
      { return (p < rhs.p); }

      inline std::complex<T> operator*() const { return std::conj(*p); }

      inline CVIt<std::complex<T>,Unit,Conj>& operator++() { ++p; return *this; }
      inline CVIt<std::complex<T>,Unit,Conj>& operator--() { --p; return *this; }
      inline CVIt<std::complex<T>,Unit,Conj> operator++(int) 
      { CVIt<std::complex<T>,Unit,Conj> p2 = *this; ++p; return p2; }
      inline CVIt<std::complex<T>,Unit,Conj> operator--(int) 
      { CVIt<std::complex<T>,Unit,Conj> p2 = *this; --p; return p2; }

      inline CVIt<std::complex<T>,Unit,Conj>& operator+=(size_t n) 
      { p += n; return *this; }
      inline CVIt<std::complex<T>,Unit,Conj>& operator-=(size_t n) 
      { p -= n; return *this; }
      inline CVIt<std::complex<T>,Unit,Conj> operator+(size_t n) const 
      { return CVIt<std::complex<T>,Unit,Conj>(p+n,1); }
      inline CVIt<std::complex<T>,Unit,Conj> operator-(size_t n) const 
      { return CVIt<std::complex<T>,Unit,Conj>(p-n,1); }

      inline ptrdiff_t operator-(const CVIt<std::complex<T>,Unit,Conj>& rhs) const 
      { return (p-rhs.p); }

      inline std::complex<T> operator[](size_t n) const 
      { return std::conj(*(p+n)); }

      typedef std::random_access_iterator_tag iterator_category;
      typedef std::complex<T>                      value_type;
      typedef ptrdiff_t                       difference_type;
      typedef CVIt<std::complex<T>,Unit,Conj>      pointer;
      typedef const ConjRef<std::complex<T> >      reference;

    private :

      const std::complex<T>* p;
  };

  template <class T> class VIt<std::complex<T>,Step,Conj> 
  { 
    public :

      VIt() : p(0), s(0) DEFFIRSTLAST(0,0) {}
      VIt(std::complex<T>* inp, int instep PARAMFIRSTLAST(std::complex<T>)) :
	p(inp), s(instep) DEFFIRSTLAST(_first,_last) {}
      VIt(const VIt<std::complex<T>,Step,Conj>& rhs) : 
	p(rhs.p), s(rhs.s) DEFFIRSTLAST(rhs.first,rhs.last) {}
      VIt(const VIt<std::complex<T>,Unit,Conj>& rhs) : 
	p(rhs.GetP()), s(1) DEFFIRSTLAST(rhs.first,rhs.last) {}
      VIt(const VIter<std::complex<T> >& rhs) : 
	p(rhs.GetP()), s(rhs.step()) DEFFIRSTLAST(rhs.first,rhs.last) 
      { TMVAssert(rhs.GetC()==Conj); }
      VIt& operator=(const VIt<std::complex<T>,Step,Conj>& rhs) 
      { 
	TMVAssert(s==rhs.s);
	p=rhs.p; 
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }
      VIt& operator=(const VIt<std::complex<T>,Unit,Conj>& rhs) 
      { 
	TMVAssert(s==1);
	p=rhs.GetP(); 
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }
      VIt& operator=(const VIter<std::complex<T> >& rhs) 
      { 
	TMVAssert(s==rhs.step());
	TMVAssert(rhs.GetC()==Conj);
	p=rhs.GetP(); 
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }
      ~VIt() {}

      std::complex<T>* GetP() const { return p; }
      int step() const { return s; }

      inline bool operator==(const VIt<std::complex<T>,Step,Conj>& rhs) const 
      { return p == rhs.p; }
      inline bool operator!=(const VIt<std::complex<T>,Step,Conj>& rhs) const 
      { return p != rhs.p; }
      inline bool operator<(const VIt<std::complex<T>,Step,Conj>& rhs) const 
      { return (s > 0 ? p < rhs.p : p > rhs.p); }

      inline ConjRef<std::complex<T> > operator*() const
      { 
#ifdef TMVFLDEBUG
	if (!(p>=first && p<last)) {
	  std::cerr<<"p = "<<p<<std::endl;
	  std::cerr<<"first,last = "<<first<<"  "<<last<<std::endl;
	}
	TMVAssert(p>=first);
	TMVAssert(p<last);
#endif
	return ConjRef<std::complex<T> >(*p);
      }

      inline VIt<std::complex<T>,Step,Conj>& operator++() { p+=s; return *this; }
      inline VIt<std::complex<T>,Step,Conj>& operator--() { p-=s; return *this; }
      inline VIt<std::complex<T>,Step,Conj> operator++(int) 
      { VIt<std::complex<T>,Step,Conj> p2 = *this; p+=s; return p2; }
      inline VIt<std::complex<T>,Step,Conj> operator--(int) 
      { VIt<std::complex<T>,Step,Conj> p2 = *this; p-=s; return p2; }

      inline VIt<std::complex<T>,Step,Conj>& operator+=(size_t n) 
      { p += n*s; return *this; }
      inline VIt<std::complex<T>,Step,Conj>& operator-=(size_t n) 
      { p -= n*s; return *this; }
      inline VIt<std::complex<T>,Step,Conj> operator+(size_t n) const 
      { return VIt<std::complex<T>,Step,Conj>(p+n*s,s FIRSTLAST ); }
      inline VIt<std::complex<T>,Step,Conj> operator-(size_t n) const 
      { return VIt<std::complex<T>,Step,Conj>(p-n*s,s FIRSTLAST ); }

      inline ptrdiff_t operator-(const VIt<std::complex<T>,Step,Conj>& rhs) const 
      { return (p-rhs.p)/s; }

      inline std::complex<T>& operator[](size_t n) const
      {
#ifdef TMVFLDEBUG
	std::complex<T>* pn = p+n*s;
	if (!(pn>=first && pn<last)) {
	  std::cerr<<"pn = "<<pn<<std::endl;
	  std::cerr<<"first,last = "<<first<<"  "<<last<<std::endl;
	}
	TMVAssert(pn >= first);
	TMVAssert(pn < first);
	return ConjRef<std::complex<T> >(*pn); 
#else
	return ConjRef<std::complex<T> >(*(p+n*s)); 
#endif
      }

      typedef std::random_access_iterator_tag iterator_category;
      typedef std::complex<T>                      value_type;
      typedef ptrdiff_t                       difference_type;
      typedef VIt<std::complex<T>,Step,Conj>       pointer;
      typedef ConjRef<std::complex<T> >            reference;

    private :

      std::complex<T>* p;
      const int s;

#ifdef TMVFLDEBUG
    public :
      const std::complex<T>* first;
      const std::complex<T>* last;
#endif
  };

  template <class T> class CVIt<std::complex<T>,Step,Conj> 
  { 
    public :

      CVIt() : p(0), s(0) {}
      CVIt(const std::complex<T>* inp, int instep) : p(inp), s(instep) {}
      CVIt(const CVIt<std::complex<T>,Step,Conj>& rhs) : p(rhs.p), s(rhs.s) {}
      CVIt(const VIt<std::complex<T>,Step,Conj>& rhs) : 
	p(rhs.GetP()), s(rhs.step()) {}
      CVIt(const CVIt<std::complex<T>,Unit,Conj>& rhs) : p(rhs.GetP()), s(1) {}
      CVIt(const VIt<std::complex<T>,Unit,Conj>& rhs) : p(rhs.GetP()), s(1) {}
      CVIt(const CVIter<std::complex<T> >& rhs) : p(rhs.GetP()), s(rhs.step()) 
      { TMVAssert(rhs.GetC()==Conj); }
      CVIt(const VIter<std::complex<T> >& rhs) : p(rhs.GetP()), s(rhs.step()) 
      { TMVAssert(rhs.GetC()==Conj); }
      CVIt<std::complex<T>,Step,Conj>& operator=(
	  const CVIt<std::complex<T>,Step,Conj>& rhs)
      { TMVAssert(s==rhs.s); p = rhs.p; return *this; }
      CVIt<std::complex<T>,Step,Conj>& operator=(
	  const VIt<std::complex<T>,Step,Conj>& rhs)
      { TMVAssert(s==rhs.s); p = rhs.GetP(); return *this; }
      CVIt<std::complex<T>,Step,Conj>& operator=(
	  const CVIt<std::complex<T>,Unit,Conj>& rhs)
      { TMVAssert(s==1); p = rhs.GetP(); return *this; }
      CVIt<std::complex<T>,Step,Conj>& operator=(
	  const VIt<std::complex<T>,Unit,Conj>& rhs)
      { TMVAssert(s==1); p = rhs.GetP(); return *this; }
      CVIt<std::complex<T>,Step,Conj>& operator=(const CVIter<std::complex<T> >& rhs)
      { 
	TMVAssert(s==rhs.step()); 
	TMVAssert(rhs.GetC()==Conj);
	p = rhs.GetP(); return *this; 
      }
      CVIt<std::complex<T>,Step,Conj>& operator=(const VIter<std::complex<T> >& rhs)
      { 
	TMVAssert(s==rhs.step()); 
	TMVAssert(rhs.GetC()==Conj);
	p = rhs.GetP(); return *this; 
      }
      ~CVIt() {}

      const std::complex<T>* GetP() const { return p; }
      int step() const { return s; }

      inline bool operator==(const CVIt<std::complex<T>,Step,Conj>& rhs) const 
      { return p == rhs.p; }
      inline bool operator!=(const CVIt<std::complex<T>,Step,Conj>& rhs) const 
      { return p != rhs.p; }
      inline bool operator<(const CVIt<std::complex<T>,Step,Conj>& rhs) const 
      { return (s > 0 ? p < rhs.p : p > rhs.p); }

      inline std::complex<T> operator*() const { return std::conj(*p); }

      inline CVIt<std::complex<T>,Step,Conj>& operator++() { p+=s; return *this; }
      inline CVIt<std::complex<T>,Step,Conj>& operator--() { p-=s; return *this; }
      inline CVIt<std::complex<T>,Step,Conj> operator++(int) 
      { CVIt<std::complex<T>,Step,Conj> p2 = *this; p+=s; return p2; }
      inline CVIt<std::complex<T>,Step,Conj> operator--(int) 
      { CVIt<std::complex<T>,Step,Conj> p2 = *this; p-=s; return p2; }

      inline CVIt<std::complex<T>,Step,Conj>& operator+=(size_t n) 
      { p += n*s; return *this; }
      inline CVIt<std::complex<T>,Step,Conj>& operator-=(size_t n) 
      { p -= n*s; return *this; }
      inline CVIt<std::complex<T>,Step,Conj> operator+(size_t n) const 
      { return CVIt<std::complex<T>,Step,Conj>(p+n*s,s); }
      inline CVIt<std::complex<T>,Step,Conj> operator-(size_t n) const 
      { return CVIt<std::complex<T>,Step,Conj>(p-n*s,s); }

      inline ptrdiff_t operator-(const CVIt<std::complex<T>,Step,Conj>& rhs) const 
      { return (p-rhs.p)/s; }

      inline std::complex<T> operator[](size_t n) const 
      { return std::conj(*(p+n*s)); }

      typedef std::random_access_iterator_tag iterator_category;
      typedef std::complex<T>                      value_type;
      typedef ptrdiff_t                       difference_type;
      typedef CVIt<std::complex<T>,Step,Conj>      pointer;
      typedef const ConjRef<std::complex<T> >      reference;

    private :

      const std::complex<T>* p;
      const int s;
  };

  template <class T> class VarConjRef
  {
    public:

      VarConjRef(T& _val, ConjType _c) : val(_val), c(_c) {}
      VarConjRef(const VarConjRef<T>& rhs) : val(rhs.val), c(rhs.c) {}
      ~VarConjRef() {}

      inline bool isconj() const { return c == Conj; }
      inline operator T() const { return c==Conj?std::conj(val):val; }
      inline T& GetRef() { return val; }
      inline T conj() const { return c==Conj?val:std::conj(val); }
      inline RealType(T) real() const { return val.real(); }
      inline RealType(T) imag() const 
      { return c==Conj? -val.imag() : val.imag(); }
      inline T operator-() const { return -T(*this); }

      inline VarConjRef<T> operator=(VarConjRef<T> rhs) const
      { val = c==rhs.c ? rhs.val : std::conj(rhs.val); return *this; }
      inline VarConjRef<T> operator=(T rhs) const
      { val = c==Conj ? std::conj(rhs) : rhs; return *this; }
      inline VarConjRef<T> operator=(RealType(T) rhs) const
      { val = rhs; return *this; }

      inline VarConjRef<T> operator+=(VarConjRef<T> x2) const
      { val += c==x2.c ? x2.val : std::conj(x2.val); return *this; }
      inline VarConjRef<T> operator+=(T x2) const
      { val += c==Conj ? std::conj(x2) : x2; return *this; }
      inline VarConjRef<T> operator+=(RealType(T) x2) const
      { val += x2; return *this; }
      inline T operator+(VarConjRef<T> x2)
      { return T(*this)+T(x2); }
      inline friend T operator+(VarConjRef<T> x1, T x2)
      { return T(x1)+x2; }
      inline friend T operator+(VarConjRef<T> x1, RealType(T) x2)
      { return T(x1)+x2; }
      inline friend T operator+(T x1, VarConjRef<T> x2)
      { return x1+T(x2); }
      inline friend T operator+(RealType(T) x1, VarConjRef<T> x2)
      { return x1+T(x2); }
      inline friend T& operator+=(T& x1, VarConjRef<T> x2)
      { return x1 += (x2.c==Conj ? std::conj(x2.val) : x2.val); }

      inline VarConjRef<T> operator-=(VarConjRef<T> x2) const
      { val -= c==x2.c ? x2.val : std::conj(x2.val); return *this; }
      inline VarConjRef<T> operator-=(T x2) const
      { val -= c==Conj ? std::conj(x2) : x2; return *this; }
      inline VarConjRef<T> operator-=(RealType(T) x2) const
      { val -= x2; return *this; }
      inline T operator-(VarConjRef<T> x2)
      { return T(*this)-T(x2); }
      inline friend T operator-(VarConjRef<T> x1, T x2)
      { return T(x1)-x2; }
      inline friend T operator-(VarConjRef<T> x1, RealType(T) x2)
      { return T(x1)-x2; }
      inline friend T operator-(T x1, VarConjRef<T> x2)
      { return x1-T(x2); }
      inline friend T operator-(RealType(T) x1, VarConjRef<T> x2)
      { return x1-T(x2); }
      inline friend T& operator-=(T& x1, VarConjRef<T> x2)
      { return x1 -= (x2.c==Conj ? std::conj(x2.val) : x2.val); }

      inline VarConjRef<T> operator*=(VarConjRef<T> x2) const
      { val *= c==x2.c ? x2.val : std::conj(x2.val); return *this; }
      inline VarConjRef<T> operator*=(T x2) const
      { val *= c==Conj ? std::conj(x2) : x2; return *this; }
      inline VarConjRef<T> operator*=(RealType(T) x2) const
      { val *= x2; return *this; }
      inline T operator*(VarConjRef<T> x2)
      { return T(*this)*T(x2); }
      inline friend T operator*(VarConjRef<T> x1, T x2)
      { return T(x1)*x2; }
      inline friend T operator*(VarConjRef<T> x1, RealType(T) x2)
      { return T(x1)*x2; }
      inline friend T operator*(T x1, VarConjRef<T> x2)
      { return x1*T(x2); }
      inline friend T operator*(RealType(T) x1, VarConjRef<T> x2)
      { return x1*T(x2); }
      inline friend T& operator*=(T& x1, VarConjRef<T> x2)
      { return x1 *= (x2.c==Conj ? std::conj(x2.val) : x2.val); }

      inline VarConjRef<T> operator/=(VarConjRef<T> x2) const
      { val /= c==x2.c ? x2.val : std::conj(x2.val); return *this; }
      inline VarConjRef<T> operator/=(T x2) const
      { val /= c==Conj ? std::conj(x2) : x2; return *this; }
      inline VarConjRef<T> operator/=(RealType(T) x2) const
      { val /= x2; return *this; }
      inline T operator/(VarConjRef<T> x2)
      { return T(*this)/T(x2); }
      inline friend T operator/(VarConjRef<T> x1, T x2)
      { return T(x1)/x2; }
      inline friend T operator/(VarConjRef<T> x1, RealType(T) x2)
      { return T(x1)/x2; }
      inline friend T operator/(T x1, VarConjRef<T> x2)
      { return x1/T(x2); }
      inline friend T operator/(RealType(T) x1, VarConjRef<T> x2)
      { return x1/T(x2); }
      inline friend T& operator/=(T& x1, VarConjRef<T> x2)
      { return x1 /= (x2.c==Conj ? std::conj(x2.val) : x2.val); }

      inline bool operator==(VarConjRef<T> x2) const
      { return val == (c==x2.c ? x2.val : std::conj(x2.val)); }
      inline bool operator==(T x2) const 
      { return val == (c==Conj ? std::conj(x2) : x2); }
      inline bool operator==(RealType(T) x2) const 
      { return std::real(val) == x2 && std::imag(val) == T(0); }
      inline friend bool operator==(T x1, VarConjRef<T> x2)
      { return x2==x1; }
      inline friend bool operator==(RealType(T) x1, VarConjRef<T> x2)
      { return x2==x1; }
      inline bool operator!=(VarConjRef<T> x2) const
      { return !(operator==(x2)); }
      inline bool operator!=(T x2) const 
      { return !(operator==(x2)); }
      inline bool operator!=(RealType(T) x2) const 
      { return !(operator==(x2)); }
      inline friend bool operator!=(T x1, VarConjRef<T> x2)
      { return !(x2==x1); }
      inline friend bool operator!=(RealType(T) x1, VarConjRef<T> x2)
      { return !(x2==x1); }

      inline void SwapWith(T& x2)
      { 
	if (&val == &x2) {
	  TMVAssert(c != Conj);
	} else {
	  T temp = x2; x2 = T(*this); *this = temp;
	}
      }
      inline void SwapWith(VarConjRef<T> x2)
      { 
	if (&val == &x2.val) {
	  TMVAssert(c == x2.c);
	} else {
	  if (c==x2.c) {
	    T temp = x2.val; x2.val = val; val = temp;
	  }
	  else {
	    T temp = x2; x2 = T(*this); *this = temp;
	  }
	}
      }

      inline friend std::ostream& operator<<(std::ostream& os, VarConjRef<T> x)
      { os << (x.c==Conj ? std::conj(x.val) : x.val); return os; }
      inline friend std::istream& operator>>(std::istream& is, VarConjRef<T> x)
      { is >> x.val; if(x.c==Conj) x.val = std::conj(x.val); return is; }

    private:

      T& val;
      ConjType c;
  };

  template <class T> inline T CONJ(VarConjRef<T> x) { return x.conj(); }
  template <class T> inline T conj(VarConjRef<T> x) { return x.conj(); }
  template <class T> inline RealType(T) NORM(VarConjRef<T> x) 
  { return norm(T(x)); }
  template <class T> inline RealType(T) norm(VarConjRef<T> x) 
  { return norm(T(x)); }
  template <class T> inline RealType(T) ABS(VarConjRef<T> x) 
  { return std::abs(T(x)); }
  template <class T> inline T SQR(VarConjRef<T> x) { return SQR(T(x)); }
  template <class T> inline T SQRT(VarConjRef<T> x) { return SQRT(T(x)); }
  template <class T> inline RealType(T) REAL(VarConjRef<T> x) 
  { return x.real(); }
  template <class T> inline RealType(T) IMAG(VarConjRef<T> x) 
  { return x.imag(); }
  template <class T> inline RealType(T) real(VarConjRef<T> x) 
  { return x.real(); }
  template <class T> inline RealType(T) imag(VarConjRef<T> x) 
  { return x.imag(); }
}

namespace std {
  template <class T> inline void swap(tmv::VarConjRef<std::complex<T> > x1,
      tmv::VarConjRef<std::complex<T> > x2)
  { return x1.SwapWith(x2); }
  template <class T> inline void swap(std::complex<T>& x1,
      tmv::VarConjRef<std::complex<T> > x2)
  { return x2.SwapWith(x1); }
  template <class T> inline void swap(tmv::VarConjRef<std::complex<T> > x1,
      std::complex<T>& x2)
  { return x1.SwapWith(x2); }
}

namespace tmv {

  template <class T> class VIter<std::complex<T> >
  { 
    public :

      VIter() : p(0), s(0), c(NonConj) DEFFIRSTLAST(0,0) {}
      VIter(std::complex<T>* inp, int instep, ConjType inc 
	  PARAMFIRSTLAST(std::complex<T>) ) : 
	p(inp), s(instep), c(inc) DEFFIRSTLAST(_first,_last) {}
      VIter(const VIter<std::complex<T> >& rhs) : 
	p(rhs.p), s(rhs.s), c(rhs.c) DEFFIRSTLAST(rhs.first,rhs.last) {}
      VIter<std::complex<T> >& operator=(const VIter<std::complex<T> >& rhs) 
      { 
	TMVAssert(s==rhs.s);
	TMVAssert(c==rhs.c);
	p=rhs.p; 
	SETFIRSTLAST(rhs.first,rhs.last);
	return *this; 
      }
      ~VIter() {}

      std::complex<T>* GetP() const { return p; }
      int step() const { return s; }
      ConjType GetC() const { return c; }

      inline bool operator==(const VIter<std::complex<T> >& rhs) const 
      { return p == rhs.p; }
      inline bool operator!=(const VIter<std::complex<T> >& rhs) const 
      { return p != rhs.p; }
      inline bool operator<(const VIter<std::complex<T> >& rhs) const 
      { return (s > 0 ? p < rhs.p : p > rhs.p); }

      inline VarConjRef<std::complex<T> > operator*() const
      { 
#ifdef TMVFLDEBUG
	if (!(p>=first && p<last)) {
	  std::cerr<<"p = "<<p<<std::endl;
	  std::cerr<<"first,last = "<<first<<"  "<<last<<std::endl;
	}
	TMVAssert(p>=first);
	TMVAssert(p<last);
#endif
	return VarConjRef<std::complex<T> >(*p,c);
      }

      inline VIter<std::complex<T> >& operator++() { p += s; return *this; }
      inline VIter<std::complex<T> >& operator--() { p -= s; return *this; }
      inline VIter<std::complex<T> > operator++(int) 
      { VIter<std::complex<T> > p2 = *this; p+=s; return p2; }
      inline VIter<std::complex<T> > operator--(int) 
      { VIter<std::complex<T> > p2 = *this; p-=s; return p2; }

      inline VIter<std::complex<T> >& operator+=(size_t n) 
      { if(s==1) ++p; else p += n*s; return *this; }
      inline VIter<std::complex<T> >& operator-=(size_t n) 
      { if(s==1) --p; else p -= n*s; return *this; }
      inline VIter<std::complex<T> > operator+(size_t n) const 
      { return VIter<std::complex<T> >(s==1?p+n:p+n*s,s,c FIRSTLAST ); }
      inline VIter<std::complex<T> > operator-(size_t n) const 
      { return VIter<std::complex<T> >(s==1?p-n:p-n*s,s,c FIRSTLAST ); }

      inline ptrdiff_t operator-(const VIter<std::complex<T> >& rhs) const 
      {
	TMVAssert(rhs.c==c);
	TMVAssert(rhs.s==s);
	return s==1 ? p-rhs.p : (p-rhs.p)/s; 
      }

      inline VarConjRef<std::complex<T> > operator[](size_t n) const
      {
#ifdef TMVFLDEBUG
	std::complex<T>* pn = s==1 ? p+n : p+n*s;
	if (!(pn>=first && pn<last)) {
	  std::cerr<<"pn = "<<pn<<std::endl;
	  std::cerr<<"first,last = "<<first<<"  "<<last<<std::endl;
	}
	TMVAssert(pn >= first);
	TMVAssert(pn < first);
	return VarConjRef<std::complex<T> >(*pn,c);
#else
	return VarConjRef<std::complex<T> >(*(s==1 ? p+n : p+n*s),c);
#endif
      }

      typedef std::random_access_iterator_tag iterator_category;
      typedef std::complex<T>                      value_type;
      typedef ptrdiff_t                       difference_type;
      typedef std::complex<T>*                     pointer;
      typedef VarConjRef<std::complex<T> >         reference;

    private :

      std::complex<T>* p;
      const int s;
      ConjType c;

#ifdef TMVFLDEBUG
    public :
      const std::complex<T>* first;
      const std::complex<T>* last;
#endif

  };

  template <class T> class CVIter<std::complex<T> >
  { 
    public :

      CVIter() : p(0), s(0), c(NonConj) {}
      CVIter(const std::complex<T>* inp, int instep, ConjType inc) :
	p(inp), s(instep), c(inc) {}
      CVIter(const CVIter<std::complex<T> >& rhs) : 
	p(rhs.p), s(rhs.s), c(rhs.c) {}
      CVIter(const VIter<std::complex<T> >& rhs) : 
	p(rhs.GetP()), s(rhs.step()), c(rhs.GetC()) {}
      CVIter<std::complex<T> >& operator=(const CVIter<std::complex<T> >& rhs) 
      { 
	TMVAssert(s==rhs.s);
	TMVAssert(c==rhs.c);
	p=rhs.p; 
	return *this; 
      }
      CVIter<std::complex<T> >& operator=(const VIter<std::complex<T> >& rhs) 
      { 
	TMVAssert(s==rhs.step());
	TMVAssert(c==rhs.GetC());
	p=rhs.GetP(); 
	return *this; 
      }
      ~CVIter() {}

      const std::complex<T>* GetP() const { return p; }
      int step() const { return s; }
      ConjType GetC() const { return c; }

      inline bool operator==(const CVIter<std::complex<T> >& rhs) const 
      { return p == rhs.p; }
      inline bool operator!=(const CVIter<std::complex<T> >& rhs) const 
      { return p != rhs.p; }
      inline bool operator<(const CVIter<std::complex<T> >& rhs) const 
      { return (s > 0 ? p < rhs.p : p > rhs.p); }

      inline std::complex<T> operator*() const 
      { return c==Conj ? std::conj(*p) : *p; }

      inline CVIter<std::complex<T> >& operator++() { p += s; return *this; }
      inline CVIter<std::complex<T> >& operator--() { p -= s; return *this; }
      inline CVIter<std::complex<T> > operator++(int) 
      { CVIter<std::complex<T> > p2 = *this; p+=s; return p2; }
      inline CVIter<std::complex<T> > operator--(int) 
      { CVIter<std::complex<T> > p2 = *this; p-=s; return p2; }

      inline CVIter<std::complex<T> >& operator+=(size_t n) 
      { if(s==1) ++p; else p += n*s; return *this; }
      inline CVIter<std::complex<T> >& operator-=(size_t n) 
      { if(s==1) --p; else p -= n*s; return *this; }
      inline CVIter<std::complex<T> > operator+(size_t n) const 
      { return CVIter<std::complex<T> >(s==1?p+n:p+n*s,s,c); }
      inline CVIter<std::complex<T> > operator-(size_t n) const 
      { return CVIter<std::complex<T> >(s==1?p-n:p-n*s,s,c); }

      inline ptrdiff_t operator-(const CVIter<std::complex<T> >& rhs) const 
      {
	TMVAssert(rhs.c==c);
	TMVAssert(rhs.s==s);
	return s==1 ? p-rhs.p : (p-rhs.p)/s; 
      }

      inline std::complex<T> operator[](size_t n) const 
      { return c==Conj ? std::conj(*(p+(s==1?n:n*s))) : *(p+(s==1?n:n*s)); }

      typedef std::random_access_iterator_tag iterator_category;
      typedef std::complex<T>                      value_type;
      typedef ptrdiff_t                       difference_type;
      typedef const std::complex<T>*               pointer;
      typedef const VarConjRef<std::complex<T> >   reference;

    private :

      const std::complex<T>* p;
      const int s;
      ConjType c;
  };

  template <class T> inline std::string Type(ConjRef<T>)
  { return std::string("ConjRef<") + Type(T()) + ">"; }

  template <class T> inline std::string Type(VarConjRef<T>)
  { return std::string("VarConjRef<") + Type(T()) + ">"; }

  template <class T, StepType S, ConjType C> 
    inline std::string Type(VIt<T,S,C>)
  { 
    return std::string("VIt<") + Type(T()) + "," +
      Text(S) + "," + Text(C) + ">"; 
  }

  template <class T, StepType S, ConjType C> 
    inline std::string Type(CVIt<T,S,C>)
  {
    return std::string("CVIt<") + Type(T()) + "," +
    Text(S) + "," + Text(C) + ">"; 
  }

  template <class T> inline std::string Type(VIter<T> it)
  { 
    return std::string("VIter<") + Type(T()) + "," +
      it.step()==1 ? Text(Unit) : Text(Step) + "," +
      Text(it.GetC()) + ">"; 
  }

  template <class T> inline std::string Type(CVIter<T> it)
  { 
    return std::string("CVIter<") + Type(T()) + "," +
      it.step()==1 ? Text(Unit) : Text(Step) + "," +
				  Text(it.GetC()) + ">"; 
  }

  template <class T> inline T& REF(T* vi, ConjType )
  { return *vi; }
  template <class T> inline VarConjRef<std::complex<T> > REF(
      std::complex<T>* vi, ConjType ct)
  { return VarConjRef<std::complex<T> >(*vi,ct); }

} // namespace tmv

#endif