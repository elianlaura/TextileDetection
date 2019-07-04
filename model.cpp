#include "model.h"

MyModel::MyModel(QObject *parent)
    :QAbstractTableModel(parent)
{
}

int MyModel::rowCount(const QModelIndex & /*parent*/) const
{
   return m_dataList.size();
}

int MyModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 5;
}

Qt::ItemFlags MyModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool MyModel::insertRows(int row, int count, const QModelIndex & /*parent*/)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    /*for (auto i = 0; i < count; ++i)
        m_dataList.insert(row, "aaa");
    endInsertRows();*/
    return true;
}


bool MyModel::removeRows(int row, int count, const QModelIndex &)
{
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    /*for (auto i = 0; i < count; ++i)
        m_dataList.removeAt(row);
    endRemoveRows();*/
    return true;
}

void MyModel::addData(const std::string &text){
    this->m_dataList.append(text);
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
        int col = index.column();

        // generate a log message when this method gets called
        qDebug() << QString("row %1, col%2, role %3")
                .arg(row).arg(col).arg(role);

        switch(role){
        case Qt::DisplayRole:
            if (row == 0 && col == 1) return QString("<--left");
            if (row == 1 && col == 1) return QString("right-->");

            return QString("Row%1, Column%2")
                    .arg(row + 1)
                    .arg(col +1);
            break;
        }


        return QVariant();
}

QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role == Qt::DisplayRole)
        {
            if (orientation == Qt::Horizontal) {
                switch (section)
                {
                case 0:
                    return QString("first");
                case 1:
                    return QString("second");
                case 2:
                    return QString("third");
                }
            }
        }
        return QVariant();
}

