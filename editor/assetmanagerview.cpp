#include "assetmanagerview.h"

#include <QAction>
#include <QClipboard>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QPushButton>
#include <QSplitter>
#include <QTreeView>

#include "editor/gui/flowlayout.h"
#include "editor/gui/layoututils.h"

#include "extras/assetmanager.h"


////////////////////// Helpers //////////////////////

QImage unsupportedPreviewImg()
{
    QPixmap pixmap(48, 48);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), Qt::red);
    painter.drawText(pixmap.rect(), "Unsupported\n preview");
    painter.end();

    QImage ret = pixmap.toImage();

    return ret;
}

////////////////////// PreviewWidget //////////////////////

PreviewWidget::PreviewWidget(const QString &name, QWidget *parent) :
    QWidget(parent),
    m_pixmapLabel(nullptr),
    m_pathLabel(nullptr),
    m_assetPath()
{
    init(name);
}

PreviewWidget::PreviewWidget(const QString &name,
                             const QImage &img,
                             QWidget *parent) :
    QWidget(parent),
    m_pixmapLabel(nullptr),
    m_pathLabel(nullptr),
    m_assetPath()
{
    init(name);

    setImage(img);
}

void PreviewWidget::setImage(const QImage &img)
{
    static const int previewSize = 48;

    m_pixmapLabel->setFixedHeight(previewSize);

    QPixmap previewPixmap = QPixmap::fromImage(img.scaled(previewSize,
                                                          previewSize,
                                                          Qt::KeepAspectRatioByExpanding));

    m_pixmapLabel->setPixmap(previewPixmap);
}

void PreviewWidget::setAssetPath(const QString &path)
{
    if (m_assetPath != path) {
        m_assetPath = path;
    }
}

void PreviewWidget::mousePressEvent(QMouseEvent *)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(m_assetPath);
}

void PreviewWidget::init(const QString &name)
{
    m_pixmapLabel = new QLabel(this);
    m_pixmapLabel->setAlignment(Qt::AlignCenter);

    m_pathLabel = new QLabel(name, this);
    m_pathLabel->setAlignment(Qt::AlignCenter);

    // Assemble UI
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget(m_pixmapLabel);
    mainLayout->addWidget(m_pathLabel);
}


////////////////////// AssetManagerView //////////////////////

AssetManagerView::AssetManagerView(QWidget *parent) :
    QMainWindow(parent),
    m_currentFolderPath(),
    m_currentFolderLabel(nullptr),
    m_fileSystemView(nullptr),
    m_folderView(nullptr),
    m_folderViewLayout(nullptr),
    m_filePaths()
{
    // Create menu
    auto *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // Create UI
    auto *mainWidget = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(mainWidget);

    //  File system view
    auto *fileSystemViewWidget = new QWidget(mainWidget);

    m_currentFolderLabel = new QLabel(fileSystemViewWidget);
    m_currentFolderLabel->setWordWrap(true);

    m_fileSystemView = new QTreeView(fileSystemViewWidget);

    m_fileSystemModel = new QFileSystemModel(m_fileSystemView);
    m_fileSystemModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    auto *fileSystemViewLayout = new QVBoxLayout(fileSystemViewWidget);
    fileSystemViewLayout->addWidget(m_currentFolderLabel);
    fileSystemViewLayout->addWidget(m_fileSystemView);

    //  Pak view
    m_folderView = new QWidget(mainWidget);
    m_folderViewLayout = new FlowLayout(m_folderView);

    //  Assemble
    mainWidget->addWidget(fileSystemViewWidget);
    mainWidget->addWidget(m_folderView);
    mainWidget->setStretchFactor(0, 1);
    mainWidget->setStretchFactor(1, 20);
}

void AssetManagerView::setProjectPath(const QString &dirPath)
{
    const QFileInfo dirInfos(dirPath);

    if (!dirInfos.exists() || !dirInfos.isDir()) {
        return;
    }

    m_currentFolderPath = dirPath;

    m_currentFolderLabel->setText(dirPath);

    if (!m_fileSystemView->model()) {
        m_fileSystemView->setModel(m_fileSystemModel);

        for (int col = 1; col < m_fileSystemModel->columnCount(); col++) {
            m_fileSystemView->hideColumn(col);
        }

        auto *selectionModel = new QItemSelectionModel(m_fileSystemModel);

        m_fileSystemView->setSelectionModel(selectionModel);

        connect(selectionModel, &QItemSelectionModel::currentChanged,
                [this] (const QModelIndex &current, const QModelIndex &) {
            setViewPath(m_fileSystemModel->filePath(current));
        });
    }

    m_fileSystemView->setRootIndex(m_fileSystemModel->setRootPath(dirPath));

    setViewPath(QString());
}

void AssetManagerView::setViewPath(const QString &dirPath)
{
    clearLayout(m_folderViewLayout);

    if (dirPath.isNull()) {
        return;
    }

    QDirIterator dirIt(dirPath);

    while (dirIt.hasNext()) {
        const QString filePath = dirIt.next();
        const QFileInfo fileInfo(filePath);

        if (fileInfo.isFile()) {
            QString labelText = filePath;
            labelText.remove(0, filePath.lastIndexOf('/') + 1);

            auto *previewWidget = new PreviewWidget(labelText, m_folderView);

            QString assetPath = dirPath.mid(dirPath.lastIndexOf('/') + 1);
            assetPath.append("/" + labelText);

            previewWidget->setAssetPath(assetPath);

            previewWidget->setImage(unsupportedPreviewImg());

            for (const QString &imgFormat : AssetManager::supportedImageFormats()) {
                if (labelText.endsWith(imgFormat)) {
                    previewWidget->setImage(QImage(filePath));
                }
            }

            m_folderViewLayout->addWidget(previewWidget);
        }
    }
}

void AssetManagerView::packBigFile()
{
    if (!m_currentFolderPath.isNull()) {
        createBigFile(m_currentFolderPath.toStdString(), "data.pak");
    }
}
