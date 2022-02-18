#include "VideoDecoder.h"

VideoDecoder::VideoDecoder()
    : m_pCodecContext(nullptr)
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

bool VideoDecoder::Open(AVCodecID eCodecID, const AVCodecParameters* pCodecParam)
{
    Clear();

    AVCodec* pCodec = nullptr;
    pCodec = avcodec_find_decoder(eCodecID);
    if (nullptr == pCodec)
    {
        printf("Unsupported codec!\n");
        return false;
    }

    m_pCodecContext = nullptr;
    m_pCodecContext = avcodec_alloc_context3(pCodec);
    int ret = avcodec_parameters_to_context(m_pCodecContext, pCodecParam);
    
    //if (0 != ret)
    //{
    //    printf("Could not created codec context.\n");
    //    return false;
    //}

    m_pCodecContext->width = 3840;
    m_pCodecContext->height = 2160;
    m_pCodecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    m_pCodecContext->codec_id = AV_CODEC_ID_H264;
    m_pCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    m_pCodecContext->extradata_size = 48;
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

    m_nVideoStreamIdx = -1;
    m_nAudioStreamIdx = -1;
}

bool VideoDecoder::operator()(uint8_t* data, int length, YUV_BUFFER& yuvBuffer)
{
	bool res = true;
	while (res)
	{
        AVPacket packet;
        av_init_packet(&packet);
        packet.data = data;
        packet.size = length;
        packet.flags = AV_PKT_FLAG_KEY;
        //packet.pts = 0;
        //packet.dts = -2002;
        //packet.duration = 1001;
        //packet.pos = 23478;
        //m_pCodecContext->codec_tag = 828601953;
        //m_pCodecContext->profile = 100;
        //m_pCodecContext->level = 52;
		int ret = avcodec_send_packet(m_pCodecContext, &packet);
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

            if(nullptr == m_sws_ctx)
                m_sws_ctx = sws_getContext(m_pCodecContext->width, m_pCodecContext->height, m_pCodecContext->pix_fmt, m_pCodecContext->width, m_pCodecContext->height, m_pCodecContext->pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);

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

	return true;
}
