#include <QApplication>
#include <QWidget>
#include <QPainter>

class CircleWidget : public QWidget {
protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);

        // Set the background color to light blue
        painter.fillRect(rect(), QColor(173, 216, 230)); // Light blue color

        // Set the pen for drawing the circle
        QPen pen(Qt::black, 3); // Black color, 3 pixels wide
        painter.setPen(pen);

        // Calculate the center and radius of the circle
        int centerX = width() / 2;
        int centerY = height() / 2;
        int radius = qMin(width(), height()) / 4;

        // Draw the circle
        painter.drawEllipse(centerX - radius, centerY - radius, radius * 2, radius * 2);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create and set up the main window
    CircleWidget window;
    window.setWindowTitle("Circle on Light Blue Background");
    window.resize(1280, 720);
    window.show();

    return app.exec();
}
