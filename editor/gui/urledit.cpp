#include "urledit.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>


////////////////////// UrlEditPrivate //////////////////////

class UrlEditPrivate
{
public:
    UrlEditPrivate(UrlEdit *qq);

    QUrl resolvedLocalUrl(const QUrl &fileUrl);
    bool urlsIsValid(const QUrl &fileUrl);

    UrlEdit *q;

    QLineEdit *lineEdit;
    QToolButton *browseBtn;
    QFileDialog *fileDialog;

    QUrl url;
    QString title;
    QUrl folder;
    QStringList nameFilters;
    bool selectFolder;
};

UrlEditPrivate::UrlEditPrivate(UrlEdit *qq) :
    q(qq),
    lineEdit(new QLineEdit(qq)),
    browseBtn(new QToolButton(qq)),
    fileDialog(new QFileDialog(qq)),
    url(),
    title(),
    folder(),
    nameFilters(),
    selectFolder(false)
{}

QUrl UrlEditPrivate::resolvedLocalUrl(const QUrl &fileUrl)
{
    QString urlString = fileUrl.toString();


#if defined (Q_OS_WIN)
    urlString.remove("file:///");
#elif defined (Q_OS_LINUX) || defined(Q_OS_UNIX) || defined (Q_OS_OSX)
    urlString.remove("file://");
#endif

    return urlString;
}

bool UrlEditPrivate::urlsIsValid(const QUrl &fileUrl)
{
    return QFileInfo::exists(fileUrl.toString());
}


////////////////////// UrlEdit //////////////////////

UrlEdit::UrlEdit(QWidget *parent) :
    QWidget(parent),
    d(new UrlEditPrivate(this))
{
    d->lineEdit->setReadOnly(true);

    d->browseBtn->setText("...");

    d->fileDialog->setModal(true);
    d->fileDialog->setVisible(false);

    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(d->lineEdit);
    setTabOrder(d->lineEdit, d->browseBtn);

    auto layout = new QHBoxLayout(this);
    layout->setMargin(0);

    layout->addWidget(d->lineEdit);
    layout->addWidget(d->browseBtn);

    connect(d->browseBtn, &QToolButton::pressed,
            this, &UrlEdit::openFileDialog);

    connect(d->fileDialog, &QFileDialog::accepted,
            this, [=] {
        const QList<QUrl> urls = d->fileDialog->selectedUrls();
        Q_ASSERT(urls.size() > 0);

        QUrl selectedFile = urls.at(0).toString();

        setUrl(selectedFile);
    });
}

UrlEdit::~UrlEdit()
{}

QUrl UrlEdit::url() const
{
    return d->url;
}

void UrlEdit::setUrl(const QUrl &url)
{
    if (d->url != url) {
        d->url = url;

        d->lineEdit->setText(d->resolvedLocalUrl(url).toString());

        Q_EMIT urlChanged(url);
    }
}

QString UrlEdit::title() const
{
    return d->title;
}

void UrlEdit::setTitle(const QString &title)
{
    if (d->title != title) {
        d->title = title;

        Q_EMIT titleChanged(title);
    }
}

QUrl UrlEdit::folder() const
{
    return d->folder;
}

void UrlEdit::setFolder(const QUrl &folder)
{
    if (d->folder != folder) {
        d->folder = folder;

        Q_EMIT folderChanged(folder);
    }
}

QStringList UrlEdit::nameFilters() const
{
    return d->nameFilters;
}

void UrlEdit::setNameFilters(const QStringList &nameFilters)
{
    if (d->nameFilters != nameFilters) {
        d->nameFilters = nameFilters;

        Q_EMIT nameFiltersChanged(nameFilters);
    }
}

bool UrlEdit::selectFolder() const
{
    return d->selectFolder;
}

void UrlEdit::setSelectFolder(bool selectFolder)
{
    if (d->selectFolder != selectFolder) {
        d->selectFolder = selectFolder;

        Q_EMIT selectFolderChanged(selectFolder);
    }
}

void UrlEdit::openFileDialog()
{
    d->fileDialog->setWindowTitle(d->title);
    d->fileDialog->setDirectoryUrl(d->folder);
    d->fileDialog->setNameFilters(d->nameFilters);

    QFileDialog::FileMode mode = QFileDialog::AnyFile;
    QFileDialog::Options options = 0;

    if (d->selectFolder) {
        mode = QFileDialog::Directory;
        options = QFileDialog::ShowDirsOnly;
    }

    d->fileDialog->setFileMode(mode);
    d->fileDialog->setOptions(options);

    d->fileDialog->show();
}
