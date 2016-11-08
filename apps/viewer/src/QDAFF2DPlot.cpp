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
	m_voColors = std::vector<QColor>(12);
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
	m_voColors[10] = Qt::darkMagenta;
	m_voColors[11] = Qt::darkYellow;

	m_iChannelIndex = 0;
	m_iRecordIndex = 0;
	m_iZoomIndexX = 0;
	m_iZoomIndexY = 0;

	scale(1, -1);
	setRenderHint(QPainter::Antialiasing);
	connect(horizontalScrollBar(), SIGNAL(sliderReleased()), this, SLOT(HorizontalScrollBarRelease()));

}

void QDAFF2DPlot::ReadDAFF( const DAFFReader* pReader )
{
	m_iRecordIndex = 0;
	m_iFrequencyIndex = 0;
	m_iChannelIndex = 0;

	m_pReader = pReader;	
	Draw();
}

void QDAFF2DPlot::CloseDAFF()
{
	scene()->clear();
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
    //Draw(); 
}

void QDAFF2DPlot::HorizontalScrollBarRelease()
{
	Draw();
}

void QDAFF2DPlot::SetAllChannelsVisible( bool bVisible )
{
	m_bShowAllChannels = bVisible;
	Draw();
}

void QDAFF2DPlot::SetDotsVisible(bool bVisible)
{
	m_bShowDots = bVisible;
	Draw();
}

void QDAFF2DPlot::Draw()
{
	if( height() == 0 || width() == 0 || m_pReader == nullptr )
		return;

	//clear and draw background
	scene()->clear();
	scene()->setSceneRect( 0, 0, width(), height() );
	m_iXAxisLength = std::max( 50, width() - m_iTipDistanceX - m_iTipLength - m_iAxisOffsetRight - m_iAxisOffsetLeft );
	m_iYAxisLength = std::max( 50, height() - m_iTipDistanceY - m_iTipLength - m_iAxisOffsetUp - m_iAxisOffsetDown );
	setBackgroundBrush(QBrush(Qt::white));
	//draw plot
	DrawCoordinateSystem();
	DrawGraph(m_iRecordIndex, m_iChannelIndex, m_bShowAllChannels, m_bShowDots);
}

