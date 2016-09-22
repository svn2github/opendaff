/*
 * -------------------------------------------------------------------------------------
 *
 *  OpenDAFF - A free, open source software package for directional audio data
 *  Copyright 2016 Institute of Technical Acoustics, RWTH Aachen University
 *  OpenDAFF is distributed under the Apache License Version 2.0.s
 *
 *  ------------------------------------------------------------------------------------
 *
 */

#ifndef QDAFF2DPLOT_H
#define QDAFF2DPLOT_H

#include <QFrame>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtOpenGL\qgl.h>
#include <QKeyEvent>
#include <QtSvg\qsvggenerator.h>

#include <DAFF.h>

#include <iostream>

class QInfoBox :public QGraphicsTextItem
{
public:
	inline QInfoBox(QString text) :QGraphicsTextItem(text){}

	inline QRectF boundingRect() const
	{
		return QGraphicsTextItem::boundingRect().adjusted(-2, -2, +2, +2);
	}

	inline void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
	{
		painter->setPen(Qt::black);
		painter->setBrush(Qt::white);
		painter->drawRect(boundingRect());
		painter->setBrush(Qt::black);
		QGraphicsTextItem::paint(painter, option, widget);
	}
};

class QGraphicsPoint : public QGraphicsItem
{
public:

	/*QGraphicsPoint(double x, double y, double diameter, QGraphicsSimpleTextItem* box) : QGraphicsEllipseItem(x, y, diameter, diameter)
	{
	m_pBox = box;
	setAcceptHoverEvents(true);
	}*/
	inline QGraphicsPoint(QString text, double x, double y, int diameter)
	{
		m_pBox = nullptr;
		m_oText = text;
		m_iDiameter = diameter;
		setPos(x, y);
		setAcceptHoverEvents(true);
	}


	inline ~QGraphicsPoint()
	{
		delete m_pBox;
	}

    inline void QGraphicsPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget* )
	{
		painter->setBrush(QBrush(Qt::black));
		painter->drawEllipse(-m_iDiameter / 2, -m_iDiameter / 2, m_iDiameter, m_iDiameter);
	}

	inline QRectF QGraphicsPoint::boundingRect() const
	{
		return QRectF(-m_iDiameter / 2, -m_iDiameter / 2, m_iDiameter, m_iDiameter);
	}

protected:

    inline void hoverEnterEvent( QGraphicsSceneHoverEvent* )
	{
		if (m_pBox == nullptr)
			m_pBox = new QInfoBox(m_oText);	
		m_pBox->setTransform(QTransform().scale(1, -1));
		m_pBox->setPos(pos().x() + boundingRect().right(), pos().y() + boundingRect().bottom() + m_pBox->boundingRect().bottom());
		scene()->addItem(m_pBox);
	}

    inline void hoverLeaveEvent(QGraphicsSceneHoverEvent* )
	{
		scene()->removeItem(m_pBox);
	}

private:
	QInfoBox* m_pBox;
	QString m_oText;
	int m_iDiameter;

};

class QDAFF2DPlot : public QGraphicsView
{
    Q_OBJECT

public:
	QDAFF2DPlot( QWidget *parent = Q_NULLPTR );
	void ExportImagePNG( const QString& sTargetFilePath, float fScaling = 1.0f, bool bShowAllChannels = false, bool bShowDataPointDots = true );
	void ExportImageSVG( const QString& sTargetFilePath, float fScaling = 1.0f, bool bShowAllChannels = false, bool bShowDataPointDots = true );

public slots:
     void ReadDAFF( const DAFFReader* pReader );
	 void CloseDAFF();
     void ChangeRecordIndex( int );
     void ChangeChannelIndex( int );
     void ChangeFrequencyIndex( int );

	 void SetAllChannelsVisible( bool bVisible );

private:
	void keyPressEvent(QKeyEvent * event);
	void keyReleaseEvent(QKeyEvent * event);
	void resizeEvent(QResizeEvent * event);
	void showEvent(QShowEvent * event);
	void wheelEvent(QWheelEvent * event);

	const DAFFReader* m_pReader;

	//help Functions
	void Draw( bool showAllChannels = true, bool showDots = true );
	void DrawCoordinateSystem();
	void DrawGraph(int);

	void ShowChannel( int iChannelIndex, bool bShowAllChannels = true, bool showDots = true );

	const std::string convertFloat(float);

	//items
	QGraphicsLineItem *m_pXAxis, *m_pYAxis;
	std::vector<QGraphicsLineItem*> m_vpXGrid, m_vpYGrid, m_vpXMarker, m_vpYMarker;
	std::vector<QGraphicsTextItem*> m_vpXMarkerText;
	std::vector<QGraphicsTextItem*> m_vpYMarkerText;
	std::vector<std::vector<QGraphicsLineItem*>> m_vvpGraphs;
	std::vector<std::vector<QGraphicsPoint*>> m_vvpPoints;
	std::vector<QColor> m_voColors;

	//variables
	int m_iXAxisLength;
	int m_iYAxisLength;
	int m_iSceneHeight = 0;
	int m_iSceneWidth = 0;
	int m_iSceneOriginalWidth = 0;
	int m_iSceneOriginalHeight = 0;

	//bool
	bool m_bScrollHorizontally = true;

	//indices
    int m_iChannelIndex;
    int m_iRecordIndex;
    int m_iFrequencyIndex;

	//parameters
	//axis
	const int m_iAxisOffsetRight = 30;
	const int m_iAxisOffsetUp = 30;
	const int m_iAxisOffsetLeft = 30; 
	const int m_iAxisOffsetDown = 30;
	const int m_iTipLength = 10;
	const int m_iTipWidth = 5; //from one end to the other is double this width
	const int m_iTipDistanceX = 50;
	const int m_iTipDistanceY = 50;
	//grid
	const int m_iGridXOffset = 60;
	const int m_iGridYOffset = 60;
	const int m_iNXGrid = 11;
	const int m_iNYGrid = 10;
	//marker
	const int m_iNXMarker = 12;
	const int m_iNYMarker = 11;
	const int m_iXMarkerLength = 5;
	const int m_iYMarkerLength = 5;
	const int m_iXMarkerTextOffset = 0;
	const int m_iYMarkerTextOffset = 0;
	//points
	const int m_iPointDiameter = 4;
	
};

#endif // QDAFF2DPLOT_H
