#include "ArtistViewModel.hpp"

QModelIndex ArtistVModel::index(int row, int column,
                                const QModelIndex &parent) const {
  (void)(parent);
  return createIndex(row, column);
}

QModelIndex ArtistVModel::parent(const QModelIndex &child) const {
  (void)(child);
  return QModelIndex{};
}

int ArtistVModel::rowCount(const QModelIndex &parent) const {
  (void)(parent);
  auto data_size = static_cast<int>(this->data_.size());
  return data_size;
}

int ArtistVModel::columnCount(const QModelIndex &parent) const {
  (void)(parent);
  return 1;
}

QVariant ArtistVModel::data(const QModelIndex &index, int role) const {
  if (index.isValid()) {
    if (role == Qt::DisplayRole) {
      return QString{
          this->data_.at(static_cast<size_t>(index.row())).name().c_str()};
    }
  }
  return QVariant{};
}

void ArtistVModel::addData(Artist data, bool sort) {
  this->data_.push_back(data);
  auto end = static_cast<int>(this->data_.size());
  this->beginInsertRows(QModelIndex{}, sort ? 0 : end - 1, end);
  if (sort)
    std::sort(this->data_.begin(), this->data_.end(),
              [](const Artist &a1, const Artist &a2) {
                return QString{a1.name().c_str()}.toLower() <
                       QString{a2.name().c_str()}.toLower();
              });
  this->endInsertRows();
}

void ArtistVModel::addData(const std::vector<Artist> &data, bool sort) {
  this->data_.insert(this->data_.end(), data.begin(), data.end());
  auto start = sort ? 0 : static_cast<int>(this->data_.size() - data.size());
  auto end = static_cast<int>(this->data_.size());
  this->beginInsertRows(QModelIndex{}, start, end);
  if (sort)
    std::sort(this->data_.begin(), this->data_.end(),
              [](const Artist &a1, const Artist &a2) {
                return QString{a1.name().c_str()}.toLower() <
                       QString{a2.name().c_str()}.toLower();
              });
  this->endInsertRows();
}

ulong ArtistVModel::getArtistId(const QModelIndex &index) const {
  if (index.isValid()) {
    return this->data_.at(static_cast<size_t>(index.row())).id();
  }
  return 0;
}

ArtistInfo *ArtistVModel::getArtistInfo(const QModelIndex &index) const {
  if (index.isValid()) {
    return new ArtistInfo{
        this->data_.at(static_cast<size_t>(index.row())).artistInfo()};
  }
  return nullptr;
}
