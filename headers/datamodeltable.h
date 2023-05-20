#ifndef DATAMODELTABLE_H
#define DATAMODELTABLE_H

#include <QAbstractItemModel>
#include <QDebug>

#include "connector.h"
#include "singleton.h"

class DataModelTable : public QAbstractTableModel
{
public:
    QObjectList data_table;
    int row_count=0;
    
    DataModelTable(int rows, QObject *parent = nullptr);
    
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    
    void takeObjects(QObjectList object_from_list);
    void setAddres(Singleton &rootItem);
    
protected:
    QStringList property_column;
    QHash<QModelIndex,QVariant> cell_data;
};

#endif // DATAMODELTABLE_H
