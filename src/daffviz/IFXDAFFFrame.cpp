// $Id: IFXDAFFFrame.cpp,v 1.8 2010/02/10 11:23:31 stienen Exp $

#include <daffviz/IFXDAFFFrame.h>

#include "DAFFTransformerIR2DFT.h"

#include <cassert>
#include <cmath>
#include <IFXFancyTable.h>
#include <DAFF.h>
#include <FXVTK2Frame.h>

// Map
FXDEFMAP(IFXDAFFFrame) IFXDAFFFrameMap[]={
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_FREQUENCY,			IFXDAFFFrame::onCmdFrequency),
	FXMAPFUNC(SEL_MOUSEWHEEL,	IFXDAFFFrame::ID_FREQUENCY,			IFXDAFFFrame::onCmdFrequency),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_CHANNEL,			IFXDAFFFrame::onCmdChannels),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_GRID,				IFXDAFFFrame::onCmdGrid),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_VECTORS,			IFXDAFFFrame::onCmdVectors),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_NSPOLE,			IFXDAFFFrame::onCmdNSPole),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_WARP,				IFXDAFFFrame::onCmdWarp),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_COLOR,				IFXDAFFFrame::onCmdColor),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_WIRE,				IFXDAFFFrame::onCmdWire),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_DATA_SCALING,			IFXDAFFFrame::onCmdScaling),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_FIXED_ANGLE,		IFXDAFFFrame::onCmdFixedAngle),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_PLOT_TYPE,			IFXDAFFFrame::onCmdPlotType),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_RANGE,				IFXDAFFFrame::onCmdRange),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_REFERENCE,			IFXDAFFFrame::onCmdReference),
	FXMAPFUNC(SEL_COMMAND,		IFXDAFFFrame::ID_PROBE,				IFXDAFFFrame::onCmdProbe)
};

FXIMPLEMENT(IFXDAFFFrame, FXHorizontalFrame, IFXDAFFFrameMap, ARRAYNUMBER(IFXDAFFFrameMap))

IFXDAFFFrame::IFXDAFFFrame(FXComposite* p, DAFFReader* pDAFF, FXuint opts, FXint x, FXint y, FXint w, FXint h, FXint pl, FXint pr, FXint pt, FXint pb)
 : FXHorizontalFrame(p, opts, x, y, w, h, pl, pr, pt, pb),
 m_pScreen(NULL), m_pRootNode(NULL), m_pBalloonPlotNode(NULL), m_pCarpetPlotNode(NULL), m_pSphericalCoordAssist(NULL), m_pCartesianCoordAssist(NULL)
{
	m_pReader = pDAFF;

	rearrange();
}

IFXDAFFFrame::~IFXDAFFFrame() {
	m_pReader = NULL;

	if (m_pScreen) m_pScreen->destroy();
}

void IFXDAFFFrame::setDAFFReader(DAFFReader* pReader) {
	m_pReader = pReader;

	rearrange();
}

void IFXDAFFFrame::rearrange() {
	if (!m_pReader) {
		FXVerticalFrame* vframe = new FXVerticalFrame(this, FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_CENTER_X | LAYOUT_CENTER_Y, 0,0,0,0, 0,0,0,0);
		new FXLabel(vframe, "No DAFF object loaded", NULL, LAYOUT_CENTER_X | LAYOUT_CENTER_Y);
	} else {
		if (m_pReader->getContentType() == DAFF_IMPULSE_RESPONSE)
			arrangeIR();
		if (m_pReader->getContentType() == DAFF_MAGNITUDE_SPECTRUM)
			arrangeMS();
		if (m_pReader->getContentType() == DAFF_DFT_SPECTRUM)
			arrangeDFT();
	}
}

