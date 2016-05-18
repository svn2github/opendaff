#include <daffviz/DAFFVizCarpetPlot.h>

#include <daffviz/DAFFVizSGNode.h>

// Todo: strip required
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkContourFilter.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkFollower.h>
#include <vtkHedgeHog.h>
#include <vtkLine.h>
#include <vtkLookupTable.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyLine.h>
#include <vtkProperty.h>
#include <vtkScalarBarActor.h>
#include <vtkVectorText.h>
#include <vtkWarpScalar.h>

#include <vtkWindowedSincPolyDataFilter.h>

#include <DAFF.h>

#include <algorithm>
#include <assert.h>

#define PI_F 3.14159265358979

namespace DAFFViz {

CarpetPlot::CarpetPlot(const DAFFContentIR* pContentIR)
	: SGNode(), m_pContentIR(pContentIR), m_fAngle(0.0f), m_iScaling(SCALING_LINEAR), m_dMin(-1.0), m_dMax(1.0), m_iFixedAngle(0),m_pPlotPolydata(0), m_pMapper(0), m_pWarp(0), m_pPlotActor(0), m_iChannel(0), m_bWarp(true), m_pProbe(0), m_pLabel(0), m_dProbeX(0), m_dProbeY(0)
{
	m_pPlotPolydata = vtkPolyData::New();

	vtkPolyDataNormals* normals = vtkPolyDataNormals::New();
	normals->SetInputData(m_pPlotPolydata);

	m_pWarp = vtkWarpScalar::New();
	m_pWarp->SetInputData(normals->GetOutput());
	m_pWarp->UseNormalOff();

	// create a default lookup table and invert it, so that high values have red color
	vtkLookupTable *lut = vtkLookupTable::New();
	vtkLookupTable *refLut = vtkLookupTable::New();
	lut->Build();
	refLut->Build();

	for (int i=0; i<256; i++) 
		lut->SetTableValue(i,refLut->GetTableValue(255-i));

	m_pMapper = vtkPolyDataMapper::New();
	m_pMapper->SetLookupTable(lut);

	EnableWarp();
	//DisableWarp();

	m_pPlotActor = vtkActor::New();
	m_pPlotActor->SetMapper(m_pMapper);

	m_pPlotActor->GetProperty()->SetInterpolationToGouraud();

	AddActor(m_pPlotActor);

	
	// Probe
	/*m_pProbe = new SGNode(this);
	Line *line = new Line(m_pProbe,0,0,0,0,-1,0);
	Label label = new Label(m_pProbe, "test");*/

	vtkPoints *points = vtkPoints::New();
	points->SetNumberOfPoints(2);
	points->InsertPoint(0, 0,0,0);
	points->InsertPoint(1, 0,1,0);

	vtkCellArray *cells = vtkCellArray::New();
	vtkLine* line = vtkLine::New();
	line->GetPointIds()->SetId(0,0);
	line->GetPointIds()->SetId(1,1);
	cells->InsertNextCell(line);
	
	vtkPolyData *polydata = vtkPolyData::New();
	polydata->SetPoints(points);
	polydata->SetLines(cells);

	// Create a mapper and actor
	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInputData(polydata);

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
	m_pLabel->SetPosition(0.00, 1.01, 0);
	m_pLabel->GetProperty()->SetOpacity(0.8);
	m_pLabel->VisibilityOff();

	AddActor(m_pLabel);

	init();

	//clean up local stuff
	lut->Delete();
	refLut->Delete();
	normals->Delete();
}

CarpetPlot::~CarpetPlot()
{
	RemoveActor( m_pLabel );
	RemoveActor( m_pProbe );
	RemoveActor( m_pPlotActor );
}


void CarpetPlot::init()
{

	const DAFFProperties* pProps = m_pContentIR->getProperties();

	// --= carpet grid points and faces =--

	vtkPoints* points = vtkPoints::New();
	vtkCellArray* cells = vtkCellArray::New();

	int iNumPointsX, iIndexMaxX;
	if (m_iFixedAngle == BETA_FIXED) {
		iNumPointsX = pProps->getAlphaPoints();
		if (pProps->coversFullAlphaRange())
			iIndexMaxX = iNumPointsX +1; // when we cover full sphere, the last row equals the first one
		else
			iIndexMaxX = iNumPointsX;
	} else {
		iNumPointsX = pProps->getBetaPoints();
		iIndexMaxX = iNumPointsX;
	}

	int iNumPointsY = m_pContentIR->getFilterLength();

	vtkIdType quadface[4];

	for (int i=0; i<iIndexMaxX; i++) 
		for (int j=0; j<iNumPointsY; j++) {
			if ((i!=iIndexMaxX-1) && (j!=iNumPointsY-1)) { // every time except from first and last row/column
				quadface[0] = (i+1)*iNumPointsY + j + 1;
				quadface[1] = (i+1)*iNumPointsY + j;
				quadface[2] = (i)*iNumPointsY + j;
				quadface[3] = (i)*iNumPointsY + j + 1;
				for (int k=0; k<4; k++)
 					assert( (quadface[k] >= 0) && (quadface[k] < iIndexMaxX*iNumPointsY) );
				cells->InsertNextCell(4, quadface);
			}
			points->InsertPoint(i*iNumPointsY + j, 2.0f*(float)i/(iNumPointsX)-1.0f, 0, 2.0f*(float)j/(iNumPointsY)-1.0f);
		}
		
	// Set scalars of polydata
	if (m_pPlotPolydata->GetPoints()) m_pPlotPolydata->GetPoints()->Delete();
	if (m_pPlotPolydata->GetPolys()) m_pPlotPolydata->GetPolys()->Delete();
	m_pPlotPolydata->SetPoints(points);
	m_pPlotPolydata->SetPolys(cells);
	SetScalars();
}

void CarpetPlot::UpdateProbe() {
	if (!m_pProbe)
		return;
	// Apply new transformation
	assert (m_pContentIR != 0);
	double val=0;
	int i;
	std::ostringstream s;
	float fMag;
	int iOffs=0, iLen=m_pContentIR->getFilterLength();
	float* pBuf=0;
	float dPosX=0, dPosY=0, dPosZ=0; // X := not fixed angle, Y := time
	dPosY = 2*m_dProbeY/iLen - 1;
	if (m_iScaling == SCALING_LINEAR) // else dPosZ=0
		dPosZ = (m_dMin)/(m_dMax-m_dMin);
	if (m_iFixedAngle==BETA_FIXED) {
		dPosX = 2*m_dProbeX/m_pContentIR->getProperties()->getBetaSpan() - 1;
		m_pContentIR->getNearestNeighbour(DAFF_DATA_VIEW, m_dProbeX, m_fAngle, i);
	} else {
		dPosX = 2*m_dProbeX/m_pContentIR->getProperties()->getAlphaSpan() - 1;
		m_pContentIR->getNearestNeighbour(DAFF_DATA_VIEW, m_fAngle, m_dProbeX, i);
	}

	m_pProbe->SetPosition(dPosX, dPosZ, dPosY);
	m_pLabel->SetPosition(dPosX, dPosZ+1.01, dPosY);

	// update label
	pBuf = new float[iLen];
	m_pContentIR->getFilterCoeffs(i, m_iChannel, pBuf);
	fMag = pBuf[(int)m_dProbeY];

	if (m_iScaling == SCALING_DECIBEL || false) {
		fMag = factor2decibel((double) fabs(fMag));
		s << fMag << "db";
	} else
		s << fMag;

	m_pProbeLabel->SetText(s.str().c_str());

	return;
}

void CarpetPlot::SetProbeAngles(double dX, double dY) { m_dProbeX = dX; m_dProbeY = dY; }

void CarpetPlot::SetProbeVisible(bool bVisible) { 
	m_pProbe->SetVisibility(bVisible);
	m_pLabel->SetVisibility(bVisible);
}

void CarpetPlot::SetSelectedAngle(float fAngle) {
	m_fAngle = fAngle;
	SetScalars();

	return;
}

float CarpetPlot::GetSelectedAngle() const {
	return m_fAngle;
}

void CarpetPlot::SetScaling(int iScaling) {
	m_iScaling = iScaling;
	SetScalars();

	return;
}

int CarpetPlot::GetScaling() const {
	return m_iScaling;
}

void CarpetPlot::SetFixedAngle(int iFixedAngle) {
	m_iFixedAngle = iFixedAngle;
	
	init();
}

int CarpetPlot::getFixedAngle() {
	return m_iFixedAngle;
}

void CarpetPlot::SetRange(double dMin, double dMax) {
	assert( m_pContentIR != NULL);
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

void CarpetPlot::SetScalarVisibility(bool bVisible) {
	m_pMapper->SetScalarVisibility(bVisible);
}

int CarpetPlot::getScalarVisibility() {
	return m_pMapper->GetScalarVisibility();
}

double CarpetPlot::GetRangeMin() const {
	if (m_iScaling == SCALING_LINEAR) {
		return m_dMin;
	} else 
		if (m_dMin < 0)
			return -100;
		else
			return factor2decibel(m_dMin);
}

double CarpetPlot::GetRangeMax() const {
	if (m_iScaling == SCALING_LINEAR) {
		return m_dMax;
	} else 
		return factor2decibel(m_dMax);
}

void CarpetPlot::EnableWarp() {
	m_pMapper->SetInputConnection(m_pWarp->GetOutputPort());
	m_bWarp = true;
	updatePlotOffset();
}

void CarpetPlot::updatePlotOffset() {
	if (m_pPlotActor) 
		if (m_bWarp)
			if ((m_iScaling == SCALING_LINEAR) && ((m_dMin < 0) && (m_dMax > 0)))
				m_pPlotActor->SetPosition(0, m_dMin/(m_dMax-m_dMin), 0);
			else if (m_iScaling == SCALING_DECIBEL)
				m_pPlotActor->SetPosition(0, 0, 0);
			else
				m_pPlotActor->SetPosition(0, 0, 0);
		else
			m_pPlotActor->SetPosition(0, 0, 0);
}

void CarpetPlot::DisableWarp() {
	m_pMapper->SetInputData(m_pPlotPolydata);
	m_bWarp = false;
	updatePlotOffset();
}

void CarpetPlot::SetScalars() {
	//TODO: asserts
	assert( m_pContentIR != NULL);
	assert( m_dMin < m_dMax );

	//m_pPlotPolydata->GetPointData()->GetScalars()->Delete();
	vtkFloatArray* pIRArray = vtkFloatArray::New();
	float* coeffs = new float[m_pContentIR->getFilterLength()];
	if (m_iFixedAngle == BETA_FIXED) {
		int iNumPoints = m_pContentIR->getProperties()->getAlphaPoints();
		float fStart = m_pContentIR->getProperties()->getAlphaStart();
		float fRange = m_pContentIR->getProperties()->getAlphaEnd() - fStart;
		for (int i=0; i<=iNumPoints; i++) {
			// Pull filter coefficients
			int index=0;
			m_pContentIR->getNearestNeighbour(DAFF_DATA_VIEW, (float)i/(float)iNumPoints*fRange+fStart, m_fAngle, index);
			m_pContentIR->getFilterCoeffs(index, m_iChannel, coeffs);
			float coeff=0.0f;
			for (int j=0; j<m_pContentIR->getFilterLength(); j++) {
				coeff = coeffs[j];

				// Check weather decibel scaling is activated
				if (m_iScaling == SCALING_DECIBEL || false) {
					// CAVE: we take absolut value instead of square
					coeff = (float) factor2decibel((double) fabs(coeff));
				
					// Decibel boundaries
					float DECIBEL_LOWER = -100;
					if (m_dMin >= 0)
						DECIBEL_LOWER = factor2decibel(m_dMin);
					float DECIBEL_UPPER = factor2decibel(m_dMax);

					// Normalize the range into the interval [0,1]
					// Limit the boundaries
					coeff = std::max(coeff, DECIBEL_LOWER);
					coeff = std::min(coeff, DECIBEL_UPPER);
					// clamp to [0 1] 
					coeff = 1.0/(DECIBEL_UPPER-DECIBEL_LOWER)*coeff + DECIBEL_LOWER/(DECIBEL_LOWER-DECIBEL_UPPER);
				} else {
					// Limit the boundaries
					coeff = std::max(coeff, m_dMin);
					coeff = std::min(coeff, m_dMax);
					// clamp to [0 1] 
					coeff = (coeff-m_dMin)/(m_dMax-m_dMin);
				}
				// store value in array
				pIRArray->InsertNextTuple1(coeff);
			}
		}
	} else {
		int iNumPoints = m_pContentIR->getProperties()->getBetaPoints();
		float fStart = m_pContentIR->getProperties()->getBetaStart();
		float fRange = m_pContentIR->getProperties()->getBetaEnd() - fStart;
		for (int i=0; i<m_pContentIR->getProperties()->getBetaPoints(); i++) {
			// Pull filter coefficients
			int index=0;
			m_pContentIR->getNearestNeighbour(DAFF_DATA_VIEW, m_fAngle, (float)i/(float)iNumPoints*fRange+fStart, index);
			m_pContentIR->getFilterCoeffs(index, m_iChannel, coeffs);
			float coeff=0.0f;
			for (int j=0; j<m_pContentIR->getFilterLength(); j++) {
				coeff = coeffs[j];

				// Check weather decibel scaling is activated
				if (m_iScaling == SCALING_DECIBEL || false) {
					// CAVE: we take absolut value instead of square
					coeff = (float) factor2decibel((double) fabs(coeff));
				
					// Decibel boundaries
					float DECIBEL_LOWER = -100;
					if (m_dMin >= 0)
						DECIBEL_LOWER = factor2decibel(m_dMin);
					float DECIBEL_UPPER = factor2decibel(m_dMax);

					// Normalize the range into the interval [0,1]
					// Limit the boundaries
					coeff = std::max(coeff, DECIBEL_LOWER);
					coeff = std::min(coeff, DECIBEL_UPPER);
					// clamp to [0 1] 
					coeff = 1.0/(DECIBEL_UPPER-DECIBEL_LOWER)*coeff + DECIBEL_LOWER/(DECIBEL_LOWER-DECIBEL_UPPER);
				} else {
					// Limit the boundaries
					coeff = std::max(coeff, m_dMin);
					coeff = std::min(coeff, m_dMax);
					// clamp to [0 1] 
					coeff = (coeff-m_dMin)/(m_dMax-m_dMin);
				}
				// store value in array
				pIRArray->InsertNextTuple1(coeff);
			}
		}
	}

	// Assign scalars to points
	m_pPlotPolydata->GetPointData()->SetScalars(pIRArray);
	// move plot down so it fits to the axes
	updatePlotOffset();
	UpdateProbe();
	delete[] coeffs;
	return;
}

double CarpetPlot::factor2decibel(double x) const {
	assert( x >= 0 );
	// Lower boundary is -100 dB
	if (x <= 0.0000000001) return -100;
	return 10*log(x)/log((double) 10);
}

double CarpetPlot::decibel2factor(double x) const {
	return pow(10.0, .1*x);
}

void CarpetPlot::SetDisplayMode(int iMode) {
	if (iMode == MODE_SURFACE)
		m_pPlotActor->GetProperty()->SetRepresentationToSurface();
	else if (iMode == MODE_WIREFRAME)
		m_pPlotActor->GetProperty()->SetRepresentationToWireframe();
	else if (iMode == MODE_POINT)
		m_pPlotActor->GetProperty()->SetRepresentationToPoints();
}

void CarpetPlot::SetChannel(int iChannel) {
	m_iChannel = iChannel;
	SetScalars();
}

int CarpetPlot::GetChannel() {
	return m_iChannel;
}

} // End of namespace "DAFFViz"
