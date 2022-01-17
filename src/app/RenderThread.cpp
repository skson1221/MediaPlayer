#include "RenderThread.h"
#include "DisplayWindow.h"

RenderThread::RenderThread(QOpenGLContext* m_pContext, QWindow* pWindow)
	: QThread()
	, m_pContext(m_pContext)
	, m_pWindow(pWindow)
	, m_bStop(false)
{

}

RenderThread::~RenderThread()
{
}

void RenderThread::Stop()
{
	m_bStop = true;
	if (wait())
		quit();
}

void RenderThread::run()
{
	DisplayWindow* pDispWindow = reinterpret_cast<DisplayWindow*>(m_pWindow);

	while (!m_bStop)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
		
		if (!m_pWindow->isExposed())
			continue;
		
		m_pContext->makeCurrent(m_pWindow);

		pDispWindow->Update();
		pDispWindow->Render();

		m_pContext->swapBuffers(m_pWindow);
		m_pContext->doneCurrent();
	}
}
