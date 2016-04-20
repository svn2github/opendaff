#include <daffviz/DAFFVizBalloonPlot.h>

// Todo: strip required
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkContourFilter.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkFollower.h>
#include <vtkHedgeHog.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyLine.h>
#include <vtkLine.h>
#include <vtkPolygon.h>
#include <vtkProperty.h>
#include <vtkVectorText.h>
#include <vtkWarpScalar.h>
#include <vtkVectorText.h>
#include <vtkScalarBarActor.h>
#include <vtkColorTransferFunction.h>
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkWindowedSincPolyDataFilter.h>

#include <DAFF.h>

#include <algorithm>
#include <assert.h>

#define HALF_PI_F 1.5707963267

namespace DAFFViz {

static float PI_F = acos(-1.0f);

double grad2rad(const double dAngleGrad);

BalloonPlot::BalloonPlot(SGNode* pParentNode, const DAFFContent* pContent)
: SGNode(pParentNode),
m_pContent(pContent), 
m_iFrequency(0),
m_iNumFrequencies(0), 
m_iScaling(SCALING_LINEAR),
m_dMin(0.0), m_dMax(1.0),
m_iChannel(0),
m_bWarp(true), m_pNormals(0),
m_bUsePhaseAsColor(false),
m_pProbe(0),
m_pProbeLabel(0)
{
	init();
}

BalloonPlot::BalloonPlot(const DAFFContent* pContent)
: SGNode(),
m_pContent(pContent), 
m_iFrequency(0),
m_iNumFrequencies(0), 
m_iScaling(SCALING_LINEAR),
m_dMin(0.0), m_dMax(1.0),
m_iChannel(0),
m_bWarp(true), m_pNormals(0),
m_bUsePhaseAsColor(false),
m_pProbe(0),
m_pProbeLabel(0)
{
	init();
}

BalloonPlot::~BalloonPlot() {
	RemoveActor(m_pPlotActor);
	RemoveActor(m_pLabel);
	RemoveActor(m_pProbe);
}

void BalloonPlot::init() {

	const DAFFProperties* pProps = m_pContent->getProperties();

	switch (pProps->getContentType()) {
	case DAFF_DFT_SPECTRUM:
		m_iNumFrequencies = dynamic_cast<const DAFFContentDFT*>( m_pContent )->getNumDFTCoeffs();
		break;

	case DAFF_MAGNITUDE_SPECTRUM:
		m_iNumFrequencies = dynamic_cast<const DAFFContentMS*>( m_pContent )->getNumFrequencies();
		break;
	}

	bool bSouthPole = pProps->getBetaStart() == 0;
	bool bNorthPole = pProps->getBetaEnd() == 180;
	bool bAzimuthWrap = (pProps->getAlphaStart() == 0) && (pProps->getAlphaEnd() == 360);


	// --= Sphere grid =--

	vtkPoints* points = vtkPoints::New();
	m_pNormals = vtkDoubleArray::New();
	m_pNormals->SetNumberOfComponents(3);
	for (int i=0; i<pProps->getNumberOfRecords(); i++) {
		// TODO: Topologies: Rings, Patches, etc.
		float phi, theta;
		m_pContent->getRecordCoords(i, DAFF_OBJECT_VIEW, phi, theta);

		// Compute Cartesian points
		double x, y, z;
		sph2cart(phi, theta, x, y, z);
		points->InsertPoint(i, x, y, z);		
		double n[3] = {-x, -y, -z}; // define flipped normals for warping
		m_pNormals->InsertNextTuple(n);
	}


	// --= Faces =--

	vtkCellArray* cells = vtkCellArray::New();
	vtkIdType quadface[4];
	vtkIdType triangleface[3];
	
	int iNumPoints = pProps->getNumberOfRecords();
	int iAziPoints = pProps->getAlphaPoints();
	int iElePoints = pProps->getBetaPoints();

	int iBodyEleRings = iElePoints-1;
	int iAziCount = (bAzimuthWrap ? iAziPoints : iAziPoints-1);
	if (bSouthPole) iBodyEleRings--;
	if (bNorthPole) iBodyEleRings--;
	int iBodyOffset=0;
	if (bSouthPole)
		iBodyOffset = 1;

	// if there is only one azimuth point there would be no surface to draw, so lets draw a line
	if ((iAziPoints == 1) || (iElePoints == 1)) {
		if ((iAziPoints == 1) && (iElePoints == 1)) {
			// we will not plot a single point
		} else {
			// add a point at center
			points->InsertPoint(iNumPoints, 0, 0, 0);
			
			for (int i=0; i<iNumPoints; i++) {
				triangleface[0] = i;
				triangleface[1] = i+1;
				triangleface[2] = iNumPoints;

				for (int k=0; k<3; k++)
					assert( (triangleface[k] >= 0) && (triangleface[k] < iNumPoints+1) );

				cells->InsertNextCell(3, triangleface);
			}
			
		}		
	} else {
		// draw surfaces
		if (bSouthPole) {
			/*
			 *  If there is a south pole, this has index 0.
			 *  The next record, one elevation step above has then index 1.
			 */

			iBodyOffset = 1;
			for (int j=0; j<iAziCount; j++) {
				triangleface[0] = 0;
				triangleface[1] = iBodyOffset + j;
				triangleface[2] = iBodyOffset + (j+1)%iAziPoints;

				for (int k=0; k<3; k++)
					assert( (triangleface[k] >= 0) && (triangleface[k] < iNumPoints) );

				cells->InsertNextCell(3, triangleface);
			}
		};

		// Body
		for (int i=0; i<iBodyEleRings; i++) {
			for (int j=0; j<iAziCount; j++) {
				quadface[0] = iBodyOffset + i*iAziPoints + j; 
				quadface[1] = iBodyOffset + i*iAziPoints + (j+1)%iAziPoints;
				quadface[2] = iBodyOffset + (i+1)*iAziPoints + (j+1)%iAziPoints;
				quadface[3] = iBodyOffset + (i+1)*iAziPoints + j;

				for (int k=0; k<4; k++)
 					assert( (quadface[k] >= 0) && (quadface[k] < iNumPoints) );

				cells->InsertNextCell(4, quadface);
			}
		}

		// North pole

		if (bNorthPole) {
			/*
			 *  If there is a north pole, this has the last index.
			 *  The next record, one elevation step above has then index 1.
			 */

			for (int j=0; j<iAziCount; j++) {
				triangleface[0] = iNumPoints-1;
				triangleface[1] = iBodyOffset + iBodyEleRings*iAziPoints + j;
				triangleface[2] = iBodyOffset + iBodyEleRings*iAziPoints + (j+1)%iAziPoints;

				for (int k=0; k<3; k++)
					assert( (triangleface[k] >= 0) && (triangleface[k] < pProps->getNumberOfRecords()) );

				cells->InsertNextCell(3, triangleface);
			}
		}
	}


	// --= VTK stuff =--
	m_pPlotPolydata = vtkPolyData::New();
	m_pPlotPolydata->SetPoints(points);
	m_pPlotPolydata->SetPolys(cells);
	
	// Set scalars of polydata
	SetScalars();
	m_pPlotPolydata->GetPointData()->SetActiveScalars("magnitudes");

	// Warp sphere
	m_pWarp = vtkWarpScalar::New();
	m_pWarp->SetInputData(m_pPlotPolydata);
	m_pWarp->UseNormalOff();

	m_pMapper = vtkPolyDataMapper::New();
	m_pMapper->SetScalarModeToUsePointFieldData();
	
	if (m_bWarp)
		EnableWarp();
	else 
		DisableWarp();
	
	SetUsePhaseAsColor(m_bUsePhaseAsColor);

	m_pPlotActor = vtkActor::New();
	m_pPlotActor->SetMapper(m_pMapper);
	
	m_pPlotActor->GetProperty()->SetInterpolationToGouraud();

	AddActor(m_pPlotActor);
	
	// Probe
	/*m_pProbe = new SGNode(this);
	Line *line = new Line(m_pProbe,0,0,0,0,-1,0);
	Label label = new Label(m_pProbe, "test");*/

	points = vtkPoints::New();
	points->SetNumberOfPoints(2);
	points->InsertPoint(0, 0,0,0);
	points->InsertPoint(1, 0,-1,0);

	cells = vtkCellArray::New();
	vtkLine* line = vtkLine::New();
	line->GetPointIds()->SetId(0,0);
	line->GetPointIds()->SetId(1,1);
	cells->InsertNextCell(line);
	
	vtkPolyData *polydata = vtkPolyData::New();
	polydata->SetPoints(points);
	polydata->SetLines(cells);

	// Create a mapper and actor
	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInputData( polydata );

	m_pProbe = vtkActor::New();
	m_pProbe->SetMapper(mapper);
	m_pProbe->VisibilityOff();

	AddActor(m_pProbe);
	
	// label the probe
	m_pProbeLabel = vtkVectorText::New();
	std::ostringstream s;
	s << "test";
	m_pProbeLabel->SetText(s.str().c_str());
	
	mapper = vtkPolyDataMapper::New();
	mapper->AddInputConnection(m_pProbeLabel->GetOutputPort());
	
	m_pLabel = vtkFollower::New();
	m_pLabel->SetMapper(mapper);
	m_pLabel->SetScale(0.03);
	m_pLabel->SetPosition(0.02, -1.01, 0);
	m_pLabel->SetOrientation(-90,0,-90);
	m_pLabel->GetProperty()->SetOpacity(0.8);
	m_pLabel->VisibilityOff();

	AddActor(m_pLabel);
	
}

void BalloonPlot::UpdateProbe() {
	if (!m_pProbe)
		return;
	double* pos = m_pProbe->GetPosition();
	//double r = grad2rad();
	double p = grad2rad(m_dProbeBeta);
	double y = grad2rad(m_dProbeAlpha);

	// Translation matrix: to origin
	vtkMatrix4x4* mOrigin = vtkMatrix4x4::New();
	mOrigin->Identity();
	mOrigin->SetElement(0,3, -pos[0]);
	mOrigin->SetElement(1,3, -pos[1]);
	mOrigin->SetElement(2,3, -pos[2]);

	// Translation matrix: to object position
	vtkMatrix4x4* mPosition = vtkMatrix4x4::New();
	mPosition->Identity();
	mPosition->SetElement(0,3, pos[0]);
	mPosition->SetElement(1,3, pos[1]);
	mPosition->SetElement(2,3, pos[2]);

	/*
	 *  Roll: homogenous rotation matrix for -Z axis
	 *
	 *
	 *  cos(alpha)   sin(alpha)      0       0
	 *  -sin(alpha)  cos(alpha)      0       0
	 *      0            0           1       0
	 *      0            0           0       1
	 */
	//vtkMatrix4x4* mRoll = vtkMatrix4x4::New();
	//mRoll->Identity();
	//mRoll->SetElement(0,0, cos(r));
	//mRoll->SetElement(0,1, sin(r));
	//mRoll->SetElement(1,0, -sin(r));
	//mRoll->SetElement(1,1, cos(r));

	/*
	 *  Pitch: homogenous rotation matrix for +X axis
	 *
	 *      1            0            0           0
	 *      0       cos(alpha)  -sin(alpha)       0
	 *      0       sin(alpha)   cos(alpha)       0
	 *      0            0            0           1
	 */
	vtkMatrix4x4* mPitch = vtkMatrix4x4::New();
	mPitch->Identity();
	mPitch->SetElement(1,1, cos(p));
	mPitch->SetElement(1,2, -sin(p));
	mPitch->SetElement(2,1, sin(p));
	mPitch->SetElement(2,2, cos(p));

	/*
	 *  Yaw: homogenous rotation matrix for +Y axis
	 *
	 *   cos(alpha)      0       sin(alpha)       0
	 *      0            1            0           0
	 *  -sin(alpha)      0       cos(alpha)       0
	 *      0            0            0           1
	 */
	vtkMatrix4x4* mYaw = vtkMatrix4x4::New();
	mYaw->Identity();
	mYaw->SetElement(0,0, cos(y));
	mYaw->SetElement(0,2, sin(y));
	mYaw->SetElement(2,0, -sin(y));
	mYaw->SetElement(2,2, cos(y));


	// Compose transforms
	// (Order: Origin, Roll, Pitch, Yaw, Position)
	vtkTransform* transform = vtkTransform::New();
	transform->Concatenate(mPosition);
	transform->Concatenate(mYaw);
	transform->Concatenate(mPitch);
	//transform->Concatenate(mRoll);
	transform->Concatenate(mOrigin);

	// Apply new transformation
	m_pProbe->SetUserTransform(transform);
	m_pLabel->SetUserTransform(transform);

	// update label
	assert (m_pContent != 0);
	double val=0;
	int i;
	m_pContent->getNearestNeighbour(DAFF_DATA_VIEW, m_dProbeAlpha, m_dProbeBeta, i);
	std::ostringstream s;
	const DAFFContentDFT* pContentDFT = NULL;
	const DAFFContentMS* pContentMS = NULL;
	const DAFFContentMPS* pContentMPS = NULL;
	
	float fMag, fPhase, fReal, fImag;

	switch (m_pContent->getProperties()->getContentType()) {
	case DAFF_DFT_SPECTRUM:
		pContentDFT = dynamic_cast<const DAFFContentDFT*>( m_pContent );
		pContentDFT->getDFTCoeff(i, m_iChannel, m_iFrequency, fReal, fImag);
		fMag = sqrt( fReal*fReal + fImag*fImag );
		if (fReal != 0.0) {
			fPhase = atan(fImag/fReal);
			if (fReal < 0.0)
				if (fImag < 0.0)
					fPhase -= PI_F;
				else
					fPhase += PI_F;
		} else {
			if (fImag < 0.0)
				fPhase = -HALF_PI_F;
			else
				fPhase = HALF_PI_F;
		}
		if (m_iScaling == SCALING_DECIBEL || false) {
			fMag = factor2decibel(fMag);
			s << "Real: " << fReal << ", Imag: " << fImag << "\n Mag: " << fMag << "db, Phase:" << fPhase;
		} else
			s << "Real: " << fReal << ", Imag: " << fImag << "\n Mag: " << fMag << ", Phase:" << fPhase;
		break;

	case DAFF_MAGNITUDE_SPECTRUM:
		pContentMS = dynamic_cast<const DAFFContentMS*>( m_pContent );
		pContentMS->getMagnitude(i, m_iChannel, m_iFrequency, fMag);
		if (m_iScaling == SCALING_DECIBEL || false) {
			fMag = factor2decibel(fMag);
			s << fMag << "db";
		} else
			s << fMag;
		break;

	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		pContentMPS = dynamic_cast<const DAFFContentMPS*>( m_pContent );
		pContentMPS->getMagnitude(i, m_iChannel, m_iFrequency, fMag);
		pContentMPS->getPhase(i, m_iChannel, m_iFrequency, fPhase);
		if (m_iScaling == SCALING_DECIBEL || false) {
			fMag = factor2decibel(fMag);
			s << "Mag: " << fMag << "db, Phase: " << fPhase;
		} else
			s << "Mag: " << fMag << ", Phase: " << fPhase;
		break;
	}
	m_pProbeLabel->SetText(s.str().c_str());

	return;
}
void BalloonPlot::SetProbeAngles(const double dAlpha, const double dBeta) { m_dProbeAlpha = dAlpha; m_dProbeBeta = dBeta; }

void BalloonPlot::SetProbeVisible(const bool bVisible) { 
	m_pProbe->SetVisibility(bVisible);
	m_pLabel->SetVisibility(bVisible);
}
void BalloonPlot::SetSelectedFrequency(int iFreq) {
	m_iFrequency = iFreq;
	SetScalars();

	return;
}

int BalloonPlot::GetSelectedFrequency() const {
	return m_iFrequency;
}

void BalloonPlot::SetScaling(int iScaling) {
	m_iScaling = iScaling;
	SetScalars();

	return;
}

int BalloonPlot::GetScaling() const {
	return m_iScaling;
}

void BalloonPlot::SetRange(double dMin, double dMax) {
	assert( m_pContent != NULL);
	assert( dMin < dMax );
	if (m_iScaling == SCALING_LINEAR) {
		// Convert from linear to decibel
		m_dMin = dMin;
		m_dMax = dMax;
	} else { 
		m_dMin = decibel2factor(dMin);
		m_dMax = decibel2factor(dMax);
	}
	SetScalars();
}

double BalloonPlot::GetRangeMin() const {
	if (m_iScaling == SCALING_LINEAR) {
		return m_dMin;
	} else 
		return factor2decibel(m_dMin);
}

double BalloonPlot::GetRangeMax() const {
	if (m_iScaling == SCALING_LINEAR) {
		return m_dMax;
	} else 
		return factor2decibel(m_dMax);
}

void BalloonPlot::EnableWarp() {
	m_pPlotPolydata->GetPointData()->SetNormals(m_pNormals);
	m_pMapper->SetInputConnection(m_pWarp->GetOutputPort());
	m_bWarp = true;
}

void BalloonPlot::DisableWarp() {
	m_pPlotPolydata->GetPointData()->SetNormals(0);
	m_pMapper->SetInputData( m_pPlotPolydata );
	m_bWarp = false;
}

void BalloonPlot::SetScalars() {
	assert( m_iFrequency >= 0 && m_iFrequency < m_iNumFrequencies );
	assert( m_pContent != NULL);
	assert( m_dMin < m_dMax );

	const DAFFContentDFT* pContentDFT = NULL;
	const DAFFContentMS* pContentMS = NULL;
	const DAFFContentMPS* pContentMPS = NULL;

	switch (m_pContent->getProperties()->getContentType()) {
	case DAFF_DFT_SPECTRUM:
		pContentDFT = dynamic_cast<const DAFFContentDFT*>( m_pContent );
		break;

	case DAFF_MAGNITUDE_SPECTRUM:
		pContentMS = dynamic_cast<const DAFFContentMS*>( m_pContent );
		break;

	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		pContentMPS = dynamic_cast<const DAFFContentMPS*>( m_pContent );
		break;
	}

	vtkFloatArray* pMagArray = vtkFloatArray::New();
	pMagArray->SetName("magnitudes");
	vtkFloatArray* pPhArray = vtkFloatArray::New();
	pPhArray->SetName("phases");
	float fMag = 0.0f, fPhase = 0.0f;
	for (int i=0; i<m_pContent->getProperties()->getNumberOfRecords(); i++) {
		// Get the magnitude value
		if (pContentDFT) {
			float fReal, fImag;
			pContentDFT->getDFTCoeff(i, m_iChannel, m_iFrequency, fReal, fImag);
			fMag = sqrt( fReal*fReal + fImag*fImag );
			if (fReal != 0.0) {
				fPhase = atan(fImag/fReal);
				if (fReal < 0.0)
					if (fImag < 0.0)
						fPhase -= PI_F;
					else
						fPhase += PI_F;
			} else {
				if (fImag < 0.0)
					fPhase = -HALF_PI_F;
				else
					fPhase = HALF_PI_F;
			}
		}

		if (pContentMS) {
			pContentMS->getMagnitude(i, m_iChannel, m_iFrequency, fMag);
		}

		if (pContentMPS) {
			pContentMPS->getMagnitude(i, m_iChannel, m_iFrequency, fMag);
			pContentMPS->getPhase(i, m_iChannel, m_iFrequency, fPhase);
		}

		// Check weather decibel scaling is activated
		if (m_iScaling == SCALING_DECIBEL || false) {
			fMag = (float) factor2decibel((double) fMag);
		
			// Decibel boundaries
			//float DECIBEL_DELTA = 30; // FIXME: should not be hard coded -> GUI
			float DECIBEL_LOWER = factor2decibel(m_dMin);
			float DECIBEL_UPPER = factor2decibel(m_dMax);

			// Limit the lower boundary
			fMag = std::max(fMag, DECIBEL_LOWER);
			fMag = std::min(fMag, DECIBEL_UPPER);

			// Normalize the range into the interval [0,1]
			fMag = 1/(DECIBEL_UPPER-DECIBEL_LOWER)*fMag + DECIBEL_LOWER/(DECIBEL_LOWER-DECIBEL_UPPER);
		} else {
			fMag = std::max(fMag, m_dMin);
			fMag = std::min(fMag, m_dMax);
			// Normalize the range into the interval [0,1]
			fMag = (fMag-m_dMin)/(m_dMax-m_dMin);
		}

		// Store (inverted) value into scalars array
		pMagArray->InsertNextTuple1(1.0 - fMag);
		pPhArray->InsertNextTuple1(fPhase);
	}
	
	// Assign scalars to points
	//m_pPlotPolydata->GetPointData()->SetScalars(pMagArray);
	m_pPlotPolydata->GetPointData()->AddArray(pMagArray);
	m_pPlotPolydata->GetPointData()->AddArray(pPhArray);

	UpdateProbe();
	return;
}

void BalloonPlot::sph2cart(double phi, double theta, double& x, double& y, double& z) {
	x = -sin((theta+90)*PI_F/180.0) * sin(phi*PI_F/180.0);
	y = -cos((theta+90)*PI_F/180.0);
	z = -sin((theta+90)*PI_F/180.0) * cos(phi*PI_F/180.0);
}

double BalloonPlot::factor2decibel(double x) const {
	assert( x >= 0 );
	// Lower boundary is -100 dB
	if (x <= 0.0000000001) return -100;
	return 10*log(x)/log((double) 10);
}

double BalloonPlot::decibel2factor(double x) const {
	return pow(10.0, .1*x);
}

void BalloonPlot::SetScalarVisibility(bool bVisible) {
	m_pMapper->SetScalarVisibility(bVisible);
}

int BalloonPlot::GetScalarVisibility() {
	return m_pMapper->GetScalarVisibility();
}

void BalloonPlot::SetDisplayMode(int iMode) {
	if (iMode == MODE_SURFACE)
		m_pPlotActor->GetProperty()->SetRepresentationToSurface();
	else if (iMode == MODE_WIREFRAME)
		m_pPlotActor->GetProperty()->SetRepresentationToWireframe();
	else if (iMode == MODE_POINT)
		m_pPlotActor->GetProperty()->SetRepresentationToPoints();
}

void BalloonPlot::SetChannel(int iChannel) {
	m_iChannel = iChannel;
	SetScalars();
}

int BalloonPlot::GetChannel() {
	return m_iChannel;
}

void BalloonPlot::SetUsePhaseAsColor(bool bUse) {
	if (bUse) {
		m_pMapper->SelectColorArray("phases");
		vtkColorTransferFunction* colors = vtkColorTransferFunction::New();
		colors->AddRGBPoint(-PI_F, 0, 0, 1);
		colors->AddRGBPoint(0, 1, 1, 1);
		colors->AddRGBPoint(PI_F, 1, 0, 0);
		m_pMapper->SetLookupTable(colors);
	} else {
		m_pMapper->SelectColorArray("magnitudes");
		m_pMapper->SetLookupTable(0);
	}
	m_bUsePhaseAsColor = bUse;
}

} // End of namespace "FXVTK2"