void QDAFF2DPlot::DrawCoordinateSystem()
{
	if (m_pReader == nullptr)
		return;

	m_vpXMarker.resize((m_iNXMarker*std::pow(2, m_iZoomIndexX)));
	m_vpXMarkerText.resize((m_iNXMarker*std::pow(2, m_iZoomIndexX)));
	m_vpYMarker.resize(m_iNYMarker*std::pow(2, m_iZoomIndexY));
	m_vpYMarkerText.resize(m_iNYMarker*std::pow(2, m_iZoomIndexY));
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
		m_vpYGrid.resize(m_iNYGrid*std::pow(2, m_iZoomIndexY));
		double offset = (double)m_iYAxisLength / (m_iNYGrid*std::pow(2, m_iZoomIndexY));
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
		offset = (double)m_iYAxisLength / ((m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY));
		for (int i = 1; i < (m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY)+1; i++)
		{
			m_vpYMarker[i] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1() + i*offset, m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1() + i*offset));
			m_vpYMarkerText[i] = scene()->addText(QString::number((i / ((m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY)))*pContent->getOverallMagnitudeMaximum(), 10, 2));
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
			m_vpXGrid.resize(m_iNXGrid*std::pow(2, m_iZoomIndexX));
			m_vpYGrid.resize(m_iNYGrid*std::pow(2, m_iZoomIndexY));
			double offset = (double)m_iYAxisLength / (m_iNYGrid*std::pow(2, m_iZoomIndexY));
			for (int i = 0; i < m_vpYGrid.size(); i++)
			{
				m_vpYGrid[i] = scene()->addLine(QLine(m_pXAxis->line().x1(), m_pXAxis->line().y1() + (i + 1)*offset, m_pXAxis->line().x1() + m_iXAxisLength, m_pXAxis->line().y2() + (i + 1)*offset), QPen(Qt::lightGray));
			}
			offset = (double)m_iXAxisLength / (m_iNXGrid*std::pow(2,m_iZoomIndexX));
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
			offset = (double)m_iXAxisLength / ((m_iNXMarker-1)*std::pow(2, m_iZoomIndexX));
			float indexValueRatio = 1000000 / (pContent->getSamplerate());
			float markerValueRatio = ((pContent->getFilterLength() - 1)*indexValueRatio) / ((m_iNXMarker - 1)*std::pow(2, m_iZoomIndexX));
			for (int i = 1; i < (m_iNXMarker-1)*std::pow(2, m_iZoomIndexX)+1; i++)
			{
				m_vpXMarker[i] = scene()->addLine(QLine(m_pXAxis->line().x1() + offset*i, m_pXAxis->line().y1(), m_pXAxis->line().x1() + offset*i, m_pXAxis->line().y1() - m_iXMarkerLength));
				m_vpXMarkerText[i] = scene()->addText(QString::number((int)(markerValueRatio*i)));
				m_vpXMarkerText[i]->setPos(m_vpXMarker[i]->line().x2() - m_vpXMarkerText[i]->boundingRect().width()/2, m_vpXMarker[i]->line().y2() - m_iXMarkerTextOffset);
				m_vpXMarkerText[i]->setTransform(QTransform().scale(1, -1));
			}
			//other Y Markers
			offset = (double)m_iYAxisLength / ((m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY));
			for (int i = 1; i < (m_iNYMarker-1)*std::pow(2, m_iZoomIndexY)+1; i++)
			{
				m_vpYMarker[i] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1() + i*offset, m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1() + i*offset));
				m_vpYMarkerText[i] = scene()->addText(QString::number(((i / (float)((m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY) + 1)) * 2) - 1, 10, 2));
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
		m_vpXGrid.resize(m_iNXGrid*std::pow(2, m_iZoomIndexX));
		m_vpYGrid.resize(m_iNYGrid*std::pow(2, m_iZoomIndexY));
		double offset = (double)m_iYAxisLength / (m_iNYGrid*std::pow(2, m_iZoomIndexY));
		for (int i = 0; i < m_vpYGrid.size(); i++)
		{
			m_vpYGrid[i] = scene()->addLine(QLine(m_pXAxis->line().x1(), m_pXAxis->line().y1() + (i + 1)*offset, m_pXAxis->line().x1() + m_iXAxisLength, m_pXAxis->line().y2() + (i + 1)*offset), QPen(Qt::lightGray));
		}
		offset = (double)m_iXAxisLength / (m_iNXGrid*std::pow(2, m_iZoomIndexX));
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
		m_vpXMarkerText[0]->setPos(m_vpXMarker[0]->line().x2() - m_vpXMarkerText[0]->boundingRect().width() / 2, m_vpXMarker[0]->line().y2() - m_iXMarkerTextOffset);
		m_vpXMarkerText[0]->setTransform(QTransform().scale(1, -1));
		//1st Y Marker
		m_vpYMarker[0] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1(), m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1()));
		m_vpYMarkerText[0] = scene()->addText(QString("0"));
		m_vpYMarkerText[0]->setPos(m_vpYMarker[0]->line().x2() - m_vpYMarkerText[0]->boundingRect().width() - m_iYMarkerTextOffset, m_vpYMarker[0]->line().y2() + m_vpYMarkerText[0]->boundingRect().height() / 2);
		m_vpYMarkerText[0]->setTransform(QTransform().scale(1, -1));
		//other X Markers
		offset = (double)m_iXAxisLength / ((m_iNXMarker - 1)*std::pow(2, m_iZoomIndexX));
		float coeffMarkerRatio = (float)pContent->getNumDFTCoeffs() / ((m_iNXMarker - 1)*std::pow(2, m_iZoomIndexX));
		for (int i = 1; i < (m_iNXMarker - 1)*std::pow(2, m_iZoomIndexX) + 1; i++)
		{
			m_vpXMarker[i] = scene()->addLine(QLine(m_pXAxis->line().x1() + offset*i, m_pXAxis->line().y1(), m_pXAxis->line().x1() + offset*i, m_pXAxis->line().y1() - m_iXMarkerLength));
			m_vpXMarkerText[i] = scene()->addText(QString::number((int)(coeffMarkerRatio*i*pContent->getFrequencyBandwidth())));
			m_vpXMarkerText[i]->setPos(m_vpXMarker[i]->line().x2() - m_vpXMarkerText[i]->boundingRect().width() / 2, m_vpXMarker[i]->line().y2() - m_iXMarkerTextOffset);
			m_vpXMarkerText[i]->setTransform(QTransform().scale(1, -1));
		}
		//other Y Markers
		offset = (double)m_iYAxisLength / ((m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY));
		float indexMarkerRatio = (float)pContent->getOverallMagnitudeMaximum() / ((m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY));
		for (int i = 1; i < (m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY)+1; i++)
		{
			m_vpYMarker[i] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1() + i*offset, m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1() + i*offset));
			m_vpYMarkerText[i] = scene()->addText(QString::number((float)indexMarkerRatio*i,10,2));
			m_vpYMarkerText[i]->setPos(m_vpYMarker[i]->line().x2() - m_vpYMarkerText[i]->boundingRect().width() - m_iYMarkerTextOffset, m_vpYMarker[i]->line().y2() + m_vpYMarkerText[i]->boundingRect().height() / 2);
			m_vpYMarkerText[i]->setTransform(QTransform().scale(1, -1));
		}
		break;
	}

	case DAFF_PHASE_SPECTRUM:
	{
		//fixed logarithmic scale, 28 from 20 to 20k Hz
		m_vpXMarker.resize(28);
		//show 20,40,60,100,200,400,600,1000,...
		m_vpXMarkerText.resize(13);
		//Draw Axis
		m_pXAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iXAxisLength + m_iAxisOffsetLeft, m_iAxisOffsetDown));
		m_pYAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iAxisOffsetLeft, m_iYAxisLength + m_iAxisOffsetDown));
		//Draw Grid
		m_vpYGrid.resize(m_iNYGrid*std::pow(2, m_iZoomIndexY));
		double offset = (double)m_iYAxisLength / (m_iNYGrid*std::pow(2, m_iZoomIndexY));
		for (int i = 0; i < m_vpYGrid.size(); i++)
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
		m_vpXMarkerText[0]->setPos(m_vpXMarker[0]->line().x2() - m_vpXMarkerText[0]->boundingRect().width() / 2, m_vpXMarker[0]->line().y2() - m_iXMarkerTextOffset);
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
		yText = scene()->addText(QString("Phase"));
		yText->setPos(tipY[1]);
		yText->setTransform(QTransform().scale(1, -1));
		//Draw Marker
		DAFFContentPS* pContent = dynamic_cast< DAFFContentPS* >(m_pReader->getContent());
		//Y Markers
		offset = (double)m_iYAxisLength / ((m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY));
		for (int j = 0; j < (m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY) + 1; j++)
		{
			m_vpYMarker[j] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1() + j*offset, m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1() + j*offset));
			m_vpYMarkerText[j] = scene()->addText(QString::number(-180 + j * 360 / ((m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY)), 10, 0).append(QChar(0x00B0)));
			m_vpYMarkerText[j]->setPos(m_vpYMarker[j]->line().x2() - m_vpYMarkerText[j]->boundingRect().width() - m_iYMarkerTextOffset, m_vpYMarker[j]->line().y2() + m_vpYMarkerText[j]->boundingRect().height() / 2);
			m_vpYMarkerText[j]->setTransform(QTransform().scale(1, -1));
		}
		break;
	}
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
	{
		//fixed logarithmic scale, 28 from 20 to 20k Hz
		m_vpXMarker.resize(28);
		//show 20,40,60,100,200,400,600,1000,...
		m_vpXMarkerText.resize(13);
		//Draw Axis
		m_pXAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iXAxisLength + m_iAxisOffsetLeft, m_iAxisOffsetDown));
		m_pYAxis = scene()->addLine(QLine(m_iAxisOffsetLeft, m_iAxisOffsetDown, m_iAxisOffsetLeft, m_iYAxisLength + m_iAxisOffsetDown));
		//Draw Grid
		m_vpYGrid.resize(m_iNYGrid*std::pow(2, m_iZoomIndexY));
		double offset = (double)m_iYAxisLength / (m_iNYGrid*std::pow(2, m_iZoomIndexY));
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
		m_vpXMarkerText[0]->setPos(m_vpXMarker[0]->line().x2() - m_vpXMarkerText[0]->boundingRect().width() / 2, m_vpXMarker[0]->line().y2() - m_iXMarkerTextOffset);
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
		offset = (double)m_iYAxisLength / ((m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY));
		for (int i = 1; i < (m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY) + 1; i++)
		{
			m_vpYMarker[i] = scene()->addLine(QLine(m_pYAxis->line().x1(), m_pYAxis->line().y1() + i*offset, m_pYAxis->line().x1() - m_iYMarkerLength, m_pYAxis->line().y1() + i*offset));
			m_vpYMarkerText[i] = scene()->addText(QString::number((i / ((m_iNYMarker - 1)*std::pow(2, m_iZoomIndexY)))*pContent->getOverallMagnitudeMaximum(), 10, 2));
			m_vpYMarkerText[i]->setPos(m_vpYMarker[i]->line().x2() - m_vpYMarkerText[i]->boundingRect().width() - m_iYMarkerTextOffset, m_vpYMarker[i]->line().y2() + m_vpYMarkerText[i]->boundingRect().height() / 2);
			m_vpYMarkerText[i]->setTransform(QTransform().scale(1, -1));
		}
		break;
	}
	}
}

