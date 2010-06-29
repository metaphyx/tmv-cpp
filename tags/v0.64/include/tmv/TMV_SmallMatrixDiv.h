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

#ifndef SmallMatrixDiv_H
#define SmallMatrixDiv_H

namespace tmv {

    //
    // Helper Functions 
    // These implement the various Triangle Matrix calculations
    // that are needed for the decompositions and solving routines.
    // We only implement ColMajor versions for L, U, since these are
    // all that are needed in the routines here.
    //

    // I use this macro to inline some of the ref() and cref()
    // calls.  The ref and cref inline to this anyway, but this way
    // I can get rid of some of the IndexStyle template parameters, 
    // which helps to reduce the compiled size of the test suite.
    // So any SmallMatrix<...,CStyle> is left as is.
#define TMV_Val(m,M,N,S,i,j) (S==RowMajor ? m[i*N+j] : m[j*M+i])

#ifndef NOTHROW
    template <class T, int M, int N, StorageType S, IndexStyle I> 
    class SingularSmallMatrix : public Singular
    {
    public:
        SmallMatrix<T,M,N,S,I> A;

        inline SingularSmallMatrix(const SmallMatrix<T,M,N,S,I>& _A) :
            Singular("SmallMatrix."), A(_A) {}
        inline ~SingularSmallMatrix() throw() {}
        inline void write(std::ostream& os) const throw()
        { Singular::write(os); os<<A<<std::endl; }
    };
#endif

    template <int N2, int K, StorageType S2, class T, class T2, int N, int M> 
    inline void LDivEq_L(const SmallMatrix<T,N,M,ColMajor>& L, T2* m)
    // SmallMatrix<T2,N2,K,S2>& m
    {
        TMVAssert(M >= N);
        TMVAssert(N2 >= N);
        //m.view() /= L.lowerTri(UnitDiag);
        for(int j=0;j<N;++j) {
            for(int k=0;k<K;++k) {
                for(int i=j+1;i<N;++i) {
                    //m.ref(i,k) -= m.cref(j,k) * L.cref(i,j);
                    TMV_Val(m,N2,K,S2,i,k) -=
                        TMV_Val(m,N2,K,S2,j,k) * L.cref(i,j);
                }
            }
        }
    }

    template <int N2, int K, StorageType S2, class T, class T2, int N, int M> 
    inline void LDivEq_U(const SmallMatrix<T,M,N,ColMajor>& U, T2* m)
    // SmallMatrix<T2,N2,K,S2>& m
    {
        TMVAssert(M >= N);
        TMVAssert(N2 >= N);
        //m.view() /= U.upperTri();
        for(int j=N-1;j>=0;--j) {
            if (U.cref(j,j) == T(0)) 
#ifdef NOTHROW
            { std::cerr<<"Singular SmallMatrix found\n"; exit(1); }
#else
            { throw Singular(); }
#endif
            for(int k=0;k<K;++k) {
                //m.ref(j,k) /= U.cref(j,j);
                TMV_Val(m,N2,K,S2,j,k) /= U.cref(j,j);
                for(int i=0;i<j;++i) {
                    //m.ref(i,k) -= m.cref(j,k) * U.cref(i,j);
                    TMV_Val(m,N2,K,S2,i,k) -= 
                        TMV_Val(m,N2,K,S2,j,k) * U.cref(i,j);
                }
            }
        }
    }

    template <int K, int N2, StorageType S2, class T, class T2, int N, int M> 
    inline void RDivEq_U(const SmallMatrix<T,M,N,ColMajor>& U, T2* m)
    // SmallMatrix<T2,K,N2,S2>& m
    {
        TMVAssert(M >= N);
        TMVAssert(N2 >= N);
        //m.view() %= U.upperTri();
        for(int i=0;i<N;++i) {
            if (U.cref(i,i) == T(0)) 
#ifdef NOTHROW
            { std::cerr<<"Singular SmallMatrix found\n"; exit(1); }
#else
            { throw Singular(); }
#endif
            for(int k=0;k<K;++k) {
                for(int j=0;j<i;++j) {
                    //m.ref(k,i) -= m.cref(k,j) * U.cref(j,i);
                    TMV_Val(m,K,N2,S2,k,i) -= 
                        TMV_Val(m,K,N2,S2,k,j) * U.cref(j,i);
                }
                //m.ref(k,i) /= U.cref(i,i);
                TMV_Val(m,K,N2,S2,k,i) /= U.cref(i,i);
            }
        }
    }

    template <class T, int M, int N> 
    inline void InvertU(SmallMatrix<T,M,N,ColMajor>& U)
    {
        // Invert the upper triangle portion of U in place
        TMVAssert(M >= N);
        for(int j=0;j<N;++j) {
            if (U.cref(j,j) == T(0)) 
#ifdef NOTHROW
            { std::cerr<<"Singular SmallMatrix found\n"; exit(1); }
#else
            { throw Singular(); }
#endif
            U.ref(j,j) = TMV_RealType(T)(1)/U.cref(j,j);
            //U.col(j,0,j) = -U.SubTriMatrix(0,j) * U.col(j,0,j) * U(j,j);
            for(int k=0;k<j;++k) {
                for(int i=0;i<k;++i) U.ref(i,j) += U.cref(k,j) * U.cref(i,k);
                U.ref(k,j) *= U.cref(k,k);
            }
            for(int i=0;i<j;++i) U.ref(i,j) *= -U.cref(j,j);
        }
    }