void IFXDAFFFrame::arrange_base() {

	// --= Basis arrangement =--

	// Tab book
	FXTabBook* pTabbook = new FXTabBook(this, this, ID_TAB, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0,0,0,0, 0,0,0,0);

	// Visualization Tab
	FXTabItem* pTabVisualization = new FXTabItem(pTabbook, "&Visualization", NULL, TAB_TOP_NORMAL, 0,0,0,0, 10,10,2,5);
	m_pFrameVisualization = new FXHorizontalFrame(pTabbook, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_FILL_Y , 0,0,0,0, 5,5,5,5);

	// Properties Tab
	FXTabItem* pTabProperties = new FXTabItem(pTabbook, "&Properties", NULL, TAB_TOP_NORMAL, 0,0,0,0, 10,10,2,5);
	m_pFrameProperties = new FXHorizontalFrame(pTabbook, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_FILL_Y , 0,0,0,0, 5,5,5,5);

	// Metadata Tab
	FXTabItem* pTabMetadata = new FXTabItem(pTabbook, "&Metadata", NULL, TAB_TOP_NORMAL, 0,0,0,0, 10,10,2,5);
	m_pFrameMetadata = new FXHorizontalFrame(pTabbook, FRAME_THICK | FRAME_RAISED | LAYOUT_FILL_X | LAYOUT_FILL_Y, 0,0,0,0, 5,5,5,5);

	FXVerticalFrame* helperframe;


	// --= Visualization =--

	// define Label Texts
	m_sMinLinText = "Min:";
	m_sMaxLinText = "Max:";
	m_sMinLogText = "Min: [dB]";
	m_sMaxLogText = "Max: [dB]";
	m_sBalloonLinResText = "r resolution:",
	m_sBalloonLogResText = "r resolution: [dB]",
	m_sBalloonPhiResText = "Phi resolution: [°]",
	m_sCarpetBetaResText = "Beta resolution: [°]",
	m_sCarpetAlphaResText = "Alpha resolution: [°]",
	m_sCarpetYLinResText = "Y resolution:",
	m_sCarpetYLogResText = "Y resolution: [dB]",
	m_sLevelLinText = "Level:",
	m_sLevelLogText = "Level: [dB]";
	m_sFrequencyText = "Frequency:";
	m_sAlphaAngleText = "Alpha angle:";
	m_sBetaAngleText = "Beta angle:";
	m_sSampleText = "Sample number:";

	FXGroupBox* helperGroupBox;
	FXMatrix* helperMatrix;
	FXVerticalFrame* helperVerticalFrame;
	FXHorizontalFrame* helperHorizontalFrame;

	// Screen
	m_pScreen = new FXVTK2::Frame(m_pFrameVisualization, FRAME_GROOVE | LAYOUT_FILL_X | LAYOUT_FILL_Y);
	m_pScreen->SetMouseInteractionMode(FXVTK2::Frame::MI_DIRECTIONAL_DATA);
	m_pScreen->SetCameraPosition(+5, +2.5, -5);
	m_pScreen->SetParallelProjection();
	m_pScreen->DollyCamera(0.6);

	m_pRootNode = new FXVTK2::SGNode();
	m_pScreen->SetSceneGraphRootNode(m_pRootNode);

	// Right-hand side controller bar
	m_pControllerSidebar = new FXVerticalFrame(m_pFrameVisualization, LAYOUT_FIX_WIDTH, 0, 0, 300, 0, 0, 0, 0, 0, 0, 0);

	// TODO: create scrollable frame
	//FXScrollArea* pScrollArea = new FXScrollArea(m_pFrameVisualization, LAYOUT_FIX_WIDTH, 0,0,300,0);
	//m_pControllerSidebar = pScrollArea;

	// Frequency control
	m_pSliderGroupBox = new FXGroupBox(m_pControllerSidebar, m_sFrequencyText, LAYOUT_FILL_X | FRAME_GROOVE);
	helperVerticalFrame = new FXVerticalFrame(m_pSliderGroupBox, LAYOUT_FILL_X);
	m_pFrequencySlider = new FXSlider(helperVerticalFrame, this, ID_FREQUENCY, LAYOUT_CENTER_Y | LAYOUT_FILL_X);
	m_pFrequencySlider->disable();
	m_pFrequencySlider->setRange(0,0);
	m_pFrequencySlider->setIncrement(1);
	m_pFrequencySlider->setValue(0);
	// TODO refactor m_pScreen->setFrequencySelector(m_pFrequencySlider);

	m_pFrequencyIndicator = new FXTextField(helperVerticalFrame, 80, NULL, ID_FREQUENCY, TEXTFIELD_NORMAL | LAYOUT_FILL_X | LAYOUT_CENTER_Y);
	m_pFrequencyIndicator->disable();
	m_pFrequencyIndicator->setBackColor(FXRGB(0, 0, 0));
	m_pFrequencyIndicator->setTextColor(FXRGB(0, 200, 0));
	
	// Channel control
	helperGroupBox = new FXGroupBox(m_pControllerSidebar, "Channels", LAYOUT_FILL_X | FRAME_GROOVE);
	m_pChannelList = new FXListBox(helperGroupBox, this, ID_CHANNEL, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pChannelList->setNumVisible(1);

	// Settings control
	helperGroupBox = new FXGroupBox(m_pControllerSidebar, "Settings", LAYOUT_FILL_X | FRAME_GROOVE);
	helperHorizontalFrame = new FXHorizontalFrame(helperGroupBox, LAYOUT_FILL_X);
	helperMatrix = new FXMatrix(helperHorizontalFrame,	1, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
	m_pGridCheckButton = new FXCheckButton(helperMatrix, "Grid", this, ID_GRID);
	m_pGridCheckButton->setCheck(1);
	m_pScaleCheckButton = new FXCheckButton(helperMatrix, "Scale", this, ID_SCALE);
	m_pScaleCheckButton->setCheck(1);
	m_pVectorsCheckButton = new FXCheckButton(helperMatrix, "Directions", this, ID_VECTORS);
	m_pVectorsCheckButton->setCheck(1);
	m_pNSPoleCheckButton = new FXCheckButton(helperMatrix, "Poles", this, ID_NSPOLE);
	m_pNSPoleCheckButton->setCheck(1);
	/*m_pNormalizeCheckButton = new FXCheckButton(helperMatrix, "Normalize
	d", this, ID_NORMALIZE);
	m_pNormalizeCheckButton->setCheck(0);*/
	m_pWarpCheckButton = new FXCheckButton(helperMatrix, "Warp", this, ID_WARP);
	m_pWarpCheckButton->setCheck(1);
	m_pColorCheckButton = new FXCheckButton(helperMatrix, "Color", this, ID_COLOR);
	m_pColorCheckButton->setCheck(1);
	m_pPhaseColorCheckButton = new FXCheckButton(helperMatrix, "Use Phase as Color", this, ID_COLOR);
	m_pPhaseColorCheckButton->setCheck(0);
	m_pWireCheckButton = new FXCheckButton(helperMatrix, "Wireframe", this, ID_WIRE);
	m_pWireCheckButton->setCheck(0);

	helperMatrix = new FXMatrix(helperHorizontalFrame,	1, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
	m_pDataLinearRadioButton = new FXRadioButton(helperMatrix, "Linear", this, ID_DATA_SCALING);
	m_pDataLinearRadioButton->setCheck(1);
	m_pDataLogRadioButton = new FXRadioButton(helperMatrix, "Decibel", this, ID_DATA_SCALING);
	m_pDataLogRadioButton->setCheck(0);
	m_pFixedAlphaRadioButton = new FXRadioButton(helperMatrix, "Alpha fixed", this, ID_FIXED_ANGLE);
	m_pFixedAlphaRadioButton->setCheck(0);
	m_pFixedBetaRadioButton = new FXRadioButton(helperMatrix, "Beta fixed", this, ID_FIXED_ANGLE);
	m_pFixedBetaRadioButton->setCheck(1);
	m_pBalloonPlotRadioButton = new FXRadioButton(helperMatrix, "Balloon Plot", this, ID_PLOT_TYPE);
	m_pBalloonPlotRadioButton->setCheck(0);
	m_pCarpetPlotRadioButton = new FXRadioButton(helperMatrix, "Carpet Plot", this, ID_PLOT_TYPE);
	m_pCarpetPlotRadioButton->setCheck(1);

	// Range control
	helperGroupBox = new FXGroupBox(m_pControllerSidebar, "Data Range", LAYOUT_FILL_X | FRAME_GROOVE);
	helperHorizontalFrame = new FXHorizontalFrame(helperGroupBox, LAYOUT_FILL_X);
	helperMatrix = new FXMatrix(helperHorizontalFrame,	1, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
	m_pMinLabel = new FXLabel(helperMatrix, m_sMinLinText);
	m_pMaxLabel = new FXLabel(helperMatrix, m_sMaxLinText);
	m_pRResLabel = new FXLabel(helperMatrix, m_sBalloonLinResText);
	m_pPhiResLabel = new FXLabel(helperMatrix, m_sBalloonPhiResText);
	m_pResetButton = new FXButton(helperMatrix, "Reset", 0, this, ID_RANGE, LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_SUNKEN);
	m_pResetButton->disable();
	helperMatrix = new FXMatrix(helperHorizontalFrame,	1, MATRIX_BY_COLUMNS | LAYOUT_FILL_X);
	m_pMinTextField = new FXTextField(helperMatrix, 15, this, ID_RANGE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pMinTextField->setText("0.0");
	m_pMinTextField->disable();
	m_pMaxTextField = new FXTextField(helperMatrix, 15, this, ID_RANGE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pMaxTextField->setText("1.0");
	m_pMaxTextField->disable();
	m_pRResTextField = new FXTextField(helperMatrix, 15, this, ID_RANGE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pRResTextField->setText("0.1");
	m_pRResTextField->disable();
	m_pPhiResTextField = new FXTextField(helperMatrix, 15, this, ID_RANGE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pPhiResTextField->setText("10");
	m_pPhiResTextField->disable();

	// Reference
	helperGroupBox = new FXGroupBox(m_pControllerSidebar, "Reference", LAYOUT_FILL_X | FRAME_GROOVE);
	helperVerticalFrame = new FXVerticalFrame(helperGroupBox, LAYOUT_FILL_X);
	
	m_pReferenceCheckButton = new FXCheckButton(helperVerticalFrame, "Enabled", this, ID_REFERENCE);
	m_pReferenceCheckButton->setCheck(1);
	
	helperHorizontalFrame = new FXHorizontalFrame(helperGroupBox, LAYOUT_FILL_X);
	m_pOpacityLabel = new FXLabel(helperHorizontalFrame, "Opacity:");
	
	helperVerticalFrame = new FXVerticalFrame(helperHorizontalFrame, LAYOUT_FILL_X);
	m_pOpacitySlider = new FXSlider(helperVerticalFrame, this, ID_REFERENCE, LAYOUT_CENTER_Y | LAYOUT_FILL_X);
	m_pOpacitySlider->disable();
	m_pOpacitySlider->setRange(0,100);
	m_pOpacitySlider->setIncrement(1);
	m_pOpacitySlider->setValue(0);

	m_pOpacityIndicator = new FXTextField(helperVerticalFrame, 80, NULL, ID_REFERENCE, TEXTFIELD_NORMAL | LAYOUT_FILL_X | LAYOUT_CENTER_Y);
	m_pOpacityIndicator->disable();
	m_pOpacityIndicator->setBackColor(FXRGB(0, 0, 0));
	m_pOpacityIndicator->setTextColor(FXRGB(0, 200, 0));
	m_pOpacityIndicator->setText("0%");

	helperHorizontalFrame = new FXHorizontalFrame(helperGroupBox, LAYOUT_FILL_X);
	m_pLevelLabel = new FXLabel(helperHorizontalFrame, m_sLevelLinText);

	helperVerticalFrame = new FXVerticalFrame(helperHorizontalFrame, LAYOUT_FILL_X);
	m_pLevelSlider = new FXSlider(helperVerticalFrame, this, ID_REFERENCE, LAYOUT_CENTER_Y | LAYOUT_FILL_X);
	m_pLevelSlider->disable();
	m_pLevelSlider->setRange(0,100);
	m_pLevelSlider->setIncrement(1);
	m_pLevelSlider->setValue(0);
	m_pLevelTextField = new FXTextField(helperVerticalFrame, 15, this, ID_REFERENCE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pLevelTextField->setText("0.0");
	m_pLevelTextField->disable();
	
	// Probe
	helperGroupBox = new FXGroupBox(m_pControllerSidebar, "Probe", LAYOUT_FILL_X | FRAME_GROOVE);
	helperVerticalFrame = new FXVerticalFrame(helperGroupBox, LAYOUT_FILL_X);
	
	m_pProbeCheckButton = new FXCheckButton(helperVerticalFrame, "Enabled", this, ID_PROBE);
	m_pProbeCheckButton->setCheck(1);
	
	helperHorizontalFrame = new FXHorizontalFrame(helperGroupBox, LAYOUT_FILL_X);
	m_pProbeFirstLabel = new FXLabel(helperHorizontalFrame, m_sAlphaAngleText);

	helperVerticalFrame = new FXVerticalFrame(helperHorizontalFrame, LAYOUT_FILL_X);
	m_pProbeFirstSlider = new FXSlider(helperVerticalFrame, this, ID_PROBE, LAYOUT_CENTER_Y | LAYOUT_FILL_X);
	m_pProbeFirstSlider->disable();
	m_pProbeFirstSlider->setRange(0,0);
	m_pProbeFirstSlider->setIncrement(1);
	m_pProbeFirstTextField = new FXTextField(helperVerticalFrame, 15, this, ID_PROBE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pProbeFirstTextField->setText("0.0");
	m_pProbeFirstTextField->disable();
	
	helperHorizontalFrame = new FXHorizontalFrame(helperGroupBox, LAYOUT_FILL_X);
	m_pProbeSecondLabel = new FXLabel(helperHorizontalFrame, m_sBetaAngleText);

	helperVerticalFrame = new FXVerticalFrame(helperHorizontalFrame, LAYOUT_FILL_X);
	m_pProbeSecondSlider = new FXSlider(helperVerticalFrame, this, ID_PROBE, LAYOUT_CENTER_Y | LAYOUT_FILL_X);
	m_pProbeSecondSlider->disable();
	m_pProbeSecondSlider->setRange(0,0);
	m_pProbeSecondSlider->setIncrement(1);
	m_pProbeSecondTextField = new FXTextField(helperVerticalFrame, 15, this, ID_PROBE, LAYOUT_CENTER_Y | LAYOUT_FILL_X | FRAME_SUNKEN);
	m_pProbeSecondTextField->setText("0.0");
	m_pProbeSecondTextField->disable();


	// --= Properties =--

	const DAFFProperties* props = m_pReader->getProperties();
	DAFFOrientationYPR ypr;
	props->getDefaultOrientation(ypr);

	int iRowCount = 15;
	int iRowIt = 0;
	helperframe = new FXVerticalFrame(m_pFrameProperties, LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_THICK | FRAME_SUNKEN, 0,0,0,0, 0,0,0,0);
	IFXFancyTable* pTable = new IFXFancyTable(helperframe, NULL, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y);
	pTable->setTableSize(iRowCount+props->getNumberOfChannels(), 2);
	pTable->setColumnText(0, "Key");
	pTable->setColumnText(1, "Value");

	pTable->setItemText(iRowIt, 0, "Content Type");
	pTable->setItemText(iRowIt++, 1, DAFFUtils::StrContentType(props->getContentType()).c_str());
	pTable->setItemText(iRowIt, 0, "File format version");
	pTable->setItemText(iRowIt++, 1, FXStringFormat("%i", props->getFileFormatVersion()).text());
	pTable->setItemText(iRowIt, 0, "Default orientation");
	pTable->setItemText(iRowIt++, 1, FXStringFormat("[%.2f°, %.2f°, %.2f] (Yaw, Pitch, Roll)", ypr.fYawAngle, ypr.fPitchAngle, ypr.fRollAngle).text());
	pTable->setItemText(iRowIt, 0, "Alpha points");
	pTable->setItemText(iRowIt++, 1, FXStringFormat("%i", props->getAlphaPoints()).text());
	pTable->setItemText(iRowIt, 0, "Alpha resolution");
	pTable->setItemText(iRowIt++, 1, FXStringFormat("%.2f°", props->getAlphaResolution()).text());
	pTable->setItemText(iRowIt, 0, "Alpha range");
	pTable->setItemText(iRowIt++, 1, FXStringFormat("[%.2f°, %.2f°]", props->getAlphaStart(), props->getAlphaEnd()).text());
	pTable->setItemText(iRowIt, 0, "Beta points");
	pTable->setItemText(iRowIt++, 1, FXStringFormat("%i", props->getBetaPoints()).text());
	pTable->setItemText(iRowIt, 0, "Beta resolution");
	pTable->setItemText(iRowIt++, 1, FXStringFormat("%.2f°", props->getBetaResolution()).text());
	pTable->setItemText(iRowIt, 0, "Beta range");
	pTable->setItemText(iRowIt++, 1, FXStringFormat("[%.2f°, %.2f°]", props->getBetaStart(), props->getBetaEnd()).text());
	pTable->setItemText(iRowIt, 0, "Covers full alpha range");
	pTable->setItemText(iRowIt++, 1, props->coversFullAlphaRange() ? "Yes" : "No");
	pTable->setItemText(iRowIt, 0, "Covers full beta range");
	pTable->setItemText(iRowIt++, 1, props->coversFullBetaRange() ? "Yes" : "No");
	pTable->setItemText(iRowIt, 0, "Covers full sphere");
	pTable->setItemText(iRowIt++, 1, props->coversFullSphere() ? "Yes" : "No");
	pTable->setItemText(iRowIt, 0, "Number of records");
	pTable->setItemText(iRowIt++, 1, FXStringFormat("%i", props->getNumberOfRecords()).text());
	pTable->setItemText(iRowIt, 0, "Quantization");
	pTable->setItemText(iRowIt++, 1, DAFFUtils::StrQuantizationType(props->getQuantization()).c_str());
	pTable->setItemText(iRowIt, 0, "Number of channels");
	pTable->setItemText(iRowIt++, 1, FXStringFormat("%i", props->getNumberOfChannels()).text());

	assert( iRowIt == iRowCount );

	for (int i = 0; i < props->getNumberOfChannels(); i++) {
		
		// Key name
		if (i==0)
			if (props->getNumberOfChannels() > 1)
				pTable->setItemText(iRowIt, 0, "Channel names");
			else
				pTable->setItemText(iRowIt, 0, "Channel name");
		else
			pTable->setItemText(iRowIt, 0, "");

		// Key value
		if (props->getChannelLabel(i).length() == 0)
			pTable->setItemText(iRowIt+i, 1, FXStringFormat("Channel %i not labeled", i).text());
		else
			pTable->setItemText(iRowIt+i, 1, props->getChannelLabel(i).c_str());
	}
		
	// --= Metadata =--
	const DAFFMetadata* pMetadata = m_pReader->getMetadata();
	std::vector<std::string> vsKeys;
	pMetadata->getKeys(vsKeys);

	helperframe = new FXVerticalFrame(m_pFrameMetadata, LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_THICK | FRAME_SUNKEN, 0,0,0,0, 0,0,0,0);
	pTable = new IFXFancyTable(helperframe, NULL, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y);
	if (!vsKeys.empty()) {
		pTable->setTableSize((int) vsKeys.size(), 2);
	} else {
		pTable->setTableSize(1, 2);
		pTable->setItemText(0, 0, "No meta data present");
	}

	pTable->setColumnText(0, "Key");
	pTable->setColumnText(1, "Value");

	int i = 0;

	// Run over metadata list
	for (std::vector<std::string>::const_iterator cit=vsKeys.begin(); cit != vsKeys.end(); ++cit) {

		// Key name
		pTable->setItemText(i, 0, (*cit).c_str());

		// Switch different key types for key values
		if (pMetadata->getKeyType(*cit) == DAFFMetadata::DAFF_BOOL)
			pTable->setItemText(i, 1, FXStringFormat("%b", pMetadata->getKeyBool(*cit)).text());
		if (pMetadata->getKeyType(*cit) == DAFFMetadata::DAFF_INT)
			pTable->setItemText(i, 1, FXStringFormat("%i", pMetadata->getKeyInt(*cit)).text());
		if (pMetadata->getKeyType(*cit) == DAFFMetadata::DAFF_FLOAT)
			pTable->setItemText(i, 1, FXStringFormat("%.2f", pMetadata->getKeyFloat(*cit)).text());
		if (pMetadata->getKeyType(*cit) == DAFFMetadata::DAFF_STRING)
			pTable->setItemText(i, 1, FXStringFormat("%s", pMetadata->getKeyString(*cit).c_str()).text());

		i++;
	}
}

void IFXDAFFFrame::arrangeDFT() {
	arrange_base();

	//new FXLabel(m_pFrameVisualization, "No visualization avaliable yet", NULL, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_CENTER_X | LAYOUT_CENTER_Y);

	// [...]
}

void IFXDAFFFrame::arrangeIR() {
	arrange_base();

	// Pull content
	DAFFContentIR* pIR = dynamic_cast<DAFFContentIR*>( m_pReader->getContent() );


	// Temporarily disable GUI elements
	m_pFrequencySlider->disable();
	m_pFrequencyIndicator->disable();
	m_pMaxTextField->disable();
	m_pMinTextField->disable();
	m_pWarpCheckButton->disable();
	m_pRResTextField->disable();
	m_pPhiResTextField->disable();
	m_pDataLinearRadioButton->disable();
	m_pCarpetPlotRadioButton->disable();
	m_pBalloonPlotRadioButton->disable();
	m_pDataLogRadioButton->disable();
	m_pChannelList->clearItems();
	m_pChannelList->disable();
	m_pGridCheckButton->disable();
	m_pScaleCheckButton->disable();
	m_pNSPoleCheckButton->disable();
	m_pVectorsCheckButton->disable();
	m_pColorCheckButton->disable();
	m_pWireCheckButton->disable();
	m_pFixedAlphaRadioButton->disable();
	m_pFixedBetaRadioButton->disable();
	m_pLevelTextField->disable();
	m_pOpacityIndicator->disable();
	m_pOpacitySlider->disable();
	m_pPhaseColorCheckButton->disable();
	m_pProbeCheckButton->disable();
	m_pProbeFirstTextField->disable();
	m_pProbeSecondTextField->disable();
	m_pProbeFirstSlider->disable();
	m_pProbeSecondSlider->disable();
	
	m_pWarpCheckButton->setCheck();
	m_pColorCheckButton->setCheck();
	m_pWireCheckButton->setCheck(false); // TODO?
	m_pPhaseColorCheckButton->setCheck(false); // TODO?
	m_pProbeCheckButton->setCheck(false);

	// Remove old plot child nodes
	if (m_pBalloonPlotNode) {
		m_pRootNode->RemoveChildNode(m_pBalloonPlotNode);
		delete m_pBalloonPlotNode;
		m_pBalloonPlotNode=0;
	}

	if (m_pCarpetPlotNode){
		m_pRootNode->RemoveChildNode(m_pCarpetPlotNode);
		delete m_pCarpetPlotNode;
		m_pCarpetPlotNode = 0;
	}

	if (m_pSphericalCoordAssist)
		m_pRootNode->RemoveChildNode(m_pSphericalCoordAssist);

	if (m_pCartesianCoordAssist)
		m_pRootNode->RemoveChildNode(m_pCartesianCoordAssist);
	
	m_vFrequencies.clear();
	
	const DAFFProperties* props = m_pReader->getProperties();

	// Carpet plot or ballon plot?
	DAFFContentDFT* pDFT = NULL;
	if (m_pCarpetPlotRadioButton->getCheck()) { // no transform
		m_pCarpetPlotNode = new FXVTK2::CarpetPlot( pIR );

		m_pRootNode->AddChildNode(m_pCarpetPlotNode);
		m_pCarpetPlotNode->SetSelectedAngle( 90 );

		m_pCartesianCoordAssist = new FXVTK2::CartesianCoordinateAssistant();
		m_pRootNode->AddChildNode(m_pCartesianCoordAssist);
		m_pCartesianCoordAssist->SetPosition(-1, 0, -1);
		m_pCartesianCoordAssist->SetAxesLength(2, 1, 2);
		m_pCartesianCoordAssist->SetMinZ(0);
		m_pCartesianCoordAssist->SetMaxZ(pIR->getFilterLength());
		m_pCartesianCoordAssist->SetResolutionZ(10);
		m_pCartesianCoordAssist->SetGridVisible(m_pGridCheckButton->getCheck() ? true : false);

	} else if (m_pBalloonPlotRadioButton->getCheck()) { // transform to directivity
		DAFFTransformerIR2DFT* transformer = new DAFFTransformerIR2DFT(pIR);
		pDFT = transformer->getOutputContent();
		m_pBalloonPlotNode = new FXVTK2::BalloonPlot( pDFT );
		m_pRootNode->AddChildNode(m_pBalloonPlotNode);
		m_pBalloonPlotNode->SetSelectedFrequency(0);
		m_pBalloonPlotNode->SetScale(1.0,1.0,1.0);
		m_pBalloonPlotNode->SetRange(0, pDFT->getOverallMagnitudeMaximum());

		m_pSphericalCoordAssist = new FXVTK2::SphericalCoordinateAssistant();
		m_pRootNode->AddChildNode(m_pSphericalCoordAssist);
	}

	// enable transformation radio buttons
	m_pCarpetPlotRadioButton->enable();
	m_pBalloonPlotRadioButton->enable();
	
	// Set up carpet plot controls
	if (m_pCarpetPlotNode) {
		// Update Range TextFields and checkboxes
		m_pMaxTextField->setText(FXStringVal(m_pCarpetPlotNode->GetRangeMax()));
		m_pMinTextField->setText(FXStringVal(m_pCarpetPlotNode->GetRangeMin()));
		m_pFixedAlphaRadioButton->enable();
		m_pFixedBetaRadioButton->enable();
		
		//m_pFixedBetaRadioButton->setCheck(1);
		if (m_pFixedBetaRadioButton->getCheck()) {
			m_pCarpetPlotNode->SetFixedAngle(FXVTK2::CarpetPlot::BETA_FIXED);
			m_pCartesianCoordAssist->SetMinX(pIR->getProperties()->getAlphaStart());
			m_pCartesianCoordAssist->SetMaxX(pIR->getProperties()->getAlphaEnd());
			m_pCartesianCoordAssist->SetResolutionX(20);
			m_pFrequencySlider->setRange(0, pIR->getProperties()->getBetaPoints()-1);
			m_pFrequencySlider->setValue(pIR->getProperties()->getBetaPoints()/2);
			m_pSliderGroupBox->setText(m_sBetaAngleText);
			m_pProbeFirstSlider->setRange(pIR->getProperties()->getAlphaStart(), pIR->getProperties()->getAlphaEnd());
			m_pProbeFirstSlider->setIncrement(pIR->getProperties()->getAlphaResolution());
			m_pProbeFirstLabel->setText(m_sAlphaAngleText);
		} else {
			m_pCarpetPlotNode->SetFixedAngle(FXVTK2::CarpetPlot::ALPHA_FIXED);
			m_pCartesianCoordAssist->SetMinX(pIR->getProperties()->getBetaStart());
			m_pCartesianCoordAssist->SetMaxX(pIR->getProperties()->getBetaEnd());
			m_pCartesianCoordAssist->SetResolutionX(10);
			m_pFrequencySlider->setRange(0, pIR->getProperties()->getAlphaPoints()-1);
			m_pFrequencySlider->setValue(pIR->getProperties()->getAlphaPoints()/2);
			m_pSliderGroupBox->setText(m_sAlphaAngleText);
			m_pProbeFirstSlider->setRange(pIR->getProperties()->getBetaStart(), pIR->getProperties()->getBetaEnd());
			m_pProbeFirstSlider->setIncrement(pIR->getProperties()->getBetaResolution());	
			m_pProbeFirstLabel->setText(m_sBetaAngleText);
		}
		m_pProbeSecondSlider->setRange(0, pIR->getFilterLength()-1);
		m_pProbeSecondSlider->setIncrement(1);
		m_pProbeSecondLabel->setText(m_sSampleText);

		for (int i=0; i<pIR->getProperties()->getBetaPoints(); i++) 
			m_vFrequencies.push_back(pIR->getProperties()->getBetaStart()+ i*pIR->getProperties()->getBetaResolution());
		
		m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + "°" );

		if (m_pDataLinearRadioButton->getCheck()) 
			m_pCarpetPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_LINEAR);
		else
			m_pCarpetPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_DECIBEL);
		
		m_pProbeCheckButton->enable();
		m_pProbeFirstTextField->enable();
		m_pProbeSecondTextField->enable();
		m_pProbeFirstSlider->enable();
		m_pProbeSecondSlider->enable();
	}

	// Set up balloon plot controls
	if (m_pBalloonPlotNode) {
		m_pSliderGroupBox->setText(m_sFrequencyText);
		m_pFrequencySlider->setValue(0);
		m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + " Hz" );
		
		m_pMaxTextField->setText(FXStringVal(m_pBalloonPlotNode->GetRangeMax()));
		m_pMinTextField->setText(FXStringVal(m_pBalloonPlotNode->GetRangeMin()));
		
		// update scaling
		if (m_pDataLinearRadioButton->getCheck()) 
			m_pBalloonPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_LINEAR);
		else
			m_pBalloonPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_DECIBEL);
		
		m_pProbeFirstLabel->setText(m_sAlphaAngleText);
		m_pProbeSecondLabel->setText(m_sBetaAngleText);
		m_pNSPoleCheckButton->enable();
		m_pVectorsCheckButton->enable();
		m_pRResTextField->enable();
		m_pProbeCheckButton->enable();
		m_pProbeFirstTextField->enable();
		m_pProbeSecondTextField->enable();
		m_pProbeFirstSlider->enable();
		m_pProbeSecondSlider->enable();
	}

	// Enable text fields and check boxes
	m_pWarpCheckButton->enable();
	m_pRResTextField->enable();
	m_pPhiResTextField->enable();
	m_pResetButton->enable();
	m_pGridCheckButton->enable();
	m_pScaleCheckButton->enable();
	m_pColorCheckButton->enable();
	m_pWireCheckButton->enable();
	m_pDataLinearRadioButton->enable();
	m_pDataLogRadioButton->enable();
	m_pMaxTextField->enable();
	m_pMinTextField->enable();
	m_pLevelTextField->enable();
	m_pLevelSlider->enable();
	m_pOpacitySlider->enable();

	// Adjust frequency slider
	m_pFrequencySlider->enable();
			
	// Update channel list
	for (int i=1; i <= props->getNumberOfChannels(); i++) {
		if( props->getChannelLabel(i-1).length() == 0) {
			m_pChannelList->appendItem("Channel " + FXStringFormat("%d", i));
		} else {
			m_pChannelList->appendItem("Ch. " + FXStringFormat("%d", i) + " - " + props->getChannelLabel(i-1).c_str());
		}
	}
	m_pChannelList->setNumVisible(props->getNumberOfChannels());
	m_pChannelList->enable();

	// TODO: use events instead
	onCmdRange(m_pResetButton, 0, 0);
	onCmdReference(m_pReferenceCheckButton, 0, 0);
	onCmdWarp(m_pWarpCheckButton, 0, 0);

	// TODO: use props ointer instead of long ->GetProperties etc
}

void IFXDAFFFrame::arrangeMS() {
	arrange_base();

	// Pull content
	DAFFContentMS* pMS = dynamic_cast<DAFFContentMS*>( m_pReader->getContent() );
		
	// Temporarily disable GUI elements
	m_pFrequencySlider->disable();
	m_pFrequencyIndicator->disable();
	m_pMaxTextField->disable();
	m_pMinTextField->disable();
	m_pWarpCheckButton->disable();
	m_pRResTextField->disable();
	m_pPhiResTextField->disable();
	m_pDataLinearRadioButton->disable();
	m_pCarpetPlotRadioButton->disable();
	m_pBalloonPlotRadioButton->disable();
	m_pDataLogRadioButton->disable();
	m_pChannelList->clearItems();
	m_pChannelList->disable();
	m_pGridCheckButton->disable();
	m_pScaleCheckButton->disable();
	m_pNSPoleCheckButton->disable();
	m_pVectorsCheckButton->disable();
	m_pColorCheckButton->disable();
	m_pWireCheckButton->disable();
	m_pFixedAlphaRadioButton->disable();
	m_pFixedBetaRadioButton->disable();
	m_pLevelTextField->disable();
	m_pOpacityIndicator->disable();
	m_pOpacitySlider->disable();
	m_pPhaseColorCheckButton->disable();
	m_pProbeCheckButton->disable();
	m_pProbeFirstTextField->disable();
	m_pProbeSecondTextField->disable();
	m_pProbeFirstSlider->disable();
	m_pProbeSecondSlider->disable();

	m_pWarpCheckButton->setCheck();
	m_pColorCheckButton->setCheck();
	m_pWireCheckButton->setCheck(false); // TODO?
	m_pPhaseColorCheckButton->setCheck(false); // TODO?
	m_pProbeCheckButton->setCheck(false);

	// Remove old plot child nodes
	if (m_pBalloonPlotNode) {
		m_pRootNode->RemoveChildNode(m_pBalloonPlotNode);
		delete m_pBalloonPlotNode;
		m_pBalloonPlotNode = NULL;
	}

	if (m_pCarpetPlotNode) {
		m_pRootNode->RemoveChildNode(m_pCarpetPlotNode);
		delete m_pCarpetPlotNode;
		m_pCarpetPlotNode = NULL;
	}

	if (m_pSphericalCoordAssist) 
		m_pRootNode->RemoveChildNode(m_pSphericalCoordAssist);

	if (m_pCartesianCoordAssist) 
		m_pRootNode->RemoveChildNode(m_pCartesianCoordAssist);

	// Reset frequencies
	m_vFrequencies.clear(); // TODO: brauchen wir so eine Membervariable?
	m_vFrequencies = pMS->getFrequencies();
	m_pFrequencySlider->setRange(0, pMS->getNumFrequencies()-1);

	// Balloon plot
	m_pBalloonPlotNode = new FXVTK2::BalloonPlot(m_pRootNode, pMS);
	m_pBalloonPlotNode->SetSelectedFrequency(0);
	m_pBalloonPlotNode->SetScale(1, 1, 1);
	m_pBalloonPlotNode->SetRange(0, pMS->getOverallMagnitudeMaximum());

	// Spherical coordinate assistant
	m_pSphericalCoordAssist = new FXVTK2::SphericalCoordinateAssistant();
	m_pRootNode->AddChildNode(m_pSphericalCoordAssist);
		
	// Probe
	m_pProbeFirstSlider->setRange(pMS->getProperties()->getAlphaStart(), pMS->getProperties()->getAlphaEnd());
	m_pProbeSecondSlider->setRange(pMS->getProperties()->getBetaStart(), pMS->getProperties()->getBetaEnd());
	m_pProbeFirstSlider->setIncrement(pMS->getProperties()->getAlphaResolution());
	m_pProbeSecondSlider->setIncrement(pMS->getProperties()->getBetaResolution());
	m_pProbeFirstLabel->setText(m_sAlphaAngleText);
	m_pProbeSecondLabel->setText(m_sBetaAngleText);

	m_pSliderGroupBox->setText(m_sFrequencyText);
	m_pFrequencySlider->setValue(0);
	m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + " Hz" );

	m_pMaxTextField->setText(FXStringVal(m_pBalloonPlotNode->GetRangeMax()));
	m_pMinTextField->setText(FXStringVal(m_pBalloonPlotNode->GetRangeMin()));

	// Update scaling
	if (m_pDataLinearRadioButton->getCheck())
		m_pBalloonPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_LINEAR);
	else
		m_pBalloonPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_DECIBEL);

	// Enable relevant controls
	m_pNSPoleCheckButton->enable();
	m_pVectorsCheckButton->enable();
	m_pRResTextField->enable();
	m_pProbeCheckButton->enable();
	m_pProbeFirstTextField->enable();
	m_pProbeSecondTextField->enable();
	m_pProbeFirstSlider->enable();
	m_pProbeSecondSlider->enable();
	
	// Enable text fields and check boxes
	m_pWarpCheckButton->enable();
	m_pRResTextField->enable();
	m_pPhiResTextField->enable();
	m_pResetButton->enable();
	m_pGridCheckButton->enable();
	m_pScaleCheckButton->enable();
	m_pColorCheckButton->enable();
	m_pWireCheckButton->enable();
	m_pDataLinearRadioButton->enable();
	m_pDataLogRadioButton->enable();
	m_pMaxTextField->enable();
	m_pMinTextField->enable();
	m_pLevelTextField->enable();
	m_pLevelSlider->enable();
	m_pOpacitySlider->enable();

	// Enable frequency slider
	m_pFrequencySlider->enable();
	
	// Update channel list
	for (int i=1; i <= pMS->getProperties()->getNumberOfChannels(); i++) {
		if( pMS->getProperties()->getChannelLabel(i-1).length() == 0) {
			m_pChannelList->appendItem("Channel " + FXStringFormat("%d", i));
		} else {
			m_pChannelList->appendItem("Ch. " + FXStringFormat("%d", i) + " - " + pMS->getProperties()->getChannelLabel(i-1).c_str());
		}
	}
	m_pChannelList->setNumVisible(pMS->getProperties()->getNumberOfChannels());
	m_pChannelList->enable();

	// Trigger range, probe and warp reset
	// TODO: use events instead
	onCmdRange(m_pResetButton, 0, 0);
	onCmdReference(m_pReferenceCheckButton, 0, 0);
	onCmdWarp(m_pWarpCheckButton, 0, 0);

}

long IFXDAFFFrame::onCmdFrequency(FXObject*, FXSelector, void*) {
	if (m_pBalloonPlotNode) {
		m_pBalloonPlotNode->SetSelectedFrequency( m_pFrequencySlider->getValue() );
		m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + " Hz" );
	} else {
		m_pCarpetPlotNode->SetSelectedAngle( m_vFrequencies[ m_pFrequencySlider->getValue() ] );
		m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + "°" );
	}
	m_pScreen->Render();

	return 1;
}

