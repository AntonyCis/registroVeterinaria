#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

Animal::Animal(const QString &name, const QString &type, int age, const QString &owner)
    : name(name), type(type), age(age), owner(owner) {}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(4);
    QStringList headers = {"Nombre", "Tipo", "Edad", "Dueño"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    setupConnections();

    loadFromFile();
}

MainWindow::~MainWindow() {
    saveToFile();
    delete ui;
}

void MainWindow::setupConnections() {
    connect(ui->createButton, &QPushButton::clicked, this, &MainWindow::on_btnCreate_clicked);
    connect(ui->readButton, &QPushButton::clicked, this, &MainWindow::on_btnRead_clicked);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::on_btnUpdate_clicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::on_btnDelete_clicked);
}

void MainWindow::updateTable() {
    ui->tableWidget->setRowCount(animals.size());
    for (int i = 0; i < animals.size(); ++i) {
        const Animal &animal = animals[i];
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(animal.name));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(animal.type));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(animal.age)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(animal.owner));
    }
}

void MainWindow::saveToFile() {
    QFile file("C:/Users/Administrator/Documents/Proyecto_CRUD_Veterinaria/animales.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const Animal &animal : animals) {
            out << animal.name << " | " << animal.type << " | " << animal.age << " | " << animal.owner << "\n";
        }
        file.close();
    }
}

void MainWindow::loadFromFile() {
    QFile file("C:/Users/Administrator/Documents/Proyecto_CRUD_Veterinaria/animales.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" | ");
            if (parts.size() == 4) {
                QString name = parts[0];
                QString type = parts[1];
                int age = parts[2].toInt();
                QString owner = parts[3];
                animals.append(Animal(name, type, age, owner));
            }
        }
        file.close();
        updateTable();
    }
}


void MainWindow::on_btnCreate_clicked() {
    QString name = ui->lineEditName->text();
    QString type = ui->lineEditType->text();
    int age = ui->lineEditAge->text().toInt();
    QString owner = ui->lineEditOwner->text();

    if (name.isEmpty() || type.isEmpty() || owner.isEmpty() || age <= 0) {
        QMessageBox::warning(this, "Error", "Por favor, llena todos los campos correctamente.");
        return;
    }

    for (const Animal &animal : animals) {
        if (animal.name == name) {
            QMessageBox::warning(this, "Error", "Ya existe un animal con ese nombre. Usa un nombre diferente.");
            return;
        }
    }

    animals.append(Animal(name, type, age, owner));
    updateTable();

    ui->lineEditName->clear();
    ui->lineEditType->clear();
    ui->lineEditAge->clear();
    ui->lineEditOwner->clear();
}

void MainWindow::on_btnRead_clicked() {
    int row = ui->tableWidget->currentRow();
    if (row < 0 || row >= animals.size()) {
        QMessageBox::warning(this, "Error", "Selecciona un animal para ver.");
        return;
    }
    const Animal &animal = animals[row];
    QMessageBox::information(this, "Detalles del Animal",
                             QString("Nombre: %1\nTipo: %2\nEdad: %3\nDueño: %4")
                                 .arg(animal.name)
                                 .arg(animal.type)
                                 .arg(animal.age)
                                 .arg(animal.owner));
}

void MainWindow::on_btnUpdate_clicked() {
    int row = ui->tableWidget->currentRow();
    if (row < 0 || row >= animals.size()) {
        QMessageBox::warning(this, "Error", "Selecciona un animal para actualizar.");
        return;
    }

    Animal &animal = animals[row];
    QString name = ui->lineEditName->text();
    QString type = ui->lineEditType->text();
    int age = ui->lineEditAge->text().toInt();
    QString owner = ui->lineEditOwner->text();

    if (!name.isEmpty()) animal.name = name;
    if (!type.isEmpty()) animal.type = type;
    if (age > 0) animal.age = age;
    if (!owner.isEmpty()) animal.owner = owner;

    updateTable();
}

void MainWindow::on_btnDelete_clicked() {
    int row = ui->tableWidget->currentRow();
    if (row < 0 || row >= animals.size()) {
        QMessageBox::warning(this, "Error", "Selecciona un animal para eliminar.");
        return;
    }
    animals.removeAt(row);
    updateTable();
}


