#pragma once
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

class PlayCtrlWidget : public QWidget
{
	Q_OBJECT

public:
	PlayCtrlWidget(QWidget* parent = Q_NULLPTR);
	~PlayCtrlWidget();

private:
	void Initialize();

private:
	QLabel*			m_pTimeLabel;
	QSlider*		m_pPlaySlider;
	QPushButton*	m_pPlayButton;
};

