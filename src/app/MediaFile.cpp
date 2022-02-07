#include "MediaFile.h"

namespace player
{
    MediaFile::MediaFile(TyFnStreamCallback fn)
        : m_pFormatContext(nullptr)
        , m_nVideoStreamIdx(-1)
        , m_nAudioStreamIdx(-1)
        , m_bRelease(false)
        , m_mutexRead()
        , m_cvRead()
        , m_thReadThread()
    {
        m_thReadThread = std::thread(&MediaFile::run, this);
        m_fnStreamCallback = fn;
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
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    bool MediaFile::Open(const QString sFileName)
    {
        const char* c_strFileName = sFileName.toStdString().c_str();

        int ret = avformat_open_input(&m_pFormatContext, c_strFileName, nullptr, nullptr);
        if (ret < 0)
        {
            printf("Could not open file %s\n", c_strFileName);
            return false;
        }

        ret = avformat_find_stream_info(m_pFormatContext, nullptr);
        if (ret < 0)
        {
            printf("Could not find stream information %s\n", c_strFileName);
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
            printf("Could not find video stream %s\n", c_strFileName);
            return false;
        }

        return false;
    }
}