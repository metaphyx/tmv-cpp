///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// The Template Matrix/Vector Library for C++ was created by Mike Jarvis     //
// Copyright (C) 1998 - 2009                                                 //
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
// along with this program in the file LICENSE.                              //
//                                                                           //
// If not, write to:                                                         //
// The Free Software Foundation, Inc.                                        //
// 51 Franklin Street, Fifth Floor,                                          //
// Boston, MA  02110-1301, USA.                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


// This file defines two classes that are used by TMV for allocating
// memory.
//
// First, AlignedArray works basically like a regular new v[]
// allocation, except that if we are doing SSE or SSE2, then we enforce
// that the allocation is aligned on a 128 byte boundary.
//
// Second, StackArray is used for small arrays and emulates a normal C array
// on the stack: T v[N].  However, when N is large, it uses the 
// heap instead to avoid stack overflows.

#ifndef Array_H
#define Array_H

const int TMV_MaxStack = 1024; // bytes

namespace tmv
{
    template <class T>
    static bool TMV_Aligned(const T* p)
    { return (reinterpret_cast<size_t>(p) & 0xf) == 0; }

    // There doesn't seem to be any portable C++ function that guarantees
    // that the memory allocated will be aligned as necessary for 
    // SSE functions.
    // Sometimes posix_memalign or memalign does this job.
    // But it doesn't seem to be standard, since some systems don't have it.
    // Other systems have _mm_malloc (ICPC) or _aligned_malloc (WIN).
    // So probably I should have ifdefs that check to see if one of these
    // is available and use that instead.
    //
    // But for now we have this simple class that simply loads a bit more 
    // memory than necessary and then finds the starting point that is 
    // 16 byte aligned.

    // Also, the TMV_END_PADDING option is implemented here.  If it is 
    // defined, then we write 0's to the end of the full 16 byte word.
    // This is mostly useful when running valgrind with a BLAS library
    // that isn't careful about reading past the end of the allocated
    // memory.  GotoBLAS for example.
    // So if end padding is enabled, we make sure to allocate enough memory
    // to finish the block of 16 bytes.  And we write 0's to the values
    // that aren't part of the requested memory.
    
    // First the regular non-SSE version, where we don't need aligment.
    template <class T>
    class AlignedMemory
    {
    public:
        AlignedMemory() : p(0) {}
        void allocate(const size_t n) 
        { 
#ifdef TMV_END_PADDING
            const size_t nn = n + 16/sizeof(T);
            p = new T[nn];
            for(size_t i=n;i<nn;++i) p[i] = T(0);
#else
            p = new T[n]; 
#endif
        }
        void deallocate()
        { if (p) delete [] p; p=0; }
        void swapWith(AlignedMemory<T>& rhs)
        { T* temp = p; p = rhs.p; rhs.p = temp; }
        T* get() { return p; }
        const T* get() const { return p; }
    private:
        T* p;
    };

    // Now specialize float and double
    // We do this regardless of whether __SSE__ or __SSE2__ is defined,
    // since these might be allocated in a unit that doesn't defined them
    // and then have get() called in a unit that does.  This leads to problems!
    // So we always make the float and double allocations aligned at
    // 16 byte boundaries.
    template <>
    class AlignedMemory<float>
    {
    public:
        AlignedMemory() : p(0) {}
        void allocate(const size_t n) 
        { 
#ifdef TMV_END_PADDING
            const size_t nn = (n<<2)+15 + 16;
            p = new char[nn];
            float* pf = get();
            for(size_t i=n;i<(nn>>2);++i) pf[i] = 0.F;
#else
            p = new char[(n<<2)+15];
#endif
            TMVAssert((void*)(p+(n<<2)+15) >= (void*)(get()+n));
        }
        void deallocate()
        { if (p) delete [] p; p=0; }
        void swapWith(AlignedMemory<float>& rhs)
        { char* temp = p; p = rhs.p; rhs.p = temp; }
        float* get() 
        {
            float* pf = reinterpret_cast<float*>(
                p + ((0x10-((size_t)(p) & 0xf)) & ~0x10));
            TMVAssert( TMV_Aligned(pf) );
            return pf;
        }
        const float* get() const 
        {
            const float* pf = reinterpret_cast<const float*>(
                p + ((0x10-((size_t)(p) & 0xf)) & ~0x10));
            TMVAssert( TMV_Aligned(pf) );
            return pf;
        }
    private:
        char* p;
    };
    template <>
    class AlignedMemory<double>
    {
    public:
        AlignedMemory() : p(0) {}
        void allocate(const size_t n) 
        { 
#ifdef TMV_END_PADDING
            const size_t nn = (n<<3)+15 + 16;
            p = new char[nn];
            double* pd = get();
            for(size_t i=n;i<(nn>>3);++i) pd[i] = 0.;
#else
            p = new char[(n<<3)+15];
#endif
            TMVAssert((void*)(p+(n<<3)+15) >= (void*)(get()+n));
        }
        void deallocate()
        { if (p) delete [] p; p=0; }
        void swapWith(AlignedMemory<double>& rhs)
        { char* temp = p; p = rhs.p; rhs.p = temp; }
        double* get() 
        {
            double* pd = reinterpret_cast<double*>(
                p + ((0x10-((size_t)(p) & 0xf)) & ~0x10));
            TMVAssert( TMV_Aligned(pd) );
            return pd;
        }
        const double* get() const 
        {
            const double* pd = reinterpret_cast<const double*>(
                p + ((0x10-((size_t)(p) & 0xf)) & ~0x10));
            TMVAssert( TMV_Aligned(pd) );
            return pd;
        }

