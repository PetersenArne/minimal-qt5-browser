#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProcess>
#include <QDebug>
#include <vector>

class CircleWidget : public QWidget {
    std::vector<QLine> lines;  // Store the lines drawn by the user
    QPoint lastPoint;          // Track the last point to draw continuous lines
    bool drawing;              // Track whether the user is currently drawing

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);

        // Set the background color to light blue
        painter.fillRect(rect(), QColor(193, 226, 250)); // Light blue color

        // Draw all lines that the user has drawn
        painter.setPen(QPen(Qt::black, 5)); // Black color, 3 pixels wide
        for (const QLine &line : lines) {
            painter.drawLine(line);
        }

        // Set the pen for drawing the circle
        QPen pen(Qt::red, 15); // Black color, 3 pixels wide
        painter.setPen(pen);

        // Calculate the available height considering the padding
        int padding = 10;
        int availableHeight = height() - 2 * padding;

        // Use the smaller of the available height and width to ensure a perfect circle
        int size = qMin(width(), availableHeight);

        // Calculate the center of the widget
        int centerX = width() / 2;
        int centerY = padding + (availableHeight / 2);

        // Draw the circle (equal width and height, with padding on top and bottom)
        painter.drawEllipse(centerX - size / 2, centerY - size / 2, size, size);

        // Set the pen for drawing the grid
        QPen gridPen(QColor(150, 150, 150), 1); // Light grey color, 1 pixel wide
        painter.setPen(gridPen);

        // Draw vertical grid lines
        int gridSpacing = 20;
        for (int x = gridSpacing; x < width(); x += gridSpacing) {
            painter.drawLine(x, 0, x, height());
        }

        // Draw horizontal grid lines
        for (int y = gridSpacing; y < height(); y += gridSpacing) {
            painter.drawLine(0, y, width(), y);
        }
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            lastPoint = event->pos();
            drawing = true;
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (drawing && (event->buttons() & Qt::LeftButton)) {
            QPoint currentPoint = event->pos();
            lines.push_back(QLine(lastPoint, currentPoint));
            lastPoint = currentPoint;
            update(); // Request a repaint to show the new line segment
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton && drawing) {
            QPoint currentPoint = event->pos();
            lines.push_back(QLine(lastPoint, currentPoint));
            drawing = false;
            update(); // Request a repaint to show the final line segment
        }
    }

public slots:
    void clearLines() {
        lines.clear();  // Clear all stored lines
        update();       // Request a repaint to clear the canvas
    }

public:
    CircleWidget(QWidget *parent = nullptr) : QWidget(parent), drawing(false) {
        setMouseTracking(true); // Enable mouse tracking even when no buttons are pressed
    }
};

class MainWindow : public QWidget {
    CircleWidget *circleWidget;
    QPushButton *clearButton;
    QPushButton *commandButton;
    QPushButton *rebootButton;

public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // Set the main window layout and central widget
        circleWidget = new CircleWidget(this);
        clearButton = new QPushButton("Clear Screen", this);
        commandButton = new QPushButton("Calibrate TS", this);
        rebootButton = new QPushButton("Reboot", this);

        // Set the size and positioning of the circleWidget
        circleWidget->setGeometry(0, 0, 800, 480);

        // Increase the font size of the button text
        QFont font = clearButton->font();
        font.setPointSize(18);  // Set the desired font size
        clearButton->setFont(font);
        commandButton->setFont(font);
        rebootButton->setFont(font);

        // Position the clearButton on top of the circleWidget
        clearButton->setGeometry(10, 440, 180, 30); // Adjust position and size as needed
        commandButton->setGeometry(610, 440, 180, 30);
        rebootButton->setGeometry(610, 10, 180, 30);

        connect(clearButton, &QPushButton::clicked, circleWidget, &CircleWidget::clearLines);
        connect(commandButton, &QPushButton::clicked, this, &MainWindow::runShellCommand(0));
        connect(rebootButton, &QPushButton::clicked, this, &MainWindow::runShellCommand(1));
    }

public slots:
    void runShellCommand(int n) {
        QString command = ""; 

        if (n == 0) {
            command = "/usr/bin/ts_calibrate";
        } else if (n == 1) {
            command = "/sbin/reboot";
        } 

        // Start the process
        QProcess process;
        process.start(command);
        process.waitForFinished();

        // Get the output
        QString output = process.readAllStandardOutput();
        QString error = process.readAllStandardError();

        // Output the result to the console (for debugging purposes)
        qDebug() << "Output:" << output;
        if (error != "") qDebug() << "Error:" << error;
    }
};

int main(int argc, char *argv[]) {
    QString command = "/usr/bin/ts_calib.sh";

    // Start the process
    QProcess process;
    process.start(command);
    process.waitForFinished();


    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("Draw on Canvas with Circle and Clear Button");
    window.resize(800, 480);
    window.show();

    return app.exec();
}
