//
// Created by fra on 14/10/18.
//

#ifndef MUSICSERVER_ARTISTINFO_HPP
#define MUSICSERVER_ARTISTINFO_HPP

#include <QJsonObject>
#include <QJsonValue>
#include <json11.hpp>
#include <qobject.h>
#include <string>

class ArtistInfo {
private:
  ulong id_;
  std::string name_;

public:
  ArtistInfo() = delete;

  ArtistInfo(ulong id, const std::string &name) : id_{id}, name_{name} {
    if (this->name_.empty())
      this->name_ = "Unknown Artist";
  }

  ArtistInfo(const ArtistInfo &) = default;
  ArtistInfo &operator=(const ArtistInfo &) = default;

  ArtistInfo(ArtistInfo &&) = default;
  ArtistInfo &operator=(ArtistInfo &&) = default;

  ulong id() const;

  std::string name() const;

  json11::Json toJson() const;

  static ArtistInfo *fromJson(const QJsonValue&);
};

#endif // MUSICSERVER_ARTISTINFO_HPP
