#ifndef __HELPFORM_H__
#define __HELPFORM_H__

#include "ui_helpform.h"

class HelpForm : public QDialog {
  Q_OBJECT

public:
  // NOTE: Fix deprecation warnings caused by QFlags
  //    HelpForm(QWidget* p=0, Qt::WindowFlags f=0);
  HelpForm(QWidget *parent = 0, Qt::WindowFlags flags = QFlag(0));
  virtual ~HelpForm();

private:
  void init();

private:
  Ui::HelpForm m_ui;
};

#endif        // __HELPFORM_H__
