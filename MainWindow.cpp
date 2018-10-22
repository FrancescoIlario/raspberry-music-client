#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  auto artistModel = new ArtistVModel();
  ui->artistsView->setModel(artistModel);

  auto albumModel = new AlbumVModel();
  ui->albumsView->setModel(albumModel);

  auto songsModel = new SongVModel();
  ui->songsView->setModel(songsModel);

  // ListViews Signals
  QObject::connect(ui->artistsView, SIGNAL(clicked(const QModelIndex &)),
                   albumModel, SLOT(updateDataModel(const QModelIndex &)));

  QObject::connect(ui->albumsView, SIGNAL(clicked(const QModelIndex &)),
                   songsModel, SLOT(updateDataModel(const QModelIndex &)));

  QObject::connect(ui->artistsView, SIGNAL(clicked(const QModelIndex &)),
                   songsModel, SLOT(clearData()));

  QObject::connect(ui->songsView, SIGNAL(doubleClicked(const QModelIndex &)),
                   this, SLOT(playSong(const QModelIndex &)));

  // Buttons Signals
  QObject::connect(ui->nextButton, SIGNAL(clicked(bool)), this, SLOT(next()));
  QObject::connect(ui->playPauseButton, SIGNAL(clicked(bool)), this,
                   SLOT(playPause()));
  QObject::connect(ui->previousButton, SIGNAL(clicked(bool)), this,
                   SLOT(previous()));

  QObject::connect(&this->player_, SIGNAL(stateChanged(QMediaPlayer::State)),
                   this, SLOT(stateChanged(QMediaPlayer::State)));

  // Slider Signalsr
  QObject::connect(&this->player_, &QMediaPlayer::positionChanged, this,
                   [&](const qint64 position) {
                     auto _p = static_cast<int>(position);
                     if (position == _p) {
                       this->ui->horizontalSlider->setValue(_p);
                       QTime time{0, 0, 0, 0};
                       time = time.addMSecs(_p);
                       QString time_representation =
                           (time.hour() == 0) ? time.toString("mm:ss")
                                              : time.toString("HH:mm:ss");
                       this->ui->elapsedTimeLabel->setText(time_representation);
                     }
                   });

  QObject::connect(
      &this->player_, &QMediaPlayer::durationChanged, this,
      [&](const qint64 duration) {
        auto _d = static_cast<int>(duration);
        if (duration == _d) {
          this->ui->horizontalSlider->setMaximum(_d);
            QTime time{0, 0, 0, 0};
            time = time.addMSecs(_d);
          QString time_representation = (time.hour() == 0)
                                            ? time.toString("mm:ss")
                                            : time.toString("HH:mm:ss");
          this->ui->songDurationLabel->setText(time_representation);
        }
      });

  // Label Signal
  QObject::connect(&this->player_, &QMediaPlayer::mediaChanged, this, [&]() {
    auto playlist = this->player_.playlist();
    if (playlist) {
      auto current_index = playlist->currentIndex();
      Song song = this->playlistSongs_.at(current_index);
      QString text{};
      text = text.append(song.artist().name().c_str())
                 .append(" - ")
                 .append(song.album().title().c_str())
                 .append(" - ")
                 .append(song.title().c_str());

      this->ui->playingSongLabel->setText(text);
    }
  });

  // ask for Artists list
  auto manager = new QNetworkAccessManager();
  QObject::connect(manager, &QNetworkAccessManager::finished, this,
                   [=](QNetworkReply *reply) {
                     if (reply->error()) {
                       qDebug() << reply->errorString();
                       return;
                     }

                     auto answer = reply->readAll();
                     QJsonDocument document{QJsonDocument::fromJson(answer)};
                     if (document.isNull()) {
                       qWarning("Could not parse response: List of Artists");
                       return;
                     }

                     int count{0};
                     QJsonArray dataArray = document.array();
                     auto aModel =
                         dynamic_cast<ArtistVModel *>(ui->artistsView->model());
                     for (auto data : dataArray) {
                       std::unique_ptr<Artist> artist{Artist::fromJson(data)};
                       if (artist) {
                         aModel->addData(*artist, ++count == dataArray.size());
                       }
                     }
                   });
  QUrl url;
  url.setHost(SERVER_NAME);
  url.setPath("/artist");
  url.setPort(SERVER_PORT);
  url.setScheme("http");

  qDebug() << url;
  QNetworkRequest request{url};
  manager->get(request);
}

MainWindow::~MainWindow() {
  delete ui;
  delete playingSongData_;
}

void MainWindow::playPause() {
  switch (player_.state()) {
  case QMediaPlayer::State::PausedState:
    this->player_.play();
    break;
  case QMediaPlayer::State::PlayingState:
    this->player_.pause();
    break;
  case QMediaPlayer::State::StoppedState:
    qDebug() << "The Player is stopped";
    break;
  }
}

void MainWindow::next() { this->player_.playlist()->next(); }

void MainWindow::previous() { this->player_.playlist()->previous(); }

void MainWindow::playSong(const QModelIndex &index) {
  auto model = static_cast<SongVModel *>(ui->songsView->model());
  std::unique_ptr<Song> song_p{model->getSong(index)};

  if (song_p) {
    qDebug() << "Playing " << song_p->title().c_str();
    this->playingSongId_ = song_p->id();

    auto playlist = new QMediaPlaylist{this};
    QList<QMediaContent> contents;
    this->playlistSongs_.clear();
    for (auto song : model->getSongs()) {
      QUrl url;
      url.setHost(SERVER_NAME);
      url.setPath(QString{"/song/%1"}.arg(QString::number(song.id())));
      url.setPort(SERVER_PORT);
      url.setScheme("http");

      qDebug() << url;
      QMediaContent content{url};
      contents.push_back(content);

      this->playlistSongs_.push_back(song);
    }
    playlist->addMedia(contents);
    playlist->setCurrentIndex(index.row());

    this->player_.setPlaylist(playlist);
    this->player_.play();
  }
}

void MainWindow::stateChanged(QMediaPlayer::State state) {
  auto setEnabledAll = [&](bool enabled) {
    this->ui->nextButton->setEnabled(enabled);
    this->ui->previousButton->setEnabled(enabled);
    this->ui->playPauseButton->setEnabled(enabled);
  };

  switch (state) {
  case QMediaPlayer::State::PausedState:
    setEnabledAll(true);
    this->ui->playPauseButton->setText("Play");
    break;
  case QMediaPlayer::State::PlayingState:
    setEnabledAll(true);
    this->ui->playPauseButton->setText("Pause");
    break;
  case QMediaPlayer::State::StoppedState:
    setEnabledAll(false);
    this->ui->playPauseButton->setText("Play");
    break;
  }
}