    private :
        char* p;

    };

#ifdef TMV_INITIALIZE_NAN
    // This option is to stress test the code to make sure it works
    // ok if uninitialized data happens to have a nan in it.
    // Naive BLAS calls can fail when there are nan's, since it recasts
    // the equation y = A*x as y = beta*y + alpha*A*x with alpha=1 and
    // beta=0.  So if y initially has nan's, then the beta*y
    // part has 0*nan => nan, which is then added to whatever 
    // alpha*A*x has, so stays nan on output.
    // TMV should be accounting for this kind of thing correctly, and 
    // this section here helps test for it.
    template <class T>
    struct TMV_Nan 
    {
        static T get() 
        { 
            static T zero(0);
            static T nan = 
                std::numeric_limits<T>::is_integer ? UNKNOWN : zero/zero;
            return nan; 
        }
    };
    template <class T>
    struct TMV_Nan<std::complex<T> >
    {
        static std::complex<T> get()
        { return std::complex<T>(TMV_Nan<T>::get(),TMV_Nan<T>::get()); }
    };
#endif

    // Now the actual class that makes AlignedMemory work like a normal 
    // pointer.
    template <class T>
    class AlignedArray
    {
    public :

        AlignedArray() 
        {
#ifdef TMV_INITIALIZE_NAN
            _n = 0;
#endif
        }
        AlignedArray(const size_t n) 
        {
            p.allocate(n); 
#ifdef TMV_INITIALIZE_NAN
            _n = n;
            for(size_t i=0;i<_n;++i) get()[i] = TMV_Nan<T>::get();
#endif
        }
        ~AlignedArray() 
        { 
#ifdef TMV_INITIALIZE_NAN
            for(size_t i=0;i<_n;++i) get()[i] = T(-999);
#endif
            p.deallocate(); 
        }

        T& operator*() { return *get(); }
        T* operator->() { return get(); }
        operator T*() { return get(); }

        const T& operator*() const { return *get(); }
        const T* operator->() const { return get(); }
        operator const T*() const { return get(); }

        void swapWith(AlignedArray<T>& rhs) 
        {
#ifdef TMV_INITIALIZE_NAN
            TMV_SWAP(_n,rhs._n);
#endif
            p.swapWith(rhs.p); 
        }
        void resize(const size_t n) 
        { 
#ifdef TMV_INITIALIZE_NAN
            for(size_t i=0;i<_n;++i) get()[i] = T(-999);
#endif
            p.deallocate(); 
            p.allocate(n); 
#ifdef TMV_INITIALIZE_NAN
            _n = n;
            for(size_t i=0;i<_n;++i) get()[i] = TMV_Nan<T>::get();
#endif
        }

        T* get() { return p.get(); }
        const T* get() const  { return p.get(); }

    private :

        AlignedMemory<T> p;
#ifdef TMV_INITIALIZE_NAN
        size_t _n;
#endif

        AlignedArray& operator=(AlignedArray& p2);
        AlignedArray(const AlignedArray& p2);
    };

    template <class RT>
    class AlignedArray<std::complex<RT> >
    {
    public :
        typedef std::complex<RT> T;

