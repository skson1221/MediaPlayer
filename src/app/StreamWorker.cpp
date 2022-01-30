#include "StreamWorker.h"

namespace player
{
	StreamWorker::StreamWorker()
		: m_eWorkMode(WORK_MODE_PAUSE)
		, m_codec()
		, m_fnRenderCallback()
	{
	}

	StreamWorker::~StreamWorker()
	{
		if (m_thWorker.joinable())
			m_thWorker.join();
	}

	void StreamWorker::Start()
	{
		m_thWorker = std::thread(&StreamWorker::Work, this);
	}

	void StreamWorker::SetRenderCallback(TyFnRenderCallback fn)
	{
		m_fnRenderCallback = fn;
	}

	void StreamWorker::SetWorkMode(WorkMode mode)
	{
		m_eWorkMode = mode;
		Start();
	}

	void StreamWorker::Work()
	{
		//while (1)
		{
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));

			if (!m_codec.IsOpen())
			{
				if (!m_codec.Open("C:/Project/MediaPlayer/bin/x64/snow_4K.mp4"))
					return;
			}

			switch (m_eWorkMode)
			{
			case WORK_MODE_PAUSE:
				break;
			case WORK_MODE_FORWARD:
				ForwardPlay();
				break;
			case WORK_MODE_FORWARD_STEP:
				ForwardStepPlay();
				break;
			default:
				break;
			}
		}

	}

	WorkResult StreamWorker::ForwardPlay()
	{
		return WORK_SUCCESS;
	}

	WorkResult StreamWorker::ForwardStepPlay()
	{
		m_codec.Decode(m_pYUVBuffer);
		m_fnRenderCallback(m_pYUVBuffer);

		return WORK_FINISH;
	}
}