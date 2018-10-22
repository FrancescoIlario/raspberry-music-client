#ifndef SONGVIEWMODEL_HPP
#define SONGVIEWMODEL_HPP

#include <QAbstractItemModel>
#include <QDebug>

#include <classes/Song.hpp>

class SongVModel : public QAbstractItemModel {
  Q_OBJECT
private:
  std::vector<Song> data_{};

public:
  SongVModel() = default;

  // QAbstractItemModel interface
public:
  QModelIndex index(int row, int column, const QModelIndex &parent) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;

public:
  void addData(Song data, bool sort = false);
  void addData(const std::vector<Song> &data, bool sort = true);
  Song *getSong(const QModelIndex &index) const;

  std::vector<Song> getSongs(int from=0) const;


public slots:
  void updateDataModel(const QModelIndex &);

  void clearData();
};

#endif // ARTISTVIEWMODEL_HPP
