#ifndef HUCHART_H
#define HUCHART_H

#include <QWidget>

namespace Ui {
class HUChart;
}

class HUChart : public QWidget
{
    Q_OBJECT

public:
    explicit HUChart(QWidget *parent = nullptr);
    ~HUChart();

private:
    Ui::HUChart *ui;
};

#endif // HUCHART_H
