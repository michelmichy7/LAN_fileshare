#include "Backend/Backend.h"
#include <QFileDialog>

FileDialogHelper::FileDialogHelper(QObject *parent)
    : QObject(parent)
{
}

QStringList FileDialogHelper::openFileDialog()
{
    QStringList files = QFileDialog::getOpenFileNames(
        nullptr,
        "Select Files",
        QString(),
        "All Files (*.*);;Video (*.mp4)",
        nullptr
        );

    if (m_selectedFiles != files) {
        m_selectedFiles = files;
        emit selectedFilesChanged();  // Emit signal to update QML
    }

    return files;
}
