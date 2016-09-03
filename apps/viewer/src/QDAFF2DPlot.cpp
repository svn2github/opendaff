#include "QDAFF2DPlot.h"

#include <DAFF.h>



QDAFF2DPlot::QDAFF2DPlot( QWidget *parent )
		: QGraphicsView( new QGraphicsScene(), parent )
		, m_pReader( NULL )
{
	//scrollbars
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
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
	//setScene( new QGraphicsScene() );
	scale(1, -1);
	setRenderHint(QPainter::Antialiasing);
}

void QDAFF2DPlot::ReadDAFF( const DAFFReader* pReader )
{
	m_iRecordIndex = 0;
	m_iFrequencyIndex = 0;
	m_iChannelIndex = 0;

	m_pReader = pReader;	
	Draw();
	ExportImage("C:\\Users\\Nigel\\Pictures\\Saved Pictures\\test1.svg",4.0, true ,true, false);
}

void QDAFF2DPlot::CloseDAFF()
{
	m_pReader = NULL;
}

void QDAFF2DPlot::ChangeRecordIndex( int iRecordIndex )
{
	m_iRecordIndex = iRecordIndex;
	Draw();
}

void QDAFF2DPlot::ChangeChannelIndex(int iChannelIndex)
{
	m_iChannelIndex = iChannelIndex;
	Draw();
}

void QDAFF2DPlot::ChangeFrequencyIndex( int iFrequencyIndex )
{
    m_iFrequencyIndex = iFrequencyIndex;
    Draw();
}

void QDAFF2DPlot::Draw(bool showAllChannels, bool showDots)
{
	if (m_iSceneHeight == 0 || m_iSceneWidth == 0 || m_pReader == nullptr)
		return;

	//clear and draw background
	scene()->clear();
	scene()->setSceneRect(0, 0, m_iSceneWidth, m_iSceneHeight);
	m_iXAxisLength = m_iSceneWidth - m_iTipDistanceX - m_iTipLength - m_iAxisOffsetRight - m_iAxisOffsetLeft;
	m_iYAxisLength = m_iSceneHeight - m_iTipDistanceY - m_iTipLength - m_iAxisOffsetUp - m_iAxisOffsetDown;
	//scene()->addRect(0, 0, m_iSceneWidth, m_iSceneHeight, QPen(), QBrush(Qt::white));
	setBackgroundBrush(QBrush(Qt::white));

	//draw plot
	DrawCoordinateSystem();
	DrawGraph(m_iRecordIndex);
	ShowChannel(m_iChannelIndex, showAllChannels, showDots);
}

