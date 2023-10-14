#pragma once

#include <Base/GLWindow.hpp>

#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>

#include <memory>

class TriangleWindow final : public fgl::GLWindow
{

public:
	void init() override;
	void render() override;

protected:
    void keyPressEvent(QKeyEvent * e) override;
    void keyReleaseEvent(QKeyEvent * e) override;

private:
    GLint windowSizeUniform_ = -1;
    GLint sizesUniform_ = -1;
    GLint fromPosUniform_ = -1;

    QVector2D fromPos_{-0.789136f, -0.150316f};
    GLfloat sizeX = 2.0;

    GLfloat zoomSpeed = 1.0;

	QOpenGLBuffer vbo_{QOpenGLBuffer::Type::VertexBuffer};
	QOpenGLBuffer ibo_{QOpenGLBuffer::Type::IndexBuffer};
	QOpenGLVertexArrayObject vao_;

	std::unique_ptr<QOpenGLShaderProgram> program_ = nullptr;

	size_t frame_ = 0;

	QVector2D mousePressPosition_{0., 0.};
    QVector2D positionChange_{0., 0.};
};
