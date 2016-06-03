#ifndef QDAFFMETADATATABLE_H
#define QDAFFMETADATATABLE_H

#include <QTableView>

#include <DAFF.h>

#include <iostream>

class QDAFFMetadataTableView : public QTableView
{
    Q_OBJECT

public:
	inline QDAFFMetadataTableView( QWidget *parent = Q_NULLPTR )
                : QTableView( parent )
    {
    }

public slots:
    inline void on_readDAFF( const DAFFReader* pReader )
    {
        std::cout << "Metadata reder: " << pReader->getFilename() << std::endl;
    }
};

#endif // QDAFFMETADATATABLE_H
