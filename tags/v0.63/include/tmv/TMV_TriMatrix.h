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
// This file defines the TMV TriMatrix class.
//
// Constructors:
//
//    There are two TriMatrix classes: UpperTriMatrix<T> and LowerTriMatrix<T>
//    For these notes, I will just write TriMatrix, but for all uses,
//    you need to write "Upper" or "Lower" before the "Tri".
//
//    In addition to the type template parameter (T), TriMatrixes have two
//    additional template parameters:
//        DiagType dt = UnitDiag || NonUnitDiag 
//        StorageType stor = RowMajor || ColMajor
//
//        They both have default values, so you can omit both, or
//        just stor.  The default values are: {NonUnitDiag, RowMajor}
//
//        If dt is UnitDiag, then the diagonal elements are not
//        actually stored or referenced.  The are all taken to be = 1.
//
//        The storage follows the same meaning as for regular Matrices.
//
//    TriMatrix<T,dt,stor>(size_t n)
//        Makes a Triangular Matrix with column size = row size = n
//        with _uninitialized_ values.
//
//    TriMatrix<T,dt,stor>(size_t n, T x)
//        Makes a Triangular Matrix with column size = row size = n
//        with all values = x
//
//    TriMatrix<T,dt,stor>(size_t n, T* vv)
//    TriMatrix<T,dt,stor>(size_t n, const std::vector<T>& vv)
//        Makes a Triangular Matrix with column size = row size = n
//        which copies the values from vv.
//
//    TriMatrix<T,dt,stor>(const Matrix<T>& m)
//    TriMatrix<T,dt,stor>(const TriMatrix<T>& m)
//        Makes a TriMatrix which copies the corresponding elements of m.
//
//    ConstUpperTriMatrixView<T> UpperTriMatrixViewOf(
//        const T* m, size, dt, stor)
//    UpperTriMatrixView<T> UpperTriMatrixViewOf(T* m, size, dt, stor)
//    ConstLowerTriMatrixView<T> LowerTriMatrixViewOf(
//        const T* m, size, dt, stor)
//    LowerTriMatrixView<T> LowerTriMatrixViewOf(T* m, size, dt, stor)
//        Make a TriMatrix view of the specific location in memory
//        specified by m.  The size, diagtype and storage are parameters.
//
// Access Functions
//
//    size_t colsize() const
//    size_t rowsize() const
//    size_t size() const
//        Return the dimensions of the TriMatrix
//
//    T& operator()(int i, int j)
//    T operator()(int i, int j) const
//        Return the (i,j) element of the TriMatrix
//
//    Vector& row(int i, int j1, int j2)
//        Return a portion of the ith row 
//        This range must be a valid range for the requested row.
//
//    Vector& col(int j, int i1, int i2)
//        Return a portion of the jth column
//        This range must be a valid range for the requested column.
//
//    Vector& diag()
//        Return the main diagonal
//        The TriMatrix must be NonUnitDiag.
//
//    Vector& diag(int i, int j1, int j2)
//    Vector& diag(int i)
//        Return the super- or sub-diagonal i
//        If i > 0 return the super diagonal starting at m_0i
//        If i < 0 return the sub diagonal starting at m_|i|0
//        If j1,j2 are given, it returns the diagonal subVector 
//        either from m_j1,i+j1 to m_j2,i+j2 (for i>0) 
//        or from m_|i|+j1,j1 to m_|i|+j2,j2 (for i<0)
//        i>0 will give an error for a LowerTriMatrix
//        i<0 will give an error for an UpperTriMatrix
//        i=0 will give an error for a UnitDiag TriMatrix
//
// Modifying Functions:
//
//    setZero()
//    setAllTo(T x)
//    conjugateSelf()
//    setToIdentity(x = 1)
//    void Swap(TriMatrix& m1, TriMatrix& m2)
//        The TriMatrices must be the same size and shape (Upper or Lower).
//
// Views of a TriMatrix:
//
//    subMatrix(int i1, int i2, int j1, int j2, int istep=1, int jstep=1)
//        This member function will return a submatrix using rows i1 to i2
//        and columns j1 to j2 which refers
//        to the same physical elements as the original.
//        The submatrix must be completely contained within the TriMatrix.
//
//    subVector(int i, int j, int istep, int jstep, int size)
//        Returns a subVector which starts at position (i,j) in the 
//        matrix, moves in the directions (istep,jstep) and has a length
//        of size.
//
//    subTriMatrix(int i1, int i2, int istep)
//        Returns the TriMatrix which runs from i1 to i2 along the diagonal
//        (not including i2) with an optional step, and includes the 
//        off diagonal in the same rows/cols.
//
//        For example, with an UpperTriMatrix of size 10, the x's below
//        are the original data, the O's are the subTriMatrix returned
//        with the command subTriMatrix(3,11,2), and the #'s are the 
//        subTriMatrix returned with subTriMatrix(0,3)
//
//        ###xxxxxxx
//         ##xxxxxxx
//          #xxxxxxx
//           OxOxOxO
//            xxxxxx
//             OxOxO
//              xxxx
//               OxO
//                xx
//                 O
//
//    offDiag()
//        Returns the (NonUnitDiag) TriMatrix of all the off-diagonal
//        elements of a TriMatrix.
//
//    realPart(), imagPart()
//        For a complex TriMatrix, returns the real or imaginary part
//        as a real TriMatrix.
//
//    view()
//    transpose()
//    adjoint()
//        Note that the transpose or adjoint of an UpperTriMatrix returns 
//        a view which is a LowerTriMatrix, and vice versa.
//    conjugate()
//
//    viewAsUnitDiag()
//        Returns a UnitDiag view of a TriMatrix.
//        i.e. a TriMatrix of the same elements, but treating the diagonl
//        as all 1's.
//
//
// Functions of Matrixs:
//
//    m.det() or Det(m)
//    m.logDet() or m.logDet(T* sign) or LogDet(m)
//    m.trace() or Trace(m)
//    m.norm() or m.normF() or Norm(m) or NormF(m)
//    m.normSq() or NormSq(m)
//    m.norm1() or Norm1(m)
//    m.norm2() or Norm2(m)
//    m.normInf() or NormInf(m)
//    m.maxAbsElement() or MaxAbsElements(m)
//
//    m.inverse() or Inverse(m)
//    m.makeInverse(minv) // Takes either a TriMatrix or Matrix argument
//    m.makeInverseATA(invata)
//
//    m.newTranspose()
//    m.newConjugate()
//    m.newAdjoint()
//    m.newInverse()
//    m.newCopy()
//
//
// I/O: 
//
//    os << m 
//        Writes m to ostream os in the usual Matrix format
//
//    m.writeCompact(os)
//        Writes m to ostream os in the following compact format:
//        For an UpperTriMatrix:
//          size 
//          ( m(0,0) m(0,1) ... m(0,size) )
//          ( m(1,1) .. m(1,size) )
//          ...
//          ( m(size,size) )
//
//        For a LowerTriMatrix:
//          size 
//          ( m(0,0) )
//          ( m(1,0) m(1,1) )
//          ...
//          ( m(size,0) ... m(size,size) )
//
//    is >> m
//        reads m from istream is in the compact format
//        m must already be the correct size for this to work.
//
//    is >> mptr
//        If you do not know the size of the TriMatrix to be read, you can
//        use this form where mptr is an auto_ptr to an undefined TriMatrix.
//        (Note: if the DiagType for the TriMatrix is UnitDiag, then
//        all of the diagonals read in must be = 1.)
//
//
// Division Control Functions:
//
//    Most of the point of using TriMatrixes is that they are easy
//    to divide using either forward substitution or back substitution.
//    Therefore, the only division available for TriMatrixes is 
//    this variety.  To do something else (like SVD), you need to 
//    copy it to a regular matrix.
//


#ifndef TMV_TriMatrix_H
#define TMV_TriMatrix_H

#include "tmv/TMV_BaseTriMatrix.h"
#include "tmv/TMV_BaseDiagMatrix.h"
#include "tmv/TMV_Vector.h"
#include "tmv/TMV_Matrix.h"
#include "tmv/TMV_DiagMatrix.h"
#include <vector>

namespace tmv {