void QDAFF2DPlot::DrawGraph(int recordIndex, int channelIndex, bool showAllChannels, bool showDots)
{
	if (m_pReader == nullptr)
		return;

	switch (m_pReader->getContentType())
	{
	case DAFF_MAGNITUDE_SPECTRUM:
	{
		DAFFContentMS* pContent = dynamic_cast< DAFFContentMS* >(m_pReader->getContent());
		float amplitudePixelRatio = m_iYAxisLength / pContent->getOverallMagnitudeMaximum();
		std::vector<float> mags(pContent->getNumFrequencies());
		QString text;
		if (showAllChannels)
		{
			m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(pContent->getProperties()->getNumberOfChannels());
			m_vvpPoints = std::vector<std::vector<QGraphicsPoint*>>(pContent->getProperties()->getNumberOfChannels());
		}
		else
		{
			m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(1);
			m_vvpPoints = std::vector<std::vector<QGraphicsPoint*>>(1);
		}
		if (pContent->getNumFrequencies() > 1)
		{
			double logRange = std::log(20000) - std::log(20);
			double offset1, offset2;
			for (int i = 0; i < m_vvpPoints.size(); i++)
			{				
				m_vvpGraphs[i] = std::vector<QGraphicsLineItem*>(pContent->getNumFrequencies() - 1);
				m_vvpPoints[i] = std::vector<QGraphicsPoint*>(pContent->getNumFrequencies());			
				for (int j = 0; j < m_vvpGraphs[i].size(); j++)
				{
					if (showAllChannels)
					{
						pContent->getMagnitudes(recordIndex, i, &mags[0]);
					}						
					else
					{
						pContent->getMagnitudes(recordIndex, channelIndex, &mags[0]);
					}
						
					offset1 = m_iXAxisLength*(std::log(pContent->getFrequencies()[j]) - std::log(20)) / logRange;
					offset2 = m_iXAxisLength*(std::log(pContent->getFrequencies()[j+1]) - std::log(20)) / logRange;
					if (showAllChannels)
						m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + offset1, m_pXAxis->line().y1() + mags[j]*amplitudePixelRatio, m_pXAxis->line().x1() + offset2, m_pXAxis->line().y1() + mags[j+1]*amplitudePixelRatio), QPen(m_voColors[i % 10]));
					else
						m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + offset1, m_pXAxis->line().y1() + mags[j] * amplitudePixelRatio, m_pXAxis->line().x1() + offset2, m_pXAxis->line().y1() + mags[j + 1] * amplitudePixelRatio), QPen(m_voColors[channelIndex % 10]));
					if (pContent->getNumFrequencies()*(m_iPointDiameter + 3) < m_iXAxisLength && showDots)
					{
						text = QString("Magnitude: ").append(QString::number(mags[j])).append("dB\nFrequency: ").append(QString::number(pContent->getFrequencies()[j])).append(QString("Hz"));
						m_vvpPoints[i][j] = new QGraphicsPoint(text, m_vvpGraphs[i][j]->line().x1(), m_vvpGraphs[i][j]->line().y1(), m_iPointDiameter);
						scene()->addItem(m_vvpPoints[i][j]);
					}				
				}
				if (pContent->getNumFrequencies()*(m_iPointDiameter + 3) < m_iXAxisLength && showDots)
				{
					size_t j = m_vvpPoints[i].size() - 1;
					text = QString("Magnitude: ").append(QString::number(mags[j])).append("\nFrequency: ").append(QString::number(pContent->getFrequencies()[j])).append(QString("Hz"));
					m_vvpPoints[i][j] = new QGraphicsPoint(text, m_vvpGraphs[i][j - 1]->line().x2(), m_vvpGraphs[i][j - 1]->line().y2(), m_iPointDiameter);
					scene()->addItem(m_vvpPoints[i][j]);
				}							
			}
		}
		break;
	}
	case DAFF_IMPULSE_RESPONSE:
	{
		DAFFContentIR* pContent = dynamic_cast< DAFFContentIR* >(m_pReader->getContent());
		std::vector<float> coeffs = std::vector<float>(pContent->getFilterLength());
		double amplitudePixelRatio = m_iYAxisLength / 2;
		double indexValueRatio = 1000000 / (pContent->getSamplerate());
		QString text;
		if (showAllChannels)
		{
			m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(pContent->getProperties()->getNumberOfChannels());
			m_vvpPoints = std::vector<std::vector<QGraphicsPoint*>>(pContent->getProperties()->getNumberOfChannels());
		}
		else
		{
			m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(1);
			m_vvpPoints = std::vector<std::vector<QGraphicsPoint*>>(1);
		}
		double dataPixelRatio; //number of data points per pixel, never smaller than 1
		double offset;
		float min, max; //used to store minimal and maximal amplitude of data points in a specific pixel column
		int xMin, xMax; //stores the right and left borders of the visible screen in x axis coordinates
		int iMin, iMax; //stores the right and left borders of the visible data points as index.
		//check if it needs to draw only whats visible
		if (pContent->getFilterLength() > m_iDrawLimit)
		{
			//check which part of the x axis is visible
			xMin = mapToScene(rect()).boundingRect().bottomLeft().x() - m_pXAxis->line().x1();
			if (xMin < 0)
				xMin = 0;
			xMax = mapToScene(rect()).boundingRect().bottomRight().x() - m_pXAxis->line().x1();
			if (xMax > m_iXAxisLength)
				xMax = m_iXAxisLength;
		}
		else
		{
			xMin = 0;
			xMax = m_iXAxisLength;
		}
		for (int i = 0; i < m_vvpGraphs.size(); i++)
		{
			if (showAllChannels)
				pContent->getFilterCoeffs(recordIndex, i, &coeffs[0]);
			else
				pContent->getFilterCoeffs(recordIndex, channelIndex, &coeffs[0]);
			
			if (pContent->getFilterLength() > m_iXAxisLength)
			{
				dataPixelRatio = (float)pContent->getFilterLength() / m_iXAxisLength; //number of data points per pixel
				offset = 1;
				m_vvpGraphs[i] = std::vector<QGraphicsLineItem*>(xMax - xMin -1);
				m_vvpPoints[i] = std::vector<QGraphicsPoint*>(xMax - xMin);
				for (int j = 0; j < m_vvpGraphs[i].size(); j++)
				{
					min = 0;
					max = 0;
					int indexMin = (j + xMin)*dataPixelRatio;
					int indexMax = (j + xMin + 1)*dataPixelRatio;
					for (int k = indexMin; k < indexMax; k++)
					{
						if (coeffs[k] > max)
							max = coeffs[k];
						if (coeffs[k] < min)
							min = coeffs[k];
					}
					if (showAllChannels)
						m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + j+xMin+1, m_pXAxis->line().y1() + (min + 1) * amplitudePixelRatio, m_pXAxis->line().x1() + j + xMin + 1, m_pXAxis->line().y1() + (max+1) * amplitudePixelRatio), QPen(m_voColors[i % 10]));
					else
						m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + j + xMin + 1, m_pXAxis->line().y1() + (min + 1) * amplitudePixelRatio, m_pXAxis->line().x1() + j + xMin + 1, m_pXAxis->line().y1() + (max + 1) * amplitudePixelRatio), QPen(m_voColors[channelIndex % 10]));
				}
			}
			else
			{
				dataPixelRatio = 1;
				offset = (double)m_iXAxisLength / (pContent->getFilterLength() - 1);
				iMin = (xMin / offset) - m_iAdditionalPoints;
				if (iMin < 0)
					iMin = 0;
				iMax = (xMax / offset) + m_iAdditionalPoints;
				if (iMax > pContent->getFilterLength() - 1)
					iMax = pContent->getFilterLength() - 1;
				m_vvpGraphs[i] = std::vector<QGraphicsLineItem*>(iMax - iMin - 1);
				m_vvpPoints[i] = std::vector<QGraphicsPoint*>(iMax - iMin);
				for (int j = 0; j < m_vvpGraphs[i].size(); j++)
				{
					if (showAllChannels)
						m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + (j + iMin)*offset, m_pXAxis->line().y1() + (coeffs[j + iMin] + 1) * amplitudePixelRatio, m_pXAxis->line().x1() + (j + iMin + 1)*offset, m_pXAxis->line().y1() + (coeffs[j + iMin + 1] + 1) * amplitudePixelRatio), QPen(m_voColors[i % 10]));
					else
						m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + (j + iMin)*offset, m_pXAxis->line().y1() + (coeffs[j + iMin] + 1) * amplitudePixelRatio, m_pXAxis->line().x1() + (j + iMin + 1)*offset, m_pXAxis->line().y1() + (coeffs[j + iMin + 1] + 1) * amplitudePixelRatio), QPen(m_voColors[channelIndex % 10]));
				}
			}						
			
			if (pContent->getFilterLength()*(m_iPointDiameter + 3) < m_iXAxisLength && showDots)
			{
				for (int j = 0; j < m_vvpGraphs[i].size(); j++)
				{
					text = QString("Amplitude: ").append(QString::number(coeffs[(int)((j + iMin)*dataPixelRatio)])).append("\nSample Time: ").append(QString::number(((int)((j + iMin)*dataPixelRatio))*indexValueRatio));
					m_vvpPoints[i][j] = new QGraphicsPoint(text, m_vvpGraphs[i][j]->line().x1(), m_vvpGraphs[i][j]->line().y1(), m_iPointDiameter);
				}
				size_t j = m_vvpGraphs[i].size();
				text = QString("Amplitude: ").append(QString::number(coeffs[(int)((j + iMin)*dataPixelRatio)])).append("\nSample Time: ").append(QString::number(((int)((j + iMin)*dataPixelRatio))*indexValueRatio));
				m_vvpPoints[i][j] = new QGraphicsPoint(text, m_vvpGraphs[i][j-1]->line().x2(), m_vvpGraphs[i][j-1]->line().y2(), m_iPointDiameter);
				for (int j = 0; j < m_vvpPoints[i].size(); j++)
				{
					scene()->addItem(m_vvpPoints[i][j]);
				}
			}
		}
		break;
	}
	case DAFF_DFT_SPECTRUM:
	{
		DAFFContentDFT* pContent = dynamic_cast< DAFFContentDFT* >(m_pReader->getContent());
		std::vector<float> coeffs = std::vector<float>(pContent->getNumDFTCoeffs()*2);
		std::vector<float> phases = std::vector<float>(pContent->getNumDFTCoeffs());
		float amplitudePixelRatio = (float)m_iYAxisLength/pContent->getOverallMagnitudeMaximum();
		float phasePixelRatio = m_iYAxisLength / (2*M_PI);
		float coeffPixelRatio = (float)m_iXAxisLength / pContent->getNumDFTCoeffs();
		QString text;
		if (showAllChannels)
		{
			m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(pContent->getProperties()->getNumberOfChannels() * 2);
			m_vvpPoints = std::vector<std::vector<QGraphicsPoint*>>(pContent->getProperties()->getNumberOfChannels() * 2);
		}
		else
		{
			m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(2);
			m_vvpPoints = std::vector<std::vector<QGraphicsPoint*>>(2);
		}
		for (int i = 0; i < m_vvpGraphs.size()/2; i++)
		{
			if (showAllChannels)
				pContent->getDFTCoeffs(recordIndex, i, &coeffs[0]);
			else
				pContent->getDFTCoeffs(recordIndex, channelIndex, &coeffs[0]);
			m_vvpGraphs[2*i] = std::vector<QGraphicsLineItem*>(pContent->getNumDFTCoeffs() - 1);
			m_vvpPoints[2*i] = std::vector<QGraphicsPoint*>(pContent->getNumDFTCoeffs());
			m_vvpGraphs[2 * i+1] = std::vector<QGraphicsLineItem*>(pContent->getNumDFTCoeffs() - 1);
			m_vvpPoints[2 * i+1] = std::vector<QGraphicsPoint*>(pContent->getNumDFTCoeffs());
			std::complex <float> z(coeffs[0], coeffs[1]);
			phases[0] = std::arg(z);
			if (showAllChannels)
			{
				for (int j = 0; j < m_vvpGraphs[2 * i].size(); j++)
				{
					m_vvpGraphs[2 * i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + (j + 0.5)*coeffPixelRatio, m_pXAxis->line().y1() + sqrt(coeffs[2 * j] * coeffs[2 * j] + coeffs[2 * j + 1] * coeffs[2 * j + 1]) * amplitudePixelRatio, m_pXAxis->line().x1() + (j + 1.5)*coeffPixelRatio, m_pXAxis->line().y1() + sqrt(coeffs[2 * (j + 1)] * coeffs[2 * (j + 1)] + coeffs[2 * (j + 1) + 1] * coeffs[2 * (j + 1) + 1]) * amplitudePixelRatio), QPen(m_voColors[i % 12]));
					z = std::complex<float>(coeffs[2 * (j + 1)], coeffs[2 * (j + 1) + 1]);
					phases[j + 1] = std::arg(z);
					m_vvpGraphs[2 * i + 1][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + (j + 0.5)*coeffPixelRatio, m_pXAxis->line().y1() + (phases[j] + M_PI)*phasePixelRatio, m_pXAxis->line().x1() + (j + 1.5)*coeffPixelRatio, m_pXAxis->line().y1() + (phases[j + 1] + M_PI)*phasePixelRatio), QPen(m_voColors[(i + 6) % 12]));
				}
			}
			else
			{
				for (int j = 0; j < m_vvpGraphs[2 * i].size(); j++)
				{
					m_vvpGraphs[2 * i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + (j + 0.5)*coeffPixelRatio, m_pXAxis->line().y1() + sqrt(coeffs[2 * j] * coeffs[2 * j] + coeffs[2 * j + 1] * coeffs[2 * j + 1]) * amplitudePixelRatio, m_pXAxis->line().x1() + (j + 1.5)*coeffPixelRatio, m_pXAxis->line().y1() + sqrt(coeffs[2 * (j + 1)] * coeffs[2 * (j + 1)] + coeffs[2 * (j + 1) + 1] * coeffs[2 * (j + 1) + 1]) * amplitudePixelRatio), QPen(m_voColors[channelIndex % 12]));
					z = std::complex<float>(coeffs[2 * (j + 1)], coeffs[2 * (j + 1) + 1]);
					phases[j + 1] = std::arg(z);
					m_vvpGraphs[2 * i + 1][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + (j + 0.5)*coeffPixelRatio, m_pXAxis->line().y1() + (phases[j] + M_PI)*phasePixelRatio, m_pXAxis->line().x1() + (j + 1.5)*coeffPixelRatio, m_pXAxis->line().y1() + (phases[j + 1] + M_PI)*phasePixelRatio), QPen(m_voColors[(channelIndex + 6) % 12]));
				}
			}
			
			if (m_vvpPoints[2*i].size()*(m_iPointDiameter + 3) < m_iXAxisLength)
			{
			  for (int j = 0; j < m_vvpGraphs[2*i].size(); j++)
			  {
				  text = QString("Amplitude: ").append(QString::number(sqrt(coeffs[2 * j] * coeffs[2 * j] + coeffs[2 * j + 1] * coeffs[2 * j + 1]))).append("\nPhase: ").append(QString::number(phases[j] * 180 / M_PI)).append(QChar(0x00B0)).append("\nFrequency bin: ").append(QString::number(j*pContent->getFrequencyBandwidth())).append("Hz - ").append(QString::number((j + 1)*pContent->getFrequencyBandwidth())).append("Hz");
				  m_vvpPoints[2*i][j] = new QGraphicsPoint(text, m_vvpGraphs[2*i][j]->line().x1(), m_vvpGraphs[2*i][j]->line().y1(), m_iPointDiameter);
				  m_vvpPoints[2 * i+1][j] = new QGraphicsPoint(text, m_vvpGraphs[2*i+1][j]->line().x1(), m_vvpGraphs[2*i+1][j]->line().y1(), m_iPointDiameter);
			  }
			  size_t j = m_vvpGraphs[i].size();
			  text = QString("Amplitude: ").append(QString::number(sqrt(coeffs[2 * j] * coeffs[2 * j] + coeffs[2 * j + 1] * coeffs[2 * j + 1]))).append("\nPhase: ").append(QString::number(phases[j] * 180 / M_PI)).append(QChar(0x00B0)).append("\nfrequency bin: ").append(QString::number(j*pContent->getFrequencyBandwidth())).append("Hz - ").append(QString::number((j + 1)*pContent->getFrequencyBandwidth())).append("Hz");
			  m_vvpPoints[i][j] = new QGraphicsPoint(text, m_vvpGraphs[i][j - 1]->line().x2(), m_vvpGraphs[i][j - 1]->line().y2(), m_iPointDiameter);
			  for (int j = 0; j < m_vvpPoints[2*i].size(); j++)
			  {
				  scene()->addItem(m_vvpPoints[2*i][j]);
				  scene()->addItem(m_vvpPoints[2 * i+1][j]);
			  }
			}
			}
			break;
	}
	case DAFF_PHASE_SPECTRUM:
	{
			DAFFContentPS* pContent = dynamic_cast< DAFFContentPS* >(m_pReader->getContent());
			float PhasePixelRatio = m_iYAxisLength / (2*M_PI);
			std::vector<float> phases(pContent->getNumFrequencies());
			QString text;
			if (showAllChannels)
			{
				m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(pContent->getProperties()->getNumberOfChannels());
				m_vvpPoints = std::vector<std::vector<QGraphicsPoint*>>(pContent->getProperties()->getNumberOfChannels());
			}
			else
			{
				m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(1);
				m_vvpPoints = std::vector<std::vector<QGraphicsPoint*>>(1);
			}
			if (pContent->getNumFrequencies() > 1)
			{
				double logRange = std::log(20000) - std::log(20);
				double offset1, offset2;
				for (int i = 0; i < m_vvpPoints.size(); i++)
				{
					m_vvpGraphs[i] = std::vector<QGraphicsLineItem*>(pContent->getNumFrequencies() - 1);
					m_vvpPoints[i] = std::vector<QGraphicsPoint*>(pContent->getNumFrequencies());
					if (showAllChannels)
						pContent->getPhases(recordIndex, i, &phases[0]);
					else
						pContent->getPhases(recordIndex, channelIndex, &phases[0]);
					for (int j = 0; j < m_vvpGraphs[i].size(); j++)
					{
						offset1 = m_iXAxisLength*(std::log(pContent->getFrequencies()[j]) - std::log(20)) / logRange;
						offset2 = m_iXAxisLength*(std::log(pContent->getFrequencies()[j + 1]) - std::log(20)) / logRange;
						if (showAllChannels)
							m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + offset1, m_pXAxis->line().y1() + (phases[j] + M_PI)*PhasePixelRatio, m_pXAxis->line().x1() + offset2, m_pXAxis->line().y1() + (phases[j+1] + M_PI)*PhasePixelRatio), QPen(m_voColors[i % 12]));
						else
							m_vvpGraphs[i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + offset1, m_pXAxis->line().y1() + (phases[j] + M_PI)*PhasePixelRatio, m_pXAxis->line().x1() + offset2, m_pXAxis->line().y1() + (phases[j + 1] + M_PI)*PhasePixelRatio), QPen(m_voColors[channelIndex % 12]));
						if (pContent->getNumFrequencies()*(m_iPointDiameter + 3) < m_iXAxisLength && showDots)
						{
							text = QString("Phase: ").append(QString::number(phases[j] * 180 / M_PI)).append(QChar(0x00B0)).append("\nFrequency: ").append(QString::number(pContent->getFrequencies()[j])).append("Hz");
							m_vvpPoints[i][j] = new QGraphicsPoint(text, m_vvpGraphs[i][j]->line().x1(), m_vvpGraphs[i][j]->line().y1(), m_iPointDiameter);
							scene()->addItem(m_vvpPoints[i][j]);
						}
					}
					if (pContent->getNumFrequencies()*(m_iPointDiameter + 3) < m_iXAxisLength && showDots)
					{
						size_t j = m_vvpPoints[i].size() - 1;
						text = QString("Phase: ").append(QString::number(phases[j] * 180 / M_PI)).append(QChar(0x00B0)).append("\nFrequency: ").append(QString::number(pContent->getFrequencies()[j])).append("Hz");
						m_vvpPoints[i][j] = new QGraphicsPoint(text, m_vvpGraphs[i][j - 1]->line().x2(), m_vvpGraphs[i][j - 1]->line().y2(), m_iPointDiameter);
						scene()->addItem(m_vvpPoints[i][j]);
					}
				}
			}
			break;
	}
	case DAFF_MAGNITUDE_PHASE_SPECTRUM:
	{
		DAFFContentMPS* pContent = dynamic_cast<DAFFContentMPS*>(m_pReader->getContent());
		float amplitudePixelRatio = m_iYAxisLength / pContent->getOverallMagnitudeMaximum();
		float phasePixelRatio = m_iYAxisLength / (2 * M_PI);
		std::vector<float> mags(pContent->getNumFrequencies()*2);
		std::vector<float> phases(pContent->getNumFrequencies()*2);
		QString text;
		if (showAllChannels)
		{
		  m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(pContent->getProperties()->getNumberOfChannels()*2);
		  m_vvpPoints = std::vector<std::vector<QGraphicsPoint*>>(pContent->getProperties()->getNumberOfChannels()*2);
		}
		else
		{
		  m_vvpGraphs = std::vector<std::vector<QGraphicsLineItem*>>(2);
		  m_vvpPoints = std::vector<std::vector<QGraphicsPoint*>>(2);
		}
		if (pContent->getNumFrequencies() > 1)
		{
		  double logRange = std::log(20000) - std::log(20);
		  double offset1, offset2;
		  for (int i = 0; i < m_vvpPoints.size()/2; i++)
		  {
			  m_vvpGraphs[2*i] = std::vector<QGraphicsLineItem*>(pContent->getNumFrequencies() - 1);
			  m_vvpGraphs[2 * i+1] = std::vector<QGraphicsLineItem*>(pContent->getNumFrequencies() - 1);
			  m_vvpPoints[2 * i] = std::vector<QGraphicsPoint*>(pContent->getNumFrequencies());
			  m_vvpPoints[2 * i+1] = std::vector<QGraphicsPoint*>(pContent->getNumFrequencies());
			  for (int j = 0; j < m_vvpGraphs[2*i].size(); j++)
			  {
				  if (showAllChannels)
				  {
					  pContent->getMagnitudes(recordIndex, i,&mags[0]);
					  pContent->getPhases(recordIndex, i, &phases[0]);
				  }
				  else
				  {
					  pContent->getMagnitudes(recordIndex, channelIndex, &mags[0]);
					  pContent->getPhases(recordIndex, channelIndex, &phases[0]);
				  }
				  offset1 = m_iXAxisLength*(std::log(pContent->getFrequencies()[j]) - std::log(20)) / logRange;
				  offset2 = m_iXAxisLength*(std::log(pContent->getFrequencies()[j + 1]) - std::log(20)) / logRange;
				  if (showAllChannels)
				  {
					  m_vvpGraphs[2 * i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + offset1, m_pXAxis->line().y1() + mags[j] * amplitudePixelRatio, m_pXAxis->line().x1() + offset2, m_pXAxis->line().y1() + mags[j + 1] * amplitudePixelRatio), QPen(m_voColors[i % 12]));
					  m_vvpGraphs[2 * i + 1][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + offset1, m_pXAxis->line().y1() + (phases[j] + M_PI)*phasePixelRatio, m_pXAxis->line().x1() + offset2, m_pXAxis->line().y1() + (phases[j+1] + M_PI)*phasePixelRatio), QPen(m_voColors[(i + 6) % 12]));
				  }
				  else
				  {
					  m_vvpGraphs[2 * i][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + offset1, m_pXAxis->line().y1() + mags[j] * amplitudePixelRatio, m_pXAxis->line().x1() + offset2, m_pXAxis->line().y1() + mags[j + 1] * amplitudePixelRatio), QPen(m_voColors[channelIndex % 12]));
					  m_vvpGraphs[2 * i + 1][j] = scene()->addLine(QLine(m_pXAxis->line().x1() + offset1, m_pXAxis->line().y1() + (phases[j] + M_PI)*phasePixelRatio, m_pXAxis->line().x1() + offset2, m_pXAxis->line().y1() + (phases[j+1] + M_PI)*phasePixelRatio), QPen(m_voColors[(channelIndex + 6) % 12]));
				  }
				  
				  if (pContent->getNumFrequencies()*(m_iPointDiameter + 3) < m_iXAxisLength && showDots)
				  {
					  text = QString("Magnitude: ").append(QString::number(mags[j])).append(QString("dB\nPhase: ")).append(QString::number(phases[j] * 180 / M_PI)).append(QChar(0x00B0)).append("\nFrequency: ").append(QString::number(pContent->getFrequencies()[j])).append("Hz");
					  m_vvpPoints[2 * i][j] = new QGraphicsPoint(text, m_vvpGraphs[2 * i][j]->line().x1(), m_vvpGraphs[2 * i][j]->line().y1(), m_iPointDiameter);
					  m_vvpPoints[2 * i+1][j] = new QGraphicsPoint(text, m_vvpGraphs[2 * i+1][j]->line().x1(), m_vvpGraphs[2 * i+1][j]->line().y1(), m_iPointDiameter);
					  scene()->addItem(m_vvpPoints[2 * i][j]);
					  scene()->addItem(m_vvpPoints[2 * i+1][j]);
				  }
			  }
			  if (pContent->getNumFrequencies()*(m_iPointDiameter + 3) < m_iXAxisLength && showDots)
			  {
				  size_t j = m_vvpPoints[i].size() - 1;
				  text = QString("Magnitude: ").append(QString::number(mags[j])).append(QString("dB\nPhase: ")).append(QString::number(phases[j] * 180 / M_PI)).append(QChar(0x00B0)).append("\nFrequency: ").append(QString::number(pContent->getFrequencies()[j])).append("Hz");
				  m_vvpPoints[2 * i][j] = new QGraphicsPoint(text, m_vvpGraphs[2 * i][j - 1]->line().x2(), m_vvpGraphs[2*i][j - 1]->line().y2(), m_iPointDiameter);
				  m_vvpPoints[2 * i + 1][j] = new QGraphicsPoint(text, m_vvpGraphs[2 * i + 1][j - 1]->line().x2(), m_vvpGraphs[2 * i+1][j - 1]->line().y2(), m_iPointDiameter);
				  scene()->addItem(m_vvpPoints[2 * i][j]);
				  scene()->addItem(m_vvpPoints[2 * i+1][j]);
			  }
		  }
		}
	}
		break;
	}
}

