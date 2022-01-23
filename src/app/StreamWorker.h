#pragma once
#include <thread>
#include "Codec.h"

class StreamWorker
{
public:
	StreamWorker();
	~StreamWorker();

	void Start();
	void SetRenderCallback(TyFnRenderCallback fn);

private:
	void Work();

private:
	std::thread m_thWorker;
	Codec m_codec;
	YUV_BUFFER				m_pYUVBuffer;
	TyFnRenderCallback	m_fnRenderCallback;
};

