#include "QDAFF2DPlot.h"

#include <DAFF.h>

QDAFF2DPlot::QDAFF2DPlot( QWidget *parent )
		: QGraphicsView( new QGraphicsScene(), parent )
		, m_pReader( NULL )
{
	//color vector
	m_voColors = std::vector<QColor>(10);
	m_voColors[0] = Qt::red;
	m_voColors[1] = Qt::green;
	m_voColors[2] = Qt::blue;
	m_voColors[3] = Qt::cyan;
	m_voColors[4] = Qt::magenta;
	m_voColors[5] = Qt::yellow;
	m_voColors[6] = Qt::darkRed;
	m_voColors[7] = Qt::darkGreen;
	m_voColors[8] = Qt::darkBlue;
	m_voColors[9] = Qt::darkCyan;
	//indices
	m_iChannelIndex = 0;
	m_iRecordIndex = 0;
	//scene
	setScene( new QGraphicsScene() );
	scale(1, -1);
}

void QDAFF2DPlot::ReadDAFF( const DAFFReader* pReader )
{
	m_pReader = pReader;
	//clear and draw background
	scene()->clear();
	scene()->setSceneRect(0, 0, m_iSceneWidth, m_iSceneHeight);
	scene()->addRect(0, 0, m_iSceneWidth, m_iSceneHeight, QPen(), QBrush(Qt::white));
	setBackgroundBrush(QBrush(Qt::lightGray));

	//draw plot
	DrawCoordinateSystem();
	DrawGraph(m_iRecordIndex);
	ShowOnlyChannel(m_iChannelIndex);
}

void QDAFF2DPlot::CloseDAFF()
{
	m_pReader = NULL;
}

void QDAFF2DPlot::ChangeRecordIndex( int iRecordIndex )
{
	m_iRecordIndex = iRecordIndex;
	//clear and draw background
	scene()->clear();
	scene()->setSceneRect(0, 0, m_iSceneWidth, m_iSceneHeight);
	scene()->addRect(0, 0, m_iSceneWidth, m_iSceneHeight, QPen(), QBrush(Qt::white));
	setBackgroundBrush(QBrush(Qt::lightGray));

	//draw plot
	DrawCoordinateSystem();
	DrawGraph(m_iRecordIndex);
	ShowOnlyChannel(m_iChannelIndex);
}

void QDAFF2DPlot::ChangeChannelIndex(int iChannelIndex)
{
	m_iChannelIndex = iChannelIndex;
	//clear and draw background
	scene()->clear();
	scene()->setSceneRect(0, 0, m_iSceneWidth, m_iSceneHeight);
	scene()->addRect(0, 0, m_iSceneWidth, m_iSceneHeight, QPen(), QBrush(Qt::white));
	setBackgroundBrush(QBrush(Qt::lightGray));

	//draw plot
	DrawCoordinateSystem();
	DrawGraph(m_iRecordIndex);
	ShowOnlyChannel(m_iChannelIndex);
}

