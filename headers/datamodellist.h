#ifndef DATAMODELLIST_H
#define DATAMODELLIST_H

#include "datamodeltree.h"

class DataModelList : public DataModelTree
{
public:
    QObjectList data_list;
    
    DataModelList(QObject *parent);

    QModelIndex index(int row, int column, const QModelIndex &parent =QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void sort(int column, Qt::SortOrder order);

    
public slots:
    void addValue(const QModelIndex &index);

};

#endif // DATAMODELLIST_H
