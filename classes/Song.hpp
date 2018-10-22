//
// Created by fra on 12/10/18.
//

#ifndef MUSICSERVER_SONG_HPP
#define MUSICSERVER_SONG_HPP

#include <QJsonObject>

#include "AlbumInfo.hpp"
#include "ArtistInfo.hpp"
#include <json11.hpp>
#include <string>

class Song {
private:
  ulong song_id_;
  std::string title_;
  uint track_no_, year_;

  ArtistInfo artistInfo_;
  AlbumInfo albumInfo_;

public:
  Song() = delete;

  Song(ulong song_id, const std::string &title, uint year, uint track_no,
       const AlbumInfo &albumInfo, const ArtistInfo &artistInfo)
      : song_id_{song_id}, title_{title}, track_no_{track_no}, year_{year},
        artistInfo_{artistInfo}, albumInfo_{albumInfo} {
    if (this->title_.empty())
      this->title_ = "Unknown Song";
  }

  std::string title() const;

  const AlbumInfo &album() const;

  const ArtistInfo &artist() const;

  ulong id() const;

  uint year() const;

  uint trackNumber() const;

  json11::Json toJson() const;

  static Song *fromJson(QJsonValue json);
};

#endif // MUSICSERVER_SONG_HPP