void QDAFF2DPlot::DrawCoordinateSystem()
{
	if (m_pReader == nullptr)
		return;

	m_vpXMarker.resize(m_iNXMarker);
	m_vpYMarker.resize(m_iNYMarker);
	m_vpXMarkerText.resize(m_iNXMarker);
	m_vpYMarkerText.resize(m_iNYMarker);
	switch (m_pReader->getContentType())
	{
	case DAFF_MAGNITUDE_SPECTRUM:
	{
		//Draw Axis
		m_pXAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iXAxisLength + m_iAxisOffsetLeft, m_iAxisOffsetDown));
		m_pYAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iAxisOffsetLeft, m_iYAxisLength + m_iAxisOffsetDown));
		//Draw Grid
		int nXGrid = m_iXAxisLength / m_iGridXOffset;
		int nYGrid = m_iYAxisLength / m_iGridYOffset;
		m_vpXGrid.resize(nXGrid);
		m_vpYGrid.resize(nYGrid);
		for (int i = 0; i < nXGrid; i++)
		{
			m_vpXGrid[i] = scene()->addLine(QLine(m_pYAxis->line().x1() + (i + 1)*m_iGridXOffset, m_pYAxis->line().y1(), m_pYAxis->line().x1() + (i + 1)*m_iGridXOffset, m_pYAxis->line().y1() + m_iYAxisLength), QPen(Qt::lightGray));
		}
		for (int i = 0; i < nYGrid; i++)
		{
			m_vpXGrid[i] = scene()->addLine(QLine(m_pXAxis->line().x1(), m_pXAxis->line().y1() + (i + 1)*m_iGridYOffset, m_pXAxis->line().x1() + m_iXAxisLength, m_pXAxis->line().y2() + (i + 1)*m_iGridYOffset), QPen(Qt::lightGray));
		}
		//Draw Tip
		QGraphicsLineItem *tipLineX, *tipLineY;
		tipLineX = scene()->addLine(QLine(m_pXAxis->line().x2(), m_pXAxis->line().y2(), m_pXAxis->line().x2() + m_iTipDistanceX, m_pXAxis->line().y2()));
		tipLineY = scene()->addLine(QLine(m_pYAxis->line().x2(), m_pYAxis->line().y2(), m_pYAxis->line().x2(), m_pYAxis->line().y2() + m_iTipDistanceX));
		QPolygonF tipX, tipY;
		tipX << QPointF(tipLineX->line().x2(), tipLineX->line().y2()) << QPointF(tipLineX->line().x2(), tipLineX->line().y2() + m_iTipWidth) << QPointF(tipLineX->line().x2() + m_iTipLength, tipLineX->line().y2()) << QPointF(tipLineX->line().x2(), tipLineX->line().y2() - m_iTipWidth);
		tipY << QPointF(tipLineY->line().x2(), tipLineY->line().y2()) << QPointF(tipLineY->line().x2() + m_iTipWidth, tipLineY->line().y2()) << QPointF(tipLineY->line().x2(), tipLineY->line().y2() + m_iTipLength) << QPointF(tipLineY->line().x2() - m_iTipWidth, tipLineY->line().y2());
		scene()->addPolygon(tipX, QPen(), QBrush(Qt::black));
		scene()->addPolygon(tipY, QPen(), QBrush(Qt::black));
		//Draw Axis Title
		QGraphicsTextItem *xText, *yText;
		xText = scene()->addText(QString("f/Hz"));
		xText->setPos(tipX.back());
		xText->setTransform(QTransform().scale(1, -1));
		yText = scene()->addText(QString("M/dB"));
		yText->setPos(tipY[1]);
		yText->setTransform(QTransform().scale(1, -1));
		//Draw Marker
		DAFFContentMS* pContent = dynamic_cast< DAFFContentMS* >(m_pReader->getContent());
		std::vector<float> frequencies = pContent->getFrequencies();
		//1st X Marker
		m_vpXMarker[0] = scene()->addLine(QLine(m_pXAxis->line().x1(), m_pXAxis->line().y1(), m_pXAxis->line().x1(), m_pXAxis->line().y1() - m_iXMarkerLength));
		m_vpXMarkerText[0] = scene()->addText(QString(convertFloat(frequencies[0]).c_str()));
		m_vpXMarkerText[0]->setPos(m_vpXMarker[0]->line().x2(), m_vpXMarker[0]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[0]->setTransform(QTransform().scale(1, -1));
		//1st Y Marker
		m_vpYMarker[0] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1(), m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1()));
		m_vpYMarkerText[0] = scene()->addText(QString("0"));
		m_vpYMarkerText[0]->setPos(m_vpYMarker[0]->line().x2() - m_iYMarkerTextOffset, m_vpYMarker[0]->line().y2());
		m_vpYMarkerText[0]->setTransform(QTransform().scale(1, -1));
		if (frequencies.size() >= m_iNXMarker)
		{
			//other X Markers
			float indexPixelRatio = m_iXAxisLength / (frequencies.size() - 1);
			float indexMarkerRatio = (frequencies.size() - 1) / (m_iNXMarker - 1);
			for (int i = 1; i < m_iNXMarker; i++)
			{
				int index = (int)(i)*indexMarkerRatio;
				m_vpXMarker[i] = scene()->addLine(QLine(m_pXAxis->line().x1() + indexPixelRatio*index, m_pXAxis->line().y1(), m_pXAxis->line().x1() + indexPixelRatio*index, m_pXAxis->line().y1() - m_iXMarkerLength));
				m_vpXMarkerText[i] = scene()->addText(QString(convertFloat(frequencies[index]).c_str()));
				m_vpXMarkerText[i]->setPos(m_vpXMarker[i]->line().x2(), m_vpXMarker[i]->line().y2() - m_iXMarkerTextOffset);
				m_vpXMarkerText[i]->setTransform(QTransform().scale(1, -1));
			}
			//other Y Markers
			indexPixelRatio = m_iYAxisLength / (m_iNYMarker - 1);
			for (int i = 1; i < m_iNYMarker; i++)
			{
				m_vpYMarker[i] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1() + i*indexPixelRatio, m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1() + i*indexPixelRatio));
				m_vpYMarkerText[i] = scene()->addText(QString(convertFloat((i / (float)(m_iNYMarker - 1))*pContent->getOverallMagnitudeMaximum()).c_str()));
				m_vpYMarkerText[i]->setPos(m_vpYMarker[i]->line().x2() - m_iYMarkerTextOffset, m_vpYMarker[i]->line().y2());
				m_vpYMarkerText[i]->setTransform(QTransform().scale(1, -1));
			}
		}
		else if (frequencies.size() > 1)
		{
			//other X Markers
			float indexPixelRatio = m_iXAxisLength / (frequencies.size() - 1);
			float indexMarkerRatio = 1;
			for (int i = 1; i < m_iNXMarker; i++)
			{
				int index = (int)(i)*indexMarkerRatio;
				m_vpXMarker[i] = scene()->addLine(QLine(m_pXAxis->line().x1() + indexPixelRatio*index, m_pXAxis->line().y1(), m_pXAxis->line().x1() + indexPixelRatio*index, m_pXAxis->line().y1() - m_iXMarkerLength));
				m_vpXMarkerText[i] = scene()->addText(QString(convertFloat(frequencies[index]).c_str()));
				m_vpXMarkerText[i]->setPos(m_vpXMarker[i]->line().x2(), m_vpXMarker[i]->line().y2() - m_iXMarkerTextOffset);
				m_vpXMarkerText[i]->setTransform(QTransform().scale(1, -1));
			}
			//other Y Markers
			indexPixelRatio = m_iYAxisLength / (m_iNYMarker - 1);
			for (int i = 1; i < m_iNYMarker; i++)
			{
				m_vpYMarker[i] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1() + i*indexPixelRatio, m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1() + i*indexPixelRatio));
				m_vpYMarkerText[i] = scene()->addText(QString(convertFloat((i / (float)(m_iNYMarker - 1))*pContent->getOverallMagnitudeMaximum()).c_str()));
				m_vpYMarkerText[i]->setPos(m_vpYMarker[i]->line().x2() - m_iYMarkerTextOffset, m_vpYMarker[i]->line().y2());
				m_vpYMarkerText[i]->setTransform(QTransform().scale(1, -1));
			}
		}		
		break;
	}
	case DAFF_IMPULSE_RESPONSE:
		{
			//Draw Axis
			m_pXAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iXAxisLength + m_iAxisOffsetLeft, m_iAxisOffsetDown));
			m_pYAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iAxisOffsetLeft, m_iYAxisLength + m_iAxisOffsetDown));
			//Draw Grid
			int nXGrid = m_iXAxisLength / m_iGridXOffset;
			int nYGrid = m_iYAxisLength / m_iGridYOffset;
			m_vpXGrid.resize(nXGrid);
			m_vpYGrid.resize(nYGrid);
			for (int i = 0; i < nXGrid; i++)
			{
				m_vpXGrid[i] = scene()->addLine(QLine(m_pYAxis->line().x1() + (i + 1)*m_iGridXOffset, m_pYAxis->line().y1(), m_pYAxis->line().x1() + (i + 1)*m_iGridXOffset, m_pYAxis->line().y1() + m_iYAxisLength), QPen(Qt::lightGray));
			}
			for (int i = 0; i < nYGrid; i++)
			{
				m_vpXGrid[i] = scene()->addLine(QLine(m_pXAxis->line().x1(), m_pXAxis->line().y1() + (i + 1)*m_iGridYOffset, m_pXAxis->line().x1() + m_iXAxisLength, m_pXAxis->line().y2() + (i + 1)*m_iGridYOffset), QPen(Qt::lightGray));
			}
			//Draw Tip
			QGraphicsLineItem *tipLineX, *tipLineY;
			tipLineX = scene()->addLine(QLine(m_pXAxis->line().x2(), m_pXAxis->line().y2(), m_pXAxis->line().x2() + m_iTipDistanceX, m_pXAxis->line().y2()));
			tipLineY = scene()->addLine(QLine(m_pYAxis->line().x2(), m_pYAxis->line().y2(), m_pYAxis->line().x2(), m_pYAxis->line().y2() + m_iTipDistanceX));
			QPolygonF tipX, tipY;
			tipX << QPointF(tipLineX->line().x2(), tipLineX->line().y2()) << QPointF(tipLineX->line().x2(), tipLineX->line().y2() + m_iTipWidth) << QPointF(tipLineX->line().x2() + m_iTipLength, tipLineX->line().y2()) << QPointF(tipLineX->line().x2(), tipLineX->line().y2() - m_iTipWidth);
			tipY << QPointF(tipLineY->line().x2(), tipLineY->line().y2()) << QPointF(tipLineY->line().x2() + m_iTipWidth, tipLineY->line().y2()) << QPointF(tipLineY->line().x2(), tipLineY->line().y2() + m_iTipLength) << QPointF(tipLineY->line().x2() - m_iTipWidth, tipLineY->line().y2());
			scene()->addPolygon(tipX, QPen(), QBrush(Qt::black));
			scene()->addPolygon(tipY, QPen(), QBrush(Qt::black));
			//Draw Axis Title
			QGraphicsTextItem *xText, *yText;
			xText = scene()->addText(QString("S/us"));
			xText->setPos(tipX.back());
			xText->setTransform(QTransform().scale(1, -1));
			yText = scene()->addText(QString("A"));
			yText->setPos(tipY[1]);
			yText->setTransform(QTransform().scale(1, -1));
			//Draw Marker
			DAFFContentIR* pContent = dynamic_cast< DAFFContentIR* >(m_pReader->getContent());
			//1st X Marker
			m_vpXMarker[0] = scene()->addLine(QLine(m_pXAxis->line().x1(), m_pXAxis->line().y1(), m_pXAxis->line().x1(), m_pXAxis->line().y1() - m_iXMarkerLength));
			m_vpXMarkerText[0] = scene()->addText(QString("0"));
			m_vpXMarkerText[0]->setPos(m_vpXMarker[0]->line().x2(), m_vpXMarker[0]->line().y2() - m_iXMarkerTextOffset);
			m_vpXMarkerText[0]->setTransform(QTransform().scale(1, -1));
			//1st Y Marker
			m_vpYMarker[0] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1(), m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1()));
			m_vpYMarkerText[0] = scene()->addText(QString("-1"));
			m_vpYMarkerText[0]->setPos(m_vpYMarker[0]->line().x2() - m_iYMarkerTextOffset, m_vpYMarker[0]->line().y2());
			m_vpYMarkerText[0]->setTransform(QTransform().scale(1, -1));
			//other X Markers
			float markerPixelRatio = m_iXAxisLength / (m_iNXMarker - 1);
			float indexValueRatio = 1000000 / (pContent->getSamplerate());
			float markerValueRatio = (pContent->getFilterLength()*indexValueRatio) / (m_iNXMarker-1);
			for (int i = 1; i < m_iNXMarker; i++)
			{
				m_vpXMarker[i] = scene()->addLine(QLine(m_pXAxis->line().x1() + markerPixelRatio*i, m_pXAxis->line().y1(), m_pXAxis->line().x1() + markerPixelRatio*i, m_pXAxis->line().y1() - m_iXMarkerLength));
				m_vpXMarkerText[i] = scene()->addText(QString(std::to_string((int)(markerValueRatio*i)).c_str()));
				m_vpXMarkerText[i]->setPos(m_vpXMarker[i]->line().x2(), m_vpXMarker[i]->line().y2() - m_iXMarkerTextOffset);
				m_vpXMarkerText[i]->setTransform(QTransform().scale(1, -1));
			}
			//other Y Markers
			float indexPixelRatio = m_iYAxisLength / (m_iNYMarker - 1);
			for (int i = 1; i < m_iNYMarker; i++)
			{
				m_vpYMarker[i] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1() + i*indexPixelRatio, m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1() + i*indexPixelRatio));
				m_vpYMarkerText[i] = scene()->addText(QString(convertFloat(((i / (float)(m_iNYMarker - 1))*2)-1).c_str()));
				m_vpYMarkerText[i]->setPos(m_vpYMarker[i]->line().x2() - m_iYMarkerTextOffset, m_vpYMarker[i]->line().y2());
				m_vpYMarkerText[i]->setTransform(QTransform().scale(1, -1));
			}
			break;
		}
	//case DAFF_DFT_SPECTRUM:
	//{
	//	//Draw Axis
	//	m_pXAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iXAxisLength + m_iAxisOffsetLeft, m_iAxisOffsetDown));
	//	m_pYAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iAxisOffsetLeft, m_iYAxisLength + m_iAxisOffsetDown));
	//	//Draw Grid
	//	int nXGrid = m_iXAxisLength / m_iGridXOffset;
	//	int nYGrid = m_iYAxisLength / m_iGridYOffset;
	//	m_vpXGrid.resize(nXGrid);
	//	m_vpYGrid.resize(nYGrid);
	//	for (int i = 0; i < nXGrid; i++)
	//	{
	//		  m_vpXGrid[i] = scene()->addLine(QLine(m_pYAxis->line().x1() + (i + 1)*m_iGridXOffset, m_pYAxis->line().y1(), m_pYAxis->line().x1() + (i + 1)*m_iGridXOffset, m_pYAxis->line().y1() + m_iYAxisLength), QPen(Qt::lightGray));
	//	}
	//	for (int i = 0; i < nYGrid; i++)
	//	{
	//		  m_vpXGrid[i] = scene()->addLine(QLine(m_pXAxis->line().x1(), m_pXAxis->line().y1() + (i + 1)*m_iGridYOffset, m_pXAxis->line().x1() + m_iXAxisLength, m_pXAxis->line().y2() + (i + 1)*m_iGridYOffset), QPen(Qt::lightGray));
	//	}
	//	//Draw Tip
	//	QGraphicsLineItem *tipLineX, *tipLineY;
	//	tipLineX = scene()->addLine(QLine(m_pXAxis->line().x2(), m_pXAxis->line().y2(), m_pXAxis->line().x2() + m_iTipDistanceX, m_pXAxis->line().y2()));
	//	tipLineY = scene()->addLine(QLine(m_pYAxis->line().x2(), m_pYAxis->line().y2(), m_pYAxis->line().x2(), m_pYAxis->line().y2() + m_iTipDistanceX));
	//	QPolygonF tipX, tipY;
	//	tipX << QPointF(tipLineX->line().x2(), tipLineX->line().y2()) << QPointF(tipLineX->line().x2(), tipLineX->line().y2() + m_iTipWidth) << QPointF(tipLineX->line().x2() + m_iTipLength, tipLineX->line().y2()) << QPointF(tipLineX->line().x2(), tipLineX->line().y2() - m_iTipWidth);
	//	tipY << QPointF(tipLineY->line().x2(), tipLineY->line().y2()) << QPointF(tipLineY->line().x2() + m_iTipWidth, tipLineY->line().y2()) << QPointF(tipLineY->line().x2(), tipLineY->line().y2() + m_iTipLength) << QPointF(tipLineY->line().x2() - m_iTipWidth, tipLineY->line().y2());
	//	scene()->addPolygon(tipX, QPen(), QBrush(Qt::black));
	//	scene()->addPolygon(tipY, QPen(), QBrush(Qt::black));
	//	//Draw Axis Title
	//	QGraphicsTextItem *xText, *yText;
	//	xText = scene()->addText(QString("f/Hz"));
	//	xText->setPos(tipX.back());
	//	xText->setTransform(QTransform().scale(1, -1));
	//	yText = scene()->addText(QString("A"));
	//	yText->setPos(tipY[1]);
	//	yText->setTransform(QTransform().scale(1, -1));
	//	//Draw Marker
	//	DAFFContentDFT* pContent = dynamic_cast< DAFFContentDFT* >(m_pReader->getContent());
	//	//1st X Marker
	//	m_vpXMarker[0] = scene()->addLine(QLine(m_pXAxis->line().x1(), m_pXAxis->line().y1(), m_pXAxis->line().x1(), m_pXAxis->line().y1() - m_iXMarkerLength));
	//	m_vpXMarkerText[0] = scene()->addText(QString("0"));
	//	m_vpXMarkerText[0]->setPos(m_vpXMarker[0]->line().x2(), m_vpXMarker[0]->line().y2() - m_iXMarkerTextOffset);
	//	m_vpXMarkerText[0]->setTransform(QTransform().scale(1, -1));
	//	//1st Y Marker
	//	m_vpYMarker[0] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1(), m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1()));
	//	m_vpYMarkerText[0] = scene()->addText(QString("0"));
	//	m_vpYMarkerText[0]->setPos(m_vpYMarker[0]->line().x2() - m_iYMarkerTextOffset, m_vpYMarker[0]->line().y2());
	//	m_vpYMarkerText[0]->setTransform(QTransform().scale(1, -1));
	//	//other X Markers
	//	float markerPixelRatio = m_iXAxisLength / (m_iNXMarker - 1);
	//	float indexValueRatio = pContent->getFrequencyBandwidth();
	//	float markerValueRatio = (pContent->getNumDFTCoeffs()*indexValueRatio) / (m_iNXMarker - 1);
	//	for (int i = 1; i < m_iNXMarker; i++)
	//	{
	//		m_vpXMarker[i] = scene()->addLine(QLine(m_pXAxis->line().x1() + markerPixelRatio*i, m_pXAxis->line().y1(), m_pXAxis->line().x1() + markerPixelRatio*i, m_pXAxis->line().y1() - m_iXMarkerLength));
	//		m_vpXMarkerText[i] = scene()->addText(QString(convertFloat(((i / (float)(markerValueRatio*i)) * 2) - 1).c_str()));
	//		m_vpXMarkerText[i]->setPos(m_vpXMarker[i]->line().x2(), m_vpXMarker[i]->line().y2() - m_iXMarkerTextOffset);
	//		m_vpXMarkerText[i]->setTransform(QTransform().scale(1, -1));
	//	}
	//	//other Y Markers
	//	float indexPixelRatio = m_iYAxisLength / (m_iNYMarker - 1);
	//	indexValueRatio = pContent->getOverallMagnitudeMaximum() / (m_iNYMarker - 1);
	//	for (int i = 1; i < m_iNYMarker; i++)
	//	{
	//		m_vpYMarker[i] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1() + i*indexPixelRatio, m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1() + i*indexPixelRatio));
	//		m_vpYMarkerText[i] = scene()->addText(QString(convertFloat(i*indexValueRatio).c_str()));
	//		m_vpYMarkerText[i]->setPos(m_vpYMarker[i]->line().x2() - m_iYMarkerTextOffset, m_vpYMarker[i]->line().y2());
	//		m_vpYMarkerText[i]->setTransform(QTransform().scale(1, -1));
	//	}
	//	break;
	//}
	}
}



