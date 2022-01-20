#pragma once
#include <QWindow>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include <array>

#include "RenderThread.h"
#include "Codec.h"

class DisplayWindow : public QWindow, public QOpenGLFunctions
{
	Q_OBJECT

	friend class RenderThread;

public:
	DisplayWindow();
	~DisplayWindow();

protected:
	//void exposeEvent(QExposeEvent* ev) override;
	void resizeEvent(QResizeEvent* ev) override;

private:
	void InitOpenGLContext();
	void Update();
	void Render();

public slots:
	void slotClickedPlay();

private:
	QOpenGLContext*			m_pContext;
	QOpenGLContext*			m_pRenderContext;
	QOpenGLShaderProgram*	m_pRenderShader;
	QOpenGLBuffer			m_vertexBuffer;
	QOpenGLBuffer			m_indexBuffer;
	QOpenGLTexture*			m_pBackgroupTexture;
	YUV_TEXTURE				m_pYUVTextures;
	YUV_BUFFER				m_pYUVBuffer;

	RenderThread*			m_pRenderThread;
	Codec					m_codec;
	bool					m_bDecode;
	
};

