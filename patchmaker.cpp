#include "patchmaker.h"
#include "rhxmlwriter.h"
#include "ui_patchmaker.h"
#include <QSettings>
#include <QXmlStreamReader>

PatchMaker::PatchMaker(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::PatchMaker) {
  ui->setupUi(this);
  connect(this, SIGNAL(logSignal(QString)), SLOT(logText(QString)));
}

PatchMaker::~PatchMaker() { delete ui; }
void PatchMaker::getFileCount(const QString &path, uint &count) {
  QDir dir(path);
  dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::NoSymLinks);
  QFileInfoList files = dir.entryInfoList();

  foreach (const QFileInfo &fi, files) {
    QString Path = fi.absoluteFilePath();
    if (fi.isDir()) {
      getFileCount(Path, count);
    } else {
      count++;
    }
  }
}
void PatchMaker::getFilesRecursive(const QString &path,
                                   QVector<QFileInfo> &fileVec) {
  QDir inDir(path);
  inDir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::NoSymLinks);
  QFileInfoList files = inDir.entryInfoList();

  foreach (const QFileInfo &fi, files) {
    QString Path = fi.absoluteFilePath();
    if (fi.isDir()) {
      getFilesRecursive(Path, fileVec);
    } else {
      fileVec.push_back(fi);
    }
  }
}

void PatchMaker::on_selectInputBtn_clicked() {
  const QString DEFAULT_DIR_KEY("default_input_dir");
  QSettings pathsSettings;

  QString path = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"),
      pathsSettings.value(DEFAULT_DIR_KEY).toString(),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if (!path.isEmpty()) {
    QDir currentDir;
    pathsSettings.setValue(DEFAULT_DIR_KEY, currentDir.absoluteFilePath(path));

    ui->inputFolderTxt->document()->setPlainText(path);

    uint count = 0;
    getFileCount(path, count);
    ui->fileCountLbl->setText("File count: " + QString::number(count));
  }
}

void PatchMaker::on_selectOutputBtn_clicked() {
  const QString DEFAULT_DIR_KEY("default_output_dir");
  QSettings pathsSettings;

  QString path = QFileDialog::getExistingDirectory(
      this, tr("Open Directory"),
      pathsSettings.value(DEFAULT_DIR_KEY).toString(),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!path.isEmpty()) {
    QDir currentDir;
    pathsSettings.setValue(DEFAULT_DIR_KEY, currentDir.absoluteFilePath(path));

    ui->outputFolderTxt->document()->setPlainText(path);
  }
}

void PatchMaker::logText(const QString &str) {
  ui->plainTextEdit->moveCursor(QTextCursor::End);
  ui->plainTextEdit->insertPlainText(str);
  ui->plainTextEdit->moveCursor(QTextCursor::End);
}

int PatchMaker::getCurrentVersion(QXmlStreamReader &reader) {
  if (reader.readNextStartElement() && reader.name() == "patch") {
    return reader.attributes().value("version").toString().toInt();
  }
  return 0;
}

QVector<std::shared_ptr<PatchElement>>
getCurrentPatchElements(QXmlStreamReader &reader) {
  QVector<std::shared_ptr<PatchElement>> currPatchElements;
  while (reader.readNextStartElement() && reader.name() == "file") {
    std::shared_ptr<PatchElement> downFile = std::make_shared<PatchElement>();

    downFile->clientPath = reader.attributes().value("clientPath").toString();
    downFile->checkSum = reader.attributes().value("checkSum").toString();
    downFile->remoteFullPath =
        reader.attributes().value("remoteFullPath").toString();
    downFile->fileName = reader.attributes().value("name").toString();
    currPatchElements << downFile;
    reader.readNext();
  }
  return currPatchElements;
}

std::shared_ptr<PatchElement>
PatchMaker::createPatchElement(const QFileInfo &fileInfo,
                               const QString &inputFolderPath) {
  std::shared_ptr<PatchElement> patchElement = std::make_shared<PatchElement>();
  patchElement->fileName = fileInfo.fileName();
  patchElement->clientPath =
      fileInfo.absolutePath().replace(inputFolderPath, "") + "/";

  QString temp = patchElement->fileName;
  patchElement->remoteFullPath = "/patch" + patchElement->clientPath +
                                 temp.replace(".", "__").append(".qz");
  return patchElement;
}

