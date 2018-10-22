#include "MainWindow.hpp"
#include <QApplication>

#include "rest/ArtistsApi.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

//  return 1;
  return a.exec();
}
