#include "StreamWorker.h"

namespace player
{
	StreamWorker::StreamWorker()
		: m_eWorkMode(WORK_MODE_PAUSE)
		, m_videoDecoder()
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

	TyFnVideoStream StreamWorker::GetVideoStreamCallback()
	{
		return std::bind(&StreamWorker::onRecvVideoStream, this, std::placeholders::_1, std::placeholders::_2);
	}

	TyFnAudioStream StreamWorker::GetAudioStreamCallback()
	{
		return std::bind(&StreamWorker::onRecvAudioStream, this, std::placeholders::_1, std::placeholders::_2);
	}

	void StreamWorker::Work()
	{
		while (1)
		{
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));

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
		m_videoDecoder(m_pYUVBuffer);
		m_fnRenderCallback(m_pYUVBuffer);

		return WORK_FINISH;
	}

	bool StreamWorker::onRecvVideoStream(const uint8_t* const data, int length)
	{
		return false;
	}

	bool StreamWorker::onRecvAudioStream(const uint8_t* const data, int length)
	{
		return false;
	}
}