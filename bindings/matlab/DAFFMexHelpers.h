/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *
 *  ------------------------------------------------------------------------------------
 *
 *  Visit the OpenDAFF website: http://www.opendaff.org
 *
 *  ------------------------------------------------------------------------------------
 *
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use the OpenDAFF software package except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
bool isScalar( const mxArray *A );
bool isVector( const mxArray *A );
bool isRowVector( const mxArray *A );
bool isColumnVector( const mxArray *A );
bool getDoubleScalar( const mxArray *A, double& dest );
bool getIntegerScalar( const mxArray *A, int& dest );
bool getIntegerVector( const mxArray *A, std::vector<int>& dest );
bool getString( const mxArray *A, std::string& dest );
bool getRealScalar( const mxArray *A, double& dest );

#endif // IW_DAFF_MATLAB_HELPERS
