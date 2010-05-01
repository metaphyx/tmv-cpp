//-----------------------------------------------------------------------------
//
// This file defines the TMV Vector class.
//
// The Vector class and all associated functions are contained
// in the namespace tmv.  Alse, the Vector class is a template, 
// so for a Vector of doubles, one would write 
// tmv::Vector<double>.  
//
// Constructors:
//
//    Vector<T>()  
//        Makes a Vector of size 0
//
//    explicit Vector<T>(size_t n)
//        Makes a Vector of size n with _uninitialized_ values
//
//    Vector<T>(size_t n, T x)
//        Makes a Vector of size n with all values = x
//
//    Vector<T>(const valarray<T>& vv)
//    Vector<T>(size_t n, const T* vv)
//    Vector<T>(const vector<T>& vv)
//        Makes a vector which copies the elements of vv
//        For the second one, n specifies the length of the vector
//
// Special Constructors
//
//    BasisVector(size_t n, size_t i)
//        Makes a Vector whose elements are all 0, except v(i) = 1
//
//    VectorViewOf(T* vv, size_t n)
//    VectorViewOf(const T* vv, size_t n)
//        Makes a Vector View (see below) which refers to the exact
//        elements of vv, not copying them to new storage.
//        The first one returns a VectorView, the second a ConstVectorView.
//
// Access Functions
//
//    size_t size() const
//        Returns the size of the Vector
//
//    T& operator[](size_t i)
//    T& operator()(size_t i)
//    T operator[](size_t i) const
//    T operator()(size_t i) const
//        Return the ith element of the Vector
//
//    Vector<T>::iterator begin()
//    Vector<T>::iterator end()
//    Vector<T>::reverse_iterator rbegin()
//    Vector<T>::reverse_iterator rend()
//    Vector<T>::const_iterator begin() const
//    Vector<T>::const_iterator end() const
//    Vector<T>::const_reverse_iterator begin() const
//    Vector<T>::const_reverse_iterator end() const
//        Return iterators that work in the usual way to traverse the Vector
//
// Modifying Functions
//
//    Vector& Zero()
//        Sets all elements to 0
//
//    Vector& SetAllTo(T x)
//        Sets all elements to x
//        We don't call this v = x, since it doesn't really make sense to
//        think of v as being 'equal' to a scalar.
//        Hence the slightly verbose function name SetAllTo.
//
//    Vector& AddToAll(T x)
//        Add x to each element of v
//
//    Vector& ConjugateSelf()
//        Sets all elements to its conjugate
//
//    Vector& MakeBasis(size_t i)
//        Set all elements to 0, except v(i) = 1
//
//    Vector& Swap(size_t i1, size_t i2)
//        Swap elements v(i1) and v(i2)
//
//    Swap(v1,v2)
//        Swap vectors v1 and v2
//
//    Vector& ReverseSelf()
//        Reverse the order of the elements of v
//
// VectorViews:
//
//    A VectorView<T> object refers to the elements of a regular Vector
//    or Matrix, so that altering the elements in the view alters the
//    corresponding elements in the original object.  A View can have non-unit
//    steps between elements and can also be a conjugation of the original
//    elements, so that v[3] = z would actually set the original element
//    to z*. 
//
//    Also, since we want to be able to pass these Views around, we have to 
//    keep track of whether we are allowed to alter the original values or
//    just look at them.  Thus, there are two VectorView objects: 
//    ConstVectorView and VectorView.  The first is only allowed to view,
//    not modify, the original elements.  The second is allowed to modify them.
//    This is akin to the const_iterator and iterator types for the
//    standard template library.
//
//    Below, VectorView is written for both of these, and I don't bother to
//    indicate the template modifiers.  In general, a ConstVectorView is 
//    returned from either a ConstVectorView object, or a const Vector object.
//    A (mutable) VectorView is returned from either a VectorView object or
//    a (non-const) Vector object.
//
//    VectorView SubVector(int i1, int i2, int istep=1)
//        This member function will return a subvector view which refers
//        to the same physical elements as the original.
//        i1 is the first element in the subvector.
//        i2 is the usual "one past the end" of the subvector.
//        istep is an optional step size.
//        Thus, if you have a vector v of length 10, and you want to
//        set all the even elements to 0, you could write:
//        v.SubVector(0,10,2).Zero();
//        And then to output the first 4 elements of v, you could write:
//        std::cout << v.SubVector(0,4);
//
//    VectorView Reverse()
//        Returns a subvector whose elements are the same as v but 
//        in the reverse order
//
//    VectorView Conjugate()
//        Returns the conjugate of a Vector (as a view, so it still points
//        to the same physical elements, but modifying this will set the 
//        actual elements to the conjugate of what you set.
//
//    VectorView View()
//        Returns a view of a Vector. 
//
//    VectorView Real()
//    VectorView Imag()
//        Returns a view of the real/imag elements of a complex Vector.
//
// Functions of Vectors:
//        (These are all both member functions and functions of a Vector,
//         so Norm(v) and v.Norm() for example are equivalent)
//
//    Norm(v) or Norm2(v) 
//        Returns the 2-norm of a Vector
//        = sqrt( sum_i |v_i|^2 )
//
//    NormSq(v)
//        Returns the square of Norm()
//
//    Norm1(v) 
//        Returns the 1-norm of a Vector
//        = sum_i |v_i|
//
//    NormInf(v) 
//        Returns the infinity-norm of a Vector
//        = max_i |v_i|
//
//    SumElements(v)
//        Returns the sum of all elements in the Vector
//
//    SumAbsElements(v)
//        Returns the sum of absolute values of  elements in the Vector
//
//    MaxElement(v,size_t* imax)
//        Returns the maximum value of any element in the Vector
//        On return, *imax_value holds the index of this element.
//        The parameter max_value can be omitted if it is not desired.
//        As "max" doesn't make sense for complex values, for these
//        we use just the real components.
//
//    MinElement(v,size_t* imin)
//        Returns the minimum value of any element in the Vector
//        On return, *imin_value holds the index of this element.
//        Again, the parameter min_value can be omitted.
//
//    MaxAbsElement(v,size_t* imax)
//        The same as MaxElement, except absolute values are used
//
//    MinAbsElement(v,size_t* imin)
//        The same as MinElement, except absolute values are used
//
//
// Operators:
//        Here we use v for a Vector and x for a Scalar.
//
//        You can also mix real and complex vectors of the same
//        underlying type.  eg. Vector<double> and Vector<complex<double> >
//
//    -v
//
//    v = v
//
//    v += v
//    v + v
//
//    v -= v
//    v - v
//
//    v *= x
//    x * v
//    v * x
//    v * v   (inner product)
//
//    v /= x
//    v / x
//
//    v == v
//    v != v
//
//       These all behave in the logical way for dealing with vectors.
//
//
// I/O: 
//
//    os << v 
//        Writes v to ostream os in the following format:
//          n ( v[0] v[1] v[2] ... v[n] )
//
//    v.Write(ostream& os, Real(T) minnonzero)
//        Write v to os as above, but if v[i] < minnonzero, write 0 instead
//
//    is >> v
//        Reads v from istream is in the same format
//        Note: v must already be the correct size
//
//    is >> vptr
//        If you do not know the size of the Vector to be read in,
//        this version will create a new Vector of the correct size.
//        So you should subsequently delete the Vector.
//
//


