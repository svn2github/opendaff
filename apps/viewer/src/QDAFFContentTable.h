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

#ifndef QDAFFCONTENTTABLE_H
#define QDAFFCONTENTTABLE_H

#include <QTableView>
#include <QHeaderView>
#include <QColor>
#include <QFont>
#include <QAbstractTableModel>

#include <DAFF.h>

#include <assert.h>
#include <iostream>

class DAFFContentModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	inline DAFFContentModel( QObject* pParent )
		: QAbstractTableModel( pParent )
	{
	};

	inline int rowCount( const QModelIndex& ) const = 0;

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


	inline QVariant data( const QModelIndex &index, int iRole ) const
	{
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
	}
};

class QDAFFContentIRModel : public DAFFContentModel
{
public:
	inline QDAFFContentIRModel( QObject* pParent, DAFFContentIR* pContent )
		: DAFFContentModel( pParent )
		, m_pContent( pContent )
	{
	}

	inline int rowCount( const QModelIndex& ) const
	{
		return 5;
	};

	inline QVariant data( const QModelIndex &index, int iRole = Qt::DisplayRole ) const
	{
		if( iRole == Qt::DisplayRole )
		{
			switch( index.row() )
			{
			case 0:
				return ( index.column() == 0 ? QString( "Filter length" ) : QString::number( m_pContent->getFilterLength() ) );
			case 1:
				return ( index.column() == 0 ? QString( "Maximum effective filter length" ) : QString::number( m_pContent->getMaxEffectiveFilterLength() ) );
			case 2:
				return ( index.column() == 0 ? QString( "Minimum effective filter offset" ) : QString::number( m_pContent->getMinEffectiveFilterOffset() ) );
			case 3:
				return ( index.column() == 0 ? QString( "Overall peak value" ) : QString::number( m_pContent->getOverallPeak() ) );
			case 4:
				return ( index.column() == 0 ? QString( "Sampling rate" ) : QString( "%1 Hz" ).arg( m_pContent->getSamplerate() ) );
			}
		}

		return DAFFContentModel::data( index, iRole );
	};

private:
	DAFFContentIR* m_pContent;
};

class QDAFFContentMSModel : public DAFFContentModel
{
public:
	inline QDAFFContentMSModel( QObject* pParent, const DAFFContentMS* pContent )
		: DAFFContentModel( pParent )
		, m_pContent( pContent )
	{
	}

	inline int rowCount( const QModelIndex& ) const
	{
		return 3;
	};

	inline QVariant data( const QModelIndex &index, int iRole = Qt::DisplayRole ) const
	{
		if( iRole == Qt::DisplayRole )
		{
			switch( index.row() )
			{
			case 0:
				return ( index.column() == 0 ? QString( "Number of frequencies" ) : QString::number( m_pContent->getNumFrequencies() ) );
			case 1:
				return ( index.column() == 0 ? QString( "Frequency range" ) : QString( "%1 Hz - %2 Hz" ).arg( m_pContent->getFrequencies().front() ).arg( m_pContent->getFrequencies().back() ) );
			case 2:
				return( index.column() == 0 ? QString( "Overall maximum magnitude" ) : QString::number( m_pContent->getOverallMagnitudeMaximum() ) );
			}
		}

		return DAFFContentModel::data( index, iRole );
	};

private:
	const DAFFContentMS* m_pContent;
};

class QDAFFContentMPSModel : public DAFFContentModel
{
public:
	inline QDAFFContentMPSModel( QObject* pParent, const DAFFContentMPS* pContent )
		: DAFFContentModel( pParent )
		, m_pContent( pContent )
	{
	}

	inline int rowCount( const QModelIndex& ) const
	{
		return 3;
	};

	inline QVariant data( const QModelIndex &index, int iRole = Qt::DisplayRole ) const
	{
		if( iRole == Qt::DisplayRole )
		{
			switch( index.row() )
			{
			case 0:
				return ( index.column() == 0 ? QString( "Number of frequencies" ) : QString::number( m_pContent->getNumFrequencies() ) );
			case 1:
				return ( index.column() == 0 ? QString( "Frequency range" ) : QString( "%1 Hz - %2 Hz" ).arg( m_pContent->getFrequencies().front() ).arg( m_pContent->getFrequencies().back() ) );
			case 2:
				return( index.column() == 0 ? QString( "Overall maximum magnitude" ) : QString::number( m_pContent->getOverallMagnitudeMaximum() ) );
			}
		}

		return DAFFContentModel::data( index, iRole );
	};

private:
	const DAFFContentMPS* m_pContent;
};


