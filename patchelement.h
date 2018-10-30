#ifndef PATCHELEMENT_H
#define PATCHELEMENT_H
#include <QString>
struct PatchElement {
  QString fileName;
  QString clientPath;
  QString remoteFullPath;
  QString checkSum;
};

#endif // PATCHELEMENT_H