#ifndef TMV_Vector_H
#define TMV_Vector_H

#include "TMV_VIt.h"

namespace tmv {

  template <class T> class GenVector;
  template <class T> class ConstVectorView;
  template <class T> class VectorView;
  template <class T> class Vector;
  template <class T> class VectorComposite;

  template <class T1, class T2> void Copy(
      const GenVector<T1>& v1, const VectorView<T2>& v2);

#define RefType(T) typename VIter<T>::reference
  template <class T> inline T& REF(T* vi, ConjItType ct)
  { return *vi; }
  template <class T> inline VarConjRef<complex<T> > REF(
      complex<T>* vi, ConjItType ct)
  { return VarConjRef<complex<T> >(*vi,ct); }


  template <class T> class GenVector
  {
    public:

      //
      // Constructor
      //

      GenVector(ConjItType ct) : itsct(ct) {}
      GenVector(const GenVector<T>& rhs) : itsct(rhs.itsct) {}
      virtual ~GenVector() {}


      //
      // Access Functions
      //

      virtual size_t size() const =0;

      inline T operator[](size_t i) const { return cref(i); }
      inline T operator()(size_t i) const { return cref(i); }

      typedef CVIter<T> const_iterator;
      typedef CVIter<T> const_reverse_iterator;

      inline CVIter<T> begin() const 
      { return CVIter<T>(cptr(),step(),itsct); }
      inline CVIter<T> end() const { return begin()+size(); }
      inline CVIter<T> rbegin() const 
      { return const_reverse_iterator(cptr()+step()*(size()-1),-step(),itsct); }
      inline CVIter<T> rend() const 
      { return rbegin()+size(); }

      template <class T2> inline bool SameStorageAs(
	  const GenVector<T2>& v2) const
      { return false; }

      inline bool SameStorageAs(const GenVector<T>& v2) const
      { return (cptr()==v2.cptr()); }

      template <class T2> inline bool SameAs(const GenVector<T2>& v2) const
      { return false; }

      inline bool SameAs(const GenVector<T>& v2) const
      {
	return (this == &v2 || (cptr()==v2.cptr() && 
	    size()==v2.size() && step()==v2.step() && itsct==v2.itsct));
      }

      //
      // SubVector
      //

      bool OKSubVector(int i1, int i2, int istep=1) const;

      inline ConstVectorView<T> SubVector(int i1, int i2) const
      {
	TMVAssert(OKSubVector(i1,i2));
	return ConstVectorView<T>(cptr()+i1*step(),i2-i1,step(),itsct);
      }

      inline ConstVectorView<T> SubVector(int i1, int i2, int istep) const
      {
	TMVAssert(OKSubVector(i1,i2,istep));
	return ConstVectorView<T>(cptr()+i1*step(),
	    (i2-i1)/istep,istep*step(),itsct);
      }

      inline ConstVectorView<T> Reverse() const
      { 
	return ConstVectorView<T>(cptr()+(size()-1)*step(),
	    size(),-step(),itsct); 
      }

      inline ConstVectorView<T> View() const
      { return ConstVectorView<T>(cptr(),size(),step(),itsct); }

      inline ConstVectorView<T> Conjugate() const
      { return ConstVectorView<T>(cptr(),size(),step(),ConjOf(T,itsct)); }

      inline ConstVectorView<RealType(T)> Real() const
      { 
	return ConstVectorView<RealType(T)>(
	    reinterpret_cast<const RealType(T)*>(cptr()),
	    size(), IsReal(T()) ? step() : 2*step(), NonConj);
      }

      inline ConstVectorView<RealType(T)> Imag() const
      { 
	TMVAssert(IsComplex(T()));
	TMVAssert(itsct == NonConj);
	return ConstVectorView<RealType(T)>(
	    reinterpret_cast<const RealType(T)*>(cptr())+1,
	    size(), 2*step(), NonConj);
      }

      //
      // Functions of Vector
      //

      inline RealType(T) Norm() const // = Norm2
      { return Norm2(); }

      RealType(T) NormSq() const; // Norm()^2

      inline RealType(T) Norm1() const // sum_i |v_i|
      { return SumAbsElements(); }

      RealType(T) Norm2() const; // sqrt( sum_i |v_i|^2 )

      inline RealType(T) NormInf() const // max_i |v_i|
      { return MaxAbsElement(); }

      T SumElements() const;

      RealType(T) SumAbsElements() const;

      T MinElement(size_t* iminout=0) const;

      T MaxElement(size_t* imaxout) const;

      RealType(T) MinAbsElement(size_t* iminout=0) const;

      RealType(T) MaxAbsElement(size_t* imaxout=0) const;

      //
      // I/O
      //

      void Write(ostream& fout) const;

      virtual const T* cptr() const =0;
      virtual int step() const =0;
      virtual inline ConjItType ct() const { return itsct; }
      inline bool isconj() const 
      { 
	TMVAssert(IsComplex(T()) || itsct==NonConj);
	return (IsComplex(T()) && itsct==Conj);
      }

    protected:

      const ConjItType itsct;

      T cref(size_t i) const;

    private:

      void operator=(const GenVector<T>&) { TMVAssert(false); }

  }; // GenVector

