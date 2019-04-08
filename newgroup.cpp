#include "newgroup.h"
#include "ui_newgroup.h"
#include "ContentProvider.h"

NewGroup::NewGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewGroup)
{
    ui->setupUi(this);
    initStaffList();
}

NewGroup::~NewGroup()
{
    delete ui;
}
bool NewGroup::initStaffList()
{
    QSqlQuery sqlQueryStaff;
    QString sqlStr = "select staff_id, staff_name, staff_group_id, staff_role from staff";
    sqlQueryStaff.prepare( sqlStr );


    StaffModel &smod =ContentProvider::getContentProvider()->staff_model;
    smod.flashBySQL();
    qDebug()<<"xxxxxxxxx    ";
    qDebug()<<smod.size();
    qDebug()<<"    xxxxxxxxx    "<<endl;



    for(int i=0;i<smod.size();++i){
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(smod.getStaffByIndex(i).staff_name);
        ui->listWidgetStaffs->addItem(item);
    }

//    if( sqlQueryStaff.exec() )
//    {
//        qDebug()<<"查询staff成功"<<endl;
//        int row = 0;
//        while( sqlQueryStaff.next() )
//        {
//            QListWidgetItem *item = new QListWidgetItem();
//            item->setText(sqlQueryStaff.value(1).toString());
//            ui->listWidgetStaffs->addItem(item);
//            qDebug()<<sqlQueryStaff.value(2).toInt()<<endl;
//            qDebug()<<sqlQueryStaff.value(0).toInt()<<endl;
//            if( sqlQueryStaff.value(2).toInt() != 0)
//                ui->listWidgetStaffs->setItemHidden(ui->listWidgetStaffs->item(row), true);
//            row++;
//        }
//    }
}
bool NewGroup::on_pushButtonOK_clicked()
{
    QItemSelectionModel *selections = ui->listWidgetStaffs->selectionModel(); //返回当前的选择模式
    QModelIndexList selectedsList = selections->selectedIndexes(); //返回所有选定的模型项目索引列表
//    qDebug()<<selectedsList.count()<<endl;

    std::vector<int> vecItemIndex;//保存选中行的索引
    for (int i = 0; i < selectedsList.count(); i++)
    {
        vecItemIndex.push_back((selectedsList.at(i).row() + 1));
//        qDebug()<<(selectedsList.at(i).row() + 1)<<endl;
    }
    if(ui->lineEditInputGroupName->text() == "")
      QMessageBox::warning(this, tr("警告"), tr("组名不能为空！"));
    else if(selectedsList.count() == 0)
      QMessageBox::warning(this, tr("警告"), tr("您未选择组员呢！"));
    else
    {
        QSqlQuery sqlQueryGroups;
        QString sqlStr = "select group_id, group_name from groups";
        sqlQueryGroups.prepare( sqlStr );
        sqlQueryGroups.exec();
        int  tempGroupId = sqlQueryGroups.size() + 1;
        for(int i=0 ;i<vecItemIndex.size();i++)
        {
    //        usedStaffId.push_back(vecItemIndex[i]);
            QSqlQuery sqlQuery;
            QString sqlStr = QString("update staff set staff_group_id =  '%1'  where staff_id = '%2' ")
                    .arg(tempGroupId).arg(vecItemIndex[i]);
            sqlQuery.prepare( sqlStr );
            sqlQuery.exec();

            QSqlQuery sqlQuery02;
            QString sqlStr02 = QString("insert into groups (group_id, group_name) values ('%1', '%2')")
                .arg(tempGroupId).arg(ui->lineEditInputGroupName->text());
            sqlQuery02.prepare( sqlStr02 );
            sqlQuery02.exec();
         }
        emit choosePc();
        this->close();
    }

}
void NewGroup::on_pushButtonCancel_clicked()
{
    this->hide();
}
