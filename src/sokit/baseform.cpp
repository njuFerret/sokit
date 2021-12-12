#include "baseform.h"
#include "setting.h"
#include "toolkit.h"

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QShortcut>
#include <QToolButton>
#include <QTreeWidget>
#include <QWidget>

#define PROP_EDIT "edit"
#define PROP_DIRT "dirt"
#define PROP_TARG "targ"

BaseForm::BaseForm(QWidget *parent, Qt::WindowFlags flags)
    : QWidget(parent, flags), m_cntRecv(0), m_cntSend(0), m_labRecv(0), m_labSend(0), m_cnlist(0) {}

BaseForm::~BaseForm() {}

bool BaseForm::init() {
  if (!initForm() || !initHotkeys())
    return false;

  initConfig();

  m_logger.setProperty(SET_SEC_DIR, property(SET_SEC_DIR).toString());

  return true;
}

void BaseForm::initCounter(QLabel *r, QLabel *s) {
  m_labRecv = r;
  m_labSend = s;
}

void BaseForm::initLogger(QCheckBox *w, QToolButton *c, QTreeWidget *o, QPlainTextEdit *d) {
  m_logger.init(o, w, d);

  connect(c, SIGNAL(released()), this, SLOT(clear()));
  connect(&m_logger, SIGNAL(clearLog()), this, SLOT(clear()));

  bindFocus(o, Qt::Key_F3);

  QShortcut *wr = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this);
  QShortcut *cl = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), this);
  QShortcut *sl = new QShortcut(QKeySequence(Qt::Key_F4), this);

  // NOTE: 修改 qVariantFromValue 为 QVariant::fromValue
  //	sl->setProperty(PROP_TARG, qVariantFromValue((void*)d));
  sl->setProperty(PROP_TARG, QVariant::fromValue((void *)d));

  connect(wr, &QShortcut::activated, w, &QCheckBox::click);
  connect(sl, &QShortcut::activated, this, &BaseForm::hotOutput);
  connect(cl, &QShortcut::activated, this, &BaseForm::clear);

  connect(this, QOverload<const QString &>::of(&BaseForm::output), &m_logger,
          QOverload<const QString &>::of(&Logger::output));
  connect(this, QOverload<const QString &, const char *, quint32>::of(&BaseForm::output), &m_logger,
          QOverload<const QString &, const char *, quint32>::of(&Logger::output));
}

void BaseForm::initLister(QToolButton *a, QToolButton *k, QListWidget *l) {
  m_cnlist = l;

  QShortcut *sk = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_K), this);
  QShortcut *sa = new QShortcut(QKeySequence(Qt::ALT + Qt::Key_A), this);

  connect(sk, &QShortcut::activated, this, QOverload<>::of(&BaseForm::kill));
  connect(sa, &QShortcut::activated, m_cnlist, &QListWidget::selectAll);

  connect(k, &QToolButton::released, this, QOverload<>::of(&BaseForm::kill));
  connect(a, &QToolButton::released, m_cnlist, &QListWidget::selectAll);

  bindFocus(m_cnlist, Qt::Key_F2);
}

void BaseForm::bindBuffer(qint32 id, QLineEdit *e, QToolButton *s, QComboBox *d) {

  // NOTE: 修改 qVariantFromValue 为 QVariant::fromValue
  //  s->setProperty(PROP_EDIT, qVariantFromValue((void *)e));
  //  s->setProperty(PROP_DIRT, qVariantFromValue((void *)d));

  s->setProperty(PROP_EDIT, QVariant::fromValue((void *)e));
  s->setProperty(PROP_DIRT, QVariant::fromValue((void *)d));

  connect(s, SIGNAL(released()), this, SLOT(send()));

  bindClick(s, Qt::Key_0 + id + Qt::CTRL);
  bindFocus(e, Qt::Key_0 + id + Qt::ALT);
  bindFocus(d, Qt::Key_0 + id + Qt::CTRL + Qt::SHIFT);
}

