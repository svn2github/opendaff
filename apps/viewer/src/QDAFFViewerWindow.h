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

#ifndef QDAFFVIEWERWINDOW_H
#define QDAFFVIEWERWINDOW_H

#include <QMainWindow>
#include <QSettings>

class DAFFReader;
class DAFFContent;

namespace Ui {
class DAFFViewer;
}

class QDAFFViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit QDAFFViewerWindow( QWidget *parent = 0, QString sPath = "" );
    ~QDAFFViewerWindow();

    void OpenDAFFFile( QString sPath, bool bQuiet = false );
	

private slots:

    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
    void on_actionClose_triggered();
    void on_actionOpenDAFFWebsite_triggered();
    void on_actionCreate_triggered();
    void on_actionAboutOpenDAFF_triggered();
    void on_actionAboutDAFFViewer_triggered();
	void on_actionDownload_triggered();
	
	void IncreaseAlpha();
	void DecreaseAlpha();
	void IncreaseBeta();
	void DecreaseBeta();

	void IncreaseFrequencyIndex();
	void DecreaseFrequencyIndex();
	void ChangeFrequencyIndex( int );
	
	void IncreaseChannelIndex();
	void DecreaseChannelIndex();
	void ChangeChannelIndex( int );

	void SetDAFFObjectView();
	void SetDAFFDataView();

	void LoadContent( const DAFFContent* );

    void on_actionIncrease_Frequency_triggered();

    void on_actionDecrease_Frequency_triggered();

signals:
	void ReadDAFF( const DAFFReader* );
	void SignalContentLoaded( const DAFFContent* );
	void CloseDAFF();
	void ChangeAlpha( float fAlphaDegrees );
	void ChangeBeta( float fBetaDegrees );
	void SignalFrequencyIndexChanged( int iIndex );
	void SignalChannelIndexChanged( int iChannelIndex );

private:
    Ui::DAFFViewer *ui;
    QSettings m_qSettings;

    DAFFReader* m_pDAFFReader;

	float m_fShowAlphaDeg, m_fShowBetaDeg; //!< Data view angle
	float m_fShowPhiDeg, m_fShowTheta;  //!< Object view angle
	int m_iShowFrequencyIndex; //!< Frequency (bin) index
	float m_fShowTimeSample; //!< Sample of time series (FIR tap)
	int m_iShowChannel; //!< Requested channel
	int m_iShowDAFFView; //!< Show DAFF object or data view
};

#endif // QDAFFVIEWERWINDOW_H
