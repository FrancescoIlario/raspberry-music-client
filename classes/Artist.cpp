//
// Created by fra on 12/10/18.
//

#include "Artist.hpp"
#include <algorithm>

#include <iostream>

ulong Artist::id() const { return this->artistInfo_.id(); }

std::string Artist::name() const { return this->artistInfo_.name(); }

void Artist::addAlbum(AlbumInfo albumInfo) {
  this->albumsInfo_.push_back(std::move(albumInfo));
}

std::string Artist::toJson(bool albumsInfo) {
  json11::Json jsonArtist = json11::Json::object{
      {"name", this->name()}, {"id", std::to_string(this->id())}};

  if (albumsInfo) {
    json11::Json::array jsonAlbums;
    for (auto &album : this->albumsInfo_)
      jsonAlbums.push_back(album.toJson());
  }

  return jsonArtist.dump();
}

Artist *Artist::fromJson(QJsonValue jsonValue) {
  auto name = jsonValue["name"].toString();
  auto id = jsonValue["id"];

  if (id.isNull())
    return nullptr;

  bool ok;
  auto uid = id.toString().toULong(&ok);

  return ok ? new Artist{uid, name.toUtf8().toStdString()} : nullptr;
}

std::vector<AlbumInfo> &Artist::albumsInfo() { return this->albumsInfo_; }

const ArtistInfo &Artist::artistInfo() const { return this->artistInfo_; }
