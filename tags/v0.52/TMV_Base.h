//-----------------------------------------------------------------------------
//
// This file defines some basic helper functions used by the TMV Matrix
// and Vector classes.  
//
// Probably the only thing someone might care about here is the
// class tmv::tmv_exception which gets thrown on run-time errors,
// so you could catch it if you don't want the program to crash.
//
// Also, it is a good idea to compile any code that uses TMV with 
// TMVDEBUG defined.  (defined below in this file)
//
// This will turn on some basic debugging in the code and will help 
// you catch programming mistakes.
// For example, if you have two vectors: 
// v1 of length 5 and v2  of lengths 10,
// then v1 = v2 will only produce a runtime error if TMVDEBUG is 
// turned on.  Otherwise some random data will be overwritten and 
// who knows what will happen then.
//
// Once you know your code is working properly, you can recompile without
// the TMVDEBUG flag to speed up the code.
// (And actually, the slow down isn't much, so it is probably
// a good idea to just leave it on to help diagnose any bug that 
// might turn up.)
//
// A comment here about the letters used to identify each type of Matrix.
// The WriteCompact routines for sparse Matrix types have an identifying
// letter(s) before the output.  The same letter (or combination) is 
// used in the names of the composite arithmetic types like SumDD, etc.
// Here is the list of letters used for reference:
//
// (* indicates that the type is not yet implemented.)
//
//  X   Scalar 
//  V   Vector
//  M   Matrix
//  D   DiagMatrix
//  U   UpperTriMatrix
//  L   LowerTriMatrix
//  B   BandMatrix
//  S   SymMatrix
//  H   HermMatrix
// *v   SmallVector
// *m   SmallMatrix
// *sB  SymBandMatrix 
// *hB  HermBandMatrix  
// *kD  BlockDiagMatrix (B was taken, so K = Block) 
// *skD SymBlockDiagMatrix 
// *hkD HermBlockDiagMatrix 
// *Q   SparseMatrix (S was taken, so Q = Sparse) 
// *sQ  SymSparseMatrix
// *hQ  HermSparseMatrix
// *kQ  BlockSparseMatrix 
// *skQ SymBlockSparseMatrix 
// *hkQ HermBlockSparseMatrix 
//
// MJ  SymBandMatrix
// MJ  BlockDiagMatrix and Sym varieties
// MJ  SparseMatrix and Sym, Block varieties
// MJ  SmallMatrix (nrows,ncols are templated)


#ifndef TMV_Base_H
#define TMV_Base_H

#ifndef NDEBUG
#define TMVDEBUG
#endif

#include <iostream>
#include <cmath>
#include <complex>
#include <valarray>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>
#include <typeinfo>

#ifndef NOFLOAT
#define INST_FLOAT
#endif
#ifdef LONGDOUBLE
#define INST_LONGDOUBLE
#endif
//#define INST_INT

namespace tmv {

  using std::abs;
  using std::complex;
  using std::vector;
  using std::valarray;
  using std::istream;
  using std::ostream;
  using std::endl;
  using std::cerr;
  using std::cout;
  using std::swap;
  using std::min;
  using std::max;
  using std::string;

  enum DivType { XXX, LU, CH, QR, QRP, SV, SVS, SVU, SVV };
  inline bool SV_Type(const DivType dt)
  { return dt >= SV && dt <= SVV; }

  // MJ: Add the packed storage varieties: RowPack, ColPack, DiagPack
  enum StorageType { RowMajor, ColMajor, DiagMajor, NoMajor };

  enum UpLoType { Upper, Lower };

  enum DiagType { UnitDiag, NonUnitDiag };

  // MJ: Any reason to add AntiSym, AntiHerm? Are they useful?
  enum SymType { Sym, Herm /*, AntiSym, AntiHerm*/ };

  template <class T> inline T SQR(T x) 
  { return x*x; }

  template <class T> inline T SQRT(T x) 
  { return sqrt(x); }

  template <class T> inline T NORM(T x) 
  { return x*x; }

  template <class T> inline T NORM(complex<T> x) 
  { return std::norm(x); }

  template <class T> inline T CONJ(T x)
  { return x; }

  template <class T> inline complex<T> CONJ(complex<T> x)
  { return std::conj(x); }

  template <class T> inline T REAL(T x)
  { return x; }

