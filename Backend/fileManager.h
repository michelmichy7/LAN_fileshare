#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QDateTime>

#include <QDataStream>
#include <QIODevice>

struct FileHeader {
    QString fileName;
    qint64 fileSize;

    //more metadata:
    QString mimeType;
    QDateTime created;
};

inline QDataStream &operator<<(QDataStream &out, const FileHeader &header)
{
    return out << header.fileName
               << header.fileSize
               << header.mimeType
               << header.created;
}
inline QDataStream &operator>>(QDataStream &in, FileHeader &header)
{
    return in >> header.fileName
           >> header.fileSize
           >> header.mimeType
           >> header.created;
}
#endif // FILEMANAGER_H
