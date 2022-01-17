#include "DisplayWindow.h"
#include <QDebug>

DisplayWindow::DisplayWindow()
	: QWindow()
	, QOpenGLFunctions()
	, m_pContext(nullptr)
	, m_pRenderContext(nullptr)
	, m_pRenderShader(nullptr)
	, m_vertexBuffer(QOpenGLBuffer::VertexBuffer)
	, m_indexBuffer(QOpenGLBuffer::IndexBuffer)
	, m_pBackgroupTexture(nullptr)
	, m_pRenderThread(nullptr)
{
	InitOpenGLContext();
}

DisplayWindow::~DisplayWindow()
{
	m_pRenderThread->Stop();
	delete m_pRenderThread;
	m_pRenderThread = nullptr;

	m_pRenderShader->release();
	m_vertexBuffer.release();
	m_indexBuffer.release();
}

//void DisplayWindow::exposeEvent(QExposeEvent* ev)
//{
//	if (isExposed() == false)
//	{
//		QWindow::exposeEvent(ev);
//		return;
//	}
//
//	QWindow::exposeEvent(ev);
//}

void DisplayWindow::resizeEvent(QResizeEvent* ev)
{
	
}

void DisplayWindow::InitOpenGLContext()
{
	setSurfaceType(OpenGLSurface);
	create();

	QSurfaceFormat format;
	if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
	{
		format.setProfile(QSurfaceFormat::CoreProfile);
		format.setRenderableType(QSurfaceFormat::OpenGL);
		format.setSamples(8);
	}
	else
	{
		format.setProfile(QSurfaceFormat::NoProfile);
		format.setRenderableType(QSurfaceFormat::OpenGLES);
	}

	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	format.setSwapInterval(0);

	m_pContext = new QOpenGLContext(this);
	m_pContext->setFormat(format);
	m_pContext->create();

	m_pRenderContext = new QOpenGLContext();
	m_pRenderContext->setFormat(format);
	m_pRenderContext->setShareContext(m_pContext);
	m_pRenderContext->create();

	m_pRenderContext->makeCurrent(this);

	initializeOpenGLFunctions();
	m_pRenderShader = new QOpenGLShaderProgram(this);

	QImage bgImage;
	if (!bgImage.load(":/MediaPlayer/images/disp_bg.png"))
	{
		qDebug() << "Load Failed BG Image";
	}

	m_pBackgroupTexture = new QOpenGLTexture(bgImage);

	m_vertexBuffer.create();
	m_indexBuffer.create();

	VertexData vertices[] =
	{
		{ QVector3D(-1.0f, 1.0f, 0.0f), QVector2D(0.0f, 1.0f) },
		{ QVector3D(1.0f, 1.0f, 0.0f), QVector2D(1.0f, 1.0f) },
		{ QVector3D(-1.0f, -1.0f, 0.0f), QVector2D(0.0f, 0.0f) },
		{ QVector3D(1.0f, -1.0f, 0.0f), QVector2D(1.0f, 0.0f) },
	};

	GLushort indices[] =
	{
		0, 1, 2, 2, 1, 3
	};

	m_vertexBuffer.bind();
	m_vertexBuffer.allocate(vertices, 4 * sizeof(VertexData));
	//m_vertexBuffer.release();

	m_indexBuffer.bind();
	m_indexBuffer.allocate(indices, 6 * sizeof(GLushort));
	//m_indexBuffer.release();

	if (!m_pRenderShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/MediaPlayer/shader/vshader.glsl"))
	{
		qDebug() << "Vertex Sharder Add Failed";
	}

	if (!m_pRenderShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/MediaPlayer/shader/fshader.glsl"))
	{
		qDebug() << "Fragment Sharder Add Failed";
	}

	m_pRenderShader->link();
	m_pRenderShader->bind();
	m_pRenderContext->doneCurrent();

	m_pRenderThread = new RenderThread(m_pRenderContext, this);
	m_pRenderContext->moveToThread(m_pRenderThread);

	m_pRenderThread->start();
}

void DisplayWindow::Update()
{

}

void DisplayWindow::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.125f, 0.125f, 0.125f, 1);
	glViewport(0, 0, size().width(), size().height());

	//m_pRenderShader->bind();

	//m_vertexBuffer.bind();
	//m_indexBuffer.bind();

	m_pRenderShader->setUniformValue("xMargin", 1.0f);
	m_pRenderShader->setUniformValue("yMargin", 1.0f);
	m_pRenderShader->setUniformValue("mTranslate", QMatrix4x4());

	m_pRenderShader->setUniformValue("isYUVDraw", false);

	GLuint id = m_pBackgroupTexture->textureId();
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, id);
	m_pRenderShader->setUniformValue("texBG", id);

	int offset = 0;
	int vertexLocation = m_pRenderShader->attributeLocation("a_Position");
	m_pRenderShader->enableAttributeArray(vertexLocation);
	m_pRenderShader->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

	offset += sizeof(QVector3D);

	int texcoordLocation = m_pRenderShader->attributeLocation("a_TexCoord");
	m_pRenderShader->enableAttributeArray(texcoordLocation);
	m_pRenderShader->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	//m_vertexBuffer.release();
	//m_indexBuffer.release();
	//m_pRenderShader->release();
}

void DisplayWindow::slotClickedPlay()
{

}