void BaseForm::bindFocus(QWidget *w, qint32 k) {
  QShortcut *s = new QShortcut(QKeySequence(k), this);
  // NOTE: 修改 qVariantFromValue 为 QVariant::fromValue
  //  s->setProperty(PROP_TARG, qVariantFromValue((void *)w));
  s->setProperty(PROP_TARG, QVariant::fromValue((void *)w));

  // NOTE: Fix 老式信号槽连接
  //  connect(s, SIGNAL(activated()), this, SLOT(focus()));
  connect(s, &QShortcut::activated, this, &BaseForm::focus);
}

void BaseForm::bindClick(QAbstractButton *b, qint32 k) {
  QShortcut *s = new QShortcut(QKeySequence(k), this);
  // NOTE: Fix 老式信号槽连接
  //  connect(s, SIGNAL(activated()), b, SLOT(click()));
  connect(s, &QShortcut::activated, b, &QAbstractButton::click);
}

void BaseForm::bindSelect(QComboBox *b, qint32 i, qint32 k) {
  QShortcut *s = new QShortcut(QKeySequence(k), this);
  // NOTE: 修改 qVariantFromValue 为 QVariant::fromValue
  //  s->setProperty(PROP_TARG, qVariantFromValue((void *)b));
  s->setProperty(PROP_TARG, QVariant::fromValue((void *)b));
  s->setObjectName(QString::number(i));

  // NOTE: Fix 老式信号槽连接
  //  connect(s, SIGNAL(activated()), this, SLOT(select()));
  connect(s, &QShortcut::activated, this, &BaseForm::select);
}

void BaseForm::focus() {
  QWidget *w = (QWidget *)sender()->property(PROP_TARG).value<void *>();
  if (w)
    w->setFocus(Qt::TabFocusReason);
}

void BaseForm::hotOutput() {
  QPlainTextEdit *t = (QPlainTextEdit *)sender()->property(PROP_TARG).value<void *>();
  if (t) {
    t->setFocus(Qt::TabFocusReason);
    t->selectAll();
  }
}

void BaseForm::select() {
  QComboBox *b = (QComboBox *)sender()->property(PROP_TARG).value<void *>();
  if (b && b->isEnabled()) {
    qint32 i = sender()->objectName().toInt();
    if (i < 0) {
      i = b->currentIndex() + 1;
      if (i >= b->count())
        i = 0;
    }

    b->setCurrentIndex(i);
  }
}

void BaseForm::countRecv(qint32 bytes) {
  if (bytes < 0)
    m_cntRecv = 0;
  else
    m_cntRecv += bytes;

  m_labRecv->setText(QString::number(m_cntRecv));
}

void BaseForm::countSend(qint32 bytes) {
  if (bytes < 0)
    m_cntSend = 0;
  else
    m_cntSend += bytes;

  m_labSend->setText(QString::number(m_cntSend));
}

void BaseForm::send() {
  QLineEdit *e = (QLineEdit *)sender()->property(PROP_EDIT).value<void *>();
  QComboBox *d = (QComboBox *)sender()->property(PROP_DIRT).value<void *>();
  if (e)
    send(e->text(), (d ? d->currentText() : ""));
}

void BaseForm::clear() {
  m_logger.clear();

  lock();
  countRecv(-1);
  countSend(-1);
  unlock();
}

void BaseForm::kill() {
  if (lock(1000)) {
    QStringList list;

    listerSelected(list);
    kill(list);

    unlock();
  }
}

void BaseForm::listerSelected(QStringList &output) {
  qint32 i = m_cnlist->count();
  while (i--) {
    QListWidgetItem *itm = m_cnlist->item(i);
    if (itm && itm->isSelected())
      output << itm->text();
  }
}

void BaseForm::listerAdd(const QString &caption) {
  listerRemove(caption);
  m_cnlist->addItem(caption);
}

void BaseForm::listerRemove(const QString &caption) {
  qint32 i = m_cnlist->count();
  while (i--) {
    QListWidgetItem *itm = m_cnlist->item(i);
    if (itm && itm->text() == caption)
      delete m_cnlist->takeItem(i);
  }
}
