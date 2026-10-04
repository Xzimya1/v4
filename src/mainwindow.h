#pragma once
#include <QMainWindow>
#include <QVector>
#include "sleepdata.h"
#include "devicecontroller.h"

class QLabel;
class QTableWidget;
class QTimeEdit;
class QPushButton;
class QProgressBar;

namespace SleepGuard {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onEpoch(const Epoch& epoch);
    void startDemo();
    void stopDemo();
    void exportCsv();
    void saveWakeWindow();

private:
    void buildUi();
    void refreshReport();
    void updateTable();

    DeviceController m_devices;
    QVector<Epoch> m_epochs;

    QLabel* m_status = nullptr;
    QLabel* m_heartRate = nullptr;
    QLabel* m_quality = nullptr;
    QLabel* m_stages = nullptr;
    QProgressBar* m_qualityBar = nullptr;
    QTableWidget* m_table = nullptr;
    QTimeEdit* m_windowStart = nullptr;
    QTimeEdit* m_windowEnd = nullptr;
    QPushButton* m_alarmButton = nullptr;
};

} // namespace SleepGuard