    template <class T, int M, int N> 
    inline void InvertL(SmallMatrix<T,N,M,ColMajor>& L)
    {
        // Invert the unit lower triangle portion of L in place
        TMVAssert(M >= N);
        for(int j=N-1;j>=0;--j) {
            //L.col(j,j+1,N) = -L.SubTriMatrix(j+1,N) * L.col(j,j+1,N);
            for(int k=N-1;k>j;--k) {
                for(int i=N-1;i>k;--i) L.ref(i,j) += L.cref(k,j) * L.cref(i,k);
            }
            for(int i=N-1;i>j;--i) L.ref(i,j) = -L.cref(i,j);
        }
    }

    template <int K, StorageType S2, class T, class T2, int M, int N> 
    inline void Q_LDivEq(const SmallMatrix<T,M,N,ColMajor>& Q,
                         const T* beta, T2* m)
    // SmallMatrix<T2,M,K,S2>& m
    {
        TMVAssert(M > N);
        for(int j=0;j<N;++j) if (beta[j] != T(0)) {
            //HouseholderLMult(Q.col(j,j+1,M),beta(j),m.rowRange(j,M));
            for(int k=0;k<K;++k) {
                T2 temp(0);
                for(int i=j+1;i<M;++i) 
                    //temp += TMV_CONJ(Q.cref(i,j)) * m.cref(i,k);
                    temp += TMV_CONJ(Q.cref(i,j)) * TMV_Val(m,M,K,S2,i,k);
                //temp += m.cref(j,k);
                temp += TMV_Val(m,M,K,S2,j,k);
                temp *= beta[j];
                //m.ref(j,k) -= temp;
                TMV_Val(m,M,K,S2,j,k) -= temp;
                for(int i=j+1;i<M;++i) 
                    //m.ref(i,k) -= Q.cref(i,j) * temp;
                    TMV_Val(m,M,K,S2,i,k) -= Q.cref(i,j) * temp;
            }
        }
    }

    template <int K, StorageType S2, class T, class T2, int M, int N> 
    inline void Q_RDivEq(const SmallMatrix<T,M,N,ColMajor>& Q,
                         const T* beta, T2* m)
    //SmallMatrix<T2,K,M,S2>& m
    {
        TMVAssert(M > N);
        for(int j=N-1;j>=0;--j) if (beta[j] != T(0)) {
            //HouseholderLMult(Q.col(j,j+1,M).conjugate(),beta(j),
            //    m.colRange(j,M).transpose());
            for(int k=0;k<K;++k) {
                T2 temp(0);
                for(int i=j+1;i<M;++i) 
                    //temp += Q.cref(i,j) * m.cref(k,i);
                    temp += Q.cref(i,j) * TMV_Val(m,K,M,S2,k,i);
                //temp += m.cref(k,j);
                temp += TMV_Val(m,K,M,S2,k,j);
                temp *= beta[j];
                //m.ref(k,j) -= temp;
                TMV_Val(m,K,M,S2,k,j) -= temp;
                for(int i=j+1;i<M;++i) 
                    //m.ref(k,i) -= TMV_CONJ(Q.cref(i,j)) * temp;
                    TMV_Val(m,K,M,S2,k,i) -= TMV_CONJ(Q.cref(i,j)) * temp;
            }
        }
    }


    //
    // Decompositions
    //

    // This basically just takes the NonBlock LU_Decompose function
    // And inlines everything so the compiler can optimize it for small N
    template <class T, int N> 
    inline void DoLUD(SmallMatrix<T,N,N,ColMajor>& LU, int* P, T& signdet)
    {
        // Input as m, output as packed LU
        for (int j=0; j<N; ++j)
        {
            if (j > 1) {
                //LU.col(j,0,j) /= LU.SubMatrix(0,j,0,j).lowerTri(UnitDiag);
                for(int k=0;k<j;++k) if (LU.cref(k,j) != T(0)) {
                    for(int i=k+1;i<j;++i) {
                        LU.ref(i,j) -= LU.cref(k,j) * LU.cref(i,k);
                    }
                }
            }

            if (j > 0) {
                //LU.col(j,j,N) -= LU.SubMatrix(j,N,0,j) * LU.col(j,0,j);
                for(int k=0;k<j;++k) {
                    for(int i=j;i<N;++i) {
                        LU.ref(i,j) -= LU.cref(i,k) * LU.cref(k,j);
                    }
                }
            }

            //LU.col(j,j,N).MaxAbsElement(&ip);
            int ip = j;
            if (j < N-1) {
                TMV_RealType(T) max = 
                    isReal(T()) ? TMV_ABS(LU.cref(j,j)) :
                    TMV_NORM(LU.cref(j,j));
                for(int k=j+1;k<N;++k) {
                    TMV_RealType(T) val = 
                        isReal(T()) ? TMV_ABS(LU.cref(k,j)) :
                        TMV_NORM(LU.cref(k,j));
                    if (val > max) {
                        max = val;
                        ip = k;
                    }
                }
                if (ip != j) {
                    LU.swapRows(ip,j);  
                    signdet = -signdet;
                }
            }
            P[j] = ip;

            //if (LU(j,j) != T(0)) LU.col(j,j+1,N) /= LU(j,j);
            if (LU.cref(j,j) != T(0)) {
                T invlujj = TMV_RealType(T)(1)/LU.cref(j,j);
                for(int k=j+1;k<N;++k) LU.ref(k,j) *= invlujj;
            }
        }
    }