    template <class T> 
    class GenUpperTriMatrix : 
        virtual public AssignableToUpperTriMatrix<T>,
        public BaseMatrix<T>
    {
        typedef TMV_RealType(T) RT;
        typedef TMV_ComplexType(T) CT;
        typedef GenUpperTriMatrix<T> type;
        typedef ConstVectorView<T> const_vec_type;
        typedef ConstMatrixView<T> const_rec_type;
        typedef ConstUpperTriMatrixView<T> const_uppertri_type;
        typedef ConstLowerTriMatrixView<T> const_lowertri_type;
        typedef ConstUpperTriMatrixView<RT> const_real_type;
        typedef UpperTriMatrixView<T> nonconst_type;

    public:

        //
        // Constructors
        //

        inline GenUpperTriMatrix() {}
        inline GenUpperTriMatrix(const type&) {}
        virtual inline ~GenUpperTriMatrix() {}

        //
        // Access Functions
        //

        using AssignableToUpperTriMatrix<T>::size;
        using AssignableToUpperTriMatrix<T>::dt;
        inline size_t colsize() const { return size(); }
        inline size_t rowsize() const { return size(); }

        inline T operator()(int i, int j) const
        {
            TMVAssert(i>=0 && i<int(size()));
            TMVAssert(j>=0 && j<int(size()));
            if (i>j) return T(0);
            else if (isunit() && i==j) return T(1);
            else {
                TMVAssert(okij(i,j));
                return cref(i,j);
            }
        }

        inline const_vec_type row(int i, int j1, int j2) const 
        { 
            TMVAssert(i>=0 && i<int(size()));
            TMVAssert(j1>=0 && j1<=j2 && j2<=int(size()));
            TMVAssert(j1==j2 || okij(i,j1));
            return const_vec_type(
                cptr()+i*stepi()+j1*stepj(),j2-j1,stepj(),ct()); 
        }

        inline const_vec_type col(int j, int i1, int i2) const
        {
            TMVAssert(j>=0 && j<int(size()));
            TMVAssert(i1>=0 && i1<=i2 && i2<=int(size()));
            TMVAssert(i1==i2 || okij(i2-1,j));
            return const_vec_type(
                cptr()+i1*stepi()+j*stepj(),i2-i1,stepi(),ct()); 
        }

        inline const_vec_type diag() const
        {
            TMVAssert(!isunit());
            return const_vec_type(cptr(),size(),stepi()+stepj(),ct()); 
        }

        inline const_vec_type diag(int i) const
        {
            TMVAssert(isunit() ? i>0 : i>=0);
            TMVAssert(i<=int(size())); 
            return const_vec_type(
                cptr()+i*stepj(),size()-i,stepi()+stepj(),ct()); 
        }

        inline const_vec_type diag(int i, int j1, int j2) const
        {
            TMVAssert(isunit() ? i>0 : i>=0);
            TMVAssert(i<=int(size())); 
            TMVAssert(j1>=0 && j1 <= j2 && j2 <= int(size())-i);
            const int ds = stepi()+stepj();
            return const_vec_type(cptr()+i*stepj()+j1*ds,j2-j1,ds,ct());
        }

        template <class T2> 
        inline bool isSameAs(const BaseMatrix<T2>& ) const
        { return false; }

        inline bool isSameAs(const GenUpperTriMatrix<T>& m2) const
        { 
            return 
                this == &m2 || 
                ( cptr()==m2.cptr() && size()==m2.size() && 
                  dt() == m2.dt() && ct() == m2.ct() &&
                  stepi()==m2.stepi() && stepj()==m2.stepj() 
                );
        }

        template <class T2>
        TMV_DEPRECATED(bool SameAs(const BaseMatrix<T2>& m2) const);
        TMV_DEPRECATED(bool SameAs(const GenUpperTriMatrix<T>& m2) const)
        { return isSameAs(m2); }

        inline void assignToM(const MatrixView<RT>& m2) const
        {
            TMVAssert(m2.colsize() == size());
            TMVAssert(m2.rowsize() == size());
            TMVAssert(isReal(T()));
            assignToU(m2.upperTri(dt()));
            if (isunit()) m2.diag().setAllTo(RT(1));
            if (size() > 0) m2.lowerTri().offDiag().setZero();
        }

        inline void assignToM(const MatrixView<CT>& m2) const
        {
            TMVAssert(m2.colsize() == size());
            TMVAssert(m2.rowsize() == size());
            assignToU(m2.upperTri(dt()));
            if (isunit()) m2.diag().setAllTo(T(1));
            if (size() > 0) m2.lowerTri().offDiag().setZero();
        }

        inline void assignToU(const UpperTriMatrixView<RT>& m2) const
        { 
            TMVAssert(m2.size() == size());
            TMVAssert(isunit() || !m2.isunit());
            TMVAssert(isReal(T()));
            if (!isSameAs(m2)) Copy(*this,m2); 
        }

        inline void assignToU(const UpperTriMatrixView<CT>& m2) const
        { 
            TMVAssert(m2.size() == size());
            TMVAssert(isunit() || !m2.isunit());
            if (!isSameAs(m2)) Copy(*this,m2); 
        }

        //
        // subMatrix
        //

        bool hasSubMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const;

        bool hasSubVector(
            int i, int j, int istep, int jstep, int size) const;

        bool hasSubTriMatrix(int i1, int i2, int istep) const;

        inline const_rec_type subMatrix(
            int i1, int i2, int j1, int j2) const
        {
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,1,1));
            return const_rec_type(
                cptr()+i1*stepi()+j1*stepj(),
                i2-i1, j2-j1, stepi(), stepj(), stor(), ct());
        }

        inline const_rec_type subMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        {
            const StorageType newstor =
                iscm() ? (istep == 1 ? ColMajor : NoMajor) :
                isrm() ? (jstep == 1 ? RowMajor : NoMajor) : NoMajor;
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,istep,jstep));
            return const_rec_type(
                cptr()+i1*stepi()+j1*stepj(),
                (i2-i1)/istep,(j2-j1)/jstep,istep*stepi(),jstep*stepj(), 
                newstor, ct());
        }

        inline const_vec_type subVector(
            int i, int j, int istep, int jstep, int size) const
        {
            TMVAssert(hasSubVector(i,j,istep,jstep,size));
            return const_vec_type(
                cptr()+i*stepi()+j*stepj(),size,
                istep*stepi()+jstep*stepj(),ct());
        }

        inline const_uppertri_type subTriMatrix(int i1, int i2) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,1));
            return const_uppertri_type(
                cptr()+i1*(stepi()+stepj()),
                i2-i1,stepi(),stepj(),dt(),stor(),ct());
        }

        inline const_uppertri_type subTriMatrix(
            int i1, int i2, int istep) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,istep));
            return const_uppertri_type(
                cptr()+i1*(stepi()+stepj()),
                (i2-i1)/istep,istep*stepi(),istep*stepj(),dt(),
                istep==1 ? stor() : NoMajor,ct());
        }

        inline const_uppertri_type offDiag(int noff=1) const
        {
            TMVAssert(noff <= int(size()));
            return const_uppertri_type(
                cptr()+noff*stepj(),size()-noff,
                stepi(),stepj(),NonUnitDiag,stor(),ct());
        }

        inline const_real_type realPart() const
        {
            return const_real_type(
                reinterpret_cast<const RT*>(cptr()), size(),
                isReal(T()) ? stepi() : 2*stepi(),
                isReal(T()) ? stepj() : 2*stepj(),
                dt(), isReal(T()) ? stor() : NoMajor, NonConj);
        }

        inline const_real_type imagPart() const
        {
            TMVAssert(isComplex(T()));
            TMVAssert(!isunit());
            // Since Imag of a UnitDiag TriMatrix has 0's on diagonal.
            return const_real_type(
                reinterpret_cast<const RT*>(cptr())+1, size(),
                2*stepi(), 2*stepj(), dt(), NoMajor, NonConj);
        }

        inline const_uppertri_type view() const
        { 
            return const_uppertri_type(
                cptr(),size(),stepi(),stepj(),dt(),stor(),ct());
        }

        inline const_uppertri_type viewAsUnitDiag() const
        { 
            return const_uppertri_type(
                cptr(),size(),stepi(),stepj(),UnitDiag,stor(),ct());
        }

        inline const_lowertri_type transpose() const
        { 
            return const_lowertri_type(
                cptr(),size(),stepj(),stepi(),dt(),TMV_TransOf(stor()),ct());
        }

        inline const_uppertri_type conjugate() const
        { 
            return const_uppertri_type(
                cptr(),size(),stepi(),stepj(),dt(),stor(),TMV_ConjOf(T,ct()));
        }

        inline const_lowertri_type adjoint() const
        { 
            return const_lowertri_type(
                cptr(),size(),stepj(),stepi(),dt(),
                TMV_TransOf(stor()),TMV_ConjOf(T,ct()));
        }

        inline nonconst_type nonConst() const
        {
            const size_t n=size();
            return nonconst_type(
                const_cast<T*>(cptr()),n,
                stepi(),dt(),stor(),ct()
                TMV_FIRSTLAST1(cptr(),row(n-1,n-1,n).end().getP()));
        }


        TMV_DEPRECATED(const_rec_type SubMatrix(
                int i1, int i2, int j1, int j2) const)
        { return subMatrix(i1,i2,j1,j2); }
        TMV_DEPRECATED(const_rec_type SubMatrix(
                int i1, int i2, int j1, int j2, int istep, int jstep) const)
        { return subMatrix(i1,i2,j1,j2,istep,jstep); }
        TMV_DEPRECATED(const_vec_type SubVector(
                int i, int j, int istep, int jstep, int s) const)
        { return subVector(i,j,istep,jstep,s); }
        TMV_DEPRECATED(const_uppertri_type SubTriMatrix(int i1, int i2) const)
        { return subTriMatrix(i1,i2); }
        TMV_DEPRECATED(const_uppertri_type SubTriMatrix(
                int i1, int i2, int istep) const)
        { return subTriMatrix(i1,i2,istep); }
        TMV_DEPRECATED(const_uppertri_type OffDiag(int noff=1) const)
        { return offDiag(noff); }
        TMV_DEPRECATED(const_real_type Real() const)
        { return realPart(); }
        TMV_DEPRECATED(const_real_type Imag() const)
        { return imagPart(); }
        TMV_DEPRECATED(const_uppertri_type View() const)
        { return view(); }
        TMV_DEPRECATED(const_lowertri_type Transpose() const)
        { return transpose(); }
        TMV_DEPRECATED(const_uppertri_type Conjugate() const)
        { return conjugate(); }
        TMV_DEPRECATED(const_lowertri_type Adjoint() const)
        { return adjoint(); }
        TMV_DEPRECATED(nonconst_type NonConst() const)
        { return nonConst(); }


        //
        // Functions of Matrix
        //

        T det() const;

        RT logDet(T* sign=0) const;

        inline T trace() const
        { return isunit() ? T(RT(size())) : diag().sumElements(); }

        RT norm() const
        { return normF(); }

        RT normF() const;

        RT normSq(const RT scale = RT(1)) const;

        RT norm1() const;

        RT doNorm2() const;
        inline RT norm2() const
        {
            TMV_Warning(
                "TriMatrix::norm2 involves performing SVD of a Matrix");
            return doNorm2();
        }

        RT doCondition() const;
        inline RT condition() const
        {
            TMV_Warning(
                "TriMatrix::condition involves performing SVD of a Matrix");
            return doCondition();
        }

        RT normInf() const;

        RT maxAbsElement() const;

        bool isSingular() const { return det() == T(0); }

        template <class T1> 
        void doMakeInverse(const UpperTriMatrixView<T1>& minv) const;
        template <class T1> 
        void doMakeInverse(const MatrixView<T1>& minv) const;
        void doMakeInverseATA(const MatrixView<T>& ata) const;

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
        inline void makeInverse(const UpperTriMatrixView<T1>& minv) const
        {
            TMVAssert(minv.size() == size());
            doMakeInverse(minv);
        }

        QuotXU<T,T> QInverse() const;
        inline QuotXU<T,T> inverse() const
        { return QInverse(); }

        inline void makeInverseATA(const MatrixView<T>& ata) const
        {
            TMVAssert(ata.colsize() == size());
            TMVAssert(ata.rowsize() == size());
            doMakeInverseATA(ata);
        }

        template <class T1, DiagType D, StorageType S, IndexStyle I> 
        inline void makeInverse(UpperTriMatrix<T1,D,S,I>& minv) const
        { 
            TMVAssert(D==NonUnitDiag || isunit());
            makeInverse(minv.view()); 
        }

        template <class T1, StorageType S, IndexStyle I> 
        inline void makeInverse(Matrix<T1,S,I>& minv) const
        { makeInverse(minv.view()); }

        template <StorageType S, IndexStyle I> 
        inline void makeInverseATA(Matrix<T,S,I>& minv) const
        { makeInverseATA(minv.view()); }

        auto_ptr<BaseMatrix<T> > newCopy() const;
        auto_ptr<BaseMatrix<T> > newView() const;
        auto_ptr<BaseMatrix<T> > newTranspose() const;
        auto_ptr<BaseMatrix<T> > newConjugate() const;
        auto_ptr<BaseMatrix<T> > newAdjoint() const;
        auto_ptr<BaseMatrix<T> > newInverse() const;

        typedef QuotXU<T,T> MyQuotXU;
        TMV_DEPRECATED(MyQuotXU Inverse() const)
        { return inverse(); }
        template <class T1>
        TMV_DEPRECATED(void Inverse(const MatrixView<T1>& minv) const);
        template <class T1, StorageType S, IndexStyle I>
        TMV_DEPRECATED(void Inverse(Matrix<T1,S,I>& minv) const);
        template <StorageType S, IndexStyle I>
        TMV_DEPRECATED(void InverseATA(Matrix<T,S,I>& ata) const);
        template <class T1> 
        TMV_DEPRECATED(void Inverse(
                const UpperTriMatrixView<T1>& minv) const);
        template <class T1, DiagType D, StorageType S, IndexStyle I> 
        TMV_DEPRECATED(void Inverse(UpperTriMatrix<T1,D,S,I>& minv) const);


        //
        // I/O
        //

        void writeCompact(std::ostream& os) const;
        void write(std::ostream& os) const;
        void writeCompact(std::ostream& os, RT thresh) const;
        void write(std::ostream& os, RT thresh) const;

        TMV_DEPRECATED(void WriteCompact(std::ostream& os) const)
        { writeCompact(os); }
        TMV_DEPRECATED(void WriteCompact(std::ostream& os, RT thresh) const)
        { writeCompact(os,thresh); }

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
        void doLDivEq(const UpperTriMatrixView<T1>& m) const;
        template <class T1, class T0> 
        void doLDiv(
            const GenUpperTriMatrix<T1>& m1,
            const UpperTriMatrixView<T0>& m0) const;

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
            TMVAssert(v0.size() == size());
            TMVAssert(v1.size() == size());
            doLDiv(v1,v0);
        }
        template <class T1> 
        inline void RDivEq(const VectorView<T1>& v) const
        { transpose().LDivEq(v); }
        template <class T1, class T0> 
        inline void RDiv(
            const GenVector<T1>& v1, const VectorView<T0>& v0) const
        { transpose().LDiv(v1,v0); }

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
            TMVAssert(m0.colsize() == size());
            TMVAssert(m1.colsize() == size());
            TMVAssert(m0.rowsize() == m1.rowsize());
            doLDiv(m1,m0);
        }
        template <class T1> 
        inline void RDivEq(const MatrixView<T1>& m) const
        { transpose().LDivEq(m.transpose()); }
        template <class T1, class T0> 
        inline void RDiv(
            const GenMatrix<T1>& m1, const MatrixView<T0>& m0) const
        { transpose().LDiv(m1.transpose(),m0.transpose()); }

        template <class T1> 
        inline void LDivEq(const UpperTriMatrixView<T1>& m) const
        { 
            TMVAssert(m.colsize() == size());
            doLDivEq(m);
        }
        template <class T1, class T0> 
        inline void LDiv(
            const GenUpperTriMatrix<T1>& m1,
            const UpperTriMatrixView<T0>& m0) const
        { 
            TMVAssert(m0.size() == size());
            TMVAssert(m1.size() == size());
            doLDiv(m1,m0);
        }
        template <class T1> 
        inline void RDivEq(const UpperTriMatrixView<T1>& m) const
        { transpose().LDivEq(m.transpose()); }
        template <class T1, class T0> 
        inline void RDiv(
            const GenUpperTriMatrix<T1>& m1,
            const UpperTriMatrixView<T0>& m0) const
        { transpose().LDiv(m1.transpose(),m0.transpose()); }

        // For easier compatibility with regular matrices:
        inline void divideInPlace() const {}
        inline void saveDiv() const {}
        inline void setDiv() const {}
        inline void unsetDiv() const {}
        inline void resetDiv() const {}
        inline bool divIsSet() const { return true; }
        inline void divideUsing(DivType TMV_DEBUGPARAM(dt)) const
        { TMVAssert(dt == LU); }

        TMV_DEPRECATED(void DivideInPlace() const) {}
        TMV_DEPRECATED(void SaveDiv() const) {}
        TMV_DEPRECATED(void SetDiv() const) {}
        TMV_DEPRECATED(void UnSetDiv() const) {}
        TMV_DEPRECATED(void ReSetDiv() const) {}
        TMV_DEPRECATED(bool DivIsSet() const) { return divIsSet(); }
        TMV_DEPRECATED(void DivideUsing(DivType dt) const) { divideUsing(dt); }
        TMV_DEPRECATED(bool CheckDecomp(std::ostream* fout=0) const)
        { return true; }
        TMV_DEPRECATED(bool CheckDecomp(
                const BaseMatrix<T>& m2, std::ostream* fout=0) const)
        { return true; }

        virtual const T* cptr() const = 0;
        virtual int stepi() const = 0;
        virtual int stepj() const = 0;
        virtual StorageType stor() const = 0;
        virtual ConjType ct() const = 0;
        inline bool isrm() const { return stor() == RowMajor; }
        inline bool iscm() const { return stor() == ColMajor; }
        inline bool isunit() const { return dt() == UnitDiag; }
        inline bool isconj() const
        {
            TMVAssert(isComplex(T()) || ct()==NonConj);
            return isComplex(T()) && ct()==Conj;
        }

    protected :

        inline bool okij(int i, int j) const
        {
            TMVAssert(i>=0 && i < int(size()));
            TMVAssert(j>=0 && j < int(size()));
            if (isunit()) return i<j; else return i<=j;
        }
        virtual T cref(int i, int j) const;

    private :

        type& operator=(const type&);

    }; // GenUpperTriMatrix

    template <class T> 
    template <class T2>
    inline bool GenUpperTriMatrix<T>::SameAs(const BaseMatrix<T2>& m2) const
    { return isSameAs(m2); }

    template <class T> 
    template <class T1>
    inline void GenUpperTriMatrix<T>::Inverse(const MatrixView<T1>& minv) const
    { makeInverse(minv); }

    template <class T> 
    template <class T1, StorageType S, IndexStyle I>
    inline void GenUpperTriMatrix<T>::Inverse(Matrix<T1,S,I>& minv) const
    { makeInverse(minv); }

    template <class T> 
    template <StorageType S, IndexStyle I>
    inline void GenUpperTriMatrix<T>::InverseATA(Matrix<T,S,I>& ata) const
    { makeInverseATA(ata); }

    template <class T> 
    template <class T1> 
    inline void GenUpperTriMatrix<T>::Inverse(
        const UpperTriMatrixView<T1>& minv) const
    { makeInverse(minv); }

    template <class T> 
    template <class T1, DiagType D, StorageType S, IndexStyle I> 
    inline void GenUpperTriMatrix<T>::Inverse(
        UpperTriMatrix<T1,D,S,I>& minv) const
    { makeInverse(minv); }

    template <class T> 
    class GenLowerTriMatrix : 
        virtual public AssignableToLowerTriMatrix<T>,
        public BaseMatrix<T>
    {
        typedef TMV_RealType(T) RT;
        typedef TMV_ComplexType(T) CT;
        typedef GenLowerTriMatrix<T> type;
        typedef ConstVectorView<T> const_vec_type;
        typedef ConstMatrixView<T> const_rec_type;
        typedef ConstUpperTriMatrixView<T> const_uppertri_type;
        typedef ConstLowerTriMatrixView<T> const_lowertri_type;
        typedef ConstLowerTriMatrixView<RT> const_real_type;
        typedef LowerTriMatrixView<T> nonconst_type;

    public:

        //
        // Constructors
        //

        inline GenLowerTriMatrix() {}
        inline GenLowerTriMatrix(const type&) {}
        virtual inline ~GenLowerTriMatrix() {}

        //
        // Access Functions
        //

        using AssignableToLowerTriMatrix<T>::size;
        using AssignableToLowerTriMatrix<T>::dt;
        inline size_t colsize() const { return size(); }
        inline size_t rowsize() const { return size(); }

        inline T operator()(int i, int j) const
        {
            TMVAssert(i>=0 && i<int(size()));
            TMVAssert(j>=0 && j<int(size()));
            if (i<j) return T(0);
            else if (isunit() && i==j) return T(1);
            else {
                TMVAssert(okij(i,j));
                return cref(i,j);
            }
        }

        inline const_vec_type row(int i, int j1, int j2) const 
        { 
            TMVAssert(i>=0 && i<int(size()));
            TMVAssert(j1>=0 && j1<=j2 && j2<=int(size()));
            TMVAssert(j1==j2 || okij(i,j2-1));
            return const_vec_type(
                cptr()+i*stepi()+j1*stepj(),j2-j1,stepj(),ct()); 
        }

        inline const_vec_type col(int j, int i1, int i2) const
        {
            TMVAssert(j>=0 && j<int(size()));
            TMVAssert(i1>=0 && i1<=i2 && i2<=int(size()));
            TMVAssert(i1==i2 || okij(i1,j));
            return const_vec_type(
                cptr()+i1*stepi()+j*stepj(),i2-i1,stepi(),ct()); 
        }

        inline const_vec_type diag() const
        {
            TMVAssert(!isunit());
            return const_vec_type(cptr(),size(),stepi()+stepj(),ct()); 
        }

        inline const_vec_type diag(int i) const
        {
            TMVAssert(i>=-int(size())); 
            TMVAssert(isunit() ? i<0 : i<=0);
            return const_vec_type(
                cptr()-i*stepi(),size()-i,stepi()+stepj(),ct());
        }

        inline const_vec_type diag(int i, int j1, int j2) const
        {
            TMVAssert(i>=-int(size())); 
            TMVAssert(isunit() ? i<0 : i<=0);
            TMVAssert(j1>=0 && j1 <= j2 && j2 <= int(size())+i);
            const int ds = stepi()+stepj();
            return const_vec_type(cptr()-i*stepi()+j1*ds,j2-j1,ds,ct());
        }

        template <class T2> 
        inline bool isSameAs(const BaseMatrix<T2>& ) const
        { return false; }

        inline bool isSameAs(const GenLowerTriMatrix<T>& m2) const
        { 
            if (this == &m2) return true;
            else return (cptr()==m2.cptr() && size()==m2.size() && 
                         dt() == m2.dt() && ct() == m2.ct() &&
                         stepi()==m2.stepi() && stepj()==m2.stepj());
        }

        template <class T2>
        TMV_DEPRECATED(bool SameAs(const BaseMatrix<T2>& m2) const);
        TMV_DEPRECATED(bool SameAs(const GenLowerTriMatrix<T>& m2) const)
        { return isSameAs(m2); }

        inline void assignToM(const MatrixView<RT>& m2) const
        { transpose().assignToM(m2.transpose()); }
        inline void assignToM(const MatrixView<CT>& m2) const
        { transpose().assignToM(m2.transpose()); }
        inline void assignToL(const LowerTriMatrixView<RT>& m2) const
        { transpose().assignToU(m2.transpose()); }
        inline void assignToL(const LowerTriMatrixView<CT>& m2) const
        { transpose().assignToU(m2.transpose()); }

        //
        // subMatrix
        //

        inline bool hasSubMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        { return transpose().hasSubMatrix(j1,j2,i1,i2,jstep,istep); }

        inline bool hasSubVector(
            int i, int j, int istep, int jstep, int size) const
        { return transpose().hasSubVector(j,i,jstep,istep,size); }

        inline bool hasSubTriMatrix(int i1, int i2, int istep) const
        { return transpose().hasSubTriMatrix(i1,i2,istep); }

        inline const_rec_type subMatrix(
            int i1, int i2, int j1, int j2) const
        {
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,1,1));
            return const_rec_type(
                cptr()+i1*stepi()+j1*stepj(),
                i2-i1, j2-j1, stepi(), stepj(), stor(), ct());
        }

        inline const_rec_type subMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        {
            const StorageType newstor =
                iscm() ? (istep == 1 ? ColMajor : NoMajor) :
                isrm() ? (jstep == 1 ? RowMajor : NoMajor) : NoMajor;
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,istep,jstep));
            return const_rec_type(
                cptr()+i1*stepi()+j1*stepj(),
                (i2-i1)/istep,(j2-j1)/jstep,istep*stepi(),jstep*stepj(), 
                newstor,ct());
        }

        inline const_vec_type subVector(
            int i, int j, int istep, int jstep, int size) const
        {
            TMVAssert(hasSubVector(i,j,istep,jstep,size));
            return const_vec_type(
                cptr()+i*stepi()+j*stepj(),size,
                istep*stepi()+jstep*stepj(),ct());
        }

        inline const_lowertri_type subTriMatrix(int i1, int i2) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,1));
            return const_lowertri_type(
                cptr()+i1*(stepi()+stepj()),
                i2-i1,stepi(),stepj(),dt(),stor(),ct());
        }

        inline const_lowertri_type subTriMatrix(
            int i1, int i2, int istep) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,istep));
            return const_lowertri_type(
                cptr()+i1*(stepi()+stepj()),
                (i2-i1)/istep,istep*stepi(),istep*stepj(),dt(),
                istep==1 ? stor() : NoMajor,ct());
        }

        inline const_lowertri_type offDiag(int noff=1) const
        {
            TMVAssert(noff <= int(size()));
            return const_lowertri_type(
                cptr()+noff*stepi(),size()-noff,
                stepi(),stepj(),NonUnitDiag,stor(),ct());
        }

        inline const_real_type realPart() const
        {
            return const_real_type(
                reinterpret_cast<const RT*>(cptr()), size(),
                isReal(T()) ? stepi() : 2*stepi(),
                isReal(T()) ? stepj() : 2*stepj(),
                dt(), isReal(T()) ? stor() : NoMajor, NonConj);
        }

        inline const_real_type imagPart() const
        {
            TMVAssert(isComplex(T()));
            TMVAssert(!isunit());
            return const_real_type(
                reinterpret_cast<const RT*>(cptr())+1, size(),
                2*stepi(), 2*stepj(), dt(), NoMajor, NonConj);
        }

        inline const_lowertri_type view() const
        { 
            return const_lowertri_type(
                cptr(),size(),
                stepi(),stepj(),dt(),stor(),ct());
        }

        inline const_lowertri_type viewAsUnitDiag() const
        { 
            return const_lowertri_type(
                cptr(),size(),
                stepi(),stepj(),UnitDiag,stor(),ct());
        }

        inline const_uppertri_type transpose() const
        { 
            return const_uppertri_type(
                cptr(),size(),
                stepj(),stepi(),dt(),TMV_TransOf(stor()),ct());
        }

        inline const_lowertri_type conjugate() const
        { 
            return const_lowertri_type(
                cptr(),size(),
                stepi(),stepj(),dt(),stor(),TMV_ConjOf(T,ct()));
        }

        inline const_uppertri_type adjoint() const
        { 
            return const_uppertri_type(
                cptr(),size(),
                stepj(),stepi(),dt(),TMV_TransOf(stor()),TMV_ConjOf(T,ct()));
        }

        inline nonconst_type nonConst() const
        {
            const size_t n=size();
            return nonconst_type(
                const_cast<T*>(cptr()),n,
                stepi(),dt(),stor(),ct()
                TMV_FIRSTLAST1(cptr(),row(n-1,n-1,n).end().getP()));
        }

        TMV_DEPRECATED(const_rec_type SubMatrix(
                int i1, int i2, int j1, int j2) const)
        { return subMatrix(i1,i2,j1,j2); }
        TMV_DEPRECATED(const_rec_type SubMatrix(
                int i1, int i2, int j1, int j2, int istep, int jstep) const)
        { return subMatrix(i1,i2,j1,j2,istep,jstep); }
        TMV_DEPRECATED(const_vec_type SubVector(
                int i, int j, int istep, int jstep, int s) const)
        { return subVector(i,j,istep,jstep,s); }
        TMV_DEPRECATED(const_lowertri_type SubTriMatrix(int i1, int i2) const)
        { return subTriMatrix(i1,i2); }
        TMV_DEPRECATED(const_lowertri_type SubTriMatrix(
                int i1, int i2, int istep) const)
        { return subTriMatrix(i1,i2,istep); }
        TMV_DEPRECATED(const_lowertri_type OffDiag(int noff=1) const)
        { return offDiag(noff); }
        TMV_DEPRECATED(const_real_type Real() const)
        { return realPart(); }
        TMV_DEPRECATED(const_real_type Imag() const)
        { return imagPart(); }
        TMV_DEPRECATED(const_lowertri_type View() const)
        { return view(); }
        TMV_DEPRECATED(const_uppertri_type Transpose() const)
        { return transpose(); }
        TMV_DEPRECATED(const_lowertri_type Conjugate() const)
        { return conjugate(); }
        TMV_DEPRECATED(const_uppertri_type Adjoint() const)
        { return adjoint(); }
        TMV_DEPRECATED(nonconst_type NonConst() const)
        { return nonConst(); }


        //
        // Functions of Matrix
        //

        T det() const
        { return transpose().det(); }

        RT logDet(T* sign=0) const
        { return transpose().logDet(sign); }

        inline T trace() const
        { return isunit() ? T(RT(size())) : diag().sumElements(); }

        inline RT norm() const 
        { return transpose().normF(); }

        inline RT normF() const 
        { return transpose().normF(); }

        inline RT normSq(const RT scale = RT(1)) const
        { return transpose().normSq(scale); }

        inline RT norm1() const
        { return transpose().normInf(); }

        inline RT doNorm2() const
        { return transpose().doNorm2(); }

        inline RT doCondition() const
        { return transpose().doCondition(); }

        inline RT norm2() const
        { return transpose().norm2(); }

        inline RT condition() const
        { return transpose().condition(); }

        inline RT normInf() const
        { return transpose().norm1(); }

        inline RT maxAbsElement() const
        { return transpose().maxAbsElement(); }

        bool isSingular() const { return det() == T(0); }

        QuotXL<T,T> QInverse() const;
        inline QuotXL<T,T> inverse() const
        { return QInverse(); }

        template <class T1> 
        inline void makeInverse(const LowerTriMatrixView<T1>& minv) const
        {
            TMVAssert(minv.size() == size());
            transpose().makeInverse(minv.transpose());
        }

        inline void makeInverse(const MatrixView<T>& minv) const
        {
            TMVAssert(minv.colsize() == size());
            TMVAssert(minv.rowsize() == size());
            transpose().makeInverse(minv.transpose());
        }

        template <class T1> 
        inline void makeInverse(const MatrixView<T1>& minv) const
        {
            TMVAssert(minv.colsize() == size());
            TMVAssert(minv.rowsize() == size());
            transpose().makeInverse(minv.transpose());
        }

        void doMakeInverseATA(const MatrixView<T>& minv) const;

        inline void makeInverseATA(const MatrixView<T>& ata) const
        {
            TMVAssert(ata.colsize() == size());
            TMVAssert(ata.rowsize() == size());
            doMakeInverseATA(ata);
        }

        template <class T1, DiagType D, StorageType S, IndexStyle I> 
        inline void makeInverse(LowerTriMatrix<T1,D,S,I>& minv) const
        { 
            TMVAssert(D==NonUnitDiag || isunit());
            makeInverse(minv.view()); 
        }

        template <class T1, StorageType S, IndexStyle I> 
        inline void makeInverse(Matrix<T1,S,I>& minv) const
        { makeInverse(minv.view()); }

        template <StorageType S, IndexStyle I> 
        inline void makeInverseATA(Matrix<T,S,I>& minv) const
        { makeInverseATA(minv.view()); }

        auto_ptr<BaseMatrix<T> > newCopy() const;
        auto_ptr<BaseMatrix<T> > newView() const;
        auto_ptr<BaseMatrix<T> > newTranspose() const ;
        auto_ptr<BaseMatrix<T> > newConjugate() const;
        auto_ptr<BaseMatrix<T> > newAdjoint() const;
        auto_ptr<BaseMatrix<T> > newInverse() const;

        typedef QuotXL<T,T> MyQuotXL;
        TMV_DEPRECATED(MyQuotXL Inverse() const)
        { return inverse(); }
        template <class T1>
        TMV_DEPRECATED(void Inverse(const MatrixView<T1>& minv) const);
        template <class T1, StorageType S, IndexStyle I>
        TMV_DEPRECATED(void Inverse(Matrix<T1,S,I>& minv) const);
        template <StorageType S, IndexStyle I>
        TMV_DEPRECATED(void InverseATA(Matrix<T,S,I>& ata) const);
        template <class T1> 
        TMV_DEPRECATED(void Inverse(
                const LowerTriMatrixView<T1>& minv) const);
        template <class T1, DiagType D, StorageType S, IndexStyle I> 
        TMV_DEPRECATED(void Inverse(LowerTriMatrix<T1,D,S,I>& minv) const);


        //
        // I/O
        //

        void writeCompact(std::ostream& os) const;
        void write(std::ostream& os) const;
        void writeCompact(std::ostream& os, RT thresh) const;
        void write(std::ostream& os, RT thresh) const;

        TMV_DEPRECATED(void WriteCompact(std::ostream& os) const)
        { writeCompact(os); }
        TMV_DEPRECATED(void WriteCompact(std::ostream& os, RT thresh) const)
        { writeCompact(os,thresh); }

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
        void doLDivEq(const LowerTriMatrixView<T1>& m) const;
        template <class T1, class T0> 
        void doLDiv(
            const GenLowerTriMatrix<T1>& m1,
            const LowerTriMatrixView<T0>& m0) const;

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
            TMVAssert(v0.size() == size());
            TMVAssert(v1.size() == size());
            doLDiv(v1,v0);
        }
        template <class T1> 
        inline void RDivEq(const VectorView<T1>& v) const
        { transpose().LDivEq(v); }
        template <class T1, class T0> 
        inline void RDiv(
            const GenVector<T1>& v1, const VectorView<T0>& v0) const
        { transpose().LDiv(v1,v0); }

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
            TMVAssert(m0.colsize() == size());
            TMVAssert(m1.colsize() == size());
            TMVAssert(m0.rowsize() == m1.rowsize());
            doLDiv(m1,m0);
        }
        template <class T1> 
        inline void RDivEq(const MatrixView<T1>& m) const
        { transpose().LDivEq(m.transpose()); }
        template <class T1, class T0> 
        inline void RDiv(
            const GenMatrix<T1>& m1, const MatrixView<T0>& m0) const
        { transpose().LDiv(m1.transpose(),m0.transpose()); }

        template <class T1> 
        inline void LDivEq(const LowerTriMatrixView<T1>& m) const
        { 
            TMVAssert(m.colsize() == size());
            doLDivEq(m);
        }
        template <class T1, class T0> 
        inline void LDiv(
            const GenLowerTriMatrix<T1>& m1,
            const LowerTriMatrixView<T0>& m0) const
        { 
            TMVAssert(m0.size() == size());
            TMVAssert(m1.size() == size());
            doLDiv(m1,m0);
        }
        template <class T1> 
        inline void RDivEq(const LowerTriMatrixView<T1>& m) const
        { transpose().LDivEq(m.transpose()); }
        template <class T1, class T0> 
        inline void RDiv(
            const GenLowerTriMatrix<T1>& m1,
            const LowerTriMatrixView<T0>& m0) const
        { transpose().LDiv(m1.transpose(),m0.transpose()); }


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

        TMV_DEPRECATED(void DivideInPlace() const) {}
        TMV_DEPRECATED(void SaveDiv() const) {}
        TMV_DEPRECATED(void SetDiv() const) {}
        TMV_DEPRECATED(void UnSetDiv() const) {}
        TMV_DEPRECATED(void ReSetDiv() const) {}
        TMV_DEPRECATED(bool DivIsSet() const) { return divIsSet(); }
        TMV_DEPRECATED(void DivideUsing(DivType dt) const) { divideUsing(dt); }
        TMV_DEPRECATED(bool CheckDecomp(std::ostream* fout=0) const)
        { return true; }
        TMV_DEPRECATED(bool CheckDecomp(
                const BaseMatrix<T>& m2, std::ostream* fout=0) const)
        { return true; }

        virtual const T* cptr() const = 0;
        virtual int stepi() const = 0;
        virtual int stepj() const = 0;
        virtual StorageType stor() const = 0;
        virtual ConjType ct() const = 0;
        inline bool isrm() const { return stor() == RowMajor; }
        inline bool iscm() const { return stor() == ColMajor; }
        inline bool isunit() const { return dt() == UnitDiag; }
        inline bool isconj() const
        {
            TMVAssert(isComplex(T()) || ct()==NonConj);
            return isComplex(T()) && ct()==Conj;
        }

    protected :

        inline bool okij(int i, int j) const
        {
            TMVAssert(i>=0 && i < int(size()));
            TMVAssert(j>=0 && j < int(size()));
            if (isunit()) return i>j; else return i>=j;
        }

        virtual T cref(int i, int j) const;

    private :

        type& operator=(const type&);

    }; // GenLowerTriMatrix

    template <class T> 
    template <class T2>
    inline bool GenLowerTriMatrix<T>::SameAs(const BaseMatrix<T2>& m2) const
    { return isSameAs(m2); }

    template <class T> 
    template <class T1>
    inline void GenLowerTriMatrix<T>::Inverse(const MatrixView<T1>& minv) const
    { makeInverse(minv); }

    template <class T> 
    template <class T1, StorageType S, IndexStyle I>
    inline void GenLowerTriMatrix<T>::Inverse(Matrix<T1,S,I>& minv) const
    { makeInverse(minv); }

    template <class T> 
    template <StorageType S, IndexStyle I>
    inline void GenLowerTriMatrix<T>::InverseATA(Matrix<T,S,I>& ata) const
    { makeInverseATA(ata); }

    template <class T> 
    template <class T1> 
    inline void GenLowerTriMatrix<T>::Inverse(
        const LowerTriMatrixView<T1>& minv) const
    { makeInverse(minv); }

    template <class T> 
    template <class T1, DiagType D, StorageType S, IndexStyle I> 
    inline void GenLowerTriMatrix<T>::Inverse(
        LowerTriMatrix<T1,D,S,I>& minv) const
    { makeInverse(minv); }


    template <class T, IndexStyle I> 
    class ConstUpperTriMatrixView : public GenUpperTriMatrix<T>
    {
        typedef GenUpperTriMatrix<T> base;
        typedef ConstUpperTriMatrixView<T,I> type;
    public :

        inline ConstUpperTriMatrixView(const type& rhs) :
            itsm(rhs.itsm), itss(rhs.itss), itssi(rhs.itssi), itssj(rhs.itssj),
            itsdiag(rhs.itsdiag), itsstor(rhs.itsstor), itsct(rhs.itsct) {}

        inline ConstUpperTriMatrixView(const base& rhs) :
            itsm(rhs.cptr()), itss(rhs.size()), 
            itssi(rhs.stepi()), itssj(rhs.stepj()),
            itsdiag(rhs.dt()), itsstor(rhs.stor()), itsct(rhs.ct()) {}

        inline ConstUpperTriMatrixView(
            const T* _m, size_t _s, int _si, int _sj,
            DiagType _dt, StorageType _stor, ConjType _ct) : 
            itsm(_m), itss(_s), itssi(_si), itssj(_sj),
            itsdiag(_dt), itsstor(_stor), itsct(_ct)
        { 
            TMVAssert(_stor==RowMajor ? _sj == 1 : _stor==ColMajor ?
                      _si==1 : true);
        }

        virtual inline ~ConstUpperTriMatrixView() 
        {
#ifdef TMVDEBUG
            const_cast<const T*&>(itsm) = 0;
#endif
        }

        inline size_t size() const { return itss; }
        inline const T* cptr() const { return itsm; }
        inline int stepi() const { return itssi; }
        inline int stepj() const { return itssj; }
        inline StorageType stor() const { return itsstor; }
        inline DiagType dt() const { return itsdiag; }
        inline ConjType ct() const { return itsct; }

    protected :

        const T*const itsm;
        const size_t itss;
        const int itssi;
        const int itssj;
        DiagType itsdiag;
        StorageType itsstor;
        ConjType itsct;

    private :

        type& operator=(const type&);

    }; // ConstUpperTriMatrixView

    template <class T, IndexStyle I> 
    class ConstLowerTriMatrixView : public GenLowerTriMatrix<T>
    {
        typedef GenLowerTriMatrix<T> base;
        typedef ConstLowerTriMatrixView<T,I> type;
    public :

        inline ConstLowerTriMatrixView(const type& rhs) :
            itsm(rhs.itsm), itss(rhs.itss), itssi(rhs.itssi), itssj(rhs.itssj),
            itsdiag(rhs.itsdiag), itsstor(rhs.itsstor), itsct(rhs.itsct) {}

        inline ConstLowerTriMatrixView(const GenLowerTriMatrix<T>& rhs) :
            itsm(rhs.cptr()), itss(rhs.size()), 
            itssi(rhs.stepi()), itssj(rhs.stepj()),
            itsdiag(rhs.dt()), itsstor(rhs.stor()), itsct(rhs.ct()) {}

        inline ConstLowerTriMatrixView(
            const T* _m, size_t _s, int _si, int _sj,
            DiagType _dt, StorageType _stor, ConjType _ct) : 
            itsm(_m), itss(_s), itssi(_si), itssj(_sj),
            itsdiag(_dt), itsstor(_stor), itsct(_ct)
        { 
            TMVAssert(_stor==RowMajor ? _sj == 1 : _stor==ColMajor ?
                      _si==1 : true);
        }

        virtual inline ~ConstLowerTriMatrixView()
        {
#ifdef TMVDEBUG
            const_cast<const T*&>(itsm) = 0;
#endif
        }

        inline size_t size() const { return itss; }
        inline const T* cptr() const { return itsm; }
        inline int stepi() const { return itssi; }
        inline int stepj() const { return itssj; }
        inline StorageType stor() const { return itsstor; }
        inline DiagType dt() const { return itsdiag; }
        inline ConjType ct() const { return itsct; }

    protected :

        const T*const itsm;
        const size_t itss;
        const int itssi;
        const int itssj;
        DiagType itsdiag;
        StorageType itsstor;
        ConjType itsct;

    private :

        type& operator=(const type&);

    }; // ConstLowerTriMatrixView

    template <class T> 
    class ConstUpperTriMatrixView<T,FortranStyle> : 
        public ConstUpperTriMatrixView<T,CStyle>
    {
        typedef TMV_RealType(T) RT;
        typedef GenUpperTriMatrix<T> base;
        typedef ConstUpperTriMatrixView<T,FortranStyle> type;
        typedef ConstUpperTriMatrixView<T,CStyle> c_type;
        typedef ConstVectorView<T,FortranStyle> const_vec_type;
        typedef ConstMatrixView<T,FortranStyle> const_rec_type;
        typedef ConstLowerTriMatrixView<T,FortranStyle> const_lowertri_type;
        typedef ConstUpperTriMatrixView<T,FortranStyle> const_uppertri_type;
        typedef ConstUpperTriMatrixView<RT,FortranStyle> const_real_type;
    public :

        inline ConstUpperTriMatrixView(const type& rhs) : c_type(rhs) {}

        inline ConstUpperTriMatrixView(const c_type& rhs) : c_type(rhs) {}

        inline ConstUpperTriMatrixView(const base& rhs) : c_type(rhs) {}

        inline ConstUpperTriMatrixView(
            const T* _m, size_t _s, int _si, int _sj,
            DiagType indt, StorageType instor, ConjType inct
        ) : 
            c_type(_m,_s,_si,_sj,indt,instor,inct) 
        {}

        virtual inline ~ConstUpperTriMatrixView() {}

        //
        // Access Functions
        //

        inline T operator()(int i, int j) const
        {
            TMVAssert(i>0 && i<=int(size()));
            TMVAssert(j>0 && j<=int(size()));
            if (i>j) return T(0);
            else if (isunit() && i==j) return T(1);
            else {
                TMVAssert(okij(i-1,j-1));
                return cref(i-1,j-1);
            }
        }

        inline const_vec_type row(int i, int j1, int j2) const 
        { 
            TMVAssert(i>0 && i<=int(size()));
            TMVAssert(j1>0 && j1<=j2 && j2<=int(size()));
            return base::row(i-1,j1-1,j2);
        }

        inline const_vec_type col(int j, int i1, int i2) const
        {
            TMVAssert(j>0 && j<=int(size()));
            TMVAssert(i1>0 && i1<=i2 && i2<=int(size()));
            return base::col(j-1,i1-1,i2);
        }

        inline const_vec_type diag() const
        { return base::diag(); }

        inline const_vec_type diag(int i) const
        { return base::diag(i); }

        inline const_vec_type diag(int i, int j1, int j2) const
        {
            TMVAssert(j1>0);
            return base::diag(i,j1-1,j2); 
        }

        //
        // subMatrix
        //

        bool hasSubMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const;

        bool hasSubVector(
            int i, int j, int istep, int jstep, int size) const;

        bool hasSubTriMatrix(int i1, int i2, int istep) const;

        inline const_rec_type subMatrix(
            int i1, int i2, int j1, int j2) const
        {
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,1,1));
            return base::subMatrix(i1-1,i2,j1-1,j2);
        }

        inline const_rec_type subMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        {
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,istep,jstep));
            return base::subMatrix(
                i1-1,i2-1+istep,j1-1,j2-1+jstep,istep,jstep);
        }

        inline const_vec_type subVector(
            int i, int j, int istep, int jstep, int size) const
        {
            TMVAssert(hasSubVector(i,j,istep,jstep,size));
            return base::subVector(i-1,j-1,istep,jstep,size);
        }

        inline const_uppertri_type subTriMatrix(int i1, int i2) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,1));
            return base::subTriMatrix(i1-1,i2);
        }

        inline const_uppertri_type subTriMatrix(
            int i1, int i2, int istep) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,istep));
            return base::subTriMatrix(i1-1,i2-1+istep,istep);
        }

        inline const_uppertri_type offDiag(int noff=1) const
        { return base::offDiag(noff); }

        inline const_real_type realPart() const
        { return base::realPart(); }

        inline const_real_type imagPart() const
        { return base::imagPart(); }

        inline const_uppertri_type view() const
        { return base::view(); }

        inline const_uppertri_type viewAsUnitDiag() const
        { return base::viewAsUnitDiag(); }

        inline const_lowertri_type transpose() const
        { return base::transpose(); }

        inline const_uppertri_type conjugate() const
        { return base::conjugate(); }

        inline const_lowertri_type adjoint() const
        { return base::adjoint(); }

        TMV_DEPRECATED(const_rec_type SubMatrix(
                int i1, int i2, int j1, int j2) const)
        { return subMatrix(i1,i2,j1,j2); }
        TMV_DEPRECATED(const_rec_type SubMatrix(
                int i1, int i2, int j1, int j2, int istep, int jstep) const)
        { return subMatrix(i1,i2,j1,j2,istep,jstep); }
        TMV_DEPRECATED(const_vec_type SubVector(
                int i, int j, int istep, int jstep, int s) const)
        { return subVector(i,j,istep,jstep,s); }
        TMV_DEPRECATED(const_uppertri_type SubTriMatrix(int i1, int i2) const)
        { return subTriMatrix(i1,i2); }
        TMV_DEPRECATED(const_uppertri_type SubTriMatrix(
                int i1, int i2, int istep) const)
        { return subTriMatrix(i1,i2,istep); }
        TMV_DEPRECATED(const_uppertri_type OffDiag(int noff=1) const)
        { return offDiag(noff); }
        TMV_DEPRECATED(const_real_type Real() const)
        { return realPart(); }
        TMV_DEPRECATED(const_real_type Imag() const)
        { return imagPart(); }
        TMV_DEPRECATED(const_uppertri_type View() const)
        { return view(); }
        TMV_DEPRECATED(const_lowertri_type Transpose() const)
        { return transpose(); }
        TMV_DEPRECATED(const_uppertri_type Conjugate() const)
        { return conjugate(); }
        TMV_DEPRECATED(const_lowertri_type Adjoint() const)
        { return adjoint(); }

        using c_type::size;
        using base::isunit;

    protected :

        using base::okij;
        using c_type::cref;

    private :

        type& operator=(const type&);

    }; // FortranStyle ConstUpperTriMatrixView

    template <class T> 
    class ConstLowerTriMatrixView<T,FortranStyle> : 
        public ConstLowerTriMatrixView<T,CStyle>
    {
        typedef TMV_RealType(T) RT;
        typedef GenLowerTriMatrix<T> base;
        typedef ConstLowerTriMatrixView<T,FortranStyle> type;
        typedef ConstLowerTriMatrixView<T,CStyle> c_type;
        typedef ConstVectorView<T,FortranStyle> const_vec_type;
        typedef ConstMatrixView<T,FortranStyle> const_rec_type;
        typedef ConstUpperTriMatrixView<T,FortranStyle> const_uppertri_type;
        typedef ConstLowerTriMatrixView<T,FortranStyle> const_lowertri_type;
        typedef ConstLowerTriMatrixView<RT,FortranStyle> const_real_type;
    public :

        inline ConstLowerTriMatrixView(const type& rhs) : c_type(rhs) {}

        inline ConstLowerTriMatrixView(const c_type& rhs) : c_type(rhs) {}

        inline ConstLowerTriMatrixView(const base& rhs) : c_type(rhs) {}

        inline ConstLowerTriMatrixView(
            const T* _m, size_t _s, int _si, int _sj,
            DiagType indt, StorageType instor, ConjType inct
        ) : 
            c_type(_m,_s,_si,_sj,indt,instor,inct) 
        {}

        virtual inline ~ConstLowerTriMatrixView() {}

        //
        // Access Functions
        //

        inline T operator()(int i, int j) const
        {
            TMVAssert(i>0 && i<=int(size()));
            TMVAssert(j>0 && j<=int(size()));
            if (i<j) return T(0);
            else if (isunit() && i==j) return T(1);
            else {
                TMVAssert(okij(i-1,j-1));
                return cref(i-1,j-1);
            }
        }

        inline const_vec_type row(int i, int j1, int j2) const 
        { 
            TMVAssert(i>0 && i<=int(size()));
            TMVAssert(j1>0 && j1<=j2 && j2<=int(size()));
            return base::row(i-1,j1-1,j2);
        }

        inline const_vec_type col(int j, int i1, int i2) const
        {
            TMVAssert(j>0 && j<=int(size()));
            TMVAssert(i1>0 && i1<=i2 && i2<=int(size()));
            return base::col(j-1,i1-1,i2);
        }

        inline const_vec_type diag() const
        { return base::diag(); }

        inline const_vec_type diag(int i) const
        { return base::diag(i); }

        inline const_vec_type diag(int i, int j1, int j2) const
        {
            TMVAssert(j1>0);
            return base::diag(i,j1-1,j2); 
        }

        //
        // subMatrix
        //

        inline bool hasSubMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        { return transpose().hasSubMatrix(j1,j2,i1,i2,jstep,istep); }

        inline bool hasSubVector(
            int i, int j, int istep, int jstep, int size) const
        { return transpose().hasSubVector(j,i,jstep,istep,size); }

        inline bool hasSubTriMatrix(int i1, int i2, int istep) const
        { return transpose().hasSubTriMatrix(i1,i2,istep); }

        inline const_rec_type subMatrix(
            int i1, int i2, int j1, int j2) const
        {
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,1,1));
            return base::subMatrix(i1-1,i2,j1-1,j2);
        }

        inline const_rec_type subMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        {
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,istep,jstep));
            return base::subMatrix(
                i1-1,i2-1+istep,j1-1,j2-1+jstep,istep,jstep);
        }

        inline const_vec_type subVector(
            int i, int j, int istep, int jstep, int size) const
        {
            TMVAssert(hasSubVector(i,j,istep,jstep,size));
            return base::subVector(i-1,j-1,istep,jstep,size);
        }

        inline const_lowertri_type subTriMatrix(int i1, int i2) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,1));
            return base::subTriMatrix(i1-1,i2);
        }

        inline const_lowertri_type subTriMatrix(
            int i1, int i2, int istep) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,istep));
            return base::subTriMatrix(i1-1,i2-1+istep,istep);
        }

        inline const_lowertri_type offDiag(int noff=1) const
        { return base::offDiag(noff); }

        inline const_real_type realPart() const
        { return base::realPart(); }

        inline const_real_type imagPart() const
        { return base::imagPart(); }

        inline const_lowertri_type view() const
        { return base::view(); }

        inline const_lowertri_type viewAsUnitDiag() const
        { return base::viewAsUnitDiag(); }

        inline const_uppertri_type transpose() const
        { return base::transpose(); }

        inline const_lowertri_type conjugate() const
        { return base::conjugate(); }

        inline const_uppertri_type adjoint() const
        { return base::adjoint(); }

        TMV_DEPRECATED(const_rec_type SubMatrix(
                int i1, int i2, int j1, int j2) const)
        { return subMatrix(i1,i2,j1,j2); }
        TMV_DEPRECATED(const_rec_type SubMatrix(
                int i1, int i2, int j1, int j2, int istep, int jstep) const)
        { return subMatrix(i1,i2,j1,j2,istep,jstep); }
        TMV_DEPRECATED(const_vec_type SubVector(
                int i, int j, int istep, int jstep, int s) const)
        { return subVector(i,j,istep,jstep,s); }
        TMV_DEPRECATED(const_lowertri_type SubTriMatrix(int i1, int i2) const)
        { return subTriMatrix(i1,i2); }
        TMV_DEPRECATED(const_lowertri_type SubTriMatrix(
                int i1, int i2, int istep) const)
        { return subTriMatrix(i1,i2,istep); }
        TMV_DEPRECATED(const_lowertri_type OffDiag(int noff=1) const)
        { return offDiag(noff); }
        TMV_DEPRECATED(const_real_type Real() const)
        { return realPart(); }
        TMV_DEPRECATED(const_real_type Imag() const)
        { return imagPart(); }
        TMV_DEPRECATED(const_lowertri_type View() const)
        { return view(); }
        TMV_DEPRECATED(const_uppertri_type Transpose() const)
        { return transpose(); }
        TMV_DEPRECATED(const_lowertri_type Conjugate() const)
        { return conjugate(); }
        TMV_DEPRECATED(const_uppertri_type Adjoint() const)
        { return adjoint(); }

        using c_type::size;
        using base::isunit;

    protected :

        using base::okij;
        using c_type::cref;

    private :

        type& operator=(const type&);

    }; // FortranStyle ConstLowerTriMatrixView

    template <class T, IndexStyle I> 
    class UpperTriMatrixView : public GenUpperTriMatrix<T>
    {
        typedef TMV_RealType(T) RT;
        typedef TMV_ComplexType(T) CT;
        typedef GenUpperTriMatrix<T> base;
        typedef UpperTriMatrixView<T,I> type;
        typedef LowerTriMatrixView<T,I> lowertri_type;
        typedef UpperTriMatrixView<T,I> uppertri_type;
        typedef MatrixView<T,I> rec_type;
        typedef VectorView<T,I> vec_type;
        typedef UpperTriMatrixView<RT,I> real_type;

    public:

        //
        // Constructors
        //

        inline UpperTriMatrixView(const type& rhs) : 
            itsm(rhs.itsm), itss(rhs.itss), itssi(rhs.itssi), itssj(rhs.itssj),
            itsdiag(rhs.itsdiag), itsstor(rhs.itsstor), itsct(rhs.itsct)
            TMV_DEFFIRSTLAST(rhs._first,rhs._last) {}

        inline UpperTriMatrixView(
            T* _m, size_t _s, int _si, int _sj,
            DiagType _dt, StorageType _stor, ConjType _ct
            TMV_PARAMFIRSTLAST(T) 
        ) :
            itsm(_m), itss(_s), itssi(_si), itssj(_sj),
            itsdiag(_dt), itsstor(_stor), itsct(_ct) 
            TMV_DEFFIRSTLAST(_first,_last)
        {
            TMVAssert(_stor==RowMajor ? _sj == 1 :
                      _stor==ColMajor ? _si==1 : true);
        }

        virtual inline ~UpperTriMatrixView() 
        {
#ifdef TMVDEBUG
            const_cast<T*&>(itsm) = 0;
#endif
        }

        //
        // Op=
        //

        inline const type& operator=(const type& m2) const
        {
            TMVAssert(size() == m2.size());
            m2.assignToU(*this);
            return *this; 
        }

        inline const type& operator=(const type& m2)
        {
            TMVAssert(size() == m2.size());
            m2.assignToU(*this);
            return *this; 
        }

        inline const type& operator=(const GenUpperTriMatrix<RT>& m2) const
        { 
            TMVAssert(size() == m2.size());
            m2.assignToU(*this);
            return *this; 
        }

        inline const type& operator=(const GenUpperTriMatrix<CT>& m2) const
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(isComplex(T()));
            m2.assignToU(*this);
            return *this; 
        }

        inline const type& operator=(const GenDiagMatrix<RT>& m2) const
        { 
            TMVAssert(size() == m2.size());
            m2.assignToD(DiagMatrixViewOf(diag()));
            offDiag().setZero();
            return *this; 
        }

        inline const type& operator=(const GenDiagMatrix<CT>& m2) const
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(isComplex(T()));
            m2.assignToD(DiagMatrixViewOf(diag()));
            offDiag().setZero();
            return *this; 
        }

        template <class T2> 
        inline const type& operator=(const GenUpperTriMatrix<T2>& m2) const
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(isReal(T2()) || isComplex(T()));
            TMVAssert(!isunit() || m2.isunit());
            Copy(m2,*this); 
            return *this; 
        }

        inline const type& operator=(const T& x) const 
        { TMVAssert(!isunit() || x==T(1)); return setToIdentity(x); }

        inline const type& operator=(
            const AssignableToUpperTriMatrix<RT>& m2) const
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(!(m2.dt()==NonUnitDiag && dt()==UnitDiag));
            m2.assignToU(view());
            return *this;
        }

        inline const type& operator=(
            const AssignableToUpperTriMatrix<CT>& m2) const
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(isComplex(T()));
            TMVAssert(!(m2.dt()==NonUnitDiag && dt()==UnitDiag));
            m2.assignToU(view());
            return *this;
        }

        //
        // Access
        //

        typedef TMV_RefType(T) reference;

        inline reference operator()(int i,int j) const 
        {
            TMVAssert(i>=0 && i<int(size()));
            TMVAssert(j>=0 && j<int(size()));
            TMVAssert(okij(i,j));
            return ref(i,j); 
        }

        inline vec_type row(int i, int j1, int j2) const 
        { 
            TMVAssert(i>=0 && i<int(size()));
            TMVAssert(j1>=0 && j1<=j2 && j2<=int(size()));
            TMVAssert(j1==j2 || okij(i,j1));
            return vec_type(
                ptr()+i*stepi()+j1*stepj(),j2-j1,stepj(),ct() TMV_FIRSTLAST); 
        }

        inline vec_type col(int j, int i1, int i2) const
        {
            TMVAssert(j>=0 && j<int(size()));
            TMVAssert(i1>=0 && i1<=i2 && i2<=int(size()));
            TMVAssert(i1==i2 || okij(i2-1,j));
            return vec_type(
                ptr()+i1*stepi()+j*stepj(),i2-i1,stepi(),ct() TMV_FIRSTLAST); 
        }

        inline vec_type diag() const
        {
            TMVAssert(!isunit());
            return vec_type(
                ptr(),size(),stepi()+stepj(),ct() TMV_FIRSTLAST); 
        }

        inline vec_type diag(int i) const
        {
            TMVAssert(isunit() ? i>0 : i>=0);
            TMVAssert(i<=int(size())); 
            return vec_type(
                ptr()+i*stepj(),size()-i,(stepi()+stepj()),ct() 
                TMV_FIRSTLAST);
        }

        inline vec_type diag(int i, int j1, int j2) const
        {
            TMVAssert(isunit() ? i>0 : i>=0);
            TMVAssert(i<=int(size())); 
            TMVAssert(j1>=0 && j1 <= j2 && j2 <= int(size())-i);
            const int ds = stepi()+stepj();
            return vec_type(
                ptr()+i*stepj()+j1*ds,j2-j1,ds,ct() TMV_FIRSTLAST);
        }

        //
        // Modifying Functions
        //

        const type& setZero() const;

        const type& setAllTo(const T& x) const;

        const type& clip(RT thresh) const;

        const type& conjugateSelf() const;

        const type& invertSelf() const;

        const type& setToIdentity(const T& x=T(1)) const;

        TMV_DEPRECATED(const type& Zero() const)
        { return setZero(); }
        TMV_DEPRECATED(const type& SetAllTo(const T& x) const)
        { return setAllTo(x); }
        TMV_DEPRECATED(const type& Clip(RT thresh) const)
        { return clip(thresh); }
        TMV_DEPRECATED(const type& ConjugateSelf() const)
        { return conjugateSelf(); }
        TMV_DEPRECATED(const type& InvertSelf() const)
        { return invertSelf(); }
        TMV_DEPRECATED(const type& SetToIdentity(const T& x=T(1)) const)
        { return setToIdentity(x); }


        //
        // subMatrix
        //

        using base::hasSubMatrix;
        using base::hasSubVector;
        using base::hasSubTriMatrix;

        inline rec_type subMatrix(int i1, int i2, int j1, int j2) const
        {
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,1,1));
            return rec_type(
                ptr()+i1*stepi()+j1*stepj(),
                i2-i1, j2-j1, stepi(),stepj(),stor(),ct() TMV_FIRSTLAST );
        }

        inline rec_type subMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        {
            const StorageType newstor =
                iscm() ? (istep == 1 ? ColMajor : NoMajor) :
                isrm() ? (jstep == 1 ? RowMajor : NoMajor) : NoMajor;
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,istep,jstep));
            return rec_type(
                ptr()+i1*stepi()+j1*stepj(),
                (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
                newstor,ct() TMV_FIRSTLAST );
        }

        inline vec_type subVector(
            int i, int j, int istep, int jstep, int size) const
        {
            TMVAssert(hasSubVector(i,j,istep,jstep,size));
            return vec_type(
                ptr()+i*stepi()+j*stepj(),size,
                istep*stepi()+jstep*stepj(),ct() TMV_FIRSTLAST );
        }

        inline uppertri_type subTriMatrix(int i1, int i2) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,1));
            return uppertri_type(
                ptr()+i1*(stepi()+stepj()),i2-i1,
                stepi(),stepj(),dt(),stor(),ct() TMV_FIRSTLAST);
        }

        inline uppertri_type subTriMatrix(int i1, int i2, int istep) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,istep));
            return uppertri_type(
                ptr()+i1*(stepi()+stepj()),
                (i2-i1)/istep,istep*stepi(),istep*stepj(),dt(),
                istep==1 ? stor() : NoMajor,ct() TMV_FIRSTLAST);
        }

        inline uppertri_type offDiag(int noff=1) const
        {
            TMVAssert(noff <= int(size()));
            return uppertri_type(
                ptr()+noff*stepj(),size()-noff,
                stepi(),stepj(),NonUnitDiag,stor(),ct() TMV_FIRSTLAST);
        }

        inline real_type realPart() const
        {
            return real_type(
                reinterpret_cast<RT*>(ptr()), size(),
                isReal(T()) ? stepi() : 2*stepi(),
                isReal(T()) ? stepj() : 2*stepj(),
                dt(), isReal(T()) ? stor() : NoMajor, NonConj
#ifdef TMVFLDEBUG
                ,reinterpret_cast<const RT*>(_first)
                ,reinterpret_cast<const RT*>(_last)
#endif
            );
        }

        inline real_type imagPart() const
        {
            TMVAssert(isComplex(T()));
            TMVAssert(!isunit());
            return real_type(
                reinterpret_cast<RT*>(ptr())+1, size(),
                2*stepi(), 2*stepj(), dt(), NoMajor, NonConj
#ifdef TMVFLDEBUG
                ,reinterpret_cast<const RT*>(_first)+1
                ,reinterpret_cast<const RT*>(_last)+1
#endif
            );
        }

        inline uppertri_type view() const
        { return *this; }

        inline uppertri_type viewAsUnitDiag() const
        { 
            return uppertri_type(
                ptr(),size(),stepi(),stepj(),UnitDiag,stor(),ct() 
                TMV_FIRSTLAST);
        }

        inline lowertri_type transpose() const
        {
            return lowertri_type(
                ptr(),size(),stepj(),stepi(),dt(),TMV_TransOf(stor()),ct() 
                TMV_FIRSTLAST);
        }

        inline uppertri_type conjugate() const
        {
            return uppertri_type(
                ptr(),size(),stepi(),stepj(),dt(),stor(),TMV_ConjOf(T,ct()) 
                TMV_FIRSTLAST);
        }

        inline lowertri_type adjoint() const
        {
            return lowertri_type(
                ptr(),size(),stepj(),stepi(),dt(),
                TMV_TransOf(stor()),TMV_ConjOf(T,ct()) TMV_FIRSTLAST);
        }

        TMV_DEPRECATED(rec_type SubMatrix(
                int i1, int i2, int j1, int j2) const)
        { return subMatrix(i1,i2,j1,j2); }
        TMV_DEPRECATED(rec_type SubMatrix(
                int i1, int i2, int j1, int j2, int istep, int jstep) const)
        { return subMatrix(i1,i2,j1,j2,istep,jstep); }
        TMV_DEPRECATED(vec_type SubVector(
                int i, int j, int istep, int jstep, int s) const)
        { return subVector(i,j,istep,jstep,s); }
        TMV_DEPRECATED(uppertri_type SubTriMatrix(int i1, int i2) const)
        { return subTriMatrix(i1,i2); }
        TMV_DEPRECATED(uppertri_type SubTriMatrix(
                int i1, int i2, int istep) const)
        { return subTriMatrix(i1,i2,istep); }
        TMV_DEPRECATED(uppertri_type OffDiag(int noff=1) const)
        { return offDiag(noff); }
        TMV_DEPRECATED(real_type Real() const)
        { return realPart(); }
        TMV_DEPRECATED(real_type Imag() const)
        { return imagPart(); }
        TMV_DEPRECATED(uppertri_type View() const)
        { return view(); }
        TMV_DEPRECATED(uppertri_type ViewAsUnitDiag() const)
        { return viewAsUnitDiag(); }
        TMV_DEPRECATED(lowertri_type Transpose() const)
        { return transpose(); }
        TMV_DEPRECATED(uppertri_type Conjugate() const)
        { return conjugate(); }
        TMV_DEPRECATED(lowertri_type Adjoint() const)
        { return adjoint(); }


        //
        // I/O
        //

        void read(std::istream& is) const;

        TMV_DEPRECATED(void Read(std::istream& is) const)
        { read(is); }

        inline size_t size() const { return itss; }
        inline const T* cptr() const { return itsm; }
        inline T* ptr() const { return itsm; }
        inline int stepi() const { return itssi; }
        inline int stepj() const { return itssj; }
        using base::isconj;
        using base::isrm;
        using base::iscm;
        using base::isunit;
        inline StorageType stor() const { return itsstor; }
        inline DiagType dt() const { return itsdiag; }
        inline ConjType ct() const { return itsct; }

    protected :

        T*const itsm;
        const size_t itss;
        const int itssi;
        const int itssj;
        DiagType itsdiag;
        StorageType itsstor;
        ConjType itsct;

