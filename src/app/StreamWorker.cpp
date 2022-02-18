#include "StreamWorker.h"
#include <iostream>

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

	void StreamWorker::CreateCodec(AVCodecID eVideoCodec, AVCodecID eAudioCodecID, const AVCodecParameters* pCodecParam)
	{
		m_videoDecoder.Open(eVideoCodec, pCodecParam);
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
		TySpMediaItem spVideoData(nullptr);
		{
			std::lock_guard<std::mutex> lg(m_mutexVideo);

			if(m_videoStreamQueue.empty())
				return WORK_FINISH;

			spVideoData = m_videoStreamQueue.front();
			m_videoStreamQueue.pop();
			std::cout << "pop video data" << std::endl;
		}

		if (!m_videoDecoder.IsOpen())
		{
			return WORK_FINISH;
		}

		uint8_t* videoData = spVideoData->GetData();
		int videoLength = spVideoData->GetLength();
		m_videoDecoder(videoData, videoLength, m_pYUVBuffer);
		m_fnRenderCallback(m_pYUVBuffer);

		return WORK_FINISH;
	}

	bool StreamWorker::onRecvVideoStream(uint8_t* data, int length)
	{
		TySpMediaItem spVideoData = std::make_shared<MediaItem>(data, length);
		{
			std::lock_guard<std::mutex> lg(m_mutexVideo);
			m_videoStreamQueue.push(spVideoData);
			std::cout << "pusu video data" << std::endl;
		}
		return false;
	}

	bool StreamWorker::onRecvAudioStream(uint8_t* data, int length)
	{
		return false;
	}
}