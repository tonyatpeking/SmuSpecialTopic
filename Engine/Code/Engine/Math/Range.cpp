﻿#include "Engine/Math/Range.hpp"
#include "Engine/Math/Random.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorUtils.hpp"

Range::Range( const Range& copyFrom )
    : min( copyFrom.min )
    , max( copyFrom.max )
{

}

Range::Range( float minInclusive, float maxInclusive )
    : min( minInclusive )
    , max( maxInclusive )
{

}

Range::Range( float minMaxInclusive )
    : min( minMaxInclusive )
    , max( minMaxInclusive )
{

}

ParseStatus Range::SetFromText( const String& text, const String& delimiter )
{
    std::vector<float> out_floats;
    ParseStatus status = StringUtils::ParseToFloats( text, out_floats, delimiter );

    //ASSERT_RECOVERABLE( out_floats.size() == 1 || out_floats.size() == 2, "Wrong number of elements passed to Range" );

    if( out_floats.size() != 1 && out_floats.size() != 2 )
        return PARSE_WRONG_NUMBER_OF_TOKENS;

    if( out_floats.size() == 1 )
    {
        min = out_floats[0];
        max = out_floats[0];
    }
    else if( out_floats.size() == 2 )
    {
        min = out_floats[0];
        max = out_floats[1];
    }

    return status;
}

float Range::GetRandomInRange() const
{
    return Random::FloatInRange( min, max );
}

bool Range::IsInRange( float val ) const
{
    return ( min <= val ) && ( val <= max );
}

bool Range::DoRangesOverlap( const Range& a, const Range& b )
{
    return a.IsInRange( b.min ) || a.IsInRange( b.max );
}

const Range Lerp( const Range& start, const Range& end, float t )
{
    float newMin = Lerp( start.min, end.min, t );
    float newMax = Lerp( start.max, end.max, t );
    return Range( newMin, newMax );
}
