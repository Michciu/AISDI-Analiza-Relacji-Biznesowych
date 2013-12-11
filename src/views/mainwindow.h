#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>
#include <QTextCodec>
#include "loadfilewindow.h"
#include "helpwindow.h"
#include "settingswindow.h"
#include "../models/Containers.hpp"
#include "graphspace.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //wektorki do ktorych zapisuje odebrane obiekty
    std::vector<Containers::Person> vecPerson;
    std::vector<Containers::Mail*> vecMail;

    //funkcja ta zostanie wywolana w oknie loadfilewindow, gdy wczytamy pliki i zostanie uruchomiony modul parsujacy
    void virtual UzupelnianieOkienek(/*utworzone obiekty*/);
    
    bool Flagwindow1 = true; //poniewaz nie chcemy by ktos do jednego uruchomienia programu ladowal
                          //dwa razy plikow, to po pierwszym zaladowaniu, przycisk"Zaladuj Plik" sie blokuje

private slots:
    //przyciski do otwierania osobnych okienek
    void on_actionLoadFile_activated();
    void on_actionSettings_activated();
    void on_actionHelp_activated();

private:
    //okienka ktore sie oddzielnie otwieraja
    LoadFileWindow* lfw ;
    SettingsWindow* sw;
    HelpWindow* hw;
    Ui::MainWindow *ui;

    //od pierwszej karty Zestawienie
    void UzupelnijZestawienie();

    //od drugiej karty Szczegoly
    void AddLine(Containers::Mail* mail, int lp);
    void ClearAll();
    void Sorting();
    void UzupelnijSzczegoly();
    QTreeWidget * treeWidget_MailList;

    //od trzeciej karty Graf
    void UzupelnijGraf();
    void WrzucGraf();//funkcja do testow, przy pelnej funkcjonalnosci programu mozna wywalic
    GraphSpace * graphspace;


};

#endif // MAINWINDOW_H