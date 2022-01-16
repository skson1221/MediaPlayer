#pragma once
#include <QWindow>

class DisplayWindow : public QWindow
{
	Q_OBJECT

public:
	DisplayWindow();
	~DisplayWindow();

	void resizeEvent(QResizeEvent* ev) override;
};

