#ifndef __SETTING_H__
#define __SETTING_H__

#define SET_APP_NAME   "sokit"

#define SET_MAX_CMBITM 10
#define SET_PFX_CMBITM "item"
#define SET_PFX_CMBTXT 'x'

#define SET_SEC_CFG    "config"
#define SET_SEC_DIR    "dir"
#define SET_KEY_LOG    "/log"

class QComboBox;
class QSettings;
class QString;

/**
 * @file setting.cpp
 * @brief 应用程序的配置管理模块，基于 Qt 的 QSettings 封装。
 *
 * 该模块负责：
 * - 自动选择合适的配置文件存储路径（当前目录或用户主目录下的隐藏目录）。
 * - 提供统一的配置读写接口。
 * - 支持 UI 控件（如 QComboBox）的状态保存与恢复。
 * - 保证全局只有一个 QSettings 实例。
 */

class Setting {
public:
  /**
   * @brief 写入配置项。
   *
   * @param section 配置分组（逻辑命名空间）。
   * @param key     配置键。
   * @param val     配置值（QString）。
   *
   * @post 将值写入内存存储，调用 flush() 可确保写入磁盘。
   *
   * @see get(), flush()
   */
  static void set(const QString &section, const QString &key, const QString &val);
  /**
   * @brief 读取配置项。
   *
   * @param section 配置分组。
   * @param key     配置键。
   * @param def     默认值，当键不存在时返回。
   *
   * @return QString 返回存储的值或默认值。
   *
   * @see set(), storage()
   */
  static QString get(const QString &section, const QString &key, const QString &def);
  /**
   * @brief 获取当前配置文件所在目录的绝对路径。
   *
   * @return QString 配置目录的绝对路径。
   *
   * @note 该路径在构造函数中确定，可能是当前目录或用户主目录下的隐藏目录。
   *
   * @see Setting(), storage()
   */
  static QString path();
  /**
   * @brief 保存 QComboBox 的状态到配置文件。
   *
   * @param section 配置分组。
   * @param prefix  键名前缀，用于区分不同控件。
   * @param cmb     QComboBox 控件引用。
   * @param all     是否保存所有条目（true 保存所有，false 仅保存当前文本）。
   *
   * 功能：
   * - 保存当前选中的文本。
   * - 如果 all=true，则保存下拉框中的所有条目（最多 SET_MAX_CMBITM 个）。
   * - 清理旧条目，避免重复。
   *
   * @see load(), flush()
   */
  static void save(const QString &section, const QString &prefix, const QComboBox &cmb, bool all = true);
  /**
   * @brief 从配置文件中恢复 QComboBox 的状态。
   *
   * @param section 配置分组。
   * @param prefix  键名前缀。
   * @param cmb     QComboBox 控件引用。
   * @param all     是否恢复所有条目（true 恢复所有，false 仅恢复当前文本）。
   *
   * 功能：
   * - 清空控件。
   * - 读取保存的条目并填充。
   * - 恢复当前选中的文本。
   *
   * @see save(), get()
   */
  static void load(const QString &section, const QString &prefix, QComboBox &cmb, bool all = true);

  /**
   * @brief 强制将内存中的配置写入磁盘。
   *
   * 调用 QSettings::sync()，确保所有修改持久化。
   *
   * @warning 频繁调用可能影响性能，建议批量写入后再 flush。
   *
   * @see set(), storage()
   */
  static void flush();

private:
  /**
   * @brief 构造函数，初始化配置存储路径。
   *
   * 逻辑：
   * - 检查当前工作目录是否可写。
   * - 如果不可写或路径等于用户主目录，则在用户主目录下创建一个隐藏目录（如 ~/.AppName）。
   * - 调用 QSettings::setPath 设置最终的存储路径。
   *
   * @post 保证存在一个可写的配置目录，并且 QSettings 已经绑定到该目录。
   *
   * @see path(), storage()
   */
  Setting();
  /**
   * @brief 获取全局唯一的 QSettings 存储对象。
   *
   * @return QSettings& 返回一个静态的 QSettings 引用。
   *
   * 特点：
   * - 第一次调用时初始化，之后复用。
   * - 所有配置读写都通过该对象完成。
   *
   * @note 线程安全性依赖 QSettings 自身实现，跨线程使用需额外同步。
   *
   * @see set(), get(), flush()
   */
  static QSettings &storage();
};

#endif        // __SETTING_H__
