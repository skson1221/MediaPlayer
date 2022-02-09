#pragma once

extern "C"
{
#include "libavformat/avformat.h"
}

#include <QTimer>
#include <qelapsedtimer.h>

#include <thread>
#include <mutex>

#include "PlayerDef.h"

namespace player
{
	class MediaFile
	{
	public:
		explicit MediaFile(TyFnStreamCallback fn);
		~MediaFile();

		void run();

		bool Open(const QString sFileName);
		AVCodecID GetCodecID();
		const AVCodecParameters* GetCodecParam();

	private:
		AVFormatContext* m_pFormatContext;

		int		m_nVideoStreamIdx;
		int		m_nAudioStreamIdx;

		std::mutex	m_mutexRead;
		std::condition_variable m_cvRead;
		std::thread m_thReadThread;

		bool m_bRelease;

		TyFnStreamCallback m_fnStreamCallback;
	};
}
