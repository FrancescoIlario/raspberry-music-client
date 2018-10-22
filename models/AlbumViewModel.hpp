#ifndef ALBUMVIEWMODEL_HPP
#define ALBUMVIEWMODEL_HPP

#include "ArtistViewModel.hpp"
#include "rest/MusicServer.hpp"
#include <classes/AlbumInfo.hpp>

#include <QAbstractItemModel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class AlbumVModel : public QAbstractItemModel {
  Q_OBJECT
private:
  ulong artistId_;
  std::vector<AlbumInfo> data_{};

public:
  AlbumVModel() = default;

  // QAbstractItemModel interface
public:
  QModelIndex index(int row, int column, const QModelIndex &parent) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent) const;
  int columnCount(const QModelIndex &parent) const;
  QVariant data(const QModelIndex &index, int role) const;

public:

  void addData(const std::vector<AlbumInfo> &data, bool sort = true);
  void addData(const AlbumInfo &data, bool sort = false);
  AlbumInfo getAlbumInfo(const QModelIndex &index) const;
  ulong getArtistId() const;

public slots:
  void updateDataModel(const QModelIndex &);
};
#endif // ALBUMVIEWMODEL_HPP