    // This isn't really worth inlining.  We just call the regular
    // Reflect function.
    template <class T> 
    T HouseholderReflect(T& x0, const VectorView<T>& x, T& det);

    // Again, this is just the normal QR_Decompose with everything inlined.
    template <class T, int M, int N> 
    inline void DoQRD(SmallMatrix<T,M,N,ColMajor>& QR, T* beta)
    {
        TMVAssert(M >= N);
        T det(0);
        for(int j=0;j<N;++j) {
            beta[j] = HouseholderReflect(QR.ref(j,j),QR.col(j,j+1,M),det);
            if (beta[j] != T(0) && j<N-1) {
                //HouseholderLMult(v,beta,QR.SubMatrix(j,M,j+1,N));
                for(int k=j+1;k<N;++k) {
                    T temp(0);
                    for(int i=j+1;i<M;++i) temp += 
                        TMV_CONJ(QR.cref(i,j)) * QR.cref(i,k);
                    temp += QR.cref(j,k);
                    temp *= beta[j];
                    QR.ref(j,k) -= temp;
                    for(int i=j+1;i<M;++i) QR.ref(i,k) -= QR.cref(i,j) * temp;
                }
            }
        }
    }

    //
    // Determinant
    //

    template <class T, int M, int N, StorageType S> 
    struct SMDet;

    template <class T, int N, StorageType S> 
    struct SMDet<T,N,N,S>
    {
        static T det(const T* m)
        {
            SmallMatrix<T,N,N,ColMajor> LU;
            // LU = m;
            DoCopy<N,N,S,ColMajor>(m,LU.ptr());
            int P[N];
            T d(1);
            DoLUD(LU,P,d);
            if (d != T(0)) for(int i=0;i<N;++i) d *= LU.cref(i,i);
            return d;
        }
    };

    template <class T, StorageType S> 
    struct SMDet<T,1,1,S>
    { static inline T det(const T* m) { return *m; } };

    template <class T, StorageType S> 
    struct SMDet<T,2,2,S>
    { static inline T det(const T* m) { return (m[0]*m[3] - m[1]*m[2]); } };

    template <class T, StorageType S> 
    struct SMDet<T,3,3,S>
    {
        static T det(const T* m) {
            return (
                m[0] * (m[4]*m[8] - m[5]*m[7]) 
                -m[1] * (m[3]*m[8] - m[5]*m[6])
                +m[2] * (m[3]*m[7] - m[4]*m[6])); 
        }
    };

    template <class T, int M, int N, StorageType S, IndexStyle I> 
    inline T DoDet(const SmallMatrix<T,M,N,S,I>& m)
    { return SMDet<T,M,N,S>::det(m.cptr()); }

    //
    // Matrix Inverse
    //

    template <StorageType S2, class T, class T2, int M, int N>
    inline void SMQRInv(SmallMatrix<T,M,N,ColMajor>& QR, T2* minv)
    //SmallMatrix<T2,N,M,S2> minv
    {
        TMVAssert(M > N);
        T beta[N];
        DoQRD(QR,beta);
        InvertU(QR);

        //minv.setZero();
        for(int i=0;i<M*N;++i) minv[i] = T2(0);
        //minv.colRange(0,N)).upperTri() = QR.upperTri()
        for(int j=0;j<N;++j) for(int i=0;i<=j;i++)
            //minv.ref(i,j) = QR.cref(i,j);
            TMV_Val(minv,N,M,S2,i,j) = QR.cref(i,j);
        Q_RDivEq<N,S2>(QR,beta,minv);
    }

    template <class T, class T2, int M, int N, bool MltN, StorageType S, StorageType S2>
    struct SMInv
    {
        SMInv(const T* m, T2* minv)
            //SmallMatrix<T,M,N,S> m
            //SmallMatrix<T2,N,M,S2> minv
        {
            TMVAssert(M > N);
            // (M < N and M == N specialized below)
            SmallMatrix<T,M,N,ColMajor> QR;
            // QR = m;
            DoCopy<M,N,S,ColMajor>(m,QR.ptr());
            SMQRInv<S2>(QR,minv);
        }
    };

    template <class T, class T2, int M, int N, StorageType S, StorageType S2>
    struct SMInv<T,T2,M,N,true,S,S2>
    {
        SMInv(const T* m, T2* minv)
            //SmallMatrix<T,M,N,S> m
            //SmallMatrix<T2,N,M,S2> minv
        {
            TMVAssert(M < N);
            SmallMatrix<T,N,M,ColMajor> QR;
            // QR = m.transpose();
            DoCopy<M,N,S,RowMajor>(m,QR.ptr());
            SMQRInv<TMV_TransOf(S2)>(QR,minv);
        }
    };

