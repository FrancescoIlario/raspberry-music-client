//
// Created by fra on 12/10/18.
//

#ifndef MUSICSERVER_ALBUM_HPP
#define MUSICSERVER_ALBUM_HPP

#include "AlbumInfo.hpp"
#include "Song.hpp"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QObject>
#include <vector>

class Album {
private:
  AlbumInfo albumInfo_;
  ArtistInfo artistInfo_;
  std::vector<Song> songs_{};

public:
  Album() = delete;

  Album(const AlbumInfo &albumInfo, const ArtistInfo &artistInfo,
        const std::vector<Song> &songs = {})
      : albumInfo_{albumInfo}, artistInfo_{artistInfo}, songs_{songs} {}

  Album(const Album &) = default;

  Album &operator=(const Album &) = default;

  Album(Album &&) = default;

  Album &operator=(Album &&) = default;

  std::vector<Song> songs() const;

  std::string title() const;

  uint year() const;

  ulong id() const;

  const AlbumInfo &albumInfo() const;

  const ArtistInfo &artistInfo() const;

  json11::Json toJson(bool songs = false);

      static Album *fromJson(const QJsonValue &obj);
  // internal
  void addSong(Song &song);
};

#endif // MUSICSERVER_ALBUM_HPP
