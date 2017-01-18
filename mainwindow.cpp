#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->criaCB();

    ui->actionAbrir->setShortcut(QKeySequence::Open);
    this->setMaximumSize(this->size());
    this->setMinimumSize(this->size());


    //Conexões
    connect(caixasDeSelecao[0], SIGNAL(clicked()), this, SLOT(setCB()));
    connect(ui->actionAbrir, SIGNAL(triggered()), this, SLOT(slotAbrir()));
    connect(ui->pbExtrair, SIGNAL(clicked()), this, SLOT(criaWgEmail()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Funções

void MainWindow::enableUI()
{
    for(int i = 0; i < 14; i++)
        caixasDeSelecao[i]->setEnabled(true);

    ui->spinBoxDMCO->setEnabled(true);
    ui->spinBoxNT->setEnabled(true);

    ui->pbExtrair->setEnabled(true);
}
bool MainWindow::carregaCaminho()
{
    QString path = "/run/user/1000/gvfs/sftp:host=acesso.nbcgib.uesc.br,port=2222/home_nfs/computacao/gabrielrs/projetoWagyu/DDSM/cases/";
    QString dir = QFileDialog::getExistingDirectory(this, tr("Selecionar Pasta"), path, QFileDialog::ShowDirsOnly);

    QDirIterator itDir(dir, QDirIterator::Subdirectories);

    while(itDir.hasNext())
    {
        QString cam = itDir.next();

        int pos1 = cam.lastIndexOf(".");
        QString fim = cam;
        fim = fim.remove(0, pos1);
        if(fim == ".1")
            caminhoImg.append(cam);
    }
    if(caminhoImg.empty())
        return false;
    else
        return true;
}

void MainWindow::criaCaminho(QString cam, int i)
{
    int pos1 = cam.lastIndexOf("DDSM");
    QString fim = cam;
    fim = fim.remove(0, pos1 - 1);

    pos1 = fim.lastIndexOf("/");
    //Coloca ./ na frente do .raw
    fim.replace(pos1, 1, "/./");

    caminhoImg[i] = "~/projetoWagyu" + fim;
}

void MainWindow::criaCB()
{
    caixasDeSelecao = new QCheckBox*[14];
    nomesATH[0] = "Selecionar Todos";
    nomesATH[1] = "Energia";
    nomesATH[2] = "Contraste";
    nomesATH[3] = "Correlação";
    nomesATH[4] = "Variancia";
    nomesATH[5] = "MDI";
    nomesATH[6] = "Media Soma";
    nomesATH[7] = "Variancia Soma";
    nomesATH[8] = "Entropia Soma";
    nomesATH[9] = "Entropia";
    nomesATH[10] = "Variancia Diferença";
    nomesATH[11] = "Entropia Diferença";
    nomesATH[12] = "Medidas Correlação 1";
    nomesATH[13] = "Medidas Correlação 2";

    for(int i = 0, h = 30; i < 14; i++, h += 20)
    {
        caixasDeSelecao[i] = new QCheckBox(nomesATH[i], ui->frameATH);
        caixasDeSelecao[i]->setVisible(true);
        caixasDeSelecao[i]->setGeometry(10,h,191,25);
        caixasDeSelecao[i]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        caixasDeSelecao[i]->setEnabled(false);
    }
}

void MainWindow::criaWgSsh()
{
    wgSsh = new QWidget();
    wgSsh->resize(350, 160);
    wgSsh->show();
    wgSsh->raise();
    wgSsh->setWindowTitle("Login SSH");
    wgSsh->setMaximumSize(wgSsh->size());
    wgSsh->setMinimumSize(wgSsh->size());


    leSshLogin = new QLineEdit(wgSsh);
    leSshLogin->resize(145, 25);
    leSshLogin->show();
    leSshLogin->move(20, 25);
    leSshLogin->setText("gabrielrs");

    QLabel *lbEmail = new QLabel("User", wgSsh);
    lbEmail->show();
    lbEmail->move(leSshLogin->pos().x(), leSshLogin->pos().y() - 20);

    leSshHost = new QLineEdit(wgSsh);
    leSshHost->resize(145, 25);
    leSshHost->show();
    leSshHost->move(leSshLogin->pos().x() + leSshHost->size().width() + 10, 25);
    leSshHost->setText("acesso.nbcgib.uesc.br");

    QLabel *lbHost = new QLabel("Host", wgSsh);
    lbHost->show();
    lbHost->move(leSshHost->pos().x(), leSshHost->pos().y() - 20);

    leSshPass = new QLineEdit(wgSsh);
    leSshPass->resize(145, 25);
    leSshPass->show();
    leSshPass->move(wgSsh->size().width() / 2 - leSshPass->size().width() / 2, 80);
    leSshPass->setEchoMode(QLineEdit::Password);
    leSshPass->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);

    QLabel *lbPass = new QLabel("Password", wgSsh);
    lbPass->show();
    lbPass->move(leSshPass->pos().x(), leSshPass->pos().y() - 20);

    QPushButton *pbSsh = new QPushButton("Enviar", wgSsh);
    pbSsh->show();
    pbSsh->move(wgSsh->size().width() / 2 - pbSsh->size().width() / 2, 120);

    connect(pbSsh, SIGNAL(clicked()), this, SLOT(sshConnected()));
}


//Slots

void MainWindow::setCB()
{
    for(int i = 0; i < 14; i++)
        caixasDeSelecao[i]->setChecked(caixasDeSelecao[0]->isChecked());
}

void MainWindow::slotAbrir()
{
    if(this->carregaCaminho())
    {
        for(int i = 0; i < caminhoImg.size(); i++)
        {
            QString nome = caminhoImg.at(i);
            int pos = nome.lastIndexOf("/");
            nome = nome.remove(0, pos + 1);

            ui->tbIMG->append(nome);

            this->enableUI();
        }
    }
}

void MainWindow::criaWgEmail()
{
    wgEmail = new QWidget();
    wgEmail->resize(350, 100);
    wgEmail->show();
    wgEmail->raise();
    wgEmail->setWindowTitle("Email");
    wgEmail->setMaximumSize(wgEmail->size());
    wgEmail->setMinimumSize(wgEmail->size());

    QLabel *lbEmail = new QLabel("Insira seu Email", wgEmail);
    lbEmail->show();
    lbEmail->move(wgEmail->size().width() / 2 - lbEmail->size().width() / 2, 5);

    leEmail = new QLineEdit(wgEmail);
    leEmail->resize(300, 25);
    leEmail->show();
    leEmail->move(wgEmail->size().width() / 2 - leEmail->size().width() / 2, 25);

    QPushButton *pbEmail = new QPushButton("Enviar", wgEmail);
    pbEmail->show();
    pbEmail->move(wgEmail->size().width() / 2 - pbEmail->size().width() / 2, 60);
    connect(pbEmail, SIGNAL(clicked()), this, SLOT(slotExtrair()));
}

void MainWindow::slotExtrair()
{

    this->userEmail = this->leEmail->text();
    wgEmail->close();

    std::ofstream arqSlurm;
    std::ofstream arqLabel;


    QString dataEtempo = QDateTime::currentDateTime().toString("yyMMddhhmmss");

    srm = "/run/user/1000/gvfs/sftp:host=acesso.nbcgib.uesc.br,port=2222/home_nfs/computacao/gabrielrs/projetoWagyu/Run/ATH" + dataEtempo;

    QByteArray ba = srm.toLatin1();
    const char *c_str = ba.data();
    arqLabel.open(c_str);

    srm += ".srm";
    ba = srm.toLatin1();
    c_str = ba.data();
    arqSlurm.open(c_str);


    arqSlurm << "#!/bin/bash" << std::endl;
    arqSlurm << "#SBATCH -J " << "ATH" << "_" << dataEtempo.toStdString() << std::endl;
    arqSlurm << "#SBATCH --mail-type=ALL" << std::endl;
    arqSlurm << "#SBATCH --mail-user=" << userEmail.toStdString() << std::endl;
    arqSlurm << "#SBATCH --output=" << "%j_" << "ATH" << std::endl;
    arqSlurm << "#SBATCH -p gpu" << std::endl;
    arqSlurm << "#SBATCH -n 1" << std::endl;
    arqSlurm << "#SBATCH -c " << ui->spinBoxNT->value() << std::endl;
    arqSlurm << "#SBATCH --exclusive" << std::endl;
    arqSlurm << "export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK" << std::endl;

    arqSlurm << "echo '" << caminhoImg.size() << ";";
    for(int i = 1; i < 14; i++)
        if(caixasDeSelecao[i]->isChecked())
            arqSlurm << nomesATH[i].toStdString() << ";";
    arqSlurm << "'" << std::endl;

    for(int j = 0; j < caminhoImg.size(); j++)
    {
        if(caminhoImg.at(j).contains("cancers"))
            arqLabel << "Cancer" << std::endl;
        else if (caminhoImg.at(j).contains("normals"))
            arqLabel << "Normal" << std::endl;
        else
            arqLabel << "Benigno" << std::endl;

        int pos = caminhoImg.at(j).lastIndexOf("1");
        QString dimensao = caminhoImg.at(j);
        dimensao = dimensao.replace(pos, 1, "txt");

        QByteArray ba = dimensao.toLatin1();
        const char *c_str2 = ba.data();
        std::fstream dim(c_str2, std::ios_base::in);
        int largura, altura;
        dim >> largura >> altura;
        dim.close();

        criaCaminho(caminhoImg.at(j), j);

        arqSlurm << "srun ~/projetoWagyu/ATH/./ATH " << caminhoImg.at(j).toStdString() << " " << largura << " " << altura << " 12 " << ui->spinBoxDMCO->value() << " ";
        for(int i = 1; i < 14; i++)
        {
            if(caixasDeSelecao[i]->isChecked())
                arqSlurm << i << " ";
            else
                arqSlurm << "0 ";
        }

        arqSlurm << std::endl;
    }

    arqSlurm.close();
    arqLabel.close();

    this->criaWgSsh();
}

void MainWindow::sshConnected()
{
    proc = new QSshSocket();
    proc->connectToHost(leSshHost->text(), 2222);
    proc->login(leSshLogin->text(), leSshPass->text());
    wgSsh->close();

    int pos1 = srm.lastIndexOf("Run");
    QString fim = srm;
    fim = fim.remove(0, pos1 - 1);

    srm = "~/projetoWagyu" + fim;

    QString send = "sbatch " + srm;

    proc->executeCommand(send);

    QMessageBox::information(this, tr("Extração"),tr("Arquivos enviados para extração!"));
}
