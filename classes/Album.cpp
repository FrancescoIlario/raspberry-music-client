//
// Created by fra on 12/10/18.
//

#include "Album.hpp"
#include <iostream>
#include <json11.hpp>

uint Album::year() const { return this->albumInfo_.year(); }

std::string Album::title() const { return this->albumInfo_.title(); }

std::vector<Song> Album::songs() const { return this->songs_; }

json11::Json Album::toJson(bool songs) {
  auto jsonAlbum = this->albumInfo().toJson();
  auto jsonArtist = this->artistInfo().toJson();

  auto jsonComplete = json11::Json::object{{"artist_info", jsonArtist},
                                           {"album_info", jsonAlbum}};
  if (songs) {
    json11::Json::array jsonSongs{};
    for (auto &song : this->songs()) {
      std::cout << "Json pushed " << song.title() << std::endl;
      jsonSongs.push_back(song.toJson());
    }
    jsonComplete["songs"] = jsonSongs;
  }

  return std::move(jsonComplete);
}

Album *Album::fromJson(const QJsonValue &jVal) {
  if (!jVal.isObject())
    return nullptr;

  auto jObj = jVal.toObject();
  if (!jObj.contains("album_info") || !jObj.contains("artist_info")) {
    qWarning("AlbumInfo or ArtistInfo not present %s",
             jVal.toString().toStdString().c_str());
    return nullptr;
  }

  std::unique_ptr<AlbumInfo> albumInfo{AlbumInfo::fromJson(jObj["album_info"])};
  std::unique_ptr<ArtistInfo> artistInfo{
      ArtistInfo::fromJson(jObj["artist_info"])};

  if (!albumInfo || !artistInfo) {
    qWarning("AlbumInfo or ArtistInfo not valid %s",
             jVal.toString().toStdString().c_str());
    return nullptr;
  }

  std::vector<Song> songs{};
  if (jObj.contains("songs") && jObj["songs"].isArray()) {
    auto jSongs = jObj["songs"].toArray();
    for (auto jRawSong : jSongs) {
      auto jSong = jRawSong.toObject();
      std::unique_ptr<Song> song{Song::fromJson(jSong)};
      songs.push_back(std::move(*song));
    }
  }

  auto album = new Album{*albumInfo, *artistInfo, songs};
  return album;
}

ulong Album::id() const { return this->albumInfo_.id(); }

void Album::addSong(Song &song) { this->songs_.push_back(song); }

const ArtistInfo &Album::artistInfo() const { return this->artistInfo_; }

const AlbumInfo &Album::albumInfo() const { return this->albumInfo_; }
