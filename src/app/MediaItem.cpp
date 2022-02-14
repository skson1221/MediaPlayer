#include "MediaItem.h"

MediaItem::MediaItem(uint8_t* data, int length)
	: m_pData(nullptr)
	, m_nLength(length)
{
	m_pData = new uint8_t[length];
	memcpy(m_pData, data, length);
	m_nLength = length;
}

MediaItem::~MediaItem()
{
	safe_delete_array(m_pData);
}

uint8_t* MediaItem::GetData()
{
	return m_pData;
}

int MediaItem::GetLength()
{
	return m_nLength;
}
