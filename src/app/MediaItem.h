#pragma once
#include <iostream>
#include <memory>
#include "util_func.h"

class MediaItem
{
public:
	MediaItem(uint8_t* data, int length);
	~MediaItem();

	uint8_t* 	GetData();
	int			GetLength();

private:
	uint8_t*	m_pData;
	int			m_nLength;
};

typedef std::shared_ptr<MediaItem> TySpMediaItem;