#ifdef TMVFLDEBUG
    public:
        const T*const _first;
        const T*const _last;
    protected:
#endif

        using base::okij;
        reference ref(int i, int j) const;

    }; // UpperTriMatrixView

    template <class T, IndexStyle I> 
    class LowerTriMatrixView : public GenLowerTriMatrix<T>
    {
        typedef TMV_RealType(T) RT;
        typedef TMV_ComplexType(T) CT;
        typedef GenLowerTriMatrix<T> base;
        typedef LowerTriMatrixView<T,I> type;
        typedef UpperTriMatrixView<T,I> uppertri_type;
        typedef LowerTriMatrixView<T,I> lowertri_type;
        typedef MatrixView<T,I> rec_type;
        typedef VectorView<T,I> vec_type;
        typedef LowerTriMatrixView<RT,I> real_type;

    public:

        //
        // Constructors
        //

        inline LowerTriMatrixView(const type& rhs) : 
            itsm(rhs.itsm), itss(rhs.itss), itssi(rhs.itssi), itssj(rhs.itssj),
            itsdiag(rhs.itsdiag), itsstor(rhs.itsstor), itsct(rhs.itsct)
            TMV_DEFFIRSTLAST(rhs._first,rhs._last) {}

        inline LowerTriMatrixView(
            T* _m, size_t _s, int _si, int _sj,
            DiagType _dt, StorageType _stor, ConjType _ct
            TMV_PARAMFIRSTLAST(T) 
        ) :
            itsm(_m), itss(_s), itssi(_si), itssj(_sj),
            itsdiag(_dt), itsstor(_stor), itsct(_ct) 
            TMV_DEFFIRSTLAST(_first,_last)
        {
            TMVAssert(_stor==RowMajor ? _sj == 1 : _stor==ColMajor ?
                      _si==1 : true);
        }

        virtual inline ~LowerTriMatrixView() 
        {
#ifdef TMVDEBUG
            const_cast<T*&>(itsm) = 0;
#endif
        }

        //
        // Op=
        //

        inline const type& operator=(const type& m2) const
        {
            TMVAssert(size() == m2.size());
            m2.assignToL(*this);
            return *this; 
        }

        inline const type& operator=(const type& m2) 
        {
            TMVAssert(size() == m2.size());
            m2.assignToL(*this);
            return *this; 
        }

        inline const type& operator=(const GenLowerTriMatrix<RT>& m2) const
        {
            TMVAssert(size() == m2.size());
            m2.assignToL(*this);
            return *this; 
        }

        inline const type& operator=(const GenLowerTriMatrix<CT>& m2) const
        {
            TMVAssert(size() == m2.size());
            TMVAssert(isComplex(T()));
            m2.assignToL(*this);
            return *this; 
        }

        inline const type& operator=(const GenDiagMatrix<RT>& m2) const
        { 
            TMVAssert(size() == m2.size());
            transpose() = m2;
            return *this; 
        }

        inline const type& operator=(const GenDiagMatrix<CT>& m2) const
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(isComplex(T()));
            transpose() = m2;
            return *this; 
        }

        template <class T2> 
        inline const type& operator=(const GenLowerTriMatrix<T2>& m2) const
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(!isunit() || m2.isunit());
            Copy(m2.transpose(),transpose()); 
            return *this; 
        }

        inline const type& operator=(const T& x) const 
        { 
            TMVAssert(!isunit()); 
            return setToIdentity(x); 
        }

        inline const type& operator=(
            const AssignableToLowerTriMatrix<RT>& m2) const
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(!(m2.dt()==NonUnitDiag && dt()==UnitDiag));
            m2.assignToL(view());
            return *this;
        }

        inline const type& operator=(
            const AssignableToLowerTriMatrix<CT>& m2) const
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(isComplex(T()));
            TMVAssert(!(m2.dt()==NonUnitDiag && dt()==UnitDiag));
            m2.assignToL(view());
            return *this;
        }

        //
        // Access
        //

        typedef TMV_RefType(T) reference;

        inline reference operator()(int i,int j) const 
        {
            TMVAssert(i>=0 && i<int(size()));
            TMVAssert(j>=0 && j<int(size()));
            TMVAssert(okij(i,j));
            return ref(i,j); 
        }

        inline vec_type row(int i, int j1, int j2) const 
        { 
            TMVAssert(i>=0 && i<int(size()));
            TMVAssert(j1>=0 && j1<=j2 && j2<=int(size()));
            TMVAssert(j1==j2 || okij(i,j2-1));
            return vec_type(
                ptr()+i*stepi()+j1*stepj(),j2-j1,stepj(),ct() TMV_FIRSTLAST); 
        }

        inline vec_type col(int j, int i1, int i2) const
        {
            TMVAssert(j>=0 && j<int(size()));
            TMVAssert(i1>=0 && i1<=i2 && i2<=int(size()));
            TMVAssert(i1==i2 || okij(i1,j));
            return vec_type(
                ptr()+i1*stepi()+j*stepj(),i2-i1,stepi(),ct() TMV_FIRSTLAST); 
        }

        inline vec_type diag() const
        {
            TMVAssert(!isunit());
            return vec_type(
                ptr(),size(),stepi()+stepj(),ct() TMV_FIRSTLAST); 
        }

        inline vec_type diag(int i) const
        {
            TMVAssert(i>=-int(size())); 
            TMVAssert(isunit() ? i<0 : i<=0);
            return vec_type(
                ptr()-i*stepi(),size()+i,stepi()+stepj(),ct() TMV_FIRSTLAST);
        }

        inline vec_type diag(int i, int j1, int j2) const
        {
            TMVAssert(i>=-int(size())); 
            TMVAssert(isunit() ? i<0 : i<=0);
            TMVAssert(j1>=0 && j1 <= j2 && j2 <= int(size())+i);
            const int ds = stepi()+stepj();
            return vec_type(
                ptr()-i*stepi()+j1*ds,j2-j1,ds,ct() TMV_FIRSTLAST);
        }

        //
        // Modifying Functions
        //

        inline const type& setZero() const 
        { transpose().setZero(); return *this; }

        inline const type& setAllTo(const T& x) const
        { transpose().setAllTo(x); return *this; }

        inline const type& clip(RT thresh) const
        { transpose().clip(thresh); return *this; }

        inline const type& conjugateSelf() const
        { transpose().conjugateSelf(); return *this; }

        inline const type& invertSelf() const
        { transpose().invertSelf(); return *this; }

        inline const type& setToIdentity(const T& x=T(1)) const
        { transpose().setToIdentity(x); return *this; }

        TMV_DEPRECATED(const type& Zero() const)
        { return setZero(); }
        TMV_DEPRECATED(const type& SetAllTo(const T& x) const)
        { return setAllTo(x); }
        TMV_DEPRECATED(const type& Clip(RT thresh) const)
        { return clip(thresh); }
        TMV_DEPRECATED(const type& ConjugateSelf() const)
        { return conjugateSelf(); }
        TMV_DEPRECATED(const type& InvertSelf() const)
        { return invertSelf(); }
        TMV_DEPRECATED(const type& SetToIdentity(const T& x=T(1)) const)
        { return setToIdentity(x); }


        //
        // subMatrix
        //

        using base::hasSubMatrix;
        using base::hasSubVector;
        using base::hasSubTriMatrix;

        inline rec_type subMatrix(int i1, int i2, int j1, int j2) const
        {
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,1,1));
            return rec_type(
                ptr()+i1*stepi()+j1*stepj(),
                i2-i1, j2-j1, stepi(),stepj(),stor(),ct() TMV_FIRSTLAST );
        }

        inline rec_type subMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        {
            const StorageType newstor =
                iscm() ? (istep == 1 ? ColMajor : NoMajor) :
                isrm() ? (jstep == 1 ? RowMajor : NoMajor) : NoMajor;
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,istep,jstep));
            return rec_type(
                ptr()+i1*stepi()+j1*stepj(),
                (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
                newstor,ct() TMV_FIRSTLAST );
        }

        inline vec_type subVector(
            int i, int j, int istep, int jstep, int size) const
        {
            TMVAssert(hasSubVector(i,j,istep,jstep,size));
            return vec_type(
                ptr()+i*stepi()+j*stepj(),size,
                istep*stepi()+jstep*stepj(),ct() TMV_FIRSTLAST );
        }

        inline lowertri_type subTriMatrix(int i1, int i2) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,1));
            return lowertri_type(
                ptr()+i1*(stepi()+stepj()),
                i2-i1,stepi(),stepj(),dt(),stor(),ct() TMV_FIRSTLAST);
        }

        inline lowertri_type subTriMatrix(int i1, int i2, int istep) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,istep));
            return lowertri_type(
                ptr()+i1*(stepi()+stepj()),
                (i2-i1)/istep,istep*stepi(),istep*stepj(),dt(),
                istep==1 ? stor() : NoMajor,ct() TMV_FIRSTLAST);
        }

        inline lowertri_type offDiag(int noff=1) const
        {
            TMVAssert(noff <= int(size()));
            return lowertri_type(
                ptr()+noff*stepi(),size()-noff,
                stepi(),stepj(),NonUnitDiag,stor(),ct() TMV_FIRSTLAST);
        }

        inline real_type realPart() const
        {
            return real_type(
                reinterpret_cast<RT*>(ptr()), size(),
                isReal(T()) ? stepi() : 2*stepi(),
                isReal(T()) ? stepj() : 2*stepj(),
                dt(), isReal(T()) ? stor() : NoMajor, NonConj
#ifdef TMVFLDEBUG
                ,reinterpret_cast<const RT*>(_first)
                ,reinterpret_cast<const RT*>(_last)
#endif
            );
        }

        inline real_type imagPart() const
        {
            TMVAssert(isComplex(T()));
            TMVAssert(!isunit());
            return real_type(
                reinterpret_cast<RT*>(ptr())+1, size(),
                2*stepi(), 2*stepj(), dt(), NoMajor, NonConj
#ifdef TMVFLDEBUG
                ,reinterpret_cast<const RT*>(_first)+1
                ,reinterpret_cast<const RT*>(_last)+1
#endif
            );
        }

        inline lowertri_type view() const
        { return *this; }

        inline lowertri_type viewAsUnitDiag() const
        {
            return lowertri_type(
                ptr(),size(),
                stepi(),stepj(),UnitDiag,stor(),ct() TMV_FIRSTLAST);
        }

        inline uppertri_type transpose() const
        {
            return uppertri_type(
                ptr(),size(),
                stepj(),stepi(),dt(),TMV_TransOf(stor()),ct() TMV_FIRSTLAST);
        }

        inline lowertri_type conjugate() const
        {
            return lowertri_type(
                ptr(),size(),
                stepi(),stepj(),dt(),stor(),TMV_ConjOf(T,ct()) TMV_FIRSTLAST);
        }

        inline uppertri_type adjoint() const
        {
            return uppertri_type(
                ptr(),size(),
                stepj(),stepi(),dt(),TMV_TransOf(stor()),TMV_ConjOf(T,ct()) 
                TMV_FIRSTLAST);
        }

        TMV_DEPRECATED(rec_type SubMatrix(
                int i1, int i2, int j1, int j2) const)
        { return subMatrix(i1,i2,j1,j2); }
        TMV_DEPRECATED(rec_type SubMatrix(
                int i1, int i2, int j1, int j2, int istep, int jstep) const)
        { return subMatrix(i1,i2,j1,j2,istep,jstep); }
        TMV_DEPRECATED(vec_type SubVector(
                int i, int j, int istep, int jstep, int s) const)
        { return subVector(i,j,istep,jstep,s); }
        TMV_DEPRECATED(lowertri_type SubTriMatrix(int i1, int i2) const)
        { return subTriMatrix(i1,i2); }
        TMV_DEPRECATED(lowertri_type SubTriMatrix(
                int i1, int i2, int istep) const)
        { return subTriMatrix(i1,i2,istep); }
        TMV_DEPRECATED(lowertri_type OffDiag(int noff=1) const)
        { return offDiag(noff); }
        TMV_DEPRECATED(real_type Real() const)
        { return realPart(); }
        TMV_DEPRECATED(real_type Imag() const)
        { return imagPart(); }
        TMV_DEPRECATED(lowertri_type View() const)
        { return view(); }
        TMV_DEPRECATED(lowertri_type ViewAsUnitDiag() const)
        { return viewAsUnitDiag(); }
        TMV_DEPRECATED(uppertri_type Transpose() const)
        { return transpose(); }
        TMV_DEPRECATED(lowertri_type Conjugate() const)
        { return conjugate(); }
        TMV_DEPRECATED(uppertri_type Adjoint() const)
        { return adjoint(); }

        //
        // I/O
        //

        void read(std::istream& is) const;

        TMV_DEPRECATED(void Read(std::istream& is) const)
        { read(is); }

        inline size_t size() const { return itss; }
        inline const T* cptr() const { return itsm; }
        inline T* ptr() const { return itsm; }
        inline int stepi() const { return itssi; }
        inline int stepj() const { return itssj; }
        using base::isconj;
        using base::isrm;
        using base::iscm;
        using base::isunit;
        inline StorageType stor() const { return itsstor; }
        inline DiagType dt() const { return itsdiag; }
        inline ConjType ct() const { return itsct; }


    protected :

        T*const itsm;
        const size_t itss;
        const int itssi;
        const int itssj;
        DiagType itsdiag;
        StorageType itsstor;
        ConjType itsct;

