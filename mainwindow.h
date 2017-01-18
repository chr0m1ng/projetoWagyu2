#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDirIterator>
#include <QFileDialog>
#include <fstream>
#include <iostream>
#include <QLineEdit>
#include <QMessageBox>
#include <QCheckBox>
#include <QDateTime>
#include <QProcess>
#include "qsshsocket.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<QString> caminhoImg;
    QString userEmail;
    QWidget *wgEmail;
    QWidget  *wgSsh;
    QLineEdit *leEmail;
    QLineEdit *leSshLogin;
    QLineEdit *leSshPass;
    QLineEdit *leSshHost;
    QString srm;
    QSshSocket *proc;

    //Checkbox
    QCheckBox **caixasDeSelecao;
    QString nomesATH[14];

    void criaWgSsh();
    void criaCB();
    void criaCaminho(QString, int);
    void enableUI();
    bool carregaCaminho();

private slots:
    void sshConnected();
    void setCB();
    void slotAbrir();
    void slotExtrair();
    void criaWgEmail();
};

#endif // MAINWINDOW_H
