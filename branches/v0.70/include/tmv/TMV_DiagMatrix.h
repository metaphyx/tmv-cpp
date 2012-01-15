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


//---------------------------------------------------------------------------
//
// This file defines the TMV DiagMatrix class.
//
// The DiagMatrix class is provided for efficient storage of a diagonal
// matrix.  You can do most of the things that you can do with a 
// regular Matrix, but it will do them more efficiently.
//
// Constructors:
//
//    DiagMatrix<T,A>(int size)
//        Makes a DiagMatrix with column size and row size = size
//        with _uninitialized_ values
//
//    DiagMatrix<T,A>(int size, T x)
//        Makes a DiagMatrix of size n with all values = x
//
//    DiagMatrix<T,A>(const Vector<T>& vv)
//        Make a DiagMatrix which copies the elements of vv.
//
//
// Access Functions
//
//    int colsize() const
//    int rowsize() const
//    int size() const
//        Return the dimensions of the DiagMatrix
//
//    T& operator()(int i)
//    T operator()(int i) const
//    T& operator()(int i, int j)
//    T operator()(int i, int j) const
//        Return the (i,j) element of the DiagMatrix
//        For the single paramter version, j=i
//
//    VectorView& diag()
//    ConstVectorView& diag() const
//        Return the diagonal of the DiagMatrix as a VectorView
//
//
// Modifying Functions - The same as the regular Matrix counterparts
//
//    DiagMatrix& setZero()
//    DiagMatrix& setAllTo(T x)
//    DiagMatrix& addToAll(T x)
//    DiagMatrix<T>& transposeSelf() 
//        (Does nothing.)
//    DiagMatrix& conjugateSelf()
//    DiagMatrix& setToIdentity(x = 1)
//    void Swap(DiagMatrix& m1, DiagMatrix& m2)
//
//
// subDiagMatrix:
//
//    DiagMatrixView subDiagMatrix(int i1, int i2, int istep=1)
//        Returns a Sub-DiagMatrix which extends from i1 to i2 (step istep)
//        which refers to the same physical elements as the original.
//        As usual, i2 is the "one past the end" element.
//
//    DiagMatrixView realPart()
//    DiagMatrixView imagPart()
//        Returns a view to the real/imag elements of a complex DiagMatrix.
//
//    DiagMatrixView view()
//    DiagMatrixView transpose()
//        Returns a view of a DiagMatrix.
//
//    DiagMatrixView conjugate(m)
//    DiagMatrixView adjoint(m)
//        Returns a view to the conjugate of a DiagMatrix.
//
// Functions of DiagMatrices - Same as for regular Matrices:
//
//    m.det() or Det(m)
//    m.logDet() or m.logDet(T* sign) or LogDet(m)
//    m.trace() or Trace(m)
//    m.sumElements() or SumElements(m)
//    m.sumAbsElements() or SumAbsElements(m)
//    m.sumAbs2Elements() or SumAbs2Elements(m)
//    m.norm() or m.normF() or Norm(m) or NormF(m)
//    m.normSq() or NormSq(m)
//    m.norm1() or Norm1(m)
//    m.norm2() or Norm2(m)
//    m.normInf() or NormInf(m)
//    m.maxAbsElement() or MaxAbsElements(m)
//    m.maxAbs2Element() or MaxAbs2Elements(m)
//        (Note - for diagonal matrices, 
//        norm1 = norm2 = normInf = maxAbsElement.)
//
//    m.inverse() or Inverse(m)
//    m.invertSelf()
//    m.makeInverse(minv) (takes either Matrix or DiagMatrix argument)
//    m.makeInverseATA(invata) (takes either Matrix or DiagMatrix argument)
//
// I/O: 
//
//    os << d 
//        Writes d to ostream os as a full matrix
//
//    os << CompactIO() << d
//        Writes only the diagonal Vector to os
//
//    is >> d
//    is >> CompactIO() >> d
//        Reads in d in either format
//
//

#ifndef TMV_DiagMatrix_H
#define TMV_DiagMatrix_H

#include "tmv/TMV_BaseDiagMatrix.h"
#include "tmv/TMV_BaseTriMatrix.h"
#include "tmv/TMV_Vector.h"
#include "tmv/TMV_TriMatrix.h"
#include "tmv/TMV_Matrix.h"
#include <vector>

namespace tmv {

