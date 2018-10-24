#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "current_proto/sender.h"
#include "motion/motion.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Sender * m_sender;

private slots:
    void on_set_nozzle_1_clicked();

    void on_set_nozzle_2_clicked();

    void on_nozzle_1_req_val_textEdited(const QString &arg1);

    void on_nozzle_2_req_val_textEdited(const QString &arg1);

    void on_homeButton_clicked();

    void on_set_speed_clicked();

    void on_set_delta_x_clicked();

    void on_set_delta_y_clicked();

    void on_set_delta_z_clicked();

    void on_set_delta_e_clicked();

    void on_write_motion_clicked();

private:
    QTimer * m_pressedTimer;
    void clearAll();
    Ui::MainWindow *ui;
    float nozzle_1_req_val = 0;
    float nozzle_2_req_val = 0;
    float chamber_req_val = 0;
    float speed = 1000;
    float x_req_val = 0;
    float y_req_val = 0;
    float z_req_val = 0;
    float e_req_val = 0;

    float curr_x = 0;
    float curr_y = 0;
    float curr_z = 0;
    float curr_e = 0;
    volatile bool _isBtnPressed = false;
    void moveAt(Command cmd, float newSpeed);
    void stop();

signals:
    void writeCommand(Motion motion);
private slots:
    void on_Xminus_pressed();
    void on_Xminus_released();
    void on_Xplus_pressed();
    void on_Xplus_released();
    void on_Yminus_pressed();
    void on_Yminus_released();
    void on_Yplus_pressed();
    void on_Yplus_released();
    void on_Eminus_pressed();
    void on_Eminus_released();
    void on_Eplus_pressed();
    void on_Eplus_released();
    void on_Zminus_pressed();
    void on_Zminus_released();
    void on_Zplus_pressed();
    void on_Zplus_released();
};

#endif // MAINWINDOW_H
