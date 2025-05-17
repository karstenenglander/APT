#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dbFunctions(dbManager)
{
    ui->setupUi(this);

    // Initialize database
    if (!dbManager.initialize("metadata.db")) {
        QMessageBox::critical(this, "Error", "Failed to initialize database!");
        return;
    }

    // Connect buttons to slots
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onAddImagesClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onSearchImagesClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::onRemoveImagesClicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::onListImagesClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onAddImagesClicked()
{
    dbFunctions.handleAddImages();
}

void MainWindow::onSearchImagesClicked()
{
    dbFunctions.handleSearch();
}

void MainWindow::onRemoveImagesClicked()
{
    dbFunctions.handleRemoveImages();
}

void MainWindow::onListImagesClicked()
{
    dbFunctions.handleListImages();
}
