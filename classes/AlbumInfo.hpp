//
// Created by fra on 14/10/18.
//

#ifndef MUSICSERVER_ALBUMINFO_HPP
#define MUSICSERVER_ALBUMINFO_HPP

#include <string>

#include <QJsonObject>
#include <QObject>
#include <json11.hpp>

class AlbumInfo {

private:
  ulong id_;
  std::string title_;
  uint year_;

public:
  AlbumInfo() = delete;

  AlbumInfo(ulong id, const std::string &title, uint year = 0)
      : id_{id}, title_{title}, year_{year} {
    if (this->title_.empty())
      this->title_ = "Unknown Album";
  }

  AlbumInfo(const AlbumInfo &) = default;
  AlbumInfo &operator=(const AlbumInfo &) = default;

  AlbumInfo(AlbumInfo &&) = default;
  AlbumInfo &operator=(AlbumInfo &&) = default;

  std::string title() const;

  ulong id() const;

  uint year() const;

  json11::Json toJson() const;

  static AlbumInfo *fromJson(const QJsonValue &);
};

#endif // MUSICSERVER_ALBUMINFO_HPP
