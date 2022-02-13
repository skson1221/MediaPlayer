#include "StreamController.h"
#include "AppDefine.h"

namespace player
{
	StreamController* StreamController::m_instance = nullptr;

	StreamController* StreamController::GetInstance()
	{
		if (nullptr == m_instance)
		{
			m_instance = new StreamController();
		}

		return m_instance;
	}

	void StreamController::Release()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	void StreamController::SetRenderCallback(TyFnRenderCallback fn)
	{
		m_videoStream.SetRenderCallback(fn);
	}

	void StreamController::SetWorkMode(WorkMode mode)
	{
		m_videoStream.SetWorkMode(mode);
	}

	TyFnVideoStream StreamController::GetVideoStreamCallback()
	{
		 return m_videoStream.GetVideoStreamCallback();
	}

	 TyFnAudioStream StreamController::GetAudioStreamCallback()
	 {
		 return m_videoStream.GetAudioStreamCallback();
	 }
}
