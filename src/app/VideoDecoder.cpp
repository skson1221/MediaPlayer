#include "VideoDecoder.h"

VideoDecoder::VideoDecoder()
    : m_pCodecContext(nullptr)
    , m_pPacket(nullptr)
    , m_sws_ctx(nullptr)
    , m_nVideoStreamIdx(-1)
    , m_nAudioStreamIdx(-1)
    , m_bOpen(false)
{
    static bool bRegister = false;
    if (!bRegister)
    {
	    avcodec_register_all();
        bRegister = true;
    }
}

VideoDecoder::~VideoDecoder()
{
    Clear();
}

bool VideoDecoder::Open(const std::string& sFileName)
{
    Clear();

	
    
    AVCodec* pCodec = nullptr;
    pCodec = avcodec_find_decoder(m_pFormatContext->streams[m_nVideoStreamIdx]->codecpar->codec_id);
    if (nullptr == pCodec)
    {
        printf("Unsupported codec!\n");
        return false;
    }

    m_pCodecContext = nullptr;
    m_pCodecContext = avcodec_alloc_context3(pCodec);
    ret = avcodec_parameters_to_context(m_pCodecContext, m_pFormatContext->streams[m_nVideoStreamIdx]->codecpar);
    if (0 != ret)
    {
        printf("Could not created codec context.\n");
        return false;
    }

    ret = avcodec_open2(m_pCodecContext, pCodec, nullptr);
    if (ret < 0)
    {
        printf("Could not open codec.\n");
        return false;
    }

    AVFrame* pFrame = NULL;

    // Allocate video frame
    //pFrame = av_frame_alloc();
    //if (pFrame == NULL)
    //{
    //    printf("Could not allocate frame.\n");
    //    return false;
    //}

   // printf("Resolution : %d x %d, PixFormat : %d FPS : %d.\n", m_pCodecContext->width, m_pCodecContext->height, m_pCodecContext->pix_fmt, m_pCodecContext->framerate);
    m_sws_ctx = sws_getContext(m_pCodecContext->width, m_pCodecContext->height, m_pCodecContext->pix_fmt, m_pCodecContext->width, m_pCodecContext->height, m_pCodecContext->pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);


    m_bOpen = true;

    //while (av_read_frame(m_pFormatContext, m_pPacket) >= 0)
    //{
    //    if (m_pPacket->stream_index == m_nVideoStreamIdx)
    //    {
    //        ret = avcodec_send_packet(m_pCodecContext, m_pPacket);
    //        if (ret < 0)
    //        {
    //            printf("Error sending packet for decoding.\n");
    //            return false;
    //        }

    //        while (0 <= ret)
    //        {
    //            ret = avcodec_receive_frame(m_pCodecContext, pFrame);

    //            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
    //            {
    //                break;
    //            }
    //            else if (ret < 0)
    //            {
    //                printf("Error while decoding.\n");
    //                return false;
    //            }

    //            int sizeY = pFrame->linesize[0];
    //            int sizeUV = pFrame->linesize[1];
    //        }
    //    }
    //}

    return true;
}

bool VideoDecoder::IsOpen()
{
    return m_bOpen;
}

void VideoDecoder::Clear()
{
    if (m_pCodecContext)
    {
        avcodec_free_context(&m_pCodecContext);
        m_pCodecContext = nullptr;
    }

    if (m_pPacket)
    {
        av_packet_free(&m_pPacket);
        m_pPacket = nullptr;
    }

    m_nVideoStreamIdx = -1;
    m_nAudioStreamIdx = -1;
}

bool VideoDecoder::operator()(YUV_BUFFER& yuvBuffer)
{
    m_pPacket = av_packet_alloc();
    if (nullptr == m_pPacket)
    {
        printf("Could not alloc packet,\n");
        return false;
    }
    av_init_packet(m_pPacket);
    m_pPacket->data;
    m_pPacket->size;
    if (av_read_frame(m_pFormatContext, m_pPacket) < 0)
        return false;

    if (m_pPacket->stream_index == m_nVideoStreamIdx)
    {
        bool res = true;
        while (res)
        {
            int ret = avcodec_send_packet(m_pCodecContext, m_pPacket);
            if (ret < 0)
            {
                printf("Error sending packet for decoding.\n");
                res = false;
                break;
            }

            while (0 <= ret)
            {
                AVFrame* pFrame = NULL;

                // Allocate video frame
                pFrame = av_frame_alloc();
                ret = avcodec_receive_frame(m_pCodecContext, pFrame);

                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                {
                    break;
                }
                else if (ret < 0)
                {
                    printf("Error while decoding.\n");
                    res = false;
                    break;
                }

                AVFrame* pFrameRGB = NULL;
                pFrameRGB = av_frame_alloc();
                if (pFrameRGB == NULL)
                {
                    printf("Could not allocate frame.\n");
                    break;
                }

                int numBytes = 0;
                numBytes = av_image_get_buffer_size(m_pCodecContext->pix_fmt, m_pCodecContext->width, m_pCodecContext->height, 32); // [10]
                uint8_t* buffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));    // [11]

                av_image_fill_arrays( // [12]
                    pFrameRGB->data,
                    pFrameRGB->linesize,
                    buffer,
                    m_pCodecContext->pix_fmt,
                    m_pCodecContext->width,
                    m_pCodecContext->height,
                    32
                );

                sws_scale(  // [16]
                    m_sws_ctx,
                    (uint8_t const* const*)pFrame->data,
                    pFrame->linesize,
                    0,
                    pFrame->height,
                    pFrameRGB->data,
                    pFrameRGB->linesize
                );

                yuvBuffer[YUV_Y].clear();
                yuvBuffer[YUV_U].clear();
                yuvBuffer[YUV_V].clear();

                yuvBuffer[YUV_Y].reserve(pFrameRGB->linesize[0] * pFrame->height);
                yuvBuffer[YUV_U].reserve(pFrameRGB->linesize[1] * (pFrame->height / 2));
                yuvBuffer[YUV_V].reserve(pFrameRGB->linesize[2] * (pFrame->height / 2));

                yuvBuffer[YUV_Y].insert(yuvBuffer[YUV_Y].end(), &pFrameRGB->data[0][0], &pFrameRGB->data[0][pFrameRGB->linesize[0] * pFrame->height]);
                yuvBuffer[YUV_U].insert(yuvBuffer[YUV_U].end(), &pFrameRGB->data[1][0], &pFrameRGB->data[1][pFrameRGB->linesize[1] * (pFrame->height / 2)]);
                yuvBuffer[YUV_V].insert(yuvBuffer[YUV_V].end(), &pFrameRGB->data[2][0], &pFrameRGB->data[2][pFrameRGB->linesize[2] * (pFrame->height / 2)]);

                printf("Resolution : %d x %d, PixFormat : %d FPS : %d.\n", m_pCodecContext->width, m_pCodecContext->height, m_pCodecContext->pix_fmt, m_pCodecContext->framerate);

                av_frame_free(&pFrame);
                av_frame_free(&pFrameRGB);
                av_free(buffer);

                res = false;
            }
        }

        av_free_packet(m_pPacket);

        return true;
    }
}
