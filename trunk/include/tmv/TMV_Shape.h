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

#ifndef TMV_Shape_H
#define TMV_Shape_H

namespace tmv {

    enum Shape { Rec, Diag, UpperTri, LowerTri, UnitUpperTri, UnitLowerTri,
        Band, UpperBand, LowerBand, UnitUpperBand, UnitLowerBand,
        RealSym, Sym, Herm, RealSymBand, SymBand, HermBand };

    template <int S> 
    struct ShapeTraits;

    template <> 
    struct ShapeTraits<Rec>
    {
        enum { square = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = true };
        enum { lower = true };
        enum { band = false };
        enum { unit = false };
        enum { inverse_shape = Rec };
        enum { nonunit_shape = Rec };
    };

    template <> 
    struct ShapeTraits<Diag>
    {
        enum { square = true };
        enum { sym = false };
        enum { herm = false };
        enum { upper = false };
        enum { lower = false };
        enum { band = true };
        enum { unit = false };
        enum { inverse_shape = Diag };
        enum { nonunit_shape = Diag };
    };

    template <> 
    struct ShapeTraits<UpperTri>
    {
        enum { square = true };
        enum { sym = false };
        enum { herm = false };
        enum { upper = true };
        enum { lower = false };
        enum { band = false };
        enum { unit = false };
        enum { inverse_shape = UpperTri };
        enum { nonunit_shape = UpperTri };
    };

    template <> 
    struct ShapeTraits<LowerTri>
    {
        enum { square = true };
        enum { sym = false };
        enum { herm = false };
        enum { upper = false };
        enum { lower = true };
        enum { band = false };
        enum { unit = false };
        enum { inverse_shape = LowerTri };
        enum { nonunit_shape = LowerTri };
    };

    template <> 
    struct ShapeTraits<UnitUpperTri>
    {
        enum { square = true };
        enum { sym = false };
        enum { herm = false };
        enum { upper = true };
        enum { lower = false };
        enum { band = false };
        enum { unit = true };
        enum { inverse_shape = UnitUpperTri };
        enum { nonunit_shape = UpperTri };
    };

    template <> 
    struct ShapeTraits<UnitLowerTri>
    {
        enum { square = true };
        enum { sym = false };
        enum { herm = false };
        enum { upper = false };
        enum { lower = true };
        enum { band = false };
        enum { unit = true };
        enum { inverse_shape = UnitLowerTri };
        enum { nonunit_shape = LowerTri };
    };

    template <> 
    struct ShapeTraits<Band>
    {
        enum { square = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = true };
        enum { lower = true };
        enum { band = true };
        enum { unit = false };
        enum { inverse_shape = Rec };
        enum { nonunit_shape = Rec };
    };

    template <> 
    struct ShapeTraits<UpperBand>
    {
        enum { square = true };
        enum { sym = false };
        enum { herm = false };
        enum { upper = true };
        enum { lower = false };
        enum { band = true };
        enum { unit = false };
        enum { inverse_shape = UpperTri };
        enum { nonunit_shape = UpperBand };
    };

    template <> 
    struct ShapeTraits<LowerBand>
    {
        enum { square = true };
        enum { sym = false };
        enum { herm = false };
        enum { upper = false };
        enum { lower = true };
        enum { band = true };
        enum { unit = false };
        enum { inverse_shape = LowerTri };
        enum { nonunit_shape = LowerBand };
    };

    template <> 
    struct ShapeTraits<UnitUpperBand>
    {
        enum { square = true };
        enum { sym = false };
        enum { herm = false };
        enum { upper = true };
        enum { lower = false };
        enum { band = true };
        enum { unit = true };
        enum { inverse_shape = UnitUpperTri };
        enum { nonunit_shape = UpperBand };
    };

    template <> 
    struct ShapeTraits<UnitLowerBand>
    {
        enum { square = true };
        enum { sym = false };
        enum { herm = false };
        enum { upper = false };
        enum { lower = true };
        enum { band = true };
        enum { unit = true };
        enum { inverse_shape = UnitLowerTri };
        enum { nonunit_shape = LowerBand };
    };