        AlignedArray()
        {
#ifdef TMV_INITIALIZE_NAN
            _n = 0;
#endif
        }
        AlignedArray(const size_t n) 
        { 
            p.allocate(n<<1); 
#ifdef TMV_INITIALIZE_NAN
            _n = n;
            for(size_t i=0;i<_n;++i) 
                get()[i] = TMV_Nan<std::complex<RT> >::get();
#endif
        }
        ~AlignedArray() 
        {
#ifdef TMV_INITIALIZE_NAN
            for(size_t i=0;i<_n;++i) get()[i] = std::complex<RT>(-999,-888);
#endif
            p.deallocate(); 
        }

        T& operator*() { return *get(); }
        T* operator->() { return get(); }
        operator T*() { return get(); }

        const T& operator*() const { return *get(); }
        const T* operator->() const { return get(); }
        operator const T*() const { return get(); }

        void swapWith(AlignedArray<T>& rhs) 
        { 
#ifdef TMV_INITIALIZE_NAN
            TMV_SWAP(_n,rhs._n);
#endif
            p.swapWith(rhs.p); 
        }
        void resize(const size_t n) 
        { 
#ifdef TMV_INITIALIZE_NAN
            for(size_t i=0;i<_n;++i) get()[i] = std::complex<RT>(-999,-888);
#endif
            p.deallocate();
            p.allocate(n<<1); 
#ifdef TMV_INITIALIZE_NAN
            _n = n;
            for(size_t i=0;i<_n;++i) 
                get()[i] = TMV_Nan<std::complex<RT> >::get();
#endif
        }

        T* get() { return reinterpret_cast<T*>(p.get()); }
        const T* get() const 
        { return reinterpret_cast<const T*>(p.get()); }

    private :

        AlignedMemory<RT> p;
#ifdef TMV_INITIALIZE_NAN
        size_t _n;
#endif

        AlignedArray& operator=(AlignedArray& p2);
        AlignedArray(const AlignedArray& p2);
    };



    // The rest of this file implements memory that is allocated on 
    // the stack rather than on the heap.  Here things are a bit easier,
    // since this will always align __m128 objects on 16 byte boundaries.
    // So the way to get float or double aligned is simply to union
    // the array with one of these.

    // Another wrinkle though is that we don't actually want to use the
    // stack for very large arrays, since it will crash.  And very small
    // arrays don't need the alignement.
    // So here is a helper class that has bigN as a template parameter
    // to decide whether to use the stack or not.
    // And smallN is for N < 4 or N < 2 where the SSE alignment isn't 
    // necessary, to make sure we don't gratuitously use extra memory when 
    // we have a lot of SmallVector<float,2>'s or something like that.
    template <class T, int N, bool bigN, bool smallN>
    class StackArray2;

    template <class T, int N>
    class StackArray2<T,N,false,false>
    // !bigN, !smallN
    {
    public:
#ifdef TMV_END_PADDING
        StackArray2() { for(int i=N;i<NN;++i) get()[i] = T(0); }
#endif
        T* get() { return p; }
        const T* get() const { return p; }
    private:
#ifdef TMV_END_PADDING
        enum { NN = N + (16/sizeof(T)) };
#else 
        enum { NN = N };
#endif
#ifdef __GNUC__
        T p[NN] __attribute__ ((aligned (16)));
#else
        T p[NN]; 
#endif
    };
    template <class T, int N>
    class StackArray2<T,N,false,true>
    // smallN 
    {
    public:
#ifdef TMV_END_PADDING
        StackArray2() { for(int i=N;i<NN;++i) get()[i] = T(0); }
#endif
        T* get() { return p; }
        const T* get() const { return p; }
    private:
#ifdef TMV_END_PADDING
        enum { NN = N + 4 };
#else 
        enum { NN = N };
#endif
        T p[NN];
    };

#ifdef __SSE__
    template <int N>
    class StackArray2<float,N,false,false>
    // !bigN, !smallN 
    {
    public:
#ifdef TMV_END_PADDING
        StackArray2() { for(int i=N;i<NN;++i) get()[i] = 0.F; }
#endif
        float* get() { return xp.p; }
        const float* get() const { return xp.p; }
    private:
#ifdef TMV_END_PADDING
        enum { NN = N + 4 };
#else 
        enum { NN = N };
#endif
        union { float p[NN]; __m128 x; } xp;
    };
    template <int N>
    class StackArray2<float,N,false,true>
    // smallN 
    {
    public:
#ifdef TMV_END_PADDING
        StackArray2() { for(int i=N;i<NN;++i) get()[i] = 0.F; }
#endif
        float* get() { return p; }
        const float* get() const { return p; }
    private:
#ifdef TMV_END_PADDING
        enum { NN = N + 4 };
#else 
        enum { NN = N };
#endif
        float p[NN];
    };
#endif
#ifdef __SSE2__
    template <int N>
    class StackArray2<double,N,false,false>
    // !bigN, !smallN 
    {
    public:
#ifdef TMV_END_PADDING
        StackArray2() { for(int i=N;i<NN;++i) get()[i] = 0.; }
#endif
        double* get() { return xp.p; }
        const double* get() const { return xp.p; }
    private:
#ifdef TMV_END_PADDING
        enum { NN = N + 2 };
#else 
        enum { NN = N };
#endif
        union { double p[NN]; __m128d x; } xp;
    };
    template <int N>
    class StackArray2<double,N,false,true>
    // smallN 
    {
    public:
#ifdef TMV_END_PADDING
        StackArray2() { for(int i=N;i<NN;++i) get()[i] = 0.; }
#endif
        double* get() { return p; }
        const double* get() const { return p; }
    private:
#ifdef TMV_END_PADDING
        enum { NN = N + 2 };
#else 
        enum { NN = N };
#endif
        double p[NN];
    };
#endif

