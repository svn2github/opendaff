/*
 *  OpenDAFF - A free, open-source software package for directional audio data,
 *  OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL)
 *
 *  Copyright (C) Institute of Technical Acoustics, RWTH Aachen University, 2009-2010
 *
 *  Visit the OpenDAFF homepage: http://www.opendaff.org
 *
 *
 *  --= License & warranty =--
 *
 *  OpenDAFF is free software, distributed under the terms of the
 *  GNU Lesser General Public License (LGPL) version 3.
 *  You can redistribute it and/or modify it under the terms of the
 *  GNU Lesser General Public License (LGPL) version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  --= File information =--
 *
 *  File:		SCTransformTest.cpp
 *  Purpose:	A small tryout test for the spherical coordinate transformations
 *  Version:    $Id: SCTransformTest.cpp,v 1.5 2010/03/11 13:08:54 fwefers Exp $
 *	Authors:	Frank Wefers (Frank.Wefers@akustik.rwth-aachen.de)
 *
 */

#include <stdio.h>
#include <cmath>

#include <DAFFDefs.h>
#include <DAFFSCTransform.h>
#include <DAFFUtils.h>

using namespace std;

void test_direction_norm() {
	float a,e, a2, e2;

	printf("object view normalization\n\n");

	a = 0, e = 0;
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, a, e, a2, e2);
	printf("<%0.3f,%0.3f> --> <%0.3f,%0.3f>\n", a, e, a2, e2);

	a = 180, e = 0;
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, a, e, a2, e2);
	printf("<%0.3f,%0.3f> --> <%0.3f,%0.3f>\n", a, e, a2, e2);

	a = 200, e = 0;
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, a, e, a2, e2);
	printf("<%0.3f,%0.3f> --> <%0.3f,%0.3f>\n", a, e, a2, e2);

	a = 0, e = 90;
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, a, e, a2, e2);
	printf("<%0.3f,%0.3f> --> <%0.3f,%0.3f>\n", a, e, a2, e2);

	a = 0, e = -90;
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, a, e, a2, e2);
	printf("<%0.3f,%0.3f> --> <%0.3f,%0.3f>\n", a, e, a2, e2);

	a = 0, e = 100;
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, a, e, a2, e2);
	printf("<%0.3f,%0.3f> --> <%0.3f,%0.3f>\n", a, e, a2, e2);

	a = 0, e = -100;
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, a, e, a2, e2);
	printf("<%0.3f,%0.3f> --> <%0.3f,%0.3f>\n", a, e, a2, e2);

	a = 90, e = 100;
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, a, e, a2, e2);
	printf("<%0.3f,%0.3f> --> <%0.3f,%0.3f>\n", a, e, a2, e2);

	printf("\n");

}

void test(float y, float p, float r) {
	float azi1, ele1, azi2, ele2, alpha, beta;

	DAFFOrientationYPR o(y, p, r);
	DAFFSCTransform t(o);
	printf("Orientation = %s\n\n", o.toString().c_str());

	azi1 = 0, ele1 = 0;
	t.transformOSC2DSC(azi1, ele1, alpha, beta);
	t.transformDSC2OSC(alpha, beta, azi2, ele2);
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, azi2, ele2, azi2, ele2);

	printf("OSC <%0.3f,%0.3f> --> DSC <%0.3f,%0.3f> --> OSC <%0.3f,%0.3f>\n",
		   azi1, ele1, alpha, beta, azi2, ele2);

	azi1 = 90, ele1 = 0;
	t.transformOSC2DSC(azi1, ele1, alpha, beta);
	t.transformDSC2OSC(alpha, beta, azi2, ele2);
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, azi2, ele2, azi2, ele2);

	printf("OSC <%0.3f,%0.3f> --> DSC <%0.3f,%0.3f> --> OSC <%0.3f,%0.3f>\n",
		   azi1, ele1, alpha, beta, azi2, ele2);

	azi1 = 0, ele1 = 90;
	t.transformOSC2DSC(azi1, ele1, alpha, beta);
	t.transformDSC2OSC(alpha, beta, azi2, ele2);
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, azi2, ele2, azi2, ele2);

	printf("OSC <%0.3f,%0.3f> --> DSC <%0.3f,%0.3f> --> OSC <%0.3f,%0.3f>\n",
		   azi1, ele1, alpha, beta, azi2, ele2);

	azi1 = 20, ele1 = -10;
	t.transformOSC2DSC(azi1, ele1, alpha, beta);
	t.transformDSC2OSC(alpha, beta, azi2, ele2);
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, azi2, ele2, azi2, ele2);

	printf("OSC <%0.3f,%0.3f> --> DSC <%0.3f,%0.3f> --> OSC <%0.3f,%0.3f>\n",
		   azi1, ele1, alpha, beta, azi2, ele2);

	azi1 = 60, ele1 = 90;
	t.transformOSC2DSC(azi1, ele1, alpha, beta);
	t.transformDSC2OSC(alpha, beta, azi2, ele2);
	DAFFUtils::NormalizeDirection(DAFF_OBJECT_VIEW, azi2, ele2, azi2, ele2);

	printf("OSC <%0.3f,%0.3f> --> DSC <%0.3f,%0.3f> --> OSC <%0.3f,%0.3f>\n",
		   azi1, ele1, alpha, beta, azi2, ele2);

	printf("\n");
}

int main() {

	test_direction_norm();
	printf("\n");
	
	test(0,0,0);
	printf("\n");

	test(90,0,0);
	printf("\n");

	test(0,90,0);
	printf("\n");

	test(0,0,90);
	printf("\n");

	test(0,45,0);
	printf("\n");
	
	return 0;
}
