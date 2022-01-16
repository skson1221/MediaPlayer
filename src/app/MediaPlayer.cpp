#include "MediaPlayer.h"
#include "MainPanel.h"

MediaPlayer::MediaPlayer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    MainPanel* pMainPanel = new MainPanel(this);
    setCentralWidget(pMainPanel);
} 