void QDAFF2DPlot::DrawCoordinateSystem()
{
	if (m_pReader == nullptr)
		return;

	m_vpXMarker.resize(m_iNXMarker);
	m_vpXMarkerText.resize(m_iNXMarker);
	m_vpYMarker.resize(m_iNYMarker);
	m_vpYMarkerText.resize(m_iNYMarker);
	switch (m_pReader->getContentType())
	{
	case DAFF_MAGNITUDE_SPECTRUM:
	{
		//fixed logarithmic scale, 28 from 20 to 20k Hz
		m_vpXMarker.resize(28);
		//show 20,40,60,100,200,400,600,1000,...
		m_vpXMarkerText.resize(13);
		//Draw Axis
		m_pXAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iXAxisLength + m_iAxisOffsetLeft, m_iAxisOffsetDown));
		m_pYAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iAxisOffsetLeft, m_iYAxisLength + m_iAxisOffsetDown));
		//Draw Grid
		m_vpYGrid.resize(m_iNYGrid);
		double offset = (double)m_iYAxisLength / m_iNYGrid;
		for (int i = 0; i < m_iNYGrid; i++)
		{
			m_vpYGrid[i] = scene()->addLine(QLine(m_pXAxis->line().x1(), m_pXAxis->line().y1() + (i + 1)*offset, m_pXAxis->line().x1() + m_iXAxisLength, m_pXAxis->line().y2() + (i + 1)*offset), QPen(Qt::lightGray));
		}
		//logarithmic x scale, 27 lines from 30 to 20k Hz
		m_vpXGrid.resize(27);
		double logRange = std::log(20000) - std::log(20);
		//20 Hz marker
		m_vpXMarker[0] = scene()->addLine(QLine(m_pXAxis->line().x1(), m_pXAxis->line().y1(), m_pXAxis->line().x1(), m_pXAxis->line().y1() - m_iXMarkerLength));
		//30 to 90 Hz
		for (int i = 0; i < 7; i++)
		{
			offset = m_iXAxisLength*(std::log((i + 3) * 10) - std::log(20)) / logRange;
			m_vpXGrid[i] = scene()->addLine(QLine(m_pYAxis->line().x1() + offset, m_pYAxis->line().y1(), m_pYAxis->line().x1() + offset, m_pYAxis->line().y1() + m_iYAxisLength), QPen(Qt::lightGray));
			m_vpXMarker[i + 1] = scene()->addLine(QLine(m_pYAxis->line().x1() + offset, m_pYAxis->line().y1(), m_pYAxis->line().x1() + offset, m_pYAxis->line().y1() - m_iXMarkerLength));
		}
		//100 to 900 Hz
		for (int i = 7; i < 16; i++)
		{
			offset = m_iXAxisLength*(std::log((i - 6) * 100) - std::log(20)) / logRange;
			m_vpXGrid[i] = scene()->addLine(QLine(m_pYAxis->line().x1() + offset, m_pYAxis->line().y1(), m_pYAxis->line().x1() + offset, m_pYAxis->line().y1() + m_iYAxisLength), QPen(Qt::lightGray));
			m_vpXMarker[i + 1] = scene()->addLine(QLine(m_pYAxis->line().x1() + offset, m_pYAxis->line().y1(), m_pYAxis->line().x1() + offset, m_pYAxis->line().y1() - m_iXMarkerLength));
		}
		//1k to 9k Hz
		for (int i = 16; i < 25; i++)
		{
			offset = m_iXAxisLength*(std::log((i - 15) * 1000) - std::log(20)) / logRange;
			m_vpXGrid[i] = scene()->addLine(QLine(m_pYAxis->line().x1() + offset, m_pYAxis->line().y1(), m_pYAxis->line().x1() + offset, m_pYAxis->line().y1() + m_iYAxisLength), QPen(Qt::lightGray));
			m_vpXMarker[i + 1] = scene()->addLine(QLine(m_pYAxis->line().x1() + offset, m_pYAxis->line().y1(), m_pYAxis->line().x1() + offset, m_pYAxis->line().y1() - m_iXMarkerLength));
		}
		//10k and 20k Hz
		for (int i = 25; i < 27; i++)
		{
			offset = m_iXAxisLength*(std::log((i - 24) * 10000) - std::log(20)) / logRange;
			m_vpXGrid[i] = scene()->addLine(QLine(m_pYAxis->line().x1() + offset, m_pYAxis->line().y1(), m_pYAxis->line().x1() + offset, m_pYAxis->line().y1() + m_iYAxisLength), QPen(Qt::lightGray));
			m_vpXMarker[i + 1] = scene()->addLine(QLine(m_pYAxis->line().x1() + offset, m_pYAxis->line().y1(), m_pYAxis->line().x1() + offset, m_pYAxis->line().y1() - m_iXMarkerLength));
		}
		//marker 20
		m_vpXMarkerText[0] = scene()->addText(QString("20"));
		m_vpXMarkerText[0]->setPos(m_vpXMarker[0]->line().x2() - m_vpXMarkerText[0]->boundingRect().width()/2, m_vpXMarker[0]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[0]->setTransform(QTransform().scale(1, -1));
		//marker 40
		m_vpXMarkerText[1] = scene()->addText(QString("40"));
		m_vpXMarkerText[1]->setPos(m_vpXMarker[2]->line().x2() - m_vpXMarkerText[1]->boundingRect().width() / 2, m_vpXMarker[2]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[1]->setTransform(QTransform().scale(1, -1));
		//marker 60
		m_vpXMarkerText[2] = scene()->addText(QString("60"));
		m_vpXMarkerText[2]->setPos(m_vpXMarker[4]->line().x2() - m_vpXMarkerText[2]->boundingRect().width() / 2, m_vpXMarker[4]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[2]->setTransform(QTransform().scale(1, -1));
		//marker 100
		m_vpXMarkerText[3] = scene()->addText(QString("100"));
		m_vpXMarkerText[3]->setPos(m_vpXMarker[8]->line().x2() - m_vpXMarkerText[3]->boundingRect().width() / 2, m_vpXMarker[8]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[3]->setTransform(QTransform().scale(1, -1));
		//marker 200
		m_vpXMarkerText[4] = scene()->addText(QString("200"));
		m_vpXMarkerText[4]->setPos(m_vpXMarker[9]->line().x2() - m_vpXMarkerText[4]->boundingRect().width() / 2, m_vpXMarker[9]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[4]->setTransform(QTransform().scale(1, -1));
		//marker 400
		m_vpXMarkerText[5] = scene()->addText(QString("400"));
		m_vpXMarkerText[5]->setPos(m_vpXMarker[11]->line().x2() - m_vpXMarkerText[5]->boundingRect().width() / 2, m_vpXMarker[11]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[5]->setTransform(QTransform().scale(1, -1));
		//marker 600
		m_vpXMarkerText[6] = scene()->addText(QString("600"));
		m_vpXMarkerText[6]->setPos(m_vpXMarker[13]->line().x2() - m_vpXMarkerText[6]->boundingRect().width() / 2, m_vpXMarker[13]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[6]->setTransform(QTransform().scale(1, -1));
		//marker 1k
		m_vpXMarkerText[7] = scene()->addText(QString("1k"));
		m_vpXMarkerText[7]->setPos(m_vpXMarker[17]->line().x2() - m_vpXMarkerText[7]->boundingRect().width() / 2, m_vpXMarker[17]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[7]->setTransform(QTransform().scale(1, -1));
		//marker 2k
		m_vpXMarkerText[8] = scene()->addText(QString("2k"));
		m_vpXMarkerText[8]->setPos(m_vpXMarker[18]->line().x2() - m_vpXMarkerText[8]->boundingRect().width() / 2, m_vpXMarker[18]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[8]->setTransform(QTransform().scale(1, -1));
		//marker 4k
		m_vpXMarkerText[9] = scene()->addText(QString("4k"));
		m_vpXMarkerText[9]->setPos(m_vpXMarker[20]->line().x2() - m_vpXMarkerText[9]->boundingRect().width() / 2, m_vpXMarker[20]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[9]->setTransform(QTransform().scale(1, -1));
		//marker 6k
		m_vpXMarkerText[10] = scene()->addText(QString("6k"));
		m_vpXMarkerText[10]->setPos(m_vpXMarker[22]->line().x2() - m_vpXMarkerText[10]->boundingRect().width() / 2, m_vpXMarker[22]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[10]->setTransform(QTransform().scale(1, -1));
		//marker 10k
		m_vpXMarkerText[11] = scene()->addText(QString("10k"));
		m_vpXMarkerText[11]->setPos(m_vpXMarker[26]->line().x2() - m_vpXMarkerText[11]->boundingRect().width() / 2, m_vpXMarker[26]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[11]->setTransform(QTransform().scale(1, -1));
		//marker 20k
		m_vpXMarkerText[12] = scene()->addText(QString("20k"));
		m_vpXMarkerText[12]->setPos(m_vpXMarker[27]->line().x2() - m_vpXMarkerText[12]->boundingRect().width() / 2, m_vpXMarker[27]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[12]->setTransform(QTransform().scale(1, -1));		
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
		//1st Y Marker
		m_vpYMarker[0] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1(), m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1()));
		m_vpYMarkerText[0] = scene()->addText(QString("0"));
		m_vpYMarkerText[0]->setPos(m_vpYMarker[0]->line().x2() - m_vpYMarkerText[0]->boundingRect().width() - m_iYMarkerTextOffset, m_vpYMarker[0]->line().y2() + m_vpYMarkerText[0]->boundingRect().height() / 2);
		m_vpYMarkerText[0]->setTransform(QTransform().scale(1, -1));		
		//other Y Markers
		offset = (double)m_iYAxisLength / (m_iNYMarker-1);
		for (int i = 1; i < m_iNYMarker; i++)
		{
			m_vpYMarker[i] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1() + i*offset, m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1() + i*offset));
			m_vpYMarkerText[i] = scene()->addText(QString(convertFloat((i / (float)(m_iNYMarker - 1))*pContent->getOverallMagnitudeMaximum()).c_str()));
			m_vpYMarkerText[i]->setPos(m_vpYMarker[i]->line().x2() - m_vpYMarkerText[i]->boundingRect().width() - m_iYMarkerTextOffset, m_vpYMarker[i]->line().y2() + m_vpYMarkerText[i]->boundingRect().height() / 2);
			m_vpYMarkerText[i]->setTransform(QTransform().scale(1, -1));
		}	
		break;
	}
	case DAFF_IMPULSE_RESPONSE:
		{
			//Draw Axis
			m_pXAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iXAxisLength + m_iAxisOffsetLeft, m_iAxisOffsetDown));
			m_pYAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iAxisOffsetLeft, m_iYAxisLength + m_iAxisOffsetDown));
			//Draw Grid
			m_vpXGrid.resize(m_iNXGrid);
			m_vpYGrid.resize(m_iNYGrid);
			double offset = (double)m_iYAxisLength / m_iNYGrid;
			for (int i = 0; i < m_vpYGrid.size(); i++)
			{
				m_vpYGrid[i] = scene()->addLine(QLine(m_pXAxis->line().x1(), m_pXAxis->line().y1() + (i + 1)*offset, m_pXAxis->line().x1() + m_iXAxisLength, m_pXAxis->line().y2() + (i + 1)*offset), QPen(Qt::lightGray));
			}
			offset = (double)m_iXAxisLength / m_iNXGrid;
			for (int i = 0; i < m_vpXGrid.size(); i++)
			{
				m_vpXGrid[i] = scene()->addLine(QLine(m_pXAxis->line().x1() + (i + 1)*offset, m_pXAxis->line().y1(), m_pXAxis->line().x1() + (i + 1)*offset, m_pXAxis->line().y2() + m_iYAxisLength), QPen(Qt::lightGray));
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
			m_vpXMarkerText[0]->setPos(m_vpXMarker[0]->line().x2() - m_vpXMarkerText[0]->boundingRect().width() / 2, m_vpXMarker[0]->line().y2() - m_iXMarkerTextOffset);
			m_vpXMarkerText[0]->setTransform(QTransform().scale(1, -1));
			//1st Y Marker
			m_vpYMarker[0] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1(), m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1()));
			m_vpYMarkerText[0] = scene()->addText(QString("-1"));
			m_vpYMarkerText[0]->setPos(m_vpYMarker[0]->line().x2() - m_vpYMarkerText[0]->boundingRect().width() - m_iYMarkerTextOffset, m_vpYMarker[0]->line().y2() + m_vpYMarkerText[0]->boundingRect().height() / 2);
			m_vpYMarkerText[0]->setTransform(QTransform().scale(1, -1));
			//other X Markers
			offset = (double)m_iXAxisLength / (m_iNXMarker - 1);
			float indexValueRatio = 1000000 / (pContent->getSamplerate());
			float markerValueRatio = ((pContent->getFilterLength()-1)*indexValueRatio) / (m_iNXMarker-1);
			for (int i = 1; i < m_iNXMarker; i++)
			{
				m_vpXMarker[i] = scene()->addLine(QLine(m_pXAxis->line().x1() + offset*i, m_pXAxis->line().y1(), m_pXAxis->line().x1() + offset*i, m_pXAxis->line().y1() - m_iXMarkerLength));
				m_vpXMarkerText[i] = scene()->addText(QString(std::to_string((int)(markerValueRatio*i)).c_str()));
				m_vpXMarkerText[i]->setPos(m_vpXMarker[i]->line().x2() - m_vpXMarkerText[i]->boundingRect().width()/2, m_vpXMarker[i]->line().y2() - m_iXMarkerTextOffset);
				m_vpXMarkerText[i]->setTransform(QTransform().scale(1, -1));
			}
			//other Y Markers
			offset = (double)m_iYAxisLength / (m_iNYMarker - 1);
			for (int i = 1; i < m_iNYMarker; i++)
			{
				m_vpYMarker[i] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1() + i*offset, m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1() + i*offset));
				m_vpYMarkerText[i] = scene()->addText(QString(convertFloat(((i / (float)(m_iNYMarker - 1))*2)-1).c_str()));
				m_vpYMarkerText[i]->setPos(m_vpYMarker[i]->line().x2() - m_vpYMarkerText[i]->boundingRect().width() - m_iYMarkerTextOffset, m_vpYMarker[i]->line().y2() + m_vpYMarkerText[i]->boundingRect().height()/2);
				m_vpYMarkerText[i]->setTransform(QTransform().scale(1, -1));
			}
			break;
		}
	case DAFF_DFT_SPECTRUM:
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
		yText = scene()->addText(QString("A"));
		yText->setPos(tipY[1]);
		yText->setTransform(QTransform().scale(1, -1));
		//Draw Marker
		DAFFContentDFT* pContent = dynamic_cast< DAFFContentDFT* >(m_pReader->getContent());
		//1st X Marker
		m_vpXMarker[0] = scene()->addLine(QLine(m_pXAxis->line().x1(), m_pXAxis->line().y1(), m_pXAxis->line().x1(), m_pXAxis->line().y1() - m_iXMarkerLength));
		m_vpXMarkerText[0] = scene()->addText(QString("0"));
		m_vpXMarkerText[0]->setPos(m_vpXMarker[0]->line().x2(), m_vpXMarker[0]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[0]->setTransform(QTransform().scale(1, -1));
		//1st Y Marker
		m_vpYMarker[0] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1(), m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1()));
		m_vpYMarkerText[0] = scene()->addText(QString("0"));
		m_vpYMarkerText[0]->setPos(m_vpYMarker[0]->line().x2() - m_iYMarkerTextOffset, m_vpYMarker[0]->line().y2());
		m_vpYMarkerText[0]->setTransform(QTransform().scale(1, -1));
		//other X Markers
		float markerPixelRatio = m_iXAxisLength / (m_iNXMarker - 1);
		float a = pContent->getSamplerate();
		float b = pContent->getNumDFTCoeffs();
		float c = pContent->getTransformSize();
		float indexValueRatio = pContent->getFrequencyBandwidth();
		float markerValueRatio = (pContent->getNumDFTCoeffs()*indexValueRatio) / (m_iNXMarker - 1);
		for (int i = 1; i < m_iNXMarker; i++)
		{
			m_vpXMarker[i] = scene()->addLine(QLine(m_pXAxis->line().x1() + markerPixelRatio*i, m_pXAxis->line().y1(), m_pXAxis->line().x1() + markerPixelRatio*i, m_pXAxis->line().y1() - m_iXMarkerLength));
			//m_vpXMarkerText[i] = scene()->addText(QString(convertFloat(((i / (float)(markerValueRatio*i)) * 2) - 1).c_str()));
			m_vpXMarkerText[i] = scene()->addText(QString(convertFloat(i*markerValueRatio).c_str()));
			m_vpXMarkerText[i]->setPos(m_vpXMarker[i]->line().x2(), m_vpXMarker[i]->line().y2() - m_iXMarkerTextOffset);
			m_vpXMarkerText[i]->setTransform(QTransform().scale(1, -1));
		}
		//other Y Markers
		float indexPixelRatio = m_iYAxisLength / (m_iNYMarker - 1);
		indexValueRatio = pContent->getOverallMagnitudeMaximum() / (m_iNYMarker - 1);
		for (int i = 1; i < m_iNYMarker; i++)
		{
			m_vpYMarker[i] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1() + i*indexPixelRatio, m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1() + i*indexPixelRatio));
			m_vpYMarkerText[i] = scene()->addText(QString(convertFloat(i*indexValueRatio).c_str()));
			m_vpYMarkerText[i]->setPos(m_vpYMarker[i]->line().x2() - m_iYMarkerTextOffset, m_vpYMarker[i]->line().y2());
			m_vpYMarkerText[i]->setTransform(QTransform().scale(1, -1));
		}
		break;
	}
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
		float mag1;
		QString text;
		m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(pContent->getProperties()->getNumberOfChannels());
		m_vvpPoints = std::vector<std::vector<QGraphicsPoint*>>(pContent->getProperties()->getNumberOfChannels());
		if (pContent->getNumFrequencies() > 1)
		{
			double logRange = std::log(20000) - std::log(20);
			double offset;
			for (int i = 0; i < m_vvpPoints.size(); i++)
			{
				m_vvpGraphs[i] = std::vector<QGraphicsLineItem*>(pContent->getNumFrequencies() - 1);
				m_vvpPoints[i] = std::vector<QGraphicsPoint*>(pContent->getNumFrequencies());
				for (int j = 0; j < m_vvpPoints[i].size(); j++)
				{
					pContent->getMagnitude(recordIndex, i, j, mag1);
					offset = m_iXAxisLength*(std::log(pContent->getFrequencies()[j]) - std::log(20)) / logRange;
					text = QString("Magnitude: ").append(QString::number(mag1)).append("\nFrequency: ").append(QString::number(pContent->getFrequencies()[j]));
					m_vvpPoints[i][j] = new QGraphicsPoint(text, m_pXAxis->line().x1() + offset, m_pXAxis->line().y1() + mag1*amplitudePixelRatio, m_iPointDiameter);
					scene()->addItem(m_vvpPoints[i][j]);
				}
				for (int j = 0; j < m_vvpGraphs[i].size(); j++)
				{
					m_vvpGraphs[i][j] = scene()->addLine(QLine(m_vvpPoints[i][j]->pos().x(), m_vvpPoints[i][j]->pos().y(), m_vvpPoints[i][j + 1]->pos().x(), m_vvpPoints[i][j+1]->pos().y()), QPen(m_voColors[i % 10]));
				}
				
			}
			//for (int i = 0; i < m_vvpGraphs.size(); i++)
			//{
			//	pContent->getMagnitude(recordIndex, i, 0, mag1);
			//	m_vvpGraphs[i] = std::vector<QGraphicsLineItem*>(pContent->getNumFrequencies() - 1);
			//	m_vvpPoints[i] = std::vector<QGraphicsPoint*>(pContent->getNumFrequencies());
			//	float indexPixelRatio = m_iXAxisLength / (pContent->getNumFrequencies() - 1);
			//	//1st point
			//	text = QString("Magnitude: ").append(QString::number(mag1)).append("\nFrequency: ").append(QString::number(pContent->getFrequencies()[0]));
			//	box = new QInfoBox(text);
			//	m_vvpPoints[i][0] = new QGraphicsPoint(box, m_pXAxis->line().x1(), m_pXAxis->line().y1() + mag1*amplitudePixelRatio, m_iPointDiameter);
			//	//other points
			//	for (int j = 0; j < m_vvpGraphs[i].size(); j++)
			//	{
			//		pContent->getMagnitude(recordIndex, i, j, mag1);
			//		pContent->getMagnitude(recordIndex, i, j + 1, mag2);
			//		m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + j*indexPixelRatio, m_pXAxis->line().y1() + mag1*amplitudePixelRatio, m_pXAxis->line().x1() + (j + 1)*indexPixelRatio, m_pXAxis->line().y1() + mag2*amplitudePixelRatio), QPen(m_voColors[i%10]));
			//		//"Magnitude: "+mag1 + "\n Frequency: " + pContent->getFrequencies()[j];
			//		text = QString("Magnitude: ").append(QString::number(mag2)).append("\nFrequency: ").append(QString::number(pContent->getFrequencies()[j+1]));
			//		box = new QInfoBox(text);
			//		m_vvpPoints[i][j + 1] = new QGraphicsPoint(box, m_pXAxis->line().x1() + (j + 1)*indexPixelRatio, m_pXAxis->line().y1() + mag2*amplitudePixelRatio, m_iPointDiameter);
			//	}
			//	for (int j = 0; j < m_vvpPoints[i].size(); j++)
			//	{
			//		scene()->addItem(m_vvpPoints[i][j]);
			//	}
			//}
		}
		break;
	}
	case DAFF_IMPULSE_RESPONSE:
	{
		DAFFContentIR* pContent = dynamic_cast< DAFFContentIR* >(m_pReader->getContent());
		std::vector<float> coeffs = std::vector<float>(pContent->getFilterLength());
		float amplitudePixelRatio = m_iYAxisLength / 2;
		float indexValueRatio = 1000000 / (pContent->getSamplerate());
		double offset = (double)m_iXAxisLength / (pContent->getFilterLength() - 1);
		QString text;
		m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(pContent->getProperties()->getNumberOfChannels());
		m_vvpPoints = std::vector<std::vector<QGraphicsPoint*>>(pContent->getProperties()->getNumberOfChannels());
		for (int i = 0; i < m_vvpGraphs.size(); i++)
		{
			pContent->getFilterCoeffs(recordIndex, i, &coeffs[0]);
			m_vvpGraphs[i] = std::vector<QGraphicsLineItem*>(pContent->getFilterLength() - 1);
			m_vvpPoints[i] = std::vector<QGraphicsPoint*>(pContent->getFilterLength());
			//for (int j = 0; j < m_vvpPoints[i].size(); j++)
			//{
			//	text = QString("Amplitude: ").append(QString::number(coeffs[j])).append("\nSample Time: ").append(QString::number(j*indexValueRatio));
			//	m_vvpPoints[i][j] = new QGraphicsPoint(text, m_pXAxis->line().x1() + j*offset, m_pXAxis->line().y1() + (coeffs[j] + 1) * amplitudePixelRatio, m_iPointDiameter);
			//}
			//for (int j = 0; j < m_vvpGraphs[i].size(); j++)
			//{
			//	m_vvpGraphs[i][j] = scene()->addLine(QLine(m_vvpPoints[i][j]->pos().x(), m_vvpPoints[i][j]->pos().y(), m_vvpPoints[i][j + 1]->pos().x(), m_vvpPoints[i][j + 1]->pos().y()), QPen(m_voColors[i % 10]));
			//}
			
			for (int j = 0; j < m_vvpGraphs[i].size(); j++)
			{
				m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + j*offset, m_pXAxis->line().y1() + (coeffs[j] + 1) * amplitudePixelRatio, m_pXAxis->line().x1() + (j + 1)*offset, m_pXAxis->line().y1() + (coeffs[j + 1] + 1) * amplitudePixelRatio), QPen(m_voColors[i % 10]));
			}
			if (m_vvpPoints[i].size()*(m_iPointDiameter + 3) < m_iXAxisLength)
			{
				for (int j = 0; j < m_vvpGraphs[i].size(); j++)
				{
					text = QString("Amplitude: ").append(QString::number(coeffs[j])).append("\nSample Time: ").append(QString::number((j)*indexValueRatio));
					m_vvpPoints[i][j] = new QGraphicsPoint(text, m_vvpGraphs[i][j]->line().x1(), m_vvpGraphs[i][j]->line().y1(), m_iPointDiameter);
				}
				size_t j = m_vvpGraphs[i].size();
				text = QString("Amplitude: ").append(QString::number(coeffs[j])).append("\nSample Time: ").append(QString::number((j)*indexValueRatio));
				m_vvpPoints[i][j] = new QGraphicsPoint(text, m_vvpGraphs[i][j-1]->line().x2(), m_vvpGraphs[i][j-1]->line().y2(), m_iPointDiameter);
				for (int j = 0; j < m_vvpPoints[i].size(); j++)
				{
					scene()->addItem(m_vvpPoints[i][j]);
				}
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

void QDAFF2DPlot::ExportImage(QString filePath, float factor, bool svg, bool showAllChannels, bool showDots){
	m_iSceneWidth *= factor;
	m_iSceneHeight *= factor;
	scene()->setSceneRect(0, 0, m_iSceneWidth, m_iSceneHeight);
	Draw(showAllChannels, showDots);
	if (svg)
	{
		QSvgGenerator generator;
		generator.setFileName(filePath);
		generator.setSize(QSize(m_iSceneWidth, m_iSceneHeight));
		QPainter p;
		p.begin(&generator);
		scene()->render(&p);
		p.end();
	}
	else
	{
		QImage img(m_iSceneWidth, m_iSceneHeight, QImage::Format_ARGB32_Premultiplied);
		QPainter p(&img);
		scene()->render(&p);
		p.end();
		img.mirrored(false, true).save(filePath);
	}
	m_iSceneWidth /= factor;
	m_iSceneHeight /= factor;
	Draw();
}

void QDAFF2DPlot::ShowChannel(int iChannelIndex, bool bShowAllChannels, bool showDots)
{
	if (m_pReader->getContentType() == DAFF_DFT_SPECTRUM)
		return;

	if (bShowAllChannels)
	{
		for (int i = 0; i < m_vvpGraphs.size(); i++)
		{			
			for (int j = 0; j < m_vvpGraphs[i].size(); j++)
			{
				m_vvpGraphs[i][j]->show();
			}			
		}
		for (int i = 0; i < m_vvpPoints.size(); i++)
		{			
			for (int j = 0; j < m_vvpPoints[i].size(); j++)
			{
				if (m_vvpPoints[i][j] != nullptr && showDots)
					m_vvpPoints[i][j]->show();
				else if (m_vvpPoints[i][j] != nullptr && !showDots)
					m_vvpPoints[i][j]->hide();
			}
		}
	}
	else
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
		for (int i = 0; i < m_vvpPoints.size(); i++)
		{
			if (i == iChannelIndex)
			{
				for (int j = 0; j < m_vvpPoints[i].size(); j++)
				{
					if (m_vvpPoints[i][j] != nullptr&& showDots)
						m_vvpPoints[i][j]->show();
					else if (m_vvpPoints[i][j] != nullptr && !showDots)
						m_vvpPoints[i][j]->hide();
				}
			}
			else
			{
				for (int j = 0; j < m_vvpPoints[i].size(); j++)
				{
					if (m_vvpPoints[i][j] != nullptr)
						m_vvpPoints[i][j]->hide();
				}
			}
		}
	}
	
}

