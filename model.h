#include <QAbstractTableModel>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QFont>
#include <QBrush>

class MyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MyModel(QObject *parent);
    QTimer *timer;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    void addData(const std::string& text);

private:
    QList<std::string> m_dataList;
};