/*
long IFXDAFFFrame::onWheelFreq(FXObject*, FXSelector, void*) {
	onSlideFreq(NULL,0,NULL);
	return 1;
}
*/

long IFXDAFFFrame::onCmdChannels(FXObject*, FXSelector, void*) {
	if (m_pBalloonPlotNode) {
		m_pBalloonPlotNode->SetChannel(m_pChannelList->getCurrentItem());
		m_pScreen->Render();
	}
	if (m_pCarpetPlotNode) {
		m_pCarpetPlotNode->SetChannel(m_pChannelList->getCurrentItem());
		m_pScreen->Render();
	}

	return 1;
}

/*
long IFXDAFFFrame::onCmdScale(FXObject*, FXSelector, void*) {
	bool bEnabled = m_pScaleCheckButton->getCheck() > 0 ? true : false;
	m_pSphereCoordAssist->setGridVisible(bEnabled);
	m_pKartesianCoordAssist->setGridVisible(bEnabled);
	m_pScreen->Render();
	return 1;
}
*/

long IFXDAFFFrame::onCmdGrid(FXObject*, FXSelector, void*) {
	bool bEnabled = m_pGridCheckButton->getCheck() > 0 ? true : false;

	if (m_pSphericalCoordAssist)
		m_pSphericalCoordAssist->SetGridVisible(bEnabled);

	if (m_pCartesianCoordAssist)
		m_pCartesianCoordAssist->SetGridVisible(bEnabled);

	// Update screen
	m_pScreen->Render();

	return 1;
}