    template <class T> 
    class GenDiagMatrix : 
        virtual public AssignableToDiagMatrix<T>,
        virtual public AssignableToUpperTriMatrix<T>,
        virtual public AssignableToLowerTriMatrix<T>,
        public BaseMatrix<T>
    {
    public:

        typedef TMV_RealType(T) RT;
        typedef TMV_ComplexType(T) CT;
        typedef T value_type;
        typedef RT real_type;
        typedef CT complex_type;
        typedef GenDiagMatrix<T> type;
        typedef DiagMatrix<T> copy_type;
        typedef ConstDiagMatrixView<T> const_view_type;
        typedef const_view_type const_transpose_type;
        typedef const_view_type const_conjugate_type;
        typedef const_view_type const_adjoint_type;
        typedef ConstDiagMatrixView<RT> const_realpart_type;
        typedef ConstVectorView<T> const_vec_type;
        typedef DiagMatrixView<T> nonconst_type;
        typedef typename const_vec_type::const_iterator const_iterator;

        //
        // Constructors
        //

        inline GenDiagMatrix<T>() {}
        inline GenDiagMatrix(const type&) {}
        virtual inline ~GenDiagMatrix() {}

        //
        // Access Functions
        //

        inline int size() const { return diag().size(); }
        inline int colsize() const { return size(); }
        inline int rowsize() const { return size(); }
        inline DiagType dt() const { return NonUnitDiag; }

        inline T operator()(int i, int j) const 
        {
            TMVAssert(i>=0 && i<size());
            TMVAssert(j>=0 && j<size());
            if (i==j) return diag()(i); 
            else return T(0);
        }

        inline T operator()(int i) const 
        {
            TMVAssert(i>=0 && i<size());
            return diag()(i); 
        }

        inline const_vec_type diag() const { return cdiag(); }

        template <class T2> 
        inline bool isSameAs(const BaseMatrix<T2>& ) const
        { return false; }

        inline bool isSameAs(const GenDiagMatrix<T>& m2) const
        { 
            if (this == &m2) return true;
            else return (diag().isSameAs(m2.diag())); 
        }

        inline void assignToM(const MatrixView<RT>& m2) const
        {
            TMVAssert(m2.colsize() == size());
            TMVAssert(m2.rowsize() == size());
            TMVAssert(isReal(T()));
            m2.diag() = diag(); 
            m2.upperTri().offDiag().setZero();
            m2.lowerTri().offDiag().setZero();
        }
        inline void assignToM(const MatrixView<CT>& m2) const
        {
            TMVAssert(m2.colsize() == size());
            TMVAssert(m2.rowsize() == size());
            m2.diag() = diag(); 
            m2.upperTri().offDiag().setZero();
            m2.lowerTri().offDiag().setZero();
        }

        inline void assignToU(const UpperTriMatrixView<RT>& m2) const
        {
            TMVAssert(m2.size() == size());
            TMVAssert(isReal(T()));
            m2.diag() = diag(); 
            m2.offDiag().setZero();
        }
        inline void assignToU(const UpperTriMatrixView<CT>& m2) const
        {
            TMVAssert(m2.size() == size());
            m2.diag() = diag(); 
            m2.offDiag().setZero();
        }

        inline void assignToL(const LowerTriMatrixView<RT>& m2) const
        {
            TMVAssert(m2.size() == size());
            TMVAssert(isReal(T()));
            m2.diag() = diag(); 
            m2.offDiag().setZero();
        }
        inline void assignToL(const LowerTriMatrixView<CT>& m2) const
        {
            TMVAssert(m2.size() == size());
            m2.diag() = diag(); 
            m2.offDiag().setZero();
        }

        inline void assignToD(const DiagMatrixView<RT>& m2) const
        { 
            TMVAssert(m2.size() == size());
            TMVAssert(isReal(T()));
            m2.diag() = diag(); 
        }
        inline void assignToD(const DiagMatrixView<CT>& m2) const
        {
            TMVAssert(m2.size() == size());
            m2.diag() = diag(); 
        }

        //
        // subDiagMatrix
        //

        inline const_view_type cSubDiagMatrix(int i1, int i2) const
        { return const_view_type(diag().cSubVector(i1,i2)); }

        inline const_view_type subDiagMatrix(int i1, int i2) const
        {
            TMVAssert(diag().hasSubVector(i1,i2,1));
            return cSubDiagMatrix(i1,i2);
        }

        inline const_view_type cSubDiagMatrix(int i1, int i2, int istep) const
        { return const_view_type(diag().cSubVector(i1,i2,istep)); }

        inline const_view_type subDiagMatrix(int i1, int i2, int istep) const
        {
            TMVAssert(diag().hasSubVector(i1,i2,istep));
            return cSubDiagMatrix(i1,i2,istep);
        }

        inline const_realpart_type realPart() const
        { return const_realpart_type(diag().realPart()); }

        inline const_realpart_type imagPart() const
        { return const_realpart_type(diag().imagPart()); }

        inline const_view_type view() const
        { return const_view_type(diag()); }

        inline const_view_type transpose() const
        { return view(); }

        inline const_view_type conjugate() const
        { return const_view_type(diag().conjugate()); }

        inline const_view_type adjoint() const
        { return conjugate(); }

        inline nonconst_type nonConst() const
        { return nonconst_type(diag().nonConst()); }


        //
        // Functions of DiagMatrix
        //

        T det() const;

        RT logDet(T* sign=0) const;

        inline T trace() const
        { return diag().sumElements(); }

        inline T sumElements() const
        { return diag().sumElements(); }

        inline RT sumAbsElements() const
        { return diag().sumAbsElements(); }

        inline RT sumAbs2Elements() const
        { return diag().sumAbs2Elements(); }

        inline RT norm() const 
        { return normF(); }

        inline RT normF() const 
        { return diag().norm(); }

        inline RT normSq(RT scale = RT(1)) const 
        { return diag().normSq(scale); }

        inline RT norm1() const 
        { return diag().maxAbsElement(); }

        inline RT doNorm2() const 
        { return diag().maxAbsElement(); }

        inline RT doCondition() const 
        { return diag().maxAbsElement()/diag().minAbsElement(); }

        inline RT norm2() const 
        { return doNorm2(); }

        inline RT condition() const 
        { return doCondition(); }

        inline RT normInf() const
        { return diag().maxAbsElement(); }

        inline RT maxAbsElement() const
        { return diag().maxAbsElement(); }

        inline RT maxAbs2Element() const
        { return diag().maxAbs2Element(); }

        inline bool isSingular() const 
        { return diag().minAbsElement() == RT(0); }

        template <class T1> 
        void doMakeInverse(const MatrixView<T1>& minv) const;

        template <class T1> 
        void doMakeInverse(const DiagMatrixView<T1>& minv) const;

        void doMakeInverseATA(const MatrixView<T>& ata) const;
        void doMakeInverseATA(const DiagMatrixView<T>& ata) const;

        inline void makeInverse(const MatrixView<T>& minv) const
        { 
            TMVAssert(minv.colsize() == size());
            TMVAssert(minv.rowsize() == size());
            doMakeInverse(minv);
        }

        template <class T1> 
        inline void makeInverse(const MatrixView<T1>& minv) const
        { 
            TMVAssert(minv.colsize() == size());
            TMVAssert(minv.rowsize() == size());
            doMakeInverse(minv);
        }

        template <class T1> 
        inline void makeInverse(const DiagMatrixView<T1>& minv) const
        { 
            TMVAssert(minv.size() == size());
            doMakeInverse(minv);
        }

        QuotXD<T,T> QInverse() const;
        inline QuotXD<T,T> inverse() const
        { return QInverse(); }

        inline void makeInverseATA(const MatrixView<T>& ata) const
        { 
            TMVAssert(ata.colsize() == size());
            TMVAssert(ata.rowsize() == size());
            doMakeInverseATA(ata);
        }
        inline void makeInverseATA(const DiagMatrixView<T>& ata) const
        { 
            TMVAssert(ata.size() == size());
            doMakeInverseATA(ata);
        }

        template <class T1, int A>
        inline void makeInverse(DiagMatrix<T1,A>& minv) const
        { makeInverse(minv.view()); }

        template <class T1, int A>
        inline void makeInverse(Matrix<T1,A>& minv) const
        { makeInverse(minv.view()); }

        template <int A> 
        inline void makeInverseATA(DiagMatrix<T,A>& minv) const
        { makeInverseATA(minv.view()); }

        template <int A>
        inline void makeInverseATA(Matrix<T,A>& minv) const
        { makeInverseATA(minv.view()); }


        // 
        // I/O
        //

        void write(const TMV_Writer& writer) const;

        // 
        // Arithmetic Helpers
        //

        template <class T1> 
        void doLDivEq(const VectorView<T1>& v) const;

        template <class T1, class T0> 
        void doLDiv(const GenVector<T1>& v1, const VectorView<T0>& v0) const;

        template <class T1> 
        void doLDivEq(const MatrixView<T1>& m) const;

        template <class T1, class T0> 
        void doLDiv(const GenMatrix<T1>& m1, const MatrixView<T0>& m0) const;

        template <class T1> 
        inline void LDivEq(const VectorView<T1>& v) const
        {
            TMVAssert(v.size() == size());
            doLDivEq(v);
        }

        template <class T1, class T0> 
        inline void LDiv(
            const GenVector<T1>& v1, const VectorView<T0>& v0) const
        {
            TMVAssert(v1.size() == size());
            TMVAssert(v0.size() == size());
            doLDiv(v1,v0);
        }

        template <class T1> 
        inline void RDivEq(const VectorView<T1>& v) const
        { LDivEq(v); }

        template <class T1, class T0> 
        inline void RDiv(
            const GenVector<T1>& v1, const VectorView<T0>& v0) const
        { LDiv(v1,v0); }

        template <class T1> 
        inline void LDivEq(const MatrixView<T1>& m) const
        {
            TMVAssert(m.colsize() == size());
            doLDivEq(m);
        }
        template <class T1, class T0> 
        inline void LDiv(
            const GenMatrix<T1>& m1, const MatrixView<T0>& m0) const
        {
            TMVAssert(m1.colsize() == size());
            TMVAssert(m0.colsize() == size());
            TMVAssert(m1.rowsize() == m0.rowsize());
            doLDiv(m1,m0);
        }
        template <class T1> 
        inline void RDivEq(const MatrixView<T1>& m) const
        { LDivEq(m.transpose()); }

        template <class T1, class T0> 
        void RDiv(const GenMatrix<T1>& m1, const MatrixView<T0>& m0) const
        { LDiv(m1.transpose(),m0.transpose()); }

        template <class T1> 
        void divEq(const DiagMatrixView<T1>& m) const
        { LDivEq(m.diag()); }

        template <class T1, class T0> 
        void div(
            const GenDiagMatrix<T1>& m1, const DiagMatrixView<T0>& m0) const
        { LDiv(m1.diag(),m0.diag()); }

        // For easier compatibility with regular matrices:
        inline void divideInPlace() const {}
        inline void saveDiv() const {}
        inline void setDiv() const {}
        inline void unsetDiv() const {}
        inline void resetDiv() const {}
        inline bool divIsSet() const { return true; }
        inline void divideUsing(DivType TMV_DEBUGPARAM(dt)) const 
        { TMVAssert(dt == LU); }
        inline bool checkDecomp(std::ostream* fout=0) const { return true; }
        inline bool checkDecomp(
            const BaseMatrix<T>& m2, std::ostream* fout=0) const
        { return true; }

        inline T cref(int i, int j) const
        { return i==j ? cdiag()(i) : 0; }
        inline const T* cptr() const
        { return cdiag().cptr(); }
        inline int step() const
        { return cdiag().step(); }
        inline bool isconj() const
        { return cdiag().isconj(); }

        inline const_iterator begin() const
        { return diag().begin(); }
        inline const_iterator end() const
        { return diag().end(); }

    protected :

        virtual ConstVectorView<T> cdiag() const = 0;

    private :

        type& operator=(const type&);

    }; // GenDiagMatrix

