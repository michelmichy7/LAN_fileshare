#include "Backend/Backend.h"
#include <QFileDialog>

FileDialogHelper::FileDialogHelper(QObject *parent)
    : QObject(parent)
{
}

QStringList FileDialogHelper::openFileDialog()
{
    return QFileDialog::getOpenFileNames(
        nullptr,                                    // parent widget
        "Select Files",                            // dialog caption
        QString(),                                 // starting directory (empty = current)
        "All Files (*.*);;Video (*.mp4)",         // file filters
        nullptr                                    // selected filter (optional)
        );
}
