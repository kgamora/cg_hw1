#include "TriangleWindow.h"

#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QScreen>

#include <array>

namespace
{

constexpr std::array<GLfloat, 8u> vertices = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, 1.0f,
};
constexpr std::array<GLuint, 6u> indices = {0, 1, 2, 2, 1, 3};

}// namespace

void TriangleWindow::init()
{
	// Configure shaders
	program_ = std::make_unique<QOpenGLShaderProgram>(this);
    program_->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Shaders/fractal.vs");
	program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                      ":/Shaders/fractal.fs");
	program_->link();

	// Create VAO object
	vao_.create();
	vao_.bind();

	// Create VBO
	vbo_.create();
	vbo_.bind();
	vbo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo_.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(GLfloat)));

	// Create IBO
	ibo_.create();
	ibo_.bind();
	ibo_.setUsagePattern(QOpenGLBuffer::StaticDraw);
	ibo_.allocate(indices.data(), static_cast<int>(indices.size() * sizeof(GLuint)));

	// Bind attributes
	program_->bind();

	program_->enableAttributeArray(0);
    program_->setAttributeBuffer(0, GL_FLOAT, 0, 2, static_cast<int>(2 * sizeof(GLfloat)));

    windowSizeUniform_ = program_->uniformLocation("windowSize"); // ivec2
    sizesUniform_ = program_->uniformLocation("sizes"); // vec2
    fromPosUniform_ = program_->uniformLocation("fromPos"); // vec2

	// Release all
	program_->release();

	vao_.release();

	ibo_.release();
	vbo_.release();

	// Uncomment to enable depth test and face culling
	// glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);

	// Clear all FBO buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TriangleWindow::render()
{
	// Configure viewport
	const auto retinaScale = devicePixelRatio();
	glViewport(0, 0, static_cast<GLint>(width() * retinaScale),
			   static_cast<GLint>(height() * retinaScale));

	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate position change
    fromPos_ += positionChange_ * sizeX * 0.01;

    // Get window size
    QVector2D windowSize{(GLfloat)width(), (GLfloat)height()};

    // Calculate x and y sizes
    sizeX /= zoomSpeed;

    GLfloat sizeY = sizeX * ((GLfloat) height()) / ((float) width());

    QVector2D sizes{sizeX, sizeY};

	// Bind VAO and shader program
	program_->bind();
	vao_.bind();

    // Update uniform values
    program_->setUniformValue(fromPosUniform_, fromPos_);
    program_->setUniformValue(windowSizeUniform_, windowSize);
    program_->setUniformValue(sizesUniform_, sizes);

	// Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	// Release VAO and shader program
	vao_.release();
	program_->release();

	// Increment frame counter
	++frame_;
}

void TriangleWindow::keyPressEvent(QKeyEvent * e)
{
    switch( e->key() )
    {
    case Qt::Key_Up:
        zoomSpeed = 1.005;
        break;

    case Qt::Key_Down:
        zoomSpeed = 0.995;
        break;

    case Qt::Key_S:
        positionChange_ = {0.0, -1.0};
        break;

    case Qt::Key_W:
        positionChange_ = {0.0, 1.0};
        break;

    case Qt::Key_A:
        positionChange_ = {-1.0, 0.0};
        break;

    case Qt::Key_D:
        positionChange_ = {1.0, 0.0};
        break;

    default:
        break;
    }
}

void TriangleWindow::keyReleaseEvent(QKeyEvent *e) {
    switch( e->key() )
    {
    case Qt::Key_Down:
    case Qt::Key_Up:
        zoomSpeed = 1.0;
        break;
    case Qt::Key_S:
    case Qt::Key_W:
    case Qt::Key_A:
    case Qt::Key_D:
        positionChange_ = {0.0, 0.0};
        break;
    default:
        break;
    }
}
