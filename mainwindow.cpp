#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_sender = new Sender();
    connect(this, &MainWindow::writeCommand, m_sender, &Sender::addNewMotion);
    m_sender->init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_set_nozzle_1_clicked()
{
    Motion motion;
    motion.command = Command::SET_EXTR_1_TEMP;
    motion.x = nozzle_1_req_val;
    emit writeCommand(motion);
}

void MainWindow::on_set_nozzle_2_clicked()
{
    Motion motion;
    motion.command = Command::SET_EXTR_2_TEMP;
    motion.x = nozzle_2_req_val;
    emit writeCommand(motion);
}

void MainWindow::on_nozzle_1_req_val_textEdited(const QString &arg1)
{
    nozzle_1_req_val = arg1.toFloat();
}

void MainWindow::on_nozzle_2_req_val_textEdited(const QString &arg1)
{
    nozzle_2_req_val = arg1.toFloat();
}

void MainWindow::on_homeButton_clicked()
{
    Motion motion;
    motion.command = Command::HOME_AXIS;
    emit writeCommand(motion);
    clearAll();
}

void MainWindow::on_set_speed_clicked()
{
    auto tmp_speed = ui->speed_req_val->text().toFloat();
    if(tmp_speed < 1) tmp_speed = 1;
    speed = tmp_speed;
    ui->curr_speed->setText(ui->speed_req_val->text());
}

void MainWindow::on_set_delta_x_clicked()
{
    x_req_val = ui->x_req_val->text().toFloat();
    curr_x += x_req_val;
    QString str = QString::number(curr_x);
    ui->curr_delta_x->setText(str);
}

void MainWindow::on_set_delta_y_clicked()
{
    y_req_val = ui->y_req_val->text().toFloat();
    curr_y += y_req_val;
    QString str = QString::number(curr_y);
    ui->curr_delta_y->setText(str);
}

void MainWindow::on_set_delta_z_clicked()
{
    z_req_val = ui->z_req_val->text().toFloat();
    curr_z += z_req_val;
    QString str = QString::number(curr_z);
    ui->curr_delta_z->setText(str);
}

void MainWindow::on_set_delta_e_clicked()
{
    e_req_val = ui->e_req_val->text().toFloat();
    curr_e += e_req_val;
    QString str = QString::number(curr_e);
    ui->curr_delta_e->setText(str);
}

void MainWindow::on_write_motion_clicked()
{

    Motion motion;
    motion.command = Command::SET_NEW_POSITION;
    motion.x = curr_x;
    motion.y = curr_y;
    motion.z = curr_z;
    motion.e = curr_e;
    motion.f = speed;
    emit writeCommand(motion);

//    ui->curr_delta_x->setText("0");
//    ui->curr_delta_y->setText("0");
//    ui->curr_delta_z->setText("0");
//    ui->curr_delta_e->setText("0");
    x_req_val = 0;
    y_req_val = 0;
    z_req_val = 0;
    e_req_val = 0;
    clearAll();
}
void MainWindow::clearAll()
{
    Motion motion;
    motion.command = Command::CLEAR_STATES;
    emit writeCommand(motion);
}
