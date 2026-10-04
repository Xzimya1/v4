#include "mainwindow.h"
#include "sleepengine.h"

#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimeEdit>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QGroupBox>
#include <QTextStream>

namespace SleepGuard {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    buildUi();

    connect(&m_devices, &DeviceController::epochReceived,
            this, &MainWindow::onEpoch);
    connect(&m_devices, &DeviceController::heartRateChanged,
            this, [this](double bpm) {
                m_heartRate->setText(QString("Пульс: %1 BPM").arg(bpm, 0, 'f', 0));
            });
    connect(&m_devices, &DeviceController::deviceStatusChanged,
            this, [this](const QString& s) { m_status->setText(s); });
    connect(&m_devices, &DeviceController::lampStateChanged,
            this, [this](bool on) {
                m_alarmButton->setText(on ? "Будильник: ВКЛ" : "Будильник: ВЫКЛ");
            });
}

void MainWindow::buildUi() {
    auto* central = new QWidget;
    auto* main = new QVBoxLayout(central);

    auto* title = new QLabel("<h1>SleepGuard / Somni — MVP</h1>");
    auto* disclaimer = new QLabel(
        "Демонстрационный baseline. Оценка стадий сна не является медицинской диагностикой.");
    disclaimer->setWordWrap(true);

    m_status = new QLabel("Браслет не подключён");
    m_heartRate = new QLabel("Пульс: — BPM");
    m_quality = new QLabel("Качество сна: —");
    m_stages = new QLabel("Стадии: —");
    m_qualityBar = new QProgressBar;
    m_qualityBar->setRange(0, 100);

    auto* devices = new QGroupBox("Устройства");
    auto* dlay = new QHBoxLayout(devices);
    auto* start = new QPushButton("Подключить DEMO-браслет");
    auto* stop = new QPushButton("Отключить");
    m_alarmButton = new QPushButton("Будильник: ВЫКЛ");
    dlay->addWidget(start);
    dlay->addWidget(stop);
    dlay->addWidget(m_alarmButton);

    connect(start, &QPushButton::clicked, this, &MainWindow::startDemo);
    connect(stop, &QPushButton::clicked, this, &MainWindow::stopDemo);
    connect(m_alarmButton, &QPushButton::clicked, this, [this]() {
        m_devices.setLampAlarm(!m_devices.lampAlarmEnabled());
    });

    auto* wake = new QGroupBox("Вилка пробуждения");
    auto* wlay = new QHBoxLayout(wake);
    m_windowStart = new QTimeEdit(QTime(6, 30));
    m_windowEnd = new QTimeEdit(QTime(7, 0));
    auto* save = new QPushButton("Сохранить");
    wlay->addWidget(new QLabel("С:"));
    wlay->addWidget(m_windowStart);
    wlay->addWidget(new QLabel("До:"));
    wlay->addWidget(m_windowEnd);
    wlay->addWidget(save);
    connect(save, &QPushButton::clicked, this, &MainWindow::saveWakeWindow);

    auto* metrics = new QGroupBox("Последний сон");
    auto* mlay = new QVBoxLayout(metrics);
    mlay->addWidget(m_heartRate);
    mlay->addWidget(m_quality);
    mlay->addWidget(m_qualityBar);
    mlay->addWidget(m_stages);

    m_table = new QTableWidget(0, 6);
    m_table->setHorizontalHeaderLabels(
        {"Время", "Пульс", "RMSSD", "Движение", "Дыхание", "Стадия"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto* exportBtn = new QPushButton("Экспорт CSV");

    main->addWidget(title);
    main->addWidget(disclaimer);
    main->addWidget(m_status);
    main->addWidget(devices);
    main->addWidget(wake);
    main->addWidget(metrics);
    main->addWidget(new QLabel("<b>Эпохи (30 секунд)</b>"));
    main->addWidget(m_table);
    main->addWidget(exportBtn);

    connect(exportBtn, &QPushButton::clicked, this, &MainWindow::exportCsv);

    setCentralWidget(central);
    setWindowTitle("SleepGuard MVP");
}

void MainWindow::startDemo() {
    m_epochs.clear();
    m_table->setRowCount(0);
    m_devices.startDemoStream();
}

void MainWindow::stopDemo() {
    m_devices.stopDemoStream();
    refreshReport();
}

void MainWindow::onEpoch(const Epoch& epoch) {
    m_epochs.append(epoch);
    if (m_epochs.size() > 720) // до 6 часов демо-истории
        m_epochs.removeFirst();

    updateTable();
    refreshReport();
}

void MainWindow::refreshReport() {
    const auto report = SleepEngine::makeReport(m_epochs);
    m_quality->setText(QString("Качество сна: %1%").arg(report.quality));
    m_qualityBar->setValue(report.quality);

    m_stages->setText(QString("Бодрствование: %1 мин | Лёгкий: %2 мин | "
                              "Глубокий: %3 мин | REM: %4 мин")
                      .arg(report.awakeMinutes)
                      .arg(report.lightMinutes)
                      .arg(report.deepMinutes)
                      .arg(report.remMinutes));
}

void MainWindow::updateTable() {
    const int maxRows = 100;
    const int begin = qMax(0, m_epochs.size() - maxRows);
    m_table->setRowCount(m_epochs.size() - begin);

    int row = 0;
    for (int i = begin; i < m_epochs.size(); ++i, ++row) {
        const auto& e = m_epochs[i];
        m_table->setItem(row, 0, new QTableWidgetItem(e.timestamp.toString("dd.MM HH:mm:ss")));
        m_table->setItem(row, 1, new QTableWidgetItem(QString::number(e.heartRate, 'f', 1)));
        m_table->setItem(row, 2, new QTableWidgetItem(QString::number(e.rmssd, 'f', 1)));
        m_table->setItem(row, 3, new QTableWidgetItem(QString::number(e.movement, 'f', 2)));
        m_table->setItem(row, 4, new QTableWidgetItem(QString::number(e.respiration, 'f', 1)));
        m_table->setItem(row, 5, new QTableWidgetItem(stageName(e.stage)));
    }
}

void MainWindow::saveWakeWindow() {
    if (*m_windowStart->time() >= *m_windowEnd->time()) {
        QMessageBox::warning(this, "Ошибка", "Начало вилки должно быть раньше конца.");
        return;
    }
    QMessageBox::information(this, "Сохранено",
                             QString("Вилка: %1–%2")
                             .arg(m_windowStart->time().toString("HH:mm"))
                             .arg(m_windowEnd->time().toString("HH:mm")));
}

void MainWindow::exportCsv() {
    const QString path = QFileDialog::getSaveFileName(
        this, "Экспорт данных", "sleepguard.csv", "CSV (*.csv)");
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    QTextStream out(&file);
    out << "timestamp,heart_rate,rmssd,movement,respiration,stage\n";
    for (const auto& e : m_epochs) {
        out << e.timestamp.toString(Qt::ISODate) << ","
            << e.heartRate << ","
            << e.rmssd << ","
            << e.movement << ","
            << e.respiration << ","
            << stageName(e.stage) << "\n";
    }
    file.close();

    QMessageBox::information(this, "Экспорт", "CSV успешно сохранён.");
}

} // namespace SleepGuard
