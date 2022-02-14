#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cstdint>
#include <QApplication>
#include <windows.h>

#include <QLabel>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void ShowNoteTest();
    void PrintMidiDevices();
    void KeysCheck();
    static void CALLBACK MidiInProc(HMIDIIN, UINT, DWORD, DWORD, DWORD);
    static void DetermineKeyb(DWORD);
    void DetermineChord();
    static void SortKeyb();
    static int* SortKeyb(int*);
    void InitDetermineKey(QLabel** labels);
    void DetermineKey(QLabel** labels);
    QString minusLetter(QString);
    void ShowKeyboardKey(QLabel**, QPixmap**, int*);
private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