#ifdef TMVFLDEBUG
    public:
        const T*const _first;
        const T*const _last;
    protected:
#endif

        using base::okij;
        reference ref(int i, int j) const;

    }; // LowerTriMatrixView

    template <class T> 
    class UpperTriMatrixView<T,FortranStyle> : 
        public UpperTriMatrixView<T,CStyle>
    {
        typedef TMV_RealType(T) RT;
        typedef TMV_ComplexType(T) CT;
        typedef GenUpperTriMatrix<T> base;
        typedef UpperTriMatrixView<T,FortranStyle> type;
        typedef UpperTriMatrixView<T,CStyle> c_type;
        typedef ConstUpperTriMatrixView<T,FortranStyle> const_type;
        typedef LowerTriMatrixView<T,FortranStyle> lowertri_type;
        typedef UpperTriMatrixView<T,FortranStyle> uppertri_type;
        typedef MatrixView<T,FortranStyle> rec_type;
        typedef VectorView<T,FortranStyle> vec_type;
        typedef UpperTriMatrixView<RT,FortranStyle> real_type;

    public:

        //
        // Constructors
        //

        inline UpperTriMatrixView(const type& rhs) : c_type(rhs) {}

        inline UpperTriMatrixView(const c_type& rhs) : c_type(rhs) {}

        inline UpperTriMatrixView(
            T* _m, size_t _s, int _si, int _sj,
            DiagType indt, StorageType instor, ConjType inct 
            TMV_PARAMFIRSTLAST(T) 
        ) :
            c_type(_m,_s,_si,_sj,indt,instor,inct 
                   TMV_FIRSTLAST1(_first,_last) ) 
        {}

        virtual inline ~UpperTriMatrixView() {} 

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

        inline const type& operator=(const GenUpperTriMatrix<RT>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const GenUpperTriMatrix<CT>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const GenDiagMatrix<RT>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const GenDiagMatrix<CT>& m2) const
        { c_type::operator=(m2); return *this; }

        template <class T2> 
        inline const type& operator=(const GenUpperTriMatrix<T2>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const T& x) const 
        { c_type::operator=(x); return *this; }

        inline const type& operator=(
            const AssignableToUpperTriMatrix<RT>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(
            const AssignableToUpperTriMatrix<CT>& m2) const
        { c_type::operator=(m2); return *this; }

        //
        // Access
        //

        inline TMV_RefType(T) operator()(int i,int j) const 
        { 
            TMVAssert(i>0 && i <= int(size()));
            TMVAssert(j>0 && j <= int(size()));
            TMVAssert(okij(i-1,j-1));
            return ref(i-1,j-1); 
        }

        inline vec_type row(int i, int j1, int j2) const 
        { 
            TMVAssert(i>0 && i<=int(size()));
            TMVAssert(j1>0 && j1<=j2 && j2<=int(size()));
            return c_type::row(i-1,j1-1,j2);
        }

        inline vec_type col(int j, int i1, int i2) const
        {
            TMVAssert(j>0 && j<=int(size()));
            TMVAssert(i1>0 && i1<=i2 && i2<=int(size()));
            return c_type::col(j-1,i1-1,i2);
        }

        inline vec_type diag() const
        { return c_type::diag(); }

        inline vec_type diag(int i) const
        { return c_type::diag(i); }

        inline vec_type diag(int i, int j1, int j2) const
        {
            TMVAssert(j1>0);
            return c_type::diag(i,j1-1,j2); 
        }

        //
        // Modifying Functions
        //

        inline const type& setZero() const 
        { c_type::setZero(); return *this; }

        inline const type& setAllTo(const T& x) const
        { c_type::setAllTo(x); return *this; }

        inline const type& clip(RT thresh) const
        { c_type::clip(thresh); return *this; }

        inline const type& conjugateSelf() const
        { c_type::conjugateSelf(); return *this; }

        inline const type& invertSelf() const
        { c_type::invertSelf(); return *this; }

        inline const type& setToIdentity(const T& x=T(1)) const
        { c_type::setToIdentity(x); return *this; }

        TMV_DEPRECATED(const type& Zero() const)
        { return setZero(); }
        TMV_DEPRECATED(const type& SetAllTo(const T& x) const)
        { return setAllTo(x); }
        TMV_DEPRECATED(const type& Clip(RT thresh) const)
        { return clip(thresh); }
        TMV_DEPRECATED(const type& ConjugateSelf() const)
        { return conjugateSelf(); }
        TMV_DEPRECATED(const type& InvertSelf() const)
        { return invertSelf(); }
        TMV_DEPRECATED(const type& SetToIdentity(const T& x=T(1)) const)
        { return setToIdentity(x); }


        //
        // SubMatrix
        //

        inline bool hasSubMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        { return const_type(*this).hasSubMatrix(i1,i2,j1,j2,istep,jstep); }

        inline bool hasSubVector(
            int i, int j, int istep, int jstep, int s) const
        { return const_type(*this).hasSubVector(i,j,istep,jstep,s); }

        inline bool hasSubTriMatrix(int i1, int i2, int istep) const
        { return const_type(*this).hasSubTriMatrix(i1,i2,istep); }

        inline rec_type subMatrix(int i1, int i2, int j1, int j2) const
        {
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,1,1));
            return c_type::subMatrix(i1-1,i2,j1-1,j2);
        }

        inline rec_type subMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        {
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,istep,jstep));
            return c_type::subMatrix(
                i1-1,i2-1+istep,j1-1,j2-1+jstep,istep,jstep);
        }

        inline vec_type subVector(
            int i, int j, int istep, int jstep, int s) const
        {
            TMVAssert(hasSubVector(i,j,istep,jstep,s));
            return c_type::subVector(i-1,j-1,istep,jstep,s);
        }

        inline uppertri_type subTriMatrix(int i1, int i2) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,1));
            return c_type::subTriMatrix(i1-1,i2);
        }

        inline uppertri_type subTriMatrix(int i1, int i2, int istep) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,istep));
            return c_type::subTriMatrix(i1-1,i2-1+istep,istep);
        }

        inline uppertri_type offDiag(int noff=1) const
        {
            TMVAssert(noff <= int(size()));
            return c_type::offDiag(noff);
        }

        inline real_type realPart() const
        { return c_type::realPart(); }

        inline real_type imagPart() const
        { return c_type::imagPart(); }

        inline uppertri_type view() const
        { return *this; }

        inline uppertri_type viewAsUnitDiag() const
        { return c_type::viewAsUnitDiag(); }

        inline lowertri_type transpose() const
        { return c_type::transpose(); }

        inline uppertri_type conjugate() const
        { return c_type::conjugate(); }

        inline lowertri_type adjoint() const
        { return c_type::adjoint(); }

        TMV_DEPRECATED(rec_type SubMatrix(
                int i1, int i2, int j1, int j2) const)
        { return subMatrix(i1,i2,j1,j2); }
        TMV_DEPRECATED(rec_type SubMatrix(
                int i1, int i2, int j1, int j2, int istep, int jstep) const)
        { return subMatrix(i1,i2,j1,j2,istep,jstep); }
        TMV_DEPRECATED(vec_type SubVector(
                int i, int j, int istep, int jstep, int s) const)
        { return subVector(i,j,istep,jstep,s); }
        TMV_DEPRECATED(uppertri_type SubTriMatrix(int i1, int i2) const)
        { return subTriMatrix(i1,i2); }
        TMV_DEPRECATED(uppertri_type SubTriMatrix(
                int i1, int i2, int istep) const)
        { return subTriMatrix(i1,i2,istep); }
        TMV_DEPRECATED(uppertri_type OffDiag(int noff=1) const)
        { return offDiag(noff); }
        TMV_DEPRECATED(real_type Real() const)
        { return realPart(); }
        TMV_DEPRECATED(real_type Imag() const)
        { return imagPart(); }
        TMV_DEPRECATED(uppertri_type View() const)
        { return view(); }
        TMV_DEPRECATED(uppertri_type ViewAsUnitDiag() const)
        { return viewAsUnitDiag(); }
        TMV_DEPRECATED(lowertri_type Transpose() const)
        { return transpose(); }
        TMV_DEPRECATED(uppertri_type Conjugate() const)
        { return conjugate(); }
        TMV_DEPRECATED(lowertri_type Adjoint() const)
        { return adjoint(); }


        using c_type::size;

    protected :

        using base::okij;
        using c_type::ref;

    }; // FortranStyle UpperTriMatrixView

    template <class T> 
    class LowerTriMatrixView<T,FortranStyle> : 
        public LowerTriMatrixView<T,CStyle>
    {
        typedef TMV_RealType(T) RT;
        typedef TMV_ComplexType(T) CT;
        typedef GenLowerTriMatrix<T> base;
        typedef LowerTriMatrixView<T,FortranStyle> type;
        typedef LowerTriMatrixView<T,CStyle> c_type;
        typedef ConstLowerTriMatrixView<T,FortranStyle> const_type;
        typedef UpperTriMatrixView<T,FortranStyle> uppertri_type;
        typedef LowerTriMatrixView<T,FortranStyle> lowertri_type;
        typedef MatrixView<T,FortranStyle> rec_type;
        typedef VectorView<T,FortranStyle> vec_type;
        typedef LowerTriMatrixView<RT,FortranStyle> real_type;

    public:

        //
        // Constructors
        //

        inline LowerTriMatrixView(const type& rhs) : c_type(rhs) {}

        inline LowerTriMatrixView(const c_type& rhs) : c_type(rhs) {}

        inline LowerTriMatrixView(
            T* _m, size_t _s, int _si, int _sj,
            DiagType indt, StorageType instor, ConjType inct 
            TMV_PARAMFIRSTLAST(T) 
        ) :
            c_type(_m,_s,_si,_sj,indt,instor,inct 
                   TMV_FIRSTLAST1(_first,_last) ) 
        {}

        virtual inline ~LowerTriMatrixView() {} 

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

        inline const type& operator=(const GenLowerTriMatrix<RT>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const GenLowerTriMatrix<CT>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const GenDiagMatrix<RT>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const GenDiagMatrix<CT>& m2) const
        { c_type::operator=(m2); return *this; }

        template <class T2> 
        inline const type& operator=(const GenLowerTriMatrix<T2>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(const T& x) const 
        { c_type::operator=(x); return *this; }

        inline const type& operator=(
            const AssignableToLowerTriMatrix<RT>& m2) const
        { c_type::operator=(m2); return *this; }

        inline const type& operator=(
            const AssignableToLowerTriMatrix<CT>& m2) const
        { c_type::operator=(m2); return *this; }

        //
        // Access
        //

        inline TMV_RefType(T) operator()(int i,int j) const 
        { 
            TMVAssert(i>0 && i <= int(size()));
            TMVAssert(j>0 && j <= int(size()));
            TMVAssert(okij(i-1,j-1));
            return ref(i-1,j-1); 
        }

        inline vec_type row(int i, int j1, int j2) const 
        { 
            TMVAssert(i>0 && i<=int(size()));
            TMVAssert(j1>0 && j1<=j2 && j2<=int(size()));
            return c_type::row(i-1,j1-1,j2);
        }

        inline vec_type col(int j, int i1, int i2) const
        {
            TMVAssert(j>0 && j<=int(size()));
            TMVAssert(i1>0 && i1<=i2 && i2<=int(size()));
            return c_type::col(j-1,i1-1,i2);
        }

        inline vec_type diag() const
        { return c_type::diag(); }

        inline vec_type diag(int i) const
        { return c_type::diag(i); }

        inline vec_type diag(int i, int j1, int j2) const
        {
            TMVAssert(j1>0);
            return c_type::diag(i,j1-1,j2); 
        }

        //
        // Modifying Functions
        //

        inline const type& setZero() const 
        { c_type::setZero(); return *this; }

        inline const type& setAllTo(const T& x) const
        { c_type::setAllTo(x); return *this; }

        inline const type& clip(RT thresh) const
        { c_type::clip(thresh); return *this; }

        inline const type& conjugateSelf() const
        { c_type::conjugateSelf(); return *this; }

        inline const type& invertSelf() const
        { c_type::invertSelf(); return *this; }

        inline const type& setToIdentity(const T& x=T(1)) const
        { c_type::setToIdentity(x); return *this; }

        TMV_DEPRECATED(const type& Zero() const)
        { return setZero(); }
        TMV_DEPRECATED(const type& SetAllTo(const T& x) const)
        { return setAllTo(x); }
        TMV_DEPRECATED(const type& Clip(RT thresh) const)
        { return clip(thresh); }
        TMV_DEPRECATED(const type& ConjugateSelf() const)
        { return conjugateSelf(); }
        TMV_DEPRECATED(const type& InvertSelf() const)
        { return invertSelf(); }
        TMV_DEPRECATED(const type& SetToIdentity(const T& x=T(1)) const)
        { return setToIdentity(x); }


        //
        // subMatrix
        //

        inline bool hasSubMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        { return const_type(*this).hasSubMatrix(i1,i2,j1,j2,istep,jstep); }

        inline bool hasSubVector(
            int i, int j, int istep, int jstep, int s) const
        { return const_type(*this).hasSubVector(i,j,istep,jstep,s); }

        inline bool hasSubTriMatrix(int i1, int i2, int istep) const
        { return const_type(*this).hasSubTriMatrix(i1,i2,istep); }

        inline rec_type subMatrix(int i1, int i2, int j1, int j2) const
        {
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,1,1));
            return c_type::subMatrix(i1-1,i2,j1-1,j2);
        }

        inline rec_type subMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        {
            TMVAssert(hasSubMatrix(i1,i2,j1,j2,istep,jstep));
            return c_type::subMatrix(
                i1-1,i2-1+istep,j1-1,j2-1+jstep,istep,jstep);
        }

        inline vec_type subVector(
            int i, int j, int istep, int jstep, int s) const
        {
            TMVAssert(hasSubVector(i,j,istep,jstep,s));
            return c_type::subVector(i-1,j-1,istep,jstep,s);
        }

        inline lowertri_type subTriMatrix(int i1, int i2) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,1));
            return c_type::subTriMatrix(i1-1,i2);
        }

        inline lowertri_type subTriMatrix(int i1, int i2, int istep) const
        {
            TMVAssert(hasSubTriMatrix(i1,i2,istep));
            return c_type::subTriMatrix(i1-1,i2-1+istep,istep);
        }

        inline lowertri_type offDiag(int noff=1) const
        {
            TMVAssert(noff <= int(size()));
            return c_type::offDiag(noff);
        }

        inline real_type realPart() const
        { return c_type::realPart(); }

        inline real_type imagPart() const
        { return c_type::imagPart(); }

        inline lowertri_type view() const
        { return *this; }

        inline lowertri_type viewAsUnitDiag() const
        { return c_type::viewAsUnitDiag(); }

        inline uppertri_type transpose() const
        { return c_type::transpose(); }

        inline lowertri_type conjugate() const
        { return c_type::conjugate(); }

        inline uppertri_type adjoint() const
        { return c_type::adjoint(); }

        TMV_DEPRECATED(rec_type SubMatrix(
                int i1, int i2, int j1, int j2) const)
        { return subMatrix(i1,i2,j1,j2); }
        TMV_DEPRECATED(rec_type SubMatrix(
                int i1, int i2, int j1, int j2, int istep, int jstep) const)
        { return subMatrix(i1,i2,j1,j2,istep,jstep); }
        TMV_DEPRECATED(vec_type SubVector(
                int i, int j, int istep, int jstep, int s) const)
        { return subVector(i,j,istep,jstep,s); }
        TMV_DEPRECATED(lowertri_type SubTriMatrix(int i1, int i2) const)
        { return subTriMatrix(i1,i2); }
        TMV_DEPRECATED(lowertri_type SubTriMatrix(
                int i1, int i2, int istep) const)
        { return subTriMatrix(i1,i2,istep); }
        TMV_DEPRECATED(lowertri_type OffDiag(int noff=1) const)
        { return offDiag(noff); }
        TMV_DEPRECATED(real_type Real() const)
        { return realPart(); }
        TMV_DEPRECATED(real_type Imag() const)
        { return imagPart(); }
        TMV_DEPRECATED(lowertri_type View() const)
        { return view(); }
        TMV_DEPRECATED(lowertri_type ViewAsUnitDiag() const)
        { return viewAsUnitDiag(); }
        TMV_DEPRECATED(uppertri_type Transpose() const)
        { return transpose(); }
        TMV_DEPRECATED(lowertri_type Conjugate() const)
        { return conjugate(); }
        TMV_DEPRECATED(uppertri_type Adjoint() const)
        { return adjoint(); }


        using c_type::size;

    protected :

        using base::okij;
        using c_type::ref;

    }; // FortranStyle LowerTriMatrixView

