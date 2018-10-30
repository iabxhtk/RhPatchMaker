#ifndef PATCHMAKER_H
#define PATCHMAKER_H

#include "patchelement.h"
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QSet>
#include <QtConcurrent/QtConcurrent>
namespace Ui {
class PatchMaker;
}

class PatchMaker : public QMainWindow {
  Q_OBJECT
public:
  explicit PatchMaker(QWidget *parent = 0);
  ~PatchMaker();

  QString generateStringHash(const QByteArray &inputBytes);
  void
  updateElementIfExists(const std::shared_ptr<PatchElement> &newPatchElement);
  std::shared_ptr<PatchElement>
  createPatchElement(QFileInfo &fileInfo, const QString &inputFolderPath);

  void writeFileList(QFile &fileListInfo, int version);
  std::shared_ptr<PatchElement>
  createPatchElement(const QFileInfo &fileInfo, const QString &inputFolderPath);
  int getCurrentVersion(QXmlStreamReader &reader);
private slots:
  void on_selectInputBtn_clicked();

  void on_selectOutputBtn_clicked();

  void on_pushButton_3_clicked();

  int buildPatch(QDir inDir, QDir outDir);

  void logText(const QString &str);

private:
  Ui::PatchMaker *ui;

  void getFileCount(const QString &path, uint &count);

  void getFilesRecursive(const QString &path, QVector<QFileInfo> &fileVec);

  int getCurrentVersion(QFile &file);

  QVector<std::shared_ptr<PatchElement>> patchElements;
signals:
  void logSignal(const QString &str);
};

#endif // PATCHMAKER_H