  template <class T> class ConstVectorView : 
    public GenVector<T>
  {
    public:

      ConstVectorView(const ConstVectorView<T>& rhs) : 
	GenVector<T>(rhs),
	itsv(rhs.itsv), itssize(rhs.itssize), itsstep(rhs.itsstep) {}
      ConstVectorView(const GenVector<T>& rhs) : 
	GenVector<T>(rhs),
	itsv(rhs.cptr()), itssize(rhs.size()), itsstep(rhs.step()) {}
      ConstVectorView(const T* inv, size_t insize, int instep, 
	  ConjItType inct) : 
	GenVector<T>(inct), itsv(inv), itssize(insize), itsstep(instep) {}
      ~ConstVectorView() {}

      inline size_t size() const { return itssize; }
      inline const T* cptr() const { return itsv; }
      inline int step() const { return itsstep; }

    private:

      const T*const itsv;
      const size_t itssize;
      const size_t itsstep;

      inline void operator=(const ConstVectorView<T>&) { TMVAssert(false); }

  }; // ConstVectorView

  template <class T> class VectorView : public GenVector<T>
  {
    public:

      //
      // Constructors 
      //

      VectorView(const VectorView<T>& rhs) : 
	GenVector<T>(rhs),
	itsv(rhs.itsv), itssize(rhs.itssize), itsstep(rhs.itsstep)
	DEFFIRSTLAST(rhs.first,rhs.last) {}

#ifdef TMVFLDEBUG
      VectorView(T* inv, size_t insize, int instep, ConjItType inct,
	  const T* infirst, const T* inlast) :
	GenVector<T>(inct), itsv(inv), itssize(insize), itsstep(instep),
	first(infirst), last(inlast) {}
#else
      VectorView(T* inv, size_t insize, int instep, ConjItType inct) :
	GenVector<T>(inct), itsv(inv), itssize(insize), itsstep(instep) {}
#endif

      ~VectorView() {}


      //
      // Op =
      //

      inline const VectorView<T>& operator=(const VectorView<T>& v2) const
      { 
	TMVAssert(size() == v2.size());
	if (!SameAs(v2)) Copy(v2,*this); 
	return *this; 
      }

      inline const VectorView<T>& operator=(const GenVector<T>& v2) const
      { 
	TMVAssert(size() == v2.size());
	if (!SameAs(v2)) Copy(v2,*this);
	return *this; 
      }

      template <class T2> inline const VectorView<T>& operator=(
	  const GenVector<T2>& v2) const
      { 
	TMVAssert(size() == v2.size());
	Copy(v2,*this);
	return *this; 
      }

      inline const VectorView<T>& operator=(
	  const VectorComposite<T>& vcomp) const
      { vcomp.AssignTo(*this); return *this; }


      //
      // Access Functions
      //

      inline size_t size() const { return itssize; }

      inline RefType(T) operator[](size_t i) const 
      { return ref(i); }
      inline RefType(T) operator()(size_t i) const 
      { return ref(i); }

      typedef VIter<T> iterator;
      typedef CVIter<T> const_iterator;
      typedef VIter<T> reverse_iterator;
      typedef CVIter<T> const_reverse_iterator;

      inline VIter<T> begin() const 
      { return VIter<T>(ptr(),step(),ct() FIRSTLAST ); }
      inline VIter<T> end() const { return begin() + size(); }
      inline VIter<T> rbegin() const 
      { return VIter<T>(ptr()+step()*(size()-1),-step(),ct() FIRSTLAST ); }
      inline VIter<T> rend() const 
      { return rbegin()+size(); }

      //
      // Modifying Functions
      //

      const VectorView<T>& Zero() const { return SetAllTo(0); }
      const VectorView<T>& SetAllTo(T x) const;
      const VectorView<T>& AddToAll(T x) const;
      const VectorView<T>& ConjugateSelf() const;
      const VectorView<T>& MakeBasis(size_t i) const
      { TMVAssert(i < size()); Zero(); ref(i) = T(1); return *this; }
      const VectorView<T>& Swap(size_t i1, size_t i2) const;
      const VectorView<T>& ReverseSelf() const;

      //
      // SubVector
      //

      inline VectorView<T> SubVector(int i1, int i2) const
      {
	TMVAssert(this->OKSubVector(i1,i2));
	return VectorView<T>(ptr()+i1*step(),(i2-i1),step(),ct() FIRSTLAST );
      }

      inline VectorView<T> SubVector(int i1, int i2, int istep) const
      {
	TMVAssert(this->OKSubVector(i1,i2,istep));
	return VectorView<T>(
	    ptr()+i1*step(),(i2-i1)/istep,istep*step(),ct() FIRSTLAST );
      }

      inline VectorView<T> Reverse() const
      { 
	return VectorView<T>(
	    ptr()+(size()-1)*step(),size(),-step(),ct() FIRSTLAST ); 
      }

      inline VectorView<T> View() const
      { return *this; }

      inline VectorView<T> Conjugate() const
      {
	return VectorView<T>(ptr(),size(),step(),ConjOf(T,ct()) FIRSTLAST ); 
      }

      inline VectorView<RealType(T)> Real() const
      { 
	return VectorView<RealType(T)>(
	    reinterpret_cast<RealType(T)*>(ptr()),
	    size(), IsReal(T()) ? step() : 2*step(), NonConj
#ifdef TMVFLDEBUG
	    ,reinterpret_cast<const RealType(T)*>(first)
	    ,reinterpret_cast<const RealType(T)*>(last)
#endif
	    );
      }

      inline VectorView<RealType(T)> Imag() const
      { 
	TMVAssert(IsComplex(T()));
	TMVAssert(ct() == NonConj);
	return VectorView<RealType(T)>(
	    reinterpret_cast<RealType(T)*>(ptr())+1,size(),2*step(), NonConj
#ifdef TMVFLDEBUG
	    ,reinterpret_cast<const RealType(T)*>(first)+1
	    ,reinterpret_cast<const RealType(T)*>(last)+1
#endif
	    );
      }

      //
      // I/O
      //

      void Read(istream& fin) const;

      // 
      // Iterator Typedefs
      //

      inline const T* cptr() const { return itsv; }
      inline T* ptr() const { return itsv; }
      inline int step() const { return itsstep; }
      using GenVector<T>::ct;
      using GenVector<T>::isconj;

    private:

      T*const itsv;
      const size_t itssize;
      const int itsstep;

#ifdef TMVFLDEBUG
    public:
      const T*const first;
      const T*const last;
    private:
#endif

      RefType(T) ref(size_t i) const;

  }; // VectorView