void QDAFF2DPlot::DrawGraph(int recordIndex)
{
	if (m_pReader == nullptr)
		return;

	switch (m_pReader->getContentType())
	{
	case DAFF_MAGNITUDE_SPECTRUM:
	{
		DAFFContentMS* pContent = dynamic_cast< DAFFContentMS* >(m_pReader->getContent());
		float amplitudePixelRatio = m_iYAxisLength / pContent->getOverallMagnitudeMaximum();
		float mag1, mag2;
		m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(pContent->getProperties()->getNumberOfChannels());
		if (pContent->getNumFrequencies() > 1)
		{
			for (int i = 0; i < m_vvpGraphs.size(); i++)
			{
				m_vvpGraphs[i] = std::vector<QGraphicsLineItem*>(pContent->getNumFrequencies() - 1);
				float indexPixelRatio = m_iXAxisLength / (pContent->getNumFrequencies() - 1);
				for (int j = 0; j < m_vvpGraphs[i].size(); j++)
				{
					pContent->getMagnitude(recordIndex, i, j, mag1);
					pContent->getMagnitude(recordIndex, i, j + 1, mag2);
					m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + j*indexPixelRatio, m_pXAxis->line().y1() + mag1*amplitudePixelRatio, m_pXAxis->line().x1() + (j + 1)*indexPixelRatio, m_pXAxis->line().y1() + mag2*amplitudePixelRatio), QPen(m_voColors[i%10]));
				}
			}
		}
		break;
	}
	case DAFF_IMPULSE_RESPONSE:
	{
		DAFFContentIR* pContent = dynamic_cast< DAFFContentIR* >(m_pReader->getContent());
		std::vector<float> coeffs = std::vector<float>(pContent->getFilterLength());
		float amplitudePixelRatio = m_iYAxisLength / 2;
		m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(pContent->getProperties()->getNumberOfChannels());
		for (int i = 0; i < m_vvpGraphs.size(); i++)
		{
			pContent->getFilterCoeffs(recordIndex, i, &coeffs[0]);
			m_vvpGraphs[i] = std::vector<QGraphicsLineItem*>(pContent->getFilterLength() - 1);
			float indexPixelRatio = (float)m_iXAxisLength / (pContent->getFilterLength());
			for (int j = 0; j < m_vvpGraphs[i].size(); j++)
			{
				m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + j*indexPixelRatio, m_pXAxis->line().y1() + (coeffs[j]+1) * amplitudePixelRatio, m_pXAxis->line().x1() + (j + 1)*indexPixelRatio, m_pXAxis->line().y1() + (coeffs[j + 1]+1) * amplitudePixelRatio), QPen(m_voColors[i%10]));
			}
		}
		break;
	}
	/*case DAFF_DFT_SPECTRUM:
	{
		DAFFContentDFT* pContent = dynamic_cast< DAFFContentDFT* >(m_pReader->getContent());
		std::vector<float> coeffs = std::vector<float>(2 * pContent->getNumDFTCoeffs());
		float amplitudePixelRatio = m_iYAxisLength / pContent->getOverallMagnitudeMaximum();
		m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(pContent->getProperties()->getNumberOfChannels());
		for (int i = 0; i < m_vvpGraphs.size(); i++)
		{
			pContent->getDFTCoeffs(recordIndex, i, &coeffs[0]);
			std::vector<float> aCoeffs = std::vector<float>(pContent->getNumDFTCoeffs());
			for (int j = 0; j < aCoeffs.size(); j++)
			{
				aCoeffs[j] = (coeffs[2 * j] * coeffs[2 * j]) + (coeffs[(2 * j) + 1] * coeffs[(2 * j) + 1]);
			}
			m_vvpGraphs[i] = std::vector<QGraphicsLineItem*>(aCoeffs.size() - 1);
			float indexPixelRatio = (float)m_iXAxisLength / aCoeffs.size();
			for (int j = 0; j < m_vvpGraphs[i].size(); j++)
			{
				m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + j*indexPixelRatio, m_pXAxis->line().y1() + (aCoeffs[j]) * amplitudePixelRatio, m_pXAxis->line().x1() + (j + 1)*indexPixelRatio, m_pXAxis->line().y1() + coeffs[j + 1] * amplitudePixelRatio), QPen(m_voColors[i % 10]));
			}
		}
		break;
	}*/
	}

}

void QDAFF2DPlot::ShowOnlyChannel(int iChannelIndex)
{
	for (int i = 0; i < m_vvpGraphs.size(); i++)
	{
		if (i == iChannelIndex)
		{
			for (int j = 0; j < m_vvpGraphs[i].size(); j++)
			{
				m_vvpGraphs[i][j]->show();
			}
		}
		else
		{
			for (int j = 0; j < m_vvpGraphs[i].size(); j++)
			{
				m_vvpGraphs[i][j]->hide();
			}
		}
	}
}

const std::string QDAFF2DPlot::convertFloat(float f)
{
	std::string s;
	s = std::to_string(f);
	s.erase(s.find_last_not_of('0') + 1, std::string::npos);
	if (s.back() == '.')
		s.pop_back();
	return s;
}
