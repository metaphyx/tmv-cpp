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


//#define XDEBUG


#include "TMV_SymLDLDiv.h"
#include "tmv/TMV_SymMatrix.h"
#include "tmv/TMV_Vector.h"
#include "tmv/TMV_TriMatrix.h"
#include "tmv/TMV_VIt.h"
#include "tmv/TMV_TriMatrixArith.h"
#include "tmv/TMV_SymMatrixArith.h"
#include "tmv/TMV_MatrixArith.h"

#ifdef XDEBUG
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#endif

namespace tmv {

#ifdef TMV_BLOCKSIZE
#define SYM_LDL_BLOCKSIZE2 (TMV_BLOCKSIZE/2)
#else
#define SYM_LDL_BLOCKSIZE2 32
#endif

    template <bool herm, class T> 
    static void LDLt_InvertComponents(
        const SymMatrixView<T>& sinv, const VectorView<T>& xD)
    {
        TMVAssert(isReal(T()) || herm == sinv.isherm());
        TMVAssert(sinv.uplo() == Lower);
        TMVAssert(sinv.size() == xD.size()+1);
        TMVAssert(xD.step() == 1);

#ifdef XDEBUG
        Matrix<T> sinv0(sinv);
        Matrix<T> D0(DiagMatrixViewOf(sinv.diag()));
        D0.diag(-1) = xD;
        D0.diag(1) = sinv.isherm() ? xD.conjugate() : xD.view();
        Matrix<T> L0(sinv.lowerTri(UnitDiag));
#endif

        LowerTriMatrixView<T> L = sinv.lowerTri(UnitDiag);
        L.invertSelf();

        T* Di = sinv.diag().ptr();
        const int Dstep = sinv.diag().step();
        T* xDi = xD.ptr();
        const int N = sinv.size();
        for(int i=0;i<N;) {
            if (i==N-1 || *xDi == T(0)) { // Then 1x1
#ifdef TMVFLDEBUG
                TMVAssert(Di >= sinv.first);
                TMVAssert(Di < sinv.last);
#endif
                if (herm)
                    *Di = TMV_RealType(T)(1)/TMV_REAL(*Di);
                else
                    *Di = TMV_RealType(T)(1)/(*Di);
                Di+=Dstep; ++xDi; ++i;
            } else { // 2x2
#ifdef TMVFLDEBUG
                TMVAssert(Di >= sinv.first);
                TMVAssert(Di < sinv.last);
                TMVAssert(Di+Dstep >= sinv.first);
                TMVAssert(Di+Dstep < sinv.last);
                TMVAssert(xDi >= xD.first);
                TMVAssert(xDi < xD.last);
#endif
                SymInvert_2x2<herm>(*Di,*(Di+Dstep),*xDi);
                Di+=2*Dstep; xDi+=2; i+=2;
            }
        }
#ifdef XDEBUG
        TMV_RealType(T) normdiff1 = Norm(L0 * L - T(1));
        Matrix<T> D2(DiagMatrixViewOf(sinv.diag()));
        D2.diag(-1) = xD;
        D2.diag(1) = sinv.isherm() ? xD.conjugate() : xD.view();
        TMV_RealType(T) normdiff2 = Norm(D0 * D2 - T(1));
        TMV_RealType(T) kappa1 = Norm(L0) * Norm(L);
        TMV_RealType(T) kappa2 = Norm(D0) * Norm(D2);
        if (normdiff1 > 0.0001*kappa1*sinv.size() || 
            normdiff2 > 0.0001*kappa2*sinv.size()) {
            cerr<<"LDLInverse - Invert Components\n";
            cerr<<"sinv input = "<<sinv0<<endl;
            cerr<<"sinv output = "<<sinv<<endl;
            cerr<<"A = "<<L0 * D0 * (sinv.issym() ? L0.transpose() : L0.adjoint())<<endl;
            cerr<<"xD = "<<xD<<endl;
            cerr<<"sinv = "<<sinv<<endl;
            cerr<<"D = "<<D0<<endl;
            cerr<<"D.inverse = "<<D2<<endl;
            cerr<<"L = "<<L0<<endl;
            cerr<<"L.inverse = "<<L<<endl;
            cerr<<"Norm(D*Dinv-1) = "<<normdiff1<<endl;
            cerr<<"Norm(L*Linv-1) = "<<normdiff2<<endl;
            cerr<<"kappa = "<<kappa1<<"  "<<kappa2<<endl;
            abort();
        }
#endif
    }