  template <class T> class Vector : 
    public GenVector<T>
  {
    public:

      //
      // Constructors
      //

#define NEW_SIZE(n) \
      GenVector<T>(NonConj), itsv(new T[(n)]), itssize(n) \
      DEFFIRSTLAST(itsv,itsv+n)

      explicit Vector(size_t n) : NEW_SIZE(n) 
      {
#ifdef TMVDEBUG
	SetAllTo(T(888));
#endif
      }

      Vector(size_t n,T val) : NEW_SIZE(n)
      { std::fill(itsv,itsv+n,val); }

      explicit Vector(const valarray<T>& vv) : NEW_SIZE(vv.size())
      { 
	VIt<T,Unit,NonConj> it=begin(); 
	for(size_t i=0;i<size();++i,++it) *it = vv[i]; 
      }

      Vector(size_t n, const T* vv) : NEW_SIZE(n)
      { memmove(itsv,vv,n*sizeof(T)); }

      explicit Vector(const vector<T>& vv) : NEW_SIZE(vv.size())
      { std::copy(vv.begin(),vv.end(),itsv); }

      Vector(const Vector<T>& rhs) : NEW_SIZE(rhs.size())
      { 
#ifdef TMVFLDEBUG
	TMVAssert(rhs.cptr() >= rhs.first);
	TMVAssert(rhs.cptr()+itssize <= rhs.last);
#endif
	memmove(itsv,rhs.cptr(),itssize*sizeof(T)); 
      }

      Vector(const GenVector<T>& rhs) : NEW_SIZE(rhs.size())
      { 
	if (rhs.step() == 1 && !rhs.isconj()) 
	  memmove(itsv,rhs.cptr(),itssize*sizeof(T)); 
	else Copy(rhs,View());
      }

      template <class T2> Vector(const GenVector<T2>& rhs) : 
	NEW_SIZE(rhs.size())
      { Copy(rhs,View()); }

      Vector(const VectorComposite<T>& vcomp) : NEW_SIZE(vcomp.size())
      { vcomp.AssignTo(View()); }


#undef NEW_SIZE

      ~Vector() { if (itsv) delete[] itsv; }


      //
      // Op =
      //

      inline Vector<T>& operator=(Vector<T>& v2)
      { 
	TMVAssert(v2.size() == size());
	if (&v2 != this) memmove(itsv,v2.cptr(),itssize*sizeof(T)); 
	return *this; 
      }

      inline Vector<T>& operator=(const GenVector<T>& v2) 
      { View() = v2; return *this; }

      template <class T2> inline Vector<T>& operator=(
	  const GenVector<T2>& v2) 
      { View() = v2; return *this; }

      inline Vector<T>& operator=(const VectorComposite<T>& vcomp)
      { vcomp.AssignTo(View()); return *this; }


      //
      // Access Functions
      //

      inline size_t size() const { return itssize; }

      typedef VIt<T,Unit,NonConj> iterator;
      typedef CVIt<T,Unit,NonConj> const_iterator;
      typedef VIt<T,Step,NonConj> reverse_iterator;
      typedef CVIt<T,Step,NonConj> const_reverse_iterator;

      inline CVIt<T,Unit,NonConj> begin() const 
      { return CVIt<T,Unit,NonConj>(cptr(),1); }
      inline CVIt<T,Unit,NonConj> end() const { return begin()+size(); }
      
      inline CVIt<T,Step,NonConj> rbegin() const 
      { return const_reverse_iterator(cptr()+(size()-1),-1); }
      inline CVIt<T,Step,NonConj> rend() const { return rbegin()+size(); }

      inline T operator[](size_t i) const { return cref(i); }
      inline T operator()(size_t i) const { return cref(i); }

      inline VIt<T,Unit,NonConj> begin() 
      { return iterator(ptr(),1 FIRSTLAST ); }
      inline VIt<T,Unit,NonConj> end() { return begin() + size(); }

      inline VIt<T,Step,NonConj> rbegin() 
      { return reverse_iterator(ptr()+size()-1,-1 FIRSTLAST ); }
      inline VIt<T,Step,NonConj> rend() { return rbegin()+size(); }

      inline T& operator[](size_t i) { return ref(i); }
      inline T& operator()(size_t i) { return ref(i); }

      //
      // Modifying Functions
      //

      inline Vector<T>& Zero() { return SetAllTo(0); }

      inline Vector<T>& SetAllTo(T x)
      { std::fill(itsv,itsv+itssize,x); return *this; }

      inline Vector<T>& AddToAll(T x)
      {
	const VIt<T,Unit,NonConj> _end = end();
	for(VIt<T,Unit,NonConj> it=begin();it!=_end;++it) *it += x;
	return *this;
      }

      inline Vector<T>& ConjugateSelf()
      { View().ConjugateSelf(); return *this; }

      inline Vector<T>& MakeBasis(size_t i)
      { TMVAssert(i < size()); Zero(); ref(i) = T(1); return *this; }

      inline Vector<T>& Swap(size_t i1, size_t i2)
      {
	TMVAssert(i1 < size() && i2 < size());
	if (i1 != i2) swap(ref(i1),ref(i2));
	return *this;
      }

      inline Vector<T>& ReverseSelf()
      { View().ReverseSelf(); return *this; }

      //
      // SubVector
      //

      inline ConstVectorView<T> SubVector(int i1, int i2) const
      {
	TMVAssert(this->OKSubVector(i1,i2,1));
	return ConstVectorView<T>(cptr()+i1,i2-i1,1,NonConj);
      }

      inline VectorView<T> SubVector(int i1, int i2)
      {
	TMVAssert(this->OKSubVector(i1,i2,1));
	return VectorView<T>(ptr()+i1,i2-i1,1,NonConj FIRSTLAST );
      }

      inline ConstVectorView<T> SubVector(int i1, int i2, int istep) const
      {
	TMVAssert(this->OKSubVector(i1,i2,istep));
	return ConstVectorView<T>(
	    cptr()+i1,(i2-i1)/istep,istep,NonConj);
      }

      inline VectorView<T> SubVector(int i1, int i2, int istep)
      {
	TMVAssert(this->OKSubVector(i1,i2,istep));
	return VectorView<T>(
	    ptr()+i1,(i2-i1)/istep,istep,NonConj FIRSTLAST );
      }

      inline ConstVectorView<T> Reverse() const
      { return ConstVectorView<T>(cptr()+size()-1,size(),-1,NonConj); }

      inline VectorView<T> Reverse()
      { return VectorView<T>(ptr()+size()-1,size(),-1,NonConj FIRSTLAST ); }

      inline ConstVectorView<T> View() const
      { return ConstVectorView<T>(cptr(),size(),1,NonConj); }

      inline VectorView<T> View()
      { return VectorView<T>(ptr(),size(),1,NonConj FIRSTLAST ); }

      inline ConstVectorView<T> Conjugate() const
      { return ConstVectorView<T>(cptr(),size(),1,ConjOf(T,NonConj)); }

      inline VectorView<T> Conjugate()
      { return VectorView<T>(ptr(),size(),1,ConjOf(T,NonConj) FIRSTLAST ); }

      inline const T* cptr() const { return itsv; }
      inline T* ptr() { return itsv; }
      inline int step() const { return 1; }
      inline ConjItType ct() const { return NonConj; }
      inline bool isconj() const { return false; }

    private:

      T*const itsv;
      const size_t itssize;

#ifdef TMVFLDEBUG
    public:
      const T*const first;
      const T*const last;
    private:
#endif

      inline T cref(size_t i) const
      { 
	TMVAssert(i < size()); 
	return *(itsv+i);
      }

      inline T& ref(size_t i)
      { 
	TMVAssert(i < size()); 
	T*const vi = itsv+i;
#ifdef TMVFLDEBUG
	TMVAssert(vi >= first);
	TMVAssert(vi < last);
#endif
	return *vi; 
      }

  }; // Vector