long IFXDAFFFrame::onCmdVectors(FXObject*, FXSelector, void*) {
	bool bEnabled = m_pVectorsCheckButton->getCheck() > 0 ? true : false;
	
	if (m_pSphericalCoordAssist)
		m_pSphericalCoordAssist->SetViewUpVectorsVisible(bEnabled);

	if (m_pCartesianCoordAssist)
		m_pCartesianCoordAssist->SetAxesVisible(bEnabled);

	// Update screen
	m_pScreen->Render();

	return 1;
}

long IFXDAFFFrame::onCmdNSPole(FXObject*, FXSelector, void*) {
	bool bEnabled = m_pNSPoleCheckButton->getCheck() > 0 ? true : false;
	m_pSphericalCoordAssist->SetAxesVisible(bEnabled);

	// Update screen
	m_pScreen->Render();

	return 1;
}

long IFXDAFFFrame::onCmdWarp(FXObject* sender, FXSelector, void*) {

	// Control warping on balloon plot
	if (m_pBalloonPlotNode) {
		if (m_pWarpCheckButton->getCheck())
			m_pBalloonPlotNode->EnableWarp(); // Enable
		else
			m_pBalloonPlotNode->DisableWarp(); // Disable
	}
	
	// Control warping on carpet plot
	if (m_pCarpetPlotNode) {
		if (m_pWarpCheckButton->getCheck()) {
			// Enable
			m_pCarpetPlotNode->EnableWarp();
			/* TODO why commented?
			if (m_pLogRadioButton->getCheck()) // we have to move the plot-node, because for the negative db-scale the warping goes to the other direction
				m_pCarpetPlotNode->SetPosition(0,1.0,0);
			else
				m_pCarpetPlotNode->SetPosition(0,0.0,0);*/
		} else {
			// Disable
			m_pCarpetPlotNode->DisableWarp();
			m_pCarpetPlotNode->SetPosition(0,0.0,0);
		}
	}

	// Update screen
	m_pScreen->Render();

	return 1;
}

