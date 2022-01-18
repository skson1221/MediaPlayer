#pragma once
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>

#include <thread>

class PlayCtrlWidget : public QWidget
{
	Q_OBJECT

public:
	PlayCtrlWidget(QWidget* parent = Q_NULLPTR);
	~PlayCtrlWidget();

private:
	void Initialize();

signals:
	void sigClickedPlay();

private slots:
	void slotClickedPlay();


private:
	QLabel*			m_pTimeLabel;
	QSlider*		m_pPlaySlider;
	QPushButton*	m_pPlayButton;

	std::thread		m_pDecodeThread;
};

