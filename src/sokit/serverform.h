#ifndef __SERVERFORM_H__
#define __SERVERFORM_H__

#include "baseform.h"
#include "serverskt.h"
#include "ui_serverform.h"
#include <QFlags>

class ServerForm : public BaseForm {
  Q_OBJECT

public:
  // NOTE: Fix deprecation warnings caused by QFlags
  //    ServerForm(QWidget* p=0, Qt::WindowFlags f=0);
  ServerForm(QWidget *parent = 0, Qt::WindowFlags flags = QFlag(0));

  virtual ~ServerForm();

protected:
  virtual bool initForm();
  virtual bool initHotkeys();
  virtual void initConfig();
  virtual void saveConfig();
  virtual void send(const QString &data, const QString &dir);
  virtual void kill(QStringList &list);

private slots:
  void trigger(bool start);

private:
  ServerSktTcp m_tcp;
  ServerSktUdp m_udp;
  Ui::ServerForm m_ui;
};

#endif        // __SERVERFORM_H__
