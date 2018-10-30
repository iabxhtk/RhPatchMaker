#ifndef RHXMLWRITER_H
#define RHXMLWRITER_H
#include "patchelement.h"
#include <QXmlStreamWriter>
#include <memory>
class RhXMLWriter {
private:
  QXmlStreamWriter writer;

public:
  RhXMLWriter(QIODevice *device);
  ~RhXMLWriter();
  void writePatchElement(QString &fileName, QString &clientPath,
                         QString &remoteFullPath, QString &checkSum);
  void writeHeader(int version);
  void close();
  void writePatchElement(const std::shared_ptr<PatchElement> &toSaveElem);
};

#endif // RHXMLWRITER_H
