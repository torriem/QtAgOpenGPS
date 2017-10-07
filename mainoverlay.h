#ifndef MAINOVERLAY_H
#define MAINOVERLAY_H

#include <QWidget>

namespace Ui {
class MainOverlay;
}

class MainOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit MainOverlay(QWidget *parent = 0);
    ~MainOverlay();

private:
    Ui::MainOverlay *ui;
};

#endif // MAINOVERLAY_H