long IFXDAFFFrame::onCmdColor(FXObject* sender, FXSelector, void*) {
	bool bEnabled = m_pColorCheckButton->getCheck() > 0 ? true : false;
	if (m_pBalloonPlotNode) {
		m_pBalloonPlotNode->SetScalarVisibility(bEnabled);
		m_pBalloonPlotNode->SetUsePhaseAsColor(m_pPhaseColorCheckButton->getCheck() ? true : false);
	}
	if (m_pCarpetPlotNode)
		m_pCarpetPlotNode->SetScalarVisibility(bEnabled);
	m_pScreen->Render();
	return 1;
}

long IFXDAFFFrame::onCmdWire(FXObject* sender, FXSelector, void*) {
	if (m_pBalloonPlotNode) {
		int iMode = m_pWireCheckButton->getCheck() > 0 ? FXVTK2::BalloonPlot::MODE_WIREFRAME : FXVTK2::BalloonPlot::MODE_SURFACE;
		m_pBalloonPlotNode->SetDisplayMode(iMode);
	}
	if (m_pCarpetPlotNode) {
		int iMode = m_pWireCheckButton->getCheck() > 0 ? FXVTK2::CarpetPlot::MODE_WIREFRAME : FXVTK2::CarpetPlot::MODE_SURFACE;
		m_pCarpetPlotNode->SetDisplayMode(iMode);
	}
	m_pScreen->Render();
	return 1;
}

long IFXDAFFFrame::onCmdScaling(FXObject* sender, FXSelector, void*) {
	float fPrecision = 1.0f;
	// Invert states
	if ((FXRadioButton*) sender == m_pDataLinearRadioButton)
		m_pDataLogRadioButton->setCheck(!m_pDataLinearRadioButton->getCheck());
	else
		m_pDataLinearRadioButton->setCheck(!m_pDataLogRadioButton->getCheck());

	// IR balloon plot representation
	if (m_pBalloonPlotNode) {
		// Delegate scaling option
		if (m_pDataLinearRadioButton->getCheck()) {
			// Show linear scaling
			m_pBalloonPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_LINEAR);
			m_pMinLabel->setText(m_sMinLinText);
			m_pMaxLabel->setText(m_sMaxLinText);
			m_pRResLabel->setText(m_sBalloonLinResText);
			// TODO m_pPhiResLabel->setText(m_sBallonPhiResText);
			m_pLevelLabel->setText(m_sLevelLinText);
		} else {
			// Show decibel scaling
			m_pBalloonPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_DECIBEL);
			m_pMinLabel->setText(m_sMinLogText);
			m_pMaxLabel->setText(m_sMaxLogText);
			m_pRResLabel->setText(m_sBalloonLogResText);
			// TODO m_pPhiResLabel->setText(m_sBallonPhiResText);
			m_pLevelLabel->setText(m_sLevelLogText);
		}
	}

	// IR carpet plot representation
	if (m_pCarpetPlotNode) {
		// Delegate scaling option
		if (m_pDataLinearRadioButton->getCheck()) {
			// Show linear scaling
			m_pCarpetPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_LINEAR);
			m_pMinLabel->setText(m_sMinLinText);
			m_pMaxLabel->setText(m_sMaxLinText);
			m_pRResLabel->setText(m_sCarpetYLinResText);
			if (m_pFixedAlphaRadioButton->getCheck())
				m_pPhiResLabel->setText(m_sCarpetBetaResText);
			else
				m_pPhiResLabel->setText(m_sCarpetAlphaResText);
			m_pLevelLabel->setText(m_sLevelLinText);
		} else {
			// Show decibel scaling
			m_pCarpetPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_DECIBEL);
			m_pMinLabel->setText(m_sMinLogText);
			m_pMaxLabel->setText(m_sMaxLogText);
			m_pRResLabel->setText(m_sCarpetYLogResText);
			if (m_pFixedAlphaRadioButton->getCheck())
				m_pPhiResLabel->setText(m_sCarpetBetaResText);
			else
				m_pPhiResLabel->setText(m_sCarpetAlphaResText);
			m_pLevelLabel->setText(m_sLevelLogText);
		}
	}
	
	// Update level indicator
	float fLevel = FXDoubleVal(m_pLevelTextField->getText());
	if (m_pDataLinearRadioButton->getCheck())
		m_pLevelTextField->setText(FXStringVal(pow(10, 0.1*fLevel))); // Linear
	else
		m_pLevelTextField->setText(FXStringVal(10*log(fLevel)/log(10.0))); // Decibel
	
	// TODO: trigger event instead
	onCmdRange(m_pResetButton, 0, 0);
	onCmdWarp(m_pWarpCheckButton, 0, 0);

	// Update screen
	m_pScreen->Render();

	return 1;
}

