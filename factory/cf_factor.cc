/* emacs edit mode for this file is -*- C++ -*- */
/* $Id: cf_factor.cc,v 1.7 1997-09-09 09:25:52 schmidt Exp $ */

//{{{ docu
//
// cf_factor.cc - factorization and square free algorithms.
//
// Used by: fac_multivar.cc, fac_univar.cc, cf_irred.cc
//
// Header file: cf_algorithm.h
//
//}}}

#include <config.h>

#include "cf_gmp.h"

#include "assert.h"

#include "cf_defs.h"
#include "canonicalform.h"
#include "cf_iter.h"
#include "fac_berlekamp.h"
#include "fac_cantzass.h"
#include "fac_univar.h"
#include "fac_multivar.h"
#include "fac_sqrfree.h"

static bool isUnivariateBaseDomain( const CanonicalForm & f )
{
    CFIterator i = f;
    bool ok = i.coeff().inBaseDomain();
    i++;
    while ( i.hasTerms() && ( ok = ok && i.coeff().inBaseDomain() ) ) i++;
    return ok;
}

CFFList factorize ( const CanonicalForm & f, bool issqrfree )
{
    if ( f.inCoeffDomain() )
	return CFFList( f );
    if ( getCharacteristic() > 0 ) {
	ASSERT( f.isUnivariate(), "multivariate factorization not implemented" );
	if ( isOn( SW_BERLEKAMP ) )
	    return FpFactorizeUnivariateB( f, issqrfree );
	else
	    return FpFactorizeUnivariateCZ( f, issqrfree );
    }
    else {
	if ( f.isUnivariate() )
	    return ZFactorizeUnivariate( f, issqrfree );
	else
	    return ZFactorizeMultivariate( f, issqrfree );
    }
}

CFFList factorize ( const CanonicalForm & f, const Variable & alpha )
{
    ASSERT( alpha.level() < 0, "not an algebraic extension" );
    ASSERT( f.isUnivariate(), "multivariate factorization not implemented" );
    ASSERT( getCharacteristic() > 0, "char 0 factorization not implemented" );
    return FpFactorizeUnivariateCZ( f, false, 1, alpha );
}

CFFList sqrFree ( const CanonicalForm & f, bool sort )
{
//    ASSERT( f.isUnivariate(), "multivariate factorization not implemented" );
    CFFList result;

    if ( getCharacteristic() == 0 )
	result = sqrFreeZ( f );
    else
	result = sqrFreeFp( f );

    return ( sort ? sortCFFList( result ) : result );
}

bool isSqrFree ( const CanonicalForm & f )
{
//    ASSERT( f.isUnivariate(), "multivariate factorization not implemented" );
    if ( getCharacteristic() == 0 )
	return isSqrFreeZ( f );
    else
	return isSqrFreeFp( f );
}
