#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QRectF>
#include <QLabel>
#include <QTimer>
#include <cstdint>
#include <QApplication>
#include <QPixmap>
#include <windows.h>
#include <QDir>

const int maxAmountOfKeysb = 10;

DWORD a[15];
int currentAmount = 0;
int sharpflat = 0;

void MainWindow::PrintMidiDevices(){
    UINT nMidiDeviceNum;
    MIDIINCAPS caps;

    nMidiDeviceNum = midiInGetNumDevs();
    if (nMidiDeviceNum == 0) {
        fprintf(stderr, "midiInGetNumDevs() return 0...");
        return;
    }

    printf("== PrintMidiDevices() == \n");
    for (unsigned int i = 0; i < nMidiDeviceNum; ++i) {
        midiInGetDevCaps(i, &caps, sizeof(MIDIINCAPS));
        printf("\t%d : name = %s\n", i, caps.szPname);
    }
    printf("=====\n");
}

void MainWindow::SortKeyb(){

    DWORD mem;
    for(int i = 0; i < currentAmount - 1; i++){
        if(a[i] > a[i + 1]){
            mem = a[i];
            a[i] = a[i + 1];
            a[i + 1] = mem;
            i = -1;
        }
    }

    return;
}
int* MainWindow::SortKeyb(int* local_a){

    DWORD mem;
    for(int i = 0; i < currentAmount - 1; i++){
        if(local_a[i] > local_a[i + 1]){
            mem = local_a[i];
            local_a[i] = local_a[i + 1];
            local_a[i + 1] = mem;
            i = -1;
        }
    }

    return local_a;
}

void MainWindow::DetermineKeyb(DWORD currentKeyb){
    DWORD firstNoteHex = 0x15000000;

    currentKeyb >>= 4 * 2;
    currentKeyb <<= 4 * 6;

    DWORD currentNote = currentKeyb - firstNoteHex;
    currentNote >>= 4 * 6;

    bool found = false;

    for(int i = 0; i < currentAmount; i++){
        //printf("looooooooooooooool");
        if(currentNote == a[i]){
            found = true;
            a[i] = 0;
            while(a[i + 1] != 0){
                a[i] = a[i + 1];
                a[i + 1] = 0;
                i++;
            }
            break;
        }
    }

    if(found) currentAmount--;
    else{
        a[currentAmount] = currentNote;
        currentAmount++;

        if(currentAmount > 1){
            SortKeyb();
        }
    }

    return;
}

QString MainWindow::minusLetter(QString inp){
    QString res = "";
    for(int i = 0; i < inp.length() - 1; i++){
        res += inp[i];
    }

    return res;
}