long IFXDAFFFrame::onCmdFixedAngle(FXObject* sender, FXSelector, void*) {
	// Invert states
	if ((FXRadioButton*) sender == m_pFixedBetaRadioButton)
		m_pFixedAlphaRadioButton->setCheck(!m_pFixedBetaRadioButton->getCheck());
	else
		m_pFixedBetaRadioButton->setCheck(!m_pFixedAlphaRadioButton->getCheck());

	if (m_pCarpetPlotNode) {
		if (m_pFixedBetaRadioButton->getCheck()) {
			m_pCarpetPlotNode->SetFixedAngle(FXVTK2::CarpetPlot::BETA_FIXED);
			//adjust frequency slider
			m_pFrequencySlider->setRange(0, m_pReader->getProperties()->getBetaPoints()-1);
			m_pFrequencySlider->setValue(m_pReader->getProperties()->getBetaPoints()/2);
			m_pFrequencySlider->enable();
			m_pProbeFirstSlider->setRange(m_pReader->getProperties()->getAlphaStart(), m_pReader->getProperties()->getAlphaEnd());
			m_pProbeFirstSlider->setIncrement(m_pReader->getProperties()->getAlphaResolution());
			m_pProbeFirstLabel->setText(m_sAlphaAngleText);
			

			m_vFrequencies.clear();
			for (int i=0; i<m_pReader->getProperties()->getBetaPoints(); i++) 
				m_vFrequencies.push_back(m_pReader->getProperties()->getBetaStart()+ i*m_pReader->getProperties()->getBetaResolution());
			
			m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + "°" );
			m_pSliderGroupBox->setText(m_sBetaAngleText);
			m_pCarpetPlotNode->SetSelectedAngle( m_vFrequencies[ m_pFrequencySlider->getValue() ] );

			m_pCartesianCoordAssist->SetMinX(m_pReader->getProperties()->getAlphaStart());
			m_pCartesianCoordAssist->SetMaxX(m_pReader->getProperties()->getAlphaEnd());
			m_pCartesianCoordAssist->SetResolutionX(20);
		} else {
			m_pCarpetPlotNode->SetFixedAngle(FXVTK2::CarpetPlot::ALPHA_FIXED);
			//adjust frequency slider
			m_pFrequencySlider->setRange(0, m_pReader->getProperties()->getAlphaPoints()-1);
			m_pFrequencySlider->setValue(m_pReader->getProperties()->getAlphaPoints()/2);
			m_pFrequencySlider->enable();
			m_pProbeFirstSlider->setRange(m_pReader->getProperties()->getBetaStart(), m_pReader->getProperties()->getBetaEnd());
			m_pProbeFirstSlider->setIncrement(m_pReader->getProperties()->getBetaResolution());
			m_pProbeFirstLabel->setText(m_sBetaAngleText);

			m_vFrequencies.clear();
			for (int i=0; i<m_pReader->getProperties()->getAlphaPoints(); i++) 
				m_vFrequencies.push_back(m_pReader->getProperties()->getAlphaStart()+ i*m_pReader->getProperties()->getAlphaResolution());
			
			m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + "°" );
			m_pSliderGroupBox->setText(m_sAlphaAngleText);
			m_pCarpetPlotNode->SetSelectedAngle( m_vFrequencies[ m_pFrequencySlider->getValue() ] );

			m_pCartesianCoordAssist->SetMinX(m_pReader->getProperties()->getBetaStart());
			m_pCartesianCoordAssist->SetMaxX(m_pReader->getProperties()->getBetaEnd());
			m_pCartesianCoordAssist->SetResolutionX(10);
		}
	}
	//m_pKartesianCoordAssist->updateGrid();
	m_pScreen->Render();
	return 1;
}


long IFXDAFFFrame::onCmdPlotType(FXObject* sender, FXSelector, void*) {
	
	// Invert states
	if ((FXRadioButton*) sender == m_pBalloonPlotRadioButton) {
		m_pCarpetPlotRadioButton->setCheck(!m_pBalloonPlotRadioButton->getCheck());
		if (m_pDataLinearRadioButton->getCheck()) 
			m_pRResLabel->setText(m_sBalloonLinResText);
		else
			m_pRResLabel->setText(m_sBalloonLogResText);

		m_pPhiResLabel->setText(m_sBalloonPhiResText);
	} else {
		m_pBalloonPlotRadioButton->setCheck(!m_pCarpetPlotRadioButton->getCheck());
		if (m_pDataLinearRadioButton->getCheck()) 
			m_pRResLabel->setText(m_sCarpetYLinResText);
		else 
			m_pRResLabel->setText(m_sCarpetYLogResText);
		if (m_pFixedAlphaRadioButton->getCheck())
			m_pPhiResLabel->setText(m_sCarpetAlphaResText);
		else
			m_pPhiResLabel->setText(m_sCarpetBetaResText);
	}
	
	// Update balloon plot
	if (m_pBalloonPlotRadioButton->getCheck() && m_pReader) {
		// Remove old plot
		if (m_pBalloonPlotNode) {
			m_pRootNode->RemoveChildNode(m_pBalloonPlotNode);
			delete m_pBalloonPlotNode;
			m_pBalloonPlotNode = NULL;
		}
		if (m_pSphericalCoordAssist) m_pRootNode->RemoveChildNode(m_pSphericalCoordAssist);
		if (m_pCartesianCoordAssist) m_pRootNode->RemoveChildNode(m_pCartesianCoordAssist);

		const DAFFProperties* props = m_pReader->getProperties();

		 // Pull content and transform
		DAFFContentIR* pIR = dynamic_cast<DAFFContentIR*>( m_pReader->getContent() );
		DAFFTransformerIR2DFT* transformer = new DAFFTransformerIR2DFT(pIR);
		DAFFContentDFT* pDFT = transformer->getOutputContent();

		// Create balloon plot node
		m_pBalloonPlotNode = new FXVTK2::BalloonPlot( pDFT );
		m_pRootNode->AddChildNode(m_pBalloonPlotNode);
		m_pBalloonPlotNode->SetSelectedFrequency(0);
		m_pBalloonPlotNode->SetScale(1.0,1.0,1.0);
		m_pBalloonPlotNode->SetRange(0, pDFT->getOverallMagnitudeMaximum());

		m_pSphericalCoordAssist = new FXVTK2::SphericalCoordinateAssistant();
		m_pRootNode->AddChildNode(m_pSphericalCoordAssist);
	}

	// Update carpet plot
	if (m_pCarpetPlotRadioButton->getCheck()) {
		// Remove old plot
		if (m_pCarpetPlotNode){
			m_pRootNode->RemoveChildNode(m_pCarpetPlotNode);
			delete m_pCarpetPlotNode;
			m_pCarpetPlotNode = NULL;
		}

		if (m_pSphericalCoordAssist) m_pRootNode->RemoveChildNode(m_pSphericalCoordAssist);
		if (m_pCartesianCoordAssist) m_pRootNode->RemoveChildNode(m_pCartesianCoordAssist);

		// Pull content
		DAFFContentIR* pIR = dynamic_cast<DAFFContentIR*>( m_pReader->getContent() );

		// Create carpet plot node
		m_pCarpetPlotNode = new FXVTK2::CarpetPlot( pIR );
		m_pRootNode->AddChildNode(m_pCarpetPlotNode);
		m_pCarpetPlotNode->SetSelectedAngle( 90 );

		m_pCartesianCoordAssist = new FXVTK2::CartesianCoordinateAssistant();
		m_pRootNode->AddChildNode(m_pCartesianCoordAssist);
		m_pCartesianCoordAssist->SetPosition(-1, 0, -1);
		m_pCartesianCoordAssist->SetAxesLength(2, 1, 2);
		m_pCartesianCoordAssist->SetMinZ(0);
		m_pCartesianCoordAssist->SetMaxZ(pIR->getFilterLength());
		m_pCartesianCoordAssist->SetResolutionZ(10);
		m_pCartesianCoordAssist->SetGridVisible(m_pGridCheckButton->getCheck() ? true : false);
	}

	m_pScreen->Render();

	return 1;
}

long IFXDAFFFrame::onCmdSreenshot(FXObject* sender, FXSelector, void*) {
	m_pScreen->SaveScreenshot("IFXDAFFFrameScreenshot.png");

	return 1;
}

long IFXDAFFFrame::onCmdRange(FXObject* sender, FXSelector, void*) {
	float min = 0.0f, max = 1.0f, prec = 1.0f;
	double res = 10;
	if (sender == m_pResetButton) {
		// try to find good values
		if (m_pBalloonPlotNode) {
			min = 0.0;
			if (m_pReader->getContentType() == DAFF_DFT_SPECTRUM)
				max = dynamic_cast<DAFFContentDFT*>( m_pReader->getContent() )->getOverallMagnitudeMaximum();
			if (m_pReader->getContentType() == DAFF_MAGNITUDE_SPECTRUM)
				max = dynamic_cast<DAFFContentMS*>( m_pReader->getContent() )->getOverallMagnitudeMaximum();
		}
		if (m_pCarpetPlotNode) {
			if (m_pReader->getContentType() == DAFF_IMPULSE_RESPONSE) {
				max = dynamic_cast<DAFFContentIR*>( m_pReader->getContent() )->getOverallPeak();
				min = -max;
			}
		}
		if (m_pDataLogRadioButton->getCheck()) {
			// convert to log:
			min = -30; 
			max = 10*log(max)/log((double) 10);
		}
		prec = fabs(max-min);
		float n=1;
		while (int(prec/n)!=0)
			n*=2;
		prec = n/10;
		m_pRResTextField->setText(FXStringVal(prec));
		m_pPhiResTextField->setText(FXStringVal(res));
		m_pMaxTextField->setText(FXStringVal(max));
		m_pMinTextField->setText(FXStringVal(min));
	} else {
		min = FXFloatVal(m_pMinTextField->getText());
		max = FXFloatVal(m_pMaxTextField->getText());
		prec = FXFloatVal(m_pRResTextField->getText());
		res = FXIntVal(m_pPhiResTextField->getText());

		if (fmod(360.0, res) != 0) 
			m_pPhiResTextField->setText(FXStringVal(res - fmod(res, 360.0)));

		if (min > max) {
			if ((FXTextField*) sender == m_pMinTextField)
				m_pMaxTextField->setText(m_pMinTextField->getText());
			else
				m_pMinTextField->setText(m_pMaxTextField->getText());
		}
	}

	// Update Plot
	if (m_pBalloonPlotNode) {
		m_pBalloonPlotNode->SetRange(min, max);
		m_pSphericalCoordAssist->SetMin(min);
		m_pSphericalCoordAssist->SetMax(max);
		m_pSphericalCoordAssist->SetRResolution(prec);
		m_pSphericalCoordAssist->SetPhiResolution(res);
	
		if (fmod(360.0, res) != 0) 
			m_pPhiResTextField->setText(FXStringVal(m_pSphericalCoordAssist->GetPhiResolution()));
		// TODO: update grid ...m_pSphereCoordAssist->updateGrid();
	} else if (m_pCarpetPlotNode) {
		m_pCarpetPlotNode->SetRange(min, max);
		m_pCartesianCoordAssist->SetMinY(min);
		m_pCartesianCoordAssist->SetMaxY(max);
		m_pCartesianCoordAssist->SetResolutionY(prec);
		if ((m_pDataLinearRadioButton->getCheck()) && ((min < 0) && (max > 0)))
			m_pCartesianCoordAssist->SetOffsetY(0.0);
		else
			m_pCartesianCoordAssist->SetOffsetY(min);

		//m_pKartesianCoordAssist->updateGrid();
	}
	
	onCmdReference(m_pLevelTextField, 0, 0);
	m_pScreen->Render();

	return 1;
}

