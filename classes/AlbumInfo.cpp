//
// Created by fra on 14/10/18.
//

#include "AlbumInfo.hpp"

std::string AlbumInfo::title() const { return this->title_; }

ulong AlbumInfo::id() const { return this->id_; }

uint AlbumInfo::year() const { return this->year_; }

json11::Json AlbumInfo::toJson() const {
  json11::Json albumJson =
      json11::Json::object{{"id", std::to_string(this->id())},
                           {"title", this->title()},
                           {"year", std::to_string(this->year())}};
  return albumJson;
}

AlbumInfo *AlbumInfo::fromJson(const QJsonValue &jval) {
  if (jval.isObject()) {
    auto jobj = jval.toObject();

    if (jobj.contains("id") && jobj.contains("title")) {
      bool ok{false};
      auto id = jobj.value("id").toString().toULong(&ok);
      if (!ok)
        return nullptr;

      auto title = jobj.value("title").toString();
      auto year = jobj.value("year").toString().toUInt(&ok);

      return new AlbumInfo{id, title.toUtf8().toStdString(), ok ? year : 0};
    }
  }

  return nullptr;
}
