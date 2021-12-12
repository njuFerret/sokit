#ifndef __NOTEPADFORM_H__
#define __NOTEPADFORM_H__

#include <QPlainTextEdit>

class NotepadForm : public QWidget {
  Q_OBJECT

public:
  // NOTE: Fix deprecation warnings caused by QFlags
  // NotepadForm(QWidget *parent = 0, Qt::WindowFlags flags = 0);
  NotepadForm(QWidget *parent = 0, Qt::WindowFlags flags = QFlag(0));
  virtual ~NotepadForm();

public:
  bool init();

private slots:
  void jumptab();

private:
  void setupUi();
  void uninit();

private:
  QPlainTextEdit *m_board;
};

#endif        // __NOTEPADFORM_H__
