#pragma once
#include <thread>

#include "PlayerDef.h"
#include "VideoDecoder.h"

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

		TyFnVideoStream GetVideoStreamCallback();
		TyFnAudioStream GetAudioStreamCallback();

	private:
		void Work();

		WorkResult ForwardPlay();
		WorkResult ForwardStepPlay();

		bool onRecvVideoStream(const uint8_t* const data, int length);
		bool onRecvAudioStream(const uint8_t* const data, int length);

	private:
		std::thread m_thWorker;

		WorkMode	m_eWorkMode;
		VideoDecoder m_videoDecoder;
		YUV_BUFFER				m_pYUVBuffer;
		TyFnRenderCallback	m_fnRenderCallback;
	};
}
