#include "rhxmlwriter.h"
#include "patchelement.h"
RhXMLWriter::RhXMLWriter(QIODevice *device) : writer(device) {
  writer.setAutoFormatting(true);
  writer.writeStartDocument();
}

RhXMLWriter::~RhXMLWriter() {}
void RhXMLWriter::close() {
  writer.writeEndElement();
  writer.writeEndDocument();
}
void RhXMLWriter::writeHeader(int version) {
  writer.writeStartElement("patch");
  writer.writeAttribute("version", QString::number(version));
}
void RhXMLWriter::writePatchElement(QString &fileName, QString &clientPath,
                                    QString &remoteFullPath,
                                    QString &checkSum) {
  writer.writeStartElement("file");
  writer.writeAttribute("name", fileName);
  writer.writeAttribute("clientPath", clientPath);
  writer.writeAttribute("remoteFullPath", remoteFullPath);
  writer.writeAttribute("checkSum", checkSum);
  writer.writeCharacters("");
  writer.writeEndElement();
}
void RhXMLWriter::writePatchElement(
    const std::shared_ptr<PatchElement> &toSaveElem) {
  writer.writeStartElement("file");
  writer.writeAttribute("name", toSaveElem->fileName);
  writer.writeAttribute("clientPath", toSaveElem->clientPath);
  writer.writeAttribute("remoteFullPath", toSaveElem->remoteFullPath);
  writer.writeAttribute("checkSum", toSaveElem->checkSum);
  writer.writeCharacters("");
  writer.writeEndElement();
}
