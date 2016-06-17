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

#ifndef QDAFFMETADATATABLE_H
#define QDAFFMETADATATABLE_H

#include <QTableView>
#include <QHeaderView>
#include <QColor>
#include <QFont>
#include <QAbstractTableModel>

#include <DAFF.h>

#include <assert.h>
#include <iostream>

class DAFFMetadataModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	inline DAFFMetadataModel( QObject* pParent, const DAFFMetadata* pMetadata ) 
		: QAbstractTableModel( pParent )
	, m_pMetadata( pMetadata ) 
	{
		// This is OK because we use a read-only view (no data sync required)
		m_pMetadata->getKeys( m_vsKeyList );
        m_iNumRows = int( m_vsKeyList.size() );
	};

    inline int rowCount( const QModelIndex& ) const
	{
		return m_iNumRows;
	};
    inline int columnCount( const QModelIndex& ) const
	{
		return 3;
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
				case 2:
					return QString( "Type" );
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
		assert( index.row() < m_iNumRows );
		const std::string& sKey = m_vsKeyList[ index.row() ];

		if( iRole == Qt::DisplayRole )
		{
			if( index.column() == 0 )
				return QString::fromStdString( sKey );

			if( index.column() == 1 )
			{
				switch( m_pMetadata->getKeyType( sKey ) )
				{
				case DAFFMetadata::DAFF_STRING:
					return QString::fromStdString( m_pMetadata->getKeyString( sKey ) );
				case DAFFMetadata::DAFF_BOOL:
					return QString::fromStdString( m_pMetadata->getKeyBool( sKey ) ? "True" : "False" );
				case DAFFMetadata::DAFF_FLOAT:
				case DAFFMetadata::DAFF_INT:
					return QString::number( m_pMetadata->getKeyFloat( sKey ) );
				}
			}

			if( index.column() == 2 )
				return QString::fromStdString( DAFFMetadata::GetKeyTypeString( m_pMetadata->getKeyType( sKey ) ) );
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
	const DAFFMetadata* m_pMetadata;
	std::vector< std::string > m_vsKeyList;
	int m_iNumRows;
};


class QDAFFMetadataTableView : public QTableView
{
    Q_OBJECT

public:
	inline QDAFFMetadataTableView( QWidget *parent = Q_NULLPTR )
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
		if( pReader == nullptr )
			return;

		m_pModel = new DAFFMetadataModel( this, pReader->getMetadata()  );
		this->setModel( m_pModel );
		this->verticalHeader()->hide();
        this->horizontalHeader()->resizeSections( QHeaderView::ResizeToContents );
    }
private:
	DAFFMetadataModel*  m_pModel;
};

#endif // QDAFFMETADATATABLE_H
