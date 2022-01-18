#include "PlayCtrlWidget.h"
#include <QLayout>

PlayCtrlWidget::PlayCtrlWidget(QWidget* parent)
	: QWidget(parent)
	, m_pTimeLabel(nullptr)
	, m_pPlaySlider(nullptr)
	, m_pPlayButton(nullptr)
{
	Initialize();
}

PlayCtrlWidget::~PlayCtrlWidget()
{
}

void PlayCtrlWidget::Initialize()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	QVBoxLayout* pMainLayout = new QVBoxLayout;
	pMainLayout->setContentsMargins(0, 0, 0, 0);
	pMainLayout->setSpacing(0);
	pMainLayout->setAlignment(Qt::AlignmentFlag::AlignTop);

	m_pPlaySlider = new QSlider(Qt::Orientation::Horizontal, this);
	m_pPlaySlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	QHBoxLayout* pBottomLayout = new QHBoxLayout;
	{
		pBottomLayout->setContentsMargins(0, 0, 0, 0);
		pBottomLayout->setSpacing(5);
		pBottomLayout->setAlignment(Qt::AlignmentFlag::AlignLeft);

		m_pPlayButton = new QPushButton("Play", this);
		m_pPlayButton->setCheckable(true);
		connect(m_pPlayButton, SIGNAL(clicked()), this, SLOT(slotClickedPlay()));

		m_pTimeLabel = new QLabel("00:00:00 / 00:00:00", this);
		m_pTimeLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

		pBottomLayout->addWidget(m_pPlayButton);
		pBottomLayout->addWidget(m_pTimeLabel);
	}

	pMainLayout->addWidget(m_pPlaySlider);
	pMainLayout->addLayout(pBottomLayout);

	setLayout(pMainLayout);
}

void PlayCtrlWidget::slotClickedPlay()
{
	emit sigClickedPlay();
}