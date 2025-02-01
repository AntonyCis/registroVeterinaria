#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QString>
#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Animal {
public:
    Animal(const QString &name, const QString &type, int age, const QString &owner);

    QString name;
    QString type;
    int age;
    QString owner;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnCreate_clicked();
    void on_btnRead_clicked();
    void on_btnUpdate_clicked();
    void on_btnDelete_clicked();

private:
    Ui::MainWindow *ui;
    QList<Animal> animals;
    void updateTable();
    void setupConnections();
    void saveToFile();
    void loadFromFile();
};

#endif
