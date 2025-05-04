#ifndef SHOOTINGMAP_H
#define SHOOTINGMAP_H

#include <QMainWindow>
#include <QMap>
#include <QLineEdit>
#include <stack>
#include <vector>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
class ShootingMap;
}
QT_END_NAMESPACE

class ShootingMap : public QMainWindow
{
    Q_OBJECT

public:
    ShootingMap(QWidget *parent = nullptr);
    ~ShootingMap();

private slots:
    void onLCPushClicked();
    void onLWPushClicked();
    void onTKPushClicked();
    void onRWPushClicked();
    void onRCPushClicked();
    void onLMPushClicked();
    void onTPPushClicked();
    void onRMPushClicked();
    void onLPPushClicked();
    void onRAPushClicked();
    void onRPPushClicked();
    void onUndoPushClicked();
    void onFinalPushClicked();


private:
    Ui::ShootingMap *ui;
    QMap<QString, QLineEdit*> zoneInputs;

    struct stats {
        std::string name;
        int m = 0;
        int att = 0;
    };

    struct ShotEntry {
        std::string zoneName;
        int m;
        int att;
    };

    std::vector<std::vector<stats>> court;
    std::stack<ShotEntry> hist;

    void setupZoneInputs();
    stats* find(const std::string& loc);
    void record(const std::string& loc, int m, int att);
    std::string undo();
    QString display();
    QString total();
    QString tot_points();
    void initializeCourt();
    void processZoneInput(const QString& zoneName, QLineEdit* inputField);
};

#endif // SHOOTINGMAP_H
