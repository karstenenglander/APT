#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "C:\Users\karst\Documents\CS\APT\include\DatabaseFunctions.h"
#include "C:\Users\karst\Documents\CS\APT\include\DatabaseManager.h"
#include "C:\Users\karst\Documents\CS\APT\include\ImageProcessor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slot functions that will be connected to buttons
    void onAddImagesClicked();
    void onSearchImagesClicked();
    void onRemoveImagesClicked();
    void onListImagesClicked();

private:
    Ui::MainWindow *ui;
    DatabaseManager dbManager;
    DatabaseFunctions dbFunctions;
};

#endif // MAINWINDOW_H
