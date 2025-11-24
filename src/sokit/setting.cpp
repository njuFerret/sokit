#include <QComboBox>
#include <QDir>
#include <QFileInfo>
#include <QSettings>

#include "setting.h"

/************************************************************************************************************
 * @brief Setting::Setting
 * 构造函数 Setting::Setting() 会检查当前工作目录是否可写。
 * 如果不可写或路径等于用户主目录，则会在用户主目录下创建一个以应用名为前缀的隐藏目录（如 ~/.AppName）。
 * 最终调用 QSettings::setPath 来指定配置文件的存储位置。
 ************************************************************************************************************/
Setting::Setting() {
  QString path(QDir::currentPath());
  if (!QFileInfo(path).isWritable() || path == QDir::homePath()) {
    QDir dir(QDir::home());
    dir.mkdir("." SET_APP_NAME);
    if (dir.cd("." SET_APP_NAME))
      path = dir.absolutePath();
  }

  QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, path);
}

/************************************************************************************************************
 * @brief Setting::storage
 * Setting::storage() 返回一个静态的 QSettings 对象，用于后续所有读写操作。
 * 这样保证全局只有一个配置存储实例。
 * @return
 *************************************************************************************************************/
QSettings &Setting::storage() {
  static Setting dummy;        // to ensure call QSettings::setPath before create the g_settings
  static QSettings g_settings(QSettings::IniFormat, QSettings::UserScope, SET_APP_NAME);
  return g_settings;
}

QString Setting::path() { return QFileInfo(storage().fileName()).dir().absolutePath(); }

void Setting::flush() { storage().sync(); }

void Setting::set(const QString &section, const QString &key, const QString &val) {
  storage().setValue(section + key, val);
}

QString Setting::get(const QString &section, const QString &key, const QString &def) {
  return storage().value(section + key, def).toString();
}

/***
 *
 * Key-value schema
 * 当前文本： "<section>/<prefix>/current"
 * 条目内容： "<section>/<prefix>/itemN"，其中 N 为 0 ... count-1
 * 条目上限： 由 SET_MAX_CMBITM 限制，避免历史无限增长
 * 命名示例：
 *     server/tip/itemx,
 *     server/tip/item0,
 *     server/tip/item1,
 *     server/tport/itemx=1245
 *     server/tport/item0=1245
 *
 * */
void Setting::save(const QString &section, const QString &prefix, const QComboBox &cmb, bool all) {
  QSettings &store = storage();                      // 获取全局唯一的 QSettings 存储对象
  store.beginGroup(section);                         // 进入指定的配置分组，避免键冲突
                                                     //
  QString tkey = prefix + SET_PFX_CMBTXT;            // 构造当前文本的键名（前缀 + 特定后缀）
  QString tval = cmb.currentText().trimmed();        // 获取下拉框当前文本并去掉前后空格
  if (!tval.isEmpty())                               //
    store.setValue(tkey, tval);                      // 如果非空，则保存当前文本

  if (all) {                         // 如果需要保存所有条目
    QStringList keys, vals;          //
    keys = store.childKeys();        // 获取当前分组下的所有键
    qint32 n = keys.size();

    if (n > 0) {
      keys.sort();                                                // 对键名排序，保证遍历顺序稳定
      while (n--) {                                               // 从后往前遍历各个键
        QString k = keys[n];                                      //
        if ((k != tkey) && k.startsWith(prefix)) {                // 找出以 prefix 开头的旧条目
          QString v = store.value(k).toString().trimmed();        // 取出旧值并去空格
          if (!v.isEmpty() && (-1 == cmb.findText(v)))            //
            vals.prepend(v);                                      // 如果旧值非空且不在当前下拉框中，则暂存到 vals
          store.remove(k);                                        // 删除旧键，避免残留
        }
      }
    }

    n = cmb.count();               // 获取当前下拉框条目数
    if (n > SET_MAX_CMBITM)        //
      n = SET_MAX_CMBITM;          // 限制最大保存数量

    qint32 i = 0;
    for (i = 0; i < n; ++i)
      store.setValue(prefix + QString::number(i), cmb.itemText(i));             // 保存当前下拉框条目
                                                                                //
    n = (vals.count() > SET_MAX_CMBITM) ? SET_MAX_CMBITM : vals.count();        // 限制旧值保存数量
    for (qint32 j = 0; i < n; ++i, ++j)                                         //
      store.setValue(prefix + QString::number(i), vals[j]);                     // 追加保存旧值（不在当前下拉框中）
  }

  store.endGroup();        // 结束分组，提交写入
}

void Setting::load(const QString &section, const QString &prefix, QComboBox &cmb, bool all) {
  cmb.clear();

  QSettings &store = storage();

  QStringList keys, vals;

  store.beginGroup(section);

  keys = store.childKeys();
  qint32 n = keys.size();
  if (n > 0) {
    QString tval;
    QString tkey = prefix + SET_PFX_CMBTXT;

    keys.sort();

    while (n--) {
      QString k = keys[n];
      if (k.startsWith(prefix)) {
        QString v = store.value(k).toString().trimmed();
        if (k == tkey)
          tval = v;
        else if (all && !v.isEmpty())
          vals.append(v);
      }
    }

    vals.removeDuplicates();

    n = vals.count();
    if (n > 0) {
      if (n > SET_MAX_CMBITM)
        n = SET_MAX_CMBITM;

      while (n--)
        cmb.addItem(vals[n]);
    }

    if (!tval.isEmpty())
      cmb.setEditText(tval);
  }

  store.endGroup();
}