void PatchMaker::on_pushButton_3_clicked() {
  QString outPath = ui->outputFolderTxt->toPlainText();
  QString inPath = ui->inputFolderTxt->toPlainText();
  if (outPath.isEmpty() || inPath.isEmpty()) {
    QMessageBox mssg;
    mssg.setText("Empty in/out paths!");
    mssg.exec();
    return;
  }
  QDir inDir(inPath);
  if (!inDir.exists()) {
    QMessageBox mssg;
    mssg.setText("Input folder doesn't exist\n");
    mssg.exec();
    return;
  }
  QDir outDir(outPath);

  QFuture<int> future =
      QtConcurrent::run(this, &PatchMaker::buildPatch, inDir, outDir);
  // int result = BuildPatch(inDir,outDir);
}
QString PatchMaker::generateStringHash(const QByteArray &inputBytes) {
  QCryptographicHash hash(QCryptographicHash::Md5);
  hash.addData(inputBytes);
  return hash.result().toHex();
}
void PatchMaker::updateElementIfExists(
    const std::shared_ptr<PatchElement> &newPatchElement) {
  QVector<std::shared_ptr<PatchElement>>::iterator it = std::find_if(
      patchElements.begin(), patchElements.end(),
      [&](const std::shared_ptr<PatchElement> &toCompare) {
        return toCompare->clientPath == newPatchElement->clientPath &&
               toCompare->fileName == newPatchElement->fileName;
      });
  if (it != patchElements.end()) {
    std::shared_ptr<PatchElement> curr = *it;
    curr->remoteFullPath = newPatchElement->remoteFullPath;
    curr->checkSum = newPatchElement->checkSum;
  } else {
    patchElements << newPatchElement;
  }
}
void PatchMaker::writeFileList(QFile &fileListInfo, int version) {
  fileListInfo.resize(0);
  RhXMLWriter xmlWriter(&fileListInfo);
  xmlWriter.writeHeader(version);
  foreach (const std::shared_ptr<PatchElement> &toSaveElem, patchElements) {
    xmlWriter.writePatchElement(toSaveElem);
  }

  xmlWriter.close();
}
int PatchMaker::buildPatch(QDir inDir, QDir outDir) {
  QVector<QFileInfo> fileVec;
  QString inputFolderPath = inDir.absolutePath();
  getFilesRecursive(inDir.absolutePath(), fileVec);
  if (!fileVec.size()) {
    emit logSignal("\nEmpty input.");
    return 1;
  }

  QFile fileListInfo(QCoreApplication::applicationDirPath() + "/fileList.xml");
  fileListInfo.open(QIODevice::ReadWrite);
  QXmlStreamReader fileInfoListXmlReader(&fileListInfo);
  int version = getCurrentVersion(fileInfoListXmlReader);
  patchElements = getCurrentPatchElements(fileInfoListXmlReader);

  version++;

  // write patch files
  foreach (const QFileInfo &fi, fileVec) {
    std::shared_ptr<PatchElement> newPatchElement =
        createPatchElement(fi, inputFolderPath);
    QFile inputPatchFile(fi.absoluteFilePath());
    if (!inputPatchFile.open(QIODevice::ReadOnly)) {
      qDebug() << "Error file couldnt be opened " << fi.absoluteFilePath();
      continue;
    }

    QString toSaveFileName(fi.fileName());
    toSaveFileName.replace(".", "__").append(".qz");

    QString newPatchFilePath =
        outDir.absolutePath() + newPatchElement->clientPath;
    QDir().mkpath(newPatchFilePath);
    QFile outPatchFile(newPatchFilePath + toSaveFileName);
    if (!outPatchFile.open(QIODevice::WriteOnly)) {
      qDebug() << "Error file couldnt be opened " << fi.absoluteFilePath()
               << "\n";
      continue;
    }

    QByteArray fileBuff = inputPatchFile.readAll();
    newPatchElement->checkSum = generateStringHash(fileBuff);
    outPatchFile.write(qCompress(fileBuff));
    emit logSignal("Writing file " + outDir.absolutePath() + "/" +
                   toSaveFileName + "\n");
    updateElementIfExists(newPatchElement);
  }
  writeFileList(fileListInfo, version);
  fileListInfo.close();
  emit logSignal("\nWork done.");
}