long IFXDAFFFrame::onCmdReference(FXObject* sender, FXSelector, void*) {
	double min = FXDoubleVal(m_pMinTextField->getText()),
		max = FXDoubleVal(m_pMaxTextField->getText());
	if (sender == m_pLevelSlider){
		m_pLevelTextField->setText(FXStringVal((double)m_pLevelSlider->getValue()/100.0*(max-min)+min));
	} else if (sender == m_pLevelTextField) {
		double val = FXDoubleVal(m_pLevelTextField->getText());
		m_pLevelSlider->setValue((val-min)/(max-min)*100);
	}
	m_pOpacityIndicator->setText( FXStringFormat("%d", m_pOpacitySlider->getValue()) + " %" );

	if (m_pCarpetPlotNode) {
		if (m_pCartesianCoordAssist) {
			m_pCartesianCoordAssist->SetReferenceLevel(FXDoubleVal(m_pLevelTextField->getText()));
			m_pCartesianCoordAssist->SetReferenceOpacity((double)m_pOpacitySlider->getValue()/100.0);
		}
		
	}
	if (m_pBalloonPlotNode) {
		if (m_pSphericalCoordAssist) {
			m_pSphericalCoordAssist->SetReferenceLevel(FXDoubleVal(m_pLevelTextField->getText()));
			m_pSphericalCoordAssist->SetReferenceOpacity((double)m_pOpacitySlider->getValue()/100.0);
		}
		
	}
	
	if (m_pReferenceCheckButton->getCheck()) {
		m_pLevelTextField->enable();
		m_pOpacitySlider->enable();
	} else {
		if (m_pSphericalCoordAssist)
			m_pSphericalCoordAssist->SetReferenceOpacity(0.0);

		if (m_pCartesianCoordAssist)
			m_pCartesianCoordAssist->SetReferenceOpacity(0.0);

		m_pLevelTextField->disable();
		m_pOpacitySlider->disable();
	}

	if (m_pSphericalCoordAssist)
		m_pSphericalCoordAssist->UpdateReference();
	
	if (m_pCartesianCoordAssist)
		m_pCartesianCoordAssist->UpdateReference();

	// Render
	m_pScreen->Render();

	return 1;
}

