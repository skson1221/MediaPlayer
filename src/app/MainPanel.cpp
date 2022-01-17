#include "MainPanel.h"

#include <QLayout>

MainPanel::MainPanel(QWidget* parent)
	: QWidget(parent)
	, m_pDispWindow(nullptr)
	, m_pPlayCtrlWidget(nullptr)
{
	Initialize();
}

MainPanel::~MainPanel()
{

}

void MainPanel::Initialize()
{
	QVBoxLayout* pMainLayout = new QVBoxLayout;
	pMainLayout->setContentsMargins(0, 0, 0, 0);
	pMainLayout->setSpacing(0);
	pMainLayout->setAlignment(Qt::AlignmentFlag::AlignTop);

	m_pDispWindow = new DisplayWindow();
	QWidget* pDispWidget = QWidget::createWindowContainer((QWindow*)m_pDispWindow, this);
	pDispWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	m_pPlayCtrlWidget = new PlayCtrlWidget(this);

	pMainLayout->addWidget(pDispWidget);
	pMainLayout->addWidget(m_pPlayCtrlWidget);

	setLayout(pMainLayout);
}