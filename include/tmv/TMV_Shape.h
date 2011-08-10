
#ifndef TMV_Shape_H
#define TMV_Shape_H

namespace tmv {

    enum Shape {
        InvalidShape, Vec, Null,
        Rec, Diag, UpperTri, LowerTri, UnitUpperTri, UnitLowerTri,
        Band, UpperBand, LowerBand, UnitUpperBand, UnitLowerBand,
        RealSym, Sym, Herm, RealSymBand, SymBand, HermBand };

    template <int S>
    struct ShapeTraits;

    template <>
    struct ShapeTraits<Vec>
    {
        enum { vector = true };
        enum { sym = false };
        enum { herm = false };
        enum { upper = false };
        enum { lower = false };
        enum { band = false };
        enum { unit = false };
        enum { square = false };
        enum { inverse_shape = Vec };
        enum { nonunit_shape = Vec };
        enum { unit_shape = Vec };
    };

    template <>
    struct ShapeTraits<Null>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = false };
        enum { lower = false };
        enum { band = false };
        enum { unit = false };
        enum { square = false };
        enum { inverse_shape = Null };
        enum { nonunit_shape = Null };
        enum { unit_shape = Null };
    };

    template <>
    struct ShapeTraits<Rec>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = true };
        enum { lower = true };
        enum { band = false };
        enum { unit = false };
        enum { square = false };
        enum { inverse_shape = Rec };
        enum { nonunit_shape = Rec };
        enum { unit_shape = InvalidShape };
    };

    template <>
    struct ShapeTraits<Diag>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = false };
        enum { lower = false };
        enum { band = true };
        enum { unit = false };
        enum { square = true };
        enum { inverse_shape = Diag };
        enum { nonunit_shape = Diag };
        enum { unit_shape = InvalidShape };
    };

    template <>
    struct ShapeTraits<UpperTri>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = true };
        enum { lower = false };
        enum { band = false };
        enum { unit = false };
        enum { square = true };
        enum { inverse_shape = UpperTri };
        enum { nonunit_shape = UpperTri };
        enum { unit_shape = UnitUpperTri };
    };

    template <>
    struct ShapeTraits<LowerTri>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = false };
        enum { lower = true };
        enum { band = false };
        enum { unit = false };
        enum { square = true };
        enum { inverse_shape = LowerTri };
        enum { nonunit_shape = LowerTri };
        enum { unit_shape = UnitLowerTri };
    };

    template <>
    struct ShapeTraits<UnitUpperTri>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = true };
        enum { lower = false };
        enum { band = false };
        enum { unit = true };
        enum { square = true };
        enum { inverse_shape = UnitUpperTri };
        enum { nonunit_shape = UpperTri };
        enum { unit_shape = UnitUpperTri };
    };

    template <>
    struct ShapeTraits<UnitLowerTri>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = false };
        enum { lower = true };
        enum { band = false };
        enum { unit = true };
        enum { square = true };
        enum { inverse_shape = UnitLowerTri };
        enum { nonunit_shape = LowerTri };
        enum { unit_shape = UnitLowerTri };
    };

    template <>
    struct ShapeTraits<Band>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = true };
        enum { lower = true };
        enum { band = true };
        enum { unit = false };
        enum { square = false };
        enum { inverse_shape = Rec };
        enum { nonunit_shape = Band };
        enum { unit_shape = InvalidShape };
    };

    template <>
    struct ShapeTraits<UpperBand>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = true };
        enum { lower = false };
        enum { band = true };
        enum { unit = false };
        enum { square = true };
        enum { inverse_shape = UpperTri };
        enum { nonunit_shape = UpperBand };
        enum { unit_shape = UnitUpperBand };
    };

    template <>
    struct ShapeTraits<LowerBand>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = false };
        enum { lower = true };
        enum { band = true };
        enum { unit = false };
        enum { square = true };
        enum { inverse_shape = LowerTri };
        enum { nonunit_shape = LowerBand };
        enum { unit_shape = UnitLowerBand };
    };

    template <>
    struct ShapeTraits<UnitUpperBand>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = true };
        enum { lower = false };
        enum { band = true };
        enum { unit = true };
        enum { square = true };
        enum { inverse_shape = UnitUpperTri };
        enum { nonunit_shape = UpperBand };
        enum { unit_shape = UnitUpperBand };
    };

    template <>
    struct ShapeTraits<UnitLowerBand>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = false };
        enum { upper = false };
        enum { lower = true };
        enum { band = true };
        enum { unit = true };
        enum { square = true };
        enum { inverse_shape = UnitLowerTri };
        enum { nonunit_shape = LowerBand };
        enum { unit_shape = UnitLowerBand };
    };

    template <>
    struct ShapeTraits<RealSym>
    {
        enum { vector = false };
        enum { sym = true };
        enum { herm = true };
        enum { upper = true };
        enum { lower = true };
        enum { band = false };
        enum { unit = false };
        enum { square = true };
        enum { inverse_shape = RealSym };
        enum { nonunit_shape = RealSym };
        enum { unit_shape = InvalidShape };
    };

    template <>
    struct ShapeTraits<Sym>
    {
        enum { vector = false };
        enum { sym = true };
        enum { herm = false };
        enum { upper = true };
        enum { lower = true };
        enum { band = false };
        enum { unit = false };
        enum { square = true };
        enum { inverse_shape = Sym };
        enum { nonunit_shape = Sym };
        enum { unit_shape = InvalidShape };
    };

    template <>
    struct ShapeTraits<Herm>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = true };
        enum { upper = true };
        enum { lower = true };
        enum { band = false };
        enum { unit = false };
        enum { square = true };
        enum { inverse_shape = Herm };
        enum { nonunit_shape = Herm };
        enum { unit_shape = InvalidShape };
    };

    template <>
    struct ShapeTraits<RealSymBand>
    {
        enum { vector = false };
        enum { sym = true };
        enum { herm = true };
        enum { upper = true };
        enum { lower = true };
        enum { band = true };
        enum { unit = false };
        enum { square = true };
        enum { inverse_shape = RealSym };
        enum { nonunit_shape = RealSymBand };
        enum { unit_shape = InvalidShape };
    };

    template <>
    struct ShapeTraits<SymBand>
    {
        enum { vector = false };
        enum { sym = true };
        enum { herm = false };
        enum { upper = true };
        enum { lower = true };
        enum { band = true };
        enum { unit = false };
        enum { square = true };
        enum { inverse_shape = Sym };
        enum { nonunit_shape = SymBand };
        enum { unit_shape = InvalidShape };
    };

    template <>
    struct ShapeTraits<HermBand>
    {
        enum { vector = false };
        enum { sym = false };
        enum { herm = true };
        enum { upper = true };
        enum { lower = true };
        enum { band = true };
        enum { unit = false };
        enum { square = true };
        enum { inverse_shape = Herm };
        enum { nonunit_shape = HermBand };
        enum { unit_shape = InvalidShape };
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
                ( S1 == Null ) ? S2 :
                ( S2 == Null ) ? S1 :
                noupper ? (
                    nolower ? Diag : 
                    bothband ? bothunit ? UnitLowerBand : LowerBand :
                    bothunit ? UnitLowerTri : LowerTri ) :
                nolower ? (
                    bothband ? bothunit ? UnitUpperBand : UpperBand :
                    bothunit ? UnitUpperTri : UpperTri ) :
                bothband ? Band  :
                Rec ) };
        enum { sum = (
                ( S1 == Null ) ? S2 :
                ( S2 == Null ) ? S1 :
                noupper && nolower ? Diag :
                noupper ? ( bothband ? LowerBand : LowerTri ) :
                nolower ? ( bothband ? UpperBand : UpperTri ) :
                bothsym ? ( bothband ? SymBand : Sym ) :
                bothherm ? ( bothband ? HermBand : Herm ) :
                bothband ? Band :
                Rec ) };
        enum { assignable = (
                ( S1 == Null ) ? true :
                ( S2 == Null ) ? false :
                ( ShapeTraits<S1>::upper && !ShapeTraits<S2>::upper ) ? false :
                ( ShapeTraits<S1>::lower && !ShapeTraits<S2>::lower ) ? false :
                ( !ShapeTraits<S1>::unit && ShapeTraits<S2>::unit ) ? false :
                ( !ShapeTraits<S1>::sym && ShapeTraits<S2>::sym ) ? false :
                ( !ShapeTraits<S1>::herm && ShapeTraits<S2>::herm ) ? false :
                true ) };
    };

}

#endif
