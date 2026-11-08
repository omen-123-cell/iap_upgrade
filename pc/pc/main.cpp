#include "mainwindow.h"

#include <QApplication>
#include <QFileInfo>
#include <QTimer>

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QCoreApplication::setOrganizationName(QStringLiteral("IAP"));
    QCoreApplication::setApplicationName(QStringLiteral("IapTool"));
    QCoreApplication::setApplicationVersion(QStringLiteral("1.0"));

    QApplication a(argc, argv);
    MainWindow w;

    if (argc > 1) {
        const QString path = QFileInfo(QString::fromLocal8Bit(argv[1])).absoluteFilePath();
        if (QFileInfo::exists(path))
            w.loadFirmwareFile(path);
    }

    w.show();

    // 临时 UI 冒烟测试入口
    const QStringList args = QCoreApplication::arguments();
    const int testIndex = args.indexOf(QStringLiteral("--ui-test"));
    if (testIndex > 0 && testIndex + 1 < args.size()) {
        const QString output = args.at(testIndex + 1);
        w.showMaximized();
        QTimer::singleShot(800, [&w, output]() {
            w.grab().save(output);
            qApp->quit();
        });
    }

    return a.exec();
}
