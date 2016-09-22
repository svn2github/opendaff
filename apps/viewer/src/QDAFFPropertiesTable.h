/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.
 *
 *  ------------------------------------------------------------------------------------
 *
 */

#ifndef QDAFFPROPERTIESTABLE_H
#define QDAFFPROPERTIESTABLE_H

#include <QTableView>
#include <QHeaderView>
#include <QColor>
#include <QFont>
#include <QAbstractTableModel>

#include <DAFF.h>

#include <assert.h>
#include <iostream>

class DAFFPropertiesModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	inline DAFFPropertiesModel( QObject* pParent, const DAFFProperties* pProps ) 
		: QAbstractTableModel( pParent )
	, m_pContent( pProps ) 
	{
	};

    inline int rowCount( const QModelIndex& ) const
	{
		return 18;
	};
    inline int columnCount( const QModelIndex& ) const
	{
		return 2;
	};

	inline QVariant headerData( int section, Qt::Orientation orientation, int role /* = Qt::DisplayRole */ ) const
	{
		if( role == Qt::DisplayRole )
		{
			if( orientation == Qt::Horizontal )
			{			
				switch( section )
				{
				case 0:
					return QString( "Name" );
				case 1:
					return QString( "Value" );
				}
			}
		}

		if( role == Qt::FontRole )
		{
			QFont font;
			font.setBold( true );
			return font;
		}
				
		return QVariant();
	};

	inline QVariant data( const QModelIndex &index, int iRole = Qt::DisplayRole ) const
	{
		if( iRole == Qt::DisplayRole )
		{
			switch( index.row() )
			{
			case 0:
				return ( index.column() == 0 ? QString( "Number of channels" ) : QString::number( m_pContent->getNumberOfChannels() ) );
			case 1:
				return ( index.column() == 0 ? QString( "Number of records" ) : QString::number( m_pContent->getNumberOfRecords() ) );
			case 2:
				return ( index.column() == 0 ? QString( "Quantization" ) : QString::fromStdString( DAFFUtils::StrQuantizationType( m_pContent->getQuantization() ) ) );
			case 3:
				return ( index.column() == 0 ? QString( "Alpha points" ) : QString::number( m_pContent->getAlphaPoints() ) );
			case 4:
				return ( index.column() == 0 ? QString( "Alpha resolution" ) : QStringLiteral( "%1°" ).arg( m_pContent->getAlphaResolution() ) );
			case 5:
				return ( index.column() == 0 ? QString( "Alpha start" ) : QStringLiteral( "%1°" ).arg( m_pContent->getAlphaStart() ) );
			case 6:
				return ( index.column() == 0 ? QString( "Alpha end" ) : QStringLiteral( "%1°" ).arg( m_pContent->getAlphaEnd() ) );
			case 7:
				return ( index.column() == 0 ? QString( "Alpha span" ) : QStringLiteral( "%1°" ).arg( m_pContent->getAlphaSpan() ) );
			case 8:
				return ( index.column() == 0 ? QString( "Alpha full range" ) : m_pContent->coversFullAlphaRange() ? QString( "yes" ) : QString( "no" ) );
			case 9:
				return ( index.column() == 0 ? QString( "Beta points" ) : QString::number( m_pContent->getBetaPoints() ) );
			case 10:
				return ( index.column() == 0 ? QString( "Beta resolution" ) : QStringLiteral( "%1°" ).arg( m_pContent->getBetaResolution() ) );
			case 11:
				return ( index.column() == 0 ? QString( "Beta start" ) : QStringLiteral( "%1°" ).arg( m_pContent->getBetaStart() ) );
			case 12:
				return ( index.column() == 0 ? QString( "Beta end" ) : QStringLiteral( "%1°" ).arg( m_pContent->getBetaEnd() ) );
			case 13:
				return ( index.column() == 0 ? QString( "Beta span" ) : QStringLiteral( "%1°" ).arg( m_pContent->getBetaSpan() ) );
			case 14:
				return ( index.column() == 0 ? QString( "Beta full range" ) : m_pContent->coversFullBetaRange() ? QString( "yes" ) : QString( "no" ) );
			case 15:
				return ( index.column() == 0 ? QString( "Covers full sphere" ) : m_pContent->coversFullSphere() ? QString( "yes" ) : QString( "no" ) );
			case 16:
			{
				DAFFOrientationYPR o;
				m_pContent->getDefaultOrientation( o );
				return ( index.column() == 0 ? QString( "Default orientation YPR" ) : QStringLiteral( "%1°, %2°, %3°" ).arg( o.fYawAngleDeg ).arg( o.fPitchAngleDeg ).arg( o.fRollAngleDeg ) );
			}
			case 17:
			{
				DAFFOrientationYPR o;
				m_pContent->getOrientation( o );
				return ( index.column() == 0 ? QString( "Current orientation YPR" ) : QStringLiteral( "%1°, %2°, %3°" ).arg( o.fYawAngleDeg ).arg( o.fPitchAngleDeg ).arg( o.fRollAngleDeg ) );
			}
			}
		}

		if( iRole == Qt::FontRole )
		{
			if( index.column() == 0 )
			{
				QFont font;
				font.setBold( true );
				return font;
			}
		}

		if( iRole == Qt::BackgroundColorRole )
		{
			if( index.row() % 2 == 1 )
			{
				QColor color;
				color.setRgb( 240, 240, 240 );
				return color;
			}
		}

		return QVariant();
	};
private:
	const DAFFProperties* m_pContent;
};


class QDAFFPropertiesTableView : public QTableView
{
    Q_OBJECT

public:
	inline QDAFFPropertiesTableView( QWidget *parent = Q_NULLPTR )
                : QTableView( parent )
				, m_pModel( NULL )
    {
    }
public slots:
	inline void CloseDAFF()
	{
		delete m_pModel;
		m_pModel = NULL;
	}
    inline void ReadDAFF( const DAFFReader* pReader )
	{
		if( pReader == NULL )
			return;

		m_pModel = new DAFFPropertiesModel( this, pReader->getProperties()  );
		setModel( m_pModel );
		verticalHeader()->hide();
		horizontalHeader()->resizeSections( QHeaderView::Stretch );
    }
private:
	DAFFPropertiesModel*  m_pModel;
};

#endif // QDAFFPROPERTIESTABLE_H
