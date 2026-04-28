#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QPushButton"
#include "QFileDialog"
#include "QInputDialog"
#include "includes/Repository.h"
#include <filesystem>


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    MainWindow::repo = nullptr;
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete repo;
}


void MainWindow::on_browseBtn1_clicked()
{
    QString selectedFolder = QFileDialog::getExistingDirectory(this, tr("Select Folder"), QDir::homePath());

    if (! selectedFolder.isEmpty()) {
        this->selectedFolder = selectedFolder;
    }
    ui->lineEdit->setText(selectedFolder);
}


void MainWindow::on_browseBtn2_clicked()
{
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("Select File"), QDir::homePath());

    if (! selectedFile.isEmpty()) {
        this->fileToAdd = selectedFile;
    }
    ui->lineEdit_2->setText(selectedFile);
}




void MainWindow::on_enterToRepo_clicked()
{
    if (selectedFolder != nullptr) {

        this->repo = new Repository(selectedFolder.toStdString());

        std::string msg = repo->init();

        ui->repoInitMessage->setText(QString::fromStdString(msg));
        ui->stackedWidget->setCurrentIndex(1);
    }

}


void MainWindow::on_addBtn_clicked()
{
    if (this->fileToAdd != nullptr) {

        std::string filename = std::filesystem::path(this->fileToAdd.toStdString()).filename().generic_string();

        std::string msg = this->repo->add(filename);

        std::vector<std::string> statuses = this->repo->status();
        ui->statusTextbox->clear();

        for (auto it: statuses) {
            ui->statusTextbox->append(QString::fromStdString(it));
        }

        ui->commitSuccessLabel->setText(QString::fromStdString(msg));
    }

}




void MainWindow::on_commitBtn_clicked()
{
    if (this->fileToAdd != nullptr) {

        std::string filename = std::filesystem::path(this->fileToAdd.toStdString()).filename().generic_string();

        bool ok;
        QString userInput = QInputDialog::getText(this, "Input Dialog", "Enter the commit message:", QLineEdit::Normal, "", &ok);

        if (ok && !userInput.isEmpty()) {
            std::string msgToShow = this->repo->commit(filename, userInput.toStdString());
            ui->commitSuccessLabel->setText(QString::fromStdString(msgToShow));
        }
    }
}





void MainWindow::on_changeRepoBtn_clicked()
{
    this->selectedFolder = nullptr;
    this->repo = nullptr;
    this->fileToAdd = nullptr;

    ui->commitSuccessLabel->clear();
    ui->statusTextbox->clear();

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();

    ui->stackedWidget->setCurrentIndex(0);
}

