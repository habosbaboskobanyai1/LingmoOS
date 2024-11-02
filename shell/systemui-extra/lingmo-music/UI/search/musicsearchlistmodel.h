#pragma once

#include <QStandardItemModel>
#include <QScopedPointer>

class MusicSearchListModel : public QStandardItemModel
{
    Q_OBJECT
public:
    MusicSearchListModel(int rows, int columns, QObject *parent = Q_NULLPTR);
    ~MusicSearchListModel() override;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    //virtual Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
};

