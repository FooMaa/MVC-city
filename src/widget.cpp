#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    model_tree = new DataModelTree(this);
    model_list = new DataModelList(this);
    model_table = new DataModelTable(0, this);
    model_proxy_sort = new QSortFilterProxyModel(this);

    view_tree = new QTreeView(this);
    view_list = new QListView(this);
    view_table = new QTableView(this);

    searching = new QLineEdit("", this);
    btn_add = new QPushButton ("Add", this);
    btn_sort = new QPushButton("Sort", this);
    shortcut_for_table = new QShortcut (QKeySequence (Qt::CTRL + Qt:: Key_0), this);

    btn_sort->setCheckable(true);

    view_tree->setModel(model_tree);
    model_proxy_sort->setSourceModel(model_list);
    view_list->setModel(model_proxy_sort);
    view_table->setModel(model_table);
    view_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    view_tree->setDragEnabled(true);
    view_tree->setAcceptDrops(true);
    view_tree->setSelectionMode(QAbstractItemView::SingleSelection);

    lay = new QHBoxLayout;
    lay1 = new QHBoxLayout;
    lay2 = new QVBoxLayout;

    setLayout(new QVBoxLayout);

    lay1->addWidget(view_tree);
    lay2->addWidget(btn_sort);
    lay2->addWidget(searching);
    lay2->addWidget(view_list);
    lay1->addItem(lay2);
    lay->addWidget(btn_add);
    layout ()->expandingDirections();
    layout ()->addItem(lay1);
    layout ()->addWidget(view_table);
    layout ()->addItem(lay);

    view_tree->setCurrentIndex(view_tree->rootIndex());

    connect (btn_add, &QPushButton::clicked, this, &Widget::slot_insertValueFromLineEdit);
    connect (btn_sort, &QPushButton::clicked, this,&Widget::slot_clickSort);
    connect (view_tree, &QTreeView::doubleClicked, this, &Widget::slot_insertToLineFromTree);
    connect (view_list, &QListView::doubleClicked, this, &Widget::slot_insertToLineFromList);
    connect (searching, &QLineEdit::textChanged, this, &Widget::slot_seeText);
    connect (shortcut_for_table, &QShortcut::activated, this, &Widget::slot_realiseTable);
    connect(model_tree, &DataModelTree::signal_dragAndDrop, this, &Widget::slot_listChangeAfterDrop);
    connect(model_tree, &DataModelTree::signal_dragAndDrop, this, &Widget::slot_treeChangeAfterDrop);
    connect(model_tree, &DataModelTree::signal_addItem, this, &Widget::slot_treeChangeAfterAdd);
}

Widget::~Widget()
{

}

void Widget::slot_insertValueFromLineEdit()
{
    if (view_tree->currentIndex() == view_tree->rootIndex())
    {
        City *new_item= new City(model_tree->rootItem);

        Q_UNUSED(new_item);
    }
    else
    {

        switch (model_tree->seeParent(nullptr, view_tree->currentIndex()))
        {
            case type_Street:
               insertObjectInTree<Street,City>();
               break;

            case type_Home:
               insertObjectInTree<Home,Street>();
               break;

            case type_People:
               insertObjectInTree<People,Home>();
               break;
        }

        if (view_tree->currentIndex() == current_index_tree)
        {
            model_list->addValue(view_tree->currentIndex());
        }

    }

    model_tree->signal_addItem();
    model_tree->layoutChanged(); 
}

void Widget::slot_insertToLineFromTree()
{
    current_index_tree = view_tree->currentIndex();
    model_list->addValue(current_index_tree);
    current_index_list = QModelIndex();
    searching->clear();
}

void Widget::slot_insertToLineFromList()
{
    current_index_list = model_proxy_sort->mapToSource(view_list->currentIndex());
    model_list->addValue(current_index_list);
    current_index_tree = QModelIndex();
    searching->clear();
}

void Widget::slot_clickSort()
{
    if (btn_sort->isChecked())
    {
        model_proxy_sort->sort(0, Qt::AscendingOrder);
    }
    else
    {
        model_proxy_sort->sort(0, Qt::DescendingOrder);
    }
}

void Widget::slot_seeText()
{
    model_proxy_sort->setFilterRegExp(QRegExp(searching->text(), Qt::CaseInsensitive, QRegExp::FixedString));
    model_proxy_sort->layoutChanged();
}

void Widget::slot_realiseTable()
{
    model_table->row_count= model_proxy_sort->rowCount();

    QObjectList objects_from_list;

    for (int i = 0; i < model_proxy_sort->rowCount(); i++)
    {

        for (int j = 0; j < model_list->data_list.length(); j++)
        {

            if (model_proxy_sort->index(0,0).internalPointer() != nullptr)
            {

                if (model_proxy_sort->data(model_proxy_sort->index(i,0)).toString()
                        == model_list->data(model_list->index(j,0), Qt::DisplayRole).toString())
                {
                    objects_from_list.append(static_cast<QObject*>(model_list->index(j,0).internalPointer()));
                }

            }

        }

    }

    model_table->takeObjects(objects_from_list);
    model_table->setAddres(model_tree->rootItem);
    model_table->layoutChanged();
}

