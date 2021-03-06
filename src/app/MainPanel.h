#pragma once
#include <QWidget>

#include "DisplayWindow.h"
#include "PlayCtrlWidget.h"
#include "StreamWorker.h"
#include "MediaFile.h"

using namespace player;

namespace player
{
	class MainPanel : public QWidget
	{
		Q_OBJECT

	public:
		MainPanel(QWidget* parent = Q_NULLPTR);
		~MainPanel();

	private:
		void Initialize();

	public slots:
		void slotClickedPlay();

	private:
		DisplayWindow* m_pDispWindow;
		PlayCtrlWidget* m_pPlayCtrlWidget;
		StreamWorker m_streamWorker;

		MediaFile* m_pMediaFile;
	};
}