    template <class T, int A> 
    class ConstDiagMatrixView : 
        public GenDiagMatrix<T>
    {
    public :

        typedef GenDiagMatrix<T> base;
        typedef ConstDiagMatrixView<T,A> type;
        typedef ConstVectorView<T> const_vec_type;

        inline ConstDiagMatrixView(const type& rhs) : itsdiag(rhs.cdiag()) 
        { TMVAssert(Attrib<A>::viewok); }

        inline ConstDiagMatrixView(const base& rhs) : itsdiag(rhs.diag())
        { TMVAssert(Attrib<A>::viewok); }

        explicit inline ConstDiagMatrixView(const GenVector<T>& v) : itsdiag(v)
        { TMVAssert(Attrib<A>::viewok); }

        virtual inline ~ConstDiagMatrixView() {}

    protected :

        ConstVectorView<T> itsdiag;
        inline ConstVectorView<T> cdiag() const { return itsdiag; }

    private :

        type& operator=(const type&);

    }; // ConstDiagMatrixView

    template <class T> 
    class ConstDiagMatrixView<T,FortranStyle> : 
        public ConstDiagMatrixView<T,CStyle>
    {
    public :

        typedef TMV_RealType(T) RT;
        typedef GenDiagMatrix<T> base;
        typedef ConstDiagMatrixView<T,FortranStyle> type;
        typedef ConstVectorView<T,FortranStyle> const_vec_type;
        typedef ConstDiagMatrixView<T,FortranStyle> const_view_type;
        typedef const_view_type const_transpose_type;
        typedef const_view_type const_conjugate_type;
        typedef const_view_type const_adjoint_type;
        typedef ConstDiagMatrixView<RT,FortranStyle> const_realpart_type;
        typedef ConstDiagMatrixView<T,CStyle> c_type;
        typedef DiagMatrixView<T,FortranStyle> nonconst_type;

        inline ConstDiagMatrixView(const type& rhs) : c_type(rhs) {}

        inline ConstDiagMatrixView(const base& rhs) : c_type(rhs) {}

        explicit inline ConstDiagMatrixView(const GenVector<T>& v) :
            c_type(v) {}

        virtual inline ~ConstDiagMatrixView() {}

        //
        // Access Functions
        //

        inline T operator()(int i, int j) const 
        { 
            TMVAssert(i>0 && i<=size());
            TMVAssert(j>0 && j<=size());
            if (i==j) return diag()(i);
            else return T(0);
        }

        inline T operator()(int i) const 
        { 
            TMVAssert(i>0 && i<=size());
            return diag()(i);
        }

        inline const_vec_type diag() const 
        { return const_vec_type(c_type::diag()); }

        //
        // subDiagMatrix
        //

        inline const_view_type subDiagMatrix(int i1, int i2) const
        {
            TMVAssert(diag().hasSubVector(i1,i2,1));
            return base::cSubDiagMatrix(i1-1,i2);
        }

        inline const_view_type subDiagMatrix(int i1, int i2, int istep) const
        {
            TMVAssert(diag().hasSubVector(i1,i2,istep));
            return base::cSubDiagMatrix(i1-1,i2-1+istep,istep);
        }

        inline const_realpart_type realPart() const
        { return const_realpart_type(diag().realPart()); }

        inline const_realpart_type imagPart() const
        { return const_realpart_type(diag().imagPart()); }

        inline const_view_type view() const
        { return const_view_type(diag()); }

        inline const_view_type transpose() const
        { return view(); }

        inline const_view_type conjugate() const
        { return const_view_type(diag().conjugate()); }

        inline const_view_type adjoint() const
        { return conjugate(); }

        inline nonconst_type nonConst() const
        { return nonconst_type(diag().nonConst()); }

        using c_type::size;

        type& operator=(const type&);

    }; // ConstDiagMatrixView - FortranStyle

