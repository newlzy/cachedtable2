#include "tableeditor.h"
#include <QtWidgets>
#include <QtSql>
#pragma execution_character_set("utf-8")

TableEditor::TableEditor(const QString &tableName, QWidget *parent)
{
    model = new QSqlTableModel(this);
    model->setTable(tableName); //模型上操作的数据库表
    //所有的更改都将缓存到模型中，直到submitAll()或revertAll()被调用。
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); //把setTable设置的表的数据用来填充模型

    model->setHeaderData(0,Qt::Horizontal,tr("ID"));
    model->setHeaderData(1,Qt::Horizontal,tr("第一"));
    model->setHeaderData(2,Qt::Horizontal,tr("最后"));

    QTableView *view = new QTableView;
    view->setModel(model);
    //根据用于在列中呈现每个项的委托的大小提示调整所有列的大小
    view->resizeColumnsToContents();

    submitButton = new QPushButton(tr("提交"));
    submitButton->setDefault(true); //默认按钮
    revertButton = new QPushButton(tr("&复原"));
    quitButton = new QPushButton(tr("退出"));

    //以适合当前小部件风格的布局显示按钮
    buttonBox = new QDialogButtonBox(Qt::Vertical);
    //ActionRole单击按钮会导致对对话框中的元素的更改
    buttonBox->addButton(submitButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(revertButton,QDialogButtonBox::ActionRole);
    //点击按钮会导致对话框被拒绝
    buttonBox->addButton(quitButton,QDialogButtonBox::RejectRole);

    //提交按钮。开始数据库事务
    connect(submitButton,&QPushButton::clicked,this,&TableEditor::submit);
    //复原。revertAll还原所有挂起的更改。
    connect(revertButton,&QPushButton::clicked,model,&QSqlTableModel::revertAll);
    connect(quitButton,&QPushButton::clicked,this,&TableEditor::close);

    //水平排列小部件
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(view);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout); //将此小部件的布局管理器设置为layout。

    setWindowTitle(tr("事务缓存"));
}

//提交
void TableEditor::submit()
{
    //返回模型的数据库连接。如果驱动程序支持事务，则在数据库上开始事务。操作成功返回true
    model->database().transaction();

    if(model->submitAll()){ //提交所有挂起的更改
        //将事务提交到数据库
        model->database().commit();
    }else{
        model->database().rollback(); //回滚数据库上的事务
        QMessageBox::warning(this,tr("缓存表"),tr("数据库报错：%1").arg(model->lastError().text()));
    }
}
