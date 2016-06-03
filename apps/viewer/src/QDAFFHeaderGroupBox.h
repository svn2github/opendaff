#ifndef DAFFREADERGROUPBOX_H
#define DAFFREADERGROUPBOX_H

#include <QGroupBox>
#include <QLineEdit>
#include <QFileInfo>
#include <DAFF.h>
#include <iostream>

class DAFFReaderGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    inline DAFFReaderGroupBox(QWidget *parent = Q_NULLPTR)
        : QGroupBox(parent)
    {

    }

    inline DAFFReaderGroupBox(const QString &title, QWidget *parent = Q_NULLPTR)
        : QGroupBox( title, parent) {}

public slots:
    inline void on_readDAFF( const DAFFReader* pReader )
    {
        std::cout << pReader->toString() << std::endl;

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
           pLE->setText( QString::fromStdString( DAFFUtils::StrShortContentType( pReader->getContentType() ) ) );
           pLE->setToolTip( QString::fromStdString( DAFFUtils::StrContentType( pReader->getContentType() ) ) );
       }
    }
};

#endif // DAFFREADERGROUPBOX_H
