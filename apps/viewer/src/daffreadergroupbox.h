#ifndef DAFFREADERGROUPBOX_H
#define DAFFREADERGROUPBOX_H

#include <QGroupBox>
#include <DAFF.h>
#include <iostream>

class DAFFReaderGroupBox : public QGroupBox
{
public:
    inline DAFFReaderGroupBox(QWidget *parent = Q_NULLPTR)
        : QGroupBox(parent)
    {
        std::cout << "he" << std::endl;
    }

    inline DAFFReaderGroupBox(const QString &title, QWidget *parent = Q_NULLPTR)
        : QGroupBox( title, parent) {}

public slots:
    inline void on_readDAFF( const DAFFReader* pReader )
    {
        std::cout << pReader->toString() << std::endl;
    }
};

#endif // DAFFREADERGROUPBOX_H