void MainWindow::DetermineChord(){

    int *local_a = new int[15];
    for(int i = 0; i < 15; i++){
        if(a[i] != 0) local_a[i] = a[i];
        else local_a[i] = 0;
        //printf("%i ",local_a[i]);
    }

    const int chordAmount = 9;
    const int maxInterval = 5;

    QString keybNames[2][12] = {
        {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"},
        {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"}
    };
    QString chordNames[3][chordAmount - 1] =
        {{"5", "", "", "", "", "", "", ""},
         {"", "m", "dim", "aug", "sus2", "sus4", "", ""},
         {"7", "m7", "maj7", "mM7", "6", "m6", "add2", "add4"}
    };

    QString otherExtentions[9] = {
        "b9", "9", "#9", "b11", "11", "#11", "b13", "13", "#13"
    };
    int extentionValues[9] = {
        13, 14, 15, 16, 17, 18, 20, 21, 22
    };
    long long int extentionHex = 0x000000000;


    int chordIntervals[3][chordAmount][maxInterval] = {
        {{7, 0, 0, 0, 0}, {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}},
        {{4, 3, 0, 0, 0}, {3, 4, 0, 0, 0}, {3, 3, 0, 0, 0}, {4, 4, 0, 0, 0}, {2, 5, 0, 0, 0}, {5, 2, 0, 0, 0}, {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1}},
        {{4, 3, 3, 0, 0}, {3, 4, 3, 0, 0}, {4, 3, 4, 0, 0}, {3, 4, 4, 0, 0}, {4, 3, 2, 0, 0}, {3, 4, 2, 0, 0}, {2, 2, 3, 0, 0}, {3, 2, 2, 0, 0}, {-1, -1, -1, -1, -1}}
    };

    int firstNoteIndex = 9;
    int currentNoteIndex;

    int inversionKeyb = -1;

    bool found = true;
    bool done = false;
    int currentIndex;
    int chordType;
    int currentInterval;

    int usedAmount;
    if(currentAmount > 4) usedAmount = 4;
    else usedAmount = currentAmount;


    for(int count = 0; count < usedAmount; count++){
        found = true;
        done = false;
        for(chordType = 0; chordType < chordAmount; chordType++){
            currentIndex = 1;
            for(currentInterval = 0; currentInterval < maxInterval; currentInterval++){
                if(chordIntervals[usedAmount - 2][chordType][currentInterval] == -1){
                    done = true;
                    found = false;
                    break;
                }
                else if(chordIntervals[usedAmount - 2][chordType][currentInterval] == 0) done = true;

                if(done) break;

                if((int)(local_a[currentIndex] - local_a[currentIndex - 1]) != chordIntervals[usedAmount - 2][chordType][currentInterval]){
                    break;
                }
                currentIndex++;
            }
            if(done) break;
        }
        if(found) break;


        if(usedAmount > 2){
            if(count == 0) inversionKeyb = (int)local_a[0];
            //printf("%i\n", inversionKeyb);
            //printf("%i %i %i %i %i\n", local_a[0], local_a[1], local_a[2], local_a[3], local_a[4]);
            local_a[0] += 12;

            //printf("%i %i %i %i %i\n", local_a[0], local_a[1], local_a[2], local_a[3], local_a[4]);
            local_a = SortKeyb(local_a);
            //printf("%i %i %i %i %i\n", local_a[0], local_a[1], local_a[2], local_a[3], local_a[4]);
        }
    }
    if(usedAmount > 2){
        long long int tempHex;
        for(int i = 0; i < currentAmount - 1; i++){
            tempHex = 0x100000000;
            for(int j = 0; j < 9; j++){
                if(local_a[currentAmount - i] - local_a[0] == extentionValues[j]){
                    extentionHex += tempHex;
                    break;
                }
                tempHex >>= 4;
            }
        }
    }
    //printf("%#09x\n", extentionHex);



    QString resChord = "";

    int resExtention = 0;
    bool fullExtention = false;
    currentNoteIndex = (firstNoteIndex + (int)local_a[0]) % 12;

    bool extention = false;
    if(found){
        if(extentionHex != 0x000000000) extention = true;

        if(!extention) resChord = keybNames[sharpflat][currentNoteIndex] + chordNames[usedAmount - 2][chordType];
        else{
            switch(extentionHex){
                case 0x010010010:
                    fullExtention = true;
                    resExtention = 7;
                    break;
                case 0x010010000:
                    fullExtention = true;
                    resExtention = 4;
                    break;
                case 0x010000000:
                    fullExtention = true;
                    resExtention = 1;
                    break;
            }
            if(fullExtention) resChord = keybNames[sharpflat][currentNoteIndex] + minusLetter(chordNames[usedAmount - 2][chordType]) + otherExtentions[resExtention];
            else{
                resChord = keybNames[sharpflat][currentNoteIndex] + chordNames[usedAmount - 2][chordType];
                if(usedAmount == 4) resChord += "(";

                long long int local_extentionHex;
                for(int i = 8; i > -1; i--){
                    local_extentionHex = extentionHex << (4 * (8 - i));
                    local_extentionHex >>= 4 * 8;
                    //printf("%i\n", local_extentionHex);
                    if(local_extentionHex == 0x000000001){
                        if(usedAmount == 4) resChord += otherExtentions[8 - i];
                    }
                }
                resChord += ")";
            }
        }

        if(inversionKeyb != -1){
           resChord += "/" + keybNames[sharpflat][((firstNoteIndex + inversionKeyb) % 12)];
        }

        ui->label->setText(resChord);
    }
}

void MainWindow::InitDetermineKey(QLabel** labels){
    QString keys[2][12] = {{"C", "G", "D", "A", "E", "B", "F#/Gb", "C#/Db", "G#/Ab", "D#/Eb", "A#/Bb", "F"},
                           {"A", "E", "B", "F#/Gb", "C#/Db", "G#/Ab", "D#/Eb", "A#/Bb", "F", "C", "G", "D"}
    };

    QFormLayout *formLayout = new QFormLayout;
    QHBoxLayout *hBox = new QHBoxLayout;

    QLabel* names[2];
    names[0] = new QLabel("Major", this);
    names[1] = new QLabel("Minor", this);
    names[0]->setGeometry(20, 270, 80, 30);
    names[1]->setGeometry(120, 270, 80, 30);
    names[0]->setStyleSheet("color:white");
    names[1]->setStyleSheet("color:white");
    hBox->addWidget(names[0]);
    hBox->addWidget(names[1]);

    QFont font;
    font.setFamily("Times New Roman");
    font.setItalic(true);

    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 12; j++){
            labels[12 * i + j] = new QLabel(keys[i][j], this);
            labels[12 * i + j]->setGeometry(20 + i * 100, 300 + j * 20, 80, 30);
            labels[12 * i + j]->setFont(font);
            labels[12 * i + j]->setStyleSheet("color:white");
            hBox->addWidget(labels[2 * i + j]);
        }
    }



    formLayout->addRow(hBox);
    this->setLayout(formLayout);
}