    template <class T, int A> 
    class DiagMatrixView : 
        public GenDiagMatrix<T>
    {
    public:

        typedef TMV_RealType(T) RT;
        typedef TMV_ComplexType(T) CT;
        typedef GenDiagMatrix<T> base;
        typedef DiagMatrixView<T,A> type;
        typedef DiagMatrixView<T,A> view_type;
        typedef view_type transpose_type;
        typedef view_type conjugate_type;
        typedef view_type adjoint_type;
        typedef DiagMatrixView<RT,A> realpart_type;
        typedef VectorView<T,A> vec_type;
        typedef ConstVectorView<T,A> const_vec_type;
        typedef typename RefHelper<T>::reference reference;
        typedef typename vec_type::iterator iterator;

        //
        // Constructors
        //

        inline DiagMatrixView(const type& rhs) : itsdiag(rhs.diag()) 
        { TMVAssert(Attrib<A>::diagmatrixok); }

        explicit inline DiagMatrixView(const vec_type& _diag) : itsdiag(_diag)
        { TMVAssert(Attrib<A>::diagmatrixok); }

        virtual inline ~DiagMatrixView() {} 

        //
        // Op=
        //

        inline const type& operator=(const type& m2) const
        { m2.assignToD(*this); return *this; }

        inline const type& operator=(const type& m2) 
        { m2.assignToD(*this); return *this; }

        inline const type& operator=(const GenDiagMatrix<RT>& m2) const
        { m2.assignToD(*this); return *this; }

        inline const type& operator=(const GenDiagMatrix<CT>& m2) const
        { m2.assignToD(*this); return *this; }

        template <class T2> 
        inline const type& operator=(const GenDiagMatrix<T2>& m2) const
        { itsdiag = m2.diag(); return *this; }

        inline const type& operator=(const T& x) const 
        { return setToIdentity(x); }

        inline const type& operator=(const AssignableToDiagMatrix<RT>& m2) const
        { 
            TMVAssert(size() == m2.size());
            m2.assignToD(*this);
            return *this;
        }

        inline const type& operator=(const AssignableToDiagMatrix<CT>& m2) const
        { 
            TMVAssert(size() == m2.size());
            m2.assignToD(*this);
            return *this;
        }

        typedef ListAssigner<T,iterator> MyListAssigner;
        inline MyListAssigner operator<<(const T& x) const
        { return MyListAssigner(begin(),size(),x); }


        //
        // Access
        //

        inline reference operator()(int i) const 
        { 
            TMVAssert(i>=0 && i<size());
            return diag()(i); 
        }
        inline reference operator()(int i, int TMV_DEBUGPARAM(j)) const 
        { 
            TMVAssert(i>=0 && i<size());
            TMVAssert(i==j); 
            return diag()(i); 
        }

        inline vec_type diag() const { return itsdiag; }

        //
        // Modifying Functions
        //

        inline const type& setZero() const 
        { diag().setZero(); return *this; }

        inline const type& setAllTo(const T& x) const
        { diag().setAllTo(x); return *this; }

        inline const type& addToAll(const T& x) const
        { diag().addToAll(x); return *this; }

        inline const type& clip(RT thresh) const
        { diag().clip(thresh); return *this; }

        inline const type& transposeSelf() const
        { return *this; }

        inline const type& conjugateSelf() const
        { diag().conjugateSelf(); return *this; }

        const type& invertSelf() const;

        inline const type& setToIdentity(const T& x=T(1)) const 
        { return setAllTo(x); }

        //
        // subDiagMatrix
        //

        inline view_type cSubDiagMatrix(int i1, int i2) const
        { return view_type(itsdiag.cSubVector(i1,i2)); }

        inline view_type subDiagMatrix(int i1, int i2) const
        {
            TMVAssert(diag().hasSubVector(i1,i2,1));
            return cSubDiagMatrix(i1,i2);
        }

        inline view_type cSubDiagMatrix(int i1, int i2, int istep) const
        { return view_type(itsdiag.cSubVector(i1,i2,istep)); }

        inline view_type subDiagMatrix(int i1, int i2, int istep) const
        {
            TMVAssert(diag().hasSubVector(i1,i2,istep));
            return cSubDiagMatrix(i1,i2,istep);
        }

        inline realpart_type realPart() const
        { return realpart_type(diag().realPart()); }

        inline realpart_type imagPart() const
        { return realpart_type(diag().imagPart()); }

        inline view_type view() const
        { return *this; }

        inline view_type transpose() const
        { return *this; }

        inline view_type conjugate() const
        { return view_type(diag().conjugate()); }

        inline view_type adjoint() const
        { return conjugate(); }

        //
        // I/O
        //

        void read(const TMV_Reader& reader) const;

        using GenDiagMatrix<T>::size;

        inline iterator begin() const
        { return diag().begin(); }
        inline iterator end() const
        { return diag().end(); }

    protected:

        VectorView<T> itsdiag;
        inline ConstVectorView<T> cdiag() const { return itsdiag; }

    }; // DiagMatrixView

