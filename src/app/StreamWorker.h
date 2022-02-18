#pragma once
#include <thread>
#include <queue>
#include <mutex>

#include "PlayerDef.h"
#include "VideoDecoder.h"
#include "MediaItem.h"

namespace player
{
	class StreamWorker
	{
	public:
		StreamWorker();
		~StreamWorker();

		void Start();
		void SetRenderCallback(TyFnRenderCallback fn);
		void SetWorkMode(WorkMode mode);
		void CreateCodec(AVCodecID eVideoCodec, AVCodecID eAudioCodecID, const AVCodecParameters* pCodecParam);

		TyFnVideoStream GetVideoStreamCallback();
		TyFnAudioStream GetAudioStreamCallback();

	private:
		void Work();

		WorkResult ForwardPlay();
		WorkResult ForwardStepPlay();

		bool onRecvVideoStream(uint8_t* data, int length);
		bool onRecvAudioStream(uint8_t* data, int length);

	private:
		std::thread m_thWorker;

		WorkMode	m_eWorkMode;
		VideoDecoder m_videoDecoder;
		YUV_BUFFER				m_pYUVBuffer;
		TyFnRenderCallback	m_fnRenderCallback;

		std::mutex m_mutexVideo;
		std::queue<TySpMediaItem> m_videoStreamQueue;
	};
}