  //
  // Special Constructors
  //

  template <class T> inline Vector<T> BasisVector(size_t n, size_t i)
  { TMVAssert(i<n); Vector<T> temp(n,T(0)); temp(i) = T(1); return temp; }

  template <class T> inline VectorView<T> VectorViewOf(T* v, size_t size)
  { return VectorView<T>(v,size,1,NonConj FIRSTLAST1(v,v+size)); }

  template <class T> inline ConstVectorView<T> VectorViewOf(const T* v, 
      size_t size)
  { return ConstVectorView<T>(v,size,1,NonConj); }

  //
  // Copy Vectors
  //

  template <class T1, StepItType S1, ConjItType C1, class T2, StepItType S2> 
    inline void NonBlasCopy(CVIt<T1,S1,C1> v1it, VIt<T2,S2,NonConj> v2it,
	size_t size)
    {
      const CVIt<T1,S1,C1> _end = v1it + size;
      for(;v1it!=_end;++v1it,++v2it) *v2it = *v1it;
    }

  template <class T> inline void NonBlasCopy(CVIt<T,Unit,NonConj> v1it, 
      VIt<T,Unit,NonConj> v2it, size_t size)
  { memmove(v2it.GetP(),v1it.GetP(),size*sizeof(T)); }

#ifdef BLAS
  template <class T1, StepItType S1, ConjItType C1, class T2, StepItType S2> 
    inline void BlasCopy(CVIt<T1,S1,C1> v1it, VIt<T2,S2,NonConj>& v2it,
	size_t size)
    { NonBlasCopy(v1it,v2it,size); }
  template <StepItType S1, StepItType S2> inline void BlasCopy(
      CVIt<double,S1,NonConj> v1it, VIt<double,S2,NonConj>& v2it, size_t size)
  { cblas_dcopy(size,v1it.GetP(),v1it.step(),v2it.GetP(),v2it.step()); }
  template <StepItType S1, StepItType S2> inline void BlasCopy(
      CVIt<complex<double>,S1,NonConj>& v1it,
      VIt<complex<double>,S2,NonConj>& v2it, size_t size) 
  { cblas_zcopy(size,v1it.GetP(),v1it.step(),v2it.GetP(),v2it.step()); }

#ifndef NOFLOAT
  template <StepItType S1, StepItType S2> inline void BlasCopy(
      CVIt<float,S1,NonConj> v1it, VIt<float,S2,NonConj>& v2it, size_t size)
  { cblas_scopy(size,v1it.GetP(),v1it.step(),v2it.GetP(),v2it.step()); }
  template <StepItType S1, StepItType S2> inline void BlasCopy(
      CVIt<complex<float>,S1,NonConj>& v1it,
      VIt<complex<float>,S2,NonConj>& v2it, size_t size) 
  { cblas_ccopy(size,v1it.GetP(),v1it.step(),v2it.GetP(),v2it.step()); }
#endif
#endif
  template <class T1, StepItType S1, ConjItType C1, class T2, StepItType S2> 
    inline void DoCopy(CVIt<T1,S1,C1> v1it, VIt<T2,S2,NonConj> v2it,
	size_t size)
    { 
#ifdef BLAS
      if (v1it.step() > 0 && v2it.step() > 0) BlasCopy(v1it,v2it,size);
      else 
#endif
	NonBlasCopy(v1it,v2it,size); 
    }

