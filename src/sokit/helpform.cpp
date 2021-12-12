#include "helpform.h"
#include "setting.h"
#include "toolkit.h"

#include <QShortcut>
#include <QTextStream>

HelpForm::HelpForm(QWidget *parent, Qt::WindowFlags flags) : QDialog(parent, flags) {
  m_ui.setupUi(this);
  init();
}

HelpForm::~HelpForm() {}

void HelpForm::init() {
  QShortcut *k = new QShortcut(QKeySequence(Qt::Key_F1), this);
  // NOTE: Fix 老式信号槽连接
  //  connect(k, SIGNAL(activated()), this, SLOT(close()));
  connect(k, &QShortcut::activated, this, &HelpForm::close);
}
