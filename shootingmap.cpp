#include "shootingmap.h"
#include "./ui_shootingmap.h"
#include <QMessageBox>
#include <QTextStream>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDialog>

ShootingMap::ShootingMap(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ShootingMap)
{
    ui->setupUi(this);
    initializeCourt();
    setupZoneInputs();

    connect(ui->LCPush, &QPushButton::clicked, this, &ShootingMap::onLCPushClicked);
    connect(ui->LWPush, &QPushButton::clicked, this, &ShootingMap::onLWPushClicked);
    connect(ui->TKPush, &QPushButton::clicked, this, &ShootingMap::onTKPushClicked);
    connect(ui->RWPush, &QPushButton::clicked, this, &ShootingMap::onRWPushClicked);
    connect(ui->RCPush, &QPushButton::clicked, this, &ShootingMap::onRCPushClicked);
    connect(ui->LMPush, &QPushButton::clicked, this, &ShootingMap::onLMPushClicked);
    connect(ui->TPPush, &QPushButton::clicked, this, &ShootingMap::onTPPushClicked);
    connect(ui->RMPush, &QPushButton::clicked, this, &ShootingMap::onRMPushClicked);
    connect(ui->LPPush, &QPushButton::clicked, this, &ShootingMap::onLPPushClicked);
    connect(ui->RAPush, &QPushButton::clicked, this, &ShootingMap::onRAPushClicked);
    connect(ui->RPPush, &QPushButton::clicked, this, &ShootingMap::onRPPushClicked);

    connect(ui->UndoPush, &QPushButton::clicked, this, &ShootingMap::onUndoPushClicked);
    connect(ui->FinalPush, &QPushButton::clicked, this, &ShootingMap::onFinalPushClicked);

}

ShootingMap::~ShootingMap()
{
    delete ui;
}

void ShootingMap::setupZoneInputs()
{
    zoneInputs["Left Corner"] = ui->LCInput;
    zoneInputs["Left Wing"] = ui->LWInput;
    zoneInputs["Top Key"] = ui->TKInput;
    zoneInputs["Right Wing"] = ui->RWInput;
    zoneInputs["Right Corner"] = ui->RCInput;
    zoneInputs["Left Mid"] = ui->LMInput;
    zoneInputs["Top Paint"] = ui->TPInput;
    zoneInputs["Right Mid"] = ui->RMInput;
    zoneInputs["Left Paint"] = ui->LPInput;
    zoneInputs["Restricted Area"] = ui->RAInput;
    zoneInputs["Right Paint"] = ui->RPInput;
}


void ShootingMap::initializeCourt()
{
    court = {
        { {"Left Corner"}, {"Left Wing"}, {"Top Key"}, {"Right Wing"}, {"Right Corner"} },
        { {"Left Mid"}, {"Top Paint"}, {"Right Mid"} },
        { {"Left Paint"}, {"Restricted Area"}, {"Right Paint"} }
    };
}

ShootingMap::stats* ShootingMap::find(const std::string& loc)
{
    for (auto& row : court)
        for (auto& zone : row)
            if (zone.name == loc)
                return &zone;
    return nullptr;
}

void ShootingMap::record(const std::string& loc, int m, int att)
{
    stats* zone = find(loc);
    if (zone) {
        zone->m += m;
        zone->att += att;
        hist.push({ loc, m, att });
    }
}

std::string ShootingMap::undo()
{
    if (!hist.empty()) {
        ShotEntry last = hist.top();
        hist.pop();
        stats* zone = find(last.zoneName);
        if (zone) {
            zone->m -= last.m;
            zone->att -= last.att;
        }
        return last.zoneName;
    }
    return "";
}

QString ShootingMap::display()
{
    QString result;
    result.append("------------------------------------------------------------------\n");
    for (const auto& row : court) {
        for (const auto& zone : row) {
            double perc = (zone.att > 0) ? (zone.m * 100.0 / zone.att) : 0;
            result.append(QString("| %1 %2% (%3/%4) ")
                              .arg(QString::fromStdString(zone.name), -18)
                              .arg(QString::number(perc, 'f', 0), 3)
                              .arg(zone.m, 2)
                              .arg(zone.att, 2));
        }
        result.append("\n------------------------------------------------------------------\n");
    }
    return result;
}

QString ShootingMap::total()
{
    int total_m = 0, total_att = 0;
    for (const auto& row : court)
        for (const auto& zone : row) {
            total_m += zone.m;
            total_att += zone.att;
        }
    double perc = (total_att > 0) ? (total_m * 100.0 / total_att) : 0;
    return QString("Total Shooting Percentage: %1% (%2/%3)\n")
        .arg(perc, 0, 'f', 2)
        .arg(total_m)
        .arg(total_att);
}

QString ShootingMap::tot_points()
{
    int points = 0;
    for (size_t i = 0; i < court.size(); ++i)
        for (const auto& zone : court[i])
            points += (i == 0 ? 3 : 2) * zone.m;
    return QString("Total Points Scored: %1\n").arg(points);
}

void ShootingMap::processZoneInput(const QString& zoneName, QLineEdit* inputField)
{
    QString input = inputField->text().trimmed();
    if (input.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Input for " + zoneName + " is empty!");
        return;
    }

    int m, att;
    QChar sep;
    QTextStream stream(&input);
    stream >> m >> sep >> att;

    if (sep != '/' || m < 0 || att < 0 || m > att) {
        QMessageBox::warning(this, "Format Error", "Input must be 'X/Y' where X <= Y for " + zoneName);
        return;
    }

    record(zoneName.toStdString(), m, att);
    inputField->clear();
    ui->statusbar->showMessage("Recorded: " + zoneName);
}

// Individual Push button slots:

void ShootingMap::onLCPushClicked() { processZoneInput("Left Corner", ui->LCInput); }
void ShootingMap::onLWPushClicked() { processZoneInput("Left Wing", ui->LWInput); }
void ShootingMap::onTKPushClicked() { processZoneInput("Top Key", ui->TKInput); }
void ShootingMap::onRWPushClicked() { processZoneInput("Right Wing", ui->RWInput); }
void ShootingMap::onRCPushClicked() { processZoneInput("Right Corner", ui->RCInput); }
void ShootingMap::onLMPushClicked() { processZoneInput("Left Mid", ui->LMInput); }
void ShootingMap::onTPPushClicked() { processZoneInput("Top Paint", ui->TPInput); }
void ShootingMap::onRMPushClicked() { processZoneInput("Right Mid", ui->RMInput); }
void ShootingMap::onLPPushClicked() { processZoneInput("Left Paint", ui->LPInput); }
void ShootingMap::onRAPushClicked() { processZoneInput("Restricted Area", ui->RAInput); }
void ShootingMap::onRPPushClicked() { processZoneInput("Right Paint", ui->RPInput); }

void ShootingMap::onUndoPushClicked()
{
    QString undone = QString::fromStdString(undo());
    if (!undone.isEmpty()) {
        ui->statusbar->showMessage("Undo successful for " + undone);
    } else {
        QMessageBox::information(this, "Undo", "No entries to undo!");
    }
}

void ShootingMap::onFinalPushClicked()
{
    QString finalText;
    finalText.append(display());
    finalText.append(total());
    finalText.append(tot_points());

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Final Stats Summary");
    dialog->resize(600, 400);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    QTextEdit* textOutput = new QTextEdit(dialog);
    textOutput->setReadOnly(true);
    textOutput->setPlainText(finalText);

    layout->addWidget(textOutput);
    dialog->setLayout(layout);
    dialog->exec();
}