#ifdef XTEST
#ifdef TMVDEBUG
#define XTEST_DEBUG
#endif
#endif

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    class UpperTriMatrix : public GenUpperTriMatrix<T>
    {
        typedef TMV_RealType(T) RT;
        typedef TMV_ComplexType(T) CT;
        typedef GenUpperTriMatrix<T> base;
        typedef UpperTriMatrix<T,D,S,I> type;
        typedef ConstVectorView<T,I> const_vec_type;
        typedef ConstMatrixView<T,I> const_rec_type;
        typedef ConstUpperTriMatrixView<T,I> const_uppertri_type;
        typedef ConstLowerTriMatrixView<T,I> const_lowertri_type;
        typedef ConstUpperTriMatrixView<RT,I> const_real_type;
        typedef VectorView<T,I> vec_type;
        typedef MatrixView<T,I> rec_type;
        typedef UpperTriMatrixView<T,I> uppertri_type;
        typedef LowerTriMatrixView<T,I> lowertri_type;
        typedef UpperTriMatrixView<RT,I> real_type;

    public:

        //
        // Constructors
        //

#define NEW_SIZE(s) \
        itslen((s)*(s)), itsm(new T[itslen]), itss(s) \
        TMV_DEFFIRSTLAST(itsm.get(),itsm.get()+itslen)

        explicit inline UpperTriMatrix(size_t _size) : NEW_SIZE(_size) 
        { 
            TMVAssert(S==RowMajor || S==ColMajor); 
#ifdef TMVDEBUG
            setAllTo(T(888));
#endif
        }

        inline UpperTriMatrix(size_t _size, const T& x) : NEW_SIZE(_size)
        {
            TMVAssert(S==RowMajor || S==ColMajor);
            setAllTo(x);
        }

        inline UpperTriMatrix(size_t _size, const T* vv) : NEW_SIZE(_size)
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            std::copy(vv,vv+itslen,itsm.get());
        }

        inline UpperTriMatrix(size_t _size, const std::vector<T>& vv) :
            NEW_SIZE(_size)
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            TMVAssert(vv.size() == itslen);
            std::copy(vv.begin(),vv.end(),itsm.get());
        }

        template <class T2> 
        inline UpperTriMatrix(const GenMatrix<T2>& rhs) :
            NEW_SIZE(rhs.rowsize())
        { 
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(isReal(T2()) || isComplex(T()));
            TMVAssert(S==RowMajor || S==ColMajor); 
            Copy(rhs.upperTri(D),view()); 
        }

        template <class T2> 
        inline UpperTriMatrix(const GenUpperTriMatrix<T2>& rhs) :
            NEW_SIZE(rhs.size())
        { 
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(isReal(T2()) || isComplex(T()));
            TMVAssert(S==RowMajor || S==ColMajor); 
            if (isunit() && !rhs.isunit()) {
                if (rhs.size() > 0)
                    Copy(rhs.offDiag(),offDiag());
            }
            else {
                Copy(rhs,view());
            }
        }

        inline UpperTriMatrix(const type& rhs) :
            itslen(rhs.itslen), itsm(new T[itslen]), itss(rhs.itss)
            TMV_DEFFIRSTLAST(itsm.get(),itsm.get()+itslen)
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            std::copy(rhs.cptr(),rhs.cptr()+itslen,itsm.get());
        }

        template <DiagType D2, IndexStyle I2> 
        inline UpperTriMatrix(const UpperTriMatrix<T,D2,S,I2>& rhs) :
            NEW_SIZE(rhs.size())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            std::copy(rhs.cptr(),rhs.cptr()+itslen,itsm.get());
            if (D==NonUnitDiag && D2==UnitDiag) diag().setAllTo(T(1));
        }

        template <IndexStyle I2> 
        inline UpperTriMatrix(const Matrix<T,S,I2>& rhs) :
            NEW_SIZE(rhs.rowsize())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            if (rhs.isSquare())
                std::copy(rhs.cptr(),rhs.cptr()+itslen,itsm.get());
            else
                Copy(rhs.upperTri(D),view());
        }

        inline UpperTriMatrix(const GenMatrix<T>& rhs) :
            NEW_SIZE(rhs.rowsize())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            Copy(rhs.upperTri(D),view());
        }

        inline UpperTriMatrix(const GenUpperTriMatrix<RT>& rhs) :
            NEW_SIZE(rhs.size())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            if (isunit() && !rhs.isunit()) {
                if (rhs.size() > 0) offDiag() = rhs.offDiag();
            }
            else rhs.assignToU(view());
        }

        inline UpperTriMatrix(const GenUpperTriMatrix<CT>& rhs) :
            NEW_SIZE(rhs.size())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            TMVAssert(isComplex(T()));
            if (isunit() && !rhs.isunit()) {
                if (rhs.size() > 0) offDiag() = rhs.offDiag();
            }
            else rhs.assignToU(view());
        }

        inline UpperTriMatrix(const AssignableToUpperTriMatrix<RT>& m2) :
            NEW_SIZE(m2.size())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            m2.assignToU(view());
        }

        inline UpperTriMatrix(const AssignableToUpperTriMatrix<CT>& m2) :
            NEW_SIZE(m2.size())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            TMVAssert(isComplex(T()));
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            m2.assignToU(view());
        }

