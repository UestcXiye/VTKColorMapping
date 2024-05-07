#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VTKColorMapping.h"

#include <QVTKOpenGLNativeWidget.h>
#include <qdebug.h>

class VTKColorMapping : public QMainWindow
{
    Q_OBJECT

public:
    VTKColorMapping(QWidget *parent = nullptr);
    ~VTKColorMapping();

private:
    Ui::VTKColorMappingClass ui;

    QVTKOpenGLNativeWidget* _pVTKWidget = Q_NULLPTR;
};
