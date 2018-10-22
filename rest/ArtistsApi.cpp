#include "ArtistsApi.hpp"

#include <QByteArray>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPointer>
#include <QUrl>

#include "MusicServer.hpp"

std::vector<ArtistInfo> ArtistsApi::getArtists() const {
  return {};
}
