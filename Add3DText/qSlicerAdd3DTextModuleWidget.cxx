/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

//	MRML includes
#include <vtkMRMLModelDisplayNode.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLModelStorageNode.h>
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLViewNode.h>

// Qt includes
#include <QDebug>

// SlicerQt includes
#include <qMRMLThreeDView.h>
#include <qMRMLThreeDWidget.h>
#include <qMRMLThreeDViewControllerWidget.h>
#include <qSlicerApplication.h>
#include <qSlicerIOManager.h>
#include <qSlicerLayoutManager.h>
#include "qSlicerAdd3DTextModuleWidget.h"
#include "ui_qSlicerAdd3DTextModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerAdd3DTextModuleWidgetPrivate: public Ui_qSlicerAdd3DTextModuleWidget
{
public:
  qSlicerAdd3DTextModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qSlicerAdd3DTextModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerAdd3DTextModuleWidgetPrivate::qSlicerAdd3DTextModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerAdd3DTextModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerAdd3DTextModuleWidget::qSlicerAdd3DTextModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerAdd3DTextModuleWidgetPrivate )
{
	//  return the reference to the renderWindow of the Slicer application singleton 
	//  and those to its renderer and renderWindowInteractor
	this->app = qSlicerApplication::application();
	qSlicerLayoutManager *layoutManager = this->app->layoutManager();
	qMRMLThreeDWidget *threeDWidget = layoutManager->threeDWidget(0);
	qMRMLThreeDView *threeDView = threeDWidget->threeDView();
	this->renderWindow = threeDView->renderWindow();
	this->renderer = this->renderWindow->GetRenderers()->GetFirstRenderer();
	this->iren = this->renderWindow->GetInteractor();

}

//-----------------------------------------------------------------------------
qSlicerAdd3DTextModuleWidget::~qSlicerAdd3DTextModuleWidget()
{
	//to do:

}

//-----------------------------------------------------------------------------
void qSlicerAdd3DTextModuleWidget::setup()
{
  Q_D(qSlicerAdd3DTextModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  //Initiatilize Variance
  this->polydataResult = vtkSmartPointer<vtkPolyData>::New();
  this->polydataMain = vtkSmartPointer<vtkPolyData>::New();
  this->polydataText = vtkSmartPointer<vtkPolyData>::New();
  this->textActor = vtkSmartPointer<vtkActor>::New();
  this->CellPicker = vtkSmartPointer<vtkCellPicker>::New();
  this->isTextSet = false;

  m_vtkConnection = vtkSmartPointer<vtkEventQtSlotConnect>::New();

  d->MRMLNodeComboBox->addNode();

  //Connect slots
  connect(d->buttonSetText, SIGNAL(clicked()), this, SLOT(buttonSetTextClicked()));
  connect(d->buttonLocateText, SIGNAL(clicked()), this, SLOT(buttonLocateText()));
  connect(d->buttonTextOK, SIGNAL(clicked()), this, SLOT(buttonTextOKClicked()));
  connect(d->buttonTextCancel, SIGNAL(clicked()), this, SLOT(buttonTextCancelClicked()));
  connect(d->sliderTextSize, SIGNAL(valueChanged(int)), this, SLOT(sliderTextSizeChanged()));
}

//Set text widget as a reference
void qSlicerAdd3DTextModuleWidget::buttonSetTextClicked()
{
	Q_D(qSlicerAdd3DTextModuleWidget);

	//	obtain the source model
	vtkMRMLScene *mrmlScene = this->app->mrmlScene();

	QString nodeID;
	char *charNodeID;
	nodeID = d->MRMLNodeComboBox->currentNodeId();
	charNodeID = nodeID.toLatin1().data();
	if (nodeID == "")
	{
		QMessageBox::information(this,
			tr("Warning"),
			tr("Please select a model!"));
		return;
	} 
	else
	{
		vtkMRMLNode *sourceNode = mrmlScene->GetNodeByID(charNodeID);
		vtkSmartPointer<vtkMRMLModelNode> sourceModel =
			vtkSmartPointer<vtkMRMLModelNode>::New();
		sourceModel->Copy(sourceNode);

		//Compute normals of the model
		vtkSmartPointer<vtkPolyDataNormals>normFilter =
			vtkSmartPointer<vtkPolyDataNormals>::New();
		normFilter->SetInputData(sourceModel->GetPolyData());
		normFilter->SetComputeCellNormals(1);
		normFilter->SetComputePointNormals(1);
		normFilter->Update();

		this->polydataResult->DeepCopy(normFilter->GetOutput());
		this->polydataMain->DeepCopy(normFilter->GetOutput());
	}
	
	//Obtain text from textEdit
	d->textEdit->selectAll();
	QString textEntered;
	textEntered = d->textEdit->toPlainText();
	if (textEntered.isEmpty())
	{
		QMessageBox::information(this,
			tr("Warning"),
			tr("Please enter the text!"));
		return;
	}

	d->textEdit->setDisabled(1);
	QByteArray tempText = textEntered.toLocal8Bit();
	const char *textChar = tempText.data();

	this->textSource = vtkSmartPointer<vtkVectorText>::New();
	this->textSource->SetText(textChar);

	//Set TextWidget as a reference
	vtkSmartPointer<vtkTextProperty> textProperty = vtkSmartPointer<vtkTextProperty>::New();
	textProperty->SetColor(1.0, 0.0, 0.0);
	textProperty->SetFontSize(48);
	textProperty->SetJustificationToCentered();
	textProperty->SetVerticalJustificationToCentered();
	textProperty->SetFontFamilyToTimes();

	vtkSmartPointer<vtkTextActor> textActor = vtkSmartPointer<vtkTextActor>::New();
	textActor->SetInput(this->textSource->GetText());
	textActor->SetTextProperty(textProperty);

	vtkSmartPointer<vtkTextRepresentation> textRepresentation =
		vtkSmartPointer<vtkTextRepresentation>::New();
	//	textRepresentation->GetPositionCoordinate()->SetValue(0.0, 0.0);
	textRepresentation->GetBorderProperty()->SetColor(0.9300, 0.5700, 0.1300);

	this->textWidget = vtkSmartPointer<vtkTextWidget>::New();
	this->textWidget->ProcessEventsOn();
	this->textWidget->SetRepresentation(textRepresentation);
	this->textWidget->SetInteractor(this->iren);
	this->textWidget->SetTextActor(textActor);
	this->textWidget->SetResizable(0);
	this->textWidget->SelectableOff();
	this->textWidget->SetEnabled(1);
	this->textWidget->On();

//	this->renderer->ResetCamera();
	this->renderWindow->Render();

	d->buttonSetText->setDisabled(true);
	d->buttonLocateText->setEnabled(true);
	d->buttonTextCancel->setEnabled(true);
}

//Locate the text
void qSlicerAdd3DTextModuleWidget::buttonLocateText()
{
	Q_D(qSlicerAdd3DTextModuleWidget);

	d->buttonLocateText->setEnabled(false);

	this->m_vtkConnection->Connect(this->renderWindow->GetInteractor(),
		vtkCommand::LeftButtonPressEvent,
		this,
		SLOT(pointPickEvent()));

	this->m_vtkConnection->Connect(this->renderWindow->GetInteractor(),
		vtkCommand::RightButtonPressEvent,
		this,
		SLOT(pointPickCancel()));
}

//Carry out point pick event
void qSlicerAdd3DTextModuleWidget::pointPickEvent()
{
	Q_D(qSlicerAdd3DTextModuleWidget);

	//pick a cell from the model
	int* clickPos = this->iren->GetEventPosition();
	this->CellPicker->Pick(clickPos[0], clickPos[1], 0, this->renderer);
	double*PickPos = this->CellPicker->GetPickPosition();
	vtkIdType pickID = this->CellPicker->GetCellId();

	if (pickID <= -1)
	{
		QMessageBox::information(this,
			tr("Warning"),
			tr("Please click on the model!"));
		d->sliderTextSize->setDisabled(true);
	}
	else
	{
		if (this->isTextSet)
		{
			this->renderer->RemoveActor(this->textActor);
			this->renderWindow->Render();
		}

		d->sliderTextSize->setEnabled(true);
		double TextScale = d->sliderTextSize->value() / 10.0;

		vtkSmartPointer<vtkFloatArray> arrayNormals =
			vtkFloatArray::SafeDownCast(this->polydataMain->GetCellData()->GetNormals());
		double* pickNormal;
		pickNormal = arrayNormals->GetTuple(pickID);

		this->TextWidToPoint(PickPos[0], PickPos[1], PickPos[2]);
		this->textWidget->SetEnabled(0);
		this->textWidget->Off();
		this->renderWindow->Render();

		double cameraPosition[3];
		double oriFocalPoint[3];
		this->renderer->GetActiveCamera()->GetPosition(cameraPosition);
		this->renderer->GetActiveCamera()->GetFocalPoint(oriFocalPoint);

		this->CameraFaceToPoint(PickPos[0], PickPos[1], PickPos[2], pickNormal[0], pickNormal[1], pickNormal[2]);

		double ViewPos[3];
		this->renderer->SetWorldPoint(PickPos[0], PickPos[1], PickPos[2], 1.0);
		this->renderer->WorldToView();
		this->renderer->GetViewPoint(ViewPos);

		this->renderer->SetViewPoint(ViewPos[0] + 0.1, ViewPos[1], 0);
		this->renderer->ViewToWorld();
		this->renderer->GetWorldPoint(this->coordWorld1);
		this->renderer->SetViewPoint(ViewPos[0] - 0.1, ViewPos[1], 0);
		this->renderer->ViewToWorld();
		this->renderer->GetWorldPoint(this->coordWorld2);
		this->renderer->SetViewPoint(ViewPos[0], ViewPos[1], 0);
		this->renderer->ViewToWorld();
		this->renderer->GetWorldPoint(this->coordWorldC);
		for (int i = 0; i < 3; i++)
		{
			this->targetVector[i] = this->coordWorld1[i] - this->coordWorld2[i];
		}
		this->renderer->GetActiveCamera()->GetViewPlaneNormal(this->ViewPlaneNormal);

		this->transformText = this->TransformText(this->ViewPlaneNormal, this->targetVector, this->coordWorldC, TextScale);
		this->polydataText = this->ProjectText(this->transformText->GetOutput());

		this->renderer->GetActiveCamera()->SetPosition(cameraPosition);
		this->renderer->GetActiveCamera()->SetFocalPoint(oriFocalPoint);

		//set mapper & actor
		vtkSmartPointer<vtkPolyDataMapper>mapperText = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapperText->SetInputData(this->polydataText);
		this->textActor->SetMapper(mapperText);
		this->textActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

		this->renderer->AddActor(this->textActor);
		this->renderWindow->Render();

		this->isTextSet = true;
		d->sliderTextSize->setEnabled(true);
		d->buttonTextOK->setEnabled(true);
	}

	d->buttonLocateText->setEnabled(true);

	this->m_vtkConnection->Disconnect(this->renderWindow->GetInteractor(),
		vtkCommand::LeftButtonPressEvent,
		this,
		SLOT(pointPickEvent()));
	this->m_vtkConnection->Disconnect(this->renderWindow->GetInteractor(),
		vtkCommand::RightButtonPressEvent,
		this,
		SLOT(pointPickCancel()));
}

//Cancel point pick event
void qSlicerAdd3DTextModuleWidget::pointPickCancel()
{
	Q_D(qSlicerAdd3DTextModuleWidget);

	d->buttonLocateText->setEnabled(true);

	this->m_vtkConnection->Disconnect(this->renderWindow->GetInteractor(),
		vtkCommand::LeftButtonPressEvent,
		this,
		SLOT(pointPickEvent()));
	this->m_vtkConnection->Disconnect(this->renderWindow->GetInteractor(),
		vtkCommand::RightButtonPressEvent,
		this,
		SLOT(pointPickCancel()));
}

//Confirm operation, combine 3D text and  target model
void qSlicerAdd3DTextModuleWidget::buttonTextOKClicked()
{
	Q_D(qSlicerAdd3DTextModuleWidget);

	if (this->isTextSet)
	{
		this->renderer->RemoveActor(this->textActor);
		this->renderWindow->Render();
	}
	this->isTextSet = false;

	this->polydataResult = this->CombinePolyData(this->polydataResult, this->polydataText);

	//	display and store the result model
	vtkMRMLScene *mrmlScene = app->mrmlScene();
	vtkSmartPointer<vtkMRMLModelNode> resultModel =
		vtkSmartPointer<vtkMRMLModelNode>::New();
	QString resultName = d->MRMLNodeComboBox->currentNodeID();
	resultName.append("_text");
	resultModel->SetName(resultName.toLatin1().data());
	resultModel->SetAndObservePolyData(polydataResult);
	resultModel->SetScene(mrmlScene);

	vtkSmartPointer<vtkMRMLModelDisplayNode> resultDisplay =
		vtkSmartPointer<vtkMRMLModelDisplayNode>::New();
	vtkSmartPointer<vtkMRMLModelStorageNode> resultStorage =
		vtkSmartPointer<vtkMRMLModelStorageNode>::New();
	resultDisplay->SetScene(mrmlScene);
	resultStorage->SetScene(mrmlScene);
	resultDisplay->SetInputPolyDataConnection(resultModel->GetPolyDataConnection());
//	resultDisplay->SetColor(0.0, 1.0, 0.0);
	resultStorage->SetFileName(resultName.toLatin1().data());
	mrmlScene->AddNode(resultDisplay);
	mrmlScene->AddNode(resultStorage);
	resultModel->SetAndObserveDisplayNodeID(resultDisplay->GetID());
	resultModel->SetAndObserveStorageNodeID(resultStorage->GetID());
	mrmlScene->AddNode(resultModel);

	d->textEdit->setEnabled(true);
	d->buttonSetText->setEnabled(true);
	d->buttonLocateText->setDisabled(true);
	d->sliderTextSize->setDisabled(true);
	d->buttonTextOK->setDisabled(true);
	d->buttonTextCancel->setDisabled(true);
}

//Cancel operation
void qSlicerAdd3DTextModuleWidget::buttonTextCancelClicked()
{
	Q_D(qSlicerAdd3DTextModuleWidget);

	if (this->isTextSet)
	{
		this->renderer->RemoveActor(this->textActor);
		this->renderer->Render();
	}
	this->isTextSet = false;

	this->textWidget->SetEnabled(0);
	this->textWidget->Off();
	this->renderWindow->Render();

	d->textEdit->setEnabled(true);
	d->buttonSetText->setEnabled(true);
	d->buttonLocateText->setDisabled(true);
	d->sliderTextSize->setDisabled(true);
	d->buttonTextOK->setDisabled(true);
	d->buttonTextCancel->setDisabled(true);
}

//Resize the text
void qSlicerAdd3DTextModuleWidget::sliderTextSizeChanged()
{
	Q_D(qSlicerAdd3DTextModuleWidget);

	this->renderer->RemoveActor(this->textActor);
	this->renderWindow->Render();

	double*PickPos = this->CellPicker->GetPickPosition();
	vtkIdType pickID = this->CellPicker->GetCellId();

	double TextScale = d->sliderTextSize->value() / 10.0;

	vtkSmartPointer<vtkFloatArray> arrayNormals =
		vtkFloatArray::SafeDownCast(this->polydataMain->GetCellData()->GetNormals());
	double* pickNormal;
	pickNormal = arrayNormals->GetTuple(pickID);

	double cameraPosition[3];
	double oriFocalPoint[3];
	this->renderer->GetActiveCamera()->GetPosition(cameraPosition);
	this->renderer->GetActiveCamera()->GetFocalPoint(oriFocalPoint);

	this->CameraFaceToPoint(PickPos[0], PickPos[1], PickPos[2], pickNormal[0], pickNormal[1], pickNormal[2]);

	this->transformText = this->TransformText(this->ViewPlaneNormal, this->targetVector, this->coordWorldC, TextScale);
	this->polydataText = this->ProjectText(this->transformText->GetOutput());

	this->renderer->GetActiveCamera()->SetPosition(cameraPosition);
	this->renderer->GetActiveCamera()->SetFocalPoint(oriFocalPoint);

	//set mapper & actor
	vtkSmartPointer<vtkPolyDataMapper>mapperText = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapperText->SetInputData(this->polydataText);
	this->textActor->SetMapper(mapperText);
	this->textActor->GetProperty()->SetColor(1.0, 0.0, 0.0);

	this->renderer->AddActor(this->textActor);
	this->renderWindow->Render();
}

//Make the camera face the picked point
void qSlicerAdd3DTextModuleWidget::
CameraFaceToPoint(double posX, double posY, double posZ, double normX, double normY, double normZ)
{
	this->renderer->GetActiveCamera()->SetFocalPoint(posX, posY, posZ);
	double dist = this->renderer->GetActiveCamera()->GetDistance();

	double absNormal;
	absNormal = sqrt(normX*normX + normY*normY + normZ*normZ);
	double unitNormal[3];
	unitNormal[0] = normX / absNormal;
	unitNormal[1] = normY / absNormal;
	unitNormal[2] = normZ / absNormal;

	double newPosition[3];
	newPosition[0] = posX + unitNormal[0] * dist;
	newPosition[1] = posY + unitNormal[1] * dist;
	newPosition[2] = posZ + unitNormal[2] * dist;

	this->renderer->GetActiveCamera()->SetPosition(newPosition);
//	this->renderWindow->Render();
}

//Move TextWidget to the point
void qSlicerAdd3DTextModuleWidget::TextWidToPoint(double posX, double posY, double posZ)
{
	double ViewPos[3];
	this->renderer->SetWorldPoint(posX, posY, posZ, 1.0);
	this->renderer->WorldToView();
	this->renderer->GetViewPoint(ViewPos);

	double* coordView1;
	double* coordView2;
	coordView1 = this->textWidget->GetBorderRepresentation()->GetPosition();
	coordView2 = this->textWidget->GetBorderRepresentation()->GetPosition2();

	double newViewPos[2];
	newViewPos[0] = (ViewPos[0] + 1.0) / 2.0 - coordView2[0] / 2.0;
	newViewPos[1] = (ViewPos[1] + 1.0) / 2.0 - coordView2[1] / 2.0;

	this->textWidget->GetBorderRepresentation()->SetPosition(newViewPos);
	this->renderWindow->Render();
}

//Transform VectorText to the point
vtkSmartPointer<vtkTransformPolyDataFilter> qSlicerAdd3DTextModuleWidget::
TransformText(double Vector1[3], double Vector2[3], double Coord[3], double TextScale)
{
	vtkSmartPointer<vtkTransform> trans = vtkSmartPointer<vtkTransform>::New();
	trans->PostMultiply();

	//First rotation
	double* ParaRot1;
	double coord1[3] = { 0.0, 0.0, 0.1 };
	ParaRot1 = this->ComputeCrossMultiply(coord1, Vector1);
	trans->RotateWXYZ(ParaRot1[0], ParaRot1[1], ParaRot1[2], ParaRot1[3]);

	//Second rotation
	vtkSmartPointer<vtkMatrix4x4> matrix1 = trans->GetMatrix();
	double textVector[3];
	for (int i = 0; i < 3; i++)
	{
		textVector[i] = matrix1->GetElement(i, 0);
	}
	double* ParaRot2;
	ParaRot2 = this->ComputeCrossMultiply(textVector, Vector2);
	trans->RotateWXYZ(ParaRot2[0], ParaRot2[1], ParaRot2[2], ParaRot2[3]);

	//Translate & Scale
	vtkSmartPointer<vtkMatrix4x4> matrix2 = trans->GetMatrix();
	double* textBounds = this->textSource->GetOutput()->GetBounds();
	double textCenterIN[4] = { 0.5*(textBounds[1] + textBounds[0]),
		0.5*(textBounds[3] + textBounds[2]),
		0.5*(textBounds[5] + textBounds[4]), 1.0 };
	double textCenterOUT[4];
	matrix2->MultiplyPoint(textCenterIN, textCenterOUT);

	trans->Translate(-textCenterOUT[0], -textCenterOUT[1], -textCenterOUT[2]);
	trans->Scale(TextScale, TextScale, TextScale);
	trans->Translate(Coord[0], Coord[1], Coord[2]);

	vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	transformFilter->SetTransform(trans);
	transformFilter->SetInputConnection(this->textSource->GetOutputPort());
	transformFilter->Update();

	//clean pointer
	delete[] ParaRot1;
	delete[] ParaRot2;

	return transformFilter;
}

//Compute cross multiply
double* qSlicerAdd3DTextModuleWidget::ComputeCrossMultiply(double Vector1[3], double Vector2[3])
{
	double rotVector[3];
	rotVector[0] = Vector1[1] * Vector2[2] - Vector1[2] * Vector2[1];
	rotVector[1] = Vector1[2] * Vector2[0] - Vector1[0] * Vector2[2];
	rotVector[2] = Vector1[0] * Vector2[1] - Vector1[1] * Vector2[0];
	double rotAngle;
	rotAngle = acos((Vector1[0] * Vector2[0] + Vector1[1] * Vector2[1] + Vector1[2] * Vector2[2]) /
		(sqrt(Vector1[0] * Vector1[0] + Vector1[1] * Vector1[1] + Vector1[2] * Vector1[2])*
		sqrt(Vector2[0] * Vector2[0] + Vector2[1] * Vector2[1] + Vector2[2] * Vector2[2])));
	rotAngle = rotAngle*180.0 / 3.1415926;

	double* ComputeResult = new double[4];

	if (abs(rotVector[0]) <= 0.00001 && abs(rotVector[1]) <= 0.00001 && abs(rotVector[2]) <= 0.00001)
	{
		ComputeResult[1] = 1.0;
		ComputeResult[2] = 0.0;
		ComputeResult[3] = 0.0;
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			ComputeResult[i + 1] = rotVector[i];
		}
	}
	ComputeResult[0] = rotAngle;

	return ComputeResult;
}

//Project the text to the model
vtkSmartPointer<vtkPolyData>qSlicerAdd3DTextModuleWidget::ProjectText(vtkPolyData* PolyText)
{
	Q_D(qSlicerAdd3DTextModuleWidget);

	vtkIdType numTextPoint = PolyText->GetNumberOfPoints();
	auto cellPicker = vtkSmartPointer<vtkCellPicker>::New();
// 	cellPicker->InitializePickList();
// 	cellPicker->AddPickList(mActor);
// 	cellPicker->PickFromListOn();


	vtkSmartPointer<vtkDoubleArray>pcoords = vtkSmartPointer<vtkDoubleArray>::New();
	pcoords->SetNumberOfComponents(3);
	pcoords->SetNumberOfTuples(numTextPoint);

	for (vtkIdType i = 0; i < numTextPoint; i++)
	{
		double* WorldPoint = PolyText->GetPoint(i);
		this->renderer->SetWorldPoint(WorldPoint[0], WorldPoint[1], WorldPoint[2], 0);
		this->renderer->WorldToDisplay();

		double ScreenPoint[3];
		this->renderer->GetDisplayPoint(ScreenPoint);

		cellPicker->Pick(ScreenPoint[0], ScreenPoint[1], 0, this->renderer);
		double *TempPoint = cellPicker->GetPickPosition();
		pcoords->SetTuple(i, TempPoint);
	}

	vtkSmartPointer<vtkPoints>TextPoints = vtkSmartPointer<vtkPoints>::New();
	TextPoints->SetData(pcoords);
	vtkSmartPointer<vtkPolyData> tempPolyText = vtkSmartPointer<vtkPolyData>::New();
	tempPolyText->DeepCopy(PolyText);
	tempPolyText->SetPoints(TextPoints);
	vtkSmartPointer<vtkPolyDataNormals> textNorm = vtkSmartPointer<vtkPolyDataNormals>::New();
	textNorm->SetInputData(tempPolyText);
	textNorm->SetComputePointNormals(1);
	textNorm->SetComputeCellNormals(1);
	textNorm->SetSplitting(1);
	textNorm->Update();
	vtkSmartPointer<vtkFloatArray> arrayNormals =
		vtkFloatArray::SafeDownCast(this->polydataMain->GetCellData()->GetNormals());
	vtkIdType pickID = this->CellPicker->GetCellId();
	double* pickNorm;
	pickNorm = arrayNormals->GetTuple(pickID);

	// Apply linear extrusion 
	vtkSmartPointer<vtkLinearExtrusionFilter> extrude =
		vtkSmartPointer<vtkLinearExtrusionFilter>::New();
	extrude->SetInputConnection(textNorm->GetOutputPort());
	extrude->CappingOn();
	extrude->SetExtrusionTypeToVectorExtrusion();
	extrude->SetVector(pickNorm[0], pickNorm[1], pickNorm[2]);
	extrude->SetScaleFactor(1.0);													//distance of extrusion
	extrude->Update();
	vtkSmartPointer<vtkTriangleFilter> triangleFilter =
		vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter->SetInputConnection(extrude->GetOutputPort());
	triangleFilter->Update();

	vtkSmartPointer<vtkPolyDataNormals>norm =
		vtkSmartPointer<vtkPolyDataNormals>::New();
	norm->SetInputData(triangleFilter->GetOutput());
	norm->SetComputePointNormals(1);
	norm->SetComputeCellNormals(1);
	norm->Update();
	//reverse normals
	vtkSmartPointer<vtkReverseSense> reverseSense =
		vtkSmartPointer<vtkReverseSense>::New();
	reverseSense->SetInputConnection(norm->GetOutputPort());
	reverseSense->ReverseNormalsOn();
	reverseSense->Update();

	return reverseSense->GetOutput();
}

//Combine 3D text & model
vtkSmartPointer<vtkPolyData>qSlicerAdd3DTextModuleWidget::
CombinePolyData(vtkPolyData* input1, vtkPolyData* input2)
{
	vtkSmartPointer<vtkAppendPolyData> appendFilter =
		vtkSmartPointer<vtkAppendPolyData>::New();
#if VTK_MAJOR_VERSION <= 5
	appendFilter->AddInputConnection(input1->GetProducerPort());
	appendFilter->AddInputConnection(input2->GetProducerPort());
#else
	appendFilter->AddInputData(input1);
	appendFilter->AddInputData(input2);
#endif
	appendFilter->Update();

	// Remove any duplicate points.
	// 	vtkSmartPointer<vtkCleanPolyData> cleanFilter =
	// 		vtkSmartPointer<vtkCleanPolyData>::New();
	// 	cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
	// 	cleanFilter->Update();

	return appendFilter->GetOutput();
}

