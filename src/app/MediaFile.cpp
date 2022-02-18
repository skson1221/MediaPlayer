#include "MediaFile.h"

namespace player
{
    MediaFile::MediaFile(TyFnVideoStream fnVideoStream, TyFnAudioStream fnAudioStream)
        : m_pFormatContext(nullptr)
        , m_nVideoStreamIdx(-1)
        , m_nAudioStreamIdx(-1)
        , m_bRelease(false)
        , m_bOpen(false)
        , m_mutexRead()
        , m_cvRead()
        , m_thReadThread()
    {
        m_thReadThread = std::thread(&MediaFile::run, this);
        m_fnVideoStream = fnVideoStream;
        m_fnAudioStream = fnAudioStream;
    }

    MediaFile::~MediaFile()
    {
        m_bRelease = true;

        if (m_thReadThread.joinable())
        {
            m_thReadThread.join();
        }

        if (m_pFormatContext)
        {
            avformat_close_input(&m_pFormatContext);
            m_pFormatContext = nullptr;
        }
    }

    void MediaFile::run()
    {
        while (!m_bRelease)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            
            if (!m_bOpen)
            {
                continue;
            }

            AVPacket packet;
            av_init_packet(&packet);
            if (av_read_frame(m_pFormatContext, &packet) < 0)
                continue;
            //av_seek_frame
            if (packet.stream_index == m_nVideoStreamIdx)
            {
                bool res = m_fnVideoStream(packet.data, packet.size);
            }
            else if (packet.stream_index == m_nAudioStreamIdx)
            {
                bool res = m_fnAudioStream(packet.data, packet.size);
            }
        }
    }

    bool MediaFile::Open(const QString& sFileName)
    {
        int ret = avformat_open_input(&m_pFormatContext, sFileName.toStdString().c_str(), nullptr, nullptr);
        if (ret < 0)
        {
            printf("Could not open file %s\n", sFileName.toStdString().c_str());
            return false;
        }

        ret = avformat_find_stream_info(m_pFormatContext, nullptr);
        if (ret < 0)
        {
            printf("Could not find stream information %s\n", sFileName.toStdString().c_str());
            return false;
        }
        
        for (int i = 0; i < m_pFormatContext->nb_streams; i++)
        {
            if (AVMEDIA_TYPE_VIDEO == m_pFormatContext->streams[i]->codecpar->codec_type)
            {
                m_nVideoStreamIdx = i;
            }
            else if (AVMEDIA_TYPE_AUDIO == m_pFormatContext->streams[i]->codecpar->codec_type)
            {
                m_nAudioStreamIdx = i;
            }
        }

        if (-1 == m_nVideoStreamIdx)
        {
            printf("Could not find video stream %s\n", sFileName.toStdString().c_str());
            return false;
        }

        m_bOpen = true;

        return true;
    }

    AVCodecID MediaFile::GetVideoCodec()
    {
        if (m_pFormatContext && 0 <= m_nVideoStreamIdx)
        {
            return m_pFormatContext->streams[m_nVideoStreamIdx]->codecpar->codec_id;
        }

        return AV_CODEC_ID_NONE;
    }

    AVCodecID MediaFile::GetAudioCodec()
    {
        if (m_pFormatContext && 0 <= m_nAudioStreamIdx)
        {
            return m_pFormatContext->streams[m_nAudioStreamIdx]->codecpar->codec_id;
        }

        return AV_CODEC_ID_NONE;
    }

    const AVCodecParameters* MediaFile::GetCodecParam()
    {
        if (m_pFormatContext && 0 <= m_nVideoStreamIdx)
        {
            return m_pFormatContext->streams[m_nVideoStreamIdx]->codecpar;
        }
        
        return nullptr;
    }
}