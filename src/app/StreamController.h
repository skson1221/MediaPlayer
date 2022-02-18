#pragma once
#include <QObject>

#include "StreamWorker.h"

namespace player
{
	class StreamController : protected QObject
	{
		Q_OBJECT

	public:
		static StreamController* GetInstance();
		static void Release();

		void SetRenderCallback(TyFnRenderCallback fn);
		void SetWorkMode(WorkMode mode);
		void CreateCodec(AVCodecID eVideoCodec, AVCodecID eAudioCodecID, const AVCodecParameters* pCodecParam);
		TyFnVideoStream GetVideoStreamCallback();
		TyFnAudioStream GetAudioStreamCallback();

	private:
		StreamController() {};
		~StreamController() {};

	private:
		StreamWorker m_videoStream;

		static StreamController* m_instance;
	};
}

