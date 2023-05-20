#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QTreeView>
#include <QTableView>
#include <QShortcut>
#include <QModelIndex>
#include <QHeaderView>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QSortFilterProxyModel>

#include "connector.h"
#include "datamodeltree.h"
#include "datamodellist.h"
#include "datamodeltable.h"

class Widget : public QWidget
{
    Q_OBJECT

    QTreeView *view_tree;
    QListView *view_list;
    QTableView *view_table;

    QLineEdit *searching;
    QPushButton *btn_add;
    QPushButton *btn_sort;
    QShortcut *shortcut_for_table;

    QVBoxLayout *lay2;
    QHBoxLayout *lay1;
    QHBoxLayout *lay;

    QDialog *change_name_object;
    QLineEdit *name_to_change;
    QObject *for_repeat_objects;

    DataModelTree *model_tree;
    DataModelList *model_list;
    DataModelTable *model_table;
    QSortFilterProxyModel *model_proxy_sort;

    QModelIndex current_index_list;
    QModelIndex current_index_tree;


public:
    Widget (QWidget *parent = nullptr);
    template <typename CHILD, typename PARENT> void insertObjectInTree();
    ~Widget();


public slots:
    void slot_insertValueFromLineEdit();
    void slot_insertToLineFromTree();
    void slot_insertToLineFromList();
    void slot_clickSort();
    void slot_seeText ();
    void slot_realiseTable();
    void slot_listChangeAfterDrop();
    void slot_treeChangeAfterDrop();
    void slot_treeChangeAfterAdd();
    void slot_changeNameAfterDrop();
    void slot_changeNameAfterAdd();

};

template <typename CHILD, typename PARENT>
void Widget::insertObjectInTree()
{
   CHILD *new_item = new CHILD(model_tree->objByIndex<PARENT>(view_tree->currentIndex()));

   Q_UNUSED(new_item);
}
#endif // WIDGET_H
