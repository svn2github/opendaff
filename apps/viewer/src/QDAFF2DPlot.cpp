#include "QDAFF2DPlot.h"

#include <DAFF.h>

QDAFF2DPlot::QDAFF2DPlot( QWidget *parent )
		: QGraphicsView( new QGraphicsScene(), parent )
		, m_pReader( NULL )
{
	setScene( new QGraphicsScene() );
	QGraphicsRectItem *rect = scene()->addRect( QRectF( 0, 0, 100, 100 ) );
	QGraphicsItem *item = scene()->itemAt( 50, 50, QTransform() );
		
	//setViewport( new QOpenGLWidget( this ) );
	setBackgroundBrush( QBrush( Qt::darkBlue ) );
}

void QDAFF2DPlot::ReadDAFF( const DAFFReader* pReader )
{
	m_pReader = pReader;
}

void QDAFF2DPlot::CloseDAFF()
{
	m_pReader = NULL;
}

void QDAFF2DPlot::ChangeFrequencyIndex( int iRecordIndex )
{
	if( m_pReader == nullptr )
		return;

	switch( m_pReader->getContentType() )
	{
	case DAFF_IMPULSE_RESPONSE:
	{
		DAFFContentIR* pContent = dynamic_cast< DAFFContentIR* >( m_pReader->getContent() );
		std::vector< float > vrRecordData( pContent->getFilterLength() );

		const float fMaxValue = pContent->getOverallPeak();

		for( int iChannel = 0; iChannel < m_pReader->getProperties()->getNumberOfChannels(); iChannel++ )
		{
			pContent->getFilterCoeffs( iRecordIndex, iChannel, &vrRecordData[ 0 ] );

			// Plot data!
			//scene()->addPolygon()
		}
	}
	case DAFF_MAGNITUDE_SPECTRUM:
	{
		DAFFContentMS* pContent = dynamic_cast< DAFFContentMS* >( m_pReader->getContent() );
		pContent->getOverallMagnitudeMaximum();
	}
	case DAFF_DFT_SPECTRUM:
	{
		DAFFContentDFT* pContent = dynamic_cast< DAFFContentDFT* >( m_pReader->getContent() );
		pContent->getOverallMagnitudeMaximum();
	}
	}
}
