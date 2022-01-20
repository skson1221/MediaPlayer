#include "DisplayWindow.h"
#include <QDebug>

#include "AppDefine.h"

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
	, m_codec()
	, m_bDecode(false)
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

	for (int i = 0; i < m_pYUVTextures.size(); ++i)
	{
		glDeleteTextures(1, &m_pYUVTextures[YUV_U]);
	}
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
	m_pYUVTextures.fill(YUV_DEFAULT);

	m_vertexBuffer.create();
	m_indexBuffer.create();

	VertexData vertices[] =
	{
		{ Vector3D(-1.0f, 1.0f, 0.0f), Vector2D(0.0f, 1.0f) },
		{ Vector3D(1.0f, 1.0f, 0.0f), Vector2D(1.0f, 1.0f) },
		{ Vector3D(-1.0f, -1.0f, 0.0f),Vector2D(0.0f, 0.0f) },
		{ Vector3D(1.0f, -1.0f, 0.0f), Vector2D(1.0f, 0.0f) },
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
	if (!m_bDecode)
		return;

	if (!m_codec.IsOpen())
	{
		m_codec.Open("C:/Project/Qt/MediaPlayer/bin/x64/snow_4K.mp4");
	}

	m_codec.Decode(m_pYUVBuffer);

	if (YUV_DEFAULT == m_pYUVTextures[YUV_Y]) glGenTextures(1, &m_pYUVTextures[YUV_Y]);
	if (YUV_DEFAULT == m_pYUVTextures[YUV_U]) glGenTextures(1, &m_pYUVTextures[YUV_U]);
	if (YUV_DEFAULT == m_pYUVTextures[YUV_V]) glGenTextures(1, &m_pYUVTextures[YUV_V]);

	glBindTexture(GL_TEXTURE_2D, m_pYUVTextures[YUV_Y]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 3840, 2160, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 3840, 2160, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_pYUVBuffer[YUV_Y].data());
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, m_pYUVTextures[YUV_U]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_pYUVBuffer[YUV_U].size(), 2160, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_pYUVBuffer[YUV_U].size(), 2160, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_pYUVBuffer[YUV_U].data());
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, m_pYUVTextures[YUV_V]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, m_pYUVBuffer[YUV_V].size(), 2160, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_pYUVBuffer[YUV_U].size(), 2160, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_pYUVBuffer[YUV_V].data());
	glBindTexture(GL_TEXTURE_2D, 0);
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

	if (m_bDecode)
	{
		m_pRenderShader->setUniformValue("isYUVDraw", true);

		GLuint id = m_pYUVTextures[YUV_Y];
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_2D, id);
		m_pRenderShader->setUniformValue("texY", id);

		id = m_pYUVTextures[YUV_U];
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_2D, id);
		m_pRenderShader->setUniformValue("texU", id);

		id = m_pYUVTextures[YUV_V];
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_2D, id);
		m_pRenderShader->setUniformValue("texV", id);
	}
	else
	{
		m_pRenderShader->setUniformValue("isYUVDraw", false);
		GLuint id = m_pBackgroupTexture->textureId();
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_2D, id);
		m_pRenderShader->setUniformValue("texBG", id);
	}

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
	m_bDecode = true;
}