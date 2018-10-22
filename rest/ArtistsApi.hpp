#ifndef ARTISTSAPI_HPP
#define ARTISTSAPI_HPP

#include "classes/ArtistInfo.hpp"
#include <vector>

class ArtistsApi {
public:
  ArtistsApi() = default;

  std::vector<ArtistInfo> getArtists() const;
};

#endif // ARTISTSAPI_HPP
