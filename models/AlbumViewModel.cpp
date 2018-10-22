#include "AlbumViewModel.hpp"

#include <QFrame>

QModelIndex AlbumVModel::index(int row, int column,
                               const QModelIndex &parent) const {
  (void)(parent);
  return createIndex(row, column);
}

QModelIndex AlbumVModel::parent(const QModelIndex &child) const {
  (void)(child);
  return QModelIndex{};
}

int AlbumVModel::rowCount(const QModelIndex &parent) const {
  (void)(parent);
  auto data_size = static_cast<int>(this->data_.size());
  return data_size;
}

int AlbumVModel::columnCount(const QModelIndex &parent) const {
  (void)(parent);
  return 1;
}

QVariant AlbumVModel::data(const QModelIndex &index, int role) const {
  if (index.isValid()) {
    if (role == Qt::DisplayRole) {
      auto indexRow = static_cast<size_t>(index.row());
      auto albumAt = this->data_.at(indexRow);
      return QString{albumAt.title().c_str()};
    }
  }
  return QVariant{};
}

void AlbumVModel::addData(const std::vector<AlbumInfo> &data, bool sort) {
  this->data_.insert(this->data_.end(), data.begin(), data.end());
  auto start = sort ? 0 : static_cast<int>(this->data_.size() - data.size());
  auto end = static_cast<int>(this->data_.size());
  this->beginInsertRows(QModelIndex{}, start, end);
  if (sort)
    std::sort(this->data_.begin(), this->data_.end(),
              [](const AlbumInfo &a1, const AlbumInfo &a2) {
                return QString{a1.title().c_str()}.toLower() <
                       QString{a2.title().c_str()}.toLower();
              });
  this->endInsertRows();
}

void AlbumVModel::addData(const AlbumInfo &data, bool sort) {
  this->data_.push_back(data);
  auto start = sort ? 0 : static_cast<int>(this->data_.size() - 1);
  auto end = static_cast<int>(this->data_.size());
  this->beginInsertRows(QModelIndex{}, start, end);
  if (sort)
    std::sort(this->data_.begin(), this->data_.end(),
              [](const AlbumInfo &a1, const AlbumInfo &a2) {
                return QString{a1.title().c_str()}.toLower() <
                       QString{a2.title().c_str()}.toLower();
              });
  this->endInsertRows();
}

AlbumInfo AlbumVModel::getAlbumInfo(const QModelIndex &index) const {
  return this->data_.at(static_cast<size_t>(index.row()));
}

ulong AlbumVModel::getArtistId() const { return this->artistId_; }

void AlbumVModel::updateDataModel(const QModelIndex &modelIndex) {
  auto artistModel = dynamic_cast<const ArtistVModel *>(modelIndex.model());
  auto artistId = artistModel->getArtistId(modelIndex);
  this->artistId_ = artistId;

  // clean the view
  auto ds = static_cast<int>(this->data_.size());
  this->data_.clear();
  emit dataChanged(this->index(0, 0, QModelIndex()),
                   this->index(ds, 0, QModelIndex()));

  // request handler
  auto manager = new QNetworkAccessManager();
  QObject::connect(
      manager, &QNetworkAccessManager::finished, this,
      [=](QNetworkReply *reply) {
        if (reply->error()) {
          qDebug() << reply->errorString();
          return;
        }

        auto answer = reply->readAll();
        QJsonDocument document{QJsonDocument::fromJson(answer)};
        if (document.isNull()) {
          qWarning("Could not parse response: List of Albums");
          return;
        }

        int count{0};
        QJsonArray dataArray = document.array();
        this->data_.clear();
        for (auto data : dataArray) {
          std::unique_ptr<AlbumInfo> album{AlbumInfo::fromJson(data)};
          this->addData(*album, ++count == dataArray.size());
        }

        emit dataChanged(this->index(0, 0, QModelIndex()),
                         this->index(static_cast<int>(this->data_.size()), 0,
                                     QModelIndex()));
      });
  QUrl url;
  url.setHost(SERVER_NAME);
  QString path{"/artist/%1/album"};
  url.setPath(path.arg(artistId));
  url.setPort(SERVER_PORT);
  url.setScheme("http");

  qDebug() << url;
  QNetworkRequest request{url};
  manager->get(request);
}
