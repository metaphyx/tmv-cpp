
//#define PRINTALGO_QR
//#define XDEBUG_QR

#ifdef NOGEQP3
#ifdef LAP
#undef LAP
#endif
#endif

#include "tmv/TMV_Vector.h"
#include "tmv/TMV_UnpackQ.h"
#include "tmv/TMV_AddVV.h"
#include "tmv/TMV_AddMM.h"
#include "tmv/TMV_Norm.h"

#include "TMV_Blas.h"
#include "tmv/TMV_QRPDecompose.h"
#include "tmv/TMV_Matrix.h"
#include "tmv/TMV_SmallMatrix.h"
#include "tmv/TMV_TriMatrix.h"
#include "tmv/TMV_SmallTriMatrix.h"
#include "tmv/TMV_Vector.h"
#include "tmv/TMV_SmallVector.h"
#include "tmv/TMV_CopyV.h"
#include "tmv/TMV_SwapV.h"
#include "tmv/TMV_MinMax.h"
#include "tmv/TMV_CopyM.h"
#include "tmv/TMV_Rank1VVM.h"
#include "tmv/TMV_MultMV.h"
#include "tmv/TMV_NormM.h"
#include "tmv/TMV_MultMM.h"
#include "tmv/TMV_CopyU.h"
#include "tmv/TMV_MultUM.h"

namespace tmv {

#ifdef LAP
    template <class T, int S, class RT> 
    static inline void LapQRP_Decompose(
        MatrixView<T,S> A, VectorView<RT,Unit> beta, int* P, bool strict)
    { InlineQRP_Decompose(A,beta,P,strict); }
#endif // LAP

    template <class T, class RT> 
    void InstQRP_Decompose(
        MatrixView<T> A, VectorView<RT> beta, int* P, bool strict)
    {
        if (A.rowsize() > 0) {
            if (beta.step() == 1) {
                VectorView<RT,Unit> beta1 = beta;
                if (A.iscm()) {
                    MatrixView<T,ColMajor> Acm = A;
#ifdef LAP
                    LapQRP_Decompose(Acm,beta1,P,strict);
#else
                    InlineQRP_Decompose(Acm,beta1,P,strict);
#endif
#ifndef LAP
                } else if (A.isrm()) {
                    MatrixView<T,RowMajor> Arm = A;
                    InlineQRP_Decompose(Arm,beta1,P,strict);
#endif
                } else {
                    Matrix<T,ColMajor|NoDivider> Ac = A;
                    InstQRP_Decompose(Ac.xView(),beta,P,strict);
                    InstCopy(Ac.constView().xView(),A);
                }
            } else {
                Vector<RT> betac = beta;
                InstQRP_Decompose(A,betac.xView(),P,strict);
                InstCopy(betac.constView().xView(),beta);
            }
        }
    }

#define InstFile "TMV_QRPDecompose.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace tmv

