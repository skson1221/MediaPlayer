#pragma once
#include <QWidget>

#include "DisplayWindow.h"
#include "PlayCtrlWidget.h"

class MainPanel : public QWidget
{
	Q_OBJECT

public:
	MainPanel(QWidget* parent = Q_NULLPTR);
	~MainPanel();

private:
	void Initialize();

private:
	DisplayWindow* m_pDispWindow;
	PlayCtrlWidget* m_pPlayCtrlWidget;
};

