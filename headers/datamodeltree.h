#ifndef DATAMODELTREE_H
#define DATAMODELTREE_H

#include <QObject>
#include <QAbstractItemModel>
#include <QStringList>
#include <QObjectList>
#include <QMimeData>
#include <QDataStream>
#include <QCoreApplication>

#include "singleton.h"

class DataModelTree : public QAbstractItemModel
{
    Q_OBJECT
public:
    Singleton &rootItem = Singleton::get_instance();

    DataModelTree(QObject *parent);
    
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    Qt::DropActions supportedDropActions() const;
    Qt::DropActions supportedDragActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    template<typename CHILD, typename T1> void addValue (CHILD *item, const QModelIndex &parentIdx);
    template <typename OBJECT> OBJECT* objByIndex (const QModelIndex &index) const;
    
    int seeParent(QObject  *child = nullptr, QModelIndex index = QModelIndex());
    
    
signals:
    void signal_dragAndDrop();
    void signal_addItem();
};

template <typename CHILD, typename PARENT>
void DataModelTree::addValue(CHILD *item, const QModelIndex &parentIdx)
{
   beginInsertRows(parentIdx, rowCount(parentIdx), rowCount(parentIdx));
   
   if (objByIndex<PARENT>(parentIdx) == nullptr)
   {
     item->setParent(&rootItem);
   }
   else
   {
     item->setobjectName(item->names_obj);
   }
   
   endInsertRows();
   emit layoutChanged();
}

template <typename OBJECT>
OBJECT *DataModelTree::objByIndex(const QModelIndex &index) const
{
   if (!index.isValid())
   {
      return 0;
   }
   
   return static_cast<OBJECT*>(index.internalPointer());
}


#endif // DATAMODELTREE_H
