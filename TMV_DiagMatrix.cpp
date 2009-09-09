
#include "TMV_Diag.h"

namespace tmv {

  template <class T> void GenDiagMatrix<T>::NewDivider() const
  { this->itsdiv = new DiagDiv<T>(*this); }

  template <class T> istream& operator>>(istream& fin, DiagMatrix<T>* m)
  {
    size_t s;
    fin >> s;
    if (!fin) tmv_error("reading size in DiagMatrix::Read");
    m = new DiagMatrix<T>(s);
    m->diag().Read(fin);
    return fin;
  }

  template <class T> istream& operator>>(istream& fin,
      const DiagMatrixView<T>& m)
  {
    size_t s;
    fin >> s;
    if (!fin) tmv_error("reading size in DiagMatrix::Read");
    TMVAssert(m.colsize() == s);
    m.diag().Read(fin);
    return fin;
  }



#define InstFile "TMV_DiagMatrix.inst"
#include "TMV_Inst.h"
#undef InstFile

} // namespace mv


