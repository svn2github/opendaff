#include "DAFFMexHelpers.h"

#include <cmath>
#include <string>
#include <algorithm>

bool isScalar( const mxArray *A )
{
	// TODO: Dieser Test ist möglicherweise nicht 100%tig sicher.
	//       Leider gibt es keine MEX-Funktion zum Testen auf Skalar.

	mwSize s = mxGetNumberOfDimensions( A );
	if( s != 2 ) return false;
	const mwSize* d = mxGetDimensions( A );
	return ( ( d[ 0 ] == 1 ) && ( d[ 1 ] == 1 ) );
}

bool isVector( const mxArray *A )
{
	mwSize s = mxGetNumberOfDimensions( A );
	if( s != 2 ) return false;
	const mwSize* d = mxGetDimensions( A );
	return ( ( d[ 0 ] == 1 ) || ( d[ 1 ] == 1 ) );
}

bool isRowVector( const mxArray *A )
{
	mwSize s = mxGetNumberOfDimensions( A );
	if( s != 2 ) return false;
	const mwSize* d = mxGetDimensions( A );
	return ( d[ 0 ] == 1 );
}

bool isColumnVector( const mxArray *A )
{
	mwSize s = mxGetNumberOfDimensions( A );
	if( s != 2 ) return false;
	const mwSize* d = mxGetDimensions( A );
	return ( d[ 1 ] == 1 );
}

bool getDoubleScalar( const mxArray *A, double& dest )
{
	// TODO: Test auf Skalar
	if( !mxIsNumeric( A ) || !isScalar( A ) )
		return false;

	dest = *mxGetPr( A );
	return true;
}

bool getIntegerScalar( const mxArray *A, int& dest )
{
	double x;
	if( !getDoubleScalar( A, x ) )
		return false;

	if( floor( x ) != x )
		return false;

	dest = ( int ) x;
	return true;
}

bool getIntegerVector( const mxArray *A, std::vector<int>& dest )
{
	if( !mxIsNumeric( A ) ) return false;
	if( !isVector( A ) ) return false;

	const mwSize* d = mxGetDimensions( A );
	int n = ( int ) ( std::max )( d[ 0 ], d[ 1 ] );
	dest.resize( n );
	double* data = mxGetPr( A );
	for( int i = 0; i < n; i++ )
	{
		double x = data[ i ];

		// Test auf Ganzzahl
		if( floor( x ) != x )
			return false;

		dest[ i ] = ( int ) x;
	}

	return true;
}

bool getString( const mxArray *A, std::string& dest )
{
	// TODO: Test auf Zeichen
	if( !mxIsChar( A ) || !isRowVector( A ) )
		return false;

	char buf[ 1024 ];
	mxGetString( A, buf, 1024 );
	dest = buf;
	return true;
}

bool getRealScalar( const mxArray *A, double& dest )
{
	if( !mxIsNumeric( A ) || !isScalar( A ) || mxIsComplex( A ) )
		return false;

	dest = *mxGetPr( A );
	return true;
}