    template <class T> 
    class DiagMatrixView<T,FortranStyle> : public DiagMatrixView<T,CStyle>
    {
    public:

        typedef TMV_RealType(T) RT;
        typedef TMV_ComplexType(T) CT;
        typedef GenDiagMatrix<T> base;
        typedef DiagMatrixView<T,FortranStyle> type;
        typedef DiagMatrixView<T,CStyle> c_type;
        typedef DiagMatrixView<T,FortranStyle> view_type;
        typedef view_type transpose_type;
        typedef view_type conjugate_type;
        typedef view_type adjoint_type;
        typedef DiagMatrixView<RT,FortranStyle> realpart_type;
        typedef VectorView<T,FortranStyle> vec_type;
        typedef typename RefHelper<T>::reference reference;

        //
        // Constructors
        //

        inline DiagMatrixView(const type& rhs) : c_type(rhs) {}

        inline DiagMatrixView(const c_type& rhs) : c_type(rhs) {}

        explicit inline DiagMatrixView(const VectorView<T>& _diag) :
            c_type(_diag) {}

        virtual inline ~DiagMatrixView() {} 

        //
        // Op=
        //

        inline const type& operator=(const type& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const type& m2) 
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const c_type& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const c_type& m2)
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const GenDiagMatrix<RT>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const GenDiagMatrix<CT>& m2) const
        { c_type::operator=(m2); return *this; }

        template <class T2> 
        inline const type& operator=(const GenDiagMatrix<T2>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const T& x) const 
        { c_type::operator=(x); return *this; }

        inline const type& operator=(const AssignableToDiagMatrix<RT>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const AssignableToDiagMatrix<CT>& m2) const
        { c_type::operator=(m2); return *this; }

        typedef typename c_type::MyListAssigner MyListAssigner;
        inline MyListAssigner operator<<(const T& x) const
        { return c_type::operator<<(x); }


        //
        // Access
        //

        inline reference operator()(int i) const 
        { 
            TMVAssert(i>0 && i<=size());
            return diag()(i); 
        }
        inline reference operator()(int i, int TMV_DEBUGPARAM(j)) const 
        { 
            TMVAssert(i==j); 
            TMVAssert(i>0 && i<=size());
            return diag()(i); 
        }

        inline vec_type diag() const 
        { return c_type::diag(); }

        //
        // Modifying Functions
        //

        inline const type& setZero() const 
        { diag().setZero(); return *this; }

        inline const type& setAllTo(const T& x) const
        { diag().setAllTo(x); return *this; }

        inline const type& addToAll(const T& x) const
        { diag().addToAll(x); return *this; }

        inline const type& clip(RT thresh) const
        { diag().clip(thresh); return *this; }

        inline const type& transposeSelf() const
        { return *this; }

        inline const type& conjugateSelf() const
        { diag().conjugateSelf(); return *this; }

        inline const type& invertSelf() const
        { return c_type::invertSelf(); }

        inline const type& setToIdentity(const T& x=T(1)) const 
        { diag().setAllTo(x); return *this; }


        //
        // subDiagMatrix
        //

        inline view_type subDiagMatrix(int i1, int i2) const
        {
            TMVAssert(diag().hasSubVector(i1,i2,1));
            return c_type::cSubDiagMatrix(i1-1,i2);
        }

        inline view_type cSubDiagMatrix(int i1, int i2, int istep) const
        {
            TMVAssert(diag().hasSubVector(i1,i2,istep));
            return c_type::cSubDiagMatrix(i1-1,i2-1+istep,istep);
        }

        inline view_type subDiagMatrix(int i1, int i2, int istep) const
        {
            TMVAssert(diag().hasSubVector(i1,i2,istep));
            return view_type(diag().subVector(i1,i2,istep)); 
        }

        inline realpart_type realPart() const
        { return realpart_type(diag().realPart()); }

        inline realpart_type imagPart() const
        { return realpart_type(diag().imagPart()); }

        inline view_type view() const
        { return *this; }

        inline view_type transpose() const
        { return *this; }

        inline view_type conjugate() const
        { return view_type(diag().conjugate()); }

        inline view_type adjoint() const
        { return conjugate(); }

        using GenDiagMatrix<T>::size;

    }; // FortranStyle DiagMatrixView


    template <class T, int A> 
    class DiagMatrix : public GenDiagMatrix<T>
    {
    public:

        typedef TMV_RealType(T) RT;
        typedef TMV_ComplexType(T) CT;
        typedef GenDiagMatrix<T> base;
        typedef DiagMatrix<T,A> type;
        typedef DiagMatrixView<T,A> view_type;
        typedef view_type transpose_type;
        typedef view_type conjugate_type;
        typedef view_type adjoint_type;
        typedef DiagMatrixView<RT,A> realpart_type;
        typedef VectorView<T,A> vec_type;
        typedef ConstDiagMatrixView<T,A> const_view_type;
        typedef const_view_type const_transpose_type;
        typedef const_view_type const_conjugate_type;
        typedef const_view_type const_adjoint_type;
        typedef ConstDiagMatrixView<RT,A> const_realpart_type;
        typedef ConstVectorView<T,A> const_vec_type;
        typedef T& reference;
        typedef typename const_vec_type::const_iterator const_iterator;
        typedef typename vec_type::iterator iterator;

        //
        // Constructors
        //

        inline explicit DiagMatrix(int _size=0) : itsdiag(_size) 
        { TMVAssert(_size >= 0); }

        inline DiagMatrix(int _size, const T& x) : itsdiag(_size,x)  
        { TMVAssert(_size >= 0); }

        inline explicit DiagMatrix(const GenVector<T>& rhs) : itsdiag(rhs) {}

        inline explicit DiagMatrix(const GenMatrix<T>& m) : itsdiag(m.diag()) {}

        inline DiagMatrix(const type& rhs) : itsdiag(rhs.diag()) {}

        inline DiagMatrix(const GenDiagMatrix<RT>& rhs) : itsdiag(rhs.size()) 
        { rhs.assignToD(view()); }

        inline DiagMatrix(const GenDiagMatrix<CT>& rhs) : itsdiag(rhs.size()) 
        { 
            TMVAssert(isComplex(T()));
            rhs.assignToD(view()); 
        }

        template <class T2> 
        inline DiagMatrix(const GenDiagMatrix<T2>& rhs) : itsdiag(rhs.diag()) {}

        inline DiagMatrix(const AssignableToDiagMatrix<RT>& m2) :
            itsdiag(m2.colsize())
        {
            TMVAssert(m2.colsize() == m2.rowsize());
            m2.assignToD(view()); 
        }

        inline DiagMatrix(const AssignableToDiagMatrix<CT>& m2) :
            itsdiag(m2.colsize())
        {
            TMVAssert(m2.colsize() == m2.rowsize());
            TMVAssert(isComplex(T()));
            m2.assignToD(view()); 
        }

        virtual inline ~DiagMatrix() {}


        //
        // Op=
        //

        inline type& operator=(const type& m2)
        {
            TMVAssert(m2.size() == size());
            m2.assignToD(view());
            return *this; 
        }

        inline type& operator=(const GenDiagMatrix<RT>& m2)
        {
            TMVAssert(m2.size() == size());
            m2.assignToD(view()); 
            return *this; 
        }

        inline type& operator=(const GenDiagMatrix<CT>& m2)
        {
            TMVAssert(m2.size() == size());
            TMVAssert(isComplex(T()));
            m2.assignToD(view()); 
            return *this; 
        }

        template <class T2> 
        inline type& operator=(const GenDiagMatrix<T2>& m2)
        { 
            TMVAssert(m2.size() == size());
            view() = m2; 
            return *this; 
        }

        inline type& operator=(const T& x) { view() = x; return *this; }

        inline type& operator=(const AssignableToDiagMatrix<RT>& m2)
        { 
            TMVAssert(m2.size() == size());
            m2.assignToD(view());
            return *this; 
        }

        inline type& operator=(const AssignableToDiagMatrix<CT>& m2)
        { 
            TMVAssert(m2.size() == size());
            TMVAssert(isComplex(T()));
            m2.assignToD(view());
            return *this; 
        }

        typedef ListAssigner<T,iterator> MyListAssigner;
        inline MyListAssigner operator<<(const T& x)
        { return MyListAssigner(begin(),size(),x); }


        //
        // Access
        //

        inline T& operator()(int i) 
        { 
            if (A==CStyle) { 
                TMVAssert(i>=0 && i<size());
                return itsdiag(i); 
            } else { 
                TMVAssert(i>0 && i<=size());
                return itsdiag(i-1); 
            }
        }

        inline T& operator()(int i, int TMV_DEBUGPARAM(j)) 
        { 
            if (A==CStyle) { 
                TMVAssert(i>=0 && i<size()); 
                TMVAssert(j>=0 && j<size()); 
            } else { 
                TMVAssert(i>0 && i<=size()); 
                TMVAssert(j>0 && j<=size()); 
            }
            TMVAssert(i==j);
            return operator()(i);
        }

        inline T operator()(int i) const 
        {
            if (A==CStyle) { 
                TMVAssert(i>=0 && i<size());
                return itsdiag(i);
            } else { 
                TMVAssert(i>0 && i<=size());
                return itsdiag(i-1);
            }
        }

        inline T operator()(int i,int j) const 
        {
            if (A==CStyle) { 
                TMVAssert(i>=0 && i<size()); 
                TMVAssert(j>=0 && j<size()); 
            } else { 
                TMVAssert(i>0 && i<=size()); 
                TMVAssert(j>0 && j<=size()); 
            }
            if (i==j) return operator()(i);
            else return T(0);
        }

        inline vec_type diag() { return itsdiag.view(); }

        inline const_vec_type diag() const { return itsdiag.view(); }

        //
        // Modifying Functions
        //

        inline type& setZero() 
        { itsdiag.setZero(); return *this; }

        inline type& setAllTo(const T& x) 
        { itsdiag.setAllTo(x); return *this; }

        inline type& addToAll(const T& x) 
        { itsdiag.addToAll(x); return *this; }

        inline type& clip(RT thresh)
        { diag().clip(thresh); return *this; }

        inline type& transposeSelf() 
        { return *this; }

        inline type& conjugateSelf() 
        { itsdiag.conjugateSelf(); return *this; }

        inline type& invertSelf()
        { view().invertSelf(); return *this; }

        inline type& setToIdentity(const T& x=T(1)) 
        { itsdiag.setAllTo(x); return *this; }

        //
        // subDiagMatrix
        //

        inline view_type cSubDiagMatrix(int i1, int i2) 
        { return view_type(itsdiag.cSubVector(i1,i2)); }

        inline view_type subDiagMatrix(int i1, int i2) 
        { 
            TMVAssert(diag().hasSubVector(i1,i2,1));
            if (A == int(FortranStyle)) { --i1; }
            return cSubDiagMatrix(i1,i2); 
        }

        inline view_type cSubDiagMatrix(int i1, int i2, int istep) 
        { return view_type(itsdiag.cSubVector(i1,i2,istep)); }

        inline view_type subDiagMatrix(int i1, int i2, int istep) 
        { 
            TMVAssert(diag().hasSubVector(i1,i2,istep));
            if (A == int(FortranStyle)) { --i1; i2 += istep-1; }
            return cSubDiagMatrix(i1,i2,istep); 
        }

        inline realpart_type realPart()
        { return realpart_type(diag().realPart()); }

        inline realpart_type imagPart()
        { return realpart_type(diag().imagPart()); }

        inline const_view_type cSubDiagMatrix(int i1, int i2) const
        { return const_view_type(itsdiag.cSubVector(i1,i2)); }

        inline const_view_type subDiagMatrix(int i1, int i2) const
        {
            TMVAssert(diag().hasSubVector(i1,i2,1));
            if (A == int(FortranStyle)) { --i1; }
            return cSubDiagMatrix(i1,i2);
        }

        inline const_view_type cSubDiagMatrix(int i1, int i2, int istep) const
        { return const_view_type(itsdiag.cSubVector(i1,i2,istep)); }

        inline const_view_type subDiagMatrix(int i1, int i2, int istep) const
        { 
            TMVAssert(diag().hasSubVector(i1,i2,istep));
            if (A == int(FortranStyle)) { --i1; i2 += istep-1; }
            return cSubDiagMatrix(i1,i2,istep);
        }

        inline const_realpart_type realPart() const
        { return const_realpart_type(diag().realPart()); }

        inline const_realpart_type imagPart() const
        { return const_realpart_type(diag().imagPart()); }

        inline const_view_type view() const
        { return const_view_type(itsdiag); }

        inline const_view_type transpose() const
        { return view(); }

        inline const_view_type conjugate() const
        { return const_view_type(itsdiag.conjugate()); }

        inline const_view_type adjoint() const
        { return conjugate(); }

        inline view_type view() 
        { return view_type(itsdiag.view()); }

        inline view_type transpose() 
        { return view(); }

        inline view_type conjugate()
        { return view_type(itsdiag.conjugate()); }

        inline view_type adjoint() 
        { return conjugate(); }

        //
        // I/O
        //
        
        void read(const TMV_Reader& reader);

        using base::size;

        inline void resize(int n) 
        { 
            TMVAssert(n >= 0);
            itsdiag.resize(n); 
        }

        inline iterator begin()
        { return diag().begin(); }
        inline iterator end()
        { return diag().end(); }

        inline const_iterator begin() const
        { return diag().begin(); }
        inline const_iterator end() const
        { return diag().end(); }

    protected :

        Vector<T> itsdiag;
        inline ConstVectorView<T> cdiag() const { return itsdiag.view(); }

        friend void Swap(DiagMatrix<T,A>& m1, DiagMatrix<T,A>& m2)
        {
            TMVAssert(m1.size() == m2.size());
            Swap(m1.itsdiag,m2.itsdiag);
        }

    }; // DiagMatrix

    //---------------------------------------------------------------------------

    //
    // Special Creators:
    //   DiagMatrixViewOf(v)
    //   DiagMatrixViewOf(T* v, n)
    //   DiagMatrixViewOf(T* v, n, step)
    //

    template <class T> 
    inline ConstDiagMatrixView<T> DiagMatrixViewOf(const GenVector<T>& v)
    { return ConstDiagMatrixView<T>(v); }

    template <class T, int A> 
    inline ConstDiagMatrixView<T,A> DiagMatrixViewOf(
        const ConstVectorView<T,A>& v)
    { return ConstDiagMatrixView<T,A>(v); }

    template <class T, int A> 
    inline ConstDiagMatrixView<T,A> DiagMatrixViewOf(const Vector<T,A>& v)
    { return ConstDiagMatrixView<T,A>(v.view()); }

    template <class T, int A> 
    inline DiagMatrixView<T,A> DiagMatrixViewOf(const VectorView<T,A>& v)
    { return DiagMatrixView<T,A>(v); }

    template <class T, int A> 
    inline DiagMatrixView<T,A> DiagMatrixViewOf(Vector<T,A>& v)
    { return DiagMatrixView<T,A>(v.view()); }

    template <class T> 
    inline ConstDiagMatrixView<T> DiagMatrixViewOf(const T* m, int size)
    {
        TMVAssert(size >= 0);
        return ConstDiagMatrixView<T>(VectorViewOf(m,size)); 
    }

    template <class T> 
    inline DiagMatrixView<T> DiagMatrixViewOf(T* m, int size)
    {
        TMVAssert(size >= 0);
        return DiagMatrixView<T>(VectorViewOf(m,size)); 
    }

    template <class T> 
    inline ConstDiagMatrixView<T> DiagMatrixViewOf(
        const T* m, int size, int step)
    {
        TMVAssert(size >= 0);
        return ConstDiagMatrixView<T>(VectorViewOf(m,size,step)); 
    }

    template <class T> 
    inline DiagMatrixView<T> DiagMatrixViewOf(T* m, int size, int step)
    {
        TMVAssert(size >= 0);
        return DiagMatrixView<T>(VectorViewOf(m,size,step)); 
    }


    //
    // Swap Matrices
    //

    template <class T> 
    inline void Swap(const DiagMatrixView<T>& m1, const DiagMatrixView<T>& m2)
    { Swap(m1.diag(),m2.diag()); }

    template <class T, int A> 
    inline void Swap(const DiagMatrix<T,A>& m1, const DiagMatrixView<T>& m2)
    { Swap(m1.diag(),m2.diag()); }

    template <class T, int A> 
    inline void Swap(const DiagMatrixView<T>& m1, const DiagMatrix<T,A>& m2)
    { Swap(m1.diag(),m2.diag()); }

    //
    // Views:
    //

    template <class T> 
    inline ConstDiagMatrixView<T> Transpose(const GenDiagMatrix<T>& m)
    { return m.transpose(); }

    template <class T, int A> 
    inline ConstDiagMatrixView<T,A> Transpose(const ConstDiagMatrixView<T,A>& m)
    { return m.transpose(); }

    template <class T, int A> 
    inline ConstDiagMatrixView<T,A> Transpose(const DiagMatrix<T,A>& m)
    { return m.transpose(); }

    template <class T, int A> 
    inline DiagMatrixView<T,A> Transpose(const DiagMatrixView<T,A>& m)
    { return m.transpose(); }

    template <class T, int A> 
    inline DiagMatrixView<T,A> Transpose(DiagMatrix<T,A>& m)
    { return m.transpose(); }

    template <class T> 
    inline ConstDiagMatrixView<T> Conjugate(const GenDiagMatrix<T>& m)
    { return m.conjugate(); }

    template <class T, int A> 
    inline ConstDiagMatrixView<T,A> Conjugate(const ConstDiagMatrixView<T,A>& m)
    { return m.conjugate(); }

    template <class T, int A> 
    inline ConstDiagMatrixView<T,A> Conjugate(const DiagMatrix<T,A>& m)
    { return m.conjugate(); }

    template <class T, int A> 
    inline DiagMatrixView<T,A> Conjugate(const DiagMatrixView<T,A>& m)
    { return m.conjugate(); }

    template <class T, int A> 
    inline DiagMatrixView<T,A> Conjugate(DiagMatrix<T,A>& m)
    { return m.conjugate(); }

    template <class T> 
    inline ConstDiagMatrixView<T> Adjoint(const GenDiagMatrix<T>& m)
    { return m.adjoint(); }

    template <class T, int A> 
    inline ConstDiagMatrixView<T,A> Adjoint(const ConstDiagMatrixView<T,A>& m)
    { return m.adjoint(); }

    template <class T, int A> 
    inline ConstDiagMatrixView<T,A> Adjoint(const DiagMatrix<T,A>& m)
    { return m.adjoint(); }

    template <class T, int A> 
    inline DiagMatrixView<T,A> Adjoint(const DiagMatrixView<T,A>& m)
    { return m.adjoint(); }

    template <class T, int A> 
    inline DiagMatrixView<T,A> Adjoint(DiagMatrix<T,A>& m)
    { return m.adjoint(); }

    template <class T> 
    inline QuotXD<T,T> Inverse(const GenDiagMatrix<T>& m)
    { return m.inverse(); }



    //
    // DiagMatrix ==, != DiagMatrix
    //

    template <class T1, class T2> 
    inline bool operator==(
        const GenDiagMatrix<T1>& m1, const GenDiagMatrix<T2>& m2)
    { return m1.diag() == m2.diag(); }

    template <class T1, class T2> 
    inline bool operator!=(
        const GenDiagMatrix<T1>& m1, const GenDiagMatrix<T2>& m2)
    { return !(m1 == m2); }

    template <class T1, class T2> 
    inline bool operator==(
        const GenDiagMatrix<T1>& m1, const GenMatrix<T2>& m2)
    {
        return 
            m1.diag() == m2.diag() && 
            m2.upperTri().offDiag().maxAbs2Element() == T2(0) &&
            m2.lowerTri().offDiag().maxAbs2Element() == T2(0);
    }

    template <class T1, class T2> 
    inline bool operator==(
        const GenMatrix<T1>& m1, const GenDiagMatrix<T2>& m2)
    { return m2 == m1; }

    template <class T1, class T2> 
    inline bool operator!=(
        const GenDiagMatrix<T1>& m1, const GenMatrix<T2>& m2)
    { return !(m1 == m2); }

    template <class T1, class T2> 
    inline bool operator!=(
        const GenMatrix<T1>& m1, const GenDiagMatrix<T2>& m2)
    { return !(m1 == m2); }

    //
    // I/O
    //

    template <class T>
    inline std::istream& operator>>(
        const TMV_Reader& reader, const DiagMatrixView<T>& m)
    { m.read(reader); return reader.getis(); }

    template <class T, int A> 
    inline std::istream& operator>>(
        const TMV_Reader& reader, DiagMatrix<T,A>& m)
    { m.read(reader); return reader.getis(); }

    template <class T>
    std::istream& operator>>(std::istream& is, const DiagMatrixView<T>& m)
    { return is >> IOStyle() >> m; }

    template <class T, int A> 
    std::istream& operator>>(std::istream& is, DiagMatrix<T,A>& m)
    { return is >> IOStyle() >> m; }

} // namespace tmv

#endif