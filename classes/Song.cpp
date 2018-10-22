//
// Created by fra on 12/10/18.
//

#include "Song.hpp"

const AlbumInfo &Song::album() const { return this->albumInfo_; }

std::string Song::title() const { return this->title_; }

const ArtistInfo &Song::artist() const { return this->artistInfo_; }

ulong Song::id() const { return this->song_id_; }

uint Song::year() const { return this->year_; }

uint Song::trackNumber() const { return this->track_no_; }

json11::Json Song::toJson() const {
  json11::Json jsonSong = json11::Json::object{
      {"id", std::to_string(this->id())},
      {"album", this->albumInfo_.toJson()},
      {"artist", this->artistInfo_.toJson()},

      {"title", this->title()},
      {"year", std::to_string(this->year())},
      {"track_number", std::to_string(this->trackNumber())}};

  return jsonSong;
}

Song *Song::fromJson(QJsonValue json) {
  if (json.isObject()) {
    auto jObj = json.toObject();
    if (jObj.contains("id") && jObj.contains("title")) {
      bool ok{false};
      ulong id = jObj["id"].toString().toULong(&ok);
      if (!ok)
        return nullptr;

      std::unique_ptr<AlbumInfo> albumInfo_p{
          AlbumInfo::fromJson(jObj["album"])};
      std::unique_ptr<ArtistInfo> artistInfo_p{
          ArtistInfo::fromJson(jObj["artist"])};

      return new Song{id, jObj["title"].toString().toStdString(),
          jObj["track_number"].toString().toUInt(),
          jObj["year"].toString().toUInt(), *albumInfo_p,
                           *artistInfo_p};

    }
  }
  return nullptr;
}
