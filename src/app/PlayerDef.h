#pragma once
#include <functional>

namespace player
{
	enum WorkMode
	{
		WORK_MODE_PAUSE,
		WORK_MODE_FORWARD,
		WORK_MODE_FORWARD_STEP,
	};

	enum WorkResult
	{
		WORK_SUCCESS,
		WORK_FINISH,
	};

	typedef std::function<bool(uint8_t*, int)> TyFnVideoStream;
	typedef std::function<bool(uint8_t*, int)> TyFnAudioStream;
}