  template <class T1, class T2> inline void CallCopy(
      const GenVector<T1>& v1, const VectorView<T2>& v2)
  {
    TMVAssert(v1.size()==v2.size());
    TMVAssert(v2.ct()==NonConj);
    if (v1.isconj())
      if (v1.step() == 1)
	if (v2.step() == 1) 
	  DoCopy(CVIt<T1,Unit,Conj>(v1.begin()),
	      VIt<T2,Unit,NonConj>(v2.begin()),v1.size()); 
	else 
	  DoCopy(CVIt<T1,Unit,Conj>(v1.begin()),
	      VIt<T2,Step,NonConj>(v2.begin()),v1.size()); 
      else
	if (v2.step() == 1) 
	  DoCopy(CVIt<T1,Step,Conj>(v1.begin()),
	      VIt<T2,Unit,NonConj>(v2.begin()),v1.size()); 
	else 
	  DoCopy(CVIt<T1,Step,Conj>(v1.begin()),
	      VIt<T2,Step,NonConj>(v2.begin()),v1.size()); 
    else
      if (v1.step() == 1)
	if (v2.step() == 1) 
	  DoCopy(CVIt<T1,Unit,NonConj>(v1.begin()),
	      VIt<T2,Unit,NonConj>(v2.begin()),v1.size()); 
	else
	  DoCopy(CVIt<T1,Unit,NonConj>(v1.begin()),
	      VIt<T2,Step,NonConj>(v2.begin()),v1.size()); 
      else
	if (v2.step() == 1)
	  DoCopy(CVIt<T1,Step,NonConj>(v1.begin()),
	      VIt<T2,Unit,NonConj>(v2.begin()),v1.size()); 
	else 
	  DoCopy(CVIt<T1,Step,NonConj>(v1.begin()),
	      VIt<T2,Step,NonConj>(v2.begin()),v1.size()); 
  }
  template <class T> inline void CallCopy(
      const GenVector<T>& v1, const VectorView<complex<T> >& v2)
  { 
    v2.Imag().Zero();
    CallCopy(v1,v2.Real());
  }
  template <class T> inline void CallCopy(
      const GenVector<complex<T> >& v1, const VectorView<T>& v2)
  { TMVAssert(false); }