void Widget::slot_listChangeAfterDrop()
{
    if (current_index_list.internalPointer() != nullptr || current_index_tree.internalPointer() != nullptr)
    {
        if (current_index_list.internalPointer() != nullptr)
        {
            model_list->addValue(current_index_list);
        }
        else
        {
            model_list->addValue(current_index_tree);
        }

        model_proxy_sort->layoutChanged();
        searching->clear();
    }

}

void Widget::slot_treeChangeAfterDrop()
{
    QObject *object = static_cast<QObject*>(view_tree->currentIndex().internalPointer());
    QString name_object = object->objectName();
    General *general_object = static_cast<General*>(view_tree->currentIndex().internalPointer());
    QString name_object_by_general = general_object->name_object;

    QObjectList childrens = object->parent()->children();

    int counter_all_common = 0;
    for (int i = 0; i < childrens.size(); i++)
    {

        if (childrens[i]->objectName() == name_object)
        {
            counter_all_common++;
        }

    }

    if (counter_all_common > 1)
    {
        change_name_object = new QDialog(this);
        change_name_object->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt:: CustomizeWindowHint);
        QLabel *please_change = new QLabel (change_name_object);
        name_to_change = new QLineEdit(change_name_object);
        QPushButton *btn_change_text = new QPushButton("Change", change_name_object);

        change_name_object->setLayout(new QVBoxLayout);

        change_name_object->setWindowTitle("Changing name");
        please_change->setText("Change name object, names cannot be repeated! (" + name_object + ")");
        change_name_object->layout()->addWidget(please_change);
        change_name_object->layout()->addWidget(name_to_change);
        change_name_object->layout()->addWidget(btn_change_text);
        change_name_object->show();

        connect(btn_change_text, &QPushButton::clicked, this, &Widget::slot_changeNameAfterDrop);
    }

}

void Widget::slot_treeChangeAfterAdd()
{
    QObject *object = static_cast<QObject*>(view_tree->currentIndex().internalPointer());
    General *general_object = static_cast<General*>(view_tree->currentIndex().internalPointer());

    qDebug()<<object;
    if (object != nullptr)
    {
        QObjectList childrens = object->children();
        for_repeat_objects = new QObject();

        int counter_all_common = 1;
        for (int i = 0; i < childrens.size(); i++)
        {
            QString current_name = childrens[i]->objectName();

            for (int j = i + 1; j < childrens.size(); j++)
            {
                if (current_name == childrens[j]->objectName())
                {
                    counter_all_common++;
                    for_repeat_objects = childrens[j];
                }

            }

        }

        if (counter_all_common > 1)
        {
            change_name_object = new QDialog(this);
            change_name_object->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt:: CustomizeWindowHint);
            QLabel *please_change = new QLabel (change_name_object);
            name_to_change = new QLineEdit(change_name_object);
            QPushButton *btn_change_text = new QPushButton("Change", change_name_object);

            change_name_object->setLayout(new QVBoxLayout);

            change_name_object->setWindowTitle("Changing name");
            please_change->setText("Change name object, names cannot be repeated! (" + for_repeat_objects->objectName() + ")");
            change_name_object->layout()->addWidget(please_change);
            change_name_object->layout()->addWidget(name_to_change);
            change_name_object->layout()->addWidget(btn_change_text);
            change_name_object->show();

            connect(btn_change_text, &QPushButton::clicked, this, &Widget::slot_changeNameAfterAdd);
        }

    }
}

void Widget::slot_changeNameAfterDrop()
{
    QModelIndex index = view_tree->currentIndex();
    QObject *object = static_cast<QObject*>(view_tree->currentIndex().internalPointer());
    General *general_object = static_cast<General*>(view_tree->currentIndex().internalPointer());

    if (name_to_change->text() != "")
    {
        object->setObjectName(name_to_change->text());
        general_object->name_object = name_to_change->text();

        model_list->layoutChanged();
        model_proxy_sort->layoutChanged();

        delete change_name_object;

        model_tree->signal_dragAndDrop();
    }
    else
    {
        delete change_name_object;

        model_tree->signal_dragAndDrop();
    }

    view_tree->setCurrentIndex(index);
}

void Widget::slot_changeNameAfterAdd()
{
    QObject *object = for_repeat_objects;
    QModelIndex index = view_tree->currentIndex();
    General *general_object = static_cast<General*>(view_tree->currentIndex().internalPointer());

    if (name_to_change->text() != "")
    {
        object->setObjectName(name_to_change->text());
        general_object->name_object = name_to_change->text();

        model_list->layoutChanged();
        model_proxy_sort->layoutChanged();

        delete change_name_object;

        model_tree->signal_addItem();
    }
    else
    {
        delete change_name_object;

        model_tree->signal_addItem();
    }

    view_tree->setCurrentIndex(index);
}






