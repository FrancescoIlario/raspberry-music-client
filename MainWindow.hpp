#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTemporaryFile>

#include <iostream>
#include <memory>
#include <vector>

#include "ui_MainWindow.h"

#include "rest/MusicServer.hpp"
#include <classes/Album.hpp>
#include <classes/AlbumInfo.hpp>
#include <classes/Artist.hpp>
#include <models/AlbumViewModel.hpp>
#include <models/ArtistViewModel.hpp>
#include <models/SongViewModel.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

private:
  ulong playingSongId_{0};
  QByteArray *playingSongData_{nullptr};

  QMediaPlayer player_{this, QMediaPlayer::StreamPlayback};
  QList<Song> playlistSongs_{};

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;

public slots:
  void playPause();
  void next();
  void previous();
  void playSong(const QModelIndex &);

  void stateChanged(QMediaPlayer::State state);
};

#endif // MAINWINDOW_HPP
