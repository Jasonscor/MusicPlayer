#include "MusicPlaylist.h"

#include <QMenu>

#include <QHeaderView>
#include <QContextMenuEvent>
#include "NoFocusDelegate.h"

MusicPlaylist::MusicPlaylist(QWidget* parent):
QTableWidget(parent){
	setFrameStyle(QFrame::NoFrame);
	horizontalHeader()->setSectionsClickable(true);
	horizontalHeader()->setFrameStyle(QFrame::NoFrame);

	verticalHeader()->setHidden(true);
	horizontalHeader()->setStretchLastSection(true);
	horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	setRowCount(0);
	setColumnCount(3);

	QStringList tableHeader;
	tableHeader << QStringLiteral("����") << QStringLiteral("������") << QStringLiteral("ʱ��");
	setHorizontalHeaderLabels(tableHeader);

	setShowGrid(false);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setItemDelegate(new NoFocusDelegate(this));

	m_curRow = 0;
	createActions();
}

void MusicPlaylist::addRow(QString title, QString artist, QString duration){
	int curNRow = rowCount() + 1;
	setRowCount(curNRow);

	setItem(curNRow-1, 0, new QTableWidgetItem(title));
	setItem(curNRow-1, 1, new QTableWidgetItem(artist));
	setItem(curNRow-1, 2, new QTableWidgetItem(duration));
}

void MusicPlaylist::addRows(QStringList title, QStringList artist, QStringList duration){
	int preNRow = rowCount();
	int curNRow = preNRow + title.count();
	setRowCount(curNRow);
	
	for (size_t i = 0; i < title.count(); ++i){
		setItem(preNRow + i, 0, new QTableWidgetItem(title.at(i)));
		setItem(preNRow + i, 1, new QTableWidgetItem(artist.at(i)));
		setItem(preNRow + i, 2, new QTableWidgetItem(duration.at(i)));
	}
}

void MusicPlaylist::createActions(){
	m_popMenu = new QMenu;
	playSongAction = new QAction(QIcon("../Resources/rplay.png"), QStringLiteral("����"), this);
	pauseSongAction = new QAction(QIcon("../Resources/rpause.png"), QStringLiteral("��ͣ"), this);
	deleteSongAction = new QAction(QIcon("../Resources/rdelete.png"), QStringLiteral("ɾ��"), this);
	viewSongTagAction = new QAction(QIcon("../Resources/rinfo.png"), QStringLiteral("�鿴"), this);

	connect(deleteSongAction, SIGNAL(triggered()), this, SLOT(slotDeleteCurSong()));
	connect(playSongAction, SIGNAL(triggered()), this, SLOT(slotPlayCurSong()));
	connect(pauseSongAction, SIGNAL(triggered()), this, SLOT(slotPauseCurSong()));
}

void MusicPlaylist::contextMenuEvent(QContextMenuEvent *e){
	m_popMenu->clear(); //���ԭ�в˵�
	QPoint point = e->pos(); //�õ���������
	QTableWidgetItem *item = this->itemAt(point);

	if (item != NULL){
		m_curRow = item->row();

		m_popMenu->addAction(playSongAction);
		m_popMenu->addAction(pauseSongAction);
		m_popMenu->addSeparator();
		m_popMenu->addAction(deleteSongAction);
		m_popMenu->addSeparator();
		m_popMenu->addAction(viewSongTagAction);

		m_popMenu->setStyleSheet(QLatin1String("QMenu{\n"
		"background-color: #3A3939;\n"
		"border: 1px solid #3A3939;\n"
		"color: silver;\n"
		"margin: 2px;\n"
		"}\n"
		"QMenu::icon{\n"
		"margin: 5px;\n"
		"}\n"
		"QMenu::item{\n"
		"padding: 5px 30px 5px 30px;\n"
		"margin - left: 5px;\n"
		"border: 1px solid transparent;\n"
		"}\n"
		"QMenu::item:selected{\n"
		"background-color: #3d8ec9;\n"
		"color: black;\n"
		"}\n"
		));

		//sub menu example 
		//sort_style = pop_menu->addMenu("����");
		//sort_style->addAction(action_name);
		//sort_style->addAction(action_size);
		//sort_style->addAction(action_type);
		//sort_style->addAction(action_date);

		// �˵����ֵ�λ��Ϊ��ǰ����λ��
		m_popMenu->exec(QCursor::pos());
		e->accept();
	}
}

void MusicPlaylist::slotPlayCurSong(){
	emit sigPlayCurSong(m_curRow);
}

void MusicPlaylist::slotPauseCurSong(){
	emit sigPauseCurSong(m_curRow);
}

void MusicPlaylist::slotDeleteCurSong(){
	// delete and refresh table
	this->removeRow(m_curRow);
	emit sigDeleteCurSong(m_curRow);
}

void MusicPlaylist::slotViewCurSong(){
	emit sigViewCurSong(m_curRow);
}