    template <class T, int M, int N, bool MltN, StorageType S, StorageType S2>
    struct SMInv<std::complex<T>,T,M,N,MltN,S,S2>
    {
        SMInv(const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, int M, int N, StorageType S, StorageType S2>
    struct SMInv<std::complex<T>,T,M,N,true,S,S2>
    {
        SMInv(const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, class T2, int N, StorageType S, StorageType S2> 
    struct SMInv<T,T2,N,N,false,S,S2>
    {
        SMInv(const T* m, T2* minv)
            //SmallMatrix<T,N,N,S> m
            //SmallMatrix<T2,N,N,S2> minv
        {
            SmallMatrix<T,N,N,ColMajor> LU;
            // LU = m;
            DoCopy<N,N,S,ColMajor>(m,LU.ptr());
            int P[N];
            T signdet(0);
            DoLUD(LU,P,signdet);

            InvertU(LU);
            InvertL(LU);

            //minv = LU.upperTri() * LU.lowerTri(UnitDiag);
            //minv.setZero();
            for(int i=0;i<N*N;++i) minv[i] = T2(0);
            for(int j=0;j<N;++j) {
                for(int i=0;i<=j;++i) 
                    //minv.ref(i,j) = LU.cref(i,j);
                    TMV_Val(minv,N,N,S2,i,j) = LU.cref(i,j);
                for(int k=j+1;k<N;++k) {
                    for(int i=0;i<=k;++i) {
                        //minv.ref(i,j) += LU.cref(i,k) * LU.cref(k,j);
                        TMV_Val(minv,N,N,S2,i,j) += LU.cref(i,k) * LU.cref(k,j);
                    }
                }
            }

            //minv.reversePermuteCols(P);
            for(int j=N-1;j>=0;--j) if (P[j] != j)
                for(int i=0;i<N;++i) 
                    TMV_SWAP(TMV_Val(minv,N,N,S2,i,j),
                             TMV_Val(minv,N,N,S2,i,P[j]));
        }
    };

    template <class T, int N, StorageType S, StorageType S2>
    struct SMInv<std::complex<T>,T,N,N,false,S,S2>
    {
        SMInv(const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };


    template <class T, class T2, StorageType S, StorageType S2>
    struct SMInv<T,T2,1,1,false,S,S2>
    {
        SMInv(const T* m, T2* minv)
        {
            if (*m == T(0)) 
#ifdef NOTHROW
            { std::cerr<<"Singular SmallMatrix found\n"; exit(1); }
#else
            { throw Singular(); }
#endif
            *minv = T(1)/(*m);
        }
    };

    template <class T, class T2, StorageType S, StorageType S2>
    struct SMInv<T,T2,2,2,false,S,S2>
    {
        SMInv(const T* m, T2* minv)
        {
            T det = SMDet<T,2,2,S>::det(m);
            if (det == T(0)) 
#ifdef NOTHROW
            { std::cerr<<"Singular SmallMatrix found\n"; exit(1); }
#else
            { throw Singular(); }
#endif
            //minv.ref(0,0) = m.cref(1,1)/d;
            //minv.ref(0,1) = -m.cref(0,1)/d;
            //minv.ref(1,0) = -m.cref(1,0)/d;
            //minv.ref(1,1) = m.cref(0,0)/d;
            minv[0] = m[3]/det;
            minv[3] = m[0]/det;
            if (S == S2) {
                minv[1] = -m[1]/det;
                minv[2] = -m[2]/det;
            } else {
                minv[1] = -m[2]/det;
                minv[2] = -m[1]/det;
            }
        }
    };

    template <class T, class T2, StorageType S, StorageType S2>
    struct SMInv<T,T2,3,3,false,S,S2>
    {
        SMInv(const T* m, T2* minv)
        {
            T det = SMDet<T,3,3,S>::det(m);
            if (det == T(0)) 
#ifdef NOTHROW
            { std::cerr<<"Singular SmallMatrix found\n"; exit(1); }
#else
            { throw Singular(); }
#endif
            minv[0] = (m[4]*m[8]-m[7]*m[5])/det;
            minv[4] = (m[0]*m[8]-m[6]*m[2])/det;
            minv[8] = (m[0]*m[4]-m[3]*m[1])/det;
            if (S == S2) {
                minv[1] = -(m[1]*m[8]-m[7]*m[2])/det;
                minv[2] = (m[1]*m[5]-m[4]*m[2])/det;
                minv[3] = -(m[3]*m[8]-m[6]*m[5])/det;
                minv[5] = -(m[0]*m[5]-m[3]*m[2])/det;
                minv[6] = (m[3]*m[7]-m[6]*m[4])/det;
                minv[7] = -(m[0]*m[7]-m[6]*m[1])/det;
            } else {
                minv[1] = -(m[3]*m[8]-m[6]*m[5])/det;
                minv[2] = (m[3]*m[7]-m[6]*m[4])/det;
                minv[3] = -(m[1]*m[8]-m[7]*m[2])/det;
                minv[5] = -(m[0]*m[7]-m[6]*m[1])/det;
                minv[6] = (m[1]*m[5]-m[4]*m[2])/det;
                minv[7] = -(m[0]*m[5]-m[3]*m[2])/det;
            }
        }
    };

    template <class T, StorageType S, StorageType S2>
    struct SMInv<std::complex<T>,T,1,1,false,S,S2>
    {
        SMInv(const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, StorageType S, StorageType S2>
    struct SMInv<std::complex<T>,T,2,2,false,S,S2>
    {
        SMInv(const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, StorageType S, StorageType S2>
    struct SMInv<std::complex<T>,T,3,3,false,S,S2>
    {
        SMInv(const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, class T2, int M, int N, StorageType S, StorageType S2, IndexStyle I, IndexStyle I2> 
    inline void DoInverse(
        const SmallMatrix<T,M,N,S,I>& m, SmallMatrix<T2,N,M,S2,I2>& minv)
    {
#ifndef NOTHROW
        try {
#endif
            SMInv<T,T2,M,N,M<N,S,S2>(m.cptr(),minv.ptr());
#ifndef NOTHROW
        } catch (tmv::Singular) {
            throw SingularSmallMatrix<T,M,N,S,I>(m); 
        }
#endif
    }

    //
    // Essential Matrix Div Algorithms
    //

    template <int K, StorageType S1, StorageType S2, class T, class T1, class T2, int M, int N>
    inline void SMQRLDiv(SmallMatrix<T,M,N,ColMajor>& QR, const T1* m1, T2* m2)
    //SmallMatrix<T1,M,K,S1> m1
    //SmallMatrix<T2,N,K,S2> m2
    {
        TMVAssert(M > N);
        T beta[N];
        DoQRD(QR,beta);

        // m2 = R^-1 Qt m1
        //SmallMatrix<T,M,K,RowMajor> temp = m1;
        T2 temp[M*K];
        DoCopy<M,K,S1,RowMajor>(m1,temp);
        Q_LDivEq<K,RowMajor>(QR,beta,temp);
        //m2 = temp.rowRange(0,N);
        DoCopy<N,K,RowMajor,S2>(temp,m2);
        //m2.view() /= QR.upperTri();
        LDivEq_U<N,K,S2>(QR,m2);
    }

    template <int K, StorageType S1, StorageType S2, class T, class T1, class T2, int M, int N>
    inline void SMQRRDiv(SmallMatrix<T,M,N,ColMajor>& QR, const T1* m1, T2* m2)
    //SmallMatrix<T1,K,N,S1> m1
    //SmallMatrix<T2,K,M,S2> m2
    {
        TMVAssert(M > N);
        T beta[N];
        DoQRD(QR,beta);

        // m2 = m1 R^-1 Qt
        //m2.setZero();
        for(int i=0;i<K*M;++i) m2[i] = T2(0);
        //m2.colRange(0,N) = m1;
        if (S2 == ColMajor) 
            DoCopy<K,N,S1,S2>(m1,m2);
        else 
            for(int i=0;i<K;++i) for(int j=0;j<N;j++) 
                //m2(i,j) = m1(i,j);
                TMV_Val(m2,K,M,S2,i,j) = TMV_Val(m1,K,N,S1,i,j);
        //m2.colRange(0,N) %= QR.upperTri();
        RDivEq_U<K,M,S2>(QR,m2);
        Q_RDivEq<K,S2>(QR,beta,m2);
    }

    template <int K, StorageType S2, class T, class T2, int N>
    inline void SMLULDivEq(SmallMatrix<T,N,N,ColMajor>& LU, T2* m2)
    //SmallMatrix<T2,N,K,S2> m2
    {
        int P[N];
        T signdet(0);
        DoLUD(LU,P,signdet);

        //m2.permuteRows(P);
        for(int i=0;i<N;++i) if (P[i] != i)
            for(int j=0;j<K;++j) 
                TMV_SWAP(TMV_Val(m2,N,K,S2,i,j),TMV_Val(m2,N,K,S2,P[i],j));

        // m2 = L^-1 m2
        //m2.view() /= LU.lowerTri(UnitDiag);
        LDivEq_L<N,K,S2>(LU,m2);

        // m2 = U^-1 m2;
        //m2.view() /= LU.upperTri();
        LDivEq_U<N,K,S2>(LU,m2);
    }


    //
    // Matrix LDiv
    //

    template <class T, class T1, class T2, int M, int N, bool MltN, int K, StorageType S, StorageType S1, StorageType S2> 
    struct SMLDivM
    {
        SMLDivM(const T* m, const T1* m1, T2* m2)
            //SmallMatrix<T,M,N,S> m
            //SmallMatrix<T1,M,K,S1> m1
            //SmallMatrix<T2,N,K,S2> m2
        {
            TMVAssert(M > N);
            // (M < N and M == N specialized below)
            SmallMatrix<T,M,N,ColMajor> QR;
            // QR = m;
            DoCopy<M,N,S,ColMajor>(m,QR.ptr());
            SMQRLDiv<K,S1,S2>(QR,m1,m2);
        }
    };

    template <class T, class T1, class T2, int M, int N, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMLDivM<T,T1,T2,M,N,true,K,S,S1,S2>
    {
        SMLDivM(const T* m, const T1* m1, T2* m2)
            //SmallMatrix<T,M,N,S> m
            //SmallMatrix<T1,M,K,S1> m1
            //SmallMatrix<T2,N,K,S2> m2
        {
            TMVAssert(M < N);
            // m2 = m^-1 m1
            // m2T = m1T mT^-1
            SmallMatrix<T,N,M,ColMajor> QR;
            // QR = m.transpose();
            DoCopy<M,N,S,RowMajor>(m,QR.ptr());
            SMQRRDiv<K,TMV_TransOf(S1),TMV_TransOf(S2)>(QR,m1,m2);
        }
    };

    template <class T, class T1, int M, int N, bool MltN, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMLDivM<std::complex<T>,T1,T,M,N,MltN,K,S,S1,S2>
    {
        SMLDivM(const std::complex<T>*, const T1*, T* )
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, int M, int N, bool MltN, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMLDivM<T,std::complex<T>,T,M,N,MltN,K,S,S1,S2>
    {
        SMLDivM(const T*, const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, class T1, int M, int N, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMLDivM<std::complex<T>,T1,T,M,N,true,K,S,S1,S2>
    {
        SMLDivM(const std::complex<T>*, const T1*, T* )
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, int M, int N, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMLDivM<T,std::complex<T>,T,M,N,true,K,S,S1,S2>
    {
        SMLDivM(const T*, const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, class T2, int N, int K, StorageType S, StorageType S2>
    struct SMLDivEqM
    {
        SMLDivEqM(const T* m, T2* m2)
            //SmallMatrix<T,N,N,S> m
            //SmallMatrix<T2,N,K,S2> m2
        {
            SmallMatrix<T,N,N,ColMajor> LU;
            // LU = m;
            DoCopy<N,N,S,ColMajor>(m,LU.ptr());
            SMLULDivEq<K,S2>(LU,m2);
        }
    };

    template <class T, int N, int K, StorageType S, StorageType S2>
    struct SMLDivEqM<std::complex<T>,T,N,K,S,S2>
    {
        SMLDivEqM(const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, class T1, class T2, int N, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMLDivM<T,T1,T2,N,N,false,K,S,S1,S2>
    {
        SMLDivM(const T* m, const T1* m1, T2* m2)
            //SmallMatrix<T,N,N,S> m
            //SmallMatrix<T1,N,K,S1> m1
            //SmallMatrix<T2,N,K,S2> m2
        {
            //m2 = m1;
            DoCopy<N,K,S1,S2>(m1,m2);
            SMLDivEqM<T,T2,N,K,S,S2>(m,m2);
        }
    };

    template <class T, class T1, int N, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMLDivM<std::complex<T>,T1,T,N,N,false,K,S,S1,S2>
    {
        SMLDivM(const std::complex<T>*, const T1*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, int N, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMLDivM<T,std::complex<T>,T,N,N,false,K,S,S1,S2>
    {
        SMLDivM(const T*, const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    // m2 = m^-1 m2
    template <class T, class T2, int N, int K, StorageType S, StorageType S2, IndexStyle I, IndexStyle I2> 
    inline void DoLDivEq(
        const SmallMatrix<T,N,N,S,I>& m, SmallMatrix<T2,N,K,S2,I2>& m2)
    {
#ifndef NOTHROW
        try {
#endif
            SMLDivEqM<T,T2,N,K,S,S2>(m.cptr(),m2.ptr());
#ifndef NOTHROW
        } catch (tmv::Singular) {
            throw SingularSmallMatrix<T,N,N,S,I>(m); 
        }
#endif
    }

    // m2 = m^-1 m1
    template <class T, class T1, class T2, int M, int N, int K, StorageType S, StorageType S1, StorageType S2, IndexStyle I, IndexStyle I1, IndexStyle I2> 
    inline void DoLDiv(
        const SmallMatrix<T,M,N,S,I>& m,
        const SmallMatrix<T1,M,K,S1,I1>& m1, SmallMatrix<T2,N,K,S2,I2>& m2)
    {
#ifndef NOTHROW
        try {
#endif
            SMLDivM<T,T1,T2,M,N,M<N,K,S,S1,S2>(m.cptr(),m1.cptr(),m2.ptr());
#ifndef NOTHROW
        } catch (tmv::Singular) {
            throw SingularSmallMatrix<T,M,N,S,I>(m); 
        }
#endif
    }


    //
    // Vector LDiv
    //

    // v2 = m^-1 v2
    template <class T, class T2, int N, StorageType S, IndexStyle I, IndexStyle I2> 
    inline void DoLDivEq(
        const SmallMatrix<T,N,N,S,I>& m, SmallVector<T2,N,I2>& v2)
    {
#ifndef NOTHROW
        try {
#endif
            SMLDivEqM<T,T2,N,1,S,ColMajor>(m.cptr(),v2.ptr());
#ifndef NOTHROW
        } catch (tmv::Singular) {
            throw SingularSmallMatrix<T,N,N,S,I>(m); 
        }
#endif
    }

    // v2 = m^-1 v1
    template <class T, class T1, class T2, int M, int N, StorageType S, IndexStyle I, IndexStyle I1, IndexStyle I2> 
    inline void DoLDiv(
        const SmallMatrix<T,M,N,S,I>& m,
        const SmallVector<T1,M,I1>& v1, SmallVector<T2,N,I2>& v2)
    {
#ifndef NOTHROW
        try {
#endif
            SMLDivM<T,T1,T2,M,N,M<N,1,S,ColMajor,ColMajor>(
                m.cptr(),v1.cptr(),v2.ptr());
#ifndef NOTHROW
        } catch (tmv::Singular) {
            throw SingularSmallMatrix<T,M,N,S,I>(m); 
        }
#endif
    }


    //
    // Matrix RDiv
    //

    template <class T, class T1, class T2, int M, int N, bool MltN, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMRDivM
    {
        SMRDivM(const T* m, const T1* m1, T2* m2)
            //SmallMatrix<T,M,N,S> m
            //SmallMatrix<T1,K,N,S1> m1 
            //SmallMatrix<T2,K,M,S2> m2
        {
            TMVAssert(M > N);
            // (M < N and M == N specialized below)
            SmallMatrix<T,M,N,ColMajor> QR;
            // QR = m
            DoCopy<M,N,S,ColMajor>(m,QR.ptr());
            SMQRRDiv<K,S1,S2>(QR,m1,m2);
        }
    };

    template <class T, class T1, class T2, int M, int N, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMRDivM<T,T1,T2,M,N,true,K,S,S1,S2>
    {
        SMRDivM(const T* m, const T1* m1, T2* m2)
            //SmallMatrix<T,M,N,S> m
            //SmallMatrix<T1,K,N,S1> m1
            //SmallMatrix<T2,K,M,S2> m2
        {
            TMVAssert(M < N);
            // m2 = m1 m^-1
            // m2t = mt^-1 m1t
            SmallMatrix<T,N,M,ColMajor> QR;
            // QR = m.transpose();
            DoCopy<M,N,S,RowMajor>(m,QR.ptr());
            SMQRLDiv<K,TMV_TransOf(S1),TMV_TransOf(S2)>(QR,m1,m2);
        }
    };

    template <class T, class T1, int M, int N, bool MltN, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMRDivM<std::complex<T>,T1,T,M,N,MltN,K,S,S1,S2>
    {
        SMRDivM(const std::complex<T>*, const T1*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, int M, int N, bool MltN, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMRDivM<T,std::complex<T>,T,M,N,MltN,K,S,S1,S2>
    {
        SMRDivM(const T*, const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, class T1, int M, int N, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMRDivM<std::complex<T>,T1,T,M,N,true,K,S,S1,S2>
    {
        SMRDivM(const std::complex<T>*, const T1*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, int M, int N, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMRDivM<T,std::complex<T>,T,M,N,true,K,S,S1,S2>
    {
        SMRDivM(const T*, const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, class T2, int N, int K, StorageType S, StorageType S2>
    struct SMRDivEqM
    {
        SMRDivEqM(const T* m, T2* m2)
            //SmallMatrix<T,N,N,S> m
            //SmallMatrix<T2,K,N,S2> m2
        {
            // m2 = m2 m^-1
            // m2t = mt^-1 m2t
            SmallMatrix<T,N,N,ColMajor> LU;
            // LU = m.transpose()
            DoCopy<N,N,S,RowMajor>(m,LU.ptr());
            SMLULDivEq<K,TMV_TransOf(S2)>(LU,m2);
        }
    };

    template <class T, int N, int K, StorageType S, StorageType S2>
    struct SMRDivEqM<std::complex<T>,T,N,K,S,S2>
    {
        SMRDivEqM(const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, class T1, class T2, int N, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMRDivM<T,T1,T2,N,N,false,K,S,S1,S2>
    {
        SMRDivM(const T* m, const T1* m1, T2* m2)
            //SmallMatrix<T,N,N,S> m
            //SmallMatrix<T1,K,N,S1> m1
            //SmallMatrix<T2,K,N,S2> m2
        {
            //m2 = m1;
            DoCopy<K,N,S1,S2>(m1,m2);
            //DoRDivEq(m,m2);
            SMRDivEqM<T,T2,N,K,S,S2>(m,m2);
        }
    };

    template <class T, class T1, int N, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMRDivM<std::complex<T>,T1,T,N,N,false,K,S,S1,S2>
    {
        SMRDivM(const std::complex<T>*, const T1*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    template <class T, int N, int K, StorageType S, StorageType S1, StorageType S2>
    struct SMRDivM<T,std::complex<T>,T,N,N,false,K,S,S1,S2>
    {
        SMRDivM(const T*, const std::complex<T>*, T*)
        { TMVAssert(TMV_FALSE); }
    };

    // m2 = m2 m^-1
    template <class T, class T2, int N, int K, StorageType S, StorageType S2, IndexStyle I, IndexStyle I2> 
    inline void DoRDivEq(
        const SmallMatrix<T,N,N,S,I>& m, SmallMatrix<T2,K,N,S2,I2>& m2)
    {
#ifndef NOTHROW
        try {
#endif
            SMRDivEqM<T,T2,N,K,S,S2>(m.cptr(),m2.ptr());
#ifndef NOTHROW
        } catch (tmv::Singular) {
            throw SingularSmallMatrix<T,N,N,S,I>(m); 
        }
#endif
    }

    // m2 = m^-1 m1
    template <class T, class T1, class T2, int M, int N, int K, StorageType S, StorageType S1, StorageType S2, IndexStyle I, IndexStyle I1, IndexStyle I2> 
    inline void DoRDiv(
        const SmallMatrix<T,M,N,S,I>& m,
        const SmallMatrix<T1,K,N,S1,I1>& m1, SmallMatrix<T2,K,M,S2,I2>& m2)
    {
#ifndef NOTHROW
        try {
#endif
            SMRDivM<T,T1,T2,M,N,M<N,K,S,S1,S2>(m.cptr(),m1.cptr(),m2.ptr());
#ifndef NOTHROW
        } catch (tmv::Singular) {
            throw SingularSmallMatrix<T,M,N,S,I>(m); 
        }
#endif
    }

    //
    // Vector RDiv
    //

    // v2 = v2 m^-1
    template <class T, class T2, int N, StorageType S, IndexStyle I, IndexStyle I2> 
    inline void DoRDivEq(
        const SmallMatrix<T,N,N,S,I>& m, SmallVector<T2,N,I2>& v2)
    {
#ifndef NOTHROW
        try {
#endif
            SMRDivEqM<T,T2,N,1,S,RowMajor>(m.cptr(),v2.ptr()); 
#ifndef NOTHROW
        } catch (tmv::Singular) {
            throw SingularSmallMatrix<T,N,N,S,I>(m); 
        }
#endif
    }

    // v2 = m^-1 v1
    template <class T, class T1, class T2, int M, int N, StorageType S, IndexStyle I, IndexStyle I1, IndexStyle I2> 
    inline void DoRDiv(
        const SmallMatrix<T,M,N,S,I>& m,
        const SmallVector<T1,N,I1>& v1, SmallVector<T2,M,I2>& v2)
    { 
#ifndef NOTHROW
        try {
#endif
            SMRDivM<T,T1,T2,M,N,M<N,1,S,RowMajor,RowMajor>(
                m.cptr(),v1.cptr(),v2.ptr()); 
#ifndef NOTHROW
        } catch (tmv::Singular) {
            throw SingularSmallMatrix<T,M,N,S,I>(m); 
        }
#endif
    }

    //
    // InverseATA
    //

    template <StorageType S2, class T, int M, int N> 
    inline void SMQRATA(SmallMatrix<T,M,N,ColMajor> QR, T* ata)
    {
        TMVAssert(M >= N);
        T beta[N];
        DoQRD(QR,beta);

        InvertU(QR);

        //ata = QR.upperTri() * QR.upperTri().adjoint();
        //ata.setZero();
        for(int i=0;i<N*N;++i) ata[i] = T(0);
        for(int j=0;j<N;++j) {
            for(int k=j;k<N;++k) {
                for(int i=0;i<=k;++i) {
                    //ata.ref(i,j) += QR.cref(i,k) * TMV_CONJ(QR.cref(j,k));
                    TMV_Val(ata,N,N,S2,i,j) +=
                        QR.cref(i,k) * TMV_CONJ(QR.cref(j,k));
                }
            }
        }
    }

    template <class T, int M, int N, bool MltN, StorageType S, StorageType S2>
    struct SMATA
    {
        SMATA(const T* m, T* ata)
            //SmallMatrix<T,M,N,S> m
            //SmallMatrix<T,N,N,S2>& ata
        {
            TMVAssert(M >= N);
            SmallMatrix<T,M,N,ColMajor> QR;
            // QR = m;
            DoCopy<M,N,S,ColMajor>(m,QR.ptr());
            SMQRATA<S2>(QR,ata);
        }
    };

    template <class T, int M, int N, StorageType S, StorageType S2>
    struct SMATA<T,M,N,true,S,S2>
    {
        SMATA(const T* m, T* ata)
            //SmallMatrix<T,M,N,S> m
            //SmallMatrix<T,N,N,S2> ata
        {
            TMVAssert(M < N);
            TMV_Warning("InverseATA called for matrix with more columns than rows.");
            TMV_Warning("The result is really (AAt)^-1");

            SmallMatrix<T,N,M,ColMajor> QR;
            // QR = m.transpose();
            DoCopy<M,N,S,RowMajor>(m,QR.ptr());
            SMQRATA<S2>(QR,ata);
        }
    };

    template <class T, int M, int N, StorageType S, StorageType S2, IndexStyle I, IndexStyle I2> 
    inline void DoInverseATA(
        const SmallMatrix<T,M,N,S,I>& m, SmallMatrix<T,N,N,S2,I2>& ata)
    { SMATA<T,M,N,M<N,S,S2>(m.cptr(),ata.ptr()); }

#undef TMV_Val
}

#endif