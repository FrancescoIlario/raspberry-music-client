#ifndef ARTISTVIEWMODEL_HPP
#define ARTISTVIEWMODEL_HPP

#include <QAbstractItemModel>
#include <QDebug>

#include <classes/Artist.hpp>

class ArtistVModel : public QAbstractItemModel {
  Q_OBJECT
private:
  std::vector<Artist> data_{};

public:
  ArtistVModel() = default;

  // QAbstractItemModel interface
public:
  QModelIndex index(int row, int column, const QModelIndex &parent) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;

public:
  void addData(Artist data, bool sort = false);
  void addData(const std::vector<Artist> &data, bool sort = true);
  ulong getArtistId(const QModelIndex &index) const;
  ArtistInfo *getArtistInfo(const QModelIndex &index) const;
};

#endif // ARTISTVIEWMODEL_HPP
