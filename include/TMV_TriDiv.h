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


//---------------------------------------------------------------------------
//
// This file contains the code for doing division of Triangular matrices.
//
// This is done using back or forward substitution.


#ifndef TMV_TriDiv_H
#define TMV_TriDiv_H

#include "TMV_TriMatrix.h"

namespace tmv {

  template <class T> class SingularUpperTriMatrix :
    public Singular
  {
    public:
      UpperTriMatrix<T> A;

      SingularUpperTriMatrix(const GenUpperTriMatrix<T>& _A) :
	Singular("UpperTriMatrix"), A(_A) {}
      ~SingularUpperTriMatrix() throw() {}
      void Write(std::ostream& os) const throw()
      { Singular::Write(os); os<<A<<std::endl; }
  };

  template <class T> class SingularLowerTriMatrix :
    public Singular
  {
    public:
      LowerTriMatrix<T> A;

      SingularLowerTriMatrix(const GenLowerTriMatrix<T>& _A) :
	Singular("LowerTriMatrix"), A(_A) {}
      ~SingularLowerTriMatrix() throw() {}
      void Write(std::ostream& os) const throw()
      { Singular::Write(os); os<<A<<std::endl; }
  };

  template <class T, class T1> void Tri_LDivEq(
      const GenUpperTriMatrix<T1>& A, const VectorView<T>& v);
  template <class T, class T1> void Tri_LDivEq(
      const GenLowerTriMatrix<T1>& A, const VectorView<T>& v);
  template <class T, class T1> void Tri_LDivEq(
      const GenUpperTriMatrix<T1>& A, const MatrixView<T>& m);
  template <class T, class T1> void Tri_LDivEq(
      const GenLowerTriMatrix<T1>& A, const MatrixView<T>& m);
  template <class T, class T1> void Tri_LDivEq(
      const GenUpperTriMatrix<T1>& A, const UpperTriMatrixView<T>& m);
  template <class T, class T1> void Tri_LDivEq(
      const GenLowerTriMatrix<T1>& A, const LowerTriMatrixView<T>& m);

  template <class T> void Tri_Inverse(const UpperTriMatrixView<T>& minv);

} // namespace mv

#endif
