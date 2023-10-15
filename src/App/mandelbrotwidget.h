#pragma once

#include <Base/GLWindow.hpp>

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>

#include <memory>

class MandelbrotWidget final : public QOpenGLWidget,
                               protected QOpenGLFunctions
{
    Q_OBJECT
public:
    constexpr static int MIN_ITERS = 10;
    constexpr static int MAX_ITERS = 1000;
    constexpr static int DEFAULT_ITERS = 100;

    constexpr static int MIN_THRESHOLD = 100;
    constexpr static int MAX_THRESHOLD = 400;
    constexpr static int DEFAULT_THRESHOLD = 256;

public:
    MandelbrotWidget();

    void initializeGL() override;
    void paintGL() override;

signals:
    void updateFPS(float);

public slots:
    void setIters(int iters);
    void setThreshold(int threshold);
    void fpsTimer();

protected:
    void keyPressEvent(QKeyEvent * e) override;
    void keyReleaseEvent(QKeyEvent * e) override;

private:
    GLint thresholdUniform_ = -1;
    GLint itersUniform_ = -1;
    GLint windowSizeUniform_ = -1;
    GLint sizesUniform_ = -1;
    GLint fromPosUniform_ = -1;

    size_t framePoint_ = 0;
    int iters_ = DEFAULT_ITERS;
    float threshold_ = DEFAULT_THRESHOLD;

    QVector2D fromPos_{-0.789136f, -0.150316f};
    GLfloat sizeX = 2.0;

    GLfloat zoomSpeed = 1.0;

	QOpenGLBuffer vbo_{QOpenGLBuffer::Type::VertexBuffer};
	QOpenGLBuffer ibo_{QOpenGLBuffer::Type::IndexBuffer};
	QOpenGLVertexArrayObject vao_;

	std::unique_ptr<QOpenGLShaderProgram> program_ = nullptr;

    QTimer* updateTimer_ {};
    QTimer* frameTimer_ {};
    std::chrono::steady_clock::time_point lastFrame_;

	size_t frame_ = 0;

	QVector2D mousePressPosition_{0., 0.};
    QVector2D positionChange_{0., 0.};

    bool animate_ = false;
};
