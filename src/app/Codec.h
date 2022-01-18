#pragma once

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavutil/samplefmt.h"
#include "libavutil/imgutils.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
}

#include <string>

class Codec
{
public:
	Codec();
	~Codec();

	bool Open(const std::string& sFileName);
	bool IsOpen();
	bool Decode();
	bool VideoDecode();
	bool AudioDecode();
	void Clear();

private:
	AVFormatContext*	m_pFormatContext;
	AVCodecContext*		m_pCodecContext;
	AVPacket*			m_pPacket;
	SwsContext*			m_sws_ctx;

	int		m_nVideoStreamIdx;
	int		m_nAudioStreamIdx;
	bool	m_bOpen;
};