  template <class T1, class T2> inline void Copy(
      const GenVector<T1>& v1, const VectorView<T2>& v2)
  { 
    TMVAssert(v1.size() == v2.size());
    if (v1.size() > 0)
      if (v2.isconj())
	if (v1.step()<0 && v2.step()<0) 
	  CallCopy(v1.Conjugate().Reverse(),v2.Conjugate().Reverse());
	else CallCopy(v1.Conjugate(),v2.Conjugate());
      else 
	if (v1.step()<0 && v2.step()<0) CallCopy(v1.Reverse(),v2.Reverse());
	else CallCopy(v1,v2);
  }


  //
  // Swap Vectors
  //

  template <class T> void Swap(const VectorView<T>& v1,
      const VectorView<T>& v2);
  template <class T> void Swap(const VectorView<T>& v1, Vector<T>& v2);
  template <class T> inline void Swap(Vector<T>& v1, const VectorView<T>& v2) 
  { Swap(v2,v1); }
  template <class T> void Swap(Vector<T>& v1, Vector<T>& v2);

  //
  // Functions of Vectors
  //

  template <class T> inline RealType(T) Norm(const GenVector<T>& v)
  { return v.Norm(); }

  template <class T> inline RealType(T) Norm1(const GenVector<T>& v)
  { return v.Norm1(); }

