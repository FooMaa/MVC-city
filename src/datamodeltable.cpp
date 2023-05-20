#include "headers/datamodeltable.h"

DataModelTable::DataModelTable(int rows, QObject *parent)
    : QAbstractTableModel(parent), row_count(rows)
{
    property_column.append("Address");
    property_column.append("Property");
}

int DataModelTable::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return row_count;
}

int DataModelTable::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return property_column.length();
}

QVariant DataModelTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return cell_data.value(index,QVariant());
    }

    return QVariant();
}

bool DataModelTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
    {
        return false;
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        cell_data.insert(index, value);

        emit dataChanged(index, index);
    }

    return false;
}

QVariant DataModelTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    if (orientation == Qt::Horizontal)
    {
        return property_column.at(section);
    }
    else
    {
        return data_table.at(section)->objectName();
    }
}

void DataModelTable::takeObjects(QObjectList obj)
{
    data_table = obj;
}

void DataModelTable::setAddres(Singleton &rootItem)
{
    QString rule = "/";
    QObject *object;
    int *count_columns = new int[this->data_table.length()];
    int count_parent = 0;

    for (int i = 0; i < this->data_table.length(); i++)
    {
        object = this->data_table.at(i);

        while (object->parent() != static_cast<QObject*>(&rootItem))
        {
            rule="/" + object->parent()->objectName() + rule;
            object = object->parent();
            count_parent++;
        }

        count_columns[i] = count_parent;
        count_parent = 0;
        setData(index(i, 0), rule, Qt::EditRole);
        rule = "/";
    }

    for(int i = 0; i < this->data_table.length(); i++)
    {

        switch (count_columns[i])
        {
           case type_City:
               setData(index(i, 1), static_cast<City*>(this->data_table.at(i))->generated_property, Qt::EditRole);
               break;

           case type_Street:
               setData(index(i, 1), static_cast<City*>(this->data_table.at(i))->generated_property, Qt::EditRole);
               break;

           case type_Home:
               setData(index(i, 1), static_cast<City*>(this->data_table.at(i))->generated_property, Qt::EditRole);
               break;

           case type_People:
               setData(index(i, 1), static_cast<City*>(this->data_table.at(i))->generated_property, Qt::EditRole);
               break;
        }

    }
}
