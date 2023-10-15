#include "mainwindow.h"
#include "mandelbrotwidget.h"

#include <QSlider>
#include <QFormLayout>
#include <QDockWidget>

namespace
{
constexpr auto g_sampels = 8;
constexpr auto g_gl_major_version = 3;
constexpr auto g_gl_minor_version = 3;
}// namespace


MainWindow::MainWindow()
{
    QSlider* iterSlider = new QSlider(Qt::Horizontal);

    iterSlider->setMinimum(MandelbrotWidget::MIN_ITERS);
    iterSlider->setMaximum(MandelbrotWidget::MAX_ITERS);
    iterSlider->setValue(MandelbrotWidget::DEFAULT_ITERS);


    QSlider* thresholdSlider = new QSlider(Qt::Horizontal);

    thresholdSlider->setMinimum(MandelbrotWidget::MIN_THRESHOLD);
    thresholdSlider->setMaximum(MandelbrotWidget::MAX_THRESHOLD);
    thresholdSlider->setValue(MandelbrotWidget::DEFAULT_THRESHOLD);

    fpsLabel_ = new QLabel();
    fpsLabel_->setText("...");

    QFormLayout* formLayout = new QFormLayout();

    formLayout->addRow("Iterations: ", iterSlider);
    formLayout->addRow("Threshold:", thresholdSlider);
    formLayout->addRow("FPS:", fpsLabel_);

    QSurfaceFormat format;
    format.setSamples(g_sampels);
    format.setVersion(g_gl_major_version, g_gl_minor_version);
    format.setProfile(QSurfaceFormat::CoreProfile);

    MandelbrotWidget* mandelbrotWidget = new MandelbrotWidget;
    mandelbrotWidget->setFormat(format);

    connect(iterSlider, &QSlider::valueChanged, mandelbrotWidget, &MandelbrotWidget::setIters);
    connect(thresholdSlider, &QSlider::valueChanged, mandelbrotWidget, &MandelbrotWidget::setThreshold);
    connect(mandelbrotWidget, &MandelbrotWidget::updateFPS, this, &MainWindow::updateFPS);

    setCentralWidget(mandelbrotWidget);

    auto dock = new QDockWidget;
    auto settingsWidget = new QWidget;
    settingsWidget->setFocusPolicy(Qt::NoFocus);
    dock->setFocusPolicy(Qt::NoFocus);
    settingsWidget->setLayout(formLayout);
    dock->setWidget(settingsWidget);

    this->addDockWidget(Qt::BottomDockWidgetArea, dock);
}

void MainWindow::updateFPS(float fps)
{
    fpsLabel_->setText(QString::asprintf("FPS:%.2g", fps));
}