  template <class T> inline T REAL(complex<T> x)
  { return std::real(x); }

  template <class T> inline T IMAG(T x)
  { return T(0); }

  template <class T> inline T IMAG(complex<T> x)
  { return std::imag(x); }

  template <class T> inline T ARG(T x)
  { return x >= T(0) ? T(1) : T(-1); }

  template <class T> inline T ARG(complex<T> x)
  { return arg(x); }

  template <class T> inline T SIGN(T x, T absx)
  { return x > 0 ? 1 : -1; }

  template <class T> inline complex<T> SIGN(complex<T> x, T absx)
  { return absx > 0 ? x/absx : complex<T>(1); }

  template <class T> inline T MAXABS(complex<T> x)
  { return max(abs(real(x)),abs(imag(x))); }

  template <class T> inline T MAXABS(T x)
  { return abs(x); }

  template <class T> class RCTypeClass 
  {
    public:
      typedef T RealType;
      typedef complex<T> ComplexType;
  };

  template <class T> class RCTypeClass<complex<T> >
  {
    public:
      typedef T RealType;
      typedef complex<T> ComplexType;
  };

#define RealType(T) typename tmv::RCTypeClass<T>::RealType
#define ComplexType(T) typename tmv::RCTypeClass<T>::ComplexType

  template <class T> inline bool IsReal(T) { return true; }
  template <class T> inline bool IsReal(complex<T>) { return false; }
  template <class T> inline bool IsComplex(T) { return !IsReal(T()); }
  template <class T1, class T2> inline bool SameType(T1,T2) { return false; }
  template <class T> inline bool SameType(T,T) { return true; }

  class tmv_exception {};

#ifndef TMVDEBUG
#define TMVAssert(x)
#else
#define TMVAssert(x) if(!(x)) { \
  cerr<<"Error - TMV Assert " #x " failed\n"; \
  cerr<<"on line "<<__LINE__<<" in file "<<__FILE__<<endl; \
  throw tmv_exception(); }
#endif

  inline void tmv_error(const char *s,const char *s2 = "")
  {
    cerr << "TMV Error: " << s << ' ' << s2 << endl;
    throw tmv_exception();
  }

  template <class T> inline RealType(T) Epsilon()
  { return std::numeric_limits<RealType(T)>::epsilon(); }

  template <class T> inline RealType(T) SqrtEpsilon()
  { 
    static const RealType(T) save = SQRT(Epsilon<T>());
    return save;
  }

  template <class T> inline string Type(T)
  { return string("Unknown (") + typeid(T()).name() + ")"; }

  template <> inline string Type(double)
  { return "double"; }

#ifdef INST_FLOAT
  template <> inline string Type(float)
  { return "float"; }
#endif

#ifdef INST_INT
  inline int Epsilon<int>()
  { TMVAssert(false); return 1; }

  inline int abs(const complex<int>& z)
  { TMVAssert(false); return int(floor(abs(z))); }

  template <> inline int SQRT(int x) 
  { TMVAssert(false); return int(floor(sqrt(x))); }

  template <> inline string Type(int)
  { return "int"; }
#endif

#ifdef INST_LONGDOUBLE
  template <> inline long double SQRT(long double x)
  { return sqrtl(x); }

  template <> inline string Type(long double)
  { return "long double"; }
#endif

  template <class T> inline string Type(complex<T>)
  { return string("complex<") + Type(T()) + ">"; }

  inline string Text(UpLoType u)
  { return u == Upper ? "Upper" : "Lower"; }

  inline string Text(DiagType u)
  { return u == UnitDiag ? "UnitDiag" : "NonUnitDiag"; }

  inline string Text(SymType s)
  { 
    return s == Sym ? "Sym" : "Herm";
    //return s == Sym ? "Sym" : s == Herm ? "Herm" :
    //  s == AntiSym ? "AntiSym" : "AntiHerm";
  }

  inline string Text(DivType d)
  { 
    return 
      d==XXX ? "XXX" :
      d==LU ? "LU" :
      d==CH ? "CH" :
      d==QR ? "QR" :
      d==QRP ? "QRP" :
      d==SV ? "SV" :
      d==SVS ? "SVS" :
      d==SVU ? "SVU" :
      d==SVV ? "SVV" :
      "unkown dt";
  }

} // namespace tmv

#include "TMV_Blas.h"

#endif
