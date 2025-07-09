#include "Backend/Backend.h"
#include <QFileDialog>

FileDialogHelper::FileDialogHelper(QObject *parent)
    : QObject(parent)
{
}

QStringList FileDialogHelper::openFileDialog()
{
    return QFileDialog::getOpenFileNames(
        nullptr,
        "Select Files",
        QString(),
        "All Files (*.*)"
        );
}
