/*
* -------------------------------------------------------------------------------------
*
*  OpenDAFF - A free, open source software package for directional audio data
*  OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL)
*
*  Copyright (c) Institute of Technical Acoustics, RWTH Aachen University, 2009-2016
*
*  ------------------------------------------------------------------------------------
*
*/

#ifndef IW_DAFF_MATLAB_HELPERS
#define IW_DAFF_MATLAB_HELPERS

#include <mex.h>
#include <string>
#include <vector>

// These are helper functions to perform validations and other conversions
bool isScalar(const mxArray *A);
bool isVector(const mxArray *A);
bool isRowVector(const mxArray *A);
bool isColumnVector(const mxArray *A);
bool getDoubleScalar(const mxArray *A, double& dest);
bool getIntegerScalar(const mxArray *A, int& dest);
bool getIntegerVector(const mxArray *A, std::vector<int>& dest);
bool getString(const mxArray *A, std::string& dest);
bool getRealScalar(const mxArray *A, double& dest);

#endif // IW_DAFF_MATLAB_HELPERS
