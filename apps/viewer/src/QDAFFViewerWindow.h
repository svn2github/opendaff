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
	void ChangeAlpha( double );

    void IncreaseBeta();
	void DecreaseBeta();
	void ChangeBeta( double );

    void IncreasePhi() {};
	void DecreasePhi() {};
	void ChangePhi( double ) {};

    void IncreaseTheta() {};
	void DecreaseTheta() {};
	void ChangeTheta( double ) {};

	void IncreaseFrequencyIndex();
	void DecreaseFrequencyIndex();
	void ChangeFrequencyIndex( int );
	
	void IncreaseChannelIndex();
	void DecreaseChannelIndex();
	void ChangeChannelIndex( int );

	void SetDAFFObjectView();
	void SetDAFFDataView();

	void LoadContent( const DAFFContent* );

	void ChangeRecordIndex( int iRecordIndex );

    void on_actionIncrease_Frequency_triggered();
    void on_actionDecrease_Frequency_triggered();
    void on_actionIncrease_channel_triggered();
    void on_actionDecrease_channel_triggered();
    void on_actionIncrease_alpha_triggered();
    void on_actionDecrease_alpha_triggered();
    void on_actionIncrease_beta_triggered();
	void on_actionDecrease_beta_triggered();
	void on_actionIncrease_phi_triggered();
	void on_actionDecrease_phi_triggered();
	void on_actionIncrease_theta_triggered();
	void on_actionDecrease_theta_triggered();

signals:
	void SignalReadDAFF( const DAFFReader* pReader );
	void SignalContentLoaded( const DAFFContent* pContent );
	void SignalCloseDAFF();
    void SignalAlphaChanged( double dAlphaDegree );
    void SignalBetaChanged( double dBetaDegree );
	void SignalRecordIndexChanged( int iRecordIndex );
	void SignalFrequencyIndexChanged( int iIndex );
	void SignalChannelIndexChanged( int iChannelIndex );

private:
    Ui::DAFFViewer *ui;
    QSettings m_qSettings;

    DAFFReader* m_pDAFFReader;

	double m_dShowAlphaDeg, m_dShowBetaDeg; //!< Data view angle
	double m_dShowPhiDeg, m_dShowTheta;  //!< Object view angle
	int m_iShowFrequencyIndex; //!< Frequency (bin) index
	double m_dShowTimeSample; //!< Sample of time series (FIR tap)
	int m_iShowChannel; //!< Requested channel
	int m_iShowDAFFView; //!< Show DAFF object or data view
	int m_iShowRecordIndex; //!< Show DAFF record index / data of file
};

#endif // QDAFFVIEWERWINDOW_H