void QDAFF2DPlot::keyReleaseEvent(QKeyEvent * event)
{
	switch (event->key())
	{
	case 16777249: //Ctrl Key
	{
		m_bScrollHorizontally = true;
	}
	}
	event->accept();
}

void QDAFF2DPlot::keyPressEvent(QKeyEvent * event)
{
	int key = event->key();
    switch( key )
	{
	case 16777249: //Ctrl Key
        {
            m_bScrollHorizontally = false;
        }
	}
	event->accept();
}

void QDAFF2DPlot::resizeEvent(QResizeEvent *event)
{
	//fitInView(0, 0, event->oldSize().width(), event->oldSize().height(), Qt::KeepAspectRatio);
	fitInView(0, 0, m_iSceneOriginalWidth, m_iSceneOriginalHeight, Qt::KeepAspectRatio);
	QGraphicsView::resizeEvent(event);
}

void QDAFF2DPlot::showEvent(QShowEvent * event)
{
	m_iSceneHeight = height();
	m_iSceneWidth = width();
	m_iSceneOriginalHeight = height();
	m_iSceneOriginalWidth = width();
	QGraphicsView::showEvent(event);
}

void QDAFF2DPlot::wheelEvent(QWheelEvent * event)
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;
	if (event->orientation() == Qt::Vertical) {
		if (m_bScrollHorizontally)
		{
			m_iSceneWidth *= std::pow(1.1, numSteps);
			scene()->setSceneRect(0, 0, m_iSceneWidth, m_iSceneHeight);
			Draw();
		}
		else
		{
			m_iSceneHeight *= std::pow(1.1, numSteps);
			scene()->setSceneRect(0, 0, m_iSceneWidth, m_iSceneHeight);
			Draw();
		}		
		event->accept();
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


