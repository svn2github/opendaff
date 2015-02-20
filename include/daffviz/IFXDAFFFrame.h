/* ---------------------------------------------------------------------------
 *
 *							IFX - ITA FOX Extensions
 *            (c) Copyright Institute of Technical Acoustics (ITA)
 *                  RWTH University, Aachen, Germany, 2008-today
 *
 * ---------------------------------------------------------------------------
 *
 *    File:			IFXDAFFFrame.h
 *
 *    Purpose:		Frame for interactive exploration of DAFF objects
 *
 *    Authors:		Jonas Stienen (stienen@akustik.rwth-aachen.de)
 *    Contributors:	Hendrik Vorwerk (hendri.vorwerk@akustik.rwth-aachen.de)
 *
 *  ---------------------------------------------------------------------------
 */
// $Id: IFXDAFFFrame.h,v 1.2 2006-05-21 14:26:44 stienen Exp $

#ifndef __IFXDAFFFRAME_H__
#define __IFXDAFFFRAME_H__

// Fox include
#include <fx.h>

// STL includes
#include <string>
#include <vector>

#include <FXVTK2.h>

#include <daffviz/FXVTK2BalloonPlot.h>
#include <daffviz/FXVTK2CarpetPlot.h>

// DAFF interface forward declaration
class DAFFReader;

/* class: IFXDAFFFrame 
 *
 * The IFXDAFFFrame class creates a frame that displays various information
 * of a DAFF content. It as well provides an interactive 3D plot of the
 * data using balloon and carpet plots.
 *
 */
class IFXDAFFFrame : public FXHorizontalFrame {
	FXDECLARE(IFXDAFFFrame)
public:
	//! FOX identifiers
	enum {
		ID_TAB = FXDialogBox::ID_LAST,
		ID_SCREENSHOT,

		ID_FREQUENCY,
		ID_CHANNEL,

		ID_OPTION,
		ID_RADIO,
		ID_DATA_SCALING,
		ID_FIXED_ANGLE,
		ID_PLOT_TYPE,
		ID_GRID,
		ID_SCALE,
		ID_VECTORS,
		ID_NSPOLE,

		ID_NORMALIZE,
		ID_WARP,
		ID_COLOR,
		ID_WIRE,
		ID_RANGE,

		ID_REFERENCE,
		ID_PROBE,

		ID_LAST
    };

	//! Constructor
	IFXDAFFFrame(FXComposite* p, DAFFReader* pDAFF=NULL, FXuint opts=0, FXint x=0, FXint y=0, FXint w=0, FXint h=0, FXint pl=DEFAULT_PAD, FXint pr=DEFAULT_PAD, FXint pt=DEFAULT_PAD, FXint pb=DEFAULT_PAD);


	// --= DAFF methods =--

	void setDAFFReader(DAFFReader* pReader);


	// --= Event handler =--

	//! Controls behavior if frequency is changed
	long onCmdFrequency(FXObject*, FXSelector, void*);

	//! Controls behavior of the channel is changed (e.g. left channel, right channel)
	long onCmdChannels(FXObject*, FXSelector, void*);

	//! Controls the behavior if the plot grid is changed (show, hide)
	long onCmdGrid(FXObject*, FXSelector, void*);

	//! Controls the behavior if vectors are changed (show, hide)
	long onCmdVectors(FXObject*, FXSelector, void*);

	//! Controls the behavior if north and south poles are changed (show, hide)
	long onCmdNSPole(FXObject*, FXSelector, void*);

	//! Controls the behavior if warping should be used (in caret plot only)
	long onCmdWarp(FXObject*, FXSelector, void*);

	//! Controls the behavior if colors are changed (on, off)
	long onCmdColor(FXObject*, FXSelector, void*);

	//! Controls the behavior if display mode changes (wire frame, surface)
	long onCmdWire(FXObject*, FXSelector, void*);

	//! Controls the behavior if the scaling is changed (linear, decibel)
	//long onCmdScale(FXObject*, FXSelector, void*);
	long onCmdScaling(FXObject*, FXSelector, void*);

	//! Controls the behavior if range is changed (show, hide)
	long onCmdRange(FXObject*, FXSelector, void*);

	//! Controls the behavior if the fixed angle is changed (theta, phi)
	long onCmdFixedAngle(FXObject*, FXSelector, void*);

	//! Controls the behavior if plot type is changed (balloon, carpet)
	long onCmdPlotType(FXObject*, FXSelector, void*);

	//! Controls the behavior if a screen shot should be taken (save to disk)
	long onCmdSreenshot(FXObject* sender, FXSelector, void*);

