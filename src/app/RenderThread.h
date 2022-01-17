#pragma once
#include <QThread>
#include <QOpenGLContext>
#include <QWindow>

class RenderThread : public QThread
{
	Q_OBJECT

public:
	RenderThread(QOpenGLContext* m_pContext, QWindow* pWindow);
	~RenderThread();

	void Stop();

protected:
	void run() override;

private:
	QOpenGLContext* m_pContext;
	QWindow*		m_pWindow;

	bool m_bStop;
};

