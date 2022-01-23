#include "StreamWorker.h"

StreamWorker::StreamWorker()
	: m_codec()
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

void StreamWorker::Work()
{
	//while (1)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (!m_codec.IsOpen())
		{
			if (!m_codec.Open("C:/Project/Qt/MediaPlayer/bin/x64/snow_4K.mp4"))
				return;
		}

		m_codec.Decode(m_pYUVBuffer);
		m_fnRenderCallback(m_pYUVBuffer);
	}

}
