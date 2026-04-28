#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <includes/Repository.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief The MainWindow class represents the main window of a Qt GUI application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a MainWindow object.
     * @param parent The parent widget.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroys the MainWindow object.
     */
    ~MainWindow();



private slots:
    /**
     * @brief Slot function for the "Browse" button to select a folder.
     */
    void on_browseBtn1_clicked();

    /**
     * @brief Slot function for the "Browse" button to select a file.
     */
    void on_browseBtn2_clicked();

    /**
     * @brief Slot function for the "Enter to Repo" button to initialize the repository.
     */
    void on_enterToRepo_clicked();

    /**
     * @brief Slot function for the "Add" button to add a file to the repository.
     */
    void on_addBtn_clicked();

    /**
     * @brief Slot function for the "Commit" button to commit changes to a file.
     */
    void on_commitBtn_clicked();

    /**
     * @brief Slot function for the "Change Repo" button to go back to the initial state.
     */
    void on_changeRepoBtn_clicked();

private:
    Ui::MainWindow *ui;
    QString selectedFolder;
    Repository* repo;
    QString fileToAdd;

};
#endif // MAINWINDOW_H