  template <class T> RealType(T) NormSq(const GenVector<T>& v)
  { return v.NormSq(); }

  template <class T> RealType(T) Norm2(const GenVector<T>& v)
  { return v.Norm2(); }

  template <class T> inline RealType(T) NormInf(const GenVector<T>& v)
  { return v.NormInf(); }

  template <class T> inline T SumElements(const GenVector<T>& v)
  { return v.SumElements(); }

  template <class T> inline RealType(T) SumAbsElements(const GenVector<T>& v)
  { return v.SumAbsElements(); }

  template <class T> inline T MinElement(const GenVector<T>& v, 
      size_t* iminout=0)
  { return v.MinElement(iminout); }

  template <class T> inline T MaxElement(const GenVector<T>& v, 
      size_t* imaxout=0)
  { return v.MaxElement(imaxout); }

  template <class T> inline RealType(T) MinAbsElement(const GenVector<T>& v, 
      size_t* iminout=0)
  { return v.MinAbsElement(iminout); }

  template <class T> inline RealType(T) MaxAbsElement(const GenVector<T>& v, 
      size_t* imaxout=0)
  { return v.MaxAbsElement(imaxout); }

  template <class T> inline ConstVectorView<T> Conjugate(const GenVector<T>& v)
  { return v.Conjugate(); }

  template <class T> inline VectorView<T> Conjugate(const VectorView<T>& v)
  { return v.Conjugate(); }

  template <class T> inline VectorView<T> Conjugate(Vector<T>& v)
  { return v.Conjugate(); }


  //
  // Vector ==, != Vector
  //

  template <class T> bool operator==(
      const GenVector<T>& v1, const GenVector<T>& v2);

  template <class T> inline bool operator!=(
      const GenVector<T>& v1, const GenVector<T>& v2)
  { return !(v1 == v2); }

  //
  // I/O
  //

  template <class T> inline ostream& operator<<(
      ostream& fout, const GenVector<T>& v)
  { v.Write(fout); return fout;}

  template <class T> istream& operator>>(
      istream& fin, const VectorView<T>& v);

  template <class T> inline istream& operator>>(istream& fin, Vector<T>& v)
  { return fin >> v.View(); }

  template <class T> istream& operator>>(istream& fin, Vector<T>* v);

  template <class T> inline std::string Type(const Vector<T>& v)
  { return std::string("Vector<")+Type(T())+">"; }
  template <class T> inline std::string Type(const GenVector<T>& v)
  { return std::string("GenVector<")+Type(T())+","+Text(v.ct())+">"; }
  template <class T> inline std::string Type(const ConstVectorView<T>& v)
  { return std::string("ConstVectorView<")+Type(T())+","+Text(v.ct())+">"; }
  template <class T> inline std::string Type(const VectorView<T>& v)
  { 
    return std::string("VectorView<")+Type(T())+","+Text(v.ct())+">"; 
  }

} // namespace tmv

#endif