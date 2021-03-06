#include "loadfilewindow.h"
#include "ui_loadfilewindow.h"
#include "mainwindow.h"

#include "../controllers/parser.hpp"
#include "../models/Containers.hpp"

#include "../utils/quicksort.h"

LoadFileWindow::LoadFileWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoadFileWindow)
{
    ui->setupUi(this);
}

LoadFileWindow::~LoadFileWindow()
{
    delete ui;
}


void LoadFileWindow::on_toolButton_AddFolder_clicked()
{
    QString *folderPath = new QString();
    QString czyZmieniony = *folderPath;
    *folderPath = QFileDialog::getExistingDirectory(this, tr("Dodaj folder"), "");
    if(*folderPath == czyZmieniony)
        return;
    QDir mydir(*folderPath);
    QString filePath;
    QStringList nameFilter;
    nameFilter << "*.eml";
    QFileInfoList list = mydir.entryInfoList( nameFilter, QDir::Files );

    for(int i = 0; i < list.size(); i++)
    {
        AddLine( list.at(i) );
    }
}

void LoadFileWindow::on_toolButton_AddFile_clicked()
{
    QString *filePath = new QString();
    QString czyZmieniony = *filePath;
    *filePath = QFileDialog::getOpenFileName(this, tr("Dodaj plik"), "", tr("Files (*.eml*)"));
    if(*filePath != czyZmieniony)
    {
        QFileInfo infoPath(*filePath);
        AddLine(infoPath);
    }
}
void LoadFileWindow::AddLine(QFileInfo infoPath)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_FilesList);
    item->setText(0,infoPath.baseName());
    item->setText(1, QString::number(infoPath.size()) );
    item->setText(2, infoPath.absoluteFilePath());
    ui->treeWidget_FilesList->addTopLevelItem(item);
}

void LoadFileWindow::on_toolButton_DeleteFile_clicked()
{
    QTreeWidgetItem * item = ui->treeWidget_FilesList->currentItem();
    int itemIndex = ui->treeWidget_FilesList->indexOfTopLevelItem(item);
    delete ui->treeWidget_FilesList->takeTopLevelItem(itemIndex);
}

std:: vector<std::string> LoadFileWindow::GetPathList()
{
    return mainFileNameList;
}

bool myfunction (Containers::Mail* left,Containers::Mail* right) {
	return (left->sendDate.getUnixTimestamp() < right->sendDate.getUnixTimestamp());
}

//ostatnia dzialajaca funkcja w tym oknie przed zamknieciem !!!
void LoadFileWindow::on_pushButton_Confirm_clicked()
{
    do
    {
        takenItem = ui->treeWidget_FilesList->takeTopLevelItem(0);
        if(takenItem)
        {
            QString tmpPath = takenItem->text(2);
            takenPath = tmpPath.toStdString();
            mainFileNameList.push_back(takenPath);
        }
    }while(takenItem);

    std::vector<Containers::Mail*> tempVec;
    FileParser parser;

    for(std::string& it: mainFileNameList)
    {
        tempVec.push_back(parser.load_plik(it.c_str()));
    }
//	SortedMailsComparator comp;
//	quicksort<Containers::Mail>(tempVec, 0, tempVec.size() - 1, comp);
//	parser.checkForwards(tempVec);
	std::sort (tempVec.begin(), tempVec.end(), myfunction);
	

/*
    w tym miejscu powininien zostac uruchomiony modul parsujacy do ktorego zostanie przekazany vector stringow
    z sciezkami do plikow do sparsowania,
    modul powinien zwrocic gotowe obiekty osob i maili, ktore zostana przekazane do okna mainwindow i w nim
    odpowiednio wyswietlone

    do modułu parsującego ma zostać przekazany obiekt mainFileNameList - vector stringow z sciezkami plikow

    po wykonaniu parsowania i utworzenia obiektow, powinny one zostac przekazane do funkcji ponizej:
            myParent->UzupelnianieOkienek(utworzone obiekty);

*/

	cout << "---Check forwards---\n";
	parser.loadFwds(tempVec);
	cout << "---END---\n";
	
	for(auto x: tempVec)
	{
		cout << "Subject: " << x->headers.getHeader("Subject") << ": " << endl;
		cout << "basePointer = " << x->forwardBase << endl;
		cout << "other pointers count = " << x->forwardAll.size() << (x->forwardAll.size() > 0 ? "!!!" : "") << endl;
		cout << "---\n";
	}
	
	
    myParent->UzupelnianieOkienek(tempVec, parser.getCache(), &parser);
    parentWidget()->setEnabled(true);
    close();
}

void LoadFileWindow::closeEvent (QCloseEvent *)
{
    parentWidget()->setEnabled(true);
}
