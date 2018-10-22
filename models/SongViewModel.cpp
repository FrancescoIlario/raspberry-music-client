#include "SongViewModel.hpp"
#include "AlbumViewModel.hpp"
#include "ArtistViewModel.hpp"

#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QUrl>

#include <classes/Album.hpp>
#include <classes/Song.hpp>

QModelIndex SongVModel::index(int row, int column,
                              const QModelIndex &parent) const {
  (void)(parent);
  return createIndex(row, column);
}

QModelIndex SongVModel::parent(const QModelIndex &child) const {
  (void)(child);
  return QModelIndex{};
}

int SongVModel::rowCount(const QModelIndex &parent) const {
  (void)(parent);
  auto data_size = static_cast<int>(this->data_.size());
  return data_size;
}

int SongVModel::columnCount(const QModelIndex &parent) const {
  (void)(parent);
  return 1;
}

QVariant SongVModel::data(const QModelIndex &index, int role) const {
  if (index.isValid()) {
    if (role == Qt::DisplayRole) {
      return QString{
          this->data_.at(static_cast<size_t>(index.row())).title().c_str()};
    }
  }
  return QVariant{};
}

void SongVModel::addData(Song data, bool sort) {
  this->data_.push_back(data);
  auto end = static_cast<int>(this->data_.size());
  this->beginInsertRows(QModelIndex{}, sort ? 0 : end - 1, end);
  if (sort)
    std::sort(this->data_.begin(), this->data_.end(),
              [](const Song &a1, const Song &a2) {
                return a1.trackNumber() < a2.trackNumber();
              });
  this->endInsertRows();
}

void SongVModel::addData(const std::vector<Song> &data, bool sort) {
  this->data_.insert(this->data_.end(), data.begin(), data.end());
  auto start = sort ? 0 : static_cast<int>(this->data_.size() - data.size());
  auto end = static_cast<int>(this->data_.size());
  this->beginInsertRows(QModelIndex{}, start, end);
  if (sort)
    std::sort(this->data_.begin(), this->data_.end(),
              [](const Song &a1, const Song &a2) {
                return a1.trackNumber() < a2.trackNumber();
              });
  this->endInsertRows();
}

Song *SongVModel::getSong(const QModelIndex &index) const {
  return index.isValid()
             ? new Song{this->data_.at(static_cast<size_t>(index.row()))}
             : nullptr;
}

std::vector<Song> SongVModel::getSongs(int from) const {
  std::vector<Song> songs{};
  if (from >= 0 && static_cast<ulong>(from) < this->data_.size()) {
    for (auto it = this->data_.begin() + from; it != this->data_.end(); ++it) {
      songs.push_back(*it);
    }
  }
  return songs;
}

void SongVModel::updateDataModel(const QModelIndex &index) {
  auto albumModel = dynamic_cast<const AlbumVModel *>(index.model());
  auto albumInfo = albumModel->getAlbumInfo(index);
  auto artistId = albumModel->getArtistId();

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
          qWarning("Could not parse response: List of Albums: %s",
                   answer.toStdString().c_str());
          return;
        }

        qDebug() << "Songs Answer: " << answer.toStdString().c_str();

        this->data_.clear();
        std::unique_ptr<Album> album{Album::fromJson(document.object())};

        if (album) {
          uint count{0};
          auto end{album->songs().size()};
          for (auto &song : album->songs()) {
            this->addData(song, ++count == end);
            qDebug() << "Added song " << song.title().c_str();
          }

          emit dataChanged(this->index(0, 0, QModelIndex()),
                           this->index(static_cast<int>(this->data_.size()), 0,
                                       QModelIndex()));
        }
      });
  QUrl url;
  url.setHost(SERVER_NAME);
  QString path{"/artist/%1/album/%2"};
  url.setPath(
      path.arg(QString::number(artistId), QString::number(albumInfo.id())));
  url.setPort(SERVER_PORT);
  url.setScheme("http");

  qDebug() << url;
  QNetworkRequest request{url};
  manager->get(request);
}

void SongVModel::clearData() {
  auto ds = static_cast<int>(this->data_.size());
  this->data_.clear();
  emit dataChanged(this->index(0, 0, QModelIndex()),
                   this->index(ds, 0, QModelIndex()));
}