    template <bool herm, class T> 
    static void LDLt_AddXtDX(
        const SymMatrixView<T>& sinv, const GenMatrix<T>& X,
        const GenVector<T>& D, const GenVector<T>& xD)
        // sinv += Xt D X
    {
        TMVAssert(isReal(T()) || herm == sinv.isherm());
        TMVAssert(sinv.uplo() == Lower);
        TMVAssert(xD.step() == 1);
        TMVAssert(sinv.size() == X.rowsize());
        TMVAssert(X.colsize() == xD.size()+1);
        TMVAssert(X.colsize() == D.size());

#ifdef XDEBUG
        Matrix<T> sinv0(sinv);
        Matrix<T> X0(X);
        Vector<T> D0(D);
        Vector<T> xD0(xD);
        Matrix<T> DD(DiagMatrixViewOf(D));
        if (D.size() > 1) {
            DD.diag(-1) = xD;
            DD.diag(1) = herm ? xD.conjugate() : xD.view();
        }
        Matrix<T> m1 = sinv + (herm ? X.adjoint() : X.transpose())*DD*X;
#endif

        const int N = D.size();
        if (N == 1) {
            sinv += D(0) * ((herm ? X.row(0).conjugate() : X.row(0)) ^ X.row(0));
        } else if ((N == 2 && xD(0) != T(0)) || N <= SYM_LDL_BLOCKSIZE2) {
            Matrix<T> DX = X;
            PseudoDiag_LMultEq<herm>(D,xD,DX.view());
            SymMultMM<true>(T(1),herm?X.adjoint():X.transpose(),DX,sinv);
        } else {
            int No2 = N/2;
            if (xD(No2-1) != T(0)) ++No2;
            TMVAssert(xD(No2-1) == T(0));
            LDLt_AddXtDX<herm>(
                sinv,X.rowRange(0,No2),
                D.subVector(0,No2),xD.subVector(0,No2-1));
            LDLt_AddXtDX<herm>(
                sinv,X.rowRange(No2,N),
                D.subVector(No2,N),xD.subVector(No2,N-1));
        }
#ifdef XDEBUG
        if (Norm(m1-sinv) > 0.0001*Norm(m1)) {
            cerr<<"AddXtDX: \n";
            cerr<<"init sinv = "<<sinv0<<endl;
            cerr<<"xD = "<<xD0<<endl;
            cerr<<"D = "<<D0<<endl;
            cerr<<"DD = "<<DD<<endl;
            cerr<<"X = "<<X0<<endl;
            cerr<<"right answer = "<<m1<<endl;
            cerr<<"sinv => "<<sinv<<endl;
            abort();
        }
#endif
    }

