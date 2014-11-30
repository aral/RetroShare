#ifndef GXSFORUMTHREADWIDGET_H
#define GXSFORUMTHREADWIDGET_H

#include "gui/gxs/GxsMessageFrameWidget.h"
#include "util/TokenQueue.h"

class QTreeWidgetItem;
class RSTreeWidgetItemCompareRole;
class RsGxsForumMsg;
class GxsForumsFillThread;
class UIStateHelper;
class RsGxsForumGroup;

namespace Ui {
class GxsForumThreadWidget;
}

class GxsForumThreadWidget : public GxsMessageFrameWidget, public TokenResponse
{
	Q_OBJECT

	Q_PROPERTY(QColor textColorRead READ textColorRead WRITE setTextColorRead)
	Q_PROPERTY(QColor textColorUnread READ textColorUnread WRITE setTextColorUnread)
	Q_PROPERTY(QColor textColorUnreadChildren READ textColorUnreadChildren WRITE setTextColorUnreadChildren)
	Q_PROPERTY(QColor textColorNotSubscribed READ textColorNotSubscribed WRITE setTextColorNotSubscribed)
	Q_PROPERTY(QColor textColorMissing READ textColorMissing WRITE setTextColorMissing)

public:
	explicit GxsForumThreadWidget(const RsGxsGroupId &forumId, QWidget *parent = NULL);
	~GxsForumThreadWidget();

	QColor textColorRead() const { return mTextColorRead; }
	QColor textColorUnread() const { return mTextColorUnread; }
	QColor textColorUnreadChildren() const { return mTextColorUnreadChildren; }
	QColor textColorNotSubscribed() const { return mTextColorNotSubscribed; }
	QColor textColorMissing() const { return mTextColorMissing; }

	void setTextColorRead(QColor color) { mTextColorRead = color; }
	void setTextColorUnread(QColor color) { mTextColorUnread = color; }
	void setTextColorUnreadChildren(QColor color) { mTextColorUnreadChildren = color; }
	void setTextColorNotSubscribed(QColor color) { mTextColorNotSubscribed = color; }
	void setTextColorMissing(QColor color) { mTextColorMissing = color; }

	/* GxsMessageFrameWidget */
	virtual RsGxsGroupId groupId() { return mForumId; }
	virtual void setGroupId(const RsGxsGroupId &forumId);
	virtual QString groupName(bool withUnreadCount);
	virtual QIcon groupIcon();
	virtual void setAllMessagesRead(bool read);
	virtual bool navigate(const RsGxsMessageId& msgId);

	unsigned int newCount() { return mNewCount; }
	unsigned int unreadCount() { return mUnreadCount; }

	QTreeWidgetItem *convertMsgToThreadWidget(const RsGxsForumMsg &msg, bool useChildTS, uint32_t filterColumn);
	QTreeWidgetItem *generateMissingItem(const RsGxsMessageId &msgId);

	// Callback for all Loads.
	virtual void loadRequest(const TokenQueue *queue, const TokenRequest &req);

protected:
	bool eventFilter(QObject *obj, QEvent *ev);
	void changeEvent(QEvent *e);
	virtual void updateDisplay(bool complete);

private slots:
	/** Create the context popup menu and it's submenus */
	void threadListCustomPopupMenu(QPoint point);

	void changedThread();
	void clickedThread (QTreeWidgetItem *item, int column);

	void replytomessage();
	void replyMessageData(const RsGxsForumMsg &msg);

	//void print();
	//void printpreview();

	//void removemessage();
	void markMsgAsRead();
	void markMsgAsReadChildren();
	void markMsgAsUnread();
	void markMsgAsUnreadChildren();

	void copyMessageLink();

	/* handle splitter */
	void togglethreadview();

	void subscribeGroup(bool subscribe);
	void createthread();
	void createmessage();

	void previousMessage();
	void nextMessage();
	void nextUnreadMessage();
	void downloadAllFiles();

	void changedViewBox();

	void filterColumnChanged(int column);
	void filterItems(const QString &text);

	void fillThreadFinished();
	void fillThreadProgress(int current, int count);
	void fillThreadStatus(QString text);

private:
	void insertMessageData(const RsGxsForumMsg &msg);

	void insertThreads();
	void insertMessage();

	void fillThreads(QList<QTreeWidgetItem *> &threadList, bool expandNewMessages, QList<QTreeWidgetItem*> &itemToExpand);
	void fillChildren(QTreeWidgetItem *parentItem, QTreeWidgetItem *newParentItem, bool expandNewMessages, QList<QTreeWidgetItem*> &itemToExpand);

	int getSelectedMsgCount(QList<QTreeWidgetItem*> *pRows, QList<QTreeWidgetItem*> *pRowsRead, QList<QTreeWidgetItem*> *pRowsUnread);
	void setMsgReadStatus(QList<QTreeWidgetItem*> &rows, bool read);
	void markMsgAsReadUnread(bool read, bool children, bool forum);
	void calculateIconsAndFonts(QTreeWidgetItem *item = NULL);
	void calculateIconsAndFonts(QTreeWidgetItem *item, bool &hasReadChilddren, bool &hasUnreadChilddren);
	void calculateUnreadCount();

	void togglethreadview_internal();

	bool filterItem(QTreeWidgetItem *item, const QString &text, int filterColumn);

	void processSettings(bool bLoad);

	void requestGroupData();
	void loadGroupData(const uint32_t &token);
	void insertGroupData(const RsGxsForumGroup &group);

	void requestMessageData(const RsGxsGrpMsgIdPair &msgId);
	void loadMessageData(const uint32_t &token);
	void requestMsgData_ReplyMessage(const RsGxsGrpMsgIdPair &msgId);
	void loadMsgData_ReplyMessage(const uint32_t &token);

private:
	RsGxsGroupId mForumId;
	RsGxsGroupId mLastForumID;
	RsGxsMessageId mThreadId;
	QString mForumDescription;
	int mSubscribeFlags;
	bool mInProcessSettings;
	bool mInMsgAsReadUnread;
	int mLastViewType;
	RSTreeWidgetItemCompareRole *mThreadCompareRole;
	TokenQueue *mTokenQueue;
	GxsForumsFillThread *mFillThread;
	unsigned int mUnreadCount;
	unsigned int mNewCount;

	/* Color definitions (for standard see qss.default) */
	QColor mTextColorRead;
	QColor mTextColorUnread;
	QColor mTextColorUnreadChildren;
	QColor mTextColorNotSubscribed;
	QColor mTextColorMissing;

	UIStateHelper *mStateHelper;

	RsGxsMessageId mNavigatePendingMsgId;
	QList<RsGxsMessageId> mIgnoredMsgId;

	Ui::GxsForumThreadWidget *ui;
};

#endif // GXSFORUMTHREADWIDGET_H
