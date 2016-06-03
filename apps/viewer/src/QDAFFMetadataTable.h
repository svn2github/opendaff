#ifndef DAFFMetadataTableView_H
#define DAFFMetadataTableView_H

#include <QTableView>
#include <DAFF.h>
#include <iostream>

class DAFFMetadataTableView : public QTableView
{
    Q_OBJECT

public:
	inline DAFFMetadataTableView( QWidget *parent = Q_NULLPTR )
                : QTableView( parent )
    {
    }

public slots:
    inline void on_readDAFF( const DAFFReader* pReader )
    {

    }
};

#endif // DAFFMetadataTableView_H
