#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H
#include <QDialog> //对话类窗口的基类

QT_BEGIN_NAMESPACE
class QDialogButtonBox; //以适合当前小部件风格的布局显示按钮
class QPushButton; //命令按钮
class QSqlTableModel; //为数据库提供一个可编辑的数据模型
QT_END_NAMESPACE


class TableEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TableEditor(const QString &tableName,QWidget *parent = nullptr);

private slots:
    void submit();

private:
    QPushButton *submitButton;
    QPushButton *revertButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QSqlTableModel *model;

};

#endif // TABLEEDITOR_H
