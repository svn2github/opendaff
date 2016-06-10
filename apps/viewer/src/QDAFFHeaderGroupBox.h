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

#ifndef QDAFFHEADERGROUPBOX_H
#define QDAFFHEADERGROUPBOX_H

#include <QGroupBox>
#include <QLineEdit>
#include <QFileInfo>

#include <DAFF.h>

#include <iostream>

class QDAFFHeaderGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    inline QDAFFHeaderGroupBox(QWidget *parent = Q_NULLPTR)
        : QGroupBox(parent)
    {

    }

    inline QDAFFHeaderGroupBox(const QString &title, QWidget *parent = Q_NULLPTR)
        : QGroupBox( title, parent) {}

public slots:
inline void on_closeDAFF()
{
	QLineEdit* pLE = this->findChild<QLineEdit*>( "lineEditFileName" );
	if( pLE )
	{
		pLE->setText( "" );
		pLE->setToolTip( "" );
	}

	pLE = this->findChild<QLineEdit*>( "lineEditVersion" );
	if( pLE )
		pLE->setText( "" );

	pLE = this->findChild<QLineEdit*>( "lineEditContentType" );
	if( pLE )
	{
		pLE->setText( "" );
		pLE->setToolTip( "" );
	}
}
    inline void on_readDAFF( const DAFFReader* pReader )
    {
       QLineEdit* pLE = this->findChild<QLineEdit*>( "lineEditFileName" );
       if( pLE )
       {
           QFileInfo oFile( QString::fromStdString( pReader->getFilename() ) );
           pLE->setText( oFile.fileName() );
           pLE->setToolTip( oFile.absoluteFilePath() );
       }

       pLE = this->findChild<QLineEdit*>( "lineEditVersion" );
       if( pLE )
           pLE->setText( QString::number( pReader->getFileFormatVersion() ) );

       pLE = this->findChild<QLineEdit*>( "lineEditContentType" );
       if( pLE )
       {
		   pLE->setText( QString::fromStdString( DAFFUtils::StrContentType( pReader->getContentType() ) ) );
           pLE->setToolTip( QString::fromStdString( DAFFUtils::StrShortContentType( pReader->getContentType() ) ) );
       }
    }
};

#endif // QDAFFHEADERGROUPBOX_H