	//! Controls the behavior if reference value or display is changed (show, hide, new value)
	long onCmdReference(FXObject* sender, FXSelector, void*);

	//! Controls the behavior if the probe indicator is changed (show, hide, move)
	long onCmdProbe(FXObject* sender, FXSelector, void*);


	// --= Others methods =--
	
protected:
	//! Constructor 
	IFXDAFFFrame() {};

	//! Destructor
	~IFXDAFFFrame();

private:
	//! DAFF Reader pointer
	DAFFReader* m_pReader;


	// -- = Tab frames =--

	FXHorizontalFrame* m_pFrameVisualization;
	FXHorizontalFrame* m_pFrameProperties;
	FXHorizontalFrame* m_pFrameMetadata;


	// --= 3D visualization =--

	FXVTK2::Frame* m_pScreen;
	FXVTK2::SGNode* m_pRootNode;
	FXVTK2::SphericalCoordinateAssistant* m_pSphericalCoordAssist;
	FXVTK2::CartesianCoordinateAssistant* m_pCartesianCoordAssist;
	FXVTK2::BalloonPlot* m_pBalloonPlotNode;
	FXVTK2::CarpetPlot* m_pCarpetPlotNode;


	// --= Controls =--

	FXVerticalFrame* m_pControllerSidebar;
	FXGroupBox* m_pSliderGroupBox;
	FXSlider* m_pFrequencySlider;
	FXTextField* m_pFrequencyIndicator;

	FXListBox* m_pChannelList;
	FXCheckButton* m_pScaleCheckButton;
	FXCheckButton* m_pGridCheckButton;
	FXCheckButton* m_pVectorsCheckButton;
	FXCheckButton* m_pNSPoleCheckButton;
	FXCheckButton* m_pWarpCheckButton;
	FXCheckButton* m_pColorCheckButton;
	FXCheckButton* m_pPhaseColorCheckButton;
	FXCheckButton* m_pWireCheckButton;
	FXCheckButton* m_pReferenceCheckButton;
	FXCheckButton* m_pProbeCheckButton;

	FXRadioButton* m_pDataLinearRadioButton;
	FXRadioButton* m_pDataLogRadioButton;
	FXRadioButton* m_pFixedAlphaRadioButton;
	FXRadioButton* m_pFixedBetaRadioButton;
	FXRadioButton* m_pBalloonPlotRadioButton;
	FXRadioButton* m_pCarpetPlotRadioButton;

	FXLabel* m_pMinLabel;
	FXLabel* m_pMaxLabel;
	FXLabel* m_pRResLabel;
	FXLabel* m_pPhiResLabel;
	FXLabel* m_pOpacityLabel;
	FXLabel* m_pLevelLabel;
	FXLabel* m_pProbeFirstLabel;
	FXLabel* m_pProbeSecondLabel;

	// TODO: Do we need private strings here?
	FXString m_sBalloonLinResText;
	FXString m_sBalloonLogResText;
	FXString m_sBalloonPhiResText;
	FXString m_sCarpetBetaResText;
	FXString m_sCarpetAlphaResText;
	FXString m_sCarpetYLinResText;
	FXString m_sCarpetYLogResText;
	FXString m_sLevelLinText;
	FXString m_sLevelLogText;
	FXString m_sMinLinText;
	FXString m_sMaxLinText;
	FXString m_sMinLogText;
	FXString m_sMaxLogText;
	FXString m_sFrequencyText;
	FXString m_sAlphaAngleText;
	FXString m_sBetaAngleText;
	FXString m_sSampleText;

	FXTextField* m_pMinTextField;
	FXTextField* m_pMaxTextField;
	FXTextField* m_pRResTextField;
	FXTextField* m_pPhiResTextField;

	FXButton* m_pResetButton;

	FXSlider* m_pOpacitySlider;
	FXSlider* m_pLevelSlider;
	FXSlider* m_pProbeFirstSlider;
	FXSlider* m_pProbeSecondSlider;
	FXTextField* m_pOpacityIndicator;
	FXTextField* m_pProbeFirstTextField;
	FXTextField* m_pLevelTextField;
	FXTextField* m_pProbeSecondTextField;
	
	std::vector<float> m_vFrequencies;
	

	// --= GUI arrangement helper =--

	//! rearranges the whole DAFF frame
	void rearrange();

	//! Arranges the base structure
	void arrange_base();

	//! Arranges a DAFF Magnitude Spectrum specific frame
	void arrangeMS();

	//! Arranges a DAFF Impulse Response specific frame
	void arrangeIR();

	//! Arranges a DAFF Discrete Fourier Spectrum specific frame
	void arrangeDFT();
};

#endif // __IFXDAFFFRAME_H__