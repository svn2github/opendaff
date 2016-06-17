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
	, m_pProps( pProps ) 
	{
	};

    inline int rowCount( const QModelIndex& ) const
	{
		return 16;
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
				return ( index.column() == 0 ? QString( "Number of channels" ) : QString::number( m_pProps->getNumberOfChannels() ) );
			case 1:
				return ( index.column() == 0 ? QString( "Number of records" ) : QString::number( m_pProps->getNumberOfRecords() ) );
			case 2:
				return ( index.column() == 0 ? QString( "Quantization" ) : QString::fromStdString( DAFFUtils::StrQuantizationType( m_pProps->getQuantization() ) ) );
			case 3:
				return ( index.column() == 0 ? QString( "Alpha points" ) : QString::number( m_pProps->getAlphaPoints() ) );
			case 4:
				return ( index.column() == 0 ? QString( "Alpha start (deg)" ) : QString::number( m_pProps->getAlphaStart() ) );
			case 5:
				return ( index.column() == 0 ? QString( "Alpha end (deg)" ) : QString::number( m_pProps->getAlphaEnd() ) );
			case 6:
				return ( index.column() == 0 ? QString( "Alpha span (deg)" ) : QString::number( m_pProps->getAlphaSpan() ) );
			case 7:
				return ( index.column() == 0 ? QString( "Alpha full range" ) : m_pProps->coversFullAlphaRange() ? QString( "yes" ) : QString( "no" ) );
			case 8:
				return ( index.column() == 0 ? QString( "Beta points" ) : QString::number( m_pProps->getBetaPoints() ) );
			case 9:
				return ( index.column() == 0 ? QString( "Beta start (deg)" ) : QString::number( m_pProps->getBetaStart() ) );
			case 10:
				return ( index.column() == 0 ? QString( "Beta end (deg)" ) : QString::number( m_pProps->getBetaEnd() ) );
			case 11:
				return ( index.column() == 0 ? QString( "Beta span (deg)" ) : QString::number( m_pProps->getBetaSpan() ) );
			case 12:
				return ( index.column() == 0 ? QString( "Beta full range" ) : m_pProps->coversFullBetaRange() ? QString( "yes" ) : QString( "no" ) );
			case 13:
				return ( index.column() == 0 ? QString( "Covers full sphere" ) : m_pProps->coversFullSphere() ? QString( "yes" ) : QString( "no" ) );
			case 14:
			{
				DAFFOrientationYPR o;
				m_pProps->getDefaultOrientation( o );
				return ( index.column() == 0 ? QString( "Default orientation YPR (deg)" ) : QString( "%1, %2, %3" ).arg( o.fYawAngleDeg ).arg( o.fPitchAngleDeg ).arg( o.fRollAngleDeg ) );
			}
			case 15:
			{
				DAFFOrientationYPR o;
				m_pProps->getOrientation( o );
				return ( index.column() == 0 ? QString( "Current orientation YPR (deg)" ) : QString( "%1, %2, %3" ).arg( o.fYawAngleDeg ).arg( o.fPitchAngleDeg ).arg( o.fRollAngleDeg ) );
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
	const DAFFProperties* m_pProps;
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
		this->setModel( m_pModel );
		this->verticalHeader()->hide();
        this->horizontalHeader()->resizeSections( QHeaderView::ResizeToContents );
    }
private:
	DAFFPropertiesModel*  m_pModel;
};

#endif // QDAFFPROPERTIESTABLE_H