void QDAFF2DPlot::ExportImagePNG( const QString& filePath, float factor, bool showAllChannels, bool showDots )
{
	bool showAllChannelsOld = m_bShowAllChannels;
	bool showDotsOld = m_bShowDots;
	m_bShowAllChannels = showAllChannels;
	m_bShowDots = showDots;
	int iSceneWidth = int( width() * factor );
	int iSceneHeight = int( height() * factor );
	scene()->setSceneRect( 0, 0, iSceneWidth, iSceneHeight );
	Draw();

	QImage img( iSceneWidth, iSceneHeight, QImage::Format_ARGB32_Premultiplied );
	QPainter p( &img );
	p.setRenderHint(QPainter::Antialiasing);
	scene()->render( &p );
	p.end();
	img.mirrored( false, true ).save( filePath );
	m_bShowAllChannels = showAllChannelsOld;
	m_bShowDots = showDotsOld;

	Draw();
}

void QDAFF2DPlot::ExportImageSVG( const QString& filePath, float factor, bool showAllChannels, bool showDots )
{
	bool showAllChannelsOld = m_bShowAllChannels;
	bool showDotsOld = m_bShowDots;
	m_bShowAllChannels = showAllChannels;
	m_bShowDots = showDots;
	int iSceneWidth = int( width() * factor );
	int iSceneHeight = int( height() * factor );
	scene()->setSceneRect( 0, 0, iSceneWidth, iSceneHeight );
	Draw();
	
	QSvgGenerator generator;
	generator.setFileName( filePath );
	generator.setSize( QSize( iSceneWidth, iSceneHeight ) );
	QPainter p;
	p.begin( &generator );
	p.scale(1, -1);
	p.translate(0, -iSceneHeight);
	scene()->render( &p );
	p.end();

	m_bShowAllChannels = showAllChannelsOld;
	m_bShowDots = showDotsOld;

	Draw();
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

void QDAFF2DPlot::resizeEvent( QResizeEvent* event )
{	
	int iWidth = width();
	int iHeight = height();
	fitInView( 0, 0, iWidth, iHeight, Qt::KeepAspectRatio );
	
	QGraphicsView::resizeEvent( event );

	Draw();
}

void QDAFF2DPlot::wheelEvent( QWheelEvent* event )
{
	int numDegrees = int( event->delta() / 8.0f );
	int numSteps = int( numDegrees / 15.0f );

	if( event->orientation() == Qt::Vertical )
	{
		int iWidth = int( width() * std::pow( 1.1, numSteps ) );
		int iHeight = int( height() * std::pow( 1.1, numSteps ) );

		if( m_bScrollHorizontally )
			scene()->setSceneRect( 0, 0, iWidth, height() );
		else
			scene()->setSceneRect( 0, 0, width(), iHeight );

		Draw();

		if( iWidth > rect().width() )
			m_iZoomIndexX = ( int ) std::log2( iWidth / rect().width() );
		if( iHeight > rect().height() )
			m_iZoomIndexY = ( int ) std::log2( iHeight / rect().height() );

		event->accept();
	}
}