    template <> 
    struct ShapeTraits<RealSym>
    {
        enum { square = true };
        enum { sym = true };
        enum { herm = true };
        enum { upper = true };
        enum { lower = true };
        enum { band = false };
        enum { unit = false };
        enum { inverse_shape = RealSym };
        enum { nonunit_shape = RealSym };
    };

    template <> 
    struct ShapeTraits<Sym>
    {
        enum { square = true };
        enum { sym = true };
        enum { herm = false };
        enum { upper = true };
        enum { lower = true };
        enum { band = false };
        enum { unit = false };
        enum { inverse_shape = Sym };
        enum { nonunit_shape = Sym };
    };

    template <> 
    struct ShapeTraits<Herm>
    {
        enum { square = true };
        enum { sym = false };
        enum { herm = true };
        enum { upper = true };
        enum { lower = true };
        enum { band = false };
        enum { unit = false };
        enum { inverse_shape = Herm };
        enum { nonunit_shape = Herm };
    };

    template <> 
    struct ShapeTraits<RealSymBand>
    {
        enum { square = true };
        enum { sym = true };
        enum { herm = true };
        enum { upper = true };
        enum { lower = true };
        enum { band = true };
        enum { unit = false };
        enum { inverse_shape = RealSym };
        enum { nonunit_shape = RealSymBand };
    };

    template <> 
    struct ShapeTraits<SymBand>
    {
        enum { square = true };
        enum { sym = true };
        enum { herm = false };
        enum { upper = true };
        enum { lower = true };
        enum { band = true };
        enum { unit = false };
        enum { inverse_shape = Sym };
        enum { nonunit_shape = SymBand };
    };

    template <> 
    struct ShapeTraits<HermBand>
    {
        enum { square = true };
        enum { sym = false };
        enum { herm = true };
        enum { upper = true };
        enum { lower = true };
        enum { band = true };
        enum { unit = false };
        enum { inverse_shape = Herm };
        enum { nonunit_shape = HermBand };
    };

    template <int S1, int S2> 
    struct ShapeTraits2
    {
        enum { bothunit = ShapeTraits<S1>::unit && ShapeTraits<S2>::unit };
        enum { bothband = ShapeTraits<S1>::band && ShapeTraits<S2>::band };
        enum { noupper = !ShapeTraits<S1>::upper && !ShapeTraits<S2>::upper };
        enum { nolower = !ShapeTraits<S1>::lower && !ShapeTraits<S2>::lower };
        enum { bothsym = ShapeTraits<S1>::sym && ShapeTraits<S2>::sym };
        enum { bothherm = ShapeTraits<S1>::herm && ShapeTraits<S2>::herm };

        enum { prod = (
                noupper ? (
                    nolower ? Diag : 
                    bothband ? bothunit ? UnitLowerBand : LowerBand :
                    bothunit ? UnitLowerTri : LowerTri ) :
                nolower ? (
                    bothband ? bothunit ? UnitUpperBand : UpperBand :
                    bothunit ? UnitUpperTri : UpperTri ) :
                bothband ? Band : Rec ) };
        enum { sum = (
                noupper && nolower ? Diag :
                noupper ? ( bothband ? LowerBand : LowerTri ) :
                nolower ? ( bothband ? UpperBand : UpperTri ) :
                bothsym ? ( bothband ? SymBand : Sym ) :
                bothherm ? ( bothband ? HermBand : Herm ) :
                bothband ? Band : 
                Rec ) };
        enum { assignable = (
                ( ShapeTraits<S1>::upper && !ShapeTraits<S2>::upper ) ? false :
                ( ShapeTraits<S1>::lower && !ShapeTraits<S2>::lower ) ? false :
                ( !ShapeTraits<S1>::unit && ShapeTraits<S2>::unit ) ? false :
                ( !ShapeTraits<S1>::sym && ShapeTraits<S2>::sym ) ? false :
                ( !ShapeTraits<S1>::herm && ShapeTraits<S2>::herm ) ? false :
                true ) };
    };

}

#endif