    template <class T, int N>
    class StackArray2<T,N,true,false>
    // bigN
    {
    public:
        StackArray2() : p(N) {}
        T* get() { return p.get(); }
        const T* get() const { return p.get(); }
    private:
        AlignedArray<T> p;
    };

    // Now the real class that we use: StackArray<T,N>
    template <class T, int N>
    class StackArray
    {
    public :
        StackArray()
        {
#ifdef TMV_INITIALIZE_NAN
            for(int i=0;i<N;++i) get()[i] = TMV_Nan<T>::get();
#endif
        }
        ~StackArray() 
        {
#ifdef TMV_INITIALIZE_NAN
            for(int i=0;i<N;++i) get()[i] = T(-999);
#endif
        }

        T& operator*() { return *get(); }
        T* operator->() { return get(); }
        operator T*() { return get(); }

        const T& operator*() const { return *get(); }
        const T* operator->() const { return get(); }
        operator const T*() const { return get(); }

        T* get() { return p.get(); }
        const T* get() const { return p.get(); }

    private :
        enum { bigN = N*int(sizeof(T)) > TMV_MaxStack };
        enum { smallN = (
#ifdef __SSE__
                Traits2<T,float>::sametype ? ( N < 4 ) :
#endif
#ifdef __SSE2__
                Traits2<T,double>::sametype ? ( N < 2 ) :
#endif
                false ) };

        StackArray2<T,N,bigN,smallN> p;

        StackArray& operator=(StackArray& p2);
        StackArray(const StackArray& p2);
    };

    template <class RT, int N>
    class StackArray<std::complex<RT>,N>
    {
    public :
        typedef std::complex<RT> T;

        StackArray() 
        {
#ifdef TMV_INITIALIZE_NAN
            for(int i=0;i<N;++i) get()[i] = TMV_Nan<std::complex<RT> >::get();
#endif
        }
        ~StackArray() 
        {
#ifdef TMV_INITIALIZE_NAN
            for(int i=0;i<N;++i) get()[i] = std::complex<RT>(-999,-888);
#endif
        }

        T& operator*() { return *get(); }
        T* operator->() { return get(); }
        operator T*() { return get(); }

        const T& operator*() const { return *get(); }
        const T* operator->() const { return get(); }
        operator const T*() const { return get(); }

        T* get() { return reinterpret_cast<T*>(p.get()); }
        const T* get() const 
        { return reinterpret_cast<const T*>(p.get()); }

    private :

        enum { bigN = N*int(sizeof(T)) > TMV_MaxStack };
        enum { smallN = (
#ifdef __SSE__
                Traits2<T,float>::sametype ? ( N < 2 ) :
#endif
#ifdef __SSE2__
                Traits2<T,double>::sametype ? ( N < 1 ) :
#endif
                false ) };

        StackArray2<RT,(N<<1),bigN,smallN> p;

        StackArray& operator=(StackArray& p2);
        StackArray(const StackArray& p2);
    };

} // namespace tmv

#endif