#undef NEW_SIZE

        virtual inline ~UpperTriMatrix()
        {
#ifdef TMVDEBUG
            setAllTo(T(999));
#endif
        }


        //
        // Op=
        //

        inline type& operator=(const type& m2)
        { 
            TMVAssert(size() == m2.size());
            if (&m2 != this) 
                std::copy(m2.cptr(),m2.cptr()+itslen,itsm.get());
            return *this;
        }

        template <IndexStyle I2> 
        inline type& operator=(const UpperTriMatrix<T,D,S,I2>& m2)
        { 
            TMVAssert(size() == m2.size());
            if (&m2 != this) 
                std::copy(m2.cptr(),m2.cptr()+itslen,itsm.get());
            return *this;
        }

        inline type& operator=(const GenUpperTriMatrix<RT>& m2)
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            m2.assignToU(view());
            return *this;
        }

        inline type& operator=(const GenUpperTriMatrix<CT>& m2)
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            TMVAssert(isComplex(T()));
            m2.assignToU(view());
            return *this;
        }

        template <class T2> 
        inline type& operator=(const GenUpperTriMatrix<T2>& m2)
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(isReal(T2()) || sComplex(T()));
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            Copy(m2,view());
            return *this;
        }

        inline type& operator=(const T& x) 
        { 
            TMVAssert(!this->isunit() || x==T(1));
            return setToIdentity(x); 
        }

        inline type& operator=(const AssignableToUpperTriMatrix<RT>& m2)
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            m2.assignToU(view());
            return *this;
        }

        inline type& operator=(const AssignableToUpperTriMatrix<CT>& m2)
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            TMVAssert(isComplex(T()));
            m2.assignToU(view());
            return *this;
        }

        //
        // Access
        //

        typedef T& reference;

        inline T operator()(int i, int j) const
        {
            if (I == CStyle) {
                TMVAssert(i>=0 && i<int(size()));
                TMVAssert(j>=0 && j<int(size()));
            } else {
                TMVAssert(i>0 && i<= int(size())); --i;
                TMVAssert(j>0 && j<= int(size())); --j;
            }
            if (i>j) return T(0);
            else if (i==j && D == UnitDiag) return T(1);
            else { TMVAssert(okij(i,j)); return cref(i,j); }
        }

        inline T& operator()(int i, int j) 
        { 
            if (I == CStyle) {
                TMVAssert(i>=0 && i<int(size()));
                TMVAssert(j>=0 && j<int(size()));
                TMVAssert(okij(i,j));
                return ref(i,j);
            } else {
                TMVAssert(i>0 && i<= int(size()));
                TMVAssert(j>0 && j<= int(size()));
                TMVAssert(okij(i-1,j-1));
                return ref(i-1,j-1);
            }
        }

        inline const_vec_type row(int i, int j1, int j2) const 
        { 
            if (I==FortranStyle) { 
                TMVAssert(i>0 && i<=int(size())); --i;
                TMVAssert(j1>0 && j1<=j2 && j2<=int(size())); --j1;
            } else {
                TMVAssert(i>=0 && i<int(size()));
                TMVAssert(j1>=0 && j1<=j2 && j2<=int(size()));
            }
            TMVAssert(j1==j2 || okij(i,j1));
            return const_vec_type(
                itsm.get()+i*stepi()+j1*stepj(),j2-j1,stepj(),NonConj);
        }

        inline const_vec_type col(int j, int i1, int i2) const
        {
            if (I==FortranStyle) { 
                TMVAssert(j>0 && j<=int(size())); --j;
                TMVAssert(i1>0 && i1<=i2 && i2<=int(size())); --i1;
            } else {
                TMVAssert(j>=0 && j<int(size()));
                TMVAssert(i1>=0 && i1<=i2 && i2<=int(size())); 
            }
            TMVAssert(i1==i2 || okij(i2-1,j));
            return const_vec_type(
                itsm.get()+i1*stepi()+j*stepj(),i2-i1,stepi(),NonConj);
        }

        inline const_vec_type diag() const
        {
            TMVAssert(!isunit());
            return const_vec_type(itsm.get(),size(),stepi()+stepj(),NonConj); 
        }

        inline const_vec_type diag(int i) const
        {
            TMVAssert(isunit() ? i>0 : i>=0);
            TMVAssert(i<=int(size())); 
            return const_vec_type(
                itsm.get()+i*stepj(),size()-i,stepi()+stepj(),NonConj);
        }

        inline const_vec_type diag(int i, int j1, int j2) const
        {
            TMVAssert(isunit() ? i>0 : i>=0);
            TMVAssert(i<=int(size())); 
            if (I == FortranStyle) {
                TMVAssert(j1 > 0 && j1<=j2 && j2<=int(size())-i); --j1; 
            } else {
                TMVAssert(j1>=0 && j1<=j2 && j2<=int(size())-i); 
            }
            const int ds = stepi()+stepj();
            return const_vec_type(
                itsm.get()+i*stepj()+j1*ds,j2-j1,ds,NonConj);
        }

        inline vec_type row(int i, int j1, int j2)
        { 
            if (I==FortranStyle) { 
                TMVAssert(i>0 && i<=int(size())); --i;
                TMVAssert(j1>0 && j1<=j2 && j2<=int(size())); --j1;
            } else {
                TMVAssert(i>=0 && i<int(size()));
                TMVAssert(j1>=0 && j1<=j2 && j2<=int(size()));
            }
            TMVAssert(j1==j2 || okij(i,j1));
            return vec_type(
                itsm.get()+i*stepi()+j1*stepj(),
                j2-j1,stepj(),NonConj TMV_FIRSTLAST);
        }

        inline vec_type col(int j, int i1, int i2)
        {
            if (I==FortranStyle) { 
                TMVAssert(j>0 && j<=int(size())); --j;
                TMVAssert(i1>0 && i1<=i2 && i2<=int(size())); --i1;
            } else {
                TMVAssert(j>=0 && j<int(size()));
                TMVAssert(i1>=0 && i1<=i2 && i2<=int(size())); 
            }
            TMVAssert(i1==i2 || okij(i2-1,j));
            return vec_type(
                itsm.get()+i1*stepi()+j*stepj(),
                i2-i1,stepi(),NonConj TMV_FIRSTLAST);
        }

        inline vec_type diag()
        {
            TMVAssert(!isunit());
            return vec_type(
                itsm.get(),size(),stepi()+stepj(),NonConj TMV_FIRSTLAST); 
        }

        inline vec_type diag(int i)
        {
            TMVAssert(isunit() ? i>0 : i>=0);
            TMVAssert(i<=int(size())); 
            return vec_type(
                itsm.get()+i*stepj(),size()-i,stepi()+stepj(),NonConj 
                TMV_FIRSTLAST);
        }

        inline vec_type diag(int i, int j1, int j2)
        {
            TMVAssert(isunit() ? i>0 : i>=0);
            TMVAssert(i<=int(size())); 
            if (I == FortranStyle) {
                TMVAssert(j1 > 0 && j1<=j2 && j2<=int(size())-i); --j1; 
            } else {
                TMVAssert(j1>=0 && j1<=j2 && j2<=int(size())-i); 
            }
            const int ds = stepi()+stepj();
            return vec_type(
                itsm.get()+i*stepj()+j1*ds,j2-j1,ds,NonConj TMV_FIRSTLAST);
        }

        //
        // Modifying Functions
        //

        inline type& setZero() 
        { fill_n(itsm.get(),itslen,T(0)); return *this; }

        inline type& setAllTo(const T& x) 
        { VectorViewOf(itsm.get(),itslen).setAllTo(x); return *this; }

        inline type& clip(RT thresh)
        { VectorViewOf(itsm.get(),itslen).clip(thresh); return *this; }

        inline type& conjugateSelf() 
        { VectorViewOf(itsm.get(),itslen).conjugateSelf(); return *this; }

        inline type& invertSelf()
        { view().invertSelf(); return *this; }

        inline type& setToIdentity(const T& x=T(1)) 
        {
            TMVAssert(!isunit() || x==T(1));
            setZero(); if (!isunit()) diag().setAllTo(x);
            return *this;
        }

        TMV_DEPRECATED(type& Zero())
        { return setZero(); }
        TMV_DEPRECATED(type& SetAllTo(const T& x))
        { return setAllTo(x); }
        TMV_DEPRECATED(type& Clip(RT thresh))
        { return clip(thresh); }
        TMV_DEPRECATED(type& ConjugateSelf())
        { return conjugateSelf(); }
        TMV_DEPRECATED(type& InvertSelf())
        { return invertSelf(); }
        TMV_DEPRECATED(type& SetToIdentity(const T& x=T(1)))
        { return setToIdentity(x); }


        //
        // subMatrix
        //

        inline const_rec_type subMatrix(int i1, int i2, int j1, int j2) const
        {
            TMVAssert(view().hasSubMatrix(i1,i2,j1,j2,1,1));
            if (I==FortranStyle) { --i1; --j1; }
            return const_rec_type(
                itsm.get()+i1*stepi()+j1*stepj(),
                i2-i1, j2-j1,stepi(),stepj(),S,NonConj);
        }

        inline const_rec_type subMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        {
            StorageType newstor = S==RowMajor ?
                jstep == 1 ? RowMajor : NoMajor :
                istep == 1 ? ColMajor : NoMajor;
            TMVAssert(view().hasSubMatrix(i1,i2,j1,j2,istep,jstep));
            if (I==FortranStyle) { --i1; --j1; i2+=istep-1; j2+=jstep-1; }
            return const_rec_type(
                itsm.get()+i1*stepi()+j1*stepj(),
                (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
                newstor, NonConj);
        }

        inline const_vec_type subVector(
            int i, int j, int istep, int jstep, int size) const
        {
            TMVAssert(view().hasSubVector(i,j,istep,jstep,size));
            if (I==FortranStyle) { --i; --j; }
            return const_vec_type(
                itsm.get()+i*stepi()+j*stepj(),size,
                istep*stepi()+jstep*stepj(),NonConj);
        }

        inline const_uppertri_type subTriMatrix(int i1, int i2) const
        {
            TMVAssert(view().hasSubTriMatrix(i1,i2,1));
            if (I==FortranStyle) { --i1; }
            return const_uppertri_type(
                itsm.get()+i1*(stepi()+stepj()),
                i2-i1,stepi(),stepj(),D,S,NonConj);
        }

        inline const_uppertri_type subTriMatrix(
            int i1, int i2, int istep) const
        {
            TMVAssert(view().hasSubTriMatrix(i1,i2,istep));
            if (I==FortranStyle) { --i1; i2+=istep-1; }
            return const_uppertri_type(
                itsm.get()+i1*(stepi()+stepj()),
                (i2-i1)/istep, istep*stepi(),istep*stepj(), D,
                istep==1 ? S : NoMajor, NonConj);
        }

        inline const_uppertri_type offDiag(int noff=1) const
        {
            TMVAssert(noff <= int(size()));
            return const_uppertri_type(
                itsm.get()+noff*stepj(),
                size()-noff,stepi(),stepj(),NonUnitDiag,S,NonConj);
        }

        inline const_real_type realPart() const
        {
            return const_real_type(
                reinterpret_cast<RT*>(itsm.get()), size(),
                isReal(T()) ? stepi() : 2*stepi(),
                isReal(T()) ? stepj() : 2*stepj(),
                D, isReal(T()) ? S : NoMajor, NonConj);
        }

        inline const_real_type imagPart() const
        {
            TMVAssert(isComplex(T()));
            TMVAssert(!isunit());
            return const_real_type(
                reinterpret_cast<RT*>(itsm.get())+1, size(),
                2*stepi(), 2*stepj(), NonUnitDiag, NoMajor, NonConj);
        }

        inline rec_type subMatrix(int i1, int i2, int j1, int j2) 
        {
            TMVAssert(view().hasSubMatrix(i1,i2,j1,j2,1,1));
            if (I==FortranStyle) { --i1; --j1; }
            return rec_type(
                itsm.get()+i1*stepi()+j1*stepj(),
                i2-i1, j2-j1, stepi(),stepj(),S,NonConj TMV_FIRSTLAST);
        }

        inline rec_type subMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep)
        {
            StorageType newstor = S == RowMajor ?
                jstep == 1 ? RowMajor : NoMajor :
                istep == 1 ? ColMajor : NoMajor;
            TMVAssert(view().hasSubMatrix(i1,i2,j1,j2,istep,jstep));
            if (I==FortranStyle) { --i1; --j1; i2+=istep-1; j2+=jstep-1; }
            return rec_type(
                itsm.get()+i1*stepi()+j1*stepj(),
                (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
                newstor,NonConj TMV_FIRSTLAST);
        }

        inline vec_type subVector(int i, int j, int istep, int jstep, int size)
        {
            TMVAssert(view().hasSubVector(i,j,istep,jstep,size));
            if (I==FortranStyle) { --i; --j; }
            return vec_type(
                itsm.get()+i*stepi()+j*stepj(),size,
                istep*stepi()+jstep*stepj(),NonConj TMV_FIRSTLAST);
        }

        inline uppertri_type subTriMatrix(int i1, int i2)
        {
            TMVAssert(view().hasSubTriMatrix(i1,i2,1));
            if (I==FortranStyle) { --i1; }
            return uppertri_type(
                itsm.get()+i1*(stepi()+stepj()),
                i2-i1,stepi(),stepj(),D,S,NonConj TMV_FIRSTLAST);
        }

        inline uppertri_type subTriMatrix(int i1, int i2, int istep) 
        {
            TMVAssert(view().hasSubTriMatrix(i1,i2,istep));
            if (I==FortranStyle) { --i1; i2+=istep-1; }
            return uppertri_type(
                itsm.get()+i1*(stepi()+stepj()),
                (i2-i1)/istep,istep*stepi(),istep*stepj(),D,
                istep==1 ? S : NoMajor,NonConj TMV_FIRSTLAST);
        }

        inline uppertri_type offDiag(int noff=1)
        {
            TMVAssert(noff <= int(size()));
            return uppertri_type(
                itsm.get()+noff*stepj(),size()-noff,
                stepi(),stepj(),NonUnitDiag,S,NonConj TMV_FIRSTLAST);
        }

        inline real_type realPart() 
        {
            return real_type(
                reinterpret_cast<RT*>(itsm.get()), size(),
                isReal(T()) ? stepi() : 2*stepi(),
                isReal(T()) ? stepj() : 2*stepj(),
                D, isReal(T()) ? S : NoMajor, NonConj
#ifdef TMVFLDEBUG
                ,reinterpret_cast<const RT*>(_first)
                ,reinterpret_cast<const RT*>(_last)
#endif
            );
        }

        inline real_type imagPart()
        {
            TMVAssert(isComplex(T()));
            TMVAssert(!isunit());
            return real_type(
                reinterpret_cast<RT*>(itsm.get())+1, size(),
                2*stepi(), 2*stepj(), NonUnitDiag, NoMajor, NonConj
#ifdef TMVFLDEBUG
                ,reinterpret_cast<const RT*>(_first)+1
                ,reinterpret_cast<const RT*>(_last)+1
#endif
            );
        }

        inline const_uppertri_type view() const
        { 
            return const_uppertri_type(
                itsm.get(),size(),stepi(),stepj(),D,S,NonConj);
        }

        inline const_uppertri_type viewAsUnitDiag() const
        { 
            return const_uppertri_type(
                itsm.get(),size(),stepi(),stepj(),UnitDiag,S,NonConj);
        }

        inline const_lowertri_type transpose() const
        { 
            return const_lowertri_type(
                itsm.get(),size(),stepj(),stepi(),D,TMV_TransOf(S),NonConj);
        }

        inline const_uppertri_type conjugate() const
        { 
            return const_uppertri_type(
                itsm.get(),size(),stepi(),stepj(),D,S,TMV_ConjOf(T,NonConj));
        }

        inline const_lowertri_type adjoint() const
        { 
            return const_lowertri_type(
                itsm.get(),size(),stepj(),stepi(),D,
                TMV_TransOf(S),TMV_ConjOf(T,NonConj));
        }

        inline uppertri_type view() 
        { 
            return uppertri_type(
                itsm.get(),size(),stepi(),stepj(),D,S,NonConj TMV_FIRSTLAST);
        }

        inline uppertri_type viewAsUnitDiag() 
        { 
            return uppertri_type(
                itsm.get(),size(),stepi(),stepj(),UnitDiag,S,NonConj 
                TMV_FIRSTLAST);
        }

        inline lowertri_type transpose() 
        { 
            return lowertri_type(
                itsm.get(),size(),stepj(),stepi(),D,TMV_TransOf(S),NonConj 
                TMV_FIRSTLAST);
        }

        inline uppertri_type conjugate() 
        { 
            return uppertri_type(
                itsm.get(),size(),stepi(),stepj(),D,S,TMV_ConjOf(T,NonConj) 
                TMV_FIRSTLAST);
        }

        inline lowertri_type adjoint() 
        { 
            return lowertri_type(
                itsm.get(),size(),stepj(),stepi(),D,
                TMV_TransOf(S),TMV_ConjOf(T,NonConj) TMV_FIRSTLAST);
        }

        TMV_DEPRECATED(const_rec_type SubMatrix(
                int i1, int i2, int j1, int j2) const)
        { return subMatrix(i1,i2,j1,j2); }
        TMV_DEPRECATED(const_rec_type SubMatrix(
                int i1, int i2, int j1, int j2, int istep, int jstep) const)
        { return subMatrix(i1,i2,j1,j2,istep,jstep); }
        TMV_DEPRECATED(const_vec_type SubVector(
                int i, int j, int istep, int jstep, int s) const)
        { return subVector(i,j,istep,jstep,s); }
        TMV_DEPRECATED(const_uppertri_type SubTriMatrix(int i1, int i2) const)
        { return subTriMatrix(i1,i2); }
        TMV_DEPRECATED(const_uppertri_type SubTriMatrix(
                int i1, int i2, int istep) const)
        { return subTriMatrix(i1,i2,istep); }
        TMV_DEPRECATED(const_uppertri_type OffDiag(int noff=1) const)
        { return offDiag(noff); }
        TMV_DEPRECATED(const_real_type Real() const)
        { return realPart(); }
        TMV_DEPRECATED(const_real_type Imag() const)
        { return imagPart(); }
        TMV_DEPRECATED(const_uppertri_type View() const)
        { return view(); }
        TMV_DEPRECATED(const_uppertri_type ViewAsUnitDiag() const)
        { return viewAsUnitDiag(); }
        TMV_DEPRECATED(const_lowertri_type Transpose() const)
        { return transpose(); }
        TMV_DEPRECATED(const_uppertri_type Conjugate() const)
        { return conjugate(); }
        TMV_DEPRECATED(const_lowertri_type Adjoint() const)
        { return adjoint(); }
        TMV_DEPRECATED(rec_type SubMatrix(int i1, int i2, int j1, int j2))
        { return subMatrix(i1,i2,j1,j2); }
        TMV_DEPRECATED(rec_type SubMatrix(
                int i1, int i2, int j1, int j2, int istep, int jstep))
        { return subMatrix(i1,i2,j1,j2,istep,jstep); }
        TMV_DEPRECATED(vec_type SubVector(
                int i, int j, int istep, int jstep, int s))
        { return subVector(i,j,istep,jstep,s); }
        TMV_DEPRECATED(uppertri_type SubTriMatrix(int i1, int i2))
        { return subTriMatrix(i1,i2); }
        TMV_DEPRECATED(uppertri_type SubTriMatrix(int i1, int i2, int istep))
        { return subTriMatrix(i1,i2,istep); }
        TMV_DEPRECATED(uppertri_type OffDiag(int noff=1))
        { return offDiag(noff); }
        TMV_DEPRECATED(real_type Real())
        { return realPart(); }
        TMV_DEPRECATED(real_type Imag())
        { return imagPart(); }
        TMV_DEPRECATED(uppertri_type View())
        { return view(); }
        TMV_DEPRECATED(uppertri_type ViewAsUnitDiag())
        { return viewAsUnitDiag(); }
        TMV_DEPRECATED(lowertri_type Transpose())
        { return transpose(); }
        TMV_DEPRECATED(uppertri_type Conjugate())
        { return conjugate(); }
        TMV_DEPRECATED(lowertri_type Adjoint())
        { return adjoint(); }


        inline size_t size() const { return itss; }
        inline const T* cptr() const { return itsm.get(); }
        inline T* ptr() { return itsm.get(); }
        inline int stepi() const { return S==RowMajor ? itss : 1; }
        inline int stepj() const { return S==RowMajor ? 1 : itss; }
        inline DiagType dt() const { return D; }
        inline StorageType stor() const { return S; }
        inline ConjType ct() const { return NonConj; }
        inline bool isrm() const { return S==RowMajor; }
        inline bool iscm() const { return S==ColMajor; }
        inline bool isunit() const { return D == UnitDiag; }
        inline bool isconj() const { return false; }

        inline T& ref(int i, int j)
        { return itsm.get()[S==RowMajor ? i*itss + j : j*itss + i]; }

        inline T cref(int i, int j) const 
        { 
            return i==j && D==UnitDiag ? T(1) : i>j ? T(0) :
                itsm.get()[S==RowMajor ? i*itss + j : j*itss + i]; 
        }

    protected :

        const size_t itslen;
        auto_array<T> itsm;
        const size_t itss;

#ifdef TMVFLDEBUG
    public :
        const T*const _first;
        const T*const _last;
    protected :
#endif

        inline bool okij(int i, int j) const
        {
            TMVAssert(i>=0 && i < int(size()));
            TMVAssert(j>=0 && j < int(size()));
            if (isunit()) return i<j; else return i<=j;
        }

    }; // UpperTriMatrix

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    class LowerTriMatrix : public GenLowerTriMatrix<T>
    {
        typedef TMV_RealType(T) RT;
        typedef TMV_ComplexType(T) CT;
        typedef GenLowerTriMatrix<T> base;
        typedef LowerTriMatrix<T,D,S,I> type;
        typedef ConstVectorView<T,I> const_vec_type;
        typedef ConstMatrixView<T,I> const_rec_type;
        typedef ConstUpperTriMatrixView<T,I> const_uppertri_type;
        typedef ConstLowerTriMatrixView<T,I> const_lowertri_type;
        typedef ConstLowerTriMatrixView<RT,I> const_real_type;
        typedef VectorView<T,I> vec_type;
        typedef MatrixView<T,I> rec_type;
        typedef UpperTriMatrixView<T,I> uppertri_type;
        typedef LowerTriMatrixView<T,I> lowertri_type;
        typedef LowerTriMatrixView<RT,I> real_type;

    public:

        //
        // Constructors
        //

#define NEW_SIZE(s) \
        itslen((s)*(s)), itsm(new T[itslen]), itss(s) \
        TMV_DEFFIRSTLAST(itsm.get(),itsm.get()+itslen)

        explicit inline LowerTriMatrix(size_t _size) : NEW_SIZE(_size) 
        { 
            TMVAssert(S==RowMajor || S==ColMajor); 
#ifdef TMVDEBUG
            setAllTo(T(888));
#endif
        }

        inline LowerTriMatrix(size_t _size, const T& x) : NEW_SIZE(_size)
        {
            TMVAssert(S==RowMajor || S==ColMajor);
            setAllTo(x);
        }

        inline LowerTriMatrix(size_t _size, const T* vv) : NEW_SIZE(_size)
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            std::copy(vv,vv+itslen,itsm.get());
        }

        inline LowerTriMatrix(size_t _size, const std::vector<T>& vv) :
            NEW_SIZE(_size)
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            TMVAssert(vv.size() == itslen);
            std::copy(vv.begin(),vv.end(),itsm.get());
        }

        template <class T2> 
        inline LowerTriMatrix(const GenMatrix<T2>& rhs) :
            NEW_SIZE(rhs.colsize())
        { 
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(isReal(T2()) || isComplex(T()));
            TMVAssert(S==RowMajor || S==ColMajor); 
            Copy(rhs.lowerTri(D).transpose(),transpose()); 
        }

        template <class T2> 
        inline LowerTriMatrix(const GenLowerTriMatrix<T2>& rhs) :
            NEW_SIZE(rhs.size())
        { 
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(isReal(T2()) || isComplex(T()));
            TMVAssert(S==RowMajor || S==ColMajor); 
            if (isunit() && !rhs.isunit()) {
                if (rhs.size() > 0) 
                    Copy(rhs.offDiag().transpose(),offDiag().transpose());
            }
            else {
                Copy(rhs.transpose(),transpose());
            }
        }

        inline LowerTriMatrix(const type& rhs) :
            itslen(rhs.itslen), itsm(new T[itslen]), itss(rhs.itss)
            TMV_DEFFIRSTLAST(itsm.get(),itsm.get()+itslen)
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            std::copy(rhs.cptr(),rhs.cptr()+itslen,itsm.get());
        }

        template <DiagType D2, IndexStyle I2> 
        inline LowerTriMatrix(const LowerTriMatrix<T,D2,S,I2>& rhs) :
            NEW_SIZE(rhs.size())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            std::copy(rhs.cptr(),rhs.cptr()+itslen,itsm.get());
            if (D==NonUnitDiag && D2==UnitDiag) diag().setAllTo(T(1));
        }

        template <IndexStyle I2> 
        inline LowerTriMatrix(const Matrix<T,S,I2>& rhs) :
            NEW_SIZE(rhs.rowsize())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            if (rhs.isSquare())
                std::copy(rhs.cptr(),rhs.cptr()+itslen,itsm.get());
            else
                Copy(rhs.lowerTri(D).transpose(),transpose());
        }

        inline LowerTriMatrix(const GenMatrix<T>& rhs) :
            NEW_SIZE(rhs.rowsize())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            Copy(rhs.lowerTri(D).transpose(),transpose());
        }

        inline LowerTriMatrix(const GenLowerTriMatrix<RT>& rhs) :
            NEW_SIZE(rhs.size())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            if (isunit() && !rhs.isunit()) {
                if (rhs.size() > 0) offDiag() = rhs.offDiag();
            }
            else rhs.assignToL(view());
        }

        inline LowerTriMatrix(const GenLowerTriMatrix<CT>& rhs) :
            NEW_SIZE(rhs.size())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            TMVAssert(isComplex(T()));
            if (isunit() && !rhs.isunit()) {
                if (rhs.size() > 0) offDiag() = rhs.offDiag();
            }
            else rhs.assignToL(view());
        }

        inline LowerTriMatrix(const AssignableToLowerTriMatrix<RT>& m2) :
            NEW_SIZE(m2.size())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            m2.assignToL(view());
        }

        inline LowerTriMatrix(const AssignableToLowerTriMatrix<CT>& m2) :
            NEW_SIZE(m2.size())
        {
#ifdef XTEST_DEBUG
            setAllTo(T(888));
#endif
            TMVAssert(S==RowMajor || S==ColMajor);
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            TMVAssert(isComplex(T()));
            m2.assignToL(view());
        }

