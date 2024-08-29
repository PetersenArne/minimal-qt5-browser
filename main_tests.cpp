#include <QApplication>
#include <QWebEngineView>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create a QMainWindow to contain the browser
    QMainWindow window;

    // Create a QWebEngineView widget (the browser)
    QWebEngineView *view = new QWebEngineView(&window);

    // Load a web page (default page or localhost)
    view->setUrl(QUrl("http://www.example.com"));

    // Set the QWebEngineView as the central widget of the window
    window.setCentralWidget(view);

    // Set window size
    window.resize(1024, 768);

    // Show the window
    window.show();

    return app.exec();
}
