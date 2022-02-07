#include "MainPanel.h"

#include <QLayout>
#include <QFileDialog>

#include "StreamController.h"
#include "util_func.h"

MainPanel::MainPanel(QWidget* parent)
	: QWidget(parent)
	, m_pDispWindow(nullptr)
	, m_pPlayCtrlWidget(nullptr)
	, m_pMediaFile(nullptr)
{
	Initialize();
}

MainPanel::~MainPanel()
{
	if (m_pMediaFile)
		safe_delete(m_pMediaFile);
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

	StreamController::GetInstance()->SetRenderCallback(m_pDispWindow->GetRenderCallback());

	m_pPlayCtrlWidget = new PlayCtrlWidget(this);
	connect(m_pPlayCtrlWidget, SIGNAL(sigClickedPlay()), this, SLOT(slotClickedPlay()));

	pMainLayout->addWidget(pDispWidget);
	pMainLayout->addWidget(m_pPlayCtrlWidget);

	setLayout(pMainLayout);
}

void MainPanel::slotClickedPlay()
{
	QString sFileName = QFileDialog::getOpenFileName(this,
		tr("Open Image"), "", tr("Media File (*.mp4 *.avi)"));

	if (nullptr == m_pMediaFile)
		m_pMediaFile = new MediaFile();
	
	bool res = m_pMediaFile->Open(sFileName);
	if (false == res)
	{
		safe_delete(m_pMediaFile);
	}
}