void MainWindow::DetermineKey(QLabel** labels){
    /*
     * C     - 0
     * C#/Db - 1
     * D     - 2
     * D#/Eb - 3
     * E     - 4
     * F     - 5
     * F#/Gb - 6
     * G     - 7
     * G#/Ab - 8
     * A     - 9
     * A#/Bb - 10
     * B     - 11
     */

    int keyList[12][7] = {
        {0, 2, 4, 5, 7, 9, 11},
        {0, 2, 4, 6, 7, 9, 11},
        {1, 2, 4, 6, 7, 9, 11},
        {1, 2, 4, 6, 8, 9, 11},
        {1, 3, 4, 6, 8, 9, 11},
        {1, 3, 4, 6, 8, 10, 11},
        {1, 3, 5, 6, 8, 10, 11},
        {0, 1, 3, 5, 6, 8, 10},
        {0, 1, 3, 5, 7, 8, 10},
        {0, 2, 3, 5, 7, 8, 10},
        {0, 2, 3, 5, 7, 9, 10},
        {0, 2, 4, 5, 7, 9, 10}
    };

    bool* keyBoolList = new bool[12];

    for(int i = 0; i < 12; i++){
        keyBoolList[i] = false;
    }

    int* local_a = new int[15];
    for(int i = 0; i < 15; i++){
        if(a[i] != 0) local_a[i] = a[i];
        else local_a[i] = 0;
        //printf("%i ",local_a[i]);
    }

    for(int i = 0; i < currentAmount; i++){
        local_a[i] = (9 + (int)local_a[i]) % 12;
        //printf("%i", local_a[i]);
    }

    bool* memKeyb = new bool[7];

    int count;
    for(int i = 0; i < 12; i++){
        count = 0;
        for(int j = 0; j < 7; j++) memKeyb[j] = false;

        for(int j = 0; j < 7; j++){
            for(int k = 0; k < currentAmount; k++){
                if((int)local_a[k] == keyList[i][j]){
                    count++;
                    memKeyb[j] = true;
                }
            }
            if(count == currentAmount){
                keyBoolList[i] = true;
                break;
            }
        }
    }

    for(int i = 0; i < 12; i++){
        if(keyBoolList[i]){
            labels[i]->setStyleSheet("QLabel {color: green}");
            labels[12 + i]->setStyleSheet("QLabel {color: green}");
        }
        else{
            labels[i]->setStyleSheet("QLabel {color: red}");
            labels[12 + i]->setStyleSheet("QLabel {color: red}");
        }
    }
}