#undef NEW_SIZE

        virtual inline ~LowerTriMatrix() 
        {
#ifdef TMVDEBUG
            setAllTo(T(999));
#endif
        }


        //
        // Op=
        //

        inline type& operator=(const LowerTriMatrix<T,D,S,I>& m2)
        { 
            TMVAssert(size() == m2.size());
            if (&m2 != this)
                std::copy(m2.cptr(),m2.cptr()+itslen,itsm.get());
            return *this;
        }

        template <IndexStyle I2> 
        inline type& operator=(const LowerTriMatrix<T,D,S,I2>& m2)
        { 
            TMVAssert(size() == m2.size());
            if (&m2 != this) 
                std::copy(m2.cptr(),m2.cptr()+itslen,itsm.get());
            return *this;
        }

        inline type& operator=(const GenLowerTriMatrix<RT>& m2)
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            m2.assignToL(view());
            return *this;
        }

        inline type& operator=(const GenLowerTriMatrix<CT>& m2)
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(isComplex(T()));
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            m2.assignToL(view());
            return *this;
        }

        template <class T2> 
        inline type& operator=(const GenLowerTriMatrix<T2>& m2)
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(isReal(T2()) || isComplex(T()));
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            Copy(m2,view());
            return *this;
        }

        inline type& operator=(const T& x) 
        { 
            TMVAssert(!this->isunit() || x==T(1));
            return setToIdentity(x); 
        }

        inline type& operator=(const AssignableToLowerTriMatrix<RT>& m2)
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            m2.assignToL(view());
            return *this;
        }

        inline type& operator=(const AssignableToLowerTriMatrix<CT>& m2)
        { 
            TMVAssert(size() == m2.size());
            TMVAssert(isComplex(T()));
            TMVAssert(!(m2.dt()==NonUnitDiag && D==UnitDiag));
            m2.assignToL(view());
            return *this;
        }

        //
        // Access
        //

        typedef T& reference;

        inline T operator()(int i, int j) const
        {
            if (I == CStyle) {
                TMVAssert(i>=0 && i<int(size()));
                TMVAssert(j>=0 && j<int(size()));
            } else {
                TMVAssert(i>0 && i<=int(size())); --i;
                TMVAssert(j>0 && j<=int(size())); --j;
            }
            if (i<j) return T(0);
            else if (i==j && D == UnitDiag) return T(1);
            else { TMVAssert(okij(i,j)); return cref(i,j); }
        }

        inline T& operator()(int i, int j) 
        { 
            if (I == CStyle) {
                TMVAssert(i>=0 && i<int(size()));
                TMVAssert(j>=0 && j<int(size()));
                TMVAssert(okij(i,j));
                return ref(i,j);
            } else {
                TMVAssert(i>0 && i<= int(size()));
                TMVAssert(j>0 && j<= int(size()));
                TMVAssert(okij(i-1,j-1));
                return ref(i-1,j-1);
            }
        }

        inline const_vec_type row(int i, int j1, int j2) const 
        { 
            if (I==FortranStyle) { 
                TMVAssert(i>0 && i<=int(size())); --i;
                TMVAssert(j1>0 && j1<=j2 && j2<=int(size())); --j1;
            } else {
                TMVAssert(i>=0 && i<int(size()));
                TMVAssert(j1>=0 && j1<=j2 && j2<=int(size()));
            }
            TMVAssert(j1==j2 || okij(i,j2-1));
            return const_vec_type(
                itsm.get()+i*stepi()+j1*stepj(),j2-j1,stepj(),NonConj);
        }

        inline const_vec_type col(int j, int i1, int i2) const
        {
            if (I==FortranStyle) { 
                TMVAssert(j>0 && j<=int(size())); --j;
                TMVAssert(i1>0 && i1<=i2 && i2<=int(size())); --i1;
            } else {
                TMVAssert(j>=0 && j<int(size()));
                TMVAssert(i1>=0 && i1<=i2 && i2<=int(size()));
            }
            TMVAssert(i1==i2 || okij(i1,j));
            return const_vec_type(
                itsm.get()+i1*stepi()+j*stepj(),i2-i1,stepi(),NonConj);
        }

        inline const_vec_type diag() const
        {
            TMVAssert(!isunit());
            return const_vec_type(itsm.get(),size(),stepi()+stepj(),NonConj); 
        }

        inline const_vec_type diag(int i) const
        {
            TMVAssert(i>=-int(size())); 
            TMVAssert(isunit() ? i<0 : i<=0);
            return const_vec_type(
                itsm.get()-i*stepi(),size()+i,stepi()+stepj(),NonConj);
        }

        inline const_vec_type diag(int i, int j1, int j2) const
        {
            TMVAssert(i>=-int(size())); 
            TMVAssert(isunit() ? i<0 : i<=0);
            if (I == FortranStyle) { 
                TMVAssert(j1>0 && j1<=j2 && j2<=int(size())+i); --j1;
            } else {
                TMVAssert(j1>=0 && j1<=j2 && j2<=int(size())+i);
            }
            const int ds = stepi()+stepj();
            return const_vec_type(
                itsm.get()-i*stepi()+j1*ds,j2-j1,ds,NonConj);
        }

        inline vec_type row(int i, int j1, int j2)
        { 
            if (I==FortranStyle) { 
                TMVAssert(i>0 && i<=int(size())); --i;
                TMVAssert(j1>0 && j1<=j2 && j2<=int(size())); --j1;
            } else {
                TMVAssert(i>=0 && i<int(size()));
                TMVAssert(j1>=0 && j1<=j2 && j2<=int(size()));
            }
            TMVAssert(j1==j2 || okij(i,j2-1));
            return vec_type(
                itsm.get()+i*stepi()+j1*stepj(),
                j2-j1,stepj(),NonConj TMV_FIRSTLAST);
        }

        inline vec_type col(int j, int i1, int i2)
        {
            if (I==FortranStyle) { 
                TMVAssert(j>0 && j<=int(size())); --j;
                TMVAssert(i1>0 && i1<=i2 && i2<=int(size())); --i1;
            } else {
                TMVAssert(j>=0 && j<int(size()));
                TMVAssert(i1>=0 && i1<=i2 && i2<=int(size()));
            }
            TMVAssert(i1==i2 || okij(i1,j));
            return vec_type(
                itsm.get()+i1*stepi()+j*stepj(),i2-i1,stepi(),NonConj 
                TMV_FIRSTLAST);
        }

        inline vec_type diag()
        {
            TMVAssert(!isunit());
            return vec_type(
                itsm.get(),size(),stepi()+stepj(),NonConj TMV_FIRSTLAST); 
        }

        inline vec_type diag(int i)
        {
            TMVAssert(i>=-int(size())); 
            TMVAssert(isunit() ? i<0 : i<=0);
            return vec_type(
                itsm.get()-i*stepi(),size()+i,stepi()+stepj(),NonConj 
                TMV_FIRSTLAST);
        }

        inline vec_type diag(int i, int j1, int j2)
        {
            TMVAssert(i>=-int(size())); 
            TMVAssert(isunit() ? i<0 : i<=0);
            if (I == FortranStyle) { 
                TMVAssert(j1>0 && j1<=j2 && j2<=int(size())+i); --j1;
            } else {
                TMVAssert(j1>=0 && j1<=j2 && j2<=int(size())+i);
            }
            const int ds = stepi()+stepj();
            return vec_type(
                itsm.get()-i*stepi()+j1*ds,j2-j1,ds,NonConj TMV_FIRSTLAST);
        }

        //
        // Modifying Functions
        //

        inline type& setZero() 
        { fill_n(itsm.get(),itslen,T(0)); return *this; }

        inline type& setAllTo(const T& x) 
        { VectorViewOf(itsm.get(),itslen).setAllTo(x); return *this; }

        inline type& clip(RT thresh)
        { VectorViewOf(itsm.get(),itslen).clip(thresh); return *this; }

        inline type& conjugateSelf() 
        { VectorViewOf(itsm.get(),itslen).conjugateSelf(); return *this; }

        inline type& invertSelf()
        { view().invertSelf(); return *this; }

        inline type& setToIdentity(const T& x=T(1)) 
        { 
            TMVAssert(!isunit() || x == T(1));
            setZero(); if (!isunit()) diag().setAllTo(x);
            return *this;
        }

        TMV_DEPRECATED(type& Zero() const)
        { return setZero(); }
        TMV_DEPRECATED(type& SetAllTo(const T& x) const)
        { return setAllTo(x); }
        TMV_DEPRECATED(type& Clip(RT thresh) const)
        { return clip(thresh); }
        TMV_DEPRECATED(type& ConjugateSelf() const)
        { return conjugateSelf(); }
        TMV_DEPRECATED(type& InvertSelf() const)
        { return invertSelf(); }
        TMV_DEPRECATED(type& SetToIdentity(const T& x=T(1)) const)
        { return setToIdentity(x); }


        //
        // subMatrix
        //

        inline const_rec_type subMatrix(
            int i1, int i2, int j1, int j2) const
        {
            TMVAssert(view().hasSubMatrix(i1,i2,j1,j2,1,1));
            if (I==FortranStyle) { --i1; --j1; }
            return const_rec_type(
                itsm.get()+i1*stepi()+j1*stepj(),
                i2-i1, j2-j1,stepi(),stepj(),S,NonConj);
        }

        inline const_rec_type subMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep) const
        {
            StorageType newstor = S==RowMajor ?
                jstep == 1 ? RowMajor : NoMajor :
                istep == 1 ? ColMajor : NoMajor;
            TMVAssert(view().hasSubMatrix(i1,i2,j1,j2,istep,jstep));
            if (I==FortranStyle) { --i1; --j1; i2+=istep-1; j2+=jstep-1; }
            return const_rec_type(
                itsm.get()+i1*stepi()+j1*stepj(),
                (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
                newstor,NonConj);
        }

        inline const_vec_type subVector(
            int i, int j, int istep, int jstep, int size) const
        {
            TMVAssert(view().hasSubVector(i,j,istep,jstep,size));
            if (I==FortranStyle) { --i; --j; }
            return const_vec_type(
                itsm.get()+i*stepi()+j*stepj(),size,
                istep*stepi()+jstep*stepj(),NonConj);
        }

        inline const_lowertri_type subTriMatrix(int i1, int i2) const
        {
            TMVAssert(view().hasSubTriMatrix(i1,i2,1));
            if (I==FortranStyle) { --i1; }
            return const_lowertri_type(
                itsm.get()+i1*(stepi()+stepj()),
                i2-i1,stepi(),stepj(),D,S,NonConj);
        }

        inline const_lowertri_type subTriMatrix(
            int i1, int i2, int istep) const
        {
            TMVAssert(view().hasSubTriMatrix(i1,i2,istep));
            if (I==FortranStyle) { --i1; i2+=istep-1; }
            return const_lowertri_type(
                itsm.get()+i1*(stepi()+stepj()),
                (i2-i1)/istep,istep*stepi(),istep*stepj(),D,
                istep==1 ? S : NoMajor, NonConj);
        }

        inline const_lowertri_type offDiag(int noff=1) const
        {
            TMVAssert(noff <= int(size()));
            return const_lowertri_type(
                itsm.get()+noff*stepi(),size()-noff,
                stepi(),stepj(),NonUnitDiag,S,NonConj);
        }

        inline const_real_type realPart() const
        {
            return const_real_type(
                reinterpret_cast<RT*>(itsm.get()), size(),
                isReal(T()) ? stepi() : 2*stepi(),
                isReal(T()) ? stepj() : 2*stepj(),
                D, isReal(T()) ? S : NoMajor, NonConj
            );
        }

        inline const_real_type imagPart() const
        {
            TMVAssert(isComplex(T()));
            TMVAssert(!isunit());
            return const_real_type(
                reinterpret_cast<RT*>(itsm.get())+1, size(),
                2*stepi(), 2*stepj(), NonUnitDiag, NoMajor, NonConj
            );
        }

        inline rec_type subMatrix(int i1, int i2, int j1, int j2) 
        {
            TMVAssert(view().hasSubMatrix(i1,i2,j1,j2,1,1));
            if (I==FortranStyle) { --i1; --j1; }
            return rec_type(
                itsm.get()+i1*stepi()+j1*stepj(),
                i2-i1, j2-j1, stepi(),stepj(),S,NonConj TMV_FIRSTLAST );
        }

        inline rec_type subMatrix(
            int i1, int i2, int j1, int j2, int istep, int jstep)
        {
            StorageType newstor = S==RowMajor ?
                jstep == 1 ? RowMajor : NoMajor :
                istep == 1 ? ColMajor : NoMajor;
            TMVAssert(view().hasSubMatrix(i1,i2,j1,j2,istep,jstep));
            if (I==FortranStyle) { --i1; --j1; i2+=istep-1; j2+=jstep-1; }
            return rec_type(
                itsm.get()+i1*stepi()+j1*stepj(),
                (i2-i1)/istep, (j2-j1)/jstep, istep*stepi(), jstep*stepj(),
                newstor,NonConj TMV_FIRSTLAST );
        }

        inline vec_type subVector(
            int i, int j, int istep, int jstep, int size)
        {
            TMVAssert(view().hasSubVector(i,j,istep,jstep,size));
            if (I==FortranStyle) { --i; --j; }
            return vec_type(
                itsm.get()+i*stepi()+j*stepj(),size,
                istep*stepi()+jstep*stepj(),NonConj TMV_FIRSTLAST );
        }

        inline lowertri_type subTriMatrix(int i1, int i2)
        {
            TMVAssert(view().hasSubTriMatrix(i1,i2,1));
            if (I==FortranStyle) { --i1; }
            return lowertri_type(
                itsm.get()+i1*(stepi()+stepj()),
                i2-i1,stepi(),stepj(),D,S,NonConj TMV_FIRSTLAST);
        }

        inline lowertri_type subTriMatrix(int i1, int i2, int istep) 
        {
            TMVAssert(view().hasSubTriMatrix(i1,i2,istep));
            if (I==FortranStyle) { --i1; i2+=istep-1; }
            return lowertri_type(
                itsm.get()+i1*(stepi()+stepj()),
                (i2-i1)/istep,istep*stepi(),istep*stepj(),D,
                istep==1 ? S : NoMajor,NonConj TMV_FIRSTLAST);
        }

        inline lowertri_type offDiag(int noff=1)
        {
            TMVAssert(noff <= int(size()));
            return lowertri_type(
                itsm.get()+noff*stepi(),size()-noff,
                stepi(),stepj(),NonUnitDiag,S,NonConj TMV_FIRSTLAST);
        }

        inline real_type realPart() 
        {
            return real_type(
                reinterpret_cast<RT*>(itsm.get()), size(),
                isReal(T()) ? stepi() : 2*stepi(),
                isReal(T()) ? stepj() : 2*stepj(),
                D, isReal(T()) ? S : NoMajor, NonConj
#ifdef TMVFLDEBUG
                ,reinterpret_cast<const RT*>(_first)
                ,reinterpret_cast<const RT*>(_last)
#endif
            );
        }

        inline real_type imagPart()
        {
            TMVAssert(isComplex(T()));
            TMVAssert(!isunit());
            return real_type(
                reinterpret_cast<RT*>(itsm.get())+1,
                size(), 2*stepi(), 2*stepj(), NonUnitDiag, NoMajor, NonConj
#ifdef TMVFLDEBUG
                ,reinterpret_cast<const RT*>(_first)+1
                ,reinterpret_cast<const RT*>(_last)+1
#endif
            );
        }

        inline const_lowertri_type view() const
        { 
            return const_lowertri_type(
                itsm.get(),size(),stepi(),stepj(),D,S,NonConj);
        }

        inline const_lowertri_type viewAsUnitDiag() const
        { 
            return const_lowertri_type(
                itsm.get(),size(),stepi(),stepj(),UnitDiag,S,NonConj);
        }

        inline const_uppertri_type transpose() const
        { 
            return const_uppertri_type(
                itsm.get(),size(),stepj(),stepi(),D,TMV_TransOf(S),NonConj);
        }

        inline const_lowertri_type conjugate() const
        { 
            return const_lowertri_type(
                itsm.get(),size(),stepi(),stepj(),D,S,TMV_ConjOf(T,NonConj));
        }

        inline const_uppertri_type adjoint() const
        { 
            return const_uppertri_type(
                itsm.get(),size(),stepj(),stepi(),D,
                TMV_TransOf(S),TMV_ConjOf(T,NonConj));
        }

        inline lowertri_type view() 
        { 
            return lowertri_type(
                itsm.get(),size(),stepi(),stepj(),D,S,NonConj TMV_FIRSTLAST);
        }

        inline lowertri_type viewAsUnitDiag() 
        { 
            return lowertri_type(
                itsm.get(),size(),stepi(),stepj(),UnitDiag,S,NonConj 
                TMV_FIRSTLAST);
        }

        inline uppertri_type transpose() 
        { 
            return uppertri_type(
                itsm.get(),size(),stepj(),stepi(),D,
                TMV_TransOf(S),NonConj TMV_FIRSTLAST);
        }

        inline lowertri_type conjugate() 
        { 
            return lowertri_type(
                itsm.get(),size(),stepi(),stepj(),D,S,TMV_ConjOf(T,NonConj) 
                TMV_FIRSTLAST);
        }

        inline uppertri_type adjoint() 
        { 
            return uppertri_type(
                itsm.get(),size(),stepj(),stepi(),D,
                TMV_TransOf(S),TMV_ConjOf(T,NonConj) TMV_FIRSTLAST);
        }

        TMV_DEPRECATED(const_rec_type SubMatrix(
                int i1, int i2, int j1, int j2) const)
        { return subMatrix(i1,i2,j1,j2); }
        TMV_DEPRECATED(const_rec_type SubMatrix(
                int i1, int i2, int j1, int j2, int istep, int jstep) const)
        { return subMatrix(i1,i2,j1,j2,istep,jstep); }
        TMV_DEPRECATED(const_vec_type SubVector(
                int i, int j, int istep, int jstep, int s) const)
        { return subVector(i,j,istep,jstep,s); }
        TMV_DEPRECATED(const_lowertri_type SubTriMatrix(int i1, int i2) const)
        { return subTriMatrix(i1,i2); }
        TMV_DEPRECATED(const_lowertri_type SubTriMatrix(
                int i1, int i2, int istep) const)
        { return subTriMatrix(i1,i2,istep); }
        TMV_DEPRECATED(const_lowertri_type OffDiag(int noff=1) const)
        { return offDiag(noff); }
        TMV_DEPRECATED(const_real_type Real() const)
        { return realPart(); }
        TMV_DEPRECATED(const_real_type Imag() const)
        { return imagPart(); }
        TMV_DEPRECATED(const_lowertri_type View() const)
        { return view(); }
        TMV_DEPRECATED(const_lowertri_type ViewAsUnitDiag() const)
        { return viewAsUnitDiag(); }
        TMV_DEPRECATED(const_uppertri_type Transpose() const)
        { return transpose(); }
        TMV_DEPRECATED(const_lowertri_type Conjugate() const)
        { return conjugate(); }
        TMV_DEPRECATED(const_uppertri_type Adjoint() const)
        { return adjoint(); }
        TMV_DEPRECATED(rec_type SubMatrix(int i1, int i2, int j1, int j2))
        { return subMatrix(i1,i2,j1,j2); }
        TMV_DEPRECATED(rec_type SubMatrix(
                int i1, int i2, int j1, int j2, int istep, int jstep))
        { return subMatrix(i1,i2,j1,j2,istep,jstep); }
        TMV_DEPRECATED(vec_type SubVector(
                int i, int j, int istep, int jstep, int s))
        { return subVector(i,j,istep,jstep,s); }
        TMV_DEPRECATED(lowertri_type SubTriMatrix(int i1, int i2))
        { return subTriMatrix(i1,i2); }
        TMV_DEPRECATED(lowertri_type SubTriMatrix(int i1, int i2, int istep))
        { return subTriMatrix(i1,i2,istep); }
        TMV_DEPRECATED(lowertri_type OffDiag(int noff=1))
        { return offDiag(noff); }
        TMV_DEPRECATED(real_type Real())
        { return realPart(); }
        TMV_DEPRECATED(real_type Imag())
        { return imagPart(); }
        TMV_DEPRECATED(lowertri_type View())
        { return view(); }
        TMV_DEPRECATED(lowertri_type ViewAsUnitDiag())
        { return viewAsUnitDiag(); }
        TMV_DEPRECATED(uppertri_type Transpose())
        { return transpose(); }
        TMV_DEPRECATED(lowertri_type Conjugate())
        { return conjugate(); }
        TMV_DEPRECATED(uppertri_type Adjoint())
        { return adjoint(); }


        inline size_t size() const { return itss; }
        inline const T* cptr() const { return itsm.get(); }
        inline T* ptr() { return itsm.get(); }
        inline int stepi() const { return S==RowMajor ? itss : 1; }
        inline int stepj() const { return S==RowMajor ? 1 : itss; }
        inline DiagType dt() const { return D; }
        inline StorageType stor() const { return S; }
        inline ConjType ct() const { return NonConj; }
        inline bool isrm() const { return S==RowMajor; }
        inline bool iscm() const { return S==ColMajor; }
        inline bool isunit() const { return D == UnitDiag; }
        inline bool isconj() const { return false; }

        inline T& ref(int i, int j)
        { return itsm.get()[S==RowMajor ? i*itss + j : j*itss + i]; }

        inline T cref(int i, int j) const 
        {
            return (i==j && D==UnitDiag) ? T(1) : (i<j) ? T(0) :
                itsm.get()[S==RowMajor ? i*itss + j : j*itss + i]; 
        }

    protected :

        const size_t itslen;
        auto_array<T> itsm;
        const size_t itss;

#ifdef TMVFLDEBUG
    public:
        const T*const _first;
        const T*const _last;
    protected :
#endif

        inline bool okij(int i, int j) const
        {
            TMVAssert(i>=0 && i < int(size()));
            TMVAssert(j>=0 && j < int(size()));
            return isunit() ? i>j : i>=j;
        }

    }; // LowerTriMatrix

    //-------------------------------------------------------------------------

    //
    // Special Creators: 
    //   UpperTriMatrixViewOf(T* m, n, S, D)
    //   LowerTriMatrixViewOf(m)
    //   UnitTriMatrixViewOf(t)
    //

    template <class T> 
    inline ConstUpperTriMatrixView<T> UpperTriMatrixViewOf(
        const T* vv, size_t size, StorageType stor, DiagType dt=NonUnitDiag)
    {
        TMVAssert(stor == RowMajor || stor == ColMajor);
        if (stor == RowMajor)
            return ConstUpperTriMatrixView<T>(
                vv,size,size,1,dt,RowMajor,NonConj);
        else
            return ConstUpperTriMatrixView<T>(
                vv,size,1,size,dt,ColMajor,NonConj);
    }

    template <class T> 
    inline UpperTriMatrixView<T> UpperTriMatrixViewOf(
        T* vv, size_t size, StorageType stor, DiagType dt=NonUnitDiag)
    {
        TMVAssert(stor == RowMajor || stor == ColMajor);
        if (stor == RowMajor)
            return UpperTriMatrixView<T>(
                vv,size,size,1,dt,RowMajor,NonConj 
                TMV_FIRSTLAST1(vv,vv+size*size));
        else
            return UpperTriMatrixView<T>(
                vv,size,1,size,dt,ColMajor,NonConj 
                TMV_FIRSTLAST1(vv,vv+size*size));
    }

    template <class T> 
    inline ConstLowerTriMatrixView<T> LowerTriMatrixViewOf(
        const T* vv, size_t size, StorageType stor, DiagType dt=NonUnitDiag)
    {
        TMVAssert(stor == RowMajor || stor == ColMajor);
        if (stor == RowMajor)
            return ConstLowerTriMatrixView<T>(
                vv,size,size,1,dt,RowMajor,NonConj);
        else
            return ConstLowerTriMatrixView<T>(
                vv,size,1,size,dt,ColMajor,NonConj);
    }

    template <class T> 
    inline LowerTriMatrixView<T> LowerTriMatrixViewOf(
        T* vv, size_t size, StorageType stor, DiagType dt=NonUnitDiag)
    {
        TMVAssert(stor == RowMajor || stor == ColMajor);
        if (stor == RowMajor)
            return LowerTriMatrixView<T>(
                vv,size,size,1,dt,RowMajor,NonConj 
                TMV_FIRSTLAST1(vv,vv+size*size));
        else
            return LowerTriMatrixView<T>(
                vv,size,1,size,dt,ColMajor,NonConj 
                TMV_FIRSTLAST1(vv,vv+size*size));
    }

    //
    // Copy
    //

    template <class T1, class T2> 
    inline void nonUnitDiagCopy(
        const GenUpperTriMatrix<T1>& m1, const UpperTriMatrixView<T2>& m2)
    {
        TMVAssert(isReal(T1()) || isComplex(T2()));
        TMVAssert(m1.size() == m2.size());
        TMVAssert(m1.dt() == NonUnitDiag);
        TMVAssert(m2.dt() == NonUnitDiag);
        const int N = m1.size();

        if (!m1.isSameAs(m2) && m1.size() > 0) {
            if (m1.iscm() && m2.iscm()) 
                for(int j=0;j<N;++j) m2.col(j,0,j+1) = m1.col(j,0,j+1);
            else 
                for(int i=0;i<N;++i) m2.row(i,i,N) = m1.row(i,i,N);
        }
    }

    template <class T1, class T2> 
    inline void Copy(
        const GenUpperTriMatrix<T1>& m1, const UpperTriMatrixView<T2>& m2)
    {
        TMVAssert(isReal(T1()) || isComplex(T2()));
        TMVAssert(m1.size() == m2.size());
        TMVAssert(m1.isunit() || !m2.isunit());

        if (m1.isunit()) {
            if (m1.size() > 0)
                nonUnitDiagCopy(m1.offDiag(),m2.offDiag());
            if (!m2.isunit())
                m2.diag().setAllTo(T2(1));
        } else {
            nonUnitDiagCopy(m1,m2);
        }
    }


    //
    // Swap Matrices
    //

    template <class T> 
    void Swap(
        const UpperTriMatrixView<T>& m1, const UpperTriMatrixView<T>& m2);

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline void Swap(
        const UpperTriMatrixView<T>& m1, UpperTriMatrix<T,D,S,I>& m2)
    { Swap(m1,m2.view()); }

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline void Swap(
        UpperTriMatrix<T,D,S,I>& m1, const UpperTriMatrixView<T>& m2)
    { Swap(m1.view(),m2); }

    template <class T, DiagType D, StorageType S1, StorageType S2, IndexStyle I1, IndexStyle I2> 
    inline void Swap(
        UpperTriMatrix<T,D,S1,I1>& m1, UpperTriMatrix<T,D,S2,I2>& m2)
    { Swap(m1.view(),m2.view()); }

    template <class T> 
    inline void Swap(
        const LowerTriMatrixView<T>& m1, const LowerTriMatrixView<T>& m2)
    { Swap(m1.transpose(),m2.transpose()); }

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline void Swap(
        const LowerTriMatrixView<T>& m1, LowerTriMatrix<T,D,S,I>& m2)
    { Swap(m1.transpose(),m2.transpose()); }

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline void Swap(
        LowerTriMatrix<T,D,S,I>& m1, const LowerTriMatrixView<T>& m2)
    { Swap(m1.transpose(),m2.transpose()); }

    template <class T, DiagType D, StorageType S1, StorageType S2, IndexStyle I1, IndexStyle I2> 
    inline void Swap(
        LowerTriMatrix<T,D,S1,I1>& m1, LowerTriMatrix<T,D,S2,I2>& m2)
    { Swap(m1.transpose(),m2.transpose()); }


    //
    // Views:
    //

    template <class T> 
    inline ConstLowerTriMatrixView<T> Transpose(const GenUpperTriMatrix<T>& m)
    { return m.transpose(); }
    template <class T> 
    inline ConstUpperTriMatrixView<T> Transpose(const GenLowerTriMatrix<T>& m)
    { return m.transpose(); }

    template <class T, IndexStyle I> 
    inline ConstLowerTriMatrixView<T,I> Transpose(
        const ConstUpperTriMatrixView<T,I>& m)
    { return m.transpose(); }
    template <class T, IndexStyle I> 
    inline ConstUpperTriMatrixView<T,I> Transpose(
        const ConstLowerTriMatrixView<T,I>& m)
    { return m.transpose(); }

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline ConstLowerTriMatrixView<T,I> Transpose(
        const UpperTriMatrix<T,D,S,I>& m)
    { return m.transpose(); }
    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline ConstUpperTriMatrixView<T,I> Transpose(
        const LowerTriMatrix<T,D,S,I>& m)
    { return m.transpose(); }

    template <class T, IndexStyle I> 
    inline LowerTriMatrixView<T,I> Transpose(const UpperTriMatrixView<T,I>& m)
    { return m.transpose(); }
    template <class T, IndexStyle I> 
    inline UpperTriMatrixView<T,I> Transpose(const LowerTriMatrixView<T,I>& m)
    { return m.transpose(); }

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline LowerTriMatrixView<T,I> Transpose(UpperTriMatrix<T,D,S,I>& m)
    { return m.transpose(); }
    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline UpperTriMatrixView<T,I> Transpose(LowerTriMatrix<T,D,S,I>& m)
    { return m.transpose(); }

    template <class T> 
    inline ConstUpperTriMatrixView<T> Conjugate(const GenUpperTriMatrix<T>& m)
    { return m.conjugate(); }
    template <class T> 
    inline ConstLowerTriMatrixView<T> Conjugate(const GenLowerTriMatrix<T>& m)
    { return m.conjugate(); }

    template <class T, IndexStyle I> 
    inline ConstUpperTriMatrixView<T,I> Conjugate(
        const ConstUpperTriMatrixView<T,I>& m)
    { return m.conjugate(); }
    template <class T, IndexStyle I> 
    inline ConstLowerTriMatrixView<T,I> Conjugate(
        const ConstLowerTriMatrixView<T,I>& m)
    { return m.conjugate(); }

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline ConstUpperTriMatrixView<T,I> Conjugate(
        const UpperTriMatrix<T,D,S,I>& m)
    { return m.conjugate(); }
    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline ConstLowerTriMatrixView<T,I> Conjugate(
        const LowerTriMatrix<T,D,S,I>& m)
    { return m.conjugate(); }

    template <class T, IndexStyle I> 
    inline UpperTriMatrixView<T,I> Conjugate(const UpperTriMatrixView<T,I>& m)
    { return m.conjugate(); }
    template <class T, IndexStyle I> 
    inline LowerTriMatrixView<T,I> Conjugate(const LowerTriMatrixView<T,I>& m)
    { return m.conjugate(); }

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline UpperTriMatrixView<T,I> Conjugate(UpperTriMatrix<T,D,S,I>& m)
    { return m.conjugate(); }
    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline LowerTriMatrixView<T,I> Conjugate(LowerTriMatrix<T,D,S,I>& m)
    { return m.conjugate(); }

    template <class T> 
    inline ConstLowerTriMatrixView<T> Adjoint(const GenUpperTriMatrix<T>& m)
    { return m.adjoint(); }
    template <class T> 
    inline ConstUpperTriMatrixView<T> Adjoint(const GenLowerTriMatrix<T>& m)
    { return m.adjoint(); }

    template <class T, IndexStyle I> 
    inline ConstLowerTriMatrixView<T,I> Adjoint(
        const ConstUpperTriMatrixView<T,I>& m)
    { return m.adjoint(); }
    template <class T, IndexStyle I> 
    inline ConstUpperTriMatrixView<T,I> Adjoint(
        const ConstLowerTriMatrixView<T,I>& m)
    { return m.adjoint(); }

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline ConstLowerTriMatrixView<T,I> Adjoint(
        const UpperTriMatrix<T,D,S,I>& m)
    { return m.adjoint(); }
    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline ConstUpperTriMatrixView<T,I> Adjoint(
        const LowerTriMatrix<T,D,S,I>& m)
    { return m.adjoint(); }

    template <class T, IndexStyle I> 
    inline LowerTriMatrixView<T,I> Adjoint(const UpperTriMatrixView<T,I>& m)
    { return m.adjoint(); }
    template <class T, IndexStyle I> 
    inline UpperTriMatrixView<T,I> Adjoint(const LowerTriMatrixView<T,I>& m)
    { return m.adjoint(); }

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline LowerTriMatrixView<T,I> Adjoint(UpperTriMatrix<T,D,S,I>& m)
    { return m.adjoint(); }
    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline UpperTriMatrixView<T,I> Adjoint(LowerTriMatrix<T,D,S,I>& m)
    { return m.adjoint(); }

    template <class T> 
    inline QuotXU<T,T> Inverse(const GenUpperTriMatrix<T>& m)
    { return m.inverse(); }
    template <class T> 
    inline QuotXL<T,T> Inverse(const GenLowerTriMatrix<T>& m)
    { return m.inverse(); }


    //
    // TriMatrix ==, != TriMatrix
    //

    template <class T1, class T2> 
    bool operator==(
        const GenUpperTriMatrix<T1>& m1, const GenUpperTriMatrix<T2>& m2);
    template <class T1, class T2> 
    inline bool operator==(
        const GenLowerTriMatrix<T1>& m1, const GenLowerTriMatrix<T2>& m2)
    { return m1.transpose() == m2.transpose(); }
    template <class T1, class T2> 
    inline bool operator!=(
        const GenUpperTriMatrix<T1>& m1, const GenUpperTriMatrix<T2>& m2)
    { return !(m1 == m2); }
    template <class T1, class T2> 
    inline bool operator!=(
        const GenLowerTriMatrix<T1>& m1, const GenLowerTriMatrix<T2>& m2)
    { return !(m1 == m2); }


    //
    // I/O
    //

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    std::istream& operator>>(
        std::istream& is, auto_ptr<UpperTriMatrix<T,D,S,I> >& m);
    template <class T, DiagType D, StorageType S, IndexStyle I> 
    std::istream& operator>>(
        std::istream& is, auto_ptr<LowerTriMatrix<T,D,S,I> >& m);

    template <class T> 
    std::istream& operator>>(std::istream& is, const UpperTriMatrixView<T>& m);
    template <class T> 
    std::istream& operator>>(std::istream& is, const LowerTriMatrixView<T>& m);

    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline std::istream& operator>>(
        std::istream& is, UpperTriMatrix<T,D,S,I>& m)
    { return is>>m.view(); }
    template <class T, DiagType D, StorageType S, IndexStyle I> 
    inline std::istream& operator>>(
        std::istream& is, LowerTriMatrix<T,D,S,I>& m)
    { return is>>m.view(); }

} // namespace tmv

#endif