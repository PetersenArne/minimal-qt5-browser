#include <QCoreApplication>
#include <QApplication>
#include <QTextStream>
#include <QWebEngineView>

class MyWebEngineView : public QWebEngineView
{
    Q_OBJECT

public:
    MyWebEngineView(const QString& url, int width, int height)
        : url_(url), width_(width), height_(height)
    {
        connect(this, &QWebEngineView::loadFinished, this, &MyWebEngineView::onLoadFinished);
    }

    void startLoad()
    {
        setUrl(QUrl(url_));
        resize(width_, height_);
        show();
    }

private slots:
    void onLoadFinished(bool success)
    {
        if (success) {
            // Page loaded successfully
            QTextStream(stdout) << "Page loaded successfully" << endl;
        } else {
            // Error loading the page
            QTextStream(stdout) << "Error loading the page" << endl;
        }
    }

private:
    QString url_;
    int width_;
    int height_;
};

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    if (app.arguments().size() < 4) {
        QTextStream(stderr) << "Usage: program <port> <width> <height>" << endl;
        return 1;
    }

    QString port = app.arguments().at(1);
    int width = app.arguments().at(2).toInt();
    int height = app.arguments().at(3).toInt();

    /* disable "right click" menu */
    MyWebEngineView view("http://localhost:" + port, width, height);
    view.setContextMenuPolicy(Qt::NoContextMenu);

    QTextStream(stdout) << "Set url to: http://localhost:" + port << endl;

    view.startLoad();

    return app.exec();
}