void CALLBACK MainWindow::MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2){

    switch(wMsg) {
        case MIM_OPEN:
            printf("wMsg=MIM_OPEN\n");
            break;
        case MIM_CLOSE:
            printf("wMsg=MIM_CLOSE\n");
            break;
        case MIM_DATA:
            //printf("wMsg=MIM_DATA, dwInstance=%08x, dwParam1=%08x, dwParam2=%08x\n", dwInstance, dwParam1, dwParam2);
            if(dwParam1 != 0x000000f8 && dwParam1 != 0x000000fe){
                DetermineKeyb(dwParam1);
            }
            break;
        case MIM_LONGDATA:
            printf("wMsg=MIM_LONGDATA\n");
            break;
        case MIM_ERROR:
            printf("wMsg=MIM_ERROR\n");
            break;
        case MIM_LONGERROR:
            printf("wMsg=MIM_LONGERROR\n");
            break;
        case MIM_MOREDATA:
            printf("wMsg=MIM_MOREDATA\n");
            break;
        default:
            printf("wMsg = unknown\n");
            break;
        }
    return;
}

void MainWindow::ShowKeyboardKey(QLabel** pianoKeyLabel, QPixmap** pianoKeyTypes, int* pianoKeyTypesVals){
    int* local_a = new int[15];
    for(int i = 0; i < 15; i++){
        if(a[i] != 0) local_a[i] = a[i];
        else local_a[i] = 0;
        //printf("%i ",local_a[i]);
    }

    for(int i = 0; i < currentAmount; i++){
        local_a[i] = (9 + (int)local_a[i]) % 12;
        //printf("%i\n", local_a[i]);
    }

    for(int i = 0; i < 36; i++) pianoKeyLabel[i]->setPixmap(pianoKeyTypes[pianoKeyTypesVals[i % 12]][0]);

    int count = 0;
    for(int i = 0; i < 36; i++){
        if((i % 12) == local_a[count]){
            pianoKeyLabel[i]->setPixmap(pianoKeyTypes[pianoKeyTypesVals[i % 12]][1]);
            ++count;
            if(count == currentAmount) break;
        }
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int pianoKeyTypesVals[12] = {0, 3, 1, 3, 2, 0, 3, 1, 3, 1, 3, 2};
    QLabel* pianoKeyLabel[36];

    pianoKeyLabel[0] = ui->label_2;
    pianoKeyLabel[1] = ui->label_23;
    pianoKeyLabel[2] = ui->label_3;
    pianoKeyLabel[3] = ui->label_24;
    pianoKeyLabel[4] = ui->label_4;
    pianoKeyLabel[5] = ui->label_5;
    pianoKeyLabel[6] = ui->label_25;
    pianoKeyLabel[7] = ui->label_6;
    pianoKeyLabel[8] = ui->label_26;
    pianoKeyLabel[9] = ui->label_7;
    pianoKeyLabel[10] = ui->label_27;
    pianoKeyLabel[11] = ui->label_8;

    pianoKeyLabel[12] = ui->label_11;
    pianoKeyLabel[13] = ui->label_30;
    pianoKeyLabel[14] = ui->label_14;
    pianoKeyLabel[15] = ui->label_29;
    pianoKeyLabel[16] = ui->label_9;
    pianoKeyLabel[17] = ui->label_15;
    pianoKeyLabel[18] = ui->label_31;
    pianoKeyLabel[19] = ui->label_12;
    pianoKeyLabel[20] = ui->label_28;
    pianoKeyLabel[21] = ui->label_10;
    pianoKeyLabel[22] = ui->label_32;
    pianoKeyLabel[23] = ui->label_13;

    pianoKeyLabel[24] = ui->label_19;
    pianoKeyLabel[25] = ui->label_35;
    pianoKeyLabel[26] = ui->label_22;
    pianoKeyLabel[27] = ui->label_34;
    pianoKeyLabel[28] = ui->label_20;
    pianoKeyLabel[29] = ui->label_17;
    pianoKeyLabel[30] = ui->label_36;
    pianoKeyLabel[31] = ui->label_21;
    pianoKeyLabel[32] = ui->label_33;
    pianoKeyLabel[33] = ui->label_16;
    pianoKeyLabel[34] = ui->label_37;
    pianoKeyLabel[35] = ui->label_18;


    QString pianoKeyPath = qApp->applicationDirPath();
    QString pianoKeyNames[4] = {"\\key\\key1", "\\key\\key2", "\\key\\key3", "\\key\\keyb"};
    QString pianoKeyNamesExtention[2] = {"_u.png", "_p.png"};
    QPixmap** pianoKeyTypes;
    pianoKeyTypes = new QPixmap*[4];
    for(int i = 0; i < 4; i++) pianoKeyTypes[i] = new QPixmap[2];

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 2; j++){
            pianoKeyTypes[i][j].load(pianoKeyPath + pianoKeyNames[i] + pianoKeyNamesExtention[j]);
            if(i == 3) pianoKeyTypes[i][j] = pianoKeyTypes[i][j].scaled(pianoKeyLabel[1]->size(), Qt::KeepAspectRatio);
            else pianoKeyTypes[i][j] = pianoKeyTypes[i][j].scaled(pianoKeyLabel[0]->size(), Qt::KeepAspectRatio);
        }
    }
    for(int i = 0; i < 36; i++){
        pianoKeyLabel[i]->setAlignment(Qt::AlignCenter);
        pianoKeyLabel[i]->setPixmap(pianoKeyTypes[pianoKeyTypesVals[i % 12]][0]);
    }



    for(int i = 0; i < 15; i++){
        a[i] = 0;
    }

    HMIDIIN hMidiDevice = NULL;
    DWORD nMidiPort = 0;
    UINT nMidiDeviceNum;
    MMRESULT rv;
    //printf("loooooll");

    PrintMidiDevices();

    nMidiDeviceNum = midiInGetNumDevs();
    if (nMidiDeviceNum == 0) {
        fprintf(stderr, "midiInGetNumDevs() return 0...");
    }

    //void (*MidiInProc)(HMIDIIN, UINT, DWORD, DWORD, DWORD);

    midiInOpen(&hMidiDevice, nMidiPort, (intptr_t)(MidiInProc), 0, CALLBACK_FUNCTION);

    rv = midiInStart(hMidiDevice);

    // Ініціалізація інтерфесу

    QLabel* labels[24];

    InitDetermineKey(labels); // Ініціалізація панелі тональності

    ui->label->setText("");

    int amount_mem = 0;

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() mutable {
        //printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
        if(amount_mem != currentAmount){
            if(currentAmount > 1 && currentAmount < maxAmountOfKeysb + 1){
                DetermineChord();
            }
            else{
                ui->label->setText("");
            }

            if(currentAmount > 0){
                DetermineKey(labels);
                ShowKeyboardKey(pianoKeyLabel, pianoKeyTypes, pianoKeyTypesVals);
            }
            else{
                for(int i = 0; i < 12; i++){
                    labels[i]->setStyleSheet("QLabel {color: white}");
                    labels[12 + i]->setStyleSheet("QLabel {color: white}");
                    pianoKeyLabel[i]->setPixmap(pianoKeyTypes[pianoKeyTypesVals[i]][0]);
                    pianoKeyLabel[12 + i]->setPixmap(pianoKeyTypes[pianoKeyTypesVals[i]][0]);
                    pianoKeyLabel[24 + i]->setPixmap(pianoKeyTypes[pianoKeyTypesVals[i]][0]);
                }
            }
            amount_mem = currentAmount;
        //printf("\nAAAAAAAAAAAAAAA\n");
        //ShowNoteTest();
        }
    });
    timer->setInterval(10);
    timer->start();
    //midiInStop(hMidiDevice);
    //midiInClose(hMidiDevice);
    //hMidiDevice = NULL;

}


void MainWindow::ShowNoteTest(){

    /*if(a != -1){
        ui->label->setText(keybNames[0][a]);
    }
    */
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_radioButton_clicked()
{
    sharpflat = 0;
}

void MainWindow::on_radioButton_2_clicked()
{
    sharpflat = 1;
}