long IFXDAFFFrame::onCmdProbe(FXObject* sender, FXSelector, void*) {
	if (sender == m_pProbeFirstSlider){
		m_pProbeFirstTextField->setText(FXStringVal((double)m_pProbeFirstSlider->getValue()));
	} else if (sender == m_pProbeFirstTextField) {
		double val = FXDoubleVal(m_pProbeFirstTextField->getText());
		m_pProbeFirstSlider->setValue(val);
	}

	if (sender == m_pProbeSecondSlider){
		m_pProbeSecondTextField->setText(FXStringVal((double)m_pProbeSecondSlider->getValue()));
	} else if (sender == m_pProbeSecondTextField) {
		double val = FXDoubleVal(m_pProbeSecondTextField->getText());
		m_pProbeSecondSlider->setValue(val);
	}

	if (m_pBalloonPlotNode) {
		m_pBalloonPlotNode->SetProbeAngles(m_pProbeFirstSlider->getValue(), m_pProbeSecondSlider->getValue());
		m_pBalloonPlotNode->SetProbeVisible(m_pProbeCheckButton->getCheck() ? true : false);
		m_pBalloonPlotNode->UpdateProbe();
	}

	if (m_pCarpetPlotNode) {
		m_pCarpetPlotNode->SetProbeAngles(m_pProbeFirstSlider->getValue(), m_pProbeSecondSlider->getValue());
		m_pCarpetPlotNode->SetProbeVisible(m_pProbeCheckButton->getCheck() ? true : false);
		m_pCarpetPlotNode->UpdateProbe();
	}

	m_pScreen->Render();

	return 0;
}
/*
   +------------------+
   |                  |
   |  Other methods   |
   |                  |
   +------------------+
*/
/*
void IFXDAFFFrame::updatePlot() {
	// file open?
	if (m_pReader==0)
		return;
	
	// reset gui
	m_pFrequencySlider->disable();
	m_pFrequencyIndicator->disable();
	m_pMaxTextField->disable();
	m_pMinTextField->disable();
	m_pWarpCheckButton->disable();
	m_pRResTextField->disable();
	m_pPhiResTextField->disable();
	m_pLinearRadioButton->disable();
	m_pCarpetPlotRadioButton->disable();
	m_pBalloonPlotRadioButton->disable();
	m_pLogRadioButton->disable();
	m_pChannelList->clearItems();
	m_pChannelList->disable();
	m_pGridCheckButton->disable();
	m_pScaleCheckButton->disable();
	m_pNSPoleCheckButton->disable();
	m_pVectorsCheckButton->disable();
	m_pColorCheckButton->disable();
	m_pWireCheckButton->disable();
	m_pFixedAlphaRadioButton->disable();
	m_pFixedBetaRadioButton->disable();
	m_pLevelTextField->disable();
	m_pOpacityIndicator->disable();
	m_pOpacitySlider->disable();
	m_pPhaseColorCheckButton->disable();
	m_pProbeCheckButton->disable();
	m_pProbeFirstTextField->disable();
	m_pProbeSecondTextField->disable();
	m_pProbeFirstSlider->disable();
	m_pProbeSecondSlider->disable();
	
	m_pWarpCheckButton->setCheck();
	m_pColorCheckButton->setCheck();
	m_pWireCheckButton->setCheck(false); // TODO?
	m_pPhaseColorCheckButton->setCheck(false); // TODO?
	m_pProbeCheckButton->setCheck(false);

	// Remove old plot child nodes
	if (m_pBalloonPlotNode) {
		m_pRootNode->RemoveChildNode(m_pBalloonPlotNode);
		delete m_pBalloonPlotNode;
		m_pBalloonPlotNode=0;
	}
	if (m_pCarpetPlotNode){
		m_pRootNode->RemoveChildNode(m_pCarpetPlotNode);
		delete m_pCarpetPlotNode;
		m_pCarpetPlotNode = 0;
	}
	if (m_pSphereCoordAssist) m_pRootNode->RemoveChildNode(m_pSphereCoordAssist);
	if (m_pKartesianCoordAssist) m_pRootNode->RemoveChildNode(m_pKartesianCoordAssist);
	m_vFrequencies.clear();
	//...
	
	const DAFFProperties* props = m_pReader->getProperties();

	// create new plot node and reset coordinate system
	if (m_pContentIR != NULL) {
		if (m_pCarpetPlotRadioButton->getCheck()) { // no transform
			m_pCarpetPlotNode = new FXVTK2::CarpetPlot( m_pContentIR );
			m_pRootNode->AddChildNode(m_pCarpetPlotNode);
			m_pRootNode->AddChildNode(m_pKartesianCoordAssist);
			m_pKartesianCoordAssist->SetPosition(-1, 0, -1);
			m_pKartesianCoordAssist->SetAxesLength(2, 1, 2);
			m_pKartesianCoordAssist->setMinZ(0);
			m_pKartesianCoordAssist->setMaxZ(m_pContentIR->getFilterLength());
			m_pKartesianCoordAssist->setResolutionZ(10);
			m_pKartesianCoordAssist->setGridVisible(m_pGridCheckButton->getCheck() ? true : false);
			m_pCarpetPlotNode->SetSelectedAngle( 90 );
		} else if (m_pBalloonPlotRadioButton->getCheck() ? true : false) { // transform to directivity
			DAFFTransformerIR2DFT* transformer = new DAFFTransformerIR2DFT(m_pContentIR);
			m_pContentDFT = transformer->getOutputContent();
			m_pBalloonPlotNode = new FXVTK2::BalloonPlot( m_pContentDFT );
			m_pRootNode->AddChildNode(m_pBalloonPlotNode);
			m_pRootNode->AddChildNode(m_pSphereCoordAssist);
			m_pBalloonPlotNode->SetSelectedFrequency(0);
			m_pBalloonPlotNode->SetScale(1.0,1.0,1.0);
			m_pBalloonPlotNode->SetRange(0, m_pContentDFT->getOverallMagnitudeMaximum());
		}
		// enable transformation radio buttons
		m_pCarpetPlotRadioButton->enable();
		m_pBalloonPlotRadioButton->enable();
	} else if (m_pContentMS != NULL) {
		m_pBalloonPlotNode = new FXVTK2::BalloonPlot( m_pContentMS );
		m_pRootNode->AddChildNode(m_pBalloonPlotNode);
		m_pRootNode->AddChildNode(m_pSphereCoordAssist);
		m_pBalloonPlotNode->SetSelectedFrequency(0);
		m_pBalloonPlotNode->SetScale(1.0,1.0,1.0);
		m_pBalloonPlotNode->SetRange(0, m_pContentMS->getOverallMagnitudeMaximum());
	} else if (m_pContentDFT != NULL) {
		m_pBalloonPlotNode = new FXVTK2::BalloonPlot( m_pContentDFT );
		m_pRootNode->AddChildNode(m_pBalloonPlotNode);
		m_pRootNode->AddChildNode(m_pSphereCoordAssist);
		m_pBalloonPlotNode->SetSelectedFrequency(0);
		m_pBalloonPlotNode->SetScale(1.0,1.0,1.0);
		m_pBalloonPlotNode->SetRange(0, m_pContentDFT->getOverallMagnitudeMaximum());
	}

	
	// update GUI
	if (m_pCarpetPlotNode) {
		// Update Range TextFields and checkboxes
		m_pMaxTextField->setText(FXStringVal(m_pCarpetPlotNode->GetRangeMax()));
		m_pMinTextField->setText(FXStringVal(m_pCarpetPlotNode->GetRangeMin()));
		m_pFixedAlphaRadioButton->enable();
		m_pFixedBetaRadioButton->enable();
		
		//m_pFixedBetaRadioButton->setCheck(1);
		if (m_pFixedBetaRadioButton->getCheck()) {
			m_pCarpetPlotNode->setFixedAngle(FXVTK2::CarpetPlot::BETA_FIXED);
			m_pKartesianCoordAssist->setMinX(m_pContentIR->getProperties()->getAlphaStart());
			m_pKartesianCoordAssist->setMaxX(m_pContentIR->getProperties()->getAlphaEnd());
			m_pKartesianCoordAssist->setResolutionX(20);
			m_pFrequencySlider->setRange(0, m_pContentIR->getProperties()->getBetaPoints()-1);
			m_pFrequencySlider->setValue(m_pContentIR->getProperties()->getBetaPoints()/2);
			m_pSliderGroupBox->setText(m_sBetaAngleText);
			m_pProbeFirstSlider->setRange(m_pContentIR->getProperties()->getAlphaStart(), m_pContentIR->getProperties()->getAlphaEnd());
			m_pProbeFirstSlider->setIncrement(m_pContentIR->getProperties()->getAlphaResolution());
			m_pProbeFirstLabel->setText(m_sAlphaAngleText);
		} else {
			m_pCarpetPlotNode->setFixedAngle(FXVTK2::CarpetPlot::ALPHA_FIXED);
			m_pKartesianCoordAssist->setMinX(m_pContentIR->getProperties()->getBetaStart());
			m_pKartesianCoordAssist->setMaxX(m_pContentIR->getProperties()->getBetaEnd());
			m_pKartesianCoordAssist->setResolutionX(10);
			m_pFrequencySlider->setRange(0, m_pContentIR->getProperties()->getAlphaPoints()-1);
			m_pFrequencySlider->setValue(m_pContentIR->getProperties()->getAlphaPoints()/2);
			m_pSliderGroupBox->setText(m_sAlphaAngleText);
			m_pProbeFirstSlider->setRange(m_pContentIR->getProperties()->getBetaStart(), m_pContentIR->getProperties()->getBetaEnd());
			m_pProbeFirstSlider->setIncrement(m_pContentIR->getProperties()->getBetaResolution());	
			m_pProbeFirstLabel->setText(m_sBetaAngleText);
		}
		m_pProbeSecondSlider->setRange(0, m_pContentIR->getFilterLength()-1);
		m_pProbeSecondSlider->setIncrement(1);
		m_pProbeSecondLabel->setText(m_sSampleText);

		for (int i=0; i<m_pContentIR->getProperties()->getBetaPoints(); i++) 
			m_vFrequencies.push_back(m_pContentIR->getProperties()->getBetaStart()+ i*m_pContentIR->getProperties()->getBetaResolution());
		
		m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + "°" );

		if (m_pLinearRadioButton->getCheck()) 
			m_pCarpetPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_LINEAR);
		else
			m_pCarpetPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_DECIBEL);
		
		m_pProbeCheckButton->enable();
		m_pProbeFirstTextField->enable();
		m_pProbeSecondTextField->enable();
		m_pProbeFirstSlider->enable();
		m_pProbeSecondSlider->enable();
	}

	if (m_pBalloonPlotNode) {
		// do content specific stuff first
		if (m_pContentMS) {
			m_pBalloonPlotNode->SetRange(0.0, m_pContentMS->getOverallMagnitudeMaximum());
			m_vFrequencies = m_pContentMS->getFrequencies();
			m_pFrequencySlider->setRange(0, m_pContentMS->getNumFrequencies()-1);
			m_pProbeFirstSlider->setRange(m_pContentMS->getProperties()->getAlphaStart(), m_pContentMS->getProperties()->getAlphaEnd());
			m_pProbeSecondSlider->setRange(m_pContentMS->getProperties()->getBetaStart(), m_pContentMS->getProperties()->getBetaEnd());
			m_pProbeFirstSlider->setIncrement(m_pContentMS->getProperties()->getAlphaResolution());
			m_pProbeSecondSlider->setIncrement(m_pContentMS->getProperties()->getBetaResolution());
		} else if (m_pContentDFT) {
			m_pBalloonPlotNode->SetRange(0.0, m_pContentDFT->getOverallMagnitudeMaximum());
			for (int i=0; i<m_pContentDFT->getNumDFTCoeffs(); i++)
				m_vFrequencies.push_back( i*m_pContentDFT->getFrequencyBandwidth() );
			m_pFrequencySlider->setRange(0, m_pContentDFT->getNumDFTCoeffs()-1);
			m_pProbeFirstSlider->setRange(m_pContentDFT->getProperties()->getAlphaStart(), m_pContentDFT->getProperties()->getAlphaEnd());
			m_pProbeSecondSlider->setRange(m_pContentDFT->getProperties()->getBetaStart(), m_pContentDFT->getProperties()->getBetaEnd());
			m_pProbeFirstSlider->setIncrement(m_pContentDFT->getProperties()->getAlphaResolution());
			m_pProbeSecondSlider->setIncrement(m_pContentDFT->getProperties()->getBetaResolution());
			m_pPhaseColorCheckButton->enable();

		}
		m_pSliderGroupBox->setText(m_sFrequencyText);
		m_pFrequencySlider->setValue(0);
		m_pFrequencyIndicator->setText( FXStringFormat("%.0f", m_vFrequencies[ m_pFrequencySlider->getValue() ]) + " Hz" );
		
		m_pMaxTextField->setText(FXStringVal(m_pBalloonPlotNode->GetRangeMax()));
		m_pMinTextField->setText(FXStringVal(m_pBalloonPlotNode->GetRangeMin()));
		
		// update scaling
		if (m_pLinearRadioButton->getCheck()) 
			m_pBalloonPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_LINEAR);
		else
			m_pBalloonPlotNode->SetScaling(FXVTK2::BalloonPlot::SCALING_DECIBEL);
		
		m_pProbeFirstLabel->setText(m_sAlphaAngleText);
		m_pProbeSecondLabel->setText(m_sBetaAngleText);
		m_pNSPoleCheckButton->enable();
		m_pVectorsCheckButton->enable();
		m_pRResTextField->enable();
		m_pProbeCheckButton->enable();
		m_pProbeFirstTextField->enable();
		m_pProbeSecondTextField->enable();
		m_pProbeFirstSlider->enable();
		m_pProbeSecondSlider->enable();
	}
	
		
	// finally do all plot-node independant stuff


	// update textfields and checkboxes
	m_pWarpCheckButton->enable();
	m_pRResTextField->enable();
	m_pPhiResTextField->enable();
	m_pResetButton->enable();
	m_pGridCheckButton->enable();
	m_pScaleCheckButton->enable();
	m_pColorCheckButton->enable();
	m_pWireCheckButton->enable();
	m_pLinearRadioButton->enable();
	m_pLogRadioButton->enable();
	m_pMaxTextField->enable();
	m_pMinTextField->enable();
	m_pLevelTextField->enable();
	m_pLevelSlider->enable();
	m_pOpacitySlider->enable();

	// Adjust frequency slider
	m_pFrequencySlider->enable();
	
	/* Not in status bar but maybe somewhere else?
	m_pTxtStatusBox1->setText(FXStringFormat("%d", props->getNumberOfChannels()) + " Channel(s)");
	m_pTxtStatusBox2->setText("Resolution (A, B): " + FXStringFormat("%.0f°", props->getBetaResolution()) + ", " + FXStringFormat("%.0f°", props->getBetaResolution()));
	m_pTxtStatusBox3->setText(DAFFUtils::StrQuantizationType(props->getQuantization()).c_str());
		
	// update channel list
	for (int i=1; i <= props->getNumberOfChannels(); i++) {
		if( props->getChannelLabel(i-1).length() == 0) {
			m_pChannelList->appendItem("Channel " + FXStringFormat("%d", i));
		} else {
			m_pChannelList->appendItem("Ch. " + FXStringFormat("%d", i) + " - " + props->getChannelLabel(i-1).c_str());
		}
	}
	m_pChannelList->setNumVisible(props->getNumberOfChannels());
	m_pChannelList->enable();

	// trigger range, probe und warp reset
	onCmdRange(m_pResetButton, 0, 0);
	onCmdReference(m_pReferenceCheckButton, 0, 0);
	onCmdWarp(m_pWarpCheckButton, 0, 0);
	}
	*/

/* OLD LOADER
*  We do it like to following:
*
*  - Try to open the new file, but preserve the old one
*  - If the reading was successful, we discard the old one
*  - If it failed, we stick to the old one
*
void IFXDAFFFrame::load(const std::string& sFilename, bool bQuiet) {

	DAFFReader* pNewReader = DAFFReader::create();
	int iError = pNewReader->openFile(sFilename);
	
	if (iError != 0) {
		delete pNewReader;
		if(!bQuiet) FXMessageBox::error(this, MBOX_OK, "Error", DAFFUtils::StrError(iError).c_str());
	} else if (pNewReader->getContent()->getProperties()->getNumberOfRecords() <= 1) {
		delete pNewReader;
		if(!bQuiet) FXMessageBox::error(this, MBOX_OK, "Error", "The choosen file contains only one or zero records.");
	} else {
		// Discard the old data
		delete m_pReader;
		m_pReader = pNewReader;

		setTitle(APPNAME + FXStringFormat(" [%s]", sFilename.c_str()));
		statusbar->getStatusLine()->setText("Wow!");

		if (pNewReader->getContentType() == DAFF_IMPULSE_RESPONSE) {
			m_pContentIR = dynamic_cast<DAFFContentIR*>( m_pReader->getContent() );
			m_pContentMS = NULL;
			m_pContentDFT = NULL;
		}
		if (pNewReader->getContentType() == DAFF_MAGNITUDE_SPECTRUM) {
			m_pContentIR = NULL;
			m_pContentMS = dynamic_cast<DAFFContentMS*>( m_pReader->getContent() );
			m_pContentDFT = NULL;
		}

		if (pNewReader->getContentType() == DAFF_DFT_SPECTRUM) {
			m_pContentIR = NULL;
			m_pContentDFT = dynamic_cast<DAFFContentDFT*>( m_pReader->getContent() );
			m_pContentMS = NULL;
		}

		updatePlot();
	}
}
*/