    template <bool herm, class T> 
    static void LDLt_CombineInverse(
        const SymMatrixView<T>& sinv, const VectorView<T>& xDinv)
        // Do LDLt inverse recursively:
        // A = [ L00  0  ] [ D00  0  ] [ L00t L10t ]
        //     [ L10 L11 ] [  0  D11 ] [  0   L11t ]
        //
        // A^-1 = [ (L^-1)00t (L^-1)10t ] [ D00^-1  0   ] [ (L^-1)00    0     ]
        //        [     0     (L^-1)11t ] [  0   D11^-1 ] [ (L^-1)10 (L^-1)11 ]
        // Let L^-1 = X
        //      = [ X00t D00^-1 X00 + X10t D11^-1 X10    X10t D11^-1 X11  ]
        //        [       X11t D11^-1 X10                X11t D11^-1 X11  ]
    {
        TMVAssert(isReal(T()) || herm == sinv.isherm());
        TMVAssert(sinv.uplo() == Lower);
        TMVAssert(sinv.size() == xDinv.size()+1);
        TMVAssert(xDinv.step() == 1);

        const int N = sinv.size();

#ifdef XDEBUG
        //cerr<<"Start LDLt_CombineInverse\n";
        //cerr<<"sinv = "<<sinv<<std::endl;
        //cerr<<"xDinv = "<<xDinv<<std::endl;
        //cerr<<"herm = "<<herm<<std::endl;
        //cerr<<"N = "<<N<<std::endl;
        Matrix<T> sinv0(sinv);
        Matrix<T> Dinv(DiagMatrixViewOf(sinv.diag()));
        Dinv.diag(-1) = xDinv;
        Dinv.diag(1) = herm ? xDinv.conjugate() : xDinv;
        Matrix<T> minv1 = 
            (herm ? sinv.lowerTri(UnitDiag).adjoint() :
             sinv.lowerTri(UnitDiag).transpose()) *
            Dinv * sinv.lowerTri(UnitDiag);
        //cerr<<"minv1 = "<<minv1<<std::endl;
#endif

        if (N > 1) {
            if (N == 2) {
                if (xDinv(0) != T(0)) {
                    sinv(1,0) = xDinv(0);
                } else {
                    if (herm) {
                        TMV_RealType(T) D11inv = TMV_REAL(sinv(1,1));
                        T X10 = sinv(1,0);
                        T temp = D11inv * X10;
                        sinv(1,0) = temp;
                        sinv(0,0) += TMV_REAL(TMV_CONJ(X10) * temp);
                    } else {
                        T D11inv = sinv(1,1);
                        T X10 = sinv(1,0);
                        T temp = D11inv * X10;
                        sinv(1,0) = temp;
                        sinv(0,0) += X10 * temp;
                    }
                }
            } else {
                int No2 = N/2;
                if (xDinv(No2-1) != T(0)) ++No2;
                TMVAssert(xDinv(No2-1) == T(0));
                TMVAssert(No2 < N);
                MatrixView<T> X10 = sinv.subMatrix(No2,N,0,No2);
                //cerr<<"sinv.lowerTri(Unit) = "<<sinv.lowerTri(UnitDiag)<<std::endl;
                //cerr<<"No2 = "<<No2<<std::endl;
                //cerr<<"N = "<<N<<std::endl;
                //cerr<<"N-No2 = "<<N-No2<<std::endl;
                LowerTriMatrixView<T> X11 = 
                    sinv.lowerTri(UnitDiag).subTriMatrix(No2,N);
                //cerr<<"X11 = "<<X11<<std::endl;
                VectorView<T> D = sinv.diag();
                //cerr<<"D = "<<D<<std::endl;

                LDLt_CombineInverse<herm>(
                    sinv.subSymMatrix(0,No2),xDinv.subVector(0,No2-1));
                LDLt_AddXtDX<herm>(
                    sinv.subSymMatrix(0,No2),X10,
                    D.subVector(No2,N),xDinv.subVector(No2,N-1));
                PseudoDiag_LMultEq<herm>(
                    D.subVector(No2,N),xDinv.subVector(No2,N-1),X10);
                //cerr<<"X11.adjoint() = "<<X11.adjoint()<<std::endl;
                //cerr<<"X11.transpose() = "<<X11.transpose()<<std::endl;
                if (herm) X10 = X11.adjoint() * X10;
                else X10 = X11.transpose() * X10;
                //cerr<<"X10 => "<<X10<<std::endl;
                LDLt_CombineInverse<herm>(
                    sinv.subSymMatrix(No2,N),xDinv.subVector(No2,N-1));
            }
        }
#ifdef XDEBUG
        //cerr<<"Done: sinv = "<<sinv<<std::endl;
        if (Norm(minv1-sinv) > 0.0001*Norm(sinv)) {
            cerr<<"Combine inverse\n";
            cerr<<"Init sinv = "<<sinv0<<endl;
            cerr<<"xDinv = "<<xDinv<<endl;
            cerr<<"Right answer is "<<minv1<<endl;
            cerr<<"sinv = "<<sinv<<endl;
            cerr<<"Norm(diff) = "<<Norm(minv1-sinv)<<endl;
            abort();
        }
#endif
    }

    template <class T, class T1> 
    void LDL_Inverse(
        const GenSymMatrix<T>& LLx, const GenVector<T>& xD, const int* P,
        const SymMatrixView<T1>& sinv)
    {
        TMVAssert(sinv.size() == LLx.size());
        TMVAssert(isReal(T()) || LLx.isherm() == sinv.isherm());
        TMVAssert(isReal(T()) || LLx.issym() == sinv.issym());

#ifdef XDEBUG
        Matrix<T> D(DiagMatrixViewOf(LLx.diag()));
        D.diag(-1) = xD;
        D.diag(1) = LLx.isherm() ? xD.conjugate() : xD.view();
        Matrix<T> A = LLx.lowerTri(UnitDiag) * D * LLx.upperTri(UnitDiag);
        A.reversePermuteRows(P);
        A.reversePermuteCols(P);
        //cout<<"Start LDL_Inverse: \n";
        //cout<<"sinv = "<<TMV_Text(sinv)<<endl;
        //cout<<"A = "<<A<<endl;
        //cout<<"L = "<<LLx.lowerTri(UnitDiag)<<endl;
        //cout<<"D = "<<D<<endl;
#endif

        if (sinv.size() > 0) {
            if (sinv.isconj()) {
                LDL_Inverse(LLx.conjugate(),xD.conjugate(),P,sinv.conjugate());
            } else if (sinv.uplo() == Upper) {
                if (sinv.issym()) {
                    LDL_Inverse(LLx,xD,P,sinv.transpose());
                } else {
                    LDL_Inverse(
                        LLx.conjugate(),xD.conjugate(),P,sinv.transpose());
                }
            } else if (!(sinv.iscm() || sinv.isrm())) {
                Matrix<T,ColMajor> minv1(LLx.size(),LLx.size());
                SymMatrixView<T> sinv1 = LLx.isherm() ?
                    HermMatrixViewOf(minv1,Lower) :
                    SymMatrixViewOf(minv1,Lower);
                LDL_Inverse(LLx,xD,P,sinv1);
                sinv = sinv1;
            } else {
                sinv = LLx;
                Vector<T1> xDinv = xD;
                if (sinv.isherm()) {
                    LDLt_InvertComponents<true>(sinv,xDinv.view());
                    LDLt_CombineInverse<true>(sinv,xDinv.view());
                } else {
                    LDLt_InvertComponents<false>(sinv,xDinv.view());
                    LDLt_CombineInverse<false>(sinv,xDinv.view());
                }
                sinv.reversePermuteRowsCols(P);
            }
        }

#ifdef XDEBUG
        TMV_RealType(T) normdiff = Norm(A * sinv - T(1));
        TMV_RealType(T) kappa = Norm(A) * Norm(sinv);
        if (normdiff > 0.0001*kappa*sinv.size()) {
            cerr<<"LDLInverse\n";
            cerr<<"A = "<<A<<endl;
            cerr<<"LLx = "<<LLx<<endl;
            cerr<<"xD = "<<xD<<endl;
            cerr<<"sinv = "<<sinv<<endl;
            cerr<<"A.inverse = "<<A.inverse()<<endl;
            cerr<<"A*sinv = "<<A*sinv<<endl;
            cerr<<"sinv*A = "<<sinv*A<<endl;
            cerr<<"Norm(A*sinv-1) = "<<Norm(A*sinv-T(1))<<endl;
            cerr<<"Norm(sinv*A-1) = "<<Norm(sinv*A-T(1))<<endl;
            cerr<<"kappa = "<<kappa<<endl;
            abort();
        }
#endif
        TMVAssert(sinv.isHermOK());
    }

#define InstFile "TMV_SymLDLInverse.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv

