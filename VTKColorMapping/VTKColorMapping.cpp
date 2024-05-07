#include "VTKColorMapping.h"

#include <vtkConeSource.h>
#include <vtkSTLReader.h>
#include <vtkMultiBlockPLOT3DReader.h>
#include <vtkDataSet.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkShrinkPolyData.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkStructuredGridOutlineFilter.h>
#include <vtkLookupTable.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

VTKColorMapping::VTKColorMapping(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	_pVTKWidget = new QVTKOpenGLNativeWidget();
	this->setCentralWidget(_pVTKWidget);
	// this->showMaximized();

	// 1. generate data
	// vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	// or, read data
	// vtkMultiBlockPLOT3DReader ��һ����ȡ���������ڶ�ȡ PLOT3D ��ʽ���ļ��������ʱ���ɽṹ������
	vtkSmartPointer<vtkMultiBlockPLOT3DReader> plot3dReader = vtkSmartPointer<vtkMultiBlockPLOT3DReader>::New();
	plot3dReader->SetXYZFileName("combxyz.bin");
	plot3dReader->SetQFileName("combq.bin");
	plot3dReader->SetScalarFunctionNumber(100);
	plot3dReader->SetVectorFunctionNumber(202);
	qDebug() << plot3dReader->GetOutput()->GetNumberOfBlocks(); // 0
	// ������¹���
	plot3dReader->Update();
	qDebug() << plot3dReader->GetOutput()->GetNumberOfBlocks(); // 1
	vtkDataSet* plot3dOutput = (vtkDataSet*)(plot3dReader->GetOutput()->GetBlock(0));

	// 2. filter
	// ��ȡ��Ϊ����μ��Σ��㣬�ߣ����棩��դ���һ����
	vtkSmartPointer<vtkStructuredGridGeometryFilter> plane = vtkSmartPointer<vtkStructuredGridGeometryFilter>::New();
	plane->SetInputData(plot3dOutput);
	plane->SetExtent(1, 100, 1, 100, 7, 7);
	// �����ṹ��դ��߽��һ��������
	vtkSmartPointer<vtkStructuredGridOutlineFilter> outline = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
	outline->SetInputData(plot3dOutput);
	// ��ɫӳ���
	vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
	lut->SetNumberOfColors(256); // ָ����ɫӳ������ж�������ɫ
	lut->Build();

	// 3. mapper
	vtkSmartPointer<vtkPolyDataMapper> planeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	// 4. actor
	vtkSmartPointer<vtkActor> planeActor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();

	// 5. renderer
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0.3, 0.6, 0.3); // Background Color: Green

	// 6. connect
	planeMapper->SetLookupTable(lut);
	planeMapper->SetInputConnection(plane->GetOutputPort());
	planeMapper->SetScalarRange(plot3dOutput->GetScalarRange()); // ���ñ���ֵ�ķ�Χ
	outlineMapper->SetInputConnection(outline->GetOutputPort());
	planeActor->SetMapper(planeMapper);
	outlineActor->SetMapper(outlineMapper);
	renderer->AddActor(planeActor);
	renderer->AddActor(outlineActor);

	this->_pVTKWidget->renderWindow()->AddRenderer(renderer);
	this->_pVTKWidget->renderWindow()->Render();
}

VTKColorMapping::~VTKColorMapping()
{}