class QDAFFContentPSModel : public DAFFContentModel
{
public:
	inline QDAFFContentPSModel( QObject* pParent, const DAFFContentPS* pContent )
		: DAFFContentModel( pParent )
		, m_pContent( pContent )
	{
	}

	inline int rowCount( const QModelIndex& ) const
	{
		return 2;
	};

	inline QVariant data( const QModelIndex &index, int iRole = Qt::DisplayRole ) const
	{
		if( iRole == Qt::DisplayRole )
		{
			switch( index.row() )
			{
			case 0:
				return ( index.column() == 0 ? QString( "Number of frequencies" ) : QString::number( m_pContent->getNumFrequencies() ) );
			case 1:
				return ( index.column() == 0 ? QString( "Frequency range" ) : QString( "%1 Hz - %2 Hz" ).arg( m_pContent->getFrequencies().front() ).arg( m_pContent->getFrequencies().back() ) );
			}
		}

		return DAFFContentModel::data( index, iRole );
	};

private:
	const DAFFContentPS* m_pContent;
};

class QDAFFContentDFTModel : public DAFFContentModel
{
public:
	inline QDAFFContentDFTModel( QObject* pParent, const DAFFContentDFT* pContent )
		: DAFFContentModel( pParent )
		, m_pContent( pContent )
	{
	}

	inline int rowCount( const QModelIndex& ) const
	{
		return 6;
	};

	inline QVariant data( const QModelIndex &index, int iRole = Qt::DisplayRole ) const
	{
		if( iRole == Qt::DisplayRole )
		{
			switch( index.row() )
			{
			case 0:
				return ( index.column() == 0 ? QString( "Frequency bandwidth" ) : QString::number( m_pContent->getFrequencyBandwidth() ) );
			case 1:
				return ( index.column() == 0 ? QString( "DFT transform size" ) : QString::number( m_pContent->getTransformSize() ) );
			case 2:
				return ( index.column() == 0 ? QString( "Symmetrical DFT spectrum" ) : QString( m_pContent->isSymmetric() ? "yes" : "no" ) );
			case 3:
				return ( index.column() == 0 ? QString( "Sampling rate" ) : QString( "%1 Hz" ).arg( m_pContent->getSamplerate() ) );
			case 4:
				return ( index.column() == 0 ? QString( "Number of DFT coefficiants" ) : QString( "%1 Hz" ).arg( m_pContent->getNumDFTCoeffs() ) );
			case 5:
				return( index.column() == 0 ? QString( "Overall maximum magnitude" ) : QString::number( m_pContent->getOverallMagnitudeMaximum() ) );				
			}
		}

		return DAFFContentModel::data( index, iRole );
	};

private:
	const DAFFContentDFT* m_pContent;
};


class QDAFFContentTableView : public QTableView
{
    Q_OBJECT

public:
	inline QDAFFContentTableView( QWidget *parent = Q_NULLPTR )
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

		switch( pReader->getContentType() )
		{
		case DAFF_IMPULSE_RESPONSE:
		{
			// No const pointer because one of the methods uses lazy initialization (calculates member value on first call)
			DAFFContentIR* pContentIR = dynamic_cast< DAFFContentIR* >( pReader->getContent() );
			m_pModel = new QDAFFContentIRModel( this, pContentIR );
			break;

		}
		case DAFF_MAGNITUDE_SPECTRUM:
		{
			const DAFFContentMS* pContentMS = dynamic_cast< const DAFFContentMS* >( pReader->getContent() );
			m_pModel = new QDAFFContentMSModel( this, pContentMS );
			break;
		}
		case DAFF_MAGNITUDE_PHASE_SPECTRUM:
		{
			const DAFFContentMPS* pContent = dynamic_cast< const DAFFContentMPS* >( pReader->getContent() );
			//m_pModel = new QDAFFContentMPSModel( this, pContent );
			break;
		}
		case DAFF_PHASE_SPECTRUM:
		{
			const DAFFContentPS* pContentPS = dynamic_cast< const DAFFContentPS* >( pReader->getContent() );
			//m_pModel = new QDAFFContentPSModel( this, pContentMS );
			break;
		}
		case DAFF_DFT_SPECTRUM:
		{
			const DAFFContentDFT* pContent = dynamic_cast< const DAFFContentDFT* >( pReader->getContent() );
			//m_pModel = new QDAFFContentDFTModel( this, pContent );
			break;
		}
		}		
		
		setModel( m_pModel );
		verticalHeader()->hide();
		horizontalHeader()->resizeSections( QHeaderView::Stretch );
    }
private:
	DAFFContentModel*  m_pModel;
};

#endif // QDAFFCONTENTTABLE_H
