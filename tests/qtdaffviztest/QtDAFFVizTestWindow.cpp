#include "QtDAFFVizTestWindow.h"
#include "ui_DAFFVizQtTestWindow.h"

#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkSmartPointer.h>


QtDAFFVizTestWindow::QtDAFFVizTestWindow( QWidget *parent ) :
    QMainWindow(parent),
	ui( new Ui::QtDAFFVizTestWindow )
{
    ui->setupUi(this);
	node = new DAFFViz::SGNode();
	nodeCarpet = new DAFFViz::SGNode();
	renderer = vtkSmartPointer<vtkRenderer>::New();

	//spherical coordinates
	//node->OnSetFollowerCamera(renderer->GetActiveCamera());
	spherical = new DAFFViz::SphericalCoordinateAssistant();
	node->AddChildNode(spherical);
	
	////grid
	//grid = new DAFFViz::Grid();
	//nodeCarpet->AddChildNode(grid);

	////label
	//label = new DAFFViz::Label();
	//nodeCarpet->AddChildNode(label);

	////line not showing
	//line = new DAFFViz::Line(0, 0, 0, 1, 1, 1);
	//nodeCarpet->AddChildNode(line);

	////arrow not showing
	//carpetXArrow = new DAFFViz::Arrow(10, 1, 100, 1, 100);
	//carpetXArrow->SetVisible(true);
	//nodeCarpet->AddChildNode(carpetXArrow);

	//cartesian coordinates not showing and hiding label and grid
	cartesian = new DAFFViz::CartesianCoordinateAssistant();
	nodeCarpet->AddChildNode(cartesian);

	//this->ui->vtkWidgetBalloon->GetRenderWindow->SetRenderer( m_pRenderer );

	renderer->AddActor(node->GetNodeAssembly());
	camera = renderer->GetActiveCamera();
	camera->SetPosition(0, 0, 5);
	//node->OnSetFollowerCamera(camera);
	this->ui->vtkWidgetBalloon->GetRenderWindow()->AddRenderer(renderer);
	this->ui->lineEditFrequency->setText(0);
	this->ui->lineEditPhi->setText(0);
	this->ui->lineEditTheta->setText(0);

	//2D Plot
	view = vtkSmartPointer<vtkContextView>::New();
	view->SetInteractor(this->ui->vtkWidgetPlot->GetInteractor());
	this->ui->vtkWidgetPlot->SetRenderWindow(view->GetRenderWindow());
	table = vtkSmartPointer<vtkTable>::New();
	dataX = vtkSmartPointer<vtkFloatArray>::New();
	dataX->SetName("Frequency/Hz");
	table->AddColumn(dataX);
	dataY = vtkSmartPointer<vtkFloatArray>::New();
	dataY->SetName("Magnitude");
	table->AddColumn(dataY);
}

QtDAFFVizTestWindow::~QtDAFFVizTestWindow()
{
	delete ui, reader, node, balloonPlot;
}

void QtDAFFVizTestWindow::on_actionQuit_triggered()
{
    close();
}

void QtDAFFVizTestWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Daff Files"), "C:/", tr("Daff Files (*.daff)"));
	reader = DAFFReader::create();
	reader->openFile(fileName.toStdString());
	renderer->Clear();
	int contentType = reader->getContentType();

	if (contentType == DAFF_IMPULSE_RESPONSE)
	{
		carpetPlot = new DAFFViz::CarpetPlot(dynamic_cast<DAFFContentIR*>(reader->getContent()));
		nodeCarpet->AddChildNode(carpetPlot);
		//rendererPlot->AddActor(nodeCarpet->GetNodeAssembly());
	}
	else
	{
		balloonPlot = new DAFFViz::BalloonPlot( node, reader->getContent() );
		//renderer->AddActor(node->GetNodeAssembly());
		ui->textBrowser_2->append("current frequency index: ");
		ui->textBrowser_2->append(QString::fromStdString(std::to_string(balloonPlot->GetSelectedFrequency())));

		drawGraph();
	}
	ui->textFileInfo->clear();
	ui->textFileInfo->append("opened File ");
	ui->textFileInfo->append(fileName);
	ui->textFileInfo->append("\n Content Type: ");
	ui->textFileInfo->append(QString::fromStdString(std::to_string(contentType)));
}

void QtDAFFVizTestWindow::on_lineEditFrequency_returnPressed()
{
	balloonPlot->SetSelectedFrequency(ui->lineEditFrequency->text().toInt());
	this->ui->vtkWidgetBalloon->update();
	/*ui->textBrowser_2->clear();
	ui->textBrowser_2->append("\n current frequency index: ");
	ui->textBrowser_2->append(QString::fromStdString(std::to_string(balloonPlot->GetSelectedFrequency())));*/
}

void QtDAFFVizTestWindow::on_lineEditPhi_returnPressed()
{
	view->GetScene()->ClearItems();
	drawGraph();
}
void QtDAFFVizTestWindow::on_lineEditTheta_returnPressed()
{
	view->GetScene()->ClearItems();
	drawGraph();
}

void QtDAFFVizTestWindow::drawGraph()
{
	std::vector<float> frequencies = dynamic_cast<const DAFFContentMS*>(reader->getContent())->getFrequencies();
	float phi = ui->lineEditPhi->text().toFloat();
	float theta = ui->lineEditTheta->text().toFloat();
	float magnitude;

	int alphaPoints = dynamic_cast<const DAFFContentMS*>(reader->getContent())->getProperties()->getAlphaPoints();
	float fAlpha = ui->lineEditPhi->text().toFloat();
	float fBeta = ui->lineEditTheta->text().toFloat();
	float alphaStart = dynamic_cast<const DAFFContentMS*>(reader->getContent())->getProperties()->getAlphaStart();
	float betaStart = dynamic_cast<const DAFFContentMS*>(reader->getContent())->getProperties()->getBetaStart();
	float alphaResolution = dynamic_cast<const DAFFContentMS*>(reader->getContent())->getProperties()->getAlphaResolution();
	float betaResolution = dynamic_cast<const DAFFContentMS*>(reader->getContent())->getProperties()->getBetaResolution();
	int recordIndex;
	if (betaStart == 0.0f)
	{
		int iBeta = fBeta / betaResolution;
		if (iBeta == 0)
		{
			recordIndex = 0;
		}
		else
		{
			int iAlpha = (fAlpha - alphaStart) / alphaResolution;
			recordIndex = (iBeta-1)*alphaPoints + iAlpha + 1;
		}		
	}
	else
	{
		int iAlpha = (fAlpha - alphaStart) / alphaResolution;
		int iBeta = (fBeta-betaStart) / betaResolution;
		recordIndex = iBeta*alphaPoints + iAlpha;
	}
	
	table->SetNumberOfRows(frequencies.size());

	for (int i = 0; i < table->GetNumberOfRows(); i++)
	{
		table->SetValue(i, 0, frequencies[i]);
		dynamic_cast<const DAFFContentMS*>(reader->getContent())->getMagnitude(recordIndex, 0, i, magnitude);
		table->SetValue(i, 1, magnitude);
	}
	chart = vtkSmartPointer<vtkChartXY>::New();
	view->GetScene()->AddItem(chart);
	vtkSmartPointer< vtkPlot > line = chart->AddPlot( vtkChart::LINE );
	line->SetInputData(table, 0, 1);
	line->SetColor(255, 0, 0, 255);
	fAlpha = 0;
	fBeta = 0;
	dynamic_cast<const DAFFContentMS*>(reader->getContent())->getRecordCoords(recordIndex, 0, fAlpha, fBeta);
	ui->textBrowser_2->clear();
	ui->textBrowser_2->append("phi: ");
	ui->textBrowser_2->append(QString::fromStdString(std::to_string(fAlpha)));
	ui->textBrowser_2->append("\n theta: ");
	ui->textBrowser_2->append(QString::fromStdString(std::to_string(fBeta